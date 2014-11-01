//geo_polygon.hpp

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

#ifndef __YGGR_GEOMETRY_GEO_POLYGON_HPP__
#define __YGGR_GEOMETRY_GEO_POLYGON_HPP__

#ifdef _MSC_VER
#	include <yggr/support/max_min_undef.ipp>
#endif // _MSC_VER

#include <cassert>

#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <complex>

#include <boost/tuple/tuple.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/equal_to.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/math/math.hpp>

#include <yggr/serialization/vector.hpp>

namespace yggr
{

namespace geometry
{

template<typename Position,
		template<typename _Val> class Alloc,
		template<typename _Val, typename _Alloc> class Base>
class geo_polygon;

};

namespace geometry
{

template<typename Position,
			template<typename _Val> class Alloc = std::allocator,
			template<typename _Val, typename _Alloc> class Base = std::vector>
class geo_polygon : public Base<Position, Alloc<Position> >
{
public:
	typedef Position pos_type;
	typedef typename pos_type::val_type val_type;
	typedef typename upper_float<typename pos_type::cross_result_type>::value_type prop_type;

private:
	typedef Base<pos_type, Alloc<pos_type> > base_type;
	typedef geo_polygon this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	geo_polygon(void)
	{
	}

	template<typename OPos>
	geo_polygon(const OPos& p1, const OPos& p2, const OPos& p3)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		base_type::push_back(pos_type(p1));
		base_type::push_back(pos_type(p2));
		base_type::push_back(pos_type(p3));

		gen_polygon_type();
	}

	geo_polygon(const pos_type& p1, const pos_type& p2, const pos_type& p3)
	{
		base_type::push_back(p1);
		base_type::push_back(p2);
		base_type::push_back(p3);

		gen_polygon_type();
	}

	template<typename OPos>
	geo_polygon(const OPos& p1, const OPos& p2, const OPos& p3, const OPos& p4)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		base_type::push_back(pos_type(p1));
		base_type::push_back(pos_type(p2));
		base_type::push_back(pos_type(p3));
		base_type::push_back(pos_type(p4));

		gen_polygon_type();
	}

	geo_polygon(const pos_type& p1, const pos_type& p2, const pos_type& p3, const pos_type& p4)
	{
		base_type::push_back(p1);
		base_type::push_back(p2);
		base_type::push_back(p3);
		base_type::push_back(p4);

		gen_polygon_type();
	}

	template<typename OPos>
	geo_polygon(const Base< OPos, Alloc<OPos> >& right)
		: base_type(right.begin(), right.end())
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		gen_polygon_type();
	}

	geo_polygon(const base_type& right)
		: base_type(right)
	{
		gen_polygon_type();
	}

	template<typename OPos>
	geo_polygon(const geo_polygon<OPos, Alloc, Base>& right)
		: base_type(right.begin(), right.end())
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		gen_polygon_type();
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	geo_polygon(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<base_type>(right)), _prop(right._prop)
	{
	}
#else
	geo_polygon(BOOST_RV_REF(this_type) right)
		: _prop(right._prop)
	{
		this_type& right_ref = *this;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	geo_polygon(const this_type& right)
		: base_type(right), _prop(right._prop)
	{
	}

	~geo_polygon(void)
	{
		clear();
	}

	void clear(void)
	{
		_prop = prop_type();
		base_type::clear();
	}

	const prop_type& prop(void) const
	{
		return _prop;
	}

	template<typename OPos>
	this_type& operator=(const Base< OPos, Alloc<OPos> >& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		clear();
		base_type::resize(right.size());
		std::copy(right.begin(), right.end(), base_type::begin());

		gen_polygon_type();
		return *this;
	}

	template<typename OPos>
	this_type& operator=(const geo_polygon<OPos, Alloc, Base>& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		clear();
		base_type::resize(right.size());
		std::copy(right.begin(), right.end(), base_type::begin());

		gen_polygon_type();
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
		_prop = right._prop;
#else
		this_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base = right;
		_prop = right._prop;
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right) {return;}

		base_type& base = *this;
		base.swap(right);
		std::swap(_prop, right._prop);
	}

	template<typename OPos>
	bool operator==(const geo_polygon<OPos, Alloc, Base>& right) const
	{
		return check_same(right);
	}

	template<typename OPos>
	bool operator!=(const geo_polygon<OPos, Alloc, Base>& right) const
	{
		return !check_same(right);
	}

	size_type edge_size(void) const
	{
		return base_type::size();
	}

	size_type points_size(void) const
	{
		return base_type::size();
	}

	size_type get_point_index(const pos_type& pos) const
	{
		typename base_type::const_iterator iter = std::find(base_type::begin(), base_type::end(), pos);
		return iter - base_type::begin();
	}

	const base_type& get_points(void) const
	{
		return *this;
	}

	const base_type get_points(const size_type& idx) const
	{
		size_type size = base_type::size();
		base_type pos_vt(size, pos_type());

		size_type cp_size = base_type::end() - (base_type::begin() + idx);
		std::copy(base_type::begin() + idx, base_type::end(), pos_vt.begin());

		if(cp_size == size)
		{
			return pos_vt;
		}

		std::copy(base_type::begin(), base_type::begin() + idx, pos_vt.begin() + cp_size);

		return pos_vt;
	}

	bool is_eligibility(void) const
	{
		return _prop != prop_type();
	}

	template<typename OPos>
	bool is_encircling(const geo_polygon<OPos, Alloc, Base>& right) const
	{
		typedef geo_polygon<OPos, Alloc, Base> right_type;
		typedef typename right_type::const_iterator right_citer_type;

		if(!(is_eligibility() && right.is_eligibility()))
		{
			return false;
		}

		for(right_citer_type i = right.begin(), isize = right.end(); i != isize; ++i)
		{
			if(!is_in_it(*i))
			{
				return false;
			}
		}

		return true;
	}

	template<typename OPos>
	bool is_in_it(const OPos& right) const
	{
		typedef math::value_miss_comparer<prop_type> cmper_type;
		typedef typename upper_float<OPos>::value_type now_pos_type;

		if(!is_eligibility())
		{
			return false;
		}

		typename base_type::const_iterator j;

		prop_type zero = prop_type();
		cmper_type cmper_zero(zero);
		cmper_type cmper(_prop);

		prop_type tmp;
		for(typename base_type::const_iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			j = (i + 1 == isize? base_type::begin() : i + 1);

			tmp = gen_prop(now_pos_type(*i), now_pos_type(right), now_pos_type(*j));

			if(cmper_zero == tmp)
			{
				continue;
			}

			if(cmper == tmp || cmper != -tmp)
			{
				return false;
			}
		}

		return true;
	}

	template<typename Matrix>
	const this_type operator*(const Matrix& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
												boost::mpl::size_t<Matrix::E_MAX_RANK> >));

		this_type rst(*this);

		for(typename base_type::const_iterator i = rst.begin(), isize = rst.end(); i != isize; ++i)
		{
			(*i) *= right;
		}
		return rst;
	}

	template<typename Matrix>
	this_type& operator*=(const Matrix& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
												boost::mpl::size_t<Matrix::E_MAX_RANK> >));
		for(typename base_type::iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			(*i) *= right;
		}

		_prop = gen_prop(base_type::begin());
		return *this;
	}

private:

	void gen_polygon_type(void)
	{
		typedef math::value_miss_comparer<prop_type> cmper_type;
		if(base_type::size() < 3)
		{
			return;
		}

		_prop = prop_type();

		prop_type zero = prop_type();
		cmper_type cmper_now(_prop);
		cmper_type cmper_zero(zero);

		prop_type tmp = prop_type();

		base_type pos_vt;

		bool b = true;
		for(typename base_type::const_iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			tmp = gen_prop(i);

			if(cmper_zero == tmp)
			{
				continue;
			}

			if(cmper_now == zero)
			{
				_prop = tmp;
				pos_vt.push_back(*i);
				continue;
			}

			if(cmper_now != tmp)
			{
				b = false;
				break;
			}

			pos_vt.push_back(*i);
		}

		if(b)
		{
			base_type::swap(pos_vt);
		}
		else
		{
			clear();
		}

		pos_vt.clear();
	}

	prop_type gen_prop(const typename base_type::const_iterator& iter) const
	{
		typedef typename base_type::const_iterator citer_type;
		assert(iter != base_type::end());
		citer_type i = (iter == base_type::begin()? base_type::end() - 1 : iter - 1);
		citer_type k = (iter + 1 == base_type::end()? base_type::begin() : iter + 1);

		return gen_prop(*i, *iter, *k);
	}


	prop_type gen_prop(const size_type& idx) const
	{
		const base_type& base = *this;
		size_type size = base_type::size();
		assert(idx < size);
		size_type now_idx = idx;
		size_type idxi = now_idx? now_idx - 1 : size - 1;
		size_type idxj = now_idx;
		size_type idxk = (now_idx + 1) % size;

		return gen_prop(base[idxi], base[idxj], base[idxk]);
	}

	template<typename OPos>
	prop_type gen_prop(const OPos& posi, const OPos& posj, const OPos& posk) const
	{
		typedef OPos now_pos_type;
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		now_pos_type pij(posj - posi);
		now_pos_type pjk(posk - posj);

		return math::sign(pij.cross(pjk));
	}

	template<typename OPos>
	bool check_same(const geo_polygon<OPos, Alloc, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		typedef geo_polygon<OPos, Alloc, Base> right_type;
		typedef typename right_type::const_iterator right_citer_type;
		typedef typename base_type::const_iterator citer_type;

		if(!(is_eligibility()
				&& _prop == right.prop()
				&& base_type::size() == right.size()))
		{
			return false;
		}

		right_citer_type r_iter = std::find(right.begin(), right.end(), *(base_type::begin()));

		if(r_iter == right.end())
		{
			return false;
		}

		++r_iter;

		right_citer_type r_size = right.end();
		for(citer_type i = base_type::begin() + 1, isize = base_type::end(); i != isize; ++i, ++r_iter)
		{
			if(r_iter == r_size)
			{
				r_iter = right.begin();
			}

			if(*i != *r_iter)
			{
				return false;
			}
		}

		return true;
	}
private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("base", boost::serialization::base_object<base_type>(*this));
		ar & YGGR_SERIALIZE_NVP(_prop);
	}

private:
	prop_type _prop;
};

} // namespace geometry
} // namespace yggr


namespace std
{

template<typename Position,
			template<typename _Val> class Alloc,
			template<typename _Val, typename _Alloc> class Base>
void swap(yggr::geometry::geo_polygon<Position, Alloc, Base>& l, 
			yggr::geometry::geo_polygon<Position, Alloc, Base>& r)
{
	l.swap(r);
}

} // namespace std

#endif // __YGGR_GEOMETRY_GEO_POLYGON_HPP__
