//pointer_check.hpp

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

#ifndef __YGGR_TYPE_TRAITS_POINTER_CHECK_HPP__
#define __YGGR_TYPE_TRAITS_POINTER_CHECK_HPP__

#include <yggr/ppex/symbols.hpp>
#include <yggr/type_traits/has_operator_pointer_member_access.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace yggr
{ 
namespace type_traits 
{ 
namespace detail 
{

template<typename T>
struct _is_pointer_wrapper
	: public boost::mpl::false_
{
};

} // namespace detail
} // namespace type_traits
} // namespace yggr

#define YGGR_PP_IS_POINTER_WRAPPER_DEF( __pointer_type__ ) \
	namespace yggr{ namespace type_traits{ namespace detail{ \
	template<> \
		struct _is_pointer_wrapper< __pointer_type__ > : public boost::mpl::true_ { }; }}}

#define YGGR_PP_IS_POINTER_WRAPPER_SMART_POINTER_DEF( __template__ ) \
	namespace yggr{ namespace type_traits{ namespace detail{ \
	template<typename T> \
		struct _is_pointer_wrapper< __template__< T > > : public boost::mpl::true_ { }; }}}

namespace yggr
{

template<typename T>
struct is_pointer_wrapper
	: public
		boost::mpl::bool_
		<
			boost::mpl::or_
			<
				typename 
					has_operator_pointer_member_access_class
					< 
						typename boost::remove_cv<T>::type
					>::type,
				typename 
					type_traits::detail::_is_pointer_wrapper
					< 
						typename boost::remove_cv<T>::type 
					>::type
			>::type::value
		>
{
};

template<typename T>
struct is_pointer
	: public 
		boost::mpl::bool_
		<
			boost::mpl::or_
			<
				is_pointer_wrapper<typename boost::remove_cv<T>::type>,
				boost::is_pointer< typename boost::remove_cv<T>::type>
			>::type::value
		>
{
};

template<typename T>
struct is_native_pointer
	: public 
		boost::mpl::bool_
		<
			boost::is_pointer<typename boost::remove_cv<T>::type>::type::value
		>
{
};

} // namespace yggr

#include <yggr/type_traits/detail/pointer_checker_of_wrapper.hpp>

#endif //__YGGR_TYPE_TRAITS_POINTER_CHECK_HPP__