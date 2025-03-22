// conv_to_tuple_vtpl.hpp

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

#ifndef __YGGR_MPLEX_CONV_TO_TUPLE_VTPL_HPP__
#define __YGGR_MPLEX_CONV_TO_TUPLE_VTPL_HPP__

#ifndef __YGGR_MPLEX_CONV_TO_TUPLE_HPP__
#	error "please inclue conv_to_tuple.hpp"
#endif // __YGGR_MPLEX_CONV_TO_TUPLE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/type_traits/integer_sequence.hpp>
#include <yggr/tuple/tuple.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/at.hpp>

namespace yggr
{
namespace mplex
{
namespace detail
{

template<
	template<typename ..._Args> class Tuple, 
	typename Vector,
	typename Indexes>
struct conv_to_tuple_impl_detail;

template<
	template<typename ..._Args> class Tuple, 
	typename Vector,
	std::size_t ...I>
struct conv_to_tuple_impl_detail<Tuple, Vector, ::yggr::index_sequence<I...> >
{
	typedef Tuple<typename boost::mpl::at_c<Vector, I>::type... > type;
};

template<
	template<typename ..._Args> class Tuple,
	typename Vector>
struct conv_to_tuple_impl
	: public 
		conv_to_tuple_impl_detail
		<
			Tuple, 
			Vector, 
			::yggr::make_index_sequence< boost::mpl::size<Vector>::value >
		>
{
};


} // namespace detail
} // namespace mplex
} // namespace yggr

namespace yggr
{
namespace mplex
{

template<template<typename ..._Args> class Tuple, 
			typename Vector, 
			typename Null_T = ::yggr::tuples::null_type>
struct conv_to_tuple
{
private:
	typedef Vector vt_type;
	typedef Null_T null_type;
	typedef typename boost::mpl::find<vt_type, null_type>::type i; 
	BOOST_MPL_ASSERT((
		boost::mpl::if_c <
			((i::pos::value == boost::mpl::size<vt_type>::value)
				&& (boost::mpl::size<vt_type>::value <= YGGR_TUPLE_LIMIT_LENGTH())),
			boost::mpl::true_,
			boost::mpl::false_ > ));
public:
	typedef typename yggr::mplex::detail::conv_to_tuple_impl<Tuple, vt_type>::type type;
};


} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_CONV_TO_TUPLE_VTPL_HPP__
