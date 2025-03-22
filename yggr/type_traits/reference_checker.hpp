//reference_checker.hpp

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

#ifndef __YGGR_MPLEX_REFERENCE_CHECK_HPP__
#define __YGGR_MPLEX_REFERENCE_CHECK_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/ref.hpp>

namespace yggr
{

namespace detail
{

template<typename T>
struct _is_reference
	: public
		boost::mpl::bool_
		<
			( boost::is_reference_wrapper<T>::value
				|| boost::is_reference<T>::value)
		>
{
};

} // namespace detail

template<typename T>
struct is_reference
	: public detail::_is_reference<T>
{
};

template<typename T>
struct is_nude_reference
	: public
		boost::mpl::bool_
		<
			boost::is_reference<T>::value
		>
{
};

} // namespace yggr

#endif //__YGGR_MPLEX_REFERENCE_CHECK_HPP__