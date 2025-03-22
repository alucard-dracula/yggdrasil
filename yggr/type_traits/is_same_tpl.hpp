//is_same_tpl.hpp

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

#ifndef __YGGR_TYPE_TRAITS_IS_SAME_TPL_HPP__
#define __YGGR_TYPE_TRAITS_IS_SAME_TPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/typedef.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace type_traits
{
namespace detail
{

template<typename TL, typename TR>
struct _is_same_tpl
	: public boost::mpl::bool_< boost::is_same<TL, TR>::value >
{
};


#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<template<typename ..._Args> class TPL, 
			typename ...ArgsL, typename ...ArgsR>
struct _is_same_tpl< TPL<ArgsL...>, TPL<ArgsR...> >
	: public boost::mpl::true_
{
};

#else

#define YGGR_PP_IS_SAME_TEMPLATE( __n__ ) \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename TL), \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename TR), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T) > \
				class TPL > \
	struct _is_same_tpl< \
			TPL< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, TL) >, \
			TPL< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, TR) > > \
		: public boost::mpl::true_{};

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_PP_IS_SAME_TEMPLATE( __n__ )

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE( )

#undef YGGR_PP_IS_SAME_TEMPLATE

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace detail

template<typename TL, typename TR>
struct is_same_tpl
	: public detail::_is_same_tpl<TL, TR>::type
{
};

} // namespace type_traits

using type_traits::is_same_tpl;

} // namespace yggr

#endif // __YGGR_TYPE_TRAITS_IS_SAME_TPL_HPP__