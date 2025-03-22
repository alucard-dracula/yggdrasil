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

#ifndef __YGGR_GEOMETRY_BRESENHAM_DISPERSE_HPP__
#define __YGGR_GEOMETRY_BRESENHAM_DISPERSE_HPP__

#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>

#include <yggr/geometry/detail/bresenham_disperse.hpp>
#include <yggr/geometry/geo_line_segment.hpp>

namespace yggr
{
namespace geometry
{

// bresenham_2d

template<typename Container,
			typename T1, typename T2,
			template<typename _T, std::size_t> class Base1,
			template<typename _T, std::size_t> class Base2 > inline
typename boost::add_reference<Container>::type
	bresenham_disperse(Container& cont,
						const math::vector2d<T1, Base1>& src_pos,
						const math::vector2d<T2, Base2>& dst_pos)
{
	return 
		detail::bresenham_disperse_2d(
			cont, 
			src_pos.x, src_pos.y, 
			dst_pos.x, dst_pos.y);
}

// bresenham_3d

template<typename Container,
			typename T1, typename T2,
			template<typename _T, std::size_t> class Base1,
			template<typename _T, std::size_t> class Base2 > inline
typename boost::add_reference<Container>::type
	bresenham_disperse(Container& cont,
						const math::vector3d<T1, Base1>& src_pos,
						const math::vector3d<T2, Base2>& dst_pos)
{
	return 
		detail::bresenham_disperse_3d(
			cont, 
			src_pos.x, src_pos.y, src_pos.z, 
			dst_pos.x, dst_pos.y, dst_pos.z);
}

// line_segment
template<typename Container,
			typename Pos,
			template<typename _T, std::size_t> class Array,
			template<
				typename _T,
				template<typename __T, std::size_t> class _Base > class Base > inline
typename boost::add_reference<Container>::type
	bresenham_disperse(Container& cont, const geo_line_segment<Pos, Array, Base>& line_seg)
{
	return bresenham_disperse(cont, line_seg.src_pos, line_seg.dst_pos);
}

} // namespace geometry
} // namespace yggr

#endif // __YGGR_GEOMETRY_BRESENHAM_DISPERSE_HPP__
