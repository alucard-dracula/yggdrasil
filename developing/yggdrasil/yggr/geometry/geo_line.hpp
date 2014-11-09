//geo_line.hpp

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

#ifndef __YGGR_GEOMETRY_GEOMETRY_LINE_HPP__
#define __YGGR_GEOMETRY_GEOMETRY_LINE_HPP__

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>
#include <yggr/geometry/geo_line_t.hpp>

namespace yggr
{
namespace geometry
{
} // namespace geometry
} // namespace yggr

template<typename Position, typename Matrix,
			template<typename _Val, std::size_t> class Base>
yggr::geometry::geo_line<Position, Base> operator*(const yggr::geometry::geo_line<Position, Base>& l, 
													const Matrix& r)
{
	typedef yggr::geometry::geo_line<Position, Base> line_type;
	typedef Matrix matrix_type;
	typedef typename line_type::pos_type pos_type;

	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	return line_type(l.src_pos * r, l.dst_pos * r);
}

template<typename Position, typename ValR,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR
		>
yggr::geometry::geo_line<Position, BaseL> 
	operator*(const yggr::geometry::geo_line<Position, BaseL>& l, 
				const yggr::math::matrix2d<ValR, Array, BaseR>& r)
{
	typedef yggr::geometry::geo_line<Position, BaseL> line_type;
	typedef yggr::math::matrix2d<ValR, Array, BaseR> matrix_type;
	typedef typename line_type::pos_type pos_type;

	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	return line_type(l.src_pos * r, l.dst_pos * r);
}

template<typename Position, typename ValR,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR
		>
yggr::geometry::geo_line<Position, BaseL> 
	operator*(const yggr::geometry::geo_line<Position, BaseL>& l, 
				const yggr::math::matrix3d<ValR, Array, BaseR>& r)
{
	typedef yggr::geometry::geo_line<Position, BaseL> line_type;
	typedef yggr::math::matrix3d<ValR, Array, BaseR> matrix_type;
	typedef typename line_type::pos_type pos_type;

	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	return line_type(l.src_pos * r, l.dst_pos * r);
}

template<typename Position, typename Matrix,
			template<typename _Val, std::size_t> class Base>
yggr::geometry::geo_line<Position, Base>& operator*=(yggr::geometry::geo_line<Position, Base>& l, 
														const Matrix& r)
{
	typedef yggr::geometry::geo_line<Position, Base> line_type;
	typedef Matrix matrix_type;
	typedef typename line_type::pos_type pos_type;

	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));
	line_type tmp(l.src_pos * r, l.dst_pos * r);
	l.swap(tmp);
	return l;
}

template<typename Position, typename ValR,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR
		>
yggr::geometry::geo_line<Position, BaseL>& 
	operator*=(yggr::geometry::geo_line<Position, BaseL>& l, 
				const yggr::math::matrix2d<ValR, Array, BaseR>& r)
{
	typedef yggr::geometry::geo_line<Position, BaseL> line_type;
	typedef yggr::math::matrix2d<ValR, Array, BaseR> matrix_type;
	typedef typename line_type::pos_type pos_type;

	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	line_type tmp(l.src_pos * r, l.dst_pos * r);
	l.swap(tmp);
	return l;
}

template<typename Position, typename ValR,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR
		>
yggr::geometry::geo_line<Position, BaseL>& 
	operator*=(yggr::geometry::geo_line<Position, BaseL>& l, 
				const yggr::math::matrix3d<ValR, Array, BaseR>& r)
{
	typedef yggr::geometry::geo_line<Position, BaseL> line_type;
	typedef yggr::math::matrix3d<ValR, Array, BaseR> matrix_type;
	typedef typename line_type::pos_type pos_type;

	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
											boost::mpl::size_t<matrix_type::E_MAX_RANK> >));

	line_type tmp(l.src_pos * r, l.dst_pos * r);
	l.swap(tmp);
	return l;
}

#endif //__YGGR_GEOMETRY_GEOMETRY_LINE_HPP__
