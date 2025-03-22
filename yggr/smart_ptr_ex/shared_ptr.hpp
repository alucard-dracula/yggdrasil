// shared_ptr.hpp

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

#ifndef __YGGR_SMART_PTR_EX_SHARED_PTR_HPP__
#define __YGGR_SMART_PTR_EX_SHARED_PTR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/detail/shared_count_patch.hpp>
#include <yggr/smart_ptr_ex/detail/shared_ptr_has_element_t.hpp>
#include <yggr/smart_ptr_ex/make_shared_ex.hpp>
#include <yggr/smart_ptr_ex/allocate_shared_ex.hpp>
#include <yggr/smart_ptr_ex/construct_shared.hpp>

#if !defined(YGGR_NO_CXX11_SMART_PTR)
#	include <memory>
#endif // YGGR_NO_CXX11_SMART_PTR


namespace yggr
{
namespace smart_ptr_ex
{
#if !defined(YGGR_NO_CXX11_SMART_PTR) && defined(YGGR_SMART_PTR_USING_STL)
	using std::shared_ptr;
#else
	using boost::shared_ptr;
#endif // !defined(YGGR_NO_CXX11_SMART_PTR) && defined(YGGR_SMART_PTR_USING_STL)


namespace detail
{

template<typename T, bool has_element_t>
struct shared_ptr_element_t_impl;

template<typename T, template<typename _T> class SPtr>
struct shared_ptr_element_t_impl<SPtr<T>, true>
{
	typedef typename SPtr<T>::element_type type;
};

template<typename T, template<typename _T> class SPtr>
struct shared_ptr_element_t_impl<SPtr<T>, false>
{
	typedef typename SPtr<T>::value_type type;
};

} // namespace detail

template<typename T>
struct shared_ptr_element_t
	: public 
		detail::shared_ptr_element_t_impl
		<
			T,
			detail::shared_ptr_has_element_t<T>::value
		>
{
};

} // namespace smart_ptr_ex

using smart_ptr_ex::shared_ptr;

} // namespace yggr

#endif // __YGGR_SMART_PTR_EX_SHARED_PTR_HPP__
