//pointer_to_value_t_of_wrapper.hpp

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

#ifndef __YGGR_MPLEX_DETAIL_POINTER_TO_VALUE_T_OF_WRAPPER_HPP__
#define __YGGR_MPLEX_DETAIL_POINTER_TO_VALUE_T_OF_WRAPPER_HPP__

#ifndef __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__
#error "please include yggr/mplex/pointer_to_value_t.hpp before this file!!!"
#endif // __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/scoped_ptr.hpp>
#include <yggr/smart_ptr_ex/weak_ptr.hpp>
#include <yggr/smart_ptr_ex/intrusive_ptr.hpp>
#include <yggr/smart_ptr_ex/interface_ptr.hpp>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

// std::smart_ptr
#if !defined(YGGR_NO_CXX11_SMART_PTR)

YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX(::std::shared_ptr)
#	if !defined(BOOST_NO_CV_SPECIALIZATIONS)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::shared_ptr, const)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::shared_ptr, volatile)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::shared_ptr, const volatile)
#	endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX(::std::unique_ptr)
#	if !defined(BOOST_NO_CV_SPECIALIZATIONS)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::unique_ptr, const)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::unique_ptr, volatile)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::unique_ptr, const volatile)
#	endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX(::std::weak_ptr)
#	if !defined(BOOST_NO_CV_SPECIALIZATIONS)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::weak_ptr, const)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::weak_ptr, volatile)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::weak_ptr, const volatile)
#	endif //BOOST_NO_CV_SPECIALIZATIONS

#endif // !defined(YGGR_NO_CXX11_SMART_PTR)

// std::auto_ptr
#if !defined(YGGR_NO_AUTO_PTR)

YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX(::std::auto_ptr)
#	if !defined(BOOST_NO_CV_SPECIALIZATIONS)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::auto_ptr, const)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::auto_ptr, volatile)
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::std::auto_ptr, const volatile)
#	endif //BOOST_NO_CV_SPECIALIZATIONS

#endif // YGGR_NO_AUTO_PTR

// boost::smart_ptr
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX(::boost::shared_ptr)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::shared_ptr, const)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::shared_ptr, volatile)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::shared_ptr, const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX(::boost::scoped_ptr)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::scoped_ptr, const)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::scoped_ptr, volatile)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::scoped_ptr, const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX(::boost::weak_ptr)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::weak_ptr, const)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::weak_ptr, volatile)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::weak_ptr, const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX(::boost::intrusive_ptr)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::intrusive_ptr, const)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::intrusive_ptr, volatile)
YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF(::boost::intrusive_ptr, const volatile)
#endif //BOOST_NO_CV_SPECIALIZATIONS

namespace yggr
{
namespace mplex
{
namespace detail
{

template<typename T, 
			template<typename _T> class SPtr>
struct unwrap_pointer< interface_ptr<T, SPtr> >
{
	typedef T type;
};

#if !defined(BOOST_NO_CV_SPECIALIZATIONS)

template<typename T, 
			template<typename _T> class SPtr>
struct unwrap_pointer< interface_ptr<T, SPtr> const >
{
	typedef T type;
};

template<typename T, 
			template<typename _T> class SPtr>
struct unwrap_pointer< interface_ptr<T, SPtr> volatile >
{
	typedef T type;
};

template<typename T, 
			template<typename _T> class SPtr>
struct unwrap_pointer< interface_ptr<T, SPtr> const volatile >
{
	typedef T type;
};
#endif // BOOST_NO_CV_SPECIALIZATIONS

} // namespace detail
} // namespace mplex
} // namespace yggr

#endif //__YGGR_MPLEX_DETAIL_POINTER_TO_VALUE_T_OF_WRAPPER_HPP__