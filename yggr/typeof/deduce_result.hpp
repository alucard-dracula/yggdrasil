// deduce_result.hpp

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

#ifndef __YGGR_TYPEOF_DEDUCE_RESULT_HPP__
#define __YGGR_TYPEOF_DEDUCE_RESULT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/typeof/typeof.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{

// BOOST typeof does not recognize the void type return value, 
// so use the deduce_result function to convert void to void_

/*
* example: typedef YGGR_TYPEOF((yggr::deduce_result(boost::bind(&you_foo, params,)))) result_type
*/

template<typename Handler>
typename 
	boost::mpl::if_
	<
		typename
			boost::is_same
			<
				typename func::foo_t_info<Handler>::result_type,
				void
			>::type,
		boost::mpl::void_,
		typename func::foo_t_info<Handler>::result_type
	>::type
	deduce_result(const Handler&);

} // namespace yggr

/*
*/

#endif // __YGGR_TYPEOF_DEDUCE_RESULT_HPP__
