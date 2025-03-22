//geo_plane.hpp

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

#ifndef __YGGR_GEOMETRY_GEO_PLANE_HPP__
#define __YGGR_GEOMETRY_GEO_PLANE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/container/array.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/type_traits/operator_result.hpp>

#include <yggr/math/sign.hpp>
#include <yggr/math/sqrt.hpp>
#include <yggr/math/flaw_comparer.hpp>
#include <yggr/math/vector3d.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <algorithm>

namespace yggr
{

namespace math
{

template<typename Val, 
			template <typename _Val, std::size_t> class Base>
class vector3d;

template<typename Val, 
			template <typename _Val, std::size_t> class Base>
class geo_plane;

} // namespace math

namespace geometry
{

template<typename Val, 
			template <typename _Val, std::size_t> class Base = ::yggr::array>
class geo_plane : public Base<typename upper_signed<Val>::type, 4> 
{
public:
	typedef typename upper_signed<Val>::type val_type;
	typedef val_type value_type;

public:
	YGGR_STATIC_CONSTANT(std::size_t, E_LENGTH = 4);
	YGGR_STATIC_CONSTANT(std::size_t, E_ARRAY_LENGTH = E_LENGTH);
	
	typedef Base<val_type, E_LENGTH> base_type;

private:
	typedef geo_plane this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	geo_plane(void)
		: a(base_type::operator[](0)), b(base_type::operator[](1)), 
			c(base_type::operator[](2)), d(base_type::operator[](3))
	{
	}

	
	template<typename OVal1, typename OVal2,
				template<typename _Val, std::size_t> class OBase1,
				template<typename _Val, std::size_t> class OBase2 >
	geo_plane(const math::vector3d<OVal1, OBase1>& pos, const math::vector3d<OVal2, OBase2>& vec)
		: a(base_type::operator[](0)), b(base_type::operator[](1)), 
			c(base_type::operator[](2)), d(base_type::operator[](3))
	{
		typedef math::vector3d<OVal1, OBase1> pos_type;

		a = static_cast<val_type>(vec.x);
		b = static_cast<val_type>(vec.y);
		c = static_cast<val_type>(vec.z);
		d = static_cast<val_type>(-(pos_type(pos, 0).dot(vec)));
	}

	// cross_line or parallel_line or line_and_a_line_out_point please use three points create 

	template<typename OVal1, typename OVal2, typename OVal3,
				template<typename _Val, std::size_t> class OBase1,
				template<typename _Val, std::size_t> class OBase2,
				template<typename _Val, std::size_t> class OBase3 >
	geo_plane(const math::vector3d<OVal1, OBase1>& pos1,
				const math::vector3d<OVal2, OBase2>& pos2, 
				const math::vector3d<OVal3, OBase3>& pos3)
		: a(base_type::operator[](0)), b(base_type::operator[](1)),
			c(base_type::operator[](2)), d(base_type::operator[](3))
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

	geo_plane(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right)),
			a(base_type::operator[](0)), b(base_type::operator[](1)),
			c(base_type::operator[](2)), d(base_type::operator[](3))
	{
	}

	geo_plane(const base_type& right)
		: base_type(right),
			a(base_type::operator[](0)), b(base_type::operator[](1)),
			c(base_type::operator[](2)), d(base_type::operator[](3))
	{
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	geo_plane(const geo_plane<OVal, OBase>& right)
		: a(base_type::operator[](0)), b(base_type::operator[](1)), 
			c(base_type::operator[](2)), d(base_type::operator[](3))
	{
		(*this)[0] = static_cast<val_type>(right[0]);
		(*this)[1] = static_cast<val_type>(right[1]);
		(*this)[2] = static_cast<val_type>(right[2]);
		(*this)[3] = static_cast<val_type>(right[3]);
	}

	geo_plane(BOOST_RV_REF(this_type) right)
		: base_type(right), 
			a(base_type::operator[](0)), b(base_type::operator[](1)), 
			c(base_type::operator[](2)), d(base_type::operator[](3))
	{
	}

	geo_plane(const this_type& right)
		: base_type(right), 
			a(base_type::operator[](0)), b(base_type::operator[](1)), 
			c(base_type::operator[](2)), d(base_type::operator[](3))
	{
	}

	~geo_plane(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& base = *this;
		const base_type& right_cref = right;
		if(&base == &right_cref) 
		{
			return *this;
		}

#ifdef _DEBUG
		val_type* p = &base[0];
#endif //_DEBUG

		std::copy(boost::begin(right_cref), boost::end(right_cref), boost::begin(base));

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		base_type& base = *this;
		if(&base == &right)
		{
			return *this;
		}

#ifdef _DEBUG
		val_type* p = &base[0];
#endif //_DEBUG

		std::copy(boost::begin(right), boost::end(right), boost::begin(base));

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	this_type& operator=(const geo_plane<OVal, Base>& right)
	{
		a = static_cast<val_type>(right.a);
		b = static_cast<val_type>(right.b);
		c = static_cast<val_type>(right.c);
		d = static_cast<val_type>(right.d);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		a = right.a;
		b = right.b;
		c = right.c;
		d = right.d;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}
		a = right.a;
		b = right.b;
		c = right.c;
		d = right.d;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(a, right.a);
		yggr::swap(b, right.b);
		yggr::swap(c, right.c);
		yggr::swap(d, right.d);
	}

public:
	inline static size_type size(void)
	{
		return E_LENGTH;
	}

	inline static size_type array_size(void)
	{
		return E_ARRAY_LENGTH;
	}

	inline this_type& normalize(void)
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

	geo_plane<typename upper_float<val_type>::type, Base> inline
		get_normalized_plane(void) const
	{
		typedef geo_plane<typename upper_float<val_type>::type, Base> rst_type;
		rst_type rst(*this);
		rst.normalize();
		return rst;
	}

	inline bool is_plane(void) const
	{
		typedef math::flaw_comparer<val_type> cmper_type;
		return (cmper_type(a) != 0 || cmper_type(b) != 0 || cmper_type(c) != 0); 
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool is_in_it(const math::vector3d<OVal, OBase>& pos) const
	{
		typedef typename multiplies_operator_result<val_type, OVal>::type cmp_val_type;
		typedef math::flaw_comparer<cmp_val_type> cmper_type;
		return cmper_type(a * pos.x + b * pos.y + c * pos.z + d) == 0;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	typename upper_float<val_type>::type 
		length(const math::vector3d<OVal, OBase>& right) const
	{
		typedef typename upper_float<val_type>::type rst_type;
		rst_type rst = 0;

		for(size_type i = 0, isize = E_LENGTH - 1; i != isize; ++i)
		{
			rst += static_cast<rst_type>((*this)[i] * right[i]);
		}
		rst += static_cast<rst_type>(d);

		rst = rst * rst;
		return ::yggr::math::sqrt(rst / (a * a + b * b + c * c));
	}

	inline math::vector3d<val_type, Base> normal(void) const
	{
		typedef math::vector3d<val_type, Base> rst_type;
		return rst_type(a, b, c, val_type());
	}
		
public:
	val_type& a;
	val_type& b;
	val_type& c;
	val_type& d;
};

} // geometry
} // namespace yggr

namespace yggr
{
namespace geometry
{
namespace swap_support
{

template<typename Val, template <typename _Val, std::size_t> class Base> inline
void swap(geo_plane<Val, Base>& l, geo_plane<Val, Base>& r) 
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace geometry
} // namespace yggr

namespace std
{
	using ::yggr::geometry::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::geometry::swap_support::swap;
} // namespace boost

#endif //__YGGR_GEOMETRY_GEO_PLANE_HPP__