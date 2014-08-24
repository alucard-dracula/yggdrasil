//default_charset_config.hpp

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

#ifndef __YGGR_CHARSET_DEFAULT_CHARSET_CONFIG_HPP__
#define __YGGR_CHARSET_DEFAULT_CHARSET_CONFIG_HPP__

#include <yggr/charset/string.hpp>
#include <yggr/nonable/nonable.hpp>


#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

#define IN_CHARSET_NAME "utf-8"
#define UTF8_CHARSET_NAME IN_CHARSET_NAME

#if !defined (YGGR_SYS_DEFAULT_STRING_CHARSET)
#		define YGGR_SYS_DEFAULT_STRING_CHARSET "gbk"
#endif // YGGR_SYS_DEFAULT_STRING_CHARSET

#if !defined (YGGR_SYS_DEFAULT_WSTRING_CHARSET)
#	if _LIBICONV_VERSION < 0x010D
#		define YGGR_SYS_DEFAULT_WSTRING_CHARSET "ucs-2le"
#	else
#		define YGGR_SYS_DEFAULT_WSTRING_CHARSET "wchar_t"
#	endif //  _LIBICONV_VERSION < 0x010D
#endif // YGGR_SYS_DEFAULT_WSTRING_CHARSET

namespace yggr
{
namespace charset
{

struct default_charset_config
{
private:
	static inline const char* prv_get_default_string_charset_name(const char* const)
	{
		return YGGR_SYS_DEFAULT_STRING_CHARSET;
	}

	static inline const char* prv_get_default_string_charset_name(const wchar_t* const)
	{
		return YGGR_SYS_DEFAULT_WSTRING_CHARSET;
	}

	static inline string prv_get_str_default_string_charset_name(const char* const)
	{
		return YGGR_SYS_DEFAULT_STRING_CHARSET;
	}

	static inline string prv_get_str_default_string_charset_name(const wchar_t* const)
	{
		return YGGR_SYS_DEFAULT_WSTRING_CHARSET;
	}

public:
	template<typename Char>
	static inline const char* get_default_string_charset_name(void)
	{
		typedef Char char_type;
		char_type* p = 0;
		return prv_get_default_string_charset_name(p);
	}

	template<typename Char>
	static inline const string get_str_default_string_charset_name(void)
	{
		typedef Char char_type;
		return prv_get_default_string_charset_name((const char_type*)0);
	}

};

template<typename Char>
struct charset_name_t
	: public basic_string<char>, private nonable::noncopyable, private nonable::nonmoveable
{
	typedef Char char_type;
	typedef basic_string<char> base_type;

	charset_name_t(void)
		: base_type(default_charset_config::get_default_string_charset_name<char_type>())
	{
	}

	~charset_name_t(void)
	{
	}

	operator const base_type&(void) const
	{
		return *this;
	}
};

} // namespace charset
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_DEFAULT_CHARSET_CONFIG_HPP__
