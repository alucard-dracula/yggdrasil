//geo_plane.hpp

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

#ifndef __YGGR_GEOMETRY_GEO_PLANE_HPP__
#define __YGGR_GEOMETRY_GEO_PLANE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/upper_types.hpp>
#include <boost/any.hpp>
#include <yggr/math/math.hpp>

namespace yggr
{

namespace math
{

template<typename Val, 
			template <typename _Val, std::size_t> class Base>
class vector3d ;

template<typename Val, 
			template <typename _Val, std::size_t> class Basey>
class geo_plane;

} // namespace math

namespace geometry
{

template<typename Val, 
			template <typename _Val, std::size_t> class Base = boost::array>
class geo_plane : public Base<typename upper_signed<Val>::value_type, 4> 
{
public:
	typedef typename upper_signed<Val>::value_type val_type;

private:
	enum
	{
		E_LENGTH = 4,
		E_ARRAY_LENGTH = E_LENGTH,
		E_compile_u32 = 0xffffffff
	};

	typedef Base<val_type, E_LENGTH> base_type;
	typedef geo_plane this_type;

public:
	geo_plane(void)
		: a((*this)[0]), b((*this)[1]), c((*this)[2]), d((*this)[3])
	{
	}

	
	template<typename OVal>
	geo_plane(const math::vector3d<OVal, Base>& pos, const math::vector3d<OVal, Base>& vec)
		: a((*this)[0]), b((*this)[1]), c((*this)[2]), d((*this)[3])
	{
		a = static_cast<val_type>(vec.x);
		b = static_cast<val_type>(vec.y);
		c = static_cast<val_type>(vec.z);
		d = static_cast<val_type>(-(pos.dot(vec)));
	}

	geo_plane(const math::vector3d<val_type, Base>& pos, const math::vector3d<val_type, Base>& vec)
		: a((*this)[0]), b((*this)[1]), c((*this)[2]), d((*this)[3])
	{
		a = vec.x;
		b = vec.y;
		c = vec.z;
		d = -(pos.dot(vec));
	}

	// cross_line or parallel_line or line_and_a_line_out_point please use three points create 

	template<typename OVal>
	geo_plane(const math::vector3d<OVal, Base>& pos1,
				const math::vector3d<OVal, Base>& pos2, 
				const math::vector3d<OVal, Base>& pos3)
		: a((*this)[0]), b((*this)[1]), c((*this)[2]), d((*this)[3])
	{
		typedef math::vector3d<val_type, Base> vec_type;
		vec_type vec1(pos2 - pos1);
		vec_type vec2(pos3 - pos1);
		vec_type vec_n = vec1.cross(vec2);

		a = vec_n.x;
		b = vec_n.y;
		c = vec_n.z;
		d = -(pos1.x * a + pos1.y * b + pos1.z * c);

	}

	geo_plane(const math::vector3d<val_type, Base>& pos1, 
				const math::vector3d<val_type, Base>& pos2,
				const math::vector3d<val_type, Base>& pos3)
		: a((*this)[0]), b((*this)[1]), c((*this)[2]), d((*this)[3])
	{

		typedef math::vector3d<val_type, Base> vec_type;
		vec_type vec1(pos2 - pos1);
		vec_type vec2(pos3 - pos1);
		vec_type vec_n = vec1.cross(vec2);

		a = vec_n.x;
		b = vec_n.y;
		c = vec_n.z;
		d = -(pos1.x * a + pos1.y * b + pos1.z * c);
	}

	geo_plane(const base_type& right)
		: base_type(right), a((*this)[0]), b((*this)[1]), c((*this)[2]), d((*this)[3])
	{
	}

	template<typename OVal>
	geo_plane(const geo_plane<OVal, Base>& right)
		: a((*this)[0]), b((*this)[1]), c((*this)[2]), d((*this)[3])
	{
		(*this)[0] = static_cast<val_type>(right[0]);
		(*this)[1] = static_cast<val_type>(right[1]);
		(*this)[2] = static_cast<val_type>(right[2]);
		(*this)[3] = static_cast<val_type>(right[3]);
	}

	geo_plane(const this_type& right)
		: base_type(right), a((*this)[0]), b((*this)[1]), c((*this)[2]), d((*this)[3])
	{
	}

	~geo_plane(void)
	{
	}

	inline const size_type size(void) const
	{
		return E_LENGTH;
	}

	inline static const size_type s_size(void)
	{
		return E_LENGTH;
	}

	inline const size_type array_size(void) const
	{
		return E_ARRAY_LENGTH;
	}

	inline static const size_type s_array_size(void)
	{
		return E_ARRAY_LENGTH;
	}

	this_type& normalize(void)
	{
		typedef math::vector3d<val_type, Base> vec_type;
		vec_type vec(a, b, c);
		val_type len_sq = vec.length_sq();
		a = math::sign(a) * a * a / len_sq;
		b = math::sign(b) * b * b / len_sq;
		c = math::sign(c) * c * c / len_sq;
		d = math::sign(d) * d * d / len_sq;

		return *this;
	}

	const geo_plane<typename upper_float<val_type>::value_type, Base> get_normalized_plane(void) const
	{
		typedef geo_plane<typename upper_float<val_type>::value_type, Base> rst_type;
		rst_type rst(*this);
		rst.normalize();
		return rst;
	}

	bool is_plane(void) const
	{
		typedef math::value_miss_comparer<val_type> cmper_type;
		return (cmper_type(a) != 0 || cmper_type(b) != 0 || cmper_type(c) != 0); 
	}

	template<typename OVal>
	bool is_in_it(const math::vector3d<OVal, Base>& pos) const
	{
		typedef math::value_miss_comparer<val_type> cmper_type;
		return cmper_type(a * pos.x + b * pos.y + c * pos.z + d) == 0;
	}

	//bool is_in_it(const math::vector3d<val_type, Base>& pos) const
	//{
	//	typedef math::value_miss_comparer<val_type> cmper_type;
	//	return cmper_type(a * pos.x + b * pos.y + c * pos.z + d) == 0;
	//}

	template<typename OVal>
	typename upper_float<val_type>::value_type length(const math::vector3d<OVal, Base>& right) const
	{
		typedef typename upper_float<val_type>::value_type rst_type;
		rst_type rst = 0;

		for(size_type i = 0, isize = E_LENGTH - 1; i != isize; ++i)
		{
			rst += static_cast<rst_type>((*this)[i] * right[i]);
		}
		rst += static_cast<rst_type>(d);

		rst = rst * rst;
		return math::sqrt(rst / (a * a + b * b + c * c));
	}

	//typename upper_float<val_type>::value_type length(const math::vector3d<val_type, Base>& right) const
	//{
	//	typedef typename upper_float<val_type>::value_type rst_type;
	//	rst_type rst = 0;

	//	for(size_type i = 0, isize = E_LENGTH - 1; i != isize; ++i)
	//	{
	//		rst += static_cast<rst_type>((*this)[i] * right[i]);
	//	}
	//	rst += static_cast<rst_type>(d);

	//	rst = rst * rst;
	//	return math::sqrt(rst / (a * a + b * b + c * c));
	//}

	const math::vector3d<val_type, Base> normal(void) const
	{
		typedef math::vector3d<val_type, Base> rst_type;
		return rst_type(a, b, c);
	}
		
public:
	val_type& a;
	val_type& b;
	val_type& c;
	val_type& d;
};
} // geometry
} // namespace yggr

#endif //__YGGR_GEOMETRY_GEO_PLANE_HPP__