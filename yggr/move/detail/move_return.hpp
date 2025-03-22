// move_return.hpp

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

#ifndef __YGGR_MOVE_DETAIL_MOVE_RETURN_HPP__
#define __YGGR_MOVE_DETAIL_MOVE_RETURN_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/detail/workaround.hpp>
#include <yggr/move/detail/has_move_emulation_enabled_impl.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_lvalue_reference.hpp>
#include <boost/move/move.hpp>

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

#	if defined(BOOST_MOVE_ADDRESS_SANITIZER_ON)
#		define YGGR_MOVE_TO_RV_CAST(RV_TYPE, ARG) reinterpret_cast<RV_TYPE>(ARG)
#	else
#		define YGGR_MOVE_TO_RV_CAST(RV_TYPE, ARG) static_cast<RV_TYPE>(ARG)
#	endif //BOOST_MOVE_ADDRESS_SANITIZER_ON


namespace yggr
{
namespace move
{
namespace detail
{

template<typename Ret, typename T> YGGR_MOVE_FORCEINLINE
typename
	::boost::enable_if_c
	<
		::boost::is_lvalue_reference<Ret>::value
		|| !has_move_emulation_enabled<T>::value,
		T&
	>::type
	move_return(T& x) YGGR_NOEXCEPT
{
	return x;
}

template<typename Ret, typename T> YGGR_MOVE_FORCEINLINE
typename
	::boost::enable_if_c
	<
		!::boost::is_lvalue_reference<Ret>::value
		&& has_move_emulation_enabled<T>::value,
		::boost::rv<T>&
	>::type
	move_return(T& x) BOOST_NOEXCEPT
{
	return *YGGR_MOVE_TO_RV_CAST(::boost::rv<T>*, ::boost::addressof(x));
}

template<typename Ret, typename T> YGGR_MOVE_FORCEINLINE
typename
	::boost::enable_if_c
	<
		!::boost::is_lvalue_reference<Ret>::value
		&& has_move_emulation_enabled<T>::value,
		::boost::rv<T>&
	>::type
	move_return(::boost::rv<T>& x) YGGR_NOEXCEPT
{
	return x;
}

} // namespace detail
} // namespace move
} // namespace yggr

#else    //BOOST_NO_CXX11_RVALUE_REFERENCES


namespace yggr
{
namespace move
{
namespace detail
{

template<typename Ret, typename T> YGGR_MOVE_FORCEINLINE
typename
	::boost::enable_if_c
	<
		::boost::is_lvalue_reference<Ret>::value, T&
	>::type
	move_return(T& x) YGGR_NOEXCEPT
{
	return x;
}

template<typename Ret, typename T> YGGR_MOVE_FORCEINLINE
typename
	::boost::enable_if_c
	<
		!::boost::is_lvalue_reference<Ret>::value,
		Ret&&
	>::type
	move_return(T&& t) YGGR_NOEXCEPT
{
	return static_cast< Ret&& >(t);
}

} // namespace detail
} // namespace move
} // namespace yggr

#endif   //BOOST_NO_CXX11_RVALUE_REFERENCES

#	define YGGR_MOVE_RET(RET_TYPE, REF) ::yggr::move::detail::move_return< RET_TYPE >(REF)

#endif //#ifndef __YGGR_MOVE_DETAIL_MOVE_RETURN_HPP__
