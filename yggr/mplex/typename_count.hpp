//typename_count.hpp

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

#ifndef __YGGR_MPLEX_TYPENAME_COUNT_HPP__
#define __YGGR_MPLEX_TYPENAME_COUNT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/typedef.hpp>

#include <boost/mpl/size_t.hpp>

namespace yggr
{
namespace mplex
{
namespace detail
{

template<typename T>
struct typename_count_cal
	: public boost::mpl::size_t<0>
{
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<template<typename ..._Args> class Src, typename ...Args>
struct typename_count_cal< Src<Args...> >
	:public boost::mpl::size_t<sizeof...(Args)>
{
};

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T)> class Src, \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T)> \
	struct typename_count_cal< Src< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > > \
		: public boost::mpl::size_t< __n__ >{};

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE()

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace detail

template<typename T>
struct typename_count
	: public detail::typename_count_cal<T>
{
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_TYPENAME_COUNT_HPP__
