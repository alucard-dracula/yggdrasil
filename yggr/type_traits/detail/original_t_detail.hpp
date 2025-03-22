//original_t_detail.hpp

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

#ifndef __YGGR_TYPE_TRAITS_ORIGINAL_T_DETAIL_HPP__
#define __YGGR_TYPE_TRAITS_ORIGINAL_T_DETAIL_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_volatile.hpp>

namespace yggr
{
namespace detail
{

template<typename T>
struct original_t_detail
{
private:
	typedef T t0_type;
	typedef typename boost::remove_volatile<t0_type>::type t1_type;
	typedef typename boost::remove_reference<t1_type>::type t2_type;
	typedef typename boost::remove_pointer<t2_type>::type t3_type;
	typedef typename boost::remove_const<t3_type>::type t4_type;
	typedef typename boost::remove_cv<t4_type>::type t5_type;

public:
	typedef t5_type type;
};

} // namespace detail
}// namespace yggr

#endif // __YGGR_TYPE_TRAITS_ORIGINAL_T_DETAIL_HPP__