//geo_polygon.hpp

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

#ifndef __YGGR_GEOMETRY_GEO_POLYGON_HPP__
#define __YGGR_GEOMETRY_GEO_POLYGON_HPP__

#include <yggr/range_ex/range_iterator_ex.hpp>

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>
#include <yggr/geometry/geo_polygon_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace geometry
{

template<typename Position, typename Alloc, typename Matrix,
		template<typename _Val, typename _Alloc> class Base>
::yggr::geometry::geo_polygon<Position, Alloc, Base>
	operator*(const ::yggr::geometry::geo_polygon<Position, Alloc, Base>& l, const Matrix& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, Base> polygon_type;
	typedef Matrix matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}
	return polygon_type(boost::move(rst));
}

template<typename Position, typename Alloc, typename Matrix,
		template<typename _Val, typename _Alloc> class Base>
::yggr::geometry::geo_polygon<Position, Alloc, Base>
	operator*(const Matrix& l, const ::yggr::geometry::geo_polygon<Position, Alloc, Base>& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, Base> polygon_type;
	typedef Matrix matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst(r);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) = l * (*i);
	}
	return polygon_type(boost::move(rst));
}

template<typename Position, typename Alloc, typename ValMat,
		template<typename _Val, typename _Alloc> class BasePLYGN,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(Position::E_ARRAY_LENGTH == 3)>,
		::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>
	>::type
	operator*(const ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>& l,
				const ::yggr::math::matrix2d<ValMat, Array, BaseMat>& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN> polygon_type;
	typedef ::yggr::math::matrix2d<ValMat, Array, BaseMat> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}
	return polygon_type(boost::move(rst));
}

template<typename Position, typename ValMat, typename Alloc,
		template<typename _Val, typename _Alloc> class BasePLYGN,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(Position::E_ARRAY_LENGTH == 3)>,
		::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>
	>::type
	operator*(const ::yggr::math::matrix2d<ValMat, Array, BaseMat>& l,
				const ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN> polygon_type;
	typedef ::yggr::math::matrix2d<ValMat, Array, BaseMat> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst(r);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) = l * (*i);
	}
	return polygon_type(boost::move(rst));
}

template<typename Position, typename Alloc, typename ValMat,
		template<typename _Val, typename _Alloc> class BasePLYGN,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(Position::E_ARRAY_LENGTH == 4)>,
		::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>
	>::type
	operator*(const ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>& l,
				const ::yggr::math::matrix3d<ValMat, Array, BaseMat>& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN> polygon_type;
	typedef ::yggr::math::matrix3d<ValMat, Array, BaseMat> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}
	return polygon_type(boost::move(rst));
}

template<typename Position, typename ValMat, typename Alloc,
		template<typename _Val, typename _Alloc> class BasePLYGN,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(Position::E_ARRAY_LENGTH == 4)>,
		::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>
	>::type
	operator*(const ::yggr::math::matrix3d<ValMat, Array, BaseMat>& l,
				const ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN> polygon_type;
	typedef ::yggr::math::matrix3d<ValMat, Array, BaseMat> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst(r);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) = l * (*i);
	}

	return polygon_type(boost::move(rst));
}

template<typename Position, typename Alloc, typename Matrix,
		template<typename _Val, typename _Alloc> class Base>
::yggr::geometry::geo_polygon<Position, Alloc, Base>&
	operator*=(::yggr::geometry::geo_polygon<Position, Alloc, Base>& l, const Matrix& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, Base> polygon_type;
	typedef Matrix matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst;
	rst.swap(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}

	l.swap(rst);
	return l;
}

template<typename Position, typename Alloc, typename ValMat,
		template<typename _Val, typename _Alloc> class BasePLYGN,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat >
typename
	boost::enable_if
	<
		boost::mpl::bool_<(Position::E_ARRAY_LENGTH == 3)>,
		::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>&
	>::type
	operator*=(::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>& l,
				const ::yggr::math::matrix2d<ValMat, Array, BaseMat>& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN> polygon_type;
	typedef ::yggr::math::matrix2d<ValMat, Array, BaseMat> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst;
	rst.swap(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}

	l.swap(rst);
	return l;
}

template<typename Position, typename Alloc, typename ValMat,
		template<typename _Val, typename _Alloc> class BasePLYGN,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseMat>
typename
	boost::enable_if
	<
		boost::mpl::bool_<(Position::E_ARRAY_LENGTH == 4)>,
		::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>&
	>::type
	operator*=(::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN>& l,
				const ::yggr::math::matrix3d<ValMat, Array, BaseMat>& r)
{
	typedef ::yggr::geometry::geo_polygon<Position, Alloc, BasePLYGN> polygon_type;
	typedef ::yggr::math::matrix3d<ValMat, Array, BaseMat> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename range_ex::range_iterator_ex<base_type>::type iter_type;
	typedef typename polygon_type::pos_type pos_type;

	BOOST_MPL_ASSERT((
		boost::mpl::equal_to
		<
			boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
			boost::mpl::size_t<matrix_type::E_MAX_RANK>
		>));

	base_type rst;
	rst.swap(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}

	l.swap(rst);
	return l;
}

} // namespace geometry
} // namespace yggr


#endif // __YGGR_GEOMETRY_GEO_POLYGON_HPP__
