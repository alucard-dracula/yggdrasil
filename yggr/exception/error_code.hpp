//error_code.hpp

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

#ifndef __YGGR_EXCEPTION_ERROR_CODE_HPP__
#define __YGGR_EXCEPTION_ERROR_CODE_HPP__

#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/move/move.hpp>

#include <yggr/type_traits/native_t.hpp>
#include <yggr/ppex/swap_this_def.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/make_utf8_string_t_from_allocator.hpp>

#include <yggr/time/time_config.hpp>
#include <yggr/time/time_basic_foo.hpp>
#include <yggr/time/time_formatting.hpp>

#include <sstream>

namespace yggr
{
namespace exception
{

class error_code
{
private:
	typedef thread::boost_thread_config_type thread_config_type;
	typedef std::stringstream ss_type;

public:
	typedef thread_config_type::this_thread_type this_thread_type;
	typedef charset::utf8_string err_string_type;
	typedef thread_config_type::id_type trd_id_type;

private:
	typedef error_code this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	error_code(void);

	template<typename Char>
	error_code(u32 code, const Char* msg)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
			_trd_id(0),
			_code(code),
			_msg(msg)
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << this_thread_type::id();
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	template<typename Char>
	error_code(u32 code, const Char* msg, const string& src_charset_name)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
			_trd_id(0),
			_code(code),
			_msg(msg, src_charset_name)
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << this_thread_type::id();
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	error_code(u32 code, const Basic_String<Char, Traits, Alloc>& msg)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
				_trd_id(0),
				_code(code),
				_msg(msg)
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << this_thread_type::id();
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	error_code(u32 code,
				const Basic_String<Char, Traits, Alloc>& msg,
				const string& src_charset_name)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
				_trd_id(0),
				_code(code),
				_msg(msg, src_charset_name)
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << this_thread_type::id();
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	error_code(u32 code, BOOST_RV_REF(err_string_type) msg)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
			_trd_id(0),
			_code(code),
			_msg(boost::move(msg))
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << this_thread_type::id();
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	error_code(u32 code, const err_string_type& msg);

	template<typename Char>
	error_code(u32 code, const trd_id_type& tid, const Char* msg)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
			_trd_id(0),
			_code(code),
			_msg(msg)
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << tid;
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	template<typename Char>
	error_code(u32 code, const trd_id_type& tid,
				const Char* msg, const string& src_charset_name)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
			_trd_id(0),
			_code(code),
			_msg(msg, src_charset_name)
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << tid;
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	error_code(u32 code, const trd_id_type& tid, const Basic_String<Char, Traits, Alloc>& msg)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
				_trd_id(0),
				_code(code),
				_msg(msg)
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << tid;
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	error_code(u32 code, const trd_id_type& tid,
				const Basic_String<Char, Traits, Alloc>& msg,
				const string& src_charset_name)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
				_trd_id(0),
				_code(code),
				_msg(msg, src_charset_name)
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << tid;
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	error_code(u32 code, const trd_id_type& tid, BOOST_RV_REF(err_string_type) msg)
		: _tm(time::calendar_time_to_utc_time(std::time(0))),
			_trd_id(0),
			_code(code),
			_msg(boost::move(msg))
	{
		u64 tmp_tid = 0;
		ss_type ss;
		ss << tid;
		ss >> std::hex >> tmp_tid;

		_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
	}

	error_code(u32 code, const trd_id_type& tid, const err_string_type& msg);

	error_code(BOOST_RV_REF(this_type) right)
		: _tm(boost::move(right._tm)),
			_trd_id(boost::move(right._trd_id)),
			_code(boost::move(right._code)),
			_msg(boost::move(right._msg))
	{
	}

	error_code(const this_type& right);
	~error_code(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_tm, boost::move(right._tm));
		copy_or_move_or_swap(_trd_id, boost::move(right._trd_id));
		copy_or_move_or_swap(_code, boost::move(right._code));
		copy_or_move_or_swap(_msg, boost::move(right._msg));

		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline s64& time(void)
	{
		return _tm;
	}

	inline s64 time(void) const
	{
		return _tm;
	}

	inline s64& thread_id(void)
	{
		return _trd_id;
	}

	inline s64 thread_id(void) const
	{
		return _trd_id;
	}

	inline u32& code(void)
	{
		return _code;
	}

	inline u32 code(void) const
	{
		return _code;
	}

	inline err_string_type& message(void)
	{
		return _msg;
	}

	inline const err_string_type& message(void) const
	{
		return _msg;
	}

protected:
	inline err_string_type pro_str_time(void) const
	{
		boost::xtime xtm =
		{
			static_cast<boost::xtime::xtime_sec_t>(_tm)
				/ time::time_ratio_def::E_S_AND_MS_RATIO,

			static_cast<boost::xtime::xtime_nsec_t>(_tm)
				% time::time_ratio_def::E_S_AND_MS_RATIO
				* time::time_ratio_def::E_MS_AND_NS_RATIO
		};

		err_string_type err_str;

#ifndef YGGR_ERROR_CODE_OUTPUT_USING_UTC_TIME
		time::time_formatting_utc(err_str, time::time_format::tm_fmt_iso8601(), xtm);
#else
		time::time_formatting_local(err_str, time::time_format::tm_fmt_iso8601(), xtm);
#endif // YGGR_ERROR_CODE_OUTPUT_USING_UTC_TIME

		return err_str;
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			typename charset::make_utf8_string_t_from_allocator<Alloc>::type
		>::type
		pro_str_time(const Alloc& alloc) const
	{
		typedef typename charset::make_utf8_string_t_from_allocator<Alloc>::type now_err_string_type;

		boost::xtime xtm =
		{
			static_cast<boost::xtime::xtime_sec_t>(_tm)
				/ time::time_ratio_def::E_S_AND_MS_RATIO,

			static_cast<boost::xtime::xtime_nsec_t>(_tm)
				% time::time_ratio_def::E_S_AND_MS_RATIO
				* time::time_ratio_def::E_MS_AND_NS_RATIO
		};

		err_string_type err_str(alloc);

#ifndef YGGR_ERROR_CODE_OUTPUT_USING_UTC_TIME
		time::time_formatting_utc(err_str, time::time_format::tm_fmt_iso8601(), xtm);
#else
		time::time_formatting_local(err_str, time::time_format::tm_fmt_iso8601(), xtm);
#endif // YGGR_ERROR_CODE_OUTPUT_USING_UTC_TIME
		return err_str;
	}

public:
	inline err_string_type str_time(void) const
	{
		return this_type::pro_str_time();
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			typename charset::make_utf8_string_t_from_allocator<Alloc>::type
		>::type
		str_time(const Alloc& alloc) const
	{
		return this_type::pro_str_time(alloc);
	}

	template<typename String> inline
	String str_time(const string& charset_name
						= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type)) const
	{
		typedef String ret_string_type;

		return this_type::pro_str_time().str<ret_string_type>(charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		str_time(const Alloc& alloc,
					const string& charset_name
						= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type)) const
	{
		typedef String ret_string_type;
		return this_type::pro_str_time().str<ret_string_type>(alloc, charset_name);
	}

protected:
	inline err_string_type pro_to_string(void) const
	{
		ss_type ss;
		ss << "tm = " << str_time()
			<< ", thread_id = 0x" << std::hex << _trd_id
			<< ", code = " << std::dec << code()
			<< ", msg = ";

		return err_string_type(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()) + _msg;
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			typename charset::make_utf8_string_t_from_allocator<Alloc>::type
		>::type
		pro_to_string(const Alloc& alloc) const
	{
		typedef typename charset::make_utf8_string_t_from_allocator<Alloc>::type now_err_string_type;
		ss_type ss;
		ss << "tm = " << str_time()
			<< ", thread_id = 0x" << std::hex << _trd_id
			<< ", code = " << std::dec << code()
			<< ", msg = ";

		return now_err_string_type(ss.str(), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()) + _msg;
	}

public:
	err_string_type to_string(const yggr::string& charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_to_string();
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			typename charset::make_utf8_string_t_from_allocator<Alloc>::type
		>::type
		to_string(const Alloc& alloc,
					const yggr::string& charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_to_string(alloc);
	}

	template<typename String> inline
	String to_string(const yggr::string& charset_name
						= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type)) const
	{
		typedef String ret_string_type;

		return this_type::pro_to_string().str<ret_string_type>(charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		to_string(const Alloc& alloc,
					const yggr::string& charset_name
						= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type)) const
	{
		typedef String ret_string_type;

		return this_type::pro_to_string().str<ret_string_type>(alloc, charset_name);
	}

protected: // log append need use extends
	s64 _tm;
	s64 _trd_id;
	u32 _code;
	err_string_type _msg;
};

// io

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const error_code& e)
{
	os << e.to_string();
	return os;
}

} // namespace exception
} // namespace yggr

namespace yggr
{
namespace exception
{
namespace swap_support
{

	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(error_code)

} // namespace swap_support

using swap_support::swap;

} // namespace exception
} // namespace yggr

namespace std
{
	using ::yggr::exception::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::exception::swap_support::swap;
} // namespace boost

#endif // __YGGR_EXCEPTION_ERROR_CODE_HPP__
