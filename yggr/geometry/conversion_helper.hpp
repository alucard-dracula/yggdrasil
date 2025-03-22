//conversion_helper.hpp

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

#ifndef __YGGR_GEOMETRY_CONVERSION_HELPER_HPP__
#define __YGGR_GEOMETRY_CONVERSION_HELPER_HPP__

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>

#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>

#include <yggr/geometry/plane.hpp>
#include <yggr/geometry/geo_line.hpp>
#include <yggr/geometry/geo_line_segment.hpp>

namespace yggr
{
namespace geometry
{

class conversion_helper
	: private nonable::noncreateable
{
public:

	//---------------------two_to_three--------------------------
	template<typename Val,
				template<typename _Val, std::size_t> class Array> inline
	static const math::vector3d<Val, Array> 
		two_to_three(const math::vector2d<Val, Array>& right)
	{
		typedef math::vector3d<Val, Array> ret_type;
		return ret_type(right[0], right[1], 0);
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t, 
							template<typename __Val, std::size_t> class _Base> class Det> inline
	static const math::vector3d<Val, Array> 
		two_to_three(const math::vector2d<Val, Array>& right, 
							const math::matrix3d<Val, Array, Det>& mat)
	{
		typedef math::vector3d<Val, Array> ret_type;

		ret_type ret(right[0], right[1], 0);
		return ret * mat;
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array> inline
	static const geo_line< math::vector3d<Val, Array>, Array > 
		two_to_three(const geo_line< math::vector2d<Val, Array>, Array >& right)
	{
		typedef geo_line< math::vector3d<Val, Array>, Array > ret_type;
		
		return ret_type(two_to_three(right.src_pos), two_to_three(right.dst_pos));
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t, 
							template<typename __Val, std::size_t> class _Base> class Det> inline
	static const geo_line< math::vector3d<Val, Array>, Array > 
		two_to_three(const geo_line< math::vector2d<Val, Array>, Array >& right, 
						const math::matrix3d<Val, Array, Det>& mat)
	{
		typedef geo_line< math::vector3d<Val, Array>, Array > ret_type;

		return ret_type(two_to_three(right.src_pos, mat), two_to_three(right.dst_pos, mat));
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array,
				template<typename _Val, 
							template<tyname __Val, std::size_t> class _Base> class Line > inline
	static const geo_line_segment< math::vector3d<Val, Array>, Array, Line >
		two_to_three(const geo_line_segment< math::vector2d<Val, Array>, Array, Line >& right)
	{
		typedef geo_line_segment< math::vector3d<Val, Array>, Array, Line > ret_type;
		
		return ret_type(two_to_three(right.src_pos), two_to_three(right.dst_pos));
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t, 
							template<typename __Val, std::size_t> class Base> class Det> inline
	static const geo_line_segment< math::vector3d<Val, Array>, Array, Line >
		two_to_three(const geo_line_segment< math::vector2d<Val, Array>, Array, Line >& right, 
							const math::matrix3d<Val, Array, Det>& mat)
	{
		typedef geo_line_segment< math::vector3d<Val, Array>, Array, Line > ret_type;

		return ret_type(two_to_three(right.src_pos, mat), two_to_three(right.dst_pos, mat));
	}

	//---------------------three_to_two--------------------------

	template<typename Val,
				template<typename _Val, std::size_t> class Array> inline
	static const math::vector2d<Val, Array> 
		three_to_two(const math::vector3d<Val, Array>& right)
	{
		typedef math::vector2d<Val, Array> ret_type;
		return ret_type(right[0], right[1], right[3]);
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t, 
							template<typename __Val, std::size_t> class Base> class Det> inline
	static const math::vector2d<Val, Array> 
		three_to_two(const math::vector3d<Val, Array>& right, 
						const math::matrix3d<Val, Array, Det>& mat)
	{
		typedef math::vector2d<Val, Array> ret_type;
		typedef math::vector3d<Val, Array> vec3d_type;
		
		vec3d_type vec(right * mat);

		ret_type ret(vec[0], vec[1]);
		return ret * mat;
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array> inline
	static const geo_line< math::vector2d<Val, Array>, Array > 
		three_to_two(const geo_line< math::vector3d<Val, Array>, Array >& right)
	{
		typedef geo_line< math::vector2d<Val, Array>, Array > ret_type;

		return ret_type(three_to_two(right.src_pos), three_to_two(right.dst_pos));
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t, 
							template<typename __Val, std::size_t> class Base> class Det> inline
	static const geo_line< math::vector2d<Val, Array>, Array > 
		three_to_two(const geo_line< math::vector3d<Val, Array>, Array >& right,
						const math::matrix3d<Val, Array, Det>& mat)
	{
		typedef geo_line< math::vector2d<Val, Array>, Array > ret_type;

		return ret_type(three_to_two(right.src_pos, mat), three_to_two(right.dst_pos, mat));
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array> inline
	static const geo_line_segment< math::vector2d<Val, Array>, Array, Line > 
		three_to_two(const geo_line_segment< math::vector3d<Val, Array>, Array, Line >& right)
	{
		typedef geo_line_segment< math::vector2d<Val, Array>, Array, Line > ret_type;

		return ret_type(three_to_two(right.src_pos), three_to_two(right.dst_pos));
	}

	template<typename Val,
				template<typename _Val, std::size_t> class Array,
				template<typename _Val, std::size_t, std::size_t, 
							template<typename __Val, std::size_t> class Base> class Det> inline
	static const geo_line_segment< math::vector2d<Val, Array>, Array, Line > 
		three_to_two(const geo_line_segment< math::vector3d<Val, Array>, Array, Line >& right,
						const math::matrix3d<Val, Array, Det>& mat)
	{
		typedef geo_line_segment< math::vector2d<Val, Array>, Array, Line > ret_type;

		return ret_type(three_to_two(right.src_pos, mat), three_to_two(right.dst_pos, mat));
	}

};

} // namespace geometry
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_GEOMETRY_CONVERSION_HELPER_HPP__