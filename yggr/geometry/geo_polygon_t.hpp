//geo_polygon_t.hpp

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

#ifndef __YGGR_GEOMETRY_GEO_POLYGON_T_HPP__
#define __YGGR_GEOMETRY_GEO_POLYGON_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/tuple/tuple.hpp>
#include <yggr/move/move.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/type_traits/upper_types.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/math/flaw_comparer.hpp>
#include <yggr/math/sign.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/size.hpp>

#include <yggr/container_ex/allocator_t.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/vector.hpp>

#include <boost/ref.hpp>

#include <boost/type_traits/is_class.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/equal_to.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <cassert>
#include <memory>
#include <utility>
#include <algorithm>
#include <complex>

namespace yggr
{

namespace geometry
{

template<typename Position, typename Alloc,
		template<typename _Val, typename _Alloc> class Base>
class geo_polygon;

};

namespace geometry
{

template<typename Position,
			typename Alloc = typename mplex::typename_expand_get< container::vector<Position>, 1>::type,
			template<typename _Val, typename _Alloc> class Base = container::vector>
class geo_polygon
	: public Base<Position, Alloc>
{

public:
	typedef Position pos_type;
	typedef pos_type point_type;
	typedef pos_type value_type;

	typedef Alloc tpl_arg_allocator_type;

public:
	typedef typename point_type::value_type pos_value_type;
	typedef pos_value_type point_value_type;
	typedef typename upper_float<typename pos_type::cross_result_type>::type prop_type;

public:
	typedef Base<pos_type, tpl_arg_allocator_type> base_type;
	typedef typename container_ex::allocator_t<base_type>::type allocator_type;

private:
	typedef typename
		boost::mpl::if_
		<
			boost::is_class<prop_type>,
			const prop_type&,
			prop_type
		>::type ret_prop_type;

private:
	typedef geo_polygon this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	geo_polygon(void)
		: _prop(prop_type())
	{
	}

	template<typename OPos1, typename OPos2, typename OPos3>
	geo_polygon(const OPos1& p1, const OPos2& p2, const OPos3& p3)
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

		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos3::E_LENGTH>
			>));

		base_type::reserve(3);

		base_type::push_back(p1);
		base_type::push_back(p2);
		base_type::push_back(p3);

		this_type::prv_gen_polygon_t();
	}

	template<typename OPos1, typename OPos2, typename OPos3, typename OPos4>
	geo_polygon(const OPos1& p1, const OPos2& p2, const OPos3& p3, const OPos4& p4)
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

		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos3::E_LENGTH>
			>));

		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos4::E_LENGTH>
			>));

		base_type::reserve(4);

		base_type::push_back(pos_type(p1));
		base_type::push_back(pos_type(p2));
		base_type::push_back(pos_type(p3));
		base_type::push_back(pos_type(p4));

		this_type::prv_gen_polygon_t();
	}

	template<typename OPos, typename OAlloc,
				template<typename _Val, typename _Alloc> class OBase>
	geo_polygon(const OBase< OPos, OAlloc >& right)
		: base_type(boost::begin(right), boost::end(right))
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH>
			>));

		this_type::prv_gen_polygon_t();
	}

	geo_polygon(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
		this_type::prv_gen_polygon_t();
	}

	geo_polygon(const base_type& right)
		: base_type(right)
	{
		this_type::prv_gen_polygon_t();
	}

	template<typename OPos, typename OAlloc,
				template<typename _Val, typename _Alloc> class OBase>
	geo_polygon(const geo_polygon<OPos, OAlloc, OBase>& right)
		: base_type(right.begin(), right.end())
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH>
			>));

		this_type::prv_gen_polygon_t();
	}

	geo_polygon(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right))),
			_prop(boost::move(right._prop))
	{
	}

	geo_polygon(const this_type& right)
		: base_type(right), _prop(right._prop)
	{
	}

	~geo_polygon(void)
	{
		clear();
	}

public:
	template<typename OPos,
				template<typename _Val> class OAlloc,
				template<typename _Val, typename _Alloc> class OBase>
	this_type& operator=(const OBase< OPos, OAlloc<OPos> >& right)
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH>
			>));

		base_type::resize(::yggr::size(right));
		std::copy(boost::begin(right), boost::end(right), base_type::begin());

		this_type::prv_gen_polygon_t();
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type& base_ref = *this;
		if(&base_ref == &right_ref)
		{
			return *this;
		}

		yggr::copy_or_move_or_swap(base_ref, boost::move(right_ref));
		this_type::prv_gen_polygon_t();

		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		base_type& base_ref = *this;
		if(&base_ref == &right)
		{
			return *this;
		}

		base_ref = right;
		this_type::prv_gen_polygon_t();

		return *this;
	}

	template<typename OPos, typename OAlloc,
				template<typename _Val, typename _Alloc> class OBase>
	this_type& operator=(const geo_polygon<OPos, OAlloc, OBase>& right)
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH>
			>));

		base_type::resize(right.size());
		std::copy(right.begin(), right.end(), base_type::begin());

		this_type::prv_gen_polygon_t();
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(static_cast<base_type&>(right)));
		copy_or_move_or_swap(_prop, boost::move(right._prop));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& base = *this;
		base = static_cast<const base_type&>(right);
		_prop = right._prop;
		return *this;
	}

public:

	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type& right)
	{
		if(this == &right)
		{
			return;
		}
		base_type::swap(right);
		this_type::prv_gen_polygon_t();
	}

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

		base_type::swap(right);
		yggr::swap(_prop, right._prop);
	}

public:
	inline void clear(void)
	{
		_prop = prop_type();
		base_type::clear();
	}

	inline ret_prop_type prop(void) const
	{
		return _prop;
	}

public:
	template<typename OPos, typename OAlloc,
				template<typename _Val, typename _Alloc> class OBase> inline
	bool equal_to(const geo_polygon<OPos, OAlloc, OBase>& right) const
	{
		return this_type::prv_check_same(right);
	}

	inline bool equal_to(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| this_type::prv_check_same(right);
	}

	template<typename OPos, typename OAlloc,
				template<typename _Val, typename _Alloc> class OBase> inline
	bool not_equal_to(const geo_polygon<OPos, OAlloc, OBase>& right) const
	{
		return !this_type::equal_to(right);
	}

public:
	inline size_type edge_size(void) const
	{
		return base_type::size();
	}

	inline size_type points_size(void) const
	{
		return base_type::size();
	}

	inline size_type get_point_index(const pos_type& pos) const
	{
		typename base_type::const_iterator iter = std::find(base_type::begin(), base_type::end(), pos);
		return std::distance(base_type::begin(), iter);
	}

	inline const base_type& get_points(void) const
	{
		return *this;
	}

	inline base_type get_points(const size_type& idx) const
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

	inline bool is_eligibility(void) const
	{
		return _prop != prop_type();
	}

	template<typename OPos, typename OAlloc,
				template<typename _Val, typename _Alloc> class OBase>
	bool is_encircling(const geo_polygon<OPos, OAlloc, OBase>& right) const
	{
		typedef geo_polygon<OPos, OAlloc, OBase> right_type;
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
		typedef math::flaw_comparer<prop_type> cmper_type;
		typedef typename upper_float<OPos>::type now_pos_type;

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

			tmp = prv_gen_prop(now_pos_type(*i), now_pos_type(right), now_pos_type(*j));

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

private:
	void prv_gen_polygon_t(void)
	{
		typedef math::flaw_comparer<prop_type> cmper_type;

		assert(!(base_type::size() < 3));
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
			tmp = prv_gen_prop(i);

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

	inline prop_type prv_gen_prop(const typename base_type::const_iterator& iter) const
	{
		typedef typename base_type::const_iterator citer_type;
		assert(iter != base_type::end());
		citer_type i = (iter == base_type::begin()? base_type::end() - 1 : iter - 1);
		citer_type k = (iter + 1 == base_type::end()? base_type::begin() : iter + 1);

		return prv_gen_prop(*i, *iter, *k);
	}


	inline prop_type prv_gen_prop(const size_type& idx) const
	{
		const base_type& base = *this;
		size_type size = base_type::size();
		assert(idx < size);
		size_type now_idx = idx;
		size_type idxi = now_idx? now_idx - 1 : size - 1;
		size_type idxj = now_idx;
		size_type idxk = (now_idx + 1) % size;

		return prv_gen_prop(base[idxi], base[idxj], base[idxk]);
	}

	template<typename OPos1, typename OPos2, typename OPos3> inline
	static prop_type prv_gen_prop(const OPos1& posi, const OPos2& posj, const OPos3& posk)
	{
		//typedef OPos now_pos_type;

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

		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos3::E_LENGTH>
			>));

		//now_pos_type pij(posj - posi);
		//now_pos_type pjk(posk - posj);

		return math::sign((posj - posi).cross(posk - posj));
	}

	template<typename OPos, typename OAlloc,
				template<typename _Val, typename _Alloc> class OBase>
	bool prv_check_same(const geo_polygon<OPos, OAlloc, OBase>& right) const
	{
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to
			<
				boost::mpl::size_t<pos_type::E_LENGTH>,
				boost::mpl::size_t<OPos::E_LENGTH>
			>));

		typedef geo_polygon<OPos, OAlloc, OBase> right_type;
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
		for(citer_type i = ::yggr::advance_copy(base_type::begin(), 1), isize = base_type::end();
				i != isize; ++i, ++r_iter)
		{
			if(r_iter == r_size)
			{
				r_iter = right.begin();
			}

			if(!(*i == *r_iter))
			{
				return false;
			}
		}

		return true;
	}
private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP("base", boost::serialization::base_object<base_type>(*this));
		ar & YGGR_SERIALIZATION_NVP(_prop);
	}

private:
	prop_type _prop;
};

// non-member foo

// equal_to
template<typename PosL, typename AllocL, typename PosR, typename AllocR,
			template<typename _Val, typename _Alloc> class BaseL,
			template<typename _Val, typename _Alloc> class BaseR> inline
bool operator==(const geo_polygon<PosL, AllocL, BaseL>& l, const geo_polygon<PosR, AllocR, BaseR>& r)
{
	return l.equal_to(r);
}

template<typename PosL, typename AllocL, typename PosR, typename AllocR,
			template<typename _Val, typename _Alloc> class BaseL,
			template<typename _Val, typename _Alloc> class BaseR> inline
bool operator!=(const geo_polygon<PosL, AllocL, BaseL>& l, const geo_polygon<PosR, AllocR, BaseR>& r)
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

template<typename Position, typename Alloc,
		template<typename _Val, typename _Alloc> class Base> inline
void swap(geo_polygon<Position, Alloc, Base>& l, geo_polygon<Position, Alloc, Base>& r)
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


#endif // __YGGR_GEOMETRY_GEO_POLYGON_T_HPP__
