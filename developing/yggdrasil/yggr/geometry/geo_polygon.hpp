//geo_polygon.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>
#include <yggr/geometry/geo_polygon_t.hpp>

namespace yggr
{
namespace geometry
{
} // namespace geometry
} // namespace yggr

template<typename Position, typename Matrix,
		template<typename _Val> class Alloc,
		template<typename _Val, typename _Alloc> class Base>
yggr::geometry::geo_polygon<Position, Alloc, Base>
	operator*(const yggr::geometry::geo_polygon<Position, Alloc, Base>& l, const Matrix& r)
{
	typedef yggr::geometry::geo_polygon<Position, Alloc, Base> polygon_type;
	typedef Matrix matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename base_type::iterator iter_type;
	typedef typename polygon_type::pos_type pos_type;
	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	base_type rst(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}
	return polygon_type(rst);
}

template<typename Position, typename ValR,
		template<typename _Val> class Alloc,
		template<typename _Val, typename _Alloc> class BaseL,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR
>
yggr::geometry::geo_polygon<Position, Alloc, BaseL>
	operator*(const yggr::geometry::geo_polygon<Position, Alloc, BaseL>& l,
				const yggr::math::matrix2d<ValR, Array, BaseR>& r)
{
	typedef yggr::geometry::geo_polygon<Position, Alloc, BaseL> polygon_type;
	typedef yggr::math::matrix2d<ValR, Array, BaseR> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename base_type::iterator iter_type;
	typedef typename polygon_type::pos_type pos_type;
	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	base_type rst(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}
	return polygon_type(rst);
}

template<typename Position, typename ValR,
		template<typename _Val> class Alloc,
		template<typename _Val, typename _Alloc> class BaseL,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR
>
yggr::geometry::geo_polygon<Position, Alloc, BaseL>
	operator*(const yggr::geometry::geo_polygon<Position, Alloc, BaseL>& l,
				const yggr::math::matrix3d<ValR, Array, BaseR>& r)
{
	typedef yggr::geometry::geo_polygon<Position, Alloc, BaseL> polygon_type;
	typedef yggr::math::matrix3d<ValR, Array, BaseR> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename base_type::iterator iter_type;
	typedef typename polygon_type::pos_type pos_type;
	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	base_type rst(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}
	return polygon_type(rst);
}

template<typename Position, typename Matrix,
		template<typename _Val> class Alloc,
		template<typename _Val, typename _Alloc> class Base>
yggr::geometry::geo_polygon<Position, Alloc, Base>&
	operator*=(yggr::geometry::geo_polygon<Position, Alloc, Base>& l, const Matrix& r)
{
	typedef yggr::geometry::geo_polygon<Position, Alloc, Base> polygon_type;
	typedef Matrix matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename base_type::iterator iter_type;
	typedef typename polygon_type::pos_type pos_type;
	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	base_type rst;
	rst.swap(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}

	l.swap(rst);
	return l;
}

template<typename Position, typename ValR,
		template<typename _Val> class Alloc,
		template<typename _Val, typename _Alloc> class BaseL,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR
>
yggr::geometry::geo_polygon<Position, Alloc, BaseL>&
	operator*=(yggr::geometry::geo_polygon<Position, Alloc, BaseL>& l,
				const yggr::math::matrix2d<ValR, Array, BaseR>& r)
{
	typedef yggr::geometry::geo_polygon<Position, Alloc, BaseL> polygon_type;
	typedef yggr::math::matrix2d<ValR, Array, BaseR> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename base_type::iterator iter_type;
	typedef typename polygon_type::pos_type pos_type;
	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	base_type rst;
	rst.swap(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}

	l.swap(rst);
	return l;
}

template<typename Position, typename ValR,
		template<typename _Val> class Alloc,
		template<typename _Val, typename _Alloc> class BaseL,
		template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR
>
yggr::geometry::geo_polygon<Position, Alloc, BaseL>&
	operator*=(yggr::geometry::geo_polygon<Position, Alloc, BaseL>& l,
				const yggr::math::matrix3d<ValR, Array, BaseR>& r)
{
	typedef yggr::geometry::geo_polygon<Position, Alloc, BaseL> polygon_type;
	typedef yggr::math::matrix3d<ValR, Array, BaseR> matrix_type;
	typedef typename polygon_type::base_type base_type;
	typedef typename base_type::iterator iter_type;
	typedef typename polygon_type::pos_type pos_type;
	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	base_type rst;
	rst.swap(l);

	for(iter_type i = rst.begin(), isize = rst.end(); i != isize; ++i)
	{
		(*i) *= r;
	}

	l.swap(rst);
	return l;
}

#endif // __YGGR_GEOMETRY_GEO_POLYGON_HPP__
