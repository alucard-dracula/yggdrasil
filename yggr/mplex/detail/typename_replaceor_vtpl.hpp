//typename_repaceor_vtpl.hpp

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

#ifndef __YGGR_MPLEX_TYPENAME_REPLACEOR_VTPL_HPP__
#define __YGGR_MPLEX_TYPENAME_REPLACEOR_VTPL_HPP__

#ifndef __YGGR_MPLEX_TYPENAME_REPLACEOR_HPP__
#	error "please inclue typename_replaceor.hpp"
#endif // __YGGR_MPLEX_TYPENAME_REPLACEOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/integer_sequence.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/push_back.hpp>

namespace yggr
{
namespace mplex
{

typedef null_type _keep;


namespace detail
{

template<typename Keep, typename ArgsVt, std::size_t N>
struct typename_rep_filling_keep_detail
	: public 
		typename_rep_filling_keep_detail
		<
			Keep,
			typename 
				boost::mpl::push_back
				<
					ArgsVt, Keep
				>::type, 
			N - 1
		>
{
};

template<typename Keep, typename ArgsVt>
struct typename_rep_filling_keep_detail<Keep, ArgsVt, 0>
{
	typedef ArgsVt type;
};

template<typename Keep, std::size_t N, typename ...Args>
struct typename_rep_filling_keep
	: public 
		typename_rep_filling_keep_detail
		<
			Keep, 
			boost::mpl::vector<Args...>,
			N
		>
{
};

template<std::size_t N, typename ...Args>
struct typename_rep_erase_overage_args
{
private:
	typedef boost::mpl::vector<Args...> args_vt_type;

public:
	typedef typename
		boost::mpl::erase
		<
			args_vt_type,
			typename boost::mpl::advance<typename boost::mpl::begin<args_vt_type>::type, boost::mpl::size_t<N> >::type,
			typename boost::mpl::end<args_vt_type>::type
		>::type type;
};


template<int Chk, std::size_t N, typename ...Args>
struct typename_rep_format_args_detail;

template<std::size_t N, typename ...Args>
struct typename_rep_format_args_detail<0, N, Args...>
	: public boost::mpl::vector<Args...>
{
};

template<std::size_t N, typename ...Args>
struct typename_rep_format_args_detail<-1, N, Args...>
	: public typename_rep_erase_overage_args<N, Args...>
{
};

template<std::size_t N, typename ...Args>
struct typename_rep_format_args_detail<1, N, Args...>
	: public typename_rep_filling_keep<mplex::_keep, N, Args...>
{
};


template<std::size_t N, typename ...Args>
struct typename_rep_format_args
	: public
		typename_rep_format_args_detail
		<
			(N == sizeof...(Args)? 
				0 
				: N < sizeof...(Args)? -1 : 1),
			N,
			Args...
		>
{
};


template<typename T>
struct typename_replaceor_detail
{
	template<typename ...IArgs>
	struct apply
	{
		typedef T type;
	};
};

template< template<typename ..._Args> class TPL,
			typename VtSrc, 
			typename VtDst,
			typename Indexes>
struct typename_replaceor_op_detail;

template< template<typename ..._Args> class TPL,
			typename VtSrc,
			typename VtDst,
			std::size_t ...I>
struct typename_replaceor_op_detail<TPL, VtSrc, VtDst, ::yggr::index_sequence<I...> >
{
	typedef 
		TPL
		<
			typename
				boost::mpl::if_
				<
					boost::is_same<typename boost::mpl::at_c<VtDst, I>::type, mplex::_keep>,
					typename boost::mpl::at_c<VtSrc, I>::type,
					typename boost::mpl::at_c<VtDst, I>::type
				>::type ...
		> type;
};


template< template<typename ..._Args> class TPL, typename VtSrc, typename VtDst, std::size_t N>
struct typename_replaceor_op
	: public typename_replaceor_op_detail<TPL, VtSrc, VtDst, ::yggr::make_index_sequence<N> >
{
};

template<template<typename ..._Args> class TPL, typename ...Args>
struct typename_replaceor_detail< TPL<Args...> >
{
public:
	template<typename ...IArgs>
	struct apply
	{
	private:
		typedef typename boost::mpl::vector<Args...>::type src_vt_type;
		typedef typename typename_rep_format_args<sizeof...(Args), IArgs...>::type dst_vt_type;
	public:
		typedef typename typename_replaceor_op<TPL, src_vt_type, dst_vt_type, sizeof...(Args)>::type type;
	};
};


} // namespace detail


template<typename Src, typename ...Args>
struct typename_replaceor
	: public
		detail::typename_replaceor_detail<Src>::template apply
		<
			Args...
		>
{
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_TYPENAME_REPLACEOR_VTPL_HPP__
