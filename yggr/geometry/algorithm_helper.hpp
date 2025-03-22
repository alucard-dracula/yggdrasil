//algorithm_helper.hpp

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

#ifndef __YGGR_GEOMETRY_ALGORITHM_HELPER_HPP__
#define __YGGR_GEOMETRY_ALGORITHM_HELPER_HPP__

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/math/flaw_comparer.hpp>
#include <yggr/math/vector2d.hpp>

#include <yggr/geometry/geo_line_segment.hpp>

#include <memory>

namespace yggr
{
namespace geometry
{

class algorithm_helper
	: private nonable::noncreateable
{
public:

	template<typename Val,
				template<typename _Val, std::size_t _N> class Array,
				template < typename _Val, typename Allocator> class Container >
	static void bresenham_disperse(
				const math::vector2d<Val, Array>& src, 
				const math::vector2d<Val, Array>& dst,
				Container< math::vector2d<Val, Array>, std::allocator< math::vector2d<Val, Array> > >& container)
	{
		
		typedef Val val_type;
		typedef math::vector2d<val_type, Array> pos_type;
		typedef math::flaw_comparer<val_type> cmper_type;

		val_type x0 = src.x, y0 = src.y,
                    x1 = dst.x, y1 = dst.y;

		val_type dx = std::abs(x1-x0);
		val_type dy = std::abs(y1-y0);

		val_type sx = x0 < x1? val_type(1) : val_type(-1);
		val_type sy = y0 < y1? val_type(1) : val_type(-1);

		val_type err = dx - dy;

		for(;;)
		{
			container.push_back(pos_type(x0, y0));

			if(x0 == x1 && y0 == y1)
			{
				break;
			}

			val_type e2 = val_type(2) * err;
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
		}
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array,
				template<typename _Pos, 
							template<typename __Val, std::size_t> class _Base > class Line,
				template < typename _Pos, typename Allocator> class Container > inline
	static void bresenham_disperse(const 
									geo_line_segment
									< 
										math::vector2d< Val, Array>,
										Array, 
										Line
									>& line_seg, 
									Container
									<
										math::vector2d<Val, Array>, 
										std::allocator< math::vector2d<Val, Array> > 
									>& container)
	{
		bresenham_disperse(line_seg.src_pos, line_seg.dst_pos, container);
	}
};

} // namespace geometry
} // namspase yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_GEOMETRY_ALGORITHM_HELPER_HPP__