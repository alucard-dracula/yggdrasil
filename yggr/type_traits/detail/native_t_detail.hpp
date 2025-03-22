//native_t_detail.hpp

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

#ifndef __YGGR_TYPE_TRAITS_NATIVE_T_DETAIL_HPP__
#define __YGGR_TYPE_TRAITS_NATIVE_T_DETAIL_HPP__

#include <yggr/type_traits/detail/original_t_detail.hpp>
#include <boost/type_traits/remove_all_extents.hpp>

namespace yggr
{
namespace detail
{

template<typename T>
struct native_t_detail
{
private:
	typedef T t0_type;
	typedef typename boost::remove_all_extents<t0_type>::type t1_type;
	typedef typename original_t_detail<t1_type>::type t2_type;

public:
	typedef t2_type type;
};

} // namespace detail
} // namespace yggr

#endif // __YGGR_TYPE_TRAITS_NATIVE_T_DETAIL_HPP__