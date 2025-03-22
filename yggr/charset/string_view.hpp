// string_view.hpp

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

#ifndef __YGGR_CHARSET_STRING_VIEW_HPP__
#define __YGGR_CHARSET_STRING_VIEW_HPP__

#include <yggr/charset/string_decl.hpp>
#include <yggr/charset/charset_foo.hpp>
#include <yggr/charset/string_view_impl.hpp>

namespace yggr
{
namespace charset
{

template <typename CharT_OS, typename Traits_OS, 
			typename CharT, typename Traits> inline
std::basic_ostream<CharT_OS, Traits_OS>&
	operator<<(std::basic_ostream<CharT_OS, Traits_OS>& os, 
				const basic_string_view<CharT, Traits>& s)
{
	typedef basic_string<CharT_OS, Traits_OS> target_string_type;

	target_string_type tmp;
	tmp.reserve(s.size());
	
	return 
		os << charset_foo::s_xchg(
				tmp, s.data(), s.size(),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(CharT),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(CharT_OS));
}

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

typedef basic_string_view<char> string_view;

#ifndef YGGR_NO_CWCHAR
typedef basic_string_view<wchar_t> wstring_view;
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
typedef basic_string_view<yggr::c8> u8string_view;
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
typedef basic_string_view<yggr::c16> u16string_view;
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
typedef basic_string_view<yggr::c32> u32string_view;
#endif // YGGR_NO_CHAR32_T


} // namespace charset

using charset::string_view;

#ifndef YGGR_NO_CWCHAR
using charset::wstring_view;
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
using charset::u8string_view;
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR16_T
using charset::u16string_view;
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
using charset::u32string_view;
#endif // YGGR_NO_CHAR32_T

} // namespace yggr

#endif // __YGGR_CHARSET_STRING_VIEW_HPP__
