//geo_line_segment_t.hpp

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

#ifndef __YGGR_GEOMETRY_GEOMETRY_LINE_SEGMENT_T_HPP__
#define __YGGR_GEOMETRY_GEOMETRY_LINE_SEGMENT_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/container/array.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/math/sqrt.hpp>
#include <yggr/geometry/geo_line.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/array.hpp>

#include <boost/ref.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/equal_to.hpp>

#include <complex>
#include <utility>
#include <functional>
#include <cassert>

namespace yggr
{
namespace geometry
{
template<typename Position,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val,
						template<typename __Val, std::size_t> class _Base> class Base>
class geo_line_segment;

} // namespace geometry
} // namespace yggr

namespace yggr
{
namespace geometry
{
namespace detail
{

template<typename LineSegment>
struct pos_cmper_less
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	: public std::binary_function<typename LineSegment::pos_type, typename LineSegment::pos_type, bool>
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
{
public:
	typedef LineSegment line_segment_type;
	typedef typename line_segment_type::pos_type pos_type;
	typedef typename line_segment_type::pos_value_type pos_value_type;

	typedef const pos_type& first_argument_type;
	typedef const pos_type& second_argument_type;
	typedef bool result_type;

private:
	typedef typename upper_signed<pos_value_type>::type signed_type;

private:
	typedef pos_cmper_less this_type;

public:
	pos_cmper_less(const line_segment_type& right)
		: _idx(), _val_1(), _val_mnp_sign()
	{
		typedef math::flaw_comparer<signed_type> cmper_type;

		pos_type mnp_vec = right.src_to_dst();
		for(size_type i = 0, isize = pos_type::size(); i != isize; ++i)
		{
			if(cmper_type(mnp_vec[i]) != pos_value_type())
			{
				_idx = i;
				_val_1 = static_cast<signed_type>(right.src_pos[i]);
				_val_mnp_sign = math::sign(mnp_vec[i]);
				break;
			}
		}

		assert(cmper_type(_val_mnp_sign) != signed_type());
	}

	pos_cmper_less(const this_type& right)
		: _idx(right._idx), _val_1(right._val_1), _val_mnp_sign(right._val_mnp_sign)
	{
	}

public:
	inline bool operator()(const pos_type& left, const pos_type& right) const
	{
		// t1 = (val_left - val1) / val_mnp;
		// t2 = (val_right - val1) / val_mnp;
		// t1_val_mnp == t2_val_mnp => less comparer need sign(val_mnp)

		signed_type t1 = _val_mnp_sign * (static_cast<signed_type>(left[_idx]) - _val_1);
		signed_type t2 = _val_mnp_sign * (static_cast<signed_type>(right[_idx]) - _val_1);
		return t1 < t2;
	}

private:
	size_type _idx;
	signed_type _val_1;
	signed_type _val_mnp_sign;
};

} // namespace detail
} // namespace geometry
} // namespace yggr

namespace yggr
{
namespace geometry
{

template<typename Position,
			template<typename _Val, std::size_t> class Array = ::yggr::array,
			template<typename _Val,
						template<typename __Val, std::size_t> class _Base> class Base = geo_line>
class geo_line_segment 
	: public Base<Position, Array>
{
public:
	typedef Position pos_type;
	typedef pos_type point_type;
	typedef pos_type value_type;

public:
	typedef typename point_type::value_type pos_value_type;
	typedef pos_value_type point_value_type; 

public:
	YGGR_STATIC_CONSTANT(std::size_t, E_LENGTH = 2);
	typedef geo_line<pos_type, Array> base_type;

private:
	typedef geo_line_segment this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	geo_line_segment(void)
	{
	}

	template<typename OPos1, typename OPos2>
	geo_line_segment(const OPos1& src, const OPos2& dst)
		: base_type(src, dst)
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos1::E_LENGTH> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos2::E_LENGTH> 
			>));
	}

	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase>
	geo_line_segment(const geo_line_segment<OPos, OArray, OBase>& right)
		: base_type(right)
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));
	}

	geo_line_segment(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	geo_line_segment(const this_type& right)
		: base_type(right)
	{
	}

	~geo_line_segment(void)
	{
	}

public:
	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase>
	this_type& operator=(const geo_line_segment<OPos, OArray, OBase>& right)
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		base_type::src_pos = right.src_pos;
		base_type::dst_pos = right.dst_pos;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		base_type::operator=(boost::move(static_cast<base_type&>(right)));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		base_type::operator=(right);
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
		base_type::swap(right);
	}

public:
	inline static size_type size(void)
	{
		return E_LENGTH;
	}

	inline const base_type& take_for_line(void) const
	{
		return *this;
	}

public:
	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	bool equal_to(const OBase<OPos, OArray>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		return base_type::equal_to(right);
	}

	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	bool equal_to(const geo_line_segment<OPos, OArray, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		return 
			(base_type::src_pos == right.src_pos && base_type::dst_pos == right.dst_pos)
			|| (base_type::src_pos == right.dst_pos && base_type::dst_pos == right.src_pos);
	}

	inline bool equal_to(const this_type& right) const
	{
		return 
			(this == boost::addressof(right))
			|| (base_type::src_pos == right.src_pos && base_type::dst_pos == right.dst_pos)
			|| (base_type::src_pos == right.dst_pos && base_type::dst_pos == right.src_pos);
	}

	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	bool not_equal_to(const OBase<OPos, OArray>& right) const
	{
		return !this_type::equal_to(right);
	}

	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	bool not_equal_to(const geo_line_segment<OPos, OArray, OBase>& right) const
	{
		return !this_type::equal_to(right);
	}

public:
	inline pos_type center_pos(void) const
	{
		return (base_type::src_pos + base_type::dst_pos) / pos_value_type(2);
	}

	inline typename upper_float<pos_value_type>::type length(void) const
	{
		return (base_type::dst_pos - base_type::src_pos).length();
	}

	inline pos_value_type length_sq(void) const
	{
		return (base_type::dst_pos - base_type::src_pos).length_sq();
	}

	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	std::pair<pos_type, bool> 
		cross_point(const OBase<OPos, OArray>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		std::pair<pos_type, bool> rst = base_type::cross_point(right);
		rst.second = rst.second && is_in_it(rst.first);
		
		return rst;
	}

	template<typename OPos> 
	bool is_in_it(const OPos& pos) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		typedef math::flaw_comparer<pos_value_type> cmper_type;

		if(!base_type::is_line())
		{
			return false;
		}

		pos_value_type min_val = pos_value_type();
		pos_value_type max_val = pos_value_type();

		for(size_type i = 0, isize = pos_type::size(); i != isize; ++i)
		{
			min_val = (std::min)(base_type::src_pos[i], base_type::dst_pos[i]);
			max_val = (std::max)(base_type::src_pos[i], base_type::dst_pos[i]);

			if(cmper_type(pos[i]) < min_val || cmper_type(pos[i]) > max_val)
			{
				return false;
			}
		}

		return base_type::is_in_it(pos);
	}

	template<typename OPos, 
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	bool is_in_it(const geo_line_segment<OPos, OArray, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		return is_in_it(right.src_pos) && is_in_it(right.dst_pos);
	}

	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	bool is_superposition_line_segment(const geo_line_segment<OPos, OArray, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		if(!(base_type::is_line() && right.is_line()))
		{
			return false;
		}

		return 
			base_type::is_parallel_line(right)
			&& ((is_in_it(right.src_pos) || is_in_it(right.dst_pos))
			||(right.is_in_it(base_type::src_pos) || right.is_in_it(base_type::dst_pos)));

	}

	template<typename OPos,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	this_type get_superposition_line_segment(const geo_line_segment<OPos, OArray, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		typedef ::yggr::array<pos_type, 4> pos_array_type;
		typedef detail::pos_cmper_less<this_type> pos_cmper_less_type;

		if(!is_superposition_line_segment(right))
		{
			return this_type();
		}

		pos_array_type pos_arr = {base_type::src_pos, base_type::dst_pos, right.src_pos, right.dst_pos};
		std::sort(boost::begin(pos_arr), boost::end(pos_arr), pos_cmper_less_type(*this));

		return this_type(pos_arr[1], pos_arr[2]);
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive> inline
	void serialize(Archive& ar, const u32 version)
	{
		base_type::serialize(ar, version);
	}
};

template<typename Position, 
		template<typename _Val, std::size_t> class Array, 
		template<typename _Val, 
					template<typename __Val, std::size_t> class _Base> class Base> inline 
yggr::geometry::geo_line_segment<Position, Array, Base>
	operator~(BOOST_RV_REF_BEG yggr::geometry::geo_line_segment<Position, Array, Base> BOOST_RV_REF_END val)
{
	typedef yggr::geometry::geo_line_segment<Position, Array, Base> value_type;
	value_type tmp(boost::move(val));
	tmp.swap_src_dst();
	return tmp;
}

template<typename Position, 
		template<typename _Val, std::size_t> class Array, 
		template<typename _Val, 
					template<typename __Val, std::size_t> class _Base> class Base> inline 
yggr::geometry::geo_line_segment<Position, Array, Base>
	operator~(const yggr::geometry::geo_line_segment<Position, Array, Base>& val)
{
	typedef yggr::geometry::geo_line_segment<Position, Array, Base> value_type;
	value_type tmp(val);
	tmp.swap_src_dst();
	return tmp;
}

template<typename PositionL, typename PositionR, 
		template<typename _Val, std::size_t> class ArrayL,
		template<typename _Val, std::size_t> class ArrayR,
		template<typename _Val,
					template<typename __Val, std::size_t> class _Base> class BaseL,
		template<typename _Val,
					template<typename __Val, std::size_t> class _Base> class BaseR> inline
bool operator==(const yggr::geometry::geo_line_segment<PositionL, ArrayL, BaseL>& l, 
				const yggr::geometry::geo_line_segment<PositionR, ArrayR, BaseR>& r) 
{ 
	return l.equal_to(r); 
}

template<typename PositionL, typename PositionR, 
		template<typename _Val, std::size_t> class ArrayL,
		template<typename _Val, std::size_t> class ArrayR,
		template<typename _Val,
					template<typename __Val, std::size_t> class _Base> class BaseL,
		template<typename _Val,
					template<typename __Val, std::size_t> class _Base> class BaseR> inline
bool operator!=(const yggr::geometry::geo_line_segment<PositionL, ArrayL, BaseL>& l, 
				const yggr::geometry::geo_line_segment<PositionR, ArrayR, BaseR>& r) 
{ 
	return !l.equal_to(r); 
}

} // namespace geometry
} // namespace yggr

namespace yggr
{
namespace geometry
{
namespace swap_support
{

template<typename Position,
	template<typename _Val, std::size_t> class Array,
	template<typename _Val,
				template<typename __Val, std::size_t> class _Base> class Base> inline 
void swap(geo_line_segment<Position, Array, Base>& l, geo_line_segment<Position, Array, Base>& r) 
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

#endif //__YGGR_GEOMETRY_GEOMETRY_LINE_SEGMENT_T_HPP__
