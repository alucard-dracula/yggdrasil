//geo_rectangle.hpp

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

#ifndef __YGGR_GEOMETRY_RECTANGLE_HPP__
#define __YGGR_GEOMETRY_RECTANGLE_HPP__

//#include <yggr/geometry/geo_vector.hpp>
//#include <yggr/geometry/geo_line_segment.hpp>
#include <boost/array.hpp>
#include <yggr/math/value_miss_comparer.hpp>
#include <cassert>
#include <boost/mpl/assert.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/comparison.hpp>

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/less.hpp>

#include <boost/serialization/access.hpp>
#include <yggr/serialization/array.hpp>

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
			template<typename _Val, std::size_t> class Base = boost::array>
class geo_rectangle : public Base<Position, 2>
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

	typedef Base<pos_type, E_LENGTH> base_type;
	typedef geo_rectangle this_type;

public:
	geo_rectangle(void)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
	}

	template<typename OPos>
	geo_rectangle(const OPos& min, const OPos& max)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		min_pos = min;
		max_pos = max;
		swap();
	}

	geo_rectangle(const pos_type& min, const pos_type& max)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
		min_pos = min;
		max_pos = max;
		swap();
	}

	template<typename OPos, typename Alloc,
				template<typename _Val, typename _Alloc> class Container>
	geo_rectangle(const Container<OPos, Alloc>& right)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		typedef Container<OPos, Alloc> container_type;
		typedef typename container_type::const_iterator container_citer_type;

		if(right.size() < 2)
		{
			return;
		}

		container_citer_type i = right.begin();

		pos_type min = *i, max = *i;
		++i;
		for(container_citer_type isize = right.end(); i != isize; ++i)
		{
			for(size_type j = 0, jsize = pos_type::s_size(); j != jsize; ++j)
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

//	template<typename Alloc,
//				template<typename _Val, typename _Alloc> class Container>
//	geo_rectangle(const Container<pos_type, Alloc>& right)
//		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
//	{
//		typedef Container<pos_type, Alloc> continer_type;
//		typedef container_type::const_iterator container_citer_type;
//
//		if(right.size() < 2)
//		{
//			return;
//		}
//
//		container_citer_type i = right.begin();
//
//		pos_type min = *i, max = *i;
//		++i;
//		for(container_citer_type isize = right.end(); i != isize; ++i)
//		{
//			for(size_type j = 0, jsize = pos_type::s_size(); j != jsize; ++j)
//			{
//				if((*i)[j] < min[j])
//				{
//					min[j] = (*i)[j];
//					continue;
//				}
//
//				if((*i)[j] > max[j])
//				{
//					max[j] = (*i)[j];
//				}
//			}
//		}
//
//		min_pos = min;
//		max_pos = max;
//	}

	template<typename OPos>
	geo_rectangle(const geo_rectangle<OPos, Base>& right)
		: min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
		min_pos = right.min_pos;
		max_pos = right.max_pos;
	}

	geo_rectangle(const this_type& right)
		: base_type(right), min_pos(base_type::operator[](0)), max_pos(base_type::operator[](1))
	{
	}

	~geo_rectangle(void)
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

	void clear(void)
	{
		min_pos = max_pos = pos_type();
	}

	pos_type center(void) const
	{
		return min_pos + (max_pos - min_pos) / val_type(2);
	}

	pos_type dimension(void) const
	{
		return max_pos - min_pos;
	}

	template<size_type X, size_type Y>
	const pos_type vertex(void) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<X>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<Y>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<X>,
												boost::mpl::size_t<0> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<Y>,
												boost::mpl::size_t<0> >));

		return pos_type((*this)[X][0], (*this)[Y][1]);
	}

	template<size_type X, size_type Y, size_type Z>
	const pos_type vertex(void) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<3> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<X>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<Y>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<Z>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<X>,
												boost::mpl::size_t<0> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<Y>,
												boost::mpl::size_t<0> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<Z>,
												boost::mpl::size_t<0> >));

		return pos_type((*this)[X][0], (*this)[Y][1], (*this)[Z][2]);
	}

	template<size_type Idx>
	const val_type length(void) const
	{
		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<Idx>,
												boost::mpl::size_t<pos_type::E_LENGTH> >));

		return max_pos[Idx] - min_pos[Idx];
	}

	template<size_type X, size_type Y, typename OPos>
	const pos_type is_vertex(const OPos& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<X>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<Y>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<X>,
												boost::mpl::size_t<0> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<Y>,
												boost::mpl::size_t<0> >));

		return vertex<X, Y>() == right;
	}

	template<size_type X, size_type Y, size_type Z, typename OPos>
	const pos_type is_vertex(const OPos& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<3> >));

		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<X>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<Y>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::less<boost::mpl::size_t<Z>,
												boost::mpl::size_t<2> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<X>,
												boost::mpl::size_t<0> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<Y>,
												boost::mpl::size_t<0> >));

		BOOST_MPL_ASSERT((boost::mpl::greater_equal<boost::mpl::size_t<Z>,
												boost::mpl::size_t<0> >));

		return vertex<X, Y, Z>() == right;
	}

	bool is_rectangle(void) const
	{
		pos_type check = max_pos - min_pos;

		return (pos_type::s_size() - check.zero_count()) > 1;
	}

	bool is_box(void) const
	{
		if(pos_type::s_size() != 3)
		{
			return false;
		}

		pos_type check = max_pos - min_pos;

		return check.zero_count() == 0;
	}

	bool is_line_segment(void) const
	{
		pos_type check = max_pos - min_pos;
		return (pos_type::s_size() - check.zero_count()) == 1;

	}

	template<typename OPos>
	bool is_in_it(const OPos& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		typedef math::value_miss_comparer<val_type> cmper_type;
		for(size_type i = 0, isize = pos_type::s_size(); i != isize; ++i)
		{
			cmper_type cmper(right[i]);

			if(cmper < min_pos[i] || cmper[i] > max_pos[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename OPos>
	bool is_in_it(const geo_rectangle<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		for(size_type i = 0, isize = pos_type::s_size(); i != isize; ++i)
		{
			if(right.min_pos[i] > max_pos[i] || right.max_pos[i] < min_pos[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename OPos>
	bool is_same_range(const geo_rectangle<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		return max_pos - min_pos == right.max_pos - right.min_pos;
	}

	template<typename OPos>
	bool operator==(const geo_rectangle<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		return min_pos == right.min_pos && max_pos == right.max_pos;
	}

	template<typename OPos>
	bool is_encircling(const geo_rectangle<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		return (!is_same_rect(right)) && is_in_it(right.min_pos) && is_in_it(right.max_pos);
	}

	template<typename Matrix>
	const this_type operator*(const Matrix& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
												boost::mpl::size_t<Matrix::E_MAX_RANK> >));
		return this_type(min_pos * right, max_pos * right);
	}

	template<typename Matrix>
	this_type& operator*=(const Matrix& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
												boost::mpl::size_t<Matrix::E_MAX_RANK> >));
		min_pos *= right;
		max_pos *= right;
		return *this;
	}

private:

	void swap(void)
	{
		for(size_type i = 0, isize = pos_type::s_size(); i != isize; ++i)
		{
			if(min_pos[i] > max_pos[i])
			{
				std::swap(min_pos[i], max_pos[i]);
			}
		}
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(min_pos);
		ar & YGGR_SERIALIZE_NVP(max_pos);
	}

public:
	pos_type& min_pos;
	pos_type& max_pos;
};

} // namespace geometry
} // namespace yggr

#endif // __YGGR_GEOMETRY_RECTANGLE_HPP__
