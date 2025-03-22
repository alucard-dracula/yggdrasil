//vector2d_t.hpp

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

#ifndef __YGGR_MATH_VECTOR2D_T_HPP__
#define __YGGR_MATH_VECTOR2D_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>

#include <yggr/type_traits/upper_types.hpp>
#include <yggr/type_traits/operator_result.hpp>

#include <yggr/math/sqrt.hpp>
#include <yggr/math/sign.hpp>
#include <yggr/math/modulus.hpp>
#include <yggr/math/flaw_comparer.hpp>

#include <yggr/container/array.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array.hpp>

#include <boost/functional/hash/hash.hpp>
#include <boost/ref.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <cassert>
#include <ostream>
#include <algorithm>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4244)
#endif //_MSC_VER


namespace yggr
{
namespace math
{

template<typename Val,
			template <typename _Val, std::size_t> class Base>
class vector2d;

} // namesapce math
} // namespace yggr

namespace yggr
{
namespace detail
{

// fix error c2784 in operator_result
template<typename Val,
			template <typename _Val, std::size_t> class Base>
struct operator_result_t_getter< math::vector2d<Val, Base> >
	: public ::yggr::traits<Val>
{
public:
	operator Val() const;
};

} // namespace detail
} // namespace yggr

namespace yggr
{
namespace math
{

template<typename Val,
			template <typename _Val, std::size_t> class Base = yggr::array>
class vector2d
	: public Base<typename upper_signed<Val>::type, 3>
{
private:
	template<typename TVal,
			template <typename _Val, std::size_t> class TBase>
	friend class vector2d;

public:
	typedef typename upper_signed<Val>::type value_type;
	typedef value_type cross_result_type;

private:
	typedef value_type val_type;

public:
	YGGR_STATIC_CONSTANT(std::size_t, E_LENGTH = 2);
	YGGR_STATIC_CONSTANT(std::size_t, E_ARRAY_LENGTH = E_LENGTH + 1);

	typedef Base<val_type, E_ARRAY_LENGTH> base_type;

private:
	typedef vector2d this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	vector2d(void)
		: x(base_type::operator[](0)), y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
		x = 0;
		y = 0;
		w = 1;
	}

	template<typename OVal1, typename OVal2>
	vector2d(const OVal1& val1, const OVal2& val2)
		:  x(base_type::operator[](0)), 
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
		x = static_cast<val_type>(val1);
		y = static_cast<val_type>(val2);
		w = static_cast<val_type>(1);
	}

	template<typename OVal1, typename OVal2, typename OVal3>
	vector2d(const OVal1& val1, const OVal2& val2, const OVal3& val3)
		:  x(base_type::operator[](0)), 
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
		x = static_cast<val_type>(val1);
		y = static_cast<val_type>(val2);
		w = static_cast<val_type>(val3);
	}

	vector2d(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right)),
			x(base_type::operator[](0)),
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
	}

	vector2d(const base_type& right)
		: base_type(right),
			x(base_type::operator[](0)), 
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	vector2d(const vector2d<OVal, OBase>& right)
		: x(base_type::operator[](0)), 
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
		(*this)[0] = static_cast<val_type>(right[0]);
		(*this)[1] = static_cast<val_type>(right[1]);
		(*this)[2] = static_cast<val_type>(right[2]);
	}

	template<typename OVal, typename W,
				template <typename _Val, std::size_t> class OBase>
	vector2d(const vector2d<OVal, OBase>& right, const W& w)
		: x(base_type::operator[](0)), 
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
		(*this)[0] = static_cast<val_type>(right[0]);
		(*this)[1] = static_cast<val_type>(right[1]);
		(*this)[2] = static_cast<val_type>(w);
	}

	vector2d(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right))),
			x(base_type::operator[](0)), 
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
	}

	template<typename W>
	vector2d(BOOST_RV_REF(this_type) right, const W& w)
		: base_type(boost::move(static_cast<base_type&>(right))),
			x(base_type::operator[](0)), 
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
		(*this)[2] = static_cast<val_type>(w);
	}

	vector2d(const this_type& right)
		: base_type(right),
			x(base_type::operator[](0)), y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
	}

	template<typename W>
	vector2d(const this_type& right, const W& w)
		: base_type(right),
			x(base_type::operator[](0)), 
			y(base_type::operator[](1)),
			w(base_type::operator[](2))
	{
		(*this)[2] = static_cast<val_type>(w);
	}

	~vector2d(void)
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

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& operator=(const vector2d<OVal, OBase>& right)
	{
		(*this)[0] = static_cast<val_type>(right[0]);
		(*this)[1] = static_cast<val_type>(right[1]);
		(*this)[2] = static_cast<val_type>(right[2]);

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
		this_type& right_ref = right;

#ifdef _DEBUG
		val_type* p = &base[0];
#endif //_DEBUG

		std::copy(boost::begin(right_ref), boost::end(right_ref), boost::begin(base));

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		base_type& base = *this;

#ifdef _DEBUG
		val_type* p = &base[0];
#endif //_DEBUG

		std::copy(boost::begin(right), boost::end(right), boost::begin(base));

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
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
	    if(this == &right) {return;}
		base_type& base = *this;

#ifdef _DEBUG
		val_type* p = &base[0];
#endif //_DEBUG

		//detail::values_swap<base_type>(base, right);
		std::swap_ranges(boost::begin(base), boost::end(base), boost::begin(right));

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right) { return; }
		base_type& base =*this;

#ifdef _DEBUG
		val_type* p = &base[0];
#endif //_DEBUG

		//detail::values_swap<base_type>(base, right);
		std::swap_ranges(boost::begin(base), boost::end(base), boost::begin(right));

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG

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

	inline void clear(void)
	{
		(*this)[0] = 0;
		(*this)[1] = 0;
		(*this)[2] = 1;
	}

	inline bool is_zero(void) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		cmper_type cmper((*this)[0]);

		return cmper == 0 && cmper == (*this)[1];
	}

	inline bool is_zero_w(void) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		cmper_type cmper((*this)[0]);

		return cmper == 0 && cmper == (*this)[1] && cmper == (*this)[2];
	}

	inline size_type zero_count(void) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		return 
			static_cast<size_type>(cmper_type((*this)[0]) == 0)
			+ static_cast<size_type>(cmper_type((*this)[1]) == 0);
	}

	inline size_type zero_count_w(void) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		return 
			static_cast<size_type>(cmper_type((*this)[0]) == 0)
			+ static_cast<size_type>(cmper_type((*this)[1]) == 0)
			+ static_cast<size_type>(cmper_type((*this)[2]) == 0);
	}

public:
	inline const this_type& positive(void) const
	{
		return *this;
	}

	inline vector2d<typename negate_operator_result<val_type>::type, Base> 
		negate(void) const
	{
		typedef vector2d<typename negate_operator_result<val_type>::type, Base> ret_type;
		
		return ret_type(-(*this)[0], -(*this)[1], -(*this)[2]);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	typename
		boost::mpl::if_
		<
			boost::is_same<typename plus_operator_result<val_type, OVal>::type, val_type>,
			vector2d<typename plus_operator_result<val_type, OVal>::type, Base>,
			vector2d<typename plus_operator_result<val_type, OVal>::type, OBase>
		>::type
		plus(const vector2d<OVal, OBase>& right) const
	{
		typedef typename
			boost::mpl::if_
			<
				boost::is_same<typename plus_operator_result<val_type, OVal>::type, val_type>,
				vector2d<typename plus_operator_result<val_type, OVal>::type, Base>,
				vector2d<typename plus_operator_result<val_type, OVal>::type, OBase>
			>::type ret_type;
		
		return ret_type((*this)[0] + right[0], (*this)[1] + right[1], (*this)[2] + right[2]);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& plus_set(const vector2d<OVal, OBase>& right)
	{
		(*this)[0] += right[0];
		(*this)[1] += right[1];
		(*this)[2] += right[2];

		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	typename
		boost::mpl::if_
		<
			boost::is_same<typename minus_operator_result<val_type, OVal>::type, val_type>,
			vector2d<typename minus_operator_result<val_type, OVal>::type, Base>,
			vector2d<typename minus_operator_result<val_type, OVal>::type, OBase>
		>::type
		minus(const vector2d<OVal, OBase>& right) const
	{
		typedef typename
			boost::mpl::if_
			<
				boost::is_same<typename minus_operator_result<val_type, OVal>::type, val_type>,
				vector2d<typename minus_operator_result<val_type, OVal>::type, Base>,
				vector2d<typename minus_operator_result<val_type, OVal>::type, OBase>
			>::type ret_type;
		
		return ret_type((*this)[0] - right[0], (*this)[1] - right[1], (*this)[2] - right[2]);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& minus_set(const vector2d<OVal, OBase>& right)
	{
		(*this)[0] -= right[0];
		(*this)[1] -= right[1];
		(*this)[2] -= right[2];

		return *this;
	}

	template<typename OVal> inline
	vector2d<typename multiplies_operator_result<val_type, OVal>::type, Base> 
		multiplies(const OVal& right) const
	{
		typedef vector2d<typename multiplies_operator_result<val_type, OVal>::type, Base> ret_type;
		
		return ret_type((*this)[0] * right, (*this)[1] * right, (*this)[2] * right);
	}

	template<typename OVal> inline
	this_type& multiplies_set(const OVal& right)
	{
		(*this)[0] *= right;
		(*this)[1] *= right;
		(*this)[2] *= right;

		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	typename
		boost::mpl::if_
		<
			boost::is_same<typename multiplies_operator_result<val_type, OVal>::type, val_type>,
			vector2d<typename multiplies_operator_result<val_type, OVal>::type, Base>,
			vector2d<typename multiplies_operator_result<val_type, OVal>::type, OBase>
		>::type
		multiplies(const vector2d<OVal, OBase>& right) const
	{
		typedef typename
			boost::mpl::if_
			<
				boost::is_same<typename multiplies_operator_result<val_type, OVal>::type, val_type>,
				vector2d<typename multiplies_operator_result<val_type, OVal>::type, Base>,
				vector2d<typename multiplies_operator_result<val_type, OVal>::type, OBase>
			>::type ret_type;

		return ret_type((*this)[0] * right[0], (*this)[1] * right[1], (*this)[2] * right[2]);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& multiplies_set(const vector2d<OVal, OBase>& right)
	{
		(*this)[0] *= right[0];
		(*this)[1] *= right[1];
		(*this)[2] *= right[2];

		return *this;
	}

	template<typename OVal> inline
	vector2d<typename divides_operator_result<val_type, OVal>::type, Base> 
		divides(const OVal& right) const
	{
		typedef vector2d<typename divides_operator_result<val_type, OVal>::type, Base> ret_type;
		
		return ret_type((*this)[0] / right, (*this)[1] / right, (*this)[2] / right);
	}

	template<typename OVal> inline
	this_type& divides_set(const OVal& right)
	{
		(*this)[0] /= right;
		(*this)[1] /= right;
		(*this)[2] /= right;

		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	typename
		boost::mpl::if_
		<
			boost::is_same<typename divides_operator_result<val_type, OVal>::type, val_type>,
			vector2d<typename divides_operator_result<val_type, OVal>::type, Base>,
			vector2d<typename divides_operator_result<val_type, OVal>::type, OBase>
		>::type
		divides(const vector2d<OVal, OBase>& right) const
	{
		typedef typename
			boost::mpl::if_
			<
				boost::is_same<typename divides_operator_result<val_type, OVal>::type, val_type>,
				vector2d<typename divides_operator_result<val_type, OVal>::type, Base>,
				vector2d<typename divides_operator_result<val_type, OVal>::type, OBase>
			>::type ret_type;

		return ret_type((*this)[0] / right[0], (*this)[1] / right[1], (*this)[2] / right[2]);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& divides_set(const vector2d<OVal, OBase>& right)
	{
		(*this)[0] /= right[0];
		(*this)[1] /= right[1];
		(*this)[2] /= right[2];

		return *this;
	}

	template<typename OVal> inline
	vector2d<typename modulus_operator_result<val_type, OVal>::type, Base> 
		modulus(const OVal& right) const
	{
		typedef vector2d<typename modulus_operator_result<val_type, OVal>::type, Base> ret_type;

		return 
			ret_type(
				::yggr::math::modulus((*this)[0], right), 
				::yggr::math::modulus((*this)[1], right), 
				::yggr::math::modulus((*this)[2], right));
	}

	template<typename OVal> inline
	this_type& modulus_set(const OVal& right)
	{
		::yggr::math::modulus_set((*this)[0], right);
		::yggr::math::modulus_set((*this)[1], right);
		::yggr::math::modulus_set((*this)[2], right);

		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	typename
		boost::mpl::if_
		<
			boost::is_same<typename modulus_operator_result<val_type, OVal>::type, val_type>,
			vector2d<typename modulus_operator_result<val_type, OVal>::type, Base>,
			vector2d<typename modulus_operator_result<val_type, OVal>::type, OBase>
		>::type
		modulus(const vector2d<OVal, OBase>& right) const
	{
		typedef typename
			boost::mpl::if_
			<
				boost::is_same<typename modulus_operator_result<val_type, OVal>::type, val_type>,
				vector2d<typename modulus_operator_result<val_type, OVal>::type, Base>,
				vector2d<typename modulus_operator_result<val_type, OVal>::type, OBase>
			>::type ret_type;
		
		return 
			ret_type(
				::yggr::math::modulus((*this)[0], right[0]), 
				::yggr::math::modulus((*this)[1], right[1]), 
				::yggr::math::modulus((*this)[2], right[2]) );
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& modulus_set(const vector2d<OVal, OBase>& right)
	{
		::yggr::math::modulus_set((*this)[0], right[0]);
		::yggr::math::modulus_set((*this)[1], right[1]);
		::yggr::math::modulus_set((*this)[2], right[2]);

		return *this;
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool equal_to(const vector2d<OVal, OBase>& right) const
	{
		typedef flaw_comparer<val_type> val_cmper_type;

		return 
			val_cmper_type((*this)[0]) == right[0]
			&& val_cmper_type((*this)[1]) == right[1]
			&& val_cmper_type((*this)[2]) == right[2];
	}

	inline bool equal_to(const this_type& right) const
	{
		typedef flaw_comparer<val_type> val_cmper_type;

		return 
			(this == boost::addressof(right))
			|| (val_cmper_type((*this)[0]) == right[0]
				&& val_cmper_type((*this)[1]) == right[1]
				&& val_cmper_type((*this)[2]) == right[2]);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool not_equal_to(const vector2d<OVal, OBase>& right) const
	{
		return !this_type::equal_to(right);
	}

public:
	inline vector2d<typename upper_float<val_type>::type> normalize(void) const
	{
		typedef vector2d<typename upper_float<val_type>::type> ret_type;
		typedef flaw_comparer<val_type> cmp_type;
		assert(cmp_type(w) == val_type());

		ret_type ret(*this);
		ret.prv_normalize();
		return ret;
	}

	inline this_type& round(void)
	{
		(*this)[0] = std::floor(0.5 + (*this)[0]);
		(*this)[1] = std::floor(0.5 + (*this)[1]);
		(*this)[2] = std::floor(0.5 + (*this)[2]);

		return *this;
	}

	inline this_type& floor(void)
	{
		(*this)[0] = std::floor((*this)[0]);
		(*this)[1] = std::floor((*this)[1]);
		(*this)[2] = std::floor((*this)[2]);

		return *this;
	}

	inline this_type& ceil(void)
	{
		(*this)[0] = std::ceil((*this)[0]);
		(*this)[1] = std::ceil((*this)[1]);
		(*this)[1] = std::ceil((*this)[1]);

		return *this;
	}

	inline val_type sum(void) const
	{
		typedef flaw_comparer<val_type> cmp_type;
		assert(cmp_type(w) == val_type());
		return x + y;
	}

	inline val_type sum_w(void) const
	{
		typedef flaw_comparer<val_type> cmp_type;
		assert(cmp_type(w) == val_type());
		return x + y + w;
	}

	inline typename upper_float<val_type>::type length(void) const
	{
		typedef typename upper_float<val_type>::type rst_type;
		typedef flaw_comparer<val_type> cmp_type;
		assert(cmp_type(w) == val_type());

		return ::yggr::math::sqrt(static_cast<rst_type>(this_type::length_sq()));
	}
	
	inline val_type length_sq(void) const
	{
		typedef flaw_comparer<val_type> cmp_type;
		assert(cmp_type(w) == val_type());

		return (*this)[0] * (*this)[0] + (*this)[1] * (*this)[1];
	}
	
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	typename multiplies_operator_result<val_type, OVal>::type 
		dot(const vector2d<OVal, OBase>& right) const
	{
		typedef flaw_comparer<val_type> cmp_type;
		typedef flaw_comparer<OVal> ocmp_type;
		assert(cmp_type(w) == val_type());
		assert(ocmp_type(right.w) == val_type());

		//x1 * y1 + x2 * y2
		return (*this)[0] * right[0] + (*this)[1] * right[1];
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	typename multiplies_operator_result<val_type, OVal>::type 
		cross(const vector2d<OVal, OBase>& right) const
	{
		typedef flaw_comparer<val_type> cmp_type;
		typedef flaw_comparer<OVal> ocmp_type;
		assert(cmp_type(w) == val_type());
		assert(ocmp_type(right.w) == val_type());

		//x1 * y2 - y1 * x2
		return (*this)[0] * right[1] - (*this)[1] * right[0];
	}

	template<typename OVal> inline
	vector2d<OVal, Base> type_cast(void) const
	{
		return vector2d<OVal, Base>(*this);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	vector2d<OVal, OBase> type_cast(void) const
	{
		return vector2d<OVal, OBase>(*this);
	}

	template<typename Ret, typename Handler> inline
	Ret type_cast(const Handler& handler) const
	{
		return handler(*this);
	}

	inline std::size_t hash(void) const
	{
		const base_type& base = *this;
		std::size_t seed = 0;
		boost::hash_combine(seed, "vector2d");
		boost::hash_combine(seed, base);
		return seed;
	}

private:
	void prv_normalize(void)
	{
		typedef flaw_comparer<val_type> val_cmper_type;
		typedef typename upper_float<val_type>::type float_type;
		typedef flaw_comparer<val_type> cmp_type;
		assert(cmp_type(w) == val_type());

		val_type len_sq = length_sq();
		if(val_cmper_type(len_sq) == val_type())
		{
			return;
		}

		float_type tval = float_type();
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tval = (*this)[i];
			(*this)[i] = ::yggr::math::sign(tval) * yggr::math::sqrt((tval * tval) / len_sq);
		}

		assert((x || y));
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP("vector2d_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
	}
public:
	val_type& x;
	val_type& y;
	val_type& w;
};

// non-member foo
template<typename Val,
			template<typename _Val, std::size_t> class Base> inline
vector2d<typename upper_float<Val>::value_type, Base> 
	sign(const vector2d<Val, Base>& x)
{
	//return x.get_normalized_vector();
	return x.normalize();
}

// positive
template<typename Val,
			template <typename _Val, std::size_t> class Base> inline
const vector2d<Val, Base>& 
	operator+(const vector2d<Val, Base>& val)
{
	return val;
}

//negate
template<typename Val,
			template <typename _Val, std::size_t> class Base> inline
vector2d<typename negate_operator_result<Val>::type, Base> 
	operator-(const vector2d<Val, Base>& val)
{
	return val.negate();
}

// plus
template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename
	boost::mpl::if_
	<
		boost::is_same<typename plus_operator_result<ValL, ValR>::type, ValL>,
		vector2d<typename plus_operator_result<ValL, ValR>::type, BaseL>,
		vector2d<typename plus_operator_result<ValL, ValR>::type, BaseR> 
	>::type
	operator+(const vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.plus(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
vector2d<ValL, BaseL>&
	operator+=(vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.plus_set(r);
}

// minus
template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename
	boost::mpl::if_
	<
		boost::is_same<typename minus_operator_result<ValL, ValR>::type, ValL>,
		vector2d<typename minus_operator_result<ValL, ValR>::type, BaseL>,
		vector2d<typename minus_operator_result<ValL, ValR>::type, BaseR> 
	>::type
	operator-(const vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.minus(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
vector2d<ValL, BaseL>&
	operator-=(vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.minus_set(r);
}

// mul
template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class Base> inline
vector2d<typename multiplies_operator_result<ValL, ValR>::type, Base>
	operator*(const ValL& l, const vector2d<ValR, Base>& r)
{
	typedef vector2d<typename multiplies_operator_result<ValL, ValR>::type, Base> ret_type;

	return r.multiplies(l);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class Base> inline
vector2d<typename multiplies_operator_result<ValL, ValR>::type, Base>
	operator*(const vector2d<ValL, Base>& l, const ValR& r)
{
	return l.multiplies(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class Base> inline
vector2d<ValL, Base>&
	operator*=(vector2d<ValL, Base>& l, const ValR& r)
{
	return l.multiplies_set(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename
	boost::mpl::if_
	<
		boost::is_same<typename multiplies_operator_result<ValL, ValR>::type, ValL>,
		vector2d<typename multiplies_operator_result<ValL, ValR>::type, BaseL>,
		vector2d<typename multiplies_operator_result<ValL, ValR>::type, BaseR> 
	>::type
	operator*(const vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.multiplies(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
vector2d<ValL, BaseL>&
	operator*=(vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.multiplies_set(r);
}

// div

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class Base> inline
vector2d<typename divides_operator_result<ValL, ValR>::type, Base>
	operator/(const vector2d<ValL, Base>& l, const ValR& r)
{
	return l.divides(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class Base> inline
vector2d<ValL, Base>&
	operator/=(vector2d<ValL, Base>& l, const ValR& r)
{
	return l.divides_set(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename
	boost::mpl::if_
	<
		boost::is_same<typename divides_operator_result<ValL, ValR>::type, ValL>,
		vector2d<typename divides_operator_result<ValL, ValR>::type, BaseL>,
		vector2d<typename divides_operator_result<ValL, ValR>::type, BaseR> 
	>::type
	operator/(const vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.divides(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
vector2d<ValL, BaseL>&
	operator/=(vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.divides_set(r);
}

// mod
template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class Base> inline
vector2d<typename modulus_operator_result<ValL, ValR>::type, Base> 
	operator%(const vector2d<ValL, Base>& l, const ValR& r)
{
	return l.modulus(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class Base> inline
vector2d<ValL, Base>&
	operator%=(vector2d<ValL, Base>& l, const ValR& r)
{
	return l.modulus_set(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename
	boost::mpl::if_
	<
		boost::is_same<typename modulus_operator_result<ValL, ValR>::type, ValL>,
		vector2d<typename modulus_operator_result<ValL, ValR>::type, BaseL>,
		vector2d<typename modulus_operator_result<ValL, ValR>::type, BaseR> 
	>::type
	operator%(const vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.modulus(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
vector2d<ValL, BaseL>&
	operator%=(vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.modulus_set(r);
}

//equal_to

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
bool operator==(const vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return l.equal_to(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
bool operator!=(const vector2d<ValL, BaseL>& l, const vector2d<ValR, BaseR>& r)
{
	return !l.equal_to(r);
}

} // namespace math
} // namespace yggr

namespace yggr
{
template<typename Val,
			template <typename _Val, std::size_t>
				class Base >
struct upper_float< math::vector2d<Val, Base> >
{
	typedef typename math::vector2d< typename upper_float<Val>::type, Base > type;
};

template<typename Val,
			template <typename _Val, std::size_t>
				class Base >
struct upper_signed< math::vector2d<Val, Base> >
{
	typedef typename math::vector2d< typename upper_signed< Val >::type, Base > type;
};

} // namespace yggr

namespace yggr
{
namespace math
{
namespace swap_support
{

template<typename Val, template<typename _Val, std::size_t> class Base> inline
void swap(vector2d<Val, Base>& l, vector2d<Val, Base>& r) 
{
	l.swap(r); 
}

template<typename Val,
			template<typename _Val, std::size_t> class Base> inline 
void swap(typename vector2d<Val, Base>::base_type& l, 
			vector2d<Val, Base>& r) 
{ 
	r.swap(l); 
}

template<typename Val,
			template<typename _Val, std::size_t> class Base> inline
void swap(vector2d<Val, Base>& l,
			typename vector2d<Val, Base>::base_type& r) 
{
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace math
} // namespace yggr

namespace yggr
{
namespace math
{
namespace math_support
{

template<typename Val,
			template<typename _Val, std::size_t> class Base> inline
typename upper_float<Val>::type abs(const vector2d<Val, Base>& x)
{
	return x.length();
}

} // namespace math_support

using math_support::abs;

} // namespace math
} // namespace yggr

namespace std
{
	using ::yggr::math::math_support::abs;
	using ::yggr::math::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::math::swap_support::swap;
} // namespace boost

namespace yggr
{
namespace math
{

template<typename Char, typename Traits, typename Val,
			template<typename _Val, std::size_t> class Base>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const yggr::math::vector2d<Val, Base>& vec)
{
	typedef Val val_type;
	typedef yggr::math::vector2d<Val, Base> vec_type;
	typedef typename vec_type::const_iterator vec_c_iter_type;
	os << "[ ";
	for(vec_c_iter_type i = vec.begin(), isize = vec.end(); i != isize; ++i)
	{
		os << *i << " ";
	}
	os << "]";
	return os;
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits, typename Val,
//			template<typename _Val, std::size_t> class Base,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//				const yggr::math::vector2d<Val, Base>& vec);

} // namespace math
} // namespace yggr

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace math
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename Val,
		template<typename _Val, std::size_t> class Base> inline
std::size_t hash_value(const yggr::math::vector2d<Val, Base>& val)
{
	return val.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace math
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<typename Val,
		template<typename _Val, std::size_t> class Base>
struct hash<yggr::math::vector2d<Val, Base> >
{
	typedef yggr::math::vector2d<Val, Base> type;

	inline std::size_t operator()(const type& val) const
	{
		return val.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif //__YGGR_MATH_VECTOR2D_T_HPP__
