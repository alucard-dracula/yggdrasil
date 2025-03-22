// geo_bezier_curve.hpp

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

#ifndef __YGGR_GEOMETRY_GEO_BEZIER_CURVE_HPP__
#define __YGGR_GEOMETRY_GEO_BEZIER_CURVE_HPP__

#include <yggr/range_ex/range_iterator_ex.hpp>

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>

#include <yggr/geometry/geo_bezier_curve_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace geometry
{

//------------------geo_bezier_curve_value-------------------

template<typename Iter, typename T> inline
typename
	boost::enable_if
	<
		is_iterator<Iter>,
		typename
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	geo_bezier_curve_value(Iter s, Iter e, T t)
{
	return detail::bezier_curve_value(s, e, t);
}

template<typename Iter, typename T1, typename T2> inline
typename
	boost::enable_if
	<
		is_iterator<Iter>,
		typename
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	geo_bezier_curve_value(Iter s, Iter e, T1 t1, T2 t2)
{
	return detail::bezier_curve_value(s, e, t1, t2);
}

template<typename Container, typename T> inline
typename
	boost::enable_if
	<
		container_ex::is_container<Container>,
		typename
			upper_float
			<
				typename range_ex::range_value_ex<Container>::type
			>::type
	>::type
	geo_bezier_curve_value(const Container& cont, T t)
{
	return detail::bezier_curve_value(cont, t);
}

template<typename Container, typename T1, typename T2> inline
typename
	boost::enable_if
	<
		container_ex::is_container<Container>,
		typename
			upper_float
			<
				typename range_ex::range_value_ex<Container>::type
			>::type
	>::type
	geo_bezier_curve_value(const Container& cont, T1 t1, T2 t2)
{
	return detail::bezier_curve_value(cont, t1, t2);
}

template<typename P, u32 N, typename T,
			template<typename _P, std::size_t> class Array > inline
typename geo_bezier_curve<P, N, Array>::result_point_type
	geo_bezier_curve_value(const geo_bezier_curve<P, N, Array>& cont, T t)
{
	return cont.value(t);
}

template<typename P, u32 N, typename T1, typename T2,
			template<typename _P, std::size_t> class Array > inline
typename geo_bezier_curve<P, N, Array>::result_point_type
	geo_bezier_curve_value(const geo_bezier_curve<P, N, Array>& cont, T1 t1, T2 t2)
{
	return cont.value(t1, t2);
}

//----------------geo_bezier_curve_derivation------------------

template<typename Iter, typename T> inline
typename
	boost::enable_if
	<
		is_iterator<Iter>,
		typename
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	geo_bezier_curve_derivation(Iter s, Iter e, T t)
{
	return detail::bezier_curve_derivation(s, e, std::distance(s, e) - 1, t);
}

template<typename Iter, typename T1, typename T2> inline
typename
	boost::enable_if
	<
		is_iterator<Iter>,
		typename
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	geo_bezier_curve_derivation(Iter s, Iter e, T1 t1, T2 t2)
{
	return detail::bezier_curve_derivation(s, e, std::distance(s, e) - 1, t1, t2);
}

template<typename Container, typename T> inline
typename
	boost::enable_if
	<
		container_ex::is_container<Container>,
		typename
			upper_float
			<
				typename range_ex::range_value_ex<Container>::type
			>::type
	>::type
	geo_bezier_curve_derivation(const Container& cont, T t)
{
	return detail::bezier_curve_derivation(cont, t);
}

template<typename Container, typename T1, typename T2> inline
typename
	boost::enable_if
	<
		container_ex::is_container<Container>,
		typename
			upper_float
			<
				typename range_ex::range_value_ex<Container>::type
			>::type
	>::type
	geo_bezier_curve_derivation(const Container& cont, T1 t1, T2 t2)
{
	return detail::bezier_curve_derivation(cont, t1, t2);
}

template<typename P, u32 N, typename T,
			template<typename _P, std::size_t> class Array > inline
typename geo_bezier_curve<P, N, Array>::result_point_type
	geo_bezier_curve_derivation(const geo_bezier_curve<P, N, Array>& cont, T t)
{
	return cont.derivation(t);
}

template<typename P, u32 N, typename T1, typename T2,
			template<typename _P, std::size_t> class Array > inline
typename geo_bezier_curve<P, N, Array>::result_point_type
	geo_bezier_curve_derivation(const geo_bezier_curve<P, N, Array>& cont, T1 t1, T2 t2)
{
	return cont.derivation(t1, t2);
}

//-----------------matrix_trans------------------

template<typename P, typename Matrix, u32 N,
			template<typename _P, std::size_t> class Array>
geo_bezier_curve<P, N, Array>
	operator*(const geo_bezier_curve<P, N, Array>& l, const Matrix& r)
{
	typedef geo_bezier_curve<P, N, Array> bezier_curve_type;
	typedef Matrix matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));


	bezier_curve_type rst(l);
	base_type& base_rst = rst.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) *= r;
	}

	return rst;
}

template<typename P, typename Matrix, u32 N,
			template<typename _P, std::size_t> class Array>
geo_bezier_curve<P, N, Array>
	operator*(const Matrix& l, const geo_bezier_curve<P, N, Array>& r)
{
	typedef geo_bezier_curve<P, N, Array> bezier_curve_type;
	typedef Matrix matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));


	bezier_curve_type rst(r);
	base_type& base_rst = rst.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) = l * (*i);
	}

	return rst;
}

template<typename P, typename ValMat, u32 N,
			template<typename _Val, std::size_t> class ArrayBC,
			template<typename _Val, std::size_t> class ArrayMat,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(P::E_ARRAY_LENGTH == 3)>,
		geo_bezier_curve<P, N, ArrayBC>
	>::type
	operator*(const geo_bezier_curve<P, N, ArrayBC>& l,
				const ::yggr::math::matrix2d<ValMat, ArrayMat, BaseMat>& r)
{
	typedef geo_bezier_curve<P, N, ArrayBC> bezier_curve_type;
	typedef ::yggr::math::matrix2d<ValMat, ArrayMat, BaseMat> matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	bezier_curve_type rst(l);
	base_type& base_rst = rst.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) *= r;
	}

	return rst;
}

template<typename P, typename ValMat, u32 N,
			template<typename _Val, std::size_t> class ArrayBC,
			template<typename _Val, std::size_t> class ArrayMat,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(P::E_ARRAY_LENGTH == 3)>,
		geo_bezier_curve<P, N, ArrayBC>
	>::type
	operator*(const ::yggr::math::matrix2d<ValMat, ArrayMat, BaseMat>& r,
				const geo_bezier_curve<P, N, ArrayBC>& l)
{
	typedef geo_bezier_curve<P, N, ArrayBC> bezier_curve_type;
	typedef ::yggr::math::matrix2d<ValMat, ArrayMat, BaseMat> matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	bezier_curve_type rst(r);
	base_type& base_rst = rst.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) = l * (*i);
	}

	return rst;
}

template<typename P, typename ValMat, u32 N,
			template<typename _Val, std::size_t> class ArrayBC,
			template<typename _Val, std::size_t> class ArrayMat,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(P::E_ARRAY_LENGTH == 4)>,
		geo_bezier_curve<P, N, ArrayBC>
	>::type
	operator*(const geo_bezier_curve<P, N, ArrayBC>& l,
				const ::yggr::math::matrix3d<ValMat, ArrayMat, BaseMat>& r)
{
	typedef geo_bezier_curve<P, N, ArrayBC> bezier_curve_type;
	typedef ::yggr::math::matrix3d<ValMat, ArrayMat, BaseMat> matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	bezier_curve_type rst(l);
	base_type& base_rst = rst.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) *= r;
	}

	return rst;
}

template<typename P, typename ValMat, u32 N,
			template<typename _Val, std::size_t> class ArrayBC,
			template<typename _Val, std::size_t> class ArrayMat,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(P::E_ARRAY_LENGTH == 4)>,
		geo_bezier_curve<P, N, ArrayBC>
	>::type
	operator*(const ::yggr::math::matrix3d<ValMat, ArrayMat, BaseMat>& r,
				const geo_bezier_curve<P, N, ArrayBC>& l)
{
	typedef geo_bezier_curve<P, N, ArrayBC> bezier_curve_type;
	typedef ::yggr::math::matrix3d<ValMat, ArrayMat, BaseMat> matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	bezier_curve_type rst(r);
	base_type& base_rst = rst.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) = l * (*i);
	}

	return rst;
}

template<typename P, typename Matrix, u32 N,
			template<typename _P, std::size_t> class Array>
geo_bezier_curve<P, N, Array>&
	operator*=(geo_bezier_curve<P, N, Array>& l, const Matrix& r)
{
	typedef geo_bezier_curve<P, N, Array> bezier_curve_type;
	typedef Matrix matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type& base_rst = l.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) *= r;
	}

	return l;
}

template<typename P, typename ValMat, u32 N,
			template<typename _Val, std::size_t> class ArrayBC,
			template<typename _Val, std::size_t> class ArrayMat,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(P::E_ARRAY_LENGTH == 3)>,
		geo_bezier_curve<P, N, ArrayBC>&
	>::type
	operator*=(geo_bezier_curve<P, N, ArrayBC>& l,
				const ::yggr::math::matrix2d<ValMat, ArrayMat, BaseMat>& r)
{
	typedef geo_bezier_curve<P, N, ArrayBC> bezier_curve_type;
	typedef ::yggr::math::matrix2d<ValMat, ArrayMat, BaseMat> matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type& base_rst = l.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) *= r;
	}

	return l;
}

template<typename P, typename ValMat, u32 N,
			template<typename _Val, std::size_t> class ArrayBC,
			template<typename _Val, std::size_t> class ArrayMat,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(P::E_ARRAY_LENGTH == 4)>,
		geo_bezier_curve<P, N, ArrayBC>&
	>::type
	operator*(geo_bezier_curve<P, N, ArrayBC>& l,
				const ::yggr::math::matrix3d<ValMat, ArrayMat, BaseMat>& r)
{
	typedef geo_bezier_curve<P, N, ArrayBC> bezier_curve_type;
	typedef ::yggr::math::matrix3d<ValMat, ArrayMat, BaseMat> matrix_type;
	typedef typename bezier_curve_type::pos_type pos_type;
	typedef typename bezier_curve_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type& base_rst = l.ctrl_points;

	for(iter_type i = boost::begin(base_rst), isize = boost::end(base_rst); i != isize; ++i)
	{
		(*i) *= r;
	}

	return l;
}

} // namespace geometry
} // namespace yggr

#endif // __YGGR_GEOMETRY_GEO_BEZIER_CURVE_HPP__
