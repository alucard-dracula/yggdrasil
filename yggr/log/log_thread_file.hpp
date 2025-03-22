// log_thread_file.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

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

#ifndef __YGGR_LOG_LOGOP_LOG_THREAD_FILE_HPP__
#define __YGGR_LOG_LOGOP_LOG_THREAD_FILE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/tuple/tuple.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/time/time.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <yggr/log/log_change_checker.hpp>
#include <yggr/log/log_data_property_getter.hpp>
#include <yggr/log/detail/stream_deleter.hpp>

#include <boost/thread/mutex.hpp>

#include <fstream>
#include <sstream>
#include <utility>

namespace yggr
{
namespace log
{
namespace log_op
{
template<typename Stream, typename Thread_ID,
			typename Log_Change_Checker = log_file_day_checker>
class log_thread_file
	: private nonable::noncopyable,
		private nonable::nonmoveable
{

public:
#ifdef YGGR_AT_WINDOWS
	typedef yggr::string path_string_type;
#else
	typedef utf8_string path_string_type;
#endif // YGGR_AT_WINDOWS

	typedef Stream stream_type;
	typedef utf8_string inner_string_type;

	typedef path_string_type init_type;

private:
	typedef Thread_ID trd_id_type;
	typedef Log_Change_Checker change_checker_type;

	typedef time::time time_type;

	typedef file_system::local_fsys_operators local_fsys_operators_type;
	typedef local_fsys_operators_type::path_type path_type;

private:
	typedef safe_container::safe_wrap<path_string_type, boost::shared_mutex> path_string_wrap_type;
	typedef safe_container::safe_wrap<path_type, boost::shared_mutex> path_obj_wrap_type;
	typedef safe_container::safe_wrap<time_type> time_wrap_type;
	typedef safe_container::safe_wrap<stream_type> stream_wrap_type;

	typedef ::yggr::shared_ptr<stream_wrap_type> stream_wrap_ptr_type;

	typedef safe_container::safe_unordered_map<trd_id_type, stream_wrap_ptr_type> stream_map_type;

private:
	typedef detail::stream_deleter<stream_type> stream_deleter_type;

private:
	typedef log_thread_file this_type;

public:

	log_thread_file(void)
		: _fname(charset::string_converter::s_conv<path_string_type>(this_type::unknown_log_name()))
	{
	}

	template<typename Char>
	log_thread_file(const Char* fname)
	{
		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);

		path_string_type str_fpath;

		if(local_fsys_operators_type::parsing_file_path(str_fname, str_fpath))
		{
			_fname.store(str_fname);
			_fpath.store(local_fsys_operators_type::make_path_object(str_fpath));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	log_thread_file(const Basic_String<Char, Traits, Alloc>& fname)
	{
		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);

		path_string_type str_fpath;

		if(local_fsys_operators_type::parsing_file_path(str_fname, str_fpath))
		{
			_fname.store(str_fname);
			_fpath.store(local_fsys_operators_type::make_path_object(str_fpath));
		}
	}

	log_thread_file(const path_string_type& fname)
	{
		path_string_type str_fpath;

		if(local_fsys_operators_type::parsing_file_path(fname, str_fpath))
		{
			_fname.store(fname);
			_fpath.store(local_fsys_operators_type::make_path_object(str_fpath));
		}
	}

	~log_thread_file(void)
	{
		_stream_map.clear();
	}

	path_string_type name(void) const
	{
		path_string_type ret;

		try
		{
			ret = _fname.load();
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return path_string_type();
		}
		return ret;
	}

	template<typename String>
	String name(void) const
	{
		typedef String now_string_type;

		path_string_type ret;

		try
		{
			ret = _fname.load();
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return path_string_type();
		}

		return charset::string_converter::s_conv<now_string_type>(ret);
	}

	template<typename Value>
	bool append(const Value& val, bool bclear = false)
	{
		typedef Value now_val_type;
		typedef log_data_thread_id_getter<trd_id_type, now_val_type> tid_getter_type;
		typedef typename stream_map_type::base_type now_map_type;

		path_string_type str_fname;
		path_type str_fpath;
		try
		{
			str_fname = _fname.load();
			str_fpath = _fpath.load();
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return false;
		}

		if(!prv_connect(str_fname, str_fpath))
		{
			return false;
		}

		std::pair<path_string_type, bool> chk_rst = prv_check_and_mending_date();

		if(chk_rst.first.empty())
		{
			return false;
		}

		if(chk_rst.second)
		{
			now_map_type tmp;
			_stream_map.swap(tmp);
		}

		tid_getter_type getter;
		trd_id_type tid = getter(val);

		stream_wrap_ptr_type pwrap_stream
			= _stream_map.find(
				tid,
				boost::bind(
					&this_type::prv_handler_get_stream_wrap,
					this, _1, _2, boost::cref(tid)));

		if(!pwrap_stream)
		{
			return false;
		}

		path_string_type real_fname;
		{
			std::stringstream ss;
			ss << str_fname << chk_rst.first << this_type::log_file_tag() << std::hex << tid;
			charset::string_converter::s_conv(real_fname, ss.str());
		}

		if(real_fname.empty())
		{
			return false;
		}

		bool bright = false;
		try
		{
			bright =
				pwrap_stream->using_handler(
					boost::bind(
						&this_type::prv_handler_append<now_val_type>,
						this, _1, boost::cref(val), boost::cref(real_fname), bclear));
		}
		catch(const typename stream_wrap_type::error_type&)
		{
			bright = false;
		}

		return bright;
	}

	template<typename Value>
	bool append_all(const Value& val, bool bclear = false)
	{
		return this_type::append(val, bclear);
	}

	bool clear(const trd_id_type& tid)
	{
		typedef typename stream_map_type::base_type now_map_type;

		path_string_type str_fname;
		path_type str_fpath;
		try
		{
			str_fname = _fname.load();
			str_fpath = _fpath.load();
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return false;
		}

		if(!prv_connect(str_fname, str_fpath))
		{
			return false;
		}

		std::pair<path_string_type, bool> chk_rst = prv_check_and_mending_date();

		if(chk_rst.first.empty())
		{
			return false;
		}

		if(chk_rst.second)
		{
			now_map_type tmp;
			_stream_map.swap(tmp);
			return true;
		}

		stream_wrap_ptr_type pwrap_stream
			= _stream_map.find(
				tid,
				boost::bind(
					&this_type::prv_handler_get_stream_wrap,
					this, _1, _2, boost::cref(tid)));

		if(!pwrap_stream)
		{
			return false;
		}

		path_string_type real_fname;
		{
			std::stringstream ss;
			ss << str_fname << chk_rst.first << this_type::log_file_tag() << std::hex << tid;
			charset::string_converter::s_conv(real_fname, ss.str());
		}

		if(real_fname.empty())
		{
			return false;
		}

		bool bright = false;
		try
		{
			bright =
				pwrap_stream->using_handler(
					boost::bind(
						&this_type::prv_handler_clear,
						this, _1, boost::cref(real_fname)));
		}
		catch(const typename stream_wrap_type::error_type&)
		{
			return false;
		}

		return bright;
	}

	inline bool clear_all(void)
	{
		assert(false);
		return false;
	}

private:
	std::pair<path_string_type, bool> prv_check_and_mending_date(void)
	{
		typedef std::pair<path_string_type, bool> ret_type;

		std::pair<time_type, u32> rst
			= _time.locked_unsafe_using_handler(
						boost::bind(&this_type::prv_handler_check_and_mending_date, this, _1, time_type()));

		return ret_type(
					(rst.first == time_type::zero_value()?
						path_string_type()
						: rst.first.to_local_string<path_string_type>(change_checker_type::format()) ),
					rst.second );

	}

	std::pair<time_type, bool>
		prv_handler_check_and_mending_date(time_wrap_type::base_type& base,
											const time_type& tm)
	{
		typedef std::pair<time_type, bool> ret_type;
		if(!base)
		{
			time_wrap_type::base_type tmp = construct_shared<time_type>(yggr_nothrow_new time_type(tm));
			if(!tmp)
			{
				return ret_type(time_type::zero_value(), false); // not clear and return false
			}
			else
			{
				base.swap(tmp);
				return ret_type(tm, true); // clear
			}
		}
		else
		{
			change_checker_type checker;
			if(checker(*base))
			{
				*base = tm;
				return ret_type(tm, true); // clear
			}
			else
			{
				return ret_type(*base, false); // not clear
			}
		}
	}

	stream_wrap_ptr_type prv_handler_get_stream_wrap(typename stream_map_type::base_type& base,
														typename stream_map_type::iterator rst,
														const trd_id_type& tid)
	{
		typedef typename stream_map_type::iterator iter_type;

		if(rst == base.end())
		{
			std::pair<iter_type, bool>
				ins_rst = base.insert(
					typename stream_map_type::value_type(
						tid, construct_shared<stream_wrap_type>(
								yggr_nothrow_new stream_wrap_type(
									yggr_nothrow_new stream_type(), stream_deleter_type()))) );
			return ins_rst.second? ins_rst.first->second : stream_wrap_ptr_type();
		}
		else
		{
			return rst->second;
		}
	}

	bool prv_connect(const path_string_type& fname, const path_type& fpath)
	{
		if(fname.empty())
		{
			return false;
		}
		else
		{
			if(fpath.empty())
			{
				return true;
			}
			else
			{
				try
				{
					return local_fsys_operators_type::create_directories(fpath);
				}
				catch(const boost::filesystem::filesystem_error&)
				{
					return false;
				}
				catch(const compatibility::stl_exception&)
				{
					return false;
				}
			}
		}
	}

	template<typename Value>
	bool prv_handler_append(typename stream_wrap_type::base_type& base,
								const Value& val,
								const path_string_type& fname,
								bool bclear)
	{
		assert(base);
		stream_type& stream = *base;

		if(bclear)
		{
			if(stream.is_open())
			{
				stream.close();
			}
		}

		if(!stream.is_open())
		{
			stream.open(fname.data(),
							std::ios::out
							| (bclear? static_cast<std::ios::openmode>(0) : std::ios::app));
		}

		if((!stream.good())
			|| (stream.rdstate() & std::ios::in))
		{
			stream.close();
			return false;
		}
		else
		{
			stream << val << std::endl;
			return true;
		}
	}

	bool prv_handler_clear(typename stream_wrap_type::base_type& base,
								const path_string_type& fname)
	{
		assert(base);
		stream_type& stream = *base;

		if(stream.is_open())
		{
			stream.close();
		}

		stream.open(fname.data(), std::ios::out);

		if((!stream.good())
			|| (stream.rdstate() & std::ios::in))
		{
			stream.close();
			return false;
		}
		else
		{
			return true;
		}
	}

public:
	inline static const char* unknown_log_name(void)
	{
		return "unknown_log";
	}

private:
	inline static const char* log_file_tag(void)
	{
		return ".thread-0x";
	}

private:
	path_string_wrap_type _fname;
	path_obj_wrap_type _fpath;
	time_wrap_type _time;
	stream_map_type _stream_map;
};

// non_member_foo
template<typename Stream, typename Thread_ID, typename Log_Change_Checker, typename Char>
typename boost::enable_if< is_native_char<Char>, bool >::type
	operator==(const Char* l, const log_thread_file<Stream, Thread_ID, Log_Change_Checker>& r)
{
	typedef charset::basic_string<Char> now_string_type;

	return now_string_type(l) == r.template name<now_string_type>();
}

template<typename Stream, typename Thread_ID, typename Log_Change_Checker, typename Char>
typename boost::enable_if< is_native_char<Char>, bool >::type
	operator==(const log_thread_file<Stream, Thread_ID, Log_Change_Checker>& l, const Char* r)
{
	typedef charset::basic_string<Char> now_string_type;

	return now_string_type(r) == l.template name<now_string_type>();
}

template<typename Stream, typename Thread_ID, typename Log_Change_Checker,
			typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator==(const Basic_String<Char, Traits, Alloc>& l,
					const log_thread_file<Stream, Thread_ID, Log_Change_Checker>& r)
{
	typedef Basic_String<Char, Traits, Alloc> now_string_type;

	return l == r.template name<now_string_type>();
}

template<typename Stream, typename Thread_ID, typename Log_Change_Checker,
			typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
typename boost::enable_if< charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool >::type
	operator==(const log_thread_file<Stream, Thread_ID, Log_Change_Checker>& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	typedef Basic_String<Char, Traits, Alloc> now_string_type;

	return r == l.template name<now_string_type>();
}

} // namespace log_op
} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_THREAD_FILE_HPP__
