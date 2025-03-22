//pointer_to_value_t.hpp

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

#ifndef __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__
#define __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__

#include <yggr/mplex/null_t.hpp>
#include <yggr/ppex/symbols.hpp>

#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/if.hpp>

#define YGGR_PP_POINTER_UNWRAP_DEF( __pointer_type__, __value_type__ ) \
	namespace yggr { namespace mplex { namespace detail { \
	template<> \
	struct unwrap_pointer< __pointer_type__ > { \
		typedef __value_type__ type; }; } } }

#define YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF( __template__, __suffix__ ) \
	namespace yggr { namespace mplex { namespace detail { \
	template<typename T> \
	struct unwrap_pointer< __template__< T > __suffix__ > { \
		typedef T type; }; } } }

#define YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF_NON_SUFFIX( __template__ ) \
	YGGR_PP_POINTER_UNWRAP_SMART_POINTER_DEF( __template__, YGGR_PP_SYMBOL_EMPTY() )

namespace yggr
{
namespace mplex
{

namespace detail
{

template<typename T>
struct unwrap_pointer
	: public null_type
{
};

template<typename T, bool is_native_ptr>
struct pointer_to_value_t_imp;

template<typename T>
struct pointer_to_value_t_imp<T, false>
	: public boost::remove_cv< typename unwrap_pointer< T >::type >
{
};

template<typename T>
struct pointer_to_value_t_imp<T, true>
	: public boost::remove_cv<typename boost::remove_pointer<T>::type >
{
};

} // namespace detail

template<typename T>
struct pointer_to_value_t
	: public detail::pointer_to_value_t_imp<T, boost::is_pointer<T>::value>
{
};

} // namespace mplex
} // namespace yggr

#include <yggr/mplex/detail/pointer_to_value_t_of_wrapper.hpp>

#endif // __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__
