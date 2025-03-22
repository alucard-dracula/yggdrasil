//geo_rectangle_t.hpp

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

#ifndef __YGGR_GEOMETRY_RECTANGLE_T_HPP__
#define __YGGR_GEOMETRY_RECTANGLE_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/math/flaw_comparer.hpp>
#include <yggr/container/array.hpp>
#include <yggr/container_ex/is_container.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/array.hpp>

#include <boost/ref.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/less.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <cassert>

namespace yggr
{

namespace geometry
{

template<typename Position,
		template<typename _Val, std::size_t> class Base>
class geo_rectangle;

} // namespace geometry

namespace geometry
{

template<typename Position,
			template<typename _Val, std::size_t> class Base = ::yggr::array>
class geo_rectangle 
	: public Base<Position, 2>
{
public:
	typedef Position pos_type;
	typedef pos_type point_type;
	typedef pos_type value_type;

public:
	typedef typename point_type::value_type pos_value_type;
	typedef pos_value_type point_value_type; 

protected:
	YGGR_STATIC_CONSTANT(std::size_t, E_LENGTH = 2);

	typedef Base<pos_type, E_LENGTH> base_type;

private:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef geo_rectangle this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	geo_rectangle(void)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
	}

	template<typename OPos1, typename OPos2>
	geo_rectangle(const OPos1& min, const OPos2& max)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
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

		min_pos = min;
		max_pos = max;
		prv_swap();
	}

	template<typename Container> explicit
	geo_rectangle(const Container right,
					typename
						boost::enable_if
						<
							container_ex::is_container<Container>,
							sfinae_type
						>::type tag = 0)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
		typedef Container container_type;
		typedef typename range_ex::range_const_iterator_ex<container_type>::type container_citer_type;

		if(right.size() < 2)
		{
			return;
		}

		container_citer_type i = boost::begin(right);

		pos_type min = *i, max = *i;
		++i;
		for(container_citer_type isize = boost::end(right); i != isize; ++i)
		{
			for(size_type j = 0, jsize = pos_type::size(); j != jsize; ++j)
			{
				if((*i)[j] < min[j])
				{
					min[j] = (*i)[j];
					continue;
				}

				if((*i)[j] > max[j])
				{
					max[j] = (*i)[j];
				}
			}
		}

		min_pos = min;
		max_pos = max;
	}

	template<typename OPos, template<typename _Val, std::size_t> class OBase>
	geo_rectangle(const geo_rectangle<OPos, OBase>& right)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
		min_pos = right.min_pos;
		max_pos = right.max_pos;
	}

	geo_rectangle(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right))), 
			min_pos(base_type::operator[](0)), 
			max_pos(base_type::operator[](1))
	{
	}

	geo_rectangle(const this_type& right)
		: base_type(right), 
			min_pos(base_type::operator[](0)), 
			max_pos(base_type::operator[](1))
	{
	}

	~geo_rectangle(void)
	{
	}

public:
	template<typename OPos, template<typename _Val, std::size_t> class OBase>
	this_type& operator=(const geo_rectangle<OPos, OBase>& right)
	{
		min_pos = right.min_pos;
		max_pos = right.max_pos;

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(min_pos, boost::move(right.min_pos));
		copy_or_move_or_swap(max_pos, boost::move(right.max_pos));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		min_pos = right.min_pos;
		max_pos = right.max_pos;
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

		min_pos.swap(right.min_pos);
		max_pos.swap(right.max_pos);
	}

public:
	inline static size_type size(void)
	{
		return E_LENGTH;
	}

	inline void clear(void)
	{
		min_pos = max_pos = pos_type();
	}

	inline pos_type center(void) const
	{
		return min_pos + (max_pos - min_pos) / pos_value_type(2);
	}

	inline pos_type dimension(void) const
	{
		return max_pos - min_pos;
	}

	/*
		0, 0 ------- 1, 0
			 |	   |
			 |	   |
		0, 1 ------- 1, 1
	*/
	template<size_type X, size_type Y> inline
	pos_type vertex(void) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<X>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<Y>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<X>,
				boost::mpl::size_t<0> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<Y>,
				boost::mpl::size_t<0> 
			>));

		return pos_type((*this)[X][0], (*this)[Y][1], (*this)[X][2]);
	}

	template<size_type X, size_type Y, size_type Z> inline
	pos_type vertex(void) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<3> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<X>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<Y>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<Z>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<X>,
				boost::mpl::size_t<0> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<Y>,
				boost::mpl::size_t<0> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<Z>,
				boost::mpl::size_t<0> 
			>));

		return pos_type((*this)[X][0], (*this)[Y][1], (*this)[Z][2], (*this)[X][3]);
	}

	template<size_type Idx> inline
	pos_value_type length(void) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<Idx>,
				boost::mpl::size_t<pos_type::E_LENGTH> 
			>));

		return max_pos[Idx] - min_pos[Idx];
	}

	template<size_type X, size_type Y, typename OPos> inline
	bool is_vertex(const OPos& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<X>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<Y>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<X>,
				boost::mpl::size_t<0> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<Y>,
				boost::mpl::size_t<0> 
			>));

		return this_type::vertex<X, Y>() == right;
	}

	template<size_type X, size_type Y, size_type Z, typename OPos> inline
	bool is_vertex(const OPos& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<3> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<X>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<Y>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::less
			<
				boost::mpl::size_t<Z>,
				boost::mpl::size_t<2> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<X>,
				boost::mpl::size_t<0> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<Y>,
				boost::mpl::size_t<0> 
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::greater_equal
			<
				boost::mpl::size_t<Z>,
				boost::mpl::size_t<0> 
			>));

		return vertex<X, Y, Z>() == right;
	}

	inline bool is_rectangle(void) const
	{
		return (pos_type::size() - (max_pos - min_pos).zero_count()) > 1;
	}

	inline bool is_box(void) const
	{
		return 
			pos_type::size() == 3
			&& (max_pos - min_pos).zero_count() == 0;
	}

	inline bool is_line_segment(void) const
	{
		return (pos_type::size() - (max_pos - min_pos).zero_count()) == 1;
	}

	template<typename OPos>
	bool is_in_it(const OPos& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		typedef math::flaw_comparer<pos_value_type> cmper_type;
		for(size_type i = 0, isize = pos_type::size(); i != isize; ++i)
		{
			cmper_type cmper(right[i]);

			if(cmper < min_pos[i] || cmper > max_pos[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename OPos, template<typename _Val, std::size_t> class OBase>
	bool is_in_it(const geo_rectangle<OPos, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		for(size_type i = 0, isize = pos_type::size(); i != isize; ++i)
		{
			if(right.min_pos[i] > max_pos[i] || right.max_pos[i] < min_pos[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename OPos, template<typename _Val, std::size_t> class OBase> inline
	bool is_same_range(const geo_rectangle<OPos, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		return (max_pos - min_pos) == (right.max_pos - right.min_pos);
	}

public:
	template<typename OPos, template<typename _Val, std::size_t> class OBase> inline
	bool equal_to(const geo_rectangle<OPos, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		return min_pos == right.min_pos && max_pos == right.max_pos;
	}

	inline bool equal_to(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (min_pos == right.min_pos && max_pos == right.max_pos);
	}

	template<typename OPos, template<typename _Val, std::size_t> class OBase> inline
	bool not_equal_to(const geo_rectangle<OPos, OBase>& right) const
	{
		return !this_type::equal_to(right);
	}

public:
	template<typename OPos, template<typename _Val, std::size_t> class OBase> inline
	bool is_encircling(const geo_rectangle<OPos, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH> 
			>));

		return (!is_same_rect(right)) && is_in_it(right.min_pos) && is_in_it(right.max_pos);
	}

private:
	void prv_swap(void)
	{
		for(size_type i = 0, isize = pos_type::size(); i != isize; ++i)
		{
			if(min_pos[i] > max_pos[i])
			{
				yggr::swap(min_pos[i], max_pos[i]);
			}
		}
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(min_pos);
		ar & YGGR_SERIALIZATION_NVP(max_pos);
	}

public:
	pos_type& min_pos;
	pos_type& max_pos;
};

// non-member foo

// equal_to
template<typename PosL, typename PosR, 
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class BaseR> inline
bool operator==(const geo_rectangle<PosL, BaseL>& l, const geo_rectangle<PosR, BaseR>& r)
{
	return l.equal_to(r);
}

template<typename PosL, typename PosR, 
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class BaseR> inline
bool operator!=(const geo_rectangle<PosL, BaseL>& l, const geo_rectangle<PosR, BaseR>& r) 
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
		template<typename _Val, std::size_t> class Base> inline
void swap(geo_rectangle<Position, Base>& l, geo_rectangle<Position, Base>& r) 
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

#endif // __YGGR_GEOMETRY_RECTANGLE_T_HPP__
