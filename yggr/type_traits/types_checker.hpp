//types_checker.hpp

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

#ifndef __YGGR_TYPE_TRAITS_TYPES_CHECKER_HPP__
#define __YGGR_TYPE_TRAITS_TYPES_CHECKER_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace yggr
{
namespace detail
{

template<typename T>
struct signed_value_checker_detail
	: public 
		boost::mpl::or_
		<
			boost::is_signed<T>,
			boost::is_floating_point<T>
		>
{
};

template<typename T>
struct unsigned_value_checker_detail
	: public boost::is_unsigned<T>
{
};

} // namespace detail
} // namespace yggr


#define YGGR_PP_SIGNED_CHECKER_TRUE_IMPL( __type__ ) \
	namespace yggr { namespace detail \
	template<> \
	struct signed_value_checker_detail< __type__ > \
		: public boost::mpl::true_ {}; } } \


//YGGR_PP_SIGNED_CHECKER_TRUE_IMPL(you_type)


namespace yggr
{

template<typename T>
struct signed_value_checker
	: public detail::signed_value_checker_detail<T>
{
};

} // namespace yggr

#endif //__YGGR_TYPE_TRAITS_TYPES_CHECKER_HPP__