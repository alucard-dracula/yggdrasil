//database_string_helper.hpp

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

#ifndef __YGGR_DATABASE_SYSYEM_DATABASE_STRING_HELPER_HPP__
#define __YGGR_DATABASE_SYSYEM_DATABASE_STRING_HELPER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/database_system/database_charset_config.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/type_traits/native_t.hpp>
#include <yggr/ppex/concat.hpp>

#include <cassert>

namespace yggr
{
namespace database_system
{

class database_string_helper
{
public:
	typedef dtl::tstring org_string_type;
	typedef charset::utf8_string inner_string_type;

private:
	typedef database_string_helper this_type;

public:

	template<typename Char> inline
	static typename boost::enable_if<charset::is_convertible_char<Char>, org_string_type>::type
		s_conv(const Char* str, const string& charset_name
									= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<Char>::type))
	{
		return
			charset_name == YGGR_STR_DATABASE_CHARSET_NAME()?
				charset::string_converter::s_conv<org_string_type>(str)
				: charset::string_converter::s_conv<org_string_type>(str, charset_name, YGGR_STR_DATABASE_CHARSET_NAME());
	}

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define YGGR_TMP_STRING_TYPENAME() YGGR_PP_CONCAT(Char, Traits, Alloc)
#else
#	define YGGR_TMP_CONCAT_PARAMS Char YGGR_PP_SYMBOL_COMMA() Traits YGGR_PP_SYMBOL_COMMA() Alloc
#	define YGGR_TMP_STRING_TYPENAME() YGGR_PP_CONCAT(YGGR_TMP_CONCAT_PARAMS)
#endif //YGGR_NO_CXX11_VARIADIC_MACROS

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static typename boost::enable_if<is_native_char<Char>, org_string_type>::type
		s_conv(const Basic_String<Char, Traits, Alloc>& str,
				const string& charset_name
					= YGGR_STR_STRING_DEFAULT_CHARSET_NAME(
						typename native_t< Basic_String<YGGR_TMP_STRING_TYPENAME()> >::type) )
	{
		return
			charset_name == YGGR_STR_DATABASE_CHARSET_NAME()?
				charset::string_converter::s_conv<org_string_type>(str)
				: charset::string_converter::s_conv<org_string_type>(str, charset_name, YGGR_STR_DATABASE_CHARSET_NAME());
	}

	template<typename Char, typename Traits, typename Alloc> inline
	static org_string_type s_conv(const charset::utf8_string_impl<Char, Traits, Alloc>& str,
									const string& charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME())
	{
		assert(charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return str.template str<org_string_type>(YGGR_STR_DATABASE_CHARSET_NAME());
	}

	template<typename T1, typename T2> inline
	static org_string_type s_conv(const charset::string_charset_helper<T1, T2>& src)
	{
		return this_type::s_conv(src.first, src.second);
	}


#undef YGGR_TMP_STRING_TYPENAME

#ifndef __VAR_ARGS__
#undef YGGR_TMP_CONCAT_PARAMS
#endif // __VAR_ARGS__

};

} // namespace database_system
} // namespace yggr

#endif // __YGGR_DATABASE_SYSYEM_DATABASE_STRING_HELPER_HPP__
