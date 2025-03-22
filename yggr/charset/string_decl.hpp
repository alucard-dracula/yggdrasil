//string_decl.hpp

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

#ifndef __YGGR_CHARSET_STRING_DECL_HPP__
#define __YGGR_CHARSET_STRING_DECL_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/container/string.hpp>
#include <string>

#if !defined(YGGR_USE_STL_STRING) && !defined(YGGR_USE_BOOST_STRING)
#	define YGGR_USE_BOOST_STRING
#endif //YGGR_USE_STL_STRING

namespace boost
{
namespace container
{

#if !defined(YGGR_NO_CHAR8_T)
	typedef boost::container::basic_string<yggr::c8> u8string;
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	typedef boost::container::basic_string<yggr::c16> u16string;
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	typedef boost::container::basic_string<yggr::c32> u32string;
#endif // YGGR_NO_CHAR32_T

} // namespace container
} // namespace boost

namespace yggr
{
namespace charset
{

#if defined YGGR_USE_BOOST_STRING
	using boost::container::basic_string;
	using boost::container::string;

#	if !defined(YGGR_NO_CWCHAR)
	using boost::container::wstring;
#	endif // YGGR_NO_CWCHAR

#	if !defined(YGGR_NO_CHAR8_T)
	typedef boost::container::u8string u8string;
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	typedef boost::container::u16string u16string;
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	typedef boost::container::u32string u32string;
#	endif // YGGR_NO_CHAR32_T

#elif defined YGGR_USE_STL_STRING
	using std::basic_string;
	using std::string;

#	if !defined(YGGR_NO_CWCHAR)
	using std::wstring;
#	endif // YGGR_NO_CWCHAR

#	if !defined(YGGR_NO_CHAR8_T)
	using std::u8string;
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	using std::u16string;
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	using std::u32string;
#	endif // YGGR_NO_CHAR32_T

#else
#	error "use string type unknown error. please check source"
#endif //YGGR_USE_BOOST_STRING

} // namesapce charset

using charset::basic_string;
using charset::string;

#if !defined(YGGR_NO_CWCHAR)
using charset::wstring;
#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
using charset::u8string;
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
using charset::u16string;
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
using charset::u32string;
#endif // YGGR_NO_CHAR32_T

} // namespace yggr

#endif // __YGGR_CHARSET_STRING_DECL_HPP__
