// bresenham_disperse.hpp

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

#ifndef __YGGR_GEOMETRY_DETAIL_BRESENHAM_DISPERSE_HPP__
#define __YGGR_GEOMETRY_DETAIL_BRESENHAM_DISPERSE_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/limits.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/range_ex/range_value_ex.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_const.hpp>

#include <boost/mpl/and.hpp>

#include <boost/range/end.hpp>

#include <cassert>
#include <complex>


namespace yggr
{
namespace geometry
{
namespace detail
{

template<typename Container,
			typename T1, typename T2, 
			typename T3, typename T4>
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::and_
			<
				boost::is_integral<T1>,
				boost::is_integral<T2>
			>,
			boost::mpl::and_
			<
				boost::is_integral<T3>,
				boost::is_integral<T4>
			>,
			container_ex::is_container<Container>
		>,
		typename boost::add_reference<Container>::type
	>::type
	bresenham_disperse_2d(Container& cont, T1 nx0, T2 ny0, T3 nx1, T4 ny1)
{
	typedef Container cont_type;
	typedef typename range_ex::range_value_ex<cont_type>::type elem_type;
	typedef typename range_ex::range_value_ex<elem_type>::type val_type;
	typedef mplex::numeric_limits<val_type> val_limits_type;

	BOOST_MPL_ASSERT((boost::is_integral<val_type>));
	BOOST_MPL_ASSERT_NOT((boost::is_const<cont_type>));
	assert(nx0 < val_limits_type::max_type::value);
	assert(ny0 < val_limits_type::max_type::value);
	assert(nx1 < val_limits_type::max_type::value);
	assert(ny1 < val_limits_type::max_type::value);

	val_type x0 = static_cast<val_type>(nx0), 
				y0 = static_cast<val_type>(ny0), 
				x1 = static_cast<val_type>(nx1), 
				y1 = static_cast<val_type>(ny1);

	val_type dx = std::abs(x1 - x0);
	val_type dy = std::abs(y1 - y0);

	iterator_ex::inserter(cont, boost::end(cont)) = elem_type(x0, y0);

	if(!(dx || dy))
	{
		return cont;
	}

	val_type err = dx - dy;

	val_type sx = x0 < x1? val_type(1) : val_type(-1);
	val_type sy = y0 < y1? val_type(1) : val_type(-1);

	val_type e2 = val_type();

	assert(!(x0 == x1 && y0 == y1));
	for(;!(x0 == x1 && y0 == y1);)
	{
		e2 = err + err;
		if(e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		
		if(e2 < dx)
		{
			err += dx;
			y0 += sy;
		}

		iterator_ex::inserter(cont, boost::end(cont)) = elem_type(x0, y0);
	}

	return cont;
}

template<typename Container,
			typename T1, typename T2, typename T3, 
			typename T4, typename T5, typename T6 >
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::and_
			<
				boost::is_integral<T1>,
				boost::is_integral<T2>,
				boost::is_integral<T3>
			>,
			boost::mpl::and_
			<
				boost::is_integral<T4>,
				boost::is_integral<T5>,
				boost::is_integral<T6>
			>,
			container_ex::is_container<Container>
		>,
		typename boost::add_reference<Container>::type
	>::type
	bresenham_disperse_3d(Container& cont, 
							T1 nx0, T2 ny0, T3 nz0, 
							T4 nx1, T5 ny1, T6 nz1)
{
	typedef Container cont_type;
	typedef typename range_ex::range_value_ex<cont_type>::type elem_type;
	typedef typename range_ex::range_value_ex<elem_type>::type val_type;
	typedef mplex::numeric_limits<val_type> val_limits_type;

	BOOST_MPL_ASSERT((boost::is_integral<val_type>));
	BOOST_MPL_ASSERT_NOT((boost::is_const<cont_type>));
	assert(nx0 < val_limits_type::max_type::value);
	assert(ny0 < val_limits_type::max_type::value);
	assert(nz0 < val_limits_type::max_type::value);
	assert(nx1 < val_limits_type::max_type::value);
	assert(ny1 < val_limits_type::max_type::value);
	assert(nz1 < val_limits_type::max_type::value);

	val_type x0 = static_cast<val_type>(nx0), 
				y0 = static_cast<val_type>(ny0), 
				z0 = static_cast<val_type>(nz0),
				x1 = static_cast<val_type>(nx1), 
				y1 = static_cast<val_type>(ny1),
				z1 = static_cast<val_type>(nz1);

	val_type dx = std::abs(x1 - x0);
	val_type dy = std::abs(y1 - y0);
	val_type dz = std::abs(z1 - z0);

	iterator_ex::inserter(cont, boost::end(cont)) = elem_type(x0, y0, z0);

	if(!(dx || dy || dz))
	{
		return cont;
	}

	val_type sx = x0 < x1? val_type(1) : val_type(-1);
	val_type sy = y0 < y1? val_type(1) : val_type(-1);
	val_type sz = z0 < z1? val_type(1) : val_type(-1);

	val_type dx2 = dx + dx;
	val_type dy2 = dy + dy;
	val_type dz2 = dz + dz;

	val_type p0 = val_type();
	val_type p1 = val_type();

	if(dy <= dx && dz <= dx)
	{
		p0 = dy2 - dx;
		p1 = dz2 - dx;
	
		for(;x0 != x1;)
		{
			x0 += sx;
			if(p0 >= 0)
			{
				y0 += sy;
				p0 -= dx2;
			}

			if(p1 >= 0)
			{
				z0 += sz;
				p1 -= dx2;
			}
			p0 += dy2; 
			p1 += dz2;
			iterator_ex::inserter(cont, boost::end(cont)) = elem_type(x0, y0, z0);
		}
	}
	else if(dx <= dy && dz <= dy)
	{
		p0 = dx2 - dy;
		p1 = dz2 - dy;

		for(;y0 != y1;)
		{
			y0 += sy;
			if(p0 >= 0)
			{
				x0 += sx;
				p0 -= dy2;
			}

			if(p1 >= 0)
			{
				z0 += sz;
				p1 -= dy2;
			}

			p0 += dx2;
			p1 += dz2;
			iterator_ex::inserter(cont, boost::end(cont)) = elem_type(x0, y0, z0);
		}
	}
	else
	{
		p0 = dy2 - dz;
		p1 = dx2 - dz;

		for(;z0 != z1;)
		{
			z0 += sz;
			if(p0 >= 0)
			{
				y0 += sy;
				p0 -= dz2;
			}

			if(p1 >= 0)
			{
				x0 += sx;
				p1 -= dz2;
			}

			p0 += dy2;
			p1 += dx2;
			iterator_ex::inserter(cont, boost::end(cont)) = elem_type(x0, y0, z0);
		}
	}

	return cont;
}

} // namespace detail
} // namespace geometry
} // namespace yggr

#endif // __YGGR_GEOMETRY_DETAIL_BRESENHAM_DISPERSE_HPP__
