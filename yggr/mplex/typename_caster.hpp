//typename_caster.hpp

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

#ifndef __YGGR_MPLEX_TYPENAME_CASTER_HPP__
#define __YGGR_MPLEX_TYPENAME_CASTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/typedef.hpp>

namespace yggr
{
namespace mplex
{
namespace detail
{

template<typename T>
struct typename_caster;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<template<typename ..._Args> class Src, typename ...Args>
struct typename_caster< Src<Args...> >
{
	template<template<typename ..._Args> class Dst>
	struct apply
	{
		typedef Dst<Args...> type;
	};
};

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T), \
			template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T)> class Src> \
	struct typename_caster< Src< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > > { \
		template< template< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T) > class Dst > \
		struct apply { typedef Dst<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > type; }; };

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE()

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace detail

using detail::typename_caster;

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_TYPENAME_CASTER_HPP__
