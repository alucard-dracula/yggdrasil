// to_raw_pointer.hpp

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


#ifndef __YGGR_MOVE_DETAIL_TO_RAW_POINTER_HPP__
#define __YGGR_MOVE_DETAIL_TO_RAW_POINTER_HPP__

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/move/detail/workaround.hpp>

#include <yggr/type_traits/has_operator_pointer_member_access.hpp>
#include <yggr/typeof/typeof.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace move
{
namespace detail
{

template <typename T> YGGR_MOVE_FORCEINLINE
T* to_raw_pointer(T* p)
{ 
	return p; 
}

template<typename PtrElem> YGGR_MOVE_FORCEINLINE
typename
	boost::enable_if
	<
		has_operator_pointer_member_access_class<PtrElem>,
		YGGR_TYPEOF_TPL((PtrElem().operator->()))
	>::type
	to_raw_pointer(const PtrElem& p)
{
	return ::yggr::move::detail::to_raw_pointer(p.operator->());
}

} // namespace detail
} // namspace move
} // namespace yggr

#endif //__YGGR_MOVE_DETAIL_TO_RAW_POINTER_HPP__
