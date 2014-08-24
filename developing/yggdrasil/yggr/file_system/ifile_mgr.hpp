//ifile_mgr.hpp

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

#ifndef __YGGR_FILE_SYSTEM_FILE_INPUT_MANAGER_HPP__
#define __YGGR_FILE_SYSTEM_FILE_INPUT_MANAGER_HPP__

#include <list>

#include <boost/bind.hpp>
#include <boost/filesystem/fstream.hpp>
#include <yggr/file_system/base_file_mgr.hpp>
#include <yggr/time/time.hpp>



namespace yggr
{
namespace file_system
{

class ifile_mgr : public base_file_mgr<boost::filesystem::ifstream>
{
public:

#ifndef YGGR_SYSTEM_64
	enum
	{
		E_default_count_use_size = 31457280,
		E_compile_u64 = 0xffffffff

	};
#else
	enum
	{
		E_default_count_use_size = 31457280,
		E_compile_u64 = 0xffffffff

	};
#endif // YGGR_SYSTEM_64

	typedef base_file_mgr<boost::filesystem::ifstream> base_type;

	typedef base_type::buf_type buf_type;
	typedef base_type::file_size_type file_size_type;

	typedef base_type::file_runtime_info file_runtime_info_type;
	typedef base_type::get_file_result_type get_file_result_type;

	typedef base_type::file_list_type file_list_type;

	class file_runtime_info_container
	{

	public:
		explicit file_runtime_info_container(file_runtime_info_type& info)
			: info_(info)
		{
		}

		file_runtime_info_container(const file_runtime_info_container& right)
			: info_(right.info_)
		{
		}

		~file_runtime_info_container(void)
		{
		}

		bool operator <(const file_runtime_info_container& right) const
		{
			return info_ < right.info_;
		}

		file_runtime_info_type& info_;
	};

	typedef std::list<file_runtime_info_container> file_runtime_info_containers_type;

public:

	explicit ifile_mgr(const std::string& root_name, file_size_type count_use_size = E_default_count_use_size, //31457280 byte = 30MB
						bool auto_space_mend = false, size_type mend_num = 30) 
		:	_now_use_size(0), _count_use_size(count_use_size)
	{
		base_type::open(root_name.c_str(), std::ios::in | std::ios::binary);
		base_type::load();

		if(auto_space_mend)
		{
			set_auto_mend(mend_num);
		}
		
	}

	~ifile_mgr(void)
	{
		close();
	}

	bool get_file(const std::string& name, buf_type& buf)
	{
		get_file_result_type rst(base_type::get_file(name, buf, 
														boost::make_tuple(
																			boost::bind(&ifile_mgr::is_space_full, this, _1),
																			boost::bind(&ifile_mgr::fix_space, this, _1)
																		 )
													)
								);

		if(rst.first)
		{
			_file_runtine_info_containers.push_back(file_runtime_info_container(rst.second->second));

		}

		return !buf.empty();
	}


	void open(const std::string& root_name, file_size_type count_use_size = E_default_count_use_size, bool auto_space_mend = false, size_type mend_num = 30)
	{
		_now_use_size = 0;
		_count_use_size = count_use_size;
		base_type::open(root_name.c_str(), std::ios::in | std::ios::binary);
		base_type::load();

		if(auto_space_mend)
		{
			set_auto_mend(mend_num);
		}
	}

	void close(void)
	{
		_file_runtine_info_containers.clear();
		base_type::close();
	}
private:

	bool is_space_full(const file_runtime_info& info) const
	{
		return _now_use_size + base_type::file_size(info) > _count_use_size;
	}

	bool fix_space(const file_runtime_info& info)
	{
		file_size_type request_space = _now_use_size + base_type::file_size(info) - _count_use_size;

		if(request_space >= _count_use_size)
		{
			return false;
		}
		
		_file_runtine_info_containers.sort();

		file_size_type free_space_size = 0;
		for(; !_file_runtine_info_containers.empty();)
		{
			if(free_space_size <= request_space)
			{
				file_runtime_info_type& info = _file_runtine_info_containers.front().info_;
				free_space_size += base_type::file_size(info);
				base_type::clear_buf(info);
				_file_runtine_info_containers.pop_front();
				continue;
			}
			break;
		}
		
		return free_space_size >= request_space;
	}


private:
	void set_auto_mend(const file_size_type& mend_num)
	{
		file_size_type max_size = base_type::get_max_size_of_file();
		
		max_size *= mend_num;
		if(max_size > _count_use_size)
		{
			_count_use_size = max_size;
		}
	}

private:
	
	file_runtime_info_containers_type _file_runtine_info_containers;
	file_size_type _now_use_size;
	size_type _count_use_size;
};

} // namespace file_system
} //namesapce yggr
#endif //__YGGR_FILE_SYSTEM_FILE_INPUT_MANAGER_HPP__