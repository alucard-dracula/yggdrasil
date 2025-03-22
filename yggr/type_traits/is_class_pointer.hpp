//is_class_pointer.hpp

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

#ifndef __YGGR_TYPE_TRAITS_IS_CLASS_POINTER_HPP__
#define __YGGR_TYPE_TRAITS_IS_CLASS_POINTER_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace yggr
{
namespace type_traits
{
namespace detail
{

template<typename T>
struct _is_class_pointer
	: public
		boost::mpl::and_
		<
			boost::mpl::bool_<boost::is_pointer<T>::value>,
			boost::mpl::bool_
			<
				boost::is_class
				<
					typename
						boost::remove_cv
						<
							typename boost::remove_pointer<T>::type
						>::type
				>::value
			>
		>
{
};

} // namespace detail
} // namespace type_traits


template<typename T>
struct is_class_pointer
	: public type_traits::detail::_is_class_pointer<T>::type
{
};


} // namespace yggr

#endif // __YGGR_TYPE_TRAITS_IS_CLASS_POINTER_HPP__
