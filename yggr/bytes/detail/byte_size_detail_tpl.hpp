//byte_size_detail_tpl.hpp

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

#ifndef __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_TPL_HPP__
#define __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_TPL_HPP__

#include <yggr/bytes/detail/byte_size_detail_container.hpp>
#include <yggr/container_ex/is_container.hpp>

namespace yggr
{
namespace bytes
{
namespace detail
{


template<typename T, bool is_container = container_ex::is_container<typename ::yggr::container::unwrap_container<T>::type>::value>
struct byte_size_tpl_inner_helper;

template<typename T>
struct byte_size_tpl_inner_helper<T, true>
	: public byte_size_tpl_container_helper<T>
{
};

template<typename T>
struct byte_size_tpl_inner_helper<T, false>
	: public byte_size_helper_default<T, get_has_foo_id<T>::value>
{
};

template<typename T>
struct byte_size_tpl_helper
	: public byte_size_tpl_inner_helper<T>
{
};

} // namespace detail
} // namespace bytes
} // namespace yggr

namespace yggr
{
namespace bytes
{
namespace detail
{


#define YGGR_PP_MAKE_CONTAINER_BYTE_SIZE_HELPER( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename _T ) > class C > \
	struct byte_size_helper< C < YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > > \
		: public byte_size_tpl_helper< C< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > > {};


#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<template<typename ..._Args> class C, typename ...Args>
struct byte_size_helper< C<Args...> >
	: public byte_size_tpl_helper<  C<Args...> >
{
};

#	if defined(_MSC_VER)
#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_PP_MAKE_CONTAINER_BYTE_SIZE_HELPER( __n__ )

#		define BOOST_PP_LOCAL_LIMITS ( 1, 10 )
#		include BOOST_PP_LOCAL_ITERATE(  )
#	endif // _MSC_VER

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_MAKE_CONTAINER_BYTE_SIZE_HELPER( __n__ )

#	define BOOST_PP_LOCAL_LIMITS ( 1, 10 )
#	include BOOST_PP_LOCAL_ITERATE(  )

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#undef YGGR_PP_MAKE_CONTAINER_BYTE_SIZE_HELPER

} // namespace detail
} // namespace bytes
} // namespace yggr


#endif // __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_TPL_HPP__
