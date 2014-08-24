// string.hpp

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

#ifndef __YGGR_CHARSET_STRING_HPP__
#define __YGGR_CHARSET_STRING_HPP__

#include <yggr/base/yggrdef.h>
#include <string>
#include <boost/container/string.hpp>
#include <yggr/charset/charset_type_traits.hpp>

#if !defined(YGGR_USE_STL_STRING) && !defined(YGGR_USE_BOOST_STRING)
#	define YGGR_USE_BOOST_STRING
#endif //YGGR_USE_STL_STRING

namespace yggr
{
namespace charset
{

#if defined YGGR_USE_BOOST_STRING
	using boost::container::basic_string;
	using boost::container::string;
	using boost::container::wstring;
#elif defined YGGR_USE_STL_STRING
	using std::basic_string;
	using std::string;
	using std::wstring;
#else
#	error "use string type unknow error. please check source"
#endif //YGGR_USE_BOOST_STRING

} // namespace charset

typedef charset::string string;
typedef charset::wstring wstring;

// type_traits
template<typename T>
struct other_string;

template<typename Char, typename Traits, typename Alloc>
struct other_string< std::basic_string<Char, Traits, Alloc> >
{
	typedef boost::container::basic_string<Char, Traits, Alloc> type;
};

template<typename Char, typename Traits, typename Alloc>
struct other_string< boost::container::basic_string<Char, Traits, Alloc> >
{
	typedef std::basic_string<Char, Traits, Alloc> type;
};


} // namesapce yggr

#endif // __YGGR_CHARSET_STRING_HPP__