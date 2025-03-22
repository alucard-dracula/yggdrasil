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

#ifndef __YGGR_TULE_TUPLE_COMPARISON_HPP__
#define __YGGR_TULE_TUPLE_COMPARISON_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/is_pair.hpp>
#include <yggr/tuple/tuple.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

#include <boost/tuple/tuple_comparison.hpp>

// -------------------------------------------------------------
// equality and comparison operators 
//
// == and != compare tuples elementwise
// <, >, <= and >= use lexicographical ordering
//
// Any operator between tuples of different length fails at compile time
// No dependencies between operators are assumed 
// (i.e. !(a<b)  does not imply a>=b, a!=b does not imply a==b etc.
// so any weirdnesses of elementary operators are respected).
//
// -------------------------------------------------------------

namespace boost
{
namespace tuples
{

namespace detail
{

template<typename T1, typename T2, typename T3>
inline bool eq(const T1& l, const std::pair<T2, T3>& r) 
{
	return (l.get_head() == r.first) 
			&& (l.get_tail().get_head() == r.second);
}

template<typename T1, typename T2, typename T3>
inline bool eq(const std::pair<T1, T2>& l, const T3& r) 
{
	return (l.first == r.get_head())
			&& (l.second == r.get_tail().get_head());
}

template<typename T1, typename T2, typename T3>
inline bool neq(const T1& l, const std::pair<T2, T3>& r) 
{
	return (l.get_head() != r.first) 
			|| (l.get_tail().get_head() != r.second);
}

template<typename T1, typename T2, typename T3>
inline bool neq(const std::pair<T1, T2>& l, const T3& r) 
{
	return (l.first != r.get_head())
			|| (l.second != r.get_tail().get_head());
}

template<typename T1, typename T2, typename T3>
inline bool lt(const T1& l, const std::pair<T2, T3>& r) 
{
	return (l.get_head() < r.first) 
			|| ( !(r.first < l.get_head() ) 
				&& ( l.get_tail().get_head() < r.second ));
}

template<typename T1, typename T2, typename T3>
inline bool lt(const std::pair<T1, T2>& l, const T3& r) 
{
	return (l.first < r.get_head()) 
			|| ( !(r.get_head() < l.first ) 
				&& ( l.second < r.get_tail().get_head() ));
}

template<typename T1, typename T2, typename T3>
inline bool gt(const T1& l, const std::pair<T2, T3>& r) 
{
	return (l.get_head() > r.first) 
			|| ( !(r.first > l.get_head() ) 
				&& ( l.get_tail().get_head() > r.second ));
}

template<typename T1, typename T2, typename T3>
inline bool gt(const std::pair<T1, T2>& l, const T3& r) 
{
	return (l.first > r.get_head()) 
			|| ( !(r.get_head() > l.first ) 
				&& ( l.second > r.get_tail().get_head() ));
}

template<typename T1, typename T2, typename T3>
inline bool lte(const T1& l, const std::pair<T2, T3>& r) 
{
	return (l.get_head() <= r.first) 
			|| ( !(r.first <= l.get_head() ) 
				&& ( l.get_tail().get_head() <= r.second ));
}

template<typename T1, typename T2, typename T3>
inline bool lte(const std::pair<T1, T2>& l, const T3& r) 
{
	return (l.first <= r.get_head()) 
			|| ( !(r.get_head() <= l.first ) 
				&& ( l.second <= r.get_tail().get_head() ));
}


template<typename T1, typename T2, typename T3>
inline bool gte(const T1& l, const std::pair<T2, T3>& r) 
{
	return (l.get_head() >= r.first) 
			|| ( !(r.first >= l.get_head() ) 
				&& ( l.get_tail().get_head() >= r.second ));
}

template<typename T1, typename T2, typename T3>
inline bool gte(const std::pair<T1, T2>& l, const T3& r) 
{
	return (l.first >= r.get_head()) 
			|| ( !(r.get_head() >= l.first ) 
				&& ( l.second >= r.get_tail().get_head() ));
}

//template<typename L, typename R>
//struct cmp_operator_chk
//	: public 
//		boost::mpl::and_
//		<
//			boost::mpl::or_
//			<
//				yggr::tuples::is_tuple_cons<L>,
//				yggr::is_pair<L>
//			>,
//			boost::mpl::or_
//			<
//				yggr::tuples::is_tuple_cons<R>,
//				yggr::is_pair<R>
//			>,
//			boost::mpl::not_
//			<
//				boost::mpl::and_
//				<
//					yggr::is_pair<L>,
//					yggr::is_pair<R>
//				>
//			>
//		>::type
//{
//};

} // namespace detail

// !!! why using sfinae at this, because  network::mix_handler  iter1 != iter1 conflicted !!!!
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class LCons,
//			template<typename _T1, typename _T2> class RCons> inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			LCons<T1, T2>, 
//			RCons<S1, S2> 
//		>::type, 
//		bool 
//	>::type 
//	operator==(const LCons<T1, T2>& l, const RCons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);
//
//	return detail::eq(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			Cons<T1, T2>, 
//			std::pair<S1, S2>
//		>::type, 
//		bool 
//	>::type
//	operator==(const Cons<T1, T2>& l, const std::pair<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<T1, T2> >::value == 2 ));
//
//	return detail::eq(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			std::pair<T1, T2>, 
//			Cons<S1, S2>
//		>::type, 
//		bool 
//	>::type 
//	operator==(const std::pair<T1, T2>& l, const Cons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<S1, S2> >::value == 2 ));
//
//	return detail::eq(l, r);
//}
//
//// not equal -----
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class LCons,
//			template<typename _T1, typename _T2> class RCons> inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			LCons<T1, T2>, 
//			RCons<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator!=(const LCons<T1, T2>& l, const RCons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);
//
//	return detail::neq(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			Cons<T1, T2>, 
//			std::pair<S1, S2>
//		>::type, 
//		bool 
//	>::type 
//	operator!=(const Cons<T1, T2>& l, const std::pair<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<T1, T2> >::value == 2 ));
//
//	return detail::neq(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			std::pair<T1, T2>, 
//			Cons<S1, S2>
//		>::type, 
//		bool 
//	>::type 
//	operator!=(const std::pair<T1, T2>& l, const Cons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<S1, S2> >::value == 2 ));
//
//	return detail::neq(l, r);
//}
//
//// <
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class LCons,
//			template<typename _T1, typename _T2> class RCons> inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			LCons<T1, T2>, 
//			RCons<S1, S2>
//		>::type, 
//		bool 
//	>::type 
//	operator<(const LCons<T1, T2>& l, const RCons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);
//
//	return detail::lt(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			Cons<T1, T2>, 
//			std::pair<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator<(const Cons<T1, T2>& l, const std::pair<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<T1, T2> >::value == 2 ));
//
//	return detail::lt(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			std::pair<T1, T2>, 
//			Cons<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator<(const std::pair<T1, T2>& l, const Cons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<S1, S2> >::value == 2 ));
//
//	return detail::lt(l, r);
//}
//
//// >
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class LCons,
//			template<typename _T1, typename _T2> class RCons> inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			LCons<T1, T2>, 
//			RCons<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator>(const LCons<T1, T2>& l, const RCons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);
//
//	return detail::gt(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			Cons<T1, T2>, 
//			std::pair<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator>(const Cons<T1, T2>& l, const std::pair<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<T1, T2> >::value == 2 ));
//
//	return detail::gt(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			std::pair<T1, T2>, 
//			Cons<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator>(const std::pair<T1, T2>& l, const Cons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<S1, S2> >::value == 2 ));
//
//	return detail::gt(l, r);
//}
//
//// <=
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class LCons,
//			template<typename _T1, typename _T2> class RCons> inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			LCons<T1, T2>, 
//			RCons<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator<=(const LCons<T1, T2>& l, const RCons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);
//
//	return detail::lte(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			Cons<T1, T2>, 
//			std::pair<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator<=(const Cons<T1, T2>& l, const std::pair<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<T1, T2> >::value == 2 ));
//
//	return detail::lte(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			std::pair<T1, T2>, 
//			Cons<S1, S2>
//		>::type, 
//		bool 
//	>::type 
//	operator<=(const std::pair<T1, T2>& l, const Cons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<S1, S2> >::value == 2 ));
//
//	return detail::lte(l, r);
//}
//
//// >=
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class LCons,
//			template<typename _T1, typename _T2> class RCons> inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			LCons<T1, T2>, 
//			RCons<S1, S2>
//		>::type, 
//		bool 
//	>::type  
//	operator>=(const LCons<T1, T2>& l, const RCons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);
//
//	return detail::gte(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			Cons<T1, T2>, 
//			std::pair<S1, S2>
//		>::type, 
//		bool 
//	>::type
//	operator>=(const Cons<T1, T2>& l, const std::pair<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<T1, T2> >::value == 2 ));
//
//	return detail::gte(l, r);
//}
//
//template<typename T1, typename T2, typename S1, typename S2,
//			template<typename _T1, typename _T2> class Cons > inline 
//typename 
//	boost::enable_if
//	< 
//		typename detail::cmp_operator_chk
//		< 
//			std::pair<T1, T2>, 
//			Cons<S1, S2>
//		>::type, 
//		bool 
//	>::type 
//	operator>=(const std::pair<T1, T2>& l, const Cons<S1, S2>& r)
//{
//	// check that tuple lengths are equal
//	BOOST_STATIC_ASSERT((length< Cons<S1, S2> >::value == 2 ));
//
//	return detail::gte(l, r);
//}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator==(const cons<T1, T2>& l, const yggr::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::eq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator==(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::eq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator==(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::eq(l, r);
}

// not equal -----

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator!=(const cons<T1, T2>& l, const yggr::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::neq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator!=(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::neq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator!=(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::neq(l, r);
}

// <
template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<(const cons<T1, T2>& l, const yggr::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::lt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::lt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::lt(l, r);
}

// >
template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>(const cons<T1, T2>& l, const yggr::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::gt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::gt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::gt(l, r);
}

// <=
template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<=(const cons<T1, T2>& l, const yggr::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::lte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<=(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::lte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<=(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::lte(l, r);
}

// >=
template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>=(const cons<T1, T2>& l, const yggr::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::gte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2 > inline 
bool operator>=(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::gte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2 > inline 
bool operator>=(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::gte(l, r);
}

} // namespace tuples
} // namespace boost

namespace yggr 
{
namespace tuples 
{

namespace detail 
{
	// comparison operators check statically the length of its operands and
	// delegate the comparing task to the following functions. Hence
	// the static check is only made once (should help the compiler).  
	// These functions assume tuples to be of the same length.

using boost::tuples::detail::eq;
using boost::tuples::detail::neq;
using boost::tuples::detail::lt;
using boost::tuples::detail::gt;
using boost::tuples::detail::lte;
using boost::tuples::detail::gte;

} // end of namespace detail

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator==(const cons<T1, T2>& l, const boost::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::eq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator==(const cons<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::eq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator==(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::eq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator==(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::eq(l, r);
}

// not equal -----

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator!=(const cons<T1, T2>& l, const boost::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::neq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator!=(const cons<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::neq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator!=(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::neq(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator!=(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::neq(l, r);
}

// <
template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<(const cons<T1, T2>& l, const boost::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::lt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<(const cons<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::lt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::lt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::lt(l, r);
}

// >
template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>(const cons<T1, T2>& l, const boost::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::gt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>(const cons<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::gt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::gt(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::gt(l, r);
}

// <=
template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<=(const cons<T1, T2>& l, const boost::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::lte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<=(const cons<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::lte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<=(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::lte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator<=(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::lte(l, r);
}

// >=
template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>=(const cons<T1, T2>& l, const boost::tuples::cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::gte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2> inline 
bool operator>=(const cons<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT(length<T2>::value == length<S2>::value);

	return detail::gte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2 > inline 
bool operator>=(const cons<T1, T2>& l, const std::pair<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<T1, T2> >::value == 2 ));

	return detail::gte(l, r);
}

template<typename T1, typename T2, typename S1, typename S2 > inline 
bool operator>=(const std::pair<T1, T2>& l, const cons<S1, S2>& r)
{
	// check that tuple lengths are equal
	BOOST_STATIC_ASSERT((length< cons<S1, S2> >::value == 2 ));

	return detail::gte(l, r);
}

} //namespace tuples
} //namespace yggr

#endif // __YGGR_TULE_TUPLE_COMPARISON_HPP__
