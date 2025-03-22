// log_file.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_FILE_HPP__
#define __YGGR_LOG_LOGOP_LOG_FILE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/time/time.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/log/log_change_checker.hpp>
#include <yggr/log/detail/stream_deleter.hpp>

#ifdef YGGR_AT_WINDOWS
#	include<yggr/charset/make_string_t_from_allocator.hpp>
#else
#	include<yggr/charset/make_utf8_string_t_from_allocator.hpp>
#endif // YGGR_AT_WINDOWS

#include <boost/utility/enable_if.hpp>
#include <boost/thread/mutex.hpp>

#include <fstream>

namespace yggr
{
namespace log
{
namespace log_op
{

template<typename Stream,
			typename Log_Change_Checker = log_file_day_checker>
class log_file
{

public:
#ifdef YGGR_AT_WINDOWS
	typedef yggr::string path_string_type;
#else
	typedef yggr::utf8_string path_string_type;
#endif // YGGR_AT_WINDOWS

	typedef Stream stream_type;
	typedef yggr::utf8_string inner_string_type;

	typedef path_string_type init_type;

private:
	typedef Log_Change_Checker change_checker_type;
	typedef time::time time_type;
	typedef file_system::local_fsys_operators local_fsys_operators_type;
	typedef local_fsys_operators_type::path_type path_type;

private:
	typedef detail::stream_deleter<stream_type> stream_deleter_type;

private:
	typedef safe_container::safe_wrap<path_string_type, boost::shared_mutex> path_string_wrap_type;
	typedef safe_container::safe_wrap<path_type, boost::shared_mutex> path_obj_wrap_type;
	typedef safe_container::safe_wrap<time_type> time_wrap_type;
	typedef safe_container::safe_wrap<stream_type> stream_wrap_type;

private:
	typedef log_file this_type;

public:

	log_file(void)
		: _fname((charset::string_converter::s_conv<path_string_type>(this_type::unknown_log_name()))),
			_stream(yggr_nothrow_new stream_type(), stream_deleter_type())
	{
	}

	template<typename Char>
	log_file(const Char* fname)
		: _stream(yggr_nothrow_new stream_type(), stream_deleter_type())
	{
		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);

		path_string_type str_fpath;

		if(local_fsys_operators_type::parsing_file_path(str_fname, str_fpath))
		{
			_fname.store(fname);
			_fpath.store(local_fsys_operators_type::make_path_object(str_fpath));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	log_file(const Basic_String<Char, Traits, Alloc>& fname)
		: _stream(yggr_nothrow_new stream_type(), stream_deleter_type())
	{
		path_string_type str_fname;
		charset::string_converter::s_conv(str_fname, fname);

		path_string_type str_fpath;

		if(local_fsys_operators_type::parsing_file_path(str_fname, str_fpath))
		{
			_fname.store(fname);
			_fpath.store(local_fsys_operators_type::make_path_object(str_fpath));
		}
	}

	log_file(const path_string_type& fname)
		: _stream(yggr_nothrow_new stream_type(), stream_deleter_type())
	{
		path_string_type str_fpath;

		if(local_fsys_operators_type::parsing_file_path(fname, str_fpath))
		{
			_fname.store(fname);
			_fpath.store(local_fsys_operators_type::make_path_object(str_fpath));
		}
	}

	~log_file(void)
	{
	}

public:

	inline path_string_type name(void) const
	{
		try
		{
			return path_string_type(_fname.load());
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return path_string_type();
		}
	}

	template<typename Alloc> inline
#ifdef YGGR_AT_WINDOWS
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			typename charset::make_string_t_from_allocator<Alloc>::type
		>::type
#else
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			typename charset::make_utf8_string_t_from_allocator<Alloc>::type
		>::type
#endif // YGGR_AT_WINDOWS
		name(const Alloc& alloc) const
	{
#ifdef YGGR_AT_WINDOWS
		typedef typename charset::make_string_t_from_allocator<Alloc>::type now_path_string_type;
#else
		typedef typename charset::make_utf8_string_t_from_allocator<Alloc>::type now_path_string_type;
#endif // YGGR_AT_WINDOWS
		try
		{
			return now_path_string_type(_fname.load(), alloc);
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return now_path_string_type(alloc);
		}
	}

	template<typename String> inline
	String name(const string& charset_name
						= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type)) const
	{
		typedef String ret_string_type;

		try
		{
#ifdef YGGR_AT_WINDOWS
            if(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(path_string_type) == charset_name)
            {
                 return charset::charset_foo::s_xchg<ret_string_type>(_fname.load());
            }
            else
            {
                return
                    charset::charset_foo::s_xchg_diff_charset<ret_string_type>(
                        _fname.load(),
                        YGGR_STR_STRING_DEFAULT_CHARSET_NAME(path_string_type),
                        charset_name);
            }
#elif (defined(__APPLE__) || defined(__APPLE_CC__))
            return _fname.load().template str<ret_string_type>(charset_name);
#else
			return _fname.load().str<ret_string_type>(charset_name);
#endif // YGGR_AT_WINDOWS
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return ret_string_type();
		}
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		name(const Alloc& alloc,
				const string& charset_name
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type)) const
	{
		typedef String ret_string_type;

		try
		{
#ifdef YGGR_AT_WINDOWS
            if(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(path_string_type) == charset_name)
            {
                 return charset::charset_foo::s_xchg<ret_string_type>(_fname.load(), alloc);
            }
            else
            {
                return
                    charset::charset_foo::s_xchg_diff_charset<ret_string_type>(
                        _fname.load(), alloc,
                        YGGR_STR_STRING_DEFAULT_CHARSET_NAME(path_string_type),
                        charset_name);
            }
#elif (defined(__APPLE__) || defined(__APPLE_CC__))
			return _fname.load().template str<ret_string_type>(alloc, charset_name);
#else
            return _fname.load().str<ret_string_type>(alloc, charset_name);
#endif // YGGR_AT_WINDOWS
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return ret_string_type();
		}
	}

	template<typename Value>
	bool append(const Value& val, bool bclear = false)
	{
		typedef Value value_type;

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

		path_string_type str_new_date = prv_check_and_mending_date(bclear);
		try
		{
			return
				_stream.using_handler(
					boost::bind(
						&this_type::prv_append<value_type>,
						this, _1, boost::cref(val), bclear,
						boost::cref(str_fname), boost::cref(str_fpath),
						boost::cref(str_new_date)));
		}
		catch(const typename stream_wrap_type::error_type&)
		{
			return false;
		}
	}

	template<typename Value> inline
	bool append_all(const Value& val, bool bclear = false)
	{
		return this_type::append(val, bclear);
	}

	bool clear(void)
	{
		path_string_type str_fname;
		try
		{
			str_fname = _fname.load();
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return false;
		}

		path_type str_fpath;
		try
		{
			str_fpath = _fpath.load();
		}
		catch(const path_string_wrap_type::error_type&)
		{
			return false;
		}

		path_string_type str_new_date = prv_check_and_mending_date(true);

		try
		{
			return
				_stream.using_handler(
					boost::bind(
						&this_type::prv_connect,
						this, _1, true,
						boost::cref(str_fname), boost::cref(str_fpath),
						boost::cref(str_new_date)));
		}
		catch(const typename stream_wrap_type::error_type&)
		{
			return false;
		}
	}

	inline bool clear_all(void)
	{
		return this_type::clear();
	}

private:

	inline path_string_type prv_check_and_mending_date(bool bclear)
	{
		std::pair<time_type, bool> rst
			= _time.locked_unsafe_using_handler(
				boost::bind(
					&this_type::prv_handler_check_and_mending_date,
					this, _1, time_type()));

		return
			(rst.first != time_type::zero_value()) && (bclear || rst.second)?
				rst.first.to_local_string<path_string_type>(change_checker_type::format())
				: path_string_type();
	}

	std::pair<time_type, bool>
		prv_handler_check_and_mending_date(time_wrap_type::base_type& base, const time_type& tm)
	{
		typedef std::pair<time_type, bool> ret_type;
		if(!base)
		{
			time_wrap_type::base_type tmp = construct_shared<time_type>(yggr_nothrow_new time_type());
			if(!tmp)
			{
				return ret_type(time_type::zero_value(), false); // not clear
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

	template<typename Value> inline
	bool prv_append(typename stream_wrap_type::base_type& base,
						const Value& val, bool bclear,
						const path_string_type& fname,
						const path_type& fpath,
						const path_string_type& new_date)
	{
		assert(base);
		stream_type& stream = *base;

		if(stream.rdstate() & std::ios::in)
		{
			return false;
		}

		if(!this_type::prv_connect(base, bclear, fname, fpath, new_date))
		{
			return false;
		}

		(*base) << val << std::endl;

		return true;
	}

	bool prv_connect(typename stream_wrap_type::base_type& base,
						bool bclear,
						const path_string_type& fname,
						const path_type& fpath,
						const path_string_type& new_date)
	{
		assert(base);

		if(fname.empty())
		{
			return false;
		}

		if(!fpath.empty())
		{
			bool bright = false;
			try
			{
				bright = local_fsys_operators_type::create_directories(
							fpath,
							local_fsys_operators_type::options_type::dir_success_if_exists);
			}
			catch(const local_fsys_operators_type::exception_type&)
			{
				return false;
			}
			catch(const compatibility::stl_exception&)
			{
				return false;
			}

			if(!bright)
			{
				return false;
			}
		}

		stream_type& stream = *base;
		if(!new_date.empty() || bclear)
		{
			if(stream.is_open())
			{
				stream.close();
			}
		}

		if(!stream.is_open())
		{
			stream.open((fname + new_date).data(),
                            std::ios::out
							 | (bclear? static_cast<std::ios::openmode>(0) : std::ios::app));
		}

		return stream.good();
	}

public:
	inline static const char* unknown_log_name(void)
	{
		return "unknown_log";
	}

private:
	path_string_wrap_type _fname;
	path_obj_wrap_type _fpath;
	time_wrap_type _time;
	stream_wrap_type _stream;
};

// non_member_foo

template<typename Stream, typename Log_Change_Checker, typename Char> inline
typename boost::enable_if<yggr::is_native_char< Char >, bool>::type
	operator==(const Char* l, const log_file<Stream, Log_Change_Checker>& r)
{
	typedef charset::basic_string<Char> now_string_type;

	return now_string_type(l) == r.template name<now_string_type>();
}

template<typename Stream, typename Log_Change_Checker, typename Char> inline
typename boost::enable_if<yggr::is_native_char< Char >, bool>::type
	operator==(const log_file<Stream, Log_Change_Checker>& l, const Char* r)
{
	typedef charset::basic_string<Char> now_string_type;

	return now_string_type(r) == l.template name<now_string_type>();
}

template<typename Stream, typename Log_Change_Checker,
			typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename boost::enable_if<yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator==(const Basic_String<Char, Traits, Alloc>& l,
					const log_file<Stream, Log_Change_Checker>& r)
{
	typedef Basic_String<Char, Traits, Alloc> now_string_type;

	return l == r.template name<now_string_type>();
}

template<typename Stream, typename Log_Change_Checker,
			typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename boost::enable_if<yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, bool>::type
	operator==(const log_file<Stream, Log_Change_Checker>& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	typedef Basic_String<Char, Traits, Alloc> now_string_type;

	return r == l.template name<now_string_type>();
}

} // namespace log_op
} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_FILE_HPP__
