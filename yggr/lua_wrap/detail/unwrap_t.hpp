// unwrap_t.hpp

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

#ifndef __YGGR_LUA_WRAP_DETAIL_UNWRAP_T_HPP__
#define __YGGR_LUA_WRAP_DETAIL_UNWRAP_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/lua_wrap/detail/has_unwrap.hpp>
#include <yggr/lua_wrap/detail/is_wraped_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename T, bool is_wrap>
struct unwrap_t_impl;

template<typename T>
struct unwrap_t_impl<T, false>
{
	typedef T type;
};

template<typename T>
struct unwrap_t_impl<T, true>
{
	typedef typename T::base_type type;
};

template<typename T>
struct unwrap_t
	: public 
		unwrap_t_impl
		<
			T,
			is_wraped_t<T>::value
		>
{
};


template<typename T> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::disable_if
	<
		is_wraped_t<T>,
		typename unwrap_t<T>::type& 
	>::type
	unwrap_object(T& obj) YGGR_NOEXCEPT_OR_NOTHROW
{
	return obj;
}

template<typename T> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::disable_if
	<
		is_wraped_t<T>,
		const typename unwrap_t<T>::type& 
	>::type
	unwrap_object(const T& obj) YGGR_NOEXCEPT_OR_NOTHROW
{
	return obj;
}

template<typename T> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_wraped_t<T>,
		typename unwrap_t<T>::type& 
	>::type
	unwrap_object(T& obj) YGGR_NOEXCEPT_OR_NOTHROW
{
	return obj.unwrap();
}

template<typename T> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_wraped_t<T>,
		const typename unwrap_t<T>::type& 
	>::type
	unwrap_object(const T& obj) YGGR_NOEXCEPT_OR_NOTHROW
{
	return obj.unwrap();
}

} // namespace detail
} // namespace lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_DETAIL_UNWRAP_T_HPP__
