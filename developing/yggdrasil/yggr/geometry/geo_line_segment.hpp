//geo_line_segment.hpp

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

#ifndef __YGGR_GEOMETRY_GEOMETRY_LINE_SEGMENT_HPP__
#define __YGGR_GEOMETRY_GEOMETRY_LINE_SEGMENT_HPP__

#ifdef _MSC_VER
#	include <yggr/support/max_min_undef.ipp>
#endif // _MSC_VER

#include <yggr/base/yggrdef.h>
#include <yggr/math/sqrt.hpp>
#include <yggr/geometry/geo_line.hpp>
#include <complex>
#include <utility>
#include <functional>
#include <boost/array.hpp>
#include <cassert>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/comparison.hpp>

#include <boost/mpl/equal_to.hpp>

#include <boost/serialization/access.hpp>
#include <yggr/serialization/array.hpp>


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

namespace geometry
{
template<typename Position,
	template<typename _Val, std::size_t> class Array = boost::array,
	template<typename _Val,
				template<typename __Val, std::size_t> class _Base> class Base = geo_line>
class geo_line_segment : public Base<Position, Array>
{
public:

	typedef Position pos_type;
	typedef typename pos_type::val_type val_type;

private:
	enum
	{
		E_LENGTH = 2,
		E_compile_u32 = 0xffffffff
	};

	typedef geo_line<pos_type, Array> base_type;
	typedef geo_line_segment this_type;

public:

	geo_line_segment(void)
	{
	}

	template<typename OPos>
	geo_line_segment(const OPos& src, const OPos& dst)
		: base_type(src, dst)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
	}

	geo_line_segment(const pos_type& src, const pos_type& dst)
		: base_type(src, dst)
	{
	}

	template<typename OPos>
	geo_line_segment(const geo_line_segment<OPos, Array, Base>& right)
		: base_type(right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
	}

	geo_line_segment(const this_type& right)
		: base_type(right)
	{
	}

	~geo_line_segment(void)
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

	inline const base_type& take_for_line(void) const
	{
		return *this;
	}

	template<typename OPos>
	geo_line_segment& operator=(const geo_line_segment<OPos, Array, Base>& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		base_type::src_pos = right.src_pos;
		base_type::dst_pos = right.dst_pos;
		return *this;
	}

	template<typename OPos>
	bool operator==(const Base<OPos, Array>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		base_type& base = *this;
		return base == right;
	}

	template<typename OPos>
	bool operator==(const geo_line_segment<OPos, Array, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		return (base_type::src_pos == right.src_pos && base_type::dst_pos == right.dst_pos)
				|| (base_type::src_pos == right.dst_pos && base_type::dst_pos == right.src_pos);
	}

	template<typename OPos>
	bool operator!=(const Base<OPos, Array>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		base_type& base = *this;
		return base != right;
	}

	template<typename OPos>
	bool operator!=(const geo_line_segment<OPos, Array, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		return !(this->operator==(right));
	}

	const pos_type center_pos(void) const
	{
		return (base_type::src_pos + base_type::dst_pos) / val_type(2);
	}

	typename upper_float<val_type>::value_type length(void) const
	{
		return (base_type::dst_pos - base_type::src_pos).length();
	}

	val_type length_sq(void) const
	{
		return (base_type::dst_pos - base_type::src_pos).length_sq();
	}

	template<typename OPos>
	std::pair<pos_type, bool> cross_point(const Base<OPos, Array>& right) const // Ôö¼ÓAABBOXÅÐ¶¨
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		std::pair<pos_type, bool> rst = base_type::cross_point(right);
		if(rst.second)
		{
			rst.second = is_in_it(rst.first);
		}

		return rst;
	}

	template<typename OPos>
	bool is_in_it(const OPos& pos) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		typedef math::value_miss_comparer<val_type> cmper_type;

		if(!base_type::is_line())
		{
			return false;
		}

		val_type min_val = val_type();
		val_type max_val = val_type();

		for(size_type i = 0, isize = pos_type::s_size(); i != isize; ++i)
		{
			min_val = std::min(base_type::src_pos[i], base_type::dst_pos[i]);
			max_val = std::max(base_type::src_pos[i], base_type::dst_pos[i]);

			if(cmper_type(pos[i]) < min_val || cmper_type(pos[i]) > max_val)
			{
				return false;
			}
		}

		return base_type::is_in_it(pos);
	}

	//template<typename OPos>
	//bool is_in_it(const Base<OPos, Array>& right) const
	//{
	//	BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
	//											boost::mpl::size_t<OPos::E_LENGTH> >));

	//	return base_type::is_in_it(right);
	//}

	template<typename OPos>
	bool is_in_it(const geo_line_segment<OPos, Array, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		return is_in_it(right.src_pos) && is_in_it(right.dst_pos);
	}

	template<typename OPos>
	bool is_superposition_line_segment(const geo_line_segment<OPos, Array, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		if(!(base_type::is_line() && right.is_line()))
		{
			return false;
		}

		return is_parallel_line(right)
				&& ((is_in_it(right.src_pos) || is_in_it(right.dst_pos))
				||(right.is_in_it(base_type::src_pos) || right.is_in_it(base_type::dst_pos)));

	}

	template<typename OPos>
	this_type get_superposition_line_segment(const geo_line_segment<OPos, Array, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		typedef boost::array<pos_type, 4> pos_array_type;

		typedef struct _tag_pos_cmper_less
                    : public std::binary_function<pos_type, pos_type, bool>
		{
		private:
			typedef typename upper_signed<val_type>::value_type signed_type;

		public:
			_tag_pos_cmper_less(const this_type& right)
				: _idx(), _val_1(), _val_mnp_sign()
			{
				typedef math::value_miss_comparer<signed_type> cmper_type;

				pos_type mnp_vec = right.src_to_dst();
				for(size_type i = 0, isize = pos_type::s_size(); i != isize; ++i)
				{
					if(cmper_type(mnp_vec[i]) != val_type())
					{
						_idx = i;
						_val_1 = static_cast<signed_type>(right.src_pos[i]);
						_val_mnp_sign = math::sign(mnp_vec[i]);
						break;
					}
				}

				assert(cmper_type(_val_mnp_sign) != signed_type());
			}

			_tag_pos_cmper_less(const _tag_pos_cmper_less& right)
				: _idx(right._idx), _val_mnp_sign(right._val_mnp)
			{
			}

			bool operator()(const pos_type& left, const pos_type& right) const
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
		} pos_cmper_less_type;

		if(!is_superposition_line_segment(right))
		{
			return this_type();
		}

		pos_array_type pos_arr = {base_type::src_pos, base_type::dst_pos, right.src_pos, right.dst_pos};
		std::sort(pos_arr.begin(). pos_arr.end(), pos_cmper_less_type(*this));

		return this_type(pos_arr[1], pos_arr[2]);
	}


	template<typename Matrix>
	const this_type operator*(const Matrix& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
												boost::mpl::size_t<Matrix::E_MAX_RANK> >));
		return this_type(base_type::src_pos * right, base_type::dst_pos * right);
	}

	template<typename Matrix>
	this_type& operator*=(const Matrix& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
												boost::mpl::size_t<Matrix::E_MAX_RANK> >));
		base_type::src_pos *= right;
		base_type::dst_pos *= right;
		return *this;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		base_type::serialize(ar, version);
	}
};
} // namespace geometry
} // namespace yggr

#endif //__YGGR_GEOMETRY_GEOMETRY_LINE_SEGMENT_HPP__
