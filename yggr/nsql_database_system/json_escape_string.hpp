// json_escape_string.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_JSON_ESCAPE_STRING_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_JSON_ESCAPE_STRING_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/nonable/nonable.hpp>

#include <yggr/charset/charset_base_foo.hpp>
#include <yggr/charset/utf8_foo.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace nsql_database_system
{

struct json_escape_string
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef utf8_string_view string_view_type;

private:
	typedef json_escape_string this_type;

public:
	json_escape_string(void)
		:_str(0)
	{
	}

	json_escape_string(const char* str, std::size_t len = -1)
		: _str(str? bson_utf8_escape_for_json(str, len) : 0)
	{
	}

	~json_escape_string(void)
	{
		if(_str)
		{
			bson_free(_str);
		}
	}

public:
	inline void release(void)
	{
		if(_str)
		{
			bson_free(_str);
			_str = 0;
		}
	}

public:
	inline char* data(void)
	{
		return _str;
	}

	inline const char* data(void) const
	{
		return _str;
	}

	inline const char* c_str(void) const
	{
		return _str;
	}

	inline bool empty(void) const
	{
		return !_str;
	}

	inline std::size_t size(void) const
	{
		return charset::charset_base_foo::strlen(_str);
	}

	inline std::size_t length(void) const
	{
		return charset::utf8_foo::strlen(_str);
	}

public:
	inline string_view_type view(void) const
	{
		return string_view_type(_str);
	}

	inline string_view_type str(void) const
	{
		return string_view_type(_str);
	}

	template<typename String> inline
	String str(void) const
	{
		return this_type::view().str<String>();
	}

	template<typename String> inline
	String str(const string& ret_charset_name) const
	{
		return this_type::view().str<String>(ret_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		str(const Alloc& alloc) const
	{
		return this_type::view().str<String>(alloc);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		str(const Alloc& alloc, const string& ret_charset_name) const
	{
		return this_type::view().str<String>(alloc, ret_charset_name);
	}

private:
	char* _str;
};

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const json_escape_string& right)
{
	os << "\"" << (right.empty()? "" : right.c_str()) << "\"";
	return os;
}

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_JSON_ESCAPE_STRING_HPP__
