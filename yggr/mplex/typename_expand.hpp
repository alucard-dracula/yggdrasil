//typename_expand.hpp

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

#ifndef __YGGR_MPLEX_TYPENAME_EXPAND_HPP__
#define __YGGR_MPLEX_TYPENAME_EXPAND_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>
#include <yggr/ppex/typedef.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/less.hpp>

namespace yggr
{
namespace mplex
{

namespace detail
{

template<typename T>
struct _typename_expand
	: public boost::mpl::vector<>::type
{
	typedef boost::mpl::size_t<0> size_type;
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<template<typename ..._Args> class Other_Cont, typename ...Args>
struct _typename_expand< Other_Cont<Args...> >
	: public boost::mpl::vector<Args...>
{
	typedef boost::mpl::size_t<sizeof...(Args)> size_type;
};

#else

#define YGGR_PP_TYPENAME_EXPAND( __n__ ) \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T) > \
				class Other_Cont > \
	struct _typename_expand< \
			Other_Cont< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > > \
		: public boost::mpl::vector< \
			YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > { \
		typedef boost::mpl::size_t<__n__> size_type; };

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_PP_TYPENAME_EXPAND( __n__ )

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE( )

#undef YGGR_PP_TYPENAME_EXPAND

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


template<typename T, typename N, bool NLessSize>
struct _safe_arg;

template<typename T, typename N>
struct _safe_arg<T, N, false>
	: public mplex::null_type
{
};

template<typename T, typename N>
struct _safe_arg<T, N, true>
	: public boost::mpl::at<T, N>
{
};

template<typename T, std::size_t N, bool NLessSize>
struct _safe_arg_c;

template<typename T, std::size_t N>
struct _safe_arg_c<T, N, false>
	: public mplex::null_type
{
};

template<typename T, std::size_t N>
struct _safe_arg_c<T, N, true>
	: public boost::mpl::at_c<T, N>
{
};

} // namespace detail

template<typename T>
struct typename_expand
	: public detail::_typename_expand<T>
{
public:
	typedef T class_type;
private:
	typedef detail::_typename_expand<T> base_type;

public:
	template<typename N>
	struct arg
		: public boost::mpl::at<base_type, N>
	{
	};

	template<std::size_t N>
	struct arg_c
		: public boost::mpl::at_c<base_type, N>
	{
	};

	template<typename N>
	struct safe_arg
		: public
			detail::_safe_arg
			<
				base_type, 
				N, 
				boost::mpl::less<N, typename base_type::size_type>::value
			>
	{
	};

	template<std::size_t N>
	struct safe_arg_c
		: public 
			detail::_safe_arg_c
			<
				base_type, 
				N, 
				static_cast<bool>(N < base_type::size_type::value)
			>
	{
	};
};

template<typename T>
struct typename_expand_size
	: public typename_expand<T>::size_type
{
};

template<typename T, typename N>
struct typename_expand_t_get
	: public typename_expand<T>
{
public:
	typedef typename_expand<T> base_type;

private:
	typedef typename base_type::template arg<N> arg_type;

public:
	typedef typename arg_type::type type;
};

template<typename T, std::size_t N>
struct typename_expand_get
	: public typename_expand<T>
{
public:
	typedef typename_expand<T> base_type;

private:
	typedef typename base_type::template arg_c<N> arg_type;

public:
	typedef typename arg_type::type type;
};

template<typename T, typename N>
struct typename_expand_t_get_safe
	: public typename_expand<T>
{
public:
	typedef typename_expand<T> base_type;

private:
	typedef typename base_type::template safe_arg<N> arg_type;

public:
	typedef typename arg_type::type type;
};

template<typename T, std::size_t N>
struct typename_expand_get_safe
	: public typename_expand<T>
{
public:
	typedef typename_expand<T> base_type;

private:
	typedef typename base_type::template safe_arg_c<N> arg_type;

public:
	typedef typename arg_type::type type;
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_TYPENAME_EXPAND_HPP__