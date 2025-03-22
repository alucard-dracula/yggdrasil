// remove_cv_ref.hpp

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

#ifndef __YGGR_TYPE_TRAITS_REMOVE_CV_REF_HPP__
#define __YGGR_TYPE_TRAITS_REMOVE_CV_REF_HPP__

#include <yggr/base/yggrdef.h>

#if BOOST_VERSION < 106500

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

#else

#include <boost/type_traits/remove_cv_ref.hpp>

#endif // BOOST_VERSION < 106500

namespace yggr
{
namespace detail
{

#if BOOST_VERSION < 106500

template<typename T>
struct remove_cv_ref_detail
	: public
		boost::remove_reference
		<
			typename boost::remove_cv<T>::type
		>
{
};

#else

//template<typename T>
//struct remove_cv_ref_detail
//	: public boost::remove_cv_ref<T>
//{
//};

template<typename T>
struct remove_cv_ref_detail
	: public
		boost::remove_reference
		<
			typename boost::remove_cv<T>::type
		>
{
};


#endif // BOOST_VERSION


} // namespace detail

template<typename T>
struct remove_cv_ref 
	: public detail::remove_cv_ref_detail<T>
{
};

} // namespace yggr


#endif // __YGGR_TYPE_TRAITS_REMOVE_CV_REF_HPP__