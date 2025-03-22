// add_noexcept_impl.hpp

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

#ifndef __YGGR_FUNC_DETAIL_ADD_NOEXCEPT_IMPL_HPP__
#define __YGGR_FUNC_DETAIL_ADD_NOEXCEPT_IMPL_HPP__

#ifndef __YGGR_FUNC_ADD_NOEXCEPT_HPP__
#	error "please inclue add_noexcept.hpp"
#endif // __YGGR_FUNC_ADD_NOEXCEPT_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 106600) \
		&& (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION) \
		&& !defined(BOOST_NO_CXX11_NOEXCEPT)
#	include <boost/callable_traits/add_noexcept.hpp>
#else
#	include <yggr/type_traits/traits.hpp>
#endif // BOOST_VERSION < 106600

namespace yggr
{
namespace func
{
namespace detail
{

#if !(BOOST_VERSION < 106600) \
		&& (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION) \
		&& !defined(BOOST_NO_CXX11_NOEXCEPT) // 106600 and later

//#if 0 // test

template<typename T>
struct add_noexcept_impl
	: public boost::callable_traits::add_noexcept<T>
{
};

#else

template<typename T>
struct add_noexcept_impl
	: public yggr::traits< T >
{
};


#if !( defined(BOOST_NO_CXX11_NOEXCEPT) \
		|| defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES) \
		|| (YGGR_CPP_VERSION < YGGR_CPP_VER_17) ) 

#define YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_F( __cc__ ) \
	template<typename R, typename ...Args> \
	struct add_noexcept_impl< R __cc__ (Args...) > \
		: public ::yggr::traits< R __cc__ (Args...) noexcept > {}; \
	\
	template<typename R, typename ...Args> \
	struct add_noexcept_impl< R __cc__ (Args..., ...) > \
		: public ::yggr::traits< R __cc__ (Args..., ...) noexcept > {}; \
	\
	template<typename R, typename ...Args> \
	struct add_noexcept_impl< R( __cc__ *)(Args...) > \
		: public ::yggr::traits< R( __cc__ *)(Args...) noexcept > {}; \
	\
	template<typename R, typename ...Args> \
	struct add_noexcept_impl< R( __cc__ *)(Args..., ...) > \
		: public ::yggr::traits< R( __cc__ *)(Args..., ...) noexcept > {};


#define YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_MF( __cc__ ) \
	template<typename R, typename C, typename ...Args> \
	struct add_noexcept_impl<R(__cc__ C::*)(Args...) > \
		: public ::yggr::traits< R(__cc__ C::*)(Args...) noexcept > {}; \
	\
	template<typename R, typename C, typename ...Args> \
	struct add_noexcept_impl<R(__cc__ C::*)(Args..., ...) > \
		: public ::yggr::traits< R(__cc__ C::*)(Args..., ...) noexcept > {}; \
	\
	template<typename R, typename C, typename ...Args> \
	struct add_noexcept_impl<R(__cc__ C::*)(Args...) const > \
		: public ::yggr::traits< R(__cc__ C::*)(Args...) const noexcept > {}; \
	\
	template<typename R, typename C, typename ...Args> \
	struct add_noexcept_impl<R(__cc__ C::*)(Args..., ...) const > \
		: public ::yggr::traits< R(__cc__ C::*)(Args..., ...) const noexcept > {}; \
	\
	template<typename R, typename C, typename ...Args> \
	struct add_noexcept_impl<R(__cc__ C::*)(Args...) volatile > \
		: public ::yggr::traits< R(__cc__ C::*)(Args...) volatile noexcept > {}; \
	\
	template<typename R, typename C, typename ...Args> \
	struct add_noexcept_impl<R(__cc__ C::*)(Args..., ...) volatile > \
		: public ::yggr::traits< R(__cc__ C::*)(Args..., ...) volatile noexcept > {}; \
	\
	template<typename R, typename C, typename ...Args> \
	struct add_noexcept_impl<R(__cc__ C::*)(Args...) const volatile > \
		: public ::yggr::traits< R(__cc__ C::*)(Args...) const volatile noexcept > {}; \
	\
	template<typename R, typename C, typename ...Args> \
	struct add_noexcept_impl<R(__cc__ C::*)(Args..., ...) const volatile > \
		: public ::yggr::traits< R(__cc__ C::*)(Args..., ...) const volatile noexcept > {}; \

#	if defined(BOOST_MEM_FN_ENABLE_CDECL) 
#		define YGGR_FUNC_CC_CDECL __cdecl
			YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_F( YGGR_FUNC_CC )
			YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_MF( YGGR_FUNC_CC )
#		undef YGGR_FUNC_CC_CDECL
#	endif // BOOST_MEM_FN_ENABLE_CDECL

#	if defined(BOOST_MEM_FN_ENABLE_STDCALL) 
#		define YGGR_FUNC_CC_STDCALL __stdcall
			YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_F( YGGR_FUNC_CC_STDCALL )
			YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_MF( YGGR_FUNC_CC_STDCALL )
#		undef YGGR_FUNC_CC_STDCALL
#	endif // BOOST_MEM_FN_ENABLE_STDCALL

#	if defined(BOOST_MEM_FN_ENABLE_FASTCALL) 
#		define YGGR_FUNC_CC_FASTCALL __fastcall
			YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_F( YGGR_FUNC_CC_FASTCALL )
			YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_MF( YGGR_FUNC_CC_FASTCALL )
#		undef YGGR_FUNC_CC_FASTCALL
#	endif // BOOST_MEM_FN_ENABLE_FASTCALL

#	define YGGR_FUNC_CC_DEFAULT
		YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_F( YGGR_FUNC_CC_DEFAULT )
		YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_MF( YGGR_FUNC_CC_DEFAULT )
#	undef YGGR_FUNC_CC_DEFAULT


#undef YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_F
#undef YGGR_PP_MAKE_ADD_NOEXCEPT_IMPL_MF


#endif // !defined(BOOST_NO_CXX11_NOEXCEPT)
#endif // (BOOST_VERSION < 106600) 

} // namespace detail
} // namespace func
} // namespace yggr

#endif // __YGGR_FUNC_DETAIL_ADD_NOEXCEPT_IMPL_HPP__
