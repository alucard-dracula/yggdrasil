//swap.hpp

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

#ifndef __YGGR_UTILITY_SWAP_HPP__
#define __YGGR_UTILITY_SWAP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/swap.hpp>
#include <boost/type_traits/is_same.hpp>

#include <algorithm> //for std::swap
#include <cstddef> //for std::size_t

// at gcc boost adl not support this namespace adl why?
// so swap foo don't append boost_swap_impl namespace, need append boost namespace
//namespace boost_swap_impl
//{
//	template<typename T1, typename T2>
//	void swap_impl(T1& left, T2& right)
//	{
//		using std::swap;
//		swap(left, right);
//	}
//} // namespace boost_swap_impl

// if vc Error	1	error C2668: 'std::swap' : ambiguous call to overloaded function was occurred
// you need check you code
// 1 don't write swap(RVREF, REF) swap(REF, RVREF)
// 2 if you want to use swap(T1, T2)  T1 and T2 must need swap enable
// 3 if you template class is different swap enable non-member swap need two version
//		template<typename T1> swap(template<T1>&, template<T1>&);
//		template<typename T1, typename T2> swap(template<T1>&, template<T2>&);

// diff type swap at stl
namespace std
{

// support diff type swap

#if defined(_MSC_VER)
template<typename T1, typename T2> inline
void swap(T1& l, T2& r)
#else
template<typename T1, typename T2> inline
typename boost::disable_if<boost::is_same<T1, T2>, void>::type
    swap(T1& l, T2& r)
#endif // _MSC_VER
{
	if(reinterpret_cast<void*>(boost::addressof(l))
		== reinterpret_cast<void*>(boost::addressof(r)) )
	{
		return;
	}

	T1 tmp = boost::move(l);
	l = boost::move(r);
	r = boost::move(tmp);
}

// stl container need these foos
template<typename T1, typename T2> inline
void swap(BOOST_RV_REF(T1) l, T2& r)
{
	T1& ref_l = l;
	using ::std::swap;
	swap(ref_l, r);
}

template<typename T1, typename T2> inline
void swap(T1& l, BOOST_RV_REF(T2) r)
{
	T2& ref_r = r;
	using ::std::swap;
	swap(l, ref_r);
}

} // namespace std

// diff type swap at boost
namespace boost
{

// gcc default conv t&& -> const t&
// but msvc default conv t&& -> t&, so msvc error C2668: ambiguous call to overloaded function occured,
// so msvc not need these foos
// !!!!! and all class' swap foo vc don't need rv_ref version other compiler need rv_ref_version !!!!!

#if !defined(_MSC_VER) || (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)
template<typename T1, typename T2> inline
void swap(BOOST_RV_REF(T1) l, T2& r)
{
	T1& ref_l = l;

	using ::std::swap;
	swap(ref_l, r);
}

template<typename T1, typename T2> inline
void swap(T1& l, BOOST_RV_REF(T2) r)
{
	T2& ref_r = r;

	using ::std::swap;
	swap(l, ref_r);
}
#endif // #if !defined(_MSC_VER) || (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)

} //namespace boost


namespace yggr
{

template<typename T1, typename T2> inline
void swap(T1& l, T2& r)
{
	using ::std::swap;
	swap(l, r);
}

#if ((YGGR_CPP_VERSION < YGGR_CPP_VER_11) && !defined(__GNUC__))
template<typename T> inline
void swap(T& l, T& r)
{
	using ::std::swap;
	swap(l, r);
}

#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#if !defined(_MSC_VER) || (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)
template<typename T1, typename T2> inline
void swap(BOOST_RV_REF(T1) l, T2& r)
{
	T1& ref_l = l;
	using ::std::swap;
	swap(ref_l, r);
}

template<typename T1, typename T2> inline
void swap(T1& l, BOOST_RV_REF(T2) r)
{
	T2& ref_r = r;
	using ::std::swap;
	swap(l, ref_r);
}

#endif // #if !defined(_MSC_VER) || (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)

} // namespace yggr

#endif // __YGGR_UTILITY_SWAP_HPP__
