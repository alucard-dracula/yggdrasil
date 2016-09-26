//base_file_mgr.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_FILE_SYSTEM_BASE_FILE_MANAGER_HPP__
#define __YGGR_FILE_SYSTEM_BASE_FILE_MANAGER_HPP__

#include <memory>
#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>

#include <yggr/base/exception.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/base/error_make.hpp>

#include <yggr/file_system/file_object.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/time/time.hpp>
#include <yggr/file_system/basic_local_file_operator.hpp>

#include <yggr/serialization/vector.hpp>



namespace yggr
{
namespace file_system
{

template<typename File_Stream>
class base_file_mgr : private nonable::noncopyable
{
public:
	enum
	{
		E_File_Unknow = file_object::E_File_Unknow,
		//E_File_Network = file_object::E_File_Network,
		//E_File_Text = file_object::E_File_Text,
		E_File_Binary = file_object::E_File_Binary,
		//E_File_XML = file_object::E_File_XML,

		E_compile_File_Type_u32 = 0xffffffff
	};

protected:
	typedef base_file_mgr this_type;
	typedef packet::packet<archive::archive_partner::binary_iarchive_partner> i_info_pak_type;
	typedef packet::packet<archive::archive_partner::binary_oarchive_partner> o_info_pak_type;

	typedef File_Stream file_stream_type;
	typedef typename file_stream_type::open_mode open_mode_type;
	typedef file_object::file_size_type file_size_type;
	typedef file_object::file_type_id_type file_type_id_type;
	typedef file_object::buf_type buf_type;
	typedef file_check file_check_type;

	typedef std::vector<file_object> file_vector_type;
	typedef file_vector_type::iterator file_vector_iter_type;
	typedef file_vector_type::const_iterator file_vector_c_iter_type;

	typedef std::vector<std::string> file_list_type;

	class file_runtime_info
	{
	public:
		file_runtime_info(void)
		{
		}

		file_runtime_info(const file_size_type& idx)
			: _idx(idx), _use_count(0)
		{
		}

		file_runtime_info(const file_runtime_info& right)
			: _idx(right._idx), _use_time(right._use_time), _use_count(right._use_count)
		{
		}

		~file_runtime_info(void)
		{
		}

		operator const file_size_type(void) const
		{
			return _idx;
		}

		void use(void)
		{
			 boost::xtime_get(&_use_time, boost::TIME_UTC_);
			 ++_use_count;
		}

		void clear(void)
		{
			_use_time.reset();
			_use_count = 0;
		}

		bool operator <(const file_runtime_info& right) const
		{
			if(_use_time < right._use_time)
			{
				return true;
			}
			else
			{
				if(_use_time == right._use_time)
				{
					return _use_count < right._use_count;
				}
				else
				{
					return false;
				}
			}
		}

		file_size_type _idx;
		time::time _use_time;
		file_size_type _use_count;
	};

	typedef typename this_type::file_runtime_info file_runtime_info_type;

	typedef boost::unordered_map<std::string, file_runtime_info_type> file_hash_map_type;

	typedef typename file_hash_map_type::iterator file_hash_map_iter_type;
	typedef typename file_hash_map_type::const_iterator file_hash_map_c_iter_type;
	typedef typename file_hash_map_type::value_type file_hash_map_value_type;

	typedef std::pair<bool, file_hash_map_iter_type> get_file_result_type;

	template<typename T>
	struct array_deleter
	{
		void operator()(T* x) const
		{
			if(!x)
			{
				return;
			}

			delete []x;
		}
	};

private:

	ERROR_MAKER_BEGIN("base_file_mgr")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_5)
			ERROR_CODE_DEF(E_invalid_file)
			ERROR_CODE_DEF(E_invalid_fmt)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_5)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_file, "invalid file")
			ERROR_CODE_MSG(E_invalid_fmt, "invalid file format")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

public:
	file_size_type file_count(void) const
	{
		return _file_vector.size();
	}

	bool empty(void) const
	{
		return _file_vector.empty();
	}

	bool is_exisited(const std::string& fname) const
	{
		return _file_hash_map.find(fname) != _file_hash_map.end();
	}

	void get_file_list(file_list_type& list) const
	{
		list.clear();
		file_size_type isize = _file_vector.size();
		for(file_size_type i = 0; i != isize; ++i)
		{
			list.push_back(_file_vector[i].file_name());
		}
	}

protected:

	base_file_mgr(void)
		: _data_offset(0)
	{
	}

	base_file_mgr(const std::string& root_name, open_mode_type mode)
		: _data_offset(0)
	{
		fs.open(root_name.c_str(), mode);
	}

	~base_file_mgr(void)
	{
		close();
	}

	file_size_type file_size(file_size_type idx) const
	{
		return _file_vector[idx].size();
	}

	void clear_buf(file_size_type idx)
	{
		return _file_vector[idx].clear_buf();
	}

	file_size_type get_max_size_of_file(void) const
	{
		if(_file_vector.empty())
		{
			return file_size_type(0);
		}

		file_vector_c_iter_type iter = std::max_element(_file_vector.begin(), _file_vector.end());

		return iter->size();
	}

	bool add_file(const std::string& file_name, const file_type_id_type& type, const buf_type& buf)
	{
		std::string real_file_name(file_check_type::check_file_name(file_name));

		if(real_file_name.empty())
		{
			return false;
		}

		file_size_type spos = 0;

		if(!_file_vector.empty())
		{
			spos = _file_vector.back().next_seek_pos();
		}
		_file_vector.push_back(file_object(real_file_name, spos, type, buf));
		_file_hash_map.insert(file_hash_map_value_type(real_file_name, _file_vector.size() - 1));

		return true;
	}

	bool replace_file(const std::string& old_file_name, const std::string& new_file_name,
								const file_type_id_type& new_type, const buf_type& new_buf)
	{
		if(!is_exisited(old_file_name))
		{
			return false;
		}

		if(!add_file(new_file_name, new_type, new_buf))
		{
			return false;
		}

		if(!rm_file(old_file_name))
		{
			rm_file(new_file_name);
			return false;
		}

		return true;
	}

	bool rm_file(const std::string& file_name)
	{
		std::string real_file_name(file_check_type::check_file_name(file_name));
		if(real_file_name.empty())
		{
			return false;
		}

		file_hash_map_iter_type iter = _file_hash_map.find(real_file_name);
		if(iter == _file_hash_map.end())
		{
			return false;
		}

		file_vector_iter_type isize = _file_vector.end();

		file_size_type spos = (*(_file_vector.begin() + iter->second)).seek_pos();

		for(file_vector_iter_type i = _file_vector.begin() + iter->second + 1; i != isize; ++i)
		{
			(*i).set_seek_pos(spos);
			spos = (*i).next_seek_pos();
		}

		_file_vector.erase(_file_vector.begin() + iter->second);

		_file_hash_map.erase(iter);

		return true;
	}

	template<typename Tuple_Handler>
	get_file_result_type get_file(const std::string& name, buf_type& buf, const Tuple_Handler& handler)
	{
		bool b_load = false;
		file_hash_map_iter_type iter = _file_hash_map.find(name);
		if(iter == _file_hash_map.end())
		{
			return get_file_result_type(b_load, iter);
		}

		iter->second.use();
		if(_file_vector[iter->second].empty())
		{
			if(boost::get<0>(handler)(iter->second))
			{
				if(!boost::get<1>(handler)(iter->second))
				{
					return get_file_result_type(b_load, iter);
				}
			}
			_file_vector[iter->second].load(fs, _data_offset);
			b_load = true;
		}

		buf.resize(_file_vector[iter->second].size());
		memcpy(&(buf[0]), &((_file_vector[iter->second].buf())[0]), buf.size());
		return get_file_result_type(b_load, iter);
	}

	get_file_result_type get_file(const std::string& name, buf_type& buf)
	{
		bool b_load = false;
		file_hash_map_iter_type iter = _file_hash_map.find(name);
		if(iter == _file_hash_map.end())
		{
			return get_file_result_type(b_load, iter);
		}

		iter->second.use();
		if(_file_vector[iter->second].empty())
		{
			_file_vector[iter->second].load(fs, _data_offset);
			b_load = true;
		}

		buf.resize(_file_vector[iter->second].size());
		memcpy(&(buf[0]), &((_file_vector[iter->second].buf())[0]), buf.size());
		return get_file_result_type(b_load, iter);
	}

	void save(void)
	{
		if((!fs.good()) || (empty()))
		{
			return;
		}
		save_info();
		save_files();
	}

	void load(void)
	{
		if(!fs.good())
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_fmt, "load"));
			return;
		}

		try
		{
			load_info();
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	void open(const std::string& root_name, open_mode_type mode)
	{
		if(fs.is_open())
		{
			close();
		}

        _data_offset = 0;

#ifdef __GNUC__
		fs.open(root_name.c_str(), std::_Ios_Openmode(mode));
#endif //__GNUC__

#ifdef _MSC_VER
        fs.open(root_name.c_str(), mode);
#endif //_MSC_VER

	}

	//void clear(void)
	//{
	//	_file_hash_map.clear();
	//	_file_vector.clear();
	//	fs.close();
	//}

	void close(void)
	{
		_file_hash_map.clear();
		_file_vector.clear();

		if(fs.is_open())
		{
			fs.close();
		}
	}
private:

	void save_info(void)
	{
		file_size_type file_count = _file_vector.size();
		o_info_pak_type opak;
		opak.save(_file_vector);

		_data_offset = sizeof(file_size_type) + sizeof(file_size_type) + opak.size();

		try
		{
			fs.write(reinterpret_cast<char*>(&file_count), sizeof(file_size_type));
			fs.write(reinterpret_cast<char*>(&_data_offset), sizeof(file_size_type));
			fs.write(opak.val_buf(), opak.size());
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}
	}

	void save_files(void)
	{
		file_size_type isize = _file_vector.size();
		for(file_size_type i = 0; i != isize; ++i)
		{
			_file_vector[i].save(fs);
		}
	}

	bool load_info(void)
	{
		typedef array_deleter<i_info_pak_type::buf_val_type> array_deleter_type;

		file_size_type file_count = 0;

		try
		{
			fs.read(reinterpret_cast<char*>(&file_count), sizeof(file_size_type));

			fs.read(reinterpret_cast<char*>(&_data_offset), sizeof(file_size_type));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
		}

		file_size_type info_buf_size = _data_offset - sizeof(file_size_type) - sizeof(file_size_type);

		if(!file_count || !_data_offset || !info_buf_size)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_fmt, "load_info"));
			return false;
		}

		//std::auto_ptr<i_info_pak_type::buf_val_type> pt(new i_info_pak_type::buf_val_type[info_buf_size]);
		boost::shared_ptr<i_info_pak_type::buf_val_type> pt(new i_info_pak_type::buf_val_type[info_buf_size], array_deleter_type());

		fs.read(pt.get(), info_buf_size);

		i_info_pak_type::buf_type pak_buf;
		pak_buf.resize(info_buf_size);
		memcpy(&(pak_buf[0]), pt.get(), info_buf_size);

		_file_vector.resize(file_count);
		i_info_pak_type ipak(pak_buf);
		ipak.load(_file_vector);

		create_file_hash_map();

		return true;
	}


	void create_file_hash_map(void)
	{
		file_size_type file_size = _file_vector.size();
		for(file_size_type i = 0; i != file_size; ++i)
		{
			_file_hash_map.insert(file_hash_map_value_type(_file_vector[i].file_name(), i));
		}
	}

	void add_file(const file_object& obj)
	{
		_file_vector.push_back(obj);
		_file_hash_map.insert(file_hash_map_value_type(obj.file_name(), _file_vector.size() - 1));
	}

	/*std::string check_file_name(const std::string& name)
	{
		std::string str(name);

		std::replace(str.begin(), str.end(), '\\', '/');

		if( (str.find("//") != std::string::npos)
			|| (str.find(":") != std::string::npos)
			|| (str.find("*") != std::string::npos)
			|| (str.find("?") != std::string::npos)
			|| (str.find("\"") != std::string::npos)
			|| (str.find("<") != std::string::npos)
			|| (str.find(">") != std::string::npos)
			|| (str.find("|") != std::string::npos))
		{
			return std::string();
		}

		return str;
	}*/
private:
	file_vector_type _file_vector;
	file_hash_map_type _file_hash_map;
	file_stream_type fs;
	file_size_type _data_offset;
};

} // namespace file_system
} //namesapce yggr
#endif //__YGGR_FILE_SYSTEM_BASE_FILE_MANAGER_HPP__
