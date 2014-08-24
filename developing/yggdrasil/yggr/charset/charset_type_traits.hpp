//charset_type_traits.hpp

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

#ifndef __YGGR_CHARSET_CHARSET_TYPE_TRAITS_HPP__
#define __YGGR_CHARSET_CHARSET_TYPE_TRAITS_HPP__

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{
namespace charset
{

template<typename T>
struct is_utf8_string
{
	typedef boost::mpl::false_ type;
};

template<typename T>
struct is_not_utf8_string
{
	typedef typename boost::mpl::if_<
					typename is_utf8_string<T>::type,
					boost::mpl::false_,
					boost::mpl::true_
				>::type type;
};

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_CHARSET_TYPE_TRAITS_HPP__