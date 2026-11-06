// caller_callable.hpp

/****************************************************************************
Copyright (c) 2010-2026 yggdrasil

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

#ifndef __YGGR_CALLER_DETAIL_CALLER_CALLABLE_HPP__
#define __YGGR_CALLER_DETAIL_CALLER_CALLABLE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/is_callable.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace yggr
{
namespace caller
{
namespace detail
{

template<typename Handler>
struct is_caller_callable_foo
	: public 
		boost::mpl::and_
		<
			typename yggr::func::foo_t_info<Handler>::is_callable_type,
			typename 
				boost::mpl::not_
				<
					typename
						boost::mpl::and_
						<
							typename yggr::func::foo_t_info<Handler>::is_native_foo_type,
							typename yggr::func::foo_t_info<Handler>::is_member_foo_type
						>::type
				>::type
		>::type
{
};

template<typename Handler>
struct is_caller_callable_native_mem_foo
	: public 
		boost::mpl::and_
		<
			typename yggr::func::foo_t_info<Handler>::is_callable_type,
			typename
				boost::mpl::and_
				<
					typename yggr::func::foo_t_info<Handler>::is_native_foo_type,
					typename yggr::func::foo_t_info<Handler>::is_member_foo_type
				>::type
			
		>::type
{
};

} // namespace detail
} // namespace caller
} // namespace yggr

#endif // __YGGR_CALLER_DETAIL_CALLER_CALLABLE_HPP__
