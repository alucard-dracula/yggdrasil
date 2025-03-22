//pointer_checker_of_wrapper.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_TYPE_TRAITS_DETAIL_POINTER_CHECKER_OF_WRAPPER_HPP__
#define __YGGR_TYPE_TRAITS_DETAIL_POINTER_CHECKER_OF_WRAPPER_HPP__

#ifndef __YGGR_TYPE_TRAITS_POINTER_CHECK_HPP__
#error "please include yggr/type_traits/pointer_checker.hpp before this file!!!"
#endif // __YGGR_TYPE_TRAITS_POINTER_CHECK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/interface_ptr.hpp>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>


YGGR_PP_IS_POINTER_WRAPPER_SMART_POINTER_DEF(::yggr::shared_ptr)
YGGR_PP_IS_POINTER_WRAPPER_SMART_POINTER_DEF(boost::scoped_ptr)
YGGR_PP_IS_POINTER_WRAPPER_SMART_POINTER_DEF(boost::weak_ptr)
YGGR_PP_IS_POINTER_WRAPPER_SMART_POINTER_DEF(boost::intrusive_ptr)

namespace yggr
{
namespace type_traits
{
namespace detail
{

template<typename T, 
			template<typename _T> class SPtr>
struct _is_pointer_wrapper< yggr::interface_ptr<T, SPtr> >
	: public boost::mpl::true_
{
};

} // namespace detail
} // namespace type_traits
} // namespace yggr


#endif //__YGGR_TYPE_TRAITS_DETAIL_POINTER_CHECKER_OF_WRAPPER_HPP__