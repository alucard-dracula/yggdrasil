//tuple.hpp

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

#ifndef __YGGR_TUPLE_TUPLE_HPP__
#define __YGGR_TUPLE_TUPLE_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/mplex/index_t_value.hpp>

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
// The MSVC version
#include <yggr/tuple/detail/tuple_basic_no_partial_spec.hpp>

#else
// other compilers
#include <yggr/tuple/detail/tuple_basic.hpp>

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#include <yggr/tuple/tuple_utility_decl.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/size_t.hpp>

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
#	include <tuple>
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)


namespace boost 
{    

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

//using std::get; // don't using std::array, because in stl get<typename Index> version is occupied

template<std::size_t I, typename ...Args> inline
typename 
	boost::tuples::access_traits
	<
		typename std::tuple_element<I, std::tuple<Args...> >::type
	>::non_const_type
	get(std::tuple<Args...>& c)
{
	return std::get<I>(c);
}

template<std::size_t I, typename ...Args> inline
typename 
	boost::tuples::access_traits
	<
		typename std::tuple_element<I, std::tuple<Args...> >::type
	>::const_type
	get(const std::tuple<Args...>& c)
{
	return std::get<I>(c);
}

template<typename Index, typename ...Args> inline 
typename 
	boost::tuples::access_traits
	<
		typename std::tuple_element<YGGR_INDEX_T_VALUE_GET(Index), std::tuple<Args...> >::type
	>::non_const_type
	get(std::tuple<Args...>& c)
{
	return std::get<YGGR_INDEX_T_VALUE_GET(Index)>(c);
}

template<typename Index, typename ...Args> inline 
typename 
	boost::tuples::access_traits
	<
		typename std::tuple_element<YGGR_INDEX_T_VALUE_GET(Index), std::tuple<Args...> >::type
	>::const_type
	get(const std::tuple<Args...>& c)
{
	return std::get<YGGR_INDEX_T_VALUE_GET(Index)>(c);
}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

#if !defined(BOOST_NO_USING_TEMPLATE)

using boost::tuples::get;

// boost
template<typename Index, typename HT, typename TT> inline
typename 
	boost::tuples::access_traits
	<
		typename boost::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<HT, TT> >::type
	>::non_const_type
	get(boost::tuples::cons<HT, TT>& c)
{
	return boost::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	boost::tuples::access_traits
	<
		typename boost::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<HT, TT> >::type
	>::const_type
	get(const boost::tuples::cons<HT, TT>& c)
{
	return boost::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	yggr::tuples::access_traits
	<
		typename yggr::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), yggr::tuples::cons<HT, TT> >::type
	>::non_const_type
	get(yggr::tuples::cons<HT, TT>& c)
{
	return yggr::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	yggr::tuples::access_traits
	<
		typename yggr::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), yggr::tuples::cons<HT, TT> >::type
	>::const_type
	get(const yggr::tuples::cons<HT, TT>& c)
{
	return yggr::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

#elif !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
//
// The "using tuples::get" statement causes the
// Borland compiler to ICE, use forwarding
// functions instead:
//

// boost
template<typename Index, typename HT, typename TT> inline
typename 
	boost::tuples::access_traits
	<
		typename boost::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<HT, TT> >::type
	>::non_const_type
	get(boost::tuples::cons<HT, TT>& c)
{
	return boost::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	boost::tuples::access_traits
	<
		typename boost::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<HT, TT> >::type
	>::const_type
	get(const boost::tuples::cons<HT, TT>& c)
{
	return boost::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

// yggr
template<int N, typename HT, typename TT> inline 
typename yggr::tuples::access_traits<
				typename yggr::tuples::element<N, yggr::tuples::cons<HT, TT> >::type
			>::non_const_type
	get(yggr::tuples::cons<HT, TT>& c) 
{
	return yggr::tuples::get<N,HT,TT>(c);
} 

// get function for const cons-lists, returns a const reference to
// the element. If the element is a reference, returns the reference
// as such (that is, can return a non-const reference)

template<int N, typename HT, typename TT> inline 
typename yggr::tuples::access_traits<
				typename yggr::tuples::element<N, yggr::tuples::cons<HT, TT> >::type
			>::const_type
	get(const yggr::tuples::cons<HT, TT>& c) 
{
	return yggr::tuples::get<N,HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	yggr::tuples::access_traits
	<
		typename yggr::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), yggr::tuples::cons<HT, TT> >::type
	>::non_const_type
	get(yggr::tuples::cons<HT, TT>& c)
{
	return yggr::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	yggr::tuples::access_traits
	<
		typename yggr::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), yggr::tuples::cons<HT, TT> >::type
	>::const_type
	get(const yggr::tuples::cons<HT, TT>& c)
{
	return yggr::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

#else  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//
// MSVC, using declarations don't mix with templates well,
// so use forwarding functions instead:
//

// boost

template<typename Index, typename HT, typename TT> inline
typename boost::tuples::detail::element_ref<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<Head, Tail> >::RET
	get(boost::tuples::cons<HT, TT>& c, boost::tuples::detail::workaround_holder<N>* = 0)
{
	return boost::tuples::detail::get_class<YGGR_INDEX_T_VALUE_GET(Index)>::get(t);
}

template<typename Index, typename HT, typename TT> inline
typename boost::tuples::detail::element_const_ref<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<Head, Tail> >::RET
	get(const boost::tuples::cons<HT, TT>& c, boost::tuples::detail::workaround_holder<N>* = 0)
{
	return boost::tuples::detail::get_class<YGGR_INDEX_T_VALUE_GET(Index)>::get(t);
}

// yggr

template<int N, typename Head, typename Tail> inline
typename yggr::tuples::detail::element_ref<N, yggr::tuples::cons<Head, Tail> >::RET
	get(yggr::tuples::cons<Head, Tail>& t, yggr::tuples::detail::workaround_holder<N>* = 0)
{
	return yggr::tuples::detail::get_class<N>::get(t);
}


template<int N, typename Head, typename Tail> inline
typename yggr::tuples::detail::element_const_ref<N, yggr::tuples::cons<Head, Tail> >::RET
	get(const yggr::tuples::cons<Head, Tail>& t, yggr::tuples::detail::workaround_holder<N>* = 0)
{
	return yggr::tuples::detail::get_class<N>::get(t);
}

template<typename Index, typename HT, typename TT> inline
typename yggr::tuples::detail::element_ref<YGGR_INDEX_T_VALUE_GET(Index), yggr::tuples::cons<Head, Tail> >::RET
	get(yggr::tuples::cons<HT, TT>& c, yggr::tuples::detail::workaround_holder<N>* = 0)
{
	return yggr::tuples::detail::get_class<YGGR_INDEX_T_VALUE_GET(Index)>::get(t);
}

template<typename Index, typename HT, typename TT> inline
typename yggr::tuples::detail::element_const_ref<YGGR_INDEX_T_VALUE_GET(Index), yggr::tuples::cons<Head, Tail> >::RET
	get(const yggr::tuples::cons<HT, TT>& c, yggr::tuples::detail::workaround_holder<N>* = 0)
{
	return yggr::tuples::detail::get_class<YGGR_INDEX_T_VALUE_GET(Index)>::get(t);
}

#endif // BOOST_NO_USING_TEMPLATE
   
} // namespace boost


namespace yggr
{

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

//using std::get; // don't using std::array, because in stl get<typename Index> version is occupied

template<std::size_t I, typename ...Args> inline
typename 
	boost::tuples::access_traits
	<
		typename std::tuple_element<I, std::tuple<Args...> >::type
	>::non_const_type
	get(std::tuple<Args...>& c)
{
	return std::get<I>(c);
}

template<std::size_t I, typename ...Args> inline
typename 
	boost::tuples::access_traits
	<
		typename std::tuple_element<I, std::tuple<Args...> >::type
	>::const_type
	get(const std::tuple<Args...>& c)
{
	return std::get<I>(c);
}

template<typename Index, typename ...Args> inline 
typename 
	boost::tuples::access_traits
	<
		typename std::tuple_element<YGGR_INDEX_T_VALUE_GET(Index), std::tuple<Args...> >::type
	>::non_const_type
	get(std::tuple<Args...>& c)
{
	return std::get<YGGR_INDEX_T_VALUE_GET(Index)>(c);
}

template<typename Index, typename ...Args> inline 
typename 
	boost::tuples::access_traits
	<
		typename std::tuple_element<YGGR_INDEX_T_VALUE_GET(Index), std::tuple<Args...> >::type
	>::const_type
	get(const std::tuple<Args...>& c)
{
	return std::get<YGGR_INDEX_T_VALUE_GET(Index)>(c);
}


#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

using tuples::tuple;
using tuples::make_tuple;
using tuples::tie;

#if !defined(BOOST_NO_USING_TEMPLATE)

using boost::tuples::get;

template<typename Index, typename HT, typename TT> inline
typename 
	tuples::access_traits
	<
		typename tuples::element<YGGR_INDEX_T_VALUE_GET(Index), tuples::cons<HT, TT> >::type
	>::non_const_type
	get(tuples::cons<HT, TT>& c)
{
	return tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	boost::tuples::access_traits
	<
		typename boost::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<HT, TT> >::type
	>::non_const_type
	get(boost::tuples::cons<HT, TT>& c)
{
	return boost::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	tuples::access_traits
	<
		typename tuples::element<YGGR_INDEX_T_VALUE_GET(Index), tuples::cons<HT, TT> >::type
	>::const_type
	get(const tuples::cons<HT, TT>& c)
{
	return tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	boost::tuples::access_traits
	<
		typename boost::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<HT, TT> >::type
	>::const_type
	get(const boost::tuples::cons<HT, TT>& c)
{
	return boost::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

#elif !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
//
// The "using tuples::get" statement causes the
// Borland compiler to ICE, use forwarding
// functions instead:
//
template<int N, typename HT, typename TT> inline
typename tuples::access_traits<
					typename tuples::element<N, tuples::cons<HT, TT> >::type
				>::non_const_type
	get(tuples::cons<HT, TT>& c) 
{
	return tuples::get<N,HT,TT>(c);
}

template<int N, typename HT, typename TT> inline
typename boost::tuples::access_traits<
					typename boost::tuples::element<N, boost::tuples::cons<HT, TT> >::type
				>::non_const_type
	get(boost::tuples::cons<HT, TT>& c) 
{
	return boost::tuples::get<N,HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	tuples::access_traits
	<
		typename tuples::element<YGGR_INDEX_T_VALUE_GET(Index), tuples::cons<HT, TT> >::type
	>::non_const_type
	get(tuples::cons<HT, TT>& c) 
{
	return tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline
typename 
	boost::tuples::access_traits
	<
		typename boost::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<HT, TT> >::type
	>::non_const_type
	get(boost::tuples::cons<HT, TT>& c) 
{
	return boost::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
} 

// get function for const cons-lists, returns a const reference to
// the element. If the element is a reference, returns the reference
// as such (that is, can return a non-const reference)
template<int N, typename HT, typename TT> inline 
typename tuples::access_traits<
				typename tuples::element<N, tuples::cons<HT, TT> >::type
			>::const_type
	get(const tuples::cons<HT, TT>& c)
{
	return tuples::get<N,HT,TT>(c);
}

template<int N, typename HT, typename TT> inline 
typename boost::tuples::access_traits<
				typename boost::tuples::element<N, boost::tuples::cons<HT, TT> >::type
			>::const_type
	get(const boost::tuples::cons<HT, TT>& c)
{
	return boost::tuples::get<N,HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline 
typename 
	tuples::access_traits
	<
		typename tuples::element<YGGR_INDEX_T_VALUE_GET(Index), tuples::cons<HT, TT> >::type
	>::const_type
	get(const tuples::cons<HT, TT>& c)
{
	return tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

template<typename Index, typename HT, typename TT> inline 
typename 
	boost::tuples::access_traits
	<
		typename boost::tuples::element<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<HT, TT> >::type
	>::const_type
	get(const boost::tuples::cons<HT, TT>& c)
{
	return boost::tuples::get<YGGR_INDEX_T_VALUE_GET(Index),HT,TT>(c);
}

#else  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//
// MSVC, using declarations don't mix with templates well,
// so use forwarding functions instead:
//
template<int N, typename Head, typename Tail> inline
typename tuples::detail::element_ref<N, tuples::cons<Head, Tail> >::RET
	get(tuples::cons<Head, Tail>& t, tuples::detail::workaround_holder<N>* = 0)
{
	return tuples::detail::get_class<N>::get(t);
}

template<int N, typename Head, typename Tail> inline
typename boost::tuples::detail::element_ref<N, boost::tuples::cons<Head, Tail> >::RET
	get(boost::tuples::cons<Head, Tail>& t, boost::tuples::detail::workaround_holder<N>* = 0)
{
	return tuples::detail::get_class<N>::get(t);
}

template<typename Index, typename Head, typename Tail> inline
typename tuples::detail::element_ref<YGGR_INDEX_T_VALUE_GET(Index), tuples::cons<Head, Tail> >::RET
	get(tuples::cons<Head, Tail>& t, tuples::detail::workaround_holder<YGGR_INDEX_T_VALUE_GET(Index)>* = 0)
{
	return tuples::detail::get_class<YGGR_INDEX_T_VALUE_GET(Index)>::get(t);
}

template<typename Index, typename Head, typename Tail> inline
typename boost::tuples::detail::element_ref<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<Head, Tail> >::RET
	get(boost::tuples::cons<Head, Tail>& t, boost::tuples::detail::workaround_holder<YGGR_INDEX_T_VALUE_GET(Index)>* = 0)
{
	return tuples::detail::get_class<YGGR_INDEX_T_VALUE_GET(Index)>::get(t);
}

template<int N, typename Head, typename Tail> inline
typename tuples::detail::element_const_ref<N, tuples::cons<Head, Tail> >::RET
	get(const tuples::cons<Head, Tail>& t, tuples::detail::workaround_holder<N>* = 0)
{
	return tuples::detail::get_class<N>::get(t);
}

template<int N, typename Head, typename Tail> inline
typename boost::tuples::detail::element_const_ref<N, boost::tuples::cons<Head, Tail> >::RET
	get(const boost::tuples::cons<Head, Tail>& t, boost::tuples::detail::workaround_holder<N>* = 0)
{
	return boost::tuples::detail::get_class<N>::get(t);
}

template<typename Index, typename Head, typename Tail> inline
typename tuples::detail::element_const_ref<YGGR_INDEX_T_VALUE_GET(Index), tuples::cons<Head, Tail> >::RET
	get(const tuples::cons<Head, Tail>& t, tuples::detail::workaround_holder<YGGR_INDEX_T_VALUE_GET(Index)>* = 0)
{
	return tuples::detail::get_class<YGGR_INDEX_T_VALUE_GET(Index)>::get(t);
}

template<typename Index, typename Head, typename Tail> inline
typename boost::tuples::detail::element_const_ref<YGGR_INDEX_T_VALUE_GET(Index), boost::tuples::cons<Head, Tail> >::RET
	get(const boost::tuples::cons<Head, Tail>& t, boost::tuples::detail::workaround_holder<YGGR_INDEX_T_VALUE_GET(Index)>* = 0)
{
	return boost::tuples::detail::get_class<YGGR_INDEX_T_VALUE_GET(Index)>::get(t);
}

#endif // BOOST_NO_USING_TEMPLATE

} // namespace yggr

namespace yggr
{
namespace tuples
{

template<typename T>
struct is_tuple_cons
	: public boost::mpl::false_
{
};

template<typename HT, typename HH>
struct is_tuple_cons< cons<HT, HH> >
	: public boost::mpl::true_
{
};

template<typename HT, typename HH>
struct is_tuple_cons< boost::tuples::cons<HT, HH> >
	: public boost::mpl::true_
{
};

} // namespace tuples
} // namespace yggr

// tuple_utility (tuple_size, tuple_element)

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace boost
{
namespace tuples
{

template<typename ...Args>
struct length< std::tuple<Args...> >
	: public boost::mpl::size_t< std::tuple_size<std::tuple<Args...> >::value>
{
};

template<typename ...Args>
struct length< std::tuple<Args...> const >
	: public boost::mpl::size_t< std::tuple_size<std::tuple<Args...> >::value>
{
};

} // namespace tuples
} // namespace boost


#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace yggr
{

template<typename Tuple>
struct tuple_size
	: public boost::mpl::size_t<tuples::length<Tuple>::value>
{
};

template<std::size_t I, typename Tuple>
struct tuple_element
	: public tuples::element<I, Tuple>
{
};

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

//namespace tuples
//{
//
//template<typename ...Args>
//struct length< std::tuple<Args...> >
//	: public boost::mpl::size_t< std::tuple_size<std::tuple<Args...> >::value>
//{
//};
//
//template<typename ...Args>
//struct length< std::tuple<Args...> const >
//	: public boost::mpl::size_t< std::tuple_size<std::tuple<Args...> >::value>
//{
//};
//
//} // namespace tuples

using boost::tuples::length;

template<typename ...Args>
struct tuple_size< std::tuple<Args...> >
	: public boost::mpl::size_t< std::tuple_size<std::tuple<Args...> >::value>
{
};

template<std::size_t I, typename ...Args>
struct tuple_element< I, std::tuple<Args...> >
	: std::tuple_element<I, std::tuple<Args...> >
{
};

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

} // namespace yggr

#endif // __YGGR_TUPLE_TUPLE_HPP__
