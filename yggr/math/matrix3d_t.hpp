//matrix3d_t.hpp

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

#ifndef __YGGR_MATH_MATRIX3D_T_HPP__
#define __YGGR_MATH_MATRIX3D_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/move/move.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/type_traits/operator_result.hpp>

#include <yggr/math/determinant.hpp>

#include <yggr/geometry/geo_plane.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array.hpp>

#include <boost/functional/hash/hash.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <cassert>
#include <ostream>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4244)
#endif //_MSC_VER

namespace yggr
{

namespace math
{

template<typename Val,
		template<typename _Val, std::size_t> class Array,
		template<typename _Val, std::size_t, std::size_t,
					template<typename __Val, std::size_t> class _Base> class Base>
class matrix3d;

} // namespace math
} // namespace yggr

namespace yggr
{
namespace detail
{

// fix error c2784 in operator_result
template<typename Val,
		template<typename _Val, std::size_t> class Array,
		template<typename _Val, std::size_t, std::size_t,
					template<typename __Val, std::size_t> class _Base> class Base>
struct operator_result_t_getter< math::matrix3d<Val, Array, Base> >
	: public traits<Val>
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
			template<typename _Val, std::size_t> class Array = ::yggr::array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base = determinant>
class matrix3d 
	: public Base<typename upper_signed<Val>::type, 4, 4, Array>
{
private:
	template<typename TVal,
			template<typename _Val, std::size_t> class TArray,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class TBase>
	friend class matrix3d;

public:
	YGGR_STATIC_CONSTANT(std::size_t, E_ROW_LENGTH = 4);
	YGGR_STATIC_CONSTANT(std::size_t, E_COL_LENGTH = 4);
	YGGR_STATIC_CONSTANT(std::size_t, E_MAX_RANK = 4);

public:
	typedef typename upper_signed<Val>::type value_type;

private:
	typedef value_type val_type;

public:
	typedef Array<val_type, E_COL_LENGTH> row_type;
	typedef Base<val_type, E_ROW_LENGTH, E_COL_LENGTH, Array> base_type;

private:	
	typedef matrix3d this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	matrix3d(void)
	{
		identity();
	}

	template<typename T00, typename T01, typename T02, typename T03,
				typename T10, typename T11, typename T12, typename T13,
				typename T20, typename T21, typename T22, typename T23,
				typename T30, typename T31, typename T32, typename T33 >
	matrix3d(const T00& t00, const T01& t01, const T02& t02, const T03& t03,
				const T10& t10, const T11& t11, const T12& t12, const T13& t13,
				const T20& t20, const T21& t21, const T22& t22, const T23& t23,
				const T30& t30, const T31& t31, const T32& t32, const T33& t33)
	{
		(*this)[0][0] = static_cast<val_type>(t00);
		(*this)[0][1] = static_cast<val_type>(t01);
		(*this)[0][2] = static_cast<val_type>(t02);
		(*this)[0][3] = static_cast<val_type>(t03);

		(*this)[1][0] = static_cast<val_type>(t10);
		(*this)[1][1] = static_cast<val_type>(t11);
		(*this)[1][2] = static_cast<val_type>(t12);
		(*this)[1][3] = static_cast<val_type>(t13);

		(*this)[2][0] = static_cast<val_type>(t20);
		(*this)[2][1] = static_cast<val_type>(t21);
		(*this)[2][2] = static_cast<val_type>(t22);
		(*this)[2][3] = static_cast<val_type>(t23);

		(*this)[3][0] = static_cast<val_type>(t30);
		(*this)[3][1] = static_cast<val_type>(t31);
		(*this)[3][2] = static_cast<val_type>(t32);
		(*this)[3][3] = static_cast<val_type>(t33);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OArray>
	matrix3d(const OArray<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
		: base_type(right)
	{
	}

	matrix3d(const val_type& right)
	{
		if(!right)
		{
			base_type::zero();
			return;
		}

		identity();
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase>
	matrix3d(const matrix3d<OVal, OArray, OBase>& right)
		: base_type(right)
	{
	}

	matrix3d(const base_type& right)
		: base_type(right)
	{
	}

	matrix3d(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	matrix3d(const this_type& right)
		: base_type(right)
	{
	}

	~matrix3d(void)
	{
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OArray>
	this_type& operator=(const OArray<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		base_type& base = *this;
		base = right;
		return *this;
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase>
	this_type& operator=(const matrix3d<OVal, OArray, OBase>& right)
	{
		base_type& base = *this;
		base = right;
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

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

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
	this_type& identity(void)
	{
		typedef typename base_type::iterator iter_type;

		for(iter_type i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			std::fill((*i).begin(), (*i).end(), val_type(0));
		}

		(*this)[0][0] = val_type(1);
		(*this)[1][1] = val_type(1);
		(*this)[2][2] = val_type(1);
		(*this)[3][3] = val_type(1);
		return *this;
	}

	bool is_identity(void) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				if(i == j)
				{
					if(cmper_type((*this)[i][j]) != val_type(1))
					{
						return false;
					}
				}
				else
				{
					if(cmper_type((*this)[i][j]) != val_type(0))
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	inline size_type rank(void) const
	{
		typedef flaw_comparer<val_type> cmper_type;
		cmper_type cmper(0);

		const base_type& base = *this;
		size_type r = static_cast<size_type>(cmper != base[0][0] || cmper != base[0][1] || cmper != base[0][2] || cmper != base[0][3])
					+ static_cast<size_type>(cmper != base[1][0] || cmper != base[1][1] || cmper != base[1][2] || cmper != base[1][3])
					+ static_cast<size_type>(cmper != base[2][0] || cmper != base[2][1] || cmper != base[2][2] || cmper != base[2][3])
					+ static_cast<size_type>(cmper != base[3][0] || cmper != base[3][1] || cmper != base[3][2] || cmper != base[3][3]);

		size_type l = static_cast<size_type>(cmper != base[0][0] || cmper != base[1][0] || cmper != base[2][0] || cmper != base[3][0])
					+ static_cast<size_type>(cmper != base[0][1] || cmper != base[1][1] || cmper != base[2][1] || cmper != base[3][1])
					+ static_cast<size_type>(cmper != base[0][2] || cmper != base[1][2] || cmper != base[2][2] || cmper != base[3][2])
					+ static_cast<size_type>(cmper != base[0][3] || cmper != base[1][3] || cmper != base[2][3] || cmper != base[3][3]);

		return (std::min)(r, l);
	}

	inline static size_type max_rank(void)
	{
		return E_MAX_RANK;
	}

public:
	inline const this_type& positive(void) const
	{
		return *this;
	}

	inline matrix3d<typename negate_operator_result<val_type>::type, Array, Base>
		operator-(void) const
	{
		typedef matrix3d<typename negate_operator_result<val_type>::type, Array, Base> ret_type;

		return 
			ret_type(
				-(*this)[0][0], -(*this)[0][1], -(*this)[0][2], -(*this)[0][3],
				-(*this)[1][0], -(*this)[1][1], -(*this)[1][2], -(*this)[1][3],
				-(*this)[2][0], -(*this)[2][1], -(*this)[2][2], -(*this)[2][3],
				-(*this)[3][0], -(*this)[3][1], -(*this)[3][2], -(*this)[3][3]);
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	typename
		boost::mpl::if_
		<
			boost::is_same<typename plus_operator_result<val_type, OVal>::type, val_type>,
			matrix3d<typename plus_operator_result<val_type, OVal>::type, Array, Base>,
			matrix3d<typename plus_operator_result<val_type, OVal>::type, OArray, OBase>
		>::type
		plus(const matrix3d<OVal, OArray, OBase>& right) const
	{
		typedef typename
			boost::mpl::if_
			<
				boost::is_same<typename plus_operator_result<val_type, OVal>::type, val_type>,
				matrix3d<typename plus_operator_result<val_type, OVal>::type, Array, Base>,
				matrix3d<typename plus_operator_result<val_type, OVal>::type, OArray, OBase>
			>::type ret_type;
		
		return 
			ret_type(
				(*this)[0][0] + right[0][0], 
				(*this)[0][1] + right[0][1], 
				(*this)[0][2] + right[0][2],
				(*this)[0][3] + right[0][3],

				(*this)[1][0] + right[1][0], 
				(*this)[1][1] + right[1][1], 
				(*this)[1][2] + right[1][2],
				(*this)[1][3] + right[1][3],

				(*this)[2][0] + right[2][0], 
				(*this)[2][1] + right[2][1], 
				(*this)[2][2] + right[2][2],
				(*this)[2][3] + right[2][3],

				(*this)[3][0] + right[3][0], 
				(*this)[3][1] + right[3][1], 
				(*this)[3][2] + right[3][2],
				(*this)[3][3] + right[3][3] );
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	this_type& plus_set(const matrix3d<OVal, OArray, OBase>& right)
	{
		(*this)[0][0] += right[0][0];
		(*this)[0][1] += right[0][1];
		(*this)[0][2] += right[0][2];
		(*this)[0][3] += right[0][3];

		(*this)[1][0] += right[1][0];
		(*this)[1][1] += right[1][1];
		(*this)[1][2] += right[1][2];
		(*this)[1][3] += right[1][3];

		(*this)[2][0] += right[2][0];
		(*this)[2][1] += right[2][1];
		(*this)[2][2] += right[2][2];
		(*this)[2][3] += right[2][3];

		(*this)[3][0] += right[3][0];
		(*this)[3][1] += right[3][1];
		(*this)[3][2] += right[3][2];
		(*this)[3][3] += right[3][3];

		return *this;
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	typename
		boost::mpl::if_
		<
			boost::is_same<typename minus_operator_result<val_type, OVal>::type, val_type>,
			matrix3d<typename minus_operator_result<val_type, OVal>::type, Array, Base>,
			matrix3d<typename minus_operator_result<val_type, OVal>::type, OArray, OBase>
		>::type
		minus(const matrix3d<OVal, OArray, OBase>& right) const
	{
		typedef typename
			boost::mpl::if_
			<
				boost::is_same<typename minus_operator_result<val_type, OVal>::type, val_type>,
				matrix3d<typename minus_operator_result<val_type, OVal>::type, Array, Base>,
				matrix3d<typename minus_operator_result<val_type, OVal>::type, OArray, OBase>
			>::type ret_type;

		return
			ret_type(
				(*this)[0][0] - right[0][0], 
				(*this)[0][1] - right[0][1], 
				(*this)[0][2] - right[0][2],
				(*this)[0][3] - right[0][3],

				(*this)[1][0] - right[1][0], 
				(*this)[1][1] - right[1][1], 
				(*this)[1][2] - right[1][2],
				(*this)[1][3] - right[1][3],

				(*this)[2][0] - right[2][0], 
				(*this)[2][1] - right[2][1], 
				(*this)[2][2] - right[2][2],
				(*this)[2][3] - right[2][3],

				(*this)[3][0] - right[3][0], 
				(*this)[3][1] - right[3][1], 
				(*this)[3][2] - right[3][2],
				(*this)[3][3] - right[3][3] );
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	this_type& minus_set(const matrix3d<OVal, OArray, OBase>& right)
	{
		(*this)[0][0] -= right[0][0];
		(*this)[0][1] -= right[0][1];
		(*this)[0][2] -= right[0][2];
		(*this)[0][3] -= right[0][3];

		(*this)[1][0] -= right[1][0];
		(*this)[1][1] -= right[1][1];
		(*this)[1][2] -= right[1][2];
		(*this)[1][3] -= right[1][3];

		(*this)[2][0] -= right[2][0];
		(*this)[2][1] -= right[2][1];
		(*this)[2][2] -= right[2][2];
		(*this)[2][3] -= right[2][3];

		(*this)[3][0] -= right[3][0];
		(*this)[3][1] -= right[3][1];
		(*this)[3][2] -= right[3][2];
		(*this)[3][3] -= right[3][3];

		return *this;
	}

	template<typename OVal> inline
	matrix3d<typename multiplies_operator_result<val_type, OVal>::type, Array, Base>
		multiplies(const OVal& right) const
	{
		typedef matrix3d<typename multiplies_operator_result<val_type, OVal>::type, Array, Base> ret_type;

		return 
			ret_type(
				(*this)[0][0] * right, (*this)[0][1] * right, (*this)[0][2] * right, (*this)[0][3] * right,
				(*this)[1][0] * right, (*this)[1][1] * right, (*this)[1][2] * right, (*this)[1][3] * right,
				(*this)[2][0] * right, (*this)[2][1] * right, (*this)[2][2] * right, (*this)[2][3] * right,
				(*this)[3][0] * right, (*this)[3][1] * right, (*this)[3][2] * right, (*this)[3][3] * right);
	}

	template<typename OVal> inline
	this_type& multiplies_set(const OVal& right)
	{
		(*this)[0][0] *= right;
		(*this)[0][1] *= right;
		(*this)[0][2] *= right;
		(*this)[0][3] *= right;

		(*this)[1][0] *= right;
		(*this)[1][1] *= right;
		(*this)[1][2] *= right;
		(*this)[1][3] *= right;

		(*this)[2][0] *= right;
		(*this)[2][1] *= right;
		(*this)[2][2] *= right;
		(*this)[2][3] *= right;

		(*this)[3][0] *= right;
		(*this)[3][1] *= right;
		(*this)[3][2] *= right;
		(*this)[3][3] *= right;

		return *this;
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	typename
		boost::mpl::if_
		<
			boost::is_same<typename multiplies_operator_result<val_type, OVal>::type, val_type>,
			matrix3d<typename multiplies_operator_result<val_type, OVal>::type, Array, Base>,
			matrix3d<typename multiplies_operator_result<val_type, OVal>::type, OArray, OBase>
		>::type
		multiplies(const matrix3d<OVal, OArray, OBase>& right) const
	{
		typedef typename
			boost::mpl::if_
			<
				boost::is_same<typename multiplies_operator_result<val_type, OVal>::type, val_type>,
				matrix3d<typename multiplies_operator_result<val_type, OVal>::type, Array, Base>,
				matrix3d<typename multiplies_operator_result<val_type, OVal>::type, OArray, OBase>
			>::type ret_type;

		return 
			ret_type(
				(*this)[0][0] * right[0][0] + (*this)[0][1] * right[1][0] + (*this)[0][2] * right[2][0] + (*this)[0][3] * right[3][0],	// 00
				(*this)[0][0] * right[0][1] + (*this)[0][1] * right[1][1] + (*this)[0][2] * right[2][1] + (*this)[0][3] * right[3][1],	// 01
				(*this)[0][0] * right[0][2] + (*this)[0][1] * right[1][2] + (*this)[0][2] * right[2][2] + (*this)[0][3] * right[3][2],	// 02
				(*this)[0][0] * right[0][3] + (*this)[0][1] * right[1][3] + (*this)[0][2] * right[2][3] + (*this)[0][3] * right[3][3],	// 03

				(*this)[1][0] * right[0][0] + (*this)[1][1] * right[1][0] + (*this)[1][2] * right[2][0] + (*this)[1][3] * right[3][0],	// 10
				(*this)[1][0] * right[0][1] + (*this)[1][1] * right[1][1] + (*this)[1][2] * right[2][1] + (*this)[1][3] * right[3][1],	// 11
				(*this)[1][0] * right[0][2] + (*this)[1][1] * right[1][2] + (*this)[1][2] * right[2][2] + (*this)[1][3] * right[3][2],	// 12
				(*this)[1][0] * right[0][3] + (*this)[1][1] * right[1][3] + (*this)[1][2] * right[2][3] + (*this)[1][3] * right[3][3],	// 13

				(*this)[2][0] * right[0][0] + (*this)[2][1] * right[1][0] + (*this)[2][2] * right[2][0] + (*this)[2][3] * right[3][0],	// 20
				(*this)[2][0] * right[0][1] + (*this)[2][1] * right[1][1] + (*this)[2][2] * right[2][1] + (*this)[2][3] * right[3][1],	// 21
				(*this)[2][0] * right[0][2] + (*this)[2][1] * right[1][2] + (*this)[2][2] * right[2][2] + (*this)[2][3] * right[3][2],	// 22
				(*this)[2][0] * right[0][3] + (*this)[2][1] * right[1][3] + (*this)[2][2] * right[2][3] + (*this)[2][3] * right[3][3],	// 23

				(*this)[3][0] * right[0][0] + (*this)[3][1] * right[1][0] + (*this)[3][2] * right[2][0] + (*this)[3][3] * right[3][0],	// 30
				(*this)[3][0] * right[0][1] + (*this)[3][1] * right[1][1] + (*this)[3][2] * right[2][1] + (*this)[3][3] * right[3][1],	// 31
				(*this)[3][0] * right[0][2] + (*this)[3][1] * right[1][2] + (*this)[3][2] * right[2][2] + (*this)[3][3] * right[3][2],	// 32
				(*this)[3][0] * right[0][3] + (*this)[3][1] * right[1][3] + (*this)[3][2] * right[2][3] + (*this)[3][3] * right[3][3]	// 33
			);
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	this_type& multiplies_set(const matrix3d<OVal, OArray, OBase>& right)
	{
		this_type tmp(val_type(0));

		tmp[0][0] = (*this)[0][0] * right[0][0] + (*this)[0][1] * right[1][0] + (*this)[0][2] * right[2][0] + (*this)[0][3] * right[3][0];
		tmp[0][1] = (*this)[0][0] * right[0][1] + (*this)[0][1] * right[1][1] + (*this)[0][2] * right[2][1] + (*this)[0][3] * right[3][1];
		tmp[0][2] = (*this)[0][0] * right[0][2] + (*this)[0][1] * right[1][2] + (*this)[0][2] * right[2][2] + (*this)[0][3] * right[3][2];
		tmp[0][3] = (*this)[0][0] * right[0][3] + (*this)[0][1] * right[1][3] + (*this)[0][2] * right[2][3] + (*this)[0][3] * right[3][3];

		tmp[1][0] = (*this)[1][0] * right[0][0] + (*this)[1][1] * right[1][0] + (*this)[1][2] * right[2][0] + (*this)[1][3] * right[3][0];
		tmp[1][1] = (*this)[1][0] * right[0][1] + (*this)[1][1] * right[1][1] + (*this)[1][2] * right[2][1] + (*this)[1][3] * right[3][1];
		tmp[1][2] = (*this)[1][0] * right[0][2] + (*this)[1][1] * right[1][2] + (*this)[1][2] * right[2][2] + (*this)[1][3] * right[3][2];
		tmp[1][3] = (*this)[1][0] * right[0][3] + (*this)[1][1] * right[1][3] + (*this)[1][2] * right[2][3] + (*this)[1][3] * right[3][3];

		tmp[2][0] = (*this)[2][0] * right[0][0] + (*this)[2][1] * right[1][0] + (*this)[2][2] * right[2][0] + (*this)[2][3] * right[3][0];
		tmp[2][1] = (*this)[2][0] * right[0][1] + (*this)[2][1] * right[1][1] + (*this)[2][2] * right[2][1] + (*this)[2][3] * right[3][1];
		tmp[2][2] = (*this)[2][0] * right[0][2] + (*this)[2][1] * right[1][2] + (*this)[2][2] * right[2][2] + (*this)[2][3] * right[3][2];
		tmp[2][3] = (*this)[2][0] * right[0][3] + (*this)[2][1] * right[1][3] + (*this)[2][2] * right[2][3] + (*this)[2][3] * right[3][3];

		tmp[3][0] = (*this)[3][0] * right[0][0] + (*this)[3][1] * right[1][0] + (*this)[3][2] * right[2][0] + (*this)[3][3] * right[3][0];
		tmp[3][1] = (*this)[3][0] * right[0][1] + (*this)[3][1] * right[1][1] + (*this)[3][2] * right[2][1] + (*this)[3][3] * right[3][1];
		tmp[3][2] = (*this)[3][0] * right[0][2] + (*this)[3][1] * right[1][2] + (*this)[3][2] * right[2][2] + (*this)[3][3] * right[3][2];
		tmp[3][3] = (*this)[3][0] * right[0][3] + (*this)[3][1] * right[1][3] + (*this)[3][2] * right[2][3] + (*this)[3][3] * right[3][3];

		(*this) = tmp;
		return *this;
	}

	template<typename OVal> inline
	matrix3d<typename divides_operator_result<val_type, OVal>::type, Array, Base>
		divides(const OVal& right) const
	{
		typedef matrix3d<typename divides_operator_result<val_type, OVal>::type, Array, Base> ret_type;

		return 
			ret_type(
				(*this)[0][0] / right, (*this)[0][1] / right, (*this)[0][2] / right, (*this)[0][3] / right,
				(*this)[1][0] / right, (*this)[1][1] / right, (*this)[1][2] / right, (*this)[1][3] / right,
				(*this)[2][0] / right, (*this)[2][1] / right, (*this)[2][2] / right, (*this)[2][3] / right,
				(*this)[3][0] / right, (*this)[3][1] / right, (*this)[3][2] / right, (*this)[3][3] / right);
	}

	template<typename OVal> inline
	this_type& divides_set(const OVal& right)
	{
		(*this)[0][0] /= right;
		(*this)[0][1] /= right;
		(*this)[0][2] /= right;
		(*this)[0][3] /= right;

		(*this)[1][0] /= right;
		(*this)[1][1] /= right;
		(*this)[1][2] /= right;
		(*this)[1][3] /= right;

		(*this)[2][0] /= right;
		(*this)[2][1] /= right;
		(*this)[2][2] /= right;
		(*this)[2][3] /= right;

		(*this)[3][0] /= right;
		(*this)[3][1] /= right;
		(*this)[3][2] /= right;
		(*this)[3][3] /= right;

		return *this;
	}

	template<typename OVal> inline
	matrix3d<typename modulus_operator_result<val_type, OVal>::type, Array, Base>
		modulus(const OVal& right) const
	{
		typedef matrix3d<typename modulus_operator_result<val_type, OVal>::type, Array, Base> ret_type;

		return 
			ret_type(
				::yggr::math::modulus((*this)[0][0], right), 
				::yggr::math::modulus((*this)[0][1], right), 
				::yggr::math::modulus((*this)[0][2], right), 
				::yggr::math::modulus((*this)[0][3], right),

				::yggr::math::modulus((*this)[1][0], right), 
				::yggr::math::modulus((*this)[1][1], right), 
				::yggr::math::modulus((*this)[1][2], right),
				::yggr::math::modulus((*this)[1][2], right),

				::yggr::math::modulus((*this)[2][0], right), 
				::yggr::math::modulus((*this)[2][1], right), 
				::yggr::math::modulus((*this)[2][2], right),
				::yggr::math::modulus((*this)[2][3], right),

				::yggr::math::modulus((*this)[3][0], right), 
				::yggr::math::modulus((*this)[3][1], right), 
				::yggr::math::modulus((*this)[3][2], right),
				::yggr::math::modulus((*this)[3][3], right) );
	}

	template<typename OVal> inline
	this_type& modulus_set(const OVal& right)
	{
		::yggr::math::modulus_set((*this)[0][0], right);
		::yggr::math::modulus_set((*this)[0][1], right);
		::yggr::math::modulus_set((*this)[0][2], right);
		::yggr::math::modulus_set((*this)[0][3], right);

		::yggr::math::modulus_set((*this)[1][0], right);
		::yggr::math::modulus_set((*this)[1][1], right);
		::yggr::math::modulus_set((*this)[1][2], right);
		::yggr::math::modulus_set((*this)[1][3], right);

		::yggr::math::modulus_set((*this)[2][0], right);
		::yggr::math::modulus_set((*this)[2][1], right);
		::yggr::math::modulus_set((*this)[2][2], right);
		::yggr::math::modulus_set((*this)[2][3], right);

		::yggr::math::modulus_set((*this)[3][0], right);
		::yggr::math::modulus_set((*this)[3][1], right);
		::yggr::math::modulus_set((*this)[3][2], right);
		::yggr::math::modulus_set((*this)[3][3], right);

		return *this;
	}

public:
	inline matrix3d<typename upper_float<val_type>::type, Array, Base> 
		inverse(void) const
	{
		typedef typename upper_float<val_type>::type float_type;
		typedef matrix3d<float_type> ret_type;
		typedef flaw_comparer<float_type> float_cmper_type;

		float_type d = (*this).det();

		if(float_cmper_type(d) == float_type())
		{
			return ret_type(float_type(0));
		}

		ret_type ret(this_type::adjugate());

		if(ret.rank() != E_COL_LENGTH)
		{
			return ret_type(float_type(0));
		}

		ret /= d;

		return ret;
	}

	inline this_type adjugate(void) const
	{
		return 
			this_type(
				(*this)[1][1] * (*this)[2][2] * (*this)[3][3]
				+ (*this)[1][2] * (*this)[2][3] * (*this)[3][1]
				+ (*this)[1][3] * (*this)[2][1] * (*this)[3][2]
				- (*this)[1][3] * (*this)[2][2] * (*this)[3][1]
				- (*this)[1][2] * (*this)[2][1] * (*this)[3][3]
				- (*this)[1][1] * (*this)[2][3] * (*this)[3][2],	// 00

				-((*this)[0][1] * (*this)[2][2] * (*this)[3][3]
				+ (*this)[0][2] * (*this)[2][3] * (*this)[3][1]
				+ (*this)[0][3] * (*this)[2][1] * (*this)[3][2]
				- (*this)[0][3] * (*this)[2][2] * (*this)[3][1]
				- (*this)[0][2] * (*this)[2][1] * (*this)[3][3]
				- (*this)[0][1] * (*this)[2][3] * (*this)[3][2]),	// 01

				(*this)[0][1] * (*this)[1][2] * (*this)[3][3]
				+ (*this)[0][2] * (*this)[1][3] * (*this)[3][1]
				+ (*this)[0][3] * (*this)[1][1] * (*this)[3][2]
				- (*this)[0][3] * (*this)[1][2] * (*this)[3][1]
				- (*this)[0][2] * (*this)[1][1] * (*this)[3][3]
				- (*this)[0][1] * (*this)[1][3] * (*this)[3][2],	// 02

				-((*this)[0][1] * (*this)[1][2] * (*this)[2][3]
				+ (*this)[0][2] * (*this)[1][3] * (*this)[2][1]
				+ (*this)[0][3] * (*this)[1][1] * (*this)[2][2]
				- (*this)[0][3] * (*this)[1][2] * (*this)[2][1]
				- (*this)[0][2] * (*this)[1][1] * (*this)[2][3]
				- (*this)[0][1] * (*this)[1][3] * (*this)[2][2]),	// 03
				
				//---------------------------------------------------------------------

				-((*this)[1][0] * (*this)[2][2] * (*this)[3][3]
				+ (*this)[1][2] * (*this)[2][3] * (*this)[3][0]
				+ (*this)[1][3] * (*this)[2][0] * (*this)[3][2]
				- (*this)[1][3] * (*this)[2][2] * (*this)[3][0]
				- (*this)[1][2] * (*this)[2][0] * (*this)[3][3]
				- (*this)[1][0] * (*this)[2][3] * (*this)[3][2]),	// 10

				(*this)[0][0] * (*this)[2][2] * (*this)[3][3]
				+ (*this)[0][2] * (*this)[2][3] * (*this)[3][0]
				+ (*this)[0][3] * (*this)[2][0] * (*this)[3][2]
				- (*this)[0][3] * (*this)[2][2] * (*this)[3][0]
				- (*this)[0][2] * (*this)[2][0] * (*this)[3][3]
				- (*this)[0][0] * (*this)[2][3] * (*this)[3][2],	// 11

				-((*this)[0][0] * (*this)[1][2] * (*this)[3][3]
				+ (*this)[0][2] * (*this)[1][3] * (*this)[3][0]
				+ (*this)[0][3] * (*this)[1][0] * (*this)[3][2]
				- (*this)[0][3] * (*this)[1][2] * (*this)[3][0]
				- (*this)[0][2] * (*this)[1][0] * (*this)[3][3]
				- (*this)[0][0] * (*this)[1][3] * (*this)[3][2]),	// 12

				(*this)[0][0] * (*this)[1][2] * (*this)[2][3]
				+ (*this)[0][2] * (*this)[1][3] * (*this)[2][0]
				+ (*this)[0][3] * (*this)[1][0] * (*this)[2][2]
				- (*this)[0][3] * (*this)[1][2] * (*this)[2][0]
				- (*this)[0][2] * (*this)[1][0] * (*this)[2][3]
				- (*this)[0][0] * (*this)[1][3] * (*this)[2][2],	// 13
				
				//--------------------------------------------------------------------

				(*this)[1][0] * (*this)[2][1] * (*this)[3][3]
				+ (*this)[1][1] * (*this)[2][3] * (*this)[3][0]
				+ (*this)[1][3] * (*this)[2][0] * (*this)[3][1]
				- (*this)[1][3] * (*this)[2][1] * (*this)[3][0]
				- (*this)[1][1] * (*this)[2][0] * (*this)[3][3]
				- (*this)[1][0] * (*this)[2][3] * (*this)[3][1],	// 20

				-((*this)[0][0] * (*this)[2][1] * (*this)[3][3]
				+ (*this)[0][1] * (*this)[2][3] * (*this)[3][0]
				+ (*this)[0][3] * (*this)[2][0] * (*this)[3][1]
				- (*this)[0][3] * (*this)[2][1] * (*this)[3][0]
				- (*this)[0][1] * (*this)[2][0] * (*this)[3][3]
				- (*this)[0][0] * (*this)[2][3] * (*this)[3][1]),	// 21

				(*this)[0][0] * (*this)[1][1] * (*this)[3][3]
				+ (*this)[0][1] * (*this)[1][3] * (*this)[3][0]
				+ (*this)[0][3] * (*this)[1][0] * (*this)[3][1]
				- (*this)[0][3] * (*this)[1][1] * (*this)[3][0]
				- (*this)[0][1] * (*this)[1][0] * (*this)[3][3]
				- (*this)[0][0] * (*this)[1][3] * (*this)[3][1],	// 22

				-((*this)[0][0] * (*this)[1][1] * (*this)[2][3]
				+ (*this)[0][1] * (*this)[1][3] * (*this)[2][0]
				+ (*this)[0][3] * (*this)[1][0] * (*this)[2][1]
				- (*this)[0][3] * (*this)[1][1] * (*this)[2][0]
				- (*this)[0][1] * (*this)[1][0] * (*this)[2][3]
				- (*this)[0][0] * (*this)[1][3] * (*this)[2][1]),	// 23
				
				//-----------------------------------------------------------------

				-((*this)[1][0] * (*this)[2][1] * (*this)[3][2]
				+ (*this)[1][1] * (*this)[2][2] * (*this)[3][0]
				+ (*this)[1][2] * (*this)[2][0] * (*this)[3][1]
				- (*this)[1][2] * (*this)[2][1] * (*this)[3][0]
				- (*this)[1][1] * (*this)[2][0] * (*this)[3][2]
				- (*this)[1][0] * (*this)[2][2] * (*this)[3][1]),	// 30

				(*this)[0][0] * (*this)[2][1] * (*this)[3][2]
				+ (*this)[0][1] * (*this)[2][2] * (*this)[3][0]
				+ (*this)[0][2] * (*this)[2][0] * (*this)[3][1]
				- (*this)[0][2] * (*this)[2][1] * (*this)[3][0]
				- (*this)[0][1] * (*this)[2][0] * (*this)[3][2]
				- (*this)[0][0] * (*this)[2][2] * (*this)[3][1],	// 31

				-((*this)[0][0] * (*this)[1][1] * (*this)[3][2]
				+ (*this)[0][1] * (*this)[1][2] * (*this)[3][0]
				+ (*this)[0][2] * (*this)[1][0] * (*this)[3][1]
				- (*this)[0][2] * (*this)[1][1] * (*this)[3][0]
				- (*this)[0][1] * (*this)[1][0] * (*this)[3][2]
				- (*this)[0][0] * (*this)[1][2] * (*this)[3][1]),	// 32

				(*this)[0][0] * (*this)[1][1] * (*this)[2][2]
				+ (*this)[0][1] * (*this)[1][2] * (*this)[2][0]
				+ (*this)[0][2] * (*this)[1][0] * (*this)[2][1]
				- (*this)[0][2] * (*this)[1][1] * (*this)[2][0]
				- (*this)[0][1] * (*this)[1][0] * (*this)[2][2]
				- (*this)[0][0] * (*this)[1][2] * (*this)[2][1]		// 33
			);
	}

	inline this_type transpose(void) const
	{
		this_type tmp(*this);

		yggr::swap(tmp[0][1], tmp[1][0]);
		yggr::swap(tmp[0][2], tmp[2][0]);
		yggr::swap(tmp[0][3], tmp[3][0]);
		yggr::swap(tmp[1][2], tmp[2][1]);
		yggr::swap(tmp[1][3], tmp[3][1]);
		yggr::swap(tmp[2][3], tmp[3][2]);

		return tmp;
	}

	inline std::size_t hash(void) const
	{
		const base_type& base = *this;
		std::size_t seed = 0;
		boost::hash_combine(seed, "matrix3d");
		boost::hash_combine(seed, base);
		return seed;
	}

public:
	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	bool equal_to(const matrix3d<OVal, OArray, OBase>& right) const
	{
		return base_type::equal_to(right);
	}

	template<typename OVal,
				template<typename _Val, std::size_t> class OArray,
				template<typename _Val, std::size_t, std::size_t,
							template<typename __Val, std::size_t> class _Base> class OBase> inline
	bool not_equal_to(const matrix3d<OVal, OArray, OBase>& right) const
	{
		return !base_type::equal_to(right);
	}

public:
	template<typename OVal1, typename OVal2, typename OVal3> inline
	static 
		matrix3d
		<
			typename
				multiplies_operator_result
				<
					typename 
						multiplies_operator_result
						<
							typename multiplies_operator_result<val_type, OVal1>::type, 
							OVal2
						>::type,
					OVal3
				>::type,
			Array, 
			Base
		>
		make_translate(const OVal1& x, const OVal2& y, const OVal3& z)
	{
		typedef 
			matrix3d
			<
				typename
					multiplies_operator_result
					<
						typename 
							multiplies_operator_result
							<
								typename multiplies_operator_result<val_type, OVal1>::type, 
								OVal2
							>::type,
						OVal3
					>::type,
				Array, 
				Base
			> ret_type;

		ret_type mat;
		mat[3][0] = x;
		mat[3][1] = y;
		mat[3][2] = z;
		return mat;
	}

	inline static matrix3d<typename upper_float<val_type>::type, Array, Base>
		make_rotate_x(typename upper_float<val_type>::type& rad)
	{
		typedef matrix3d<typename upper_float<val_type>::type, Array, Base> rst_type;
		rst_type mat;
		mat[1][1] = std::cos(rad);
		mat[1][2] = std::sin(rad);
		mat[2][1] = -mat[1][2];
		mat[2][2] = mat[1][1];

		return mat;
	}

	static matrix3d<typename upper_float<val_type>::type, Array, Base>
		make_rotate_y(typename upper_float<val_type>::type& rad)
	{
		typedef matrix3d<typename upper_float<val_type>::type, Array, Base> rst_type;
		rst_type mat;
		mat[0][0] = std::cos(rad);
		mat[0][2] = -std::sin(rad);
		mat[2][0] = -mat[0][2];
		mat[2][2] = mat[0][0];

		return mat;
	}

	inline static matrix3d<typename upper_float<val_type>::type, Array, Base>
		make_rotate_z(typename upper_float<val_type>::type& rad)
	{
		typedef matrix3d<typename upper_float<val_type>::type, Array, Base> rst_type;
		rst_type mat;
		mat[0][0] = std::cos(rad);
		mat[0][1] = std::sin(rad);
		mat[1][0] = -mat[0][0];
		mat[1][1] = mat[0][0];

		return mat;
	}

	inline static matrix3d<typename upper_float<val_type>::type, Array, Base>
		make_rotate_yxz(typename upper_float<val_type>::type& xrad,
							typename upper_float<val_type>::type& yrad,
							typename upper_float<val_type>::type& zrad)
	{
		return make_rotate_y(yrad) * make_rotate_x(xrad) * make_rotate_z(zrad);
	}

	template<typename OVal1, typename OVal2, typename OVal3> inline
	static 
		matrix3d
		<
			typename
				multiplies_operator_result
				<
					typename 
						multiplies_operator_result
						<
							typename multiplies_operator_result<val_type, OVal1>::type, 
							OVal2
						>::type,
					OVal3
				>::type,
			Array, 
			Base
		>
		make_scale(const OVal1& sx, const OVal2& sy, const OVal3& sz)
	{
		typedef 
			matrix3d
			<
				typename
					multiplies_operator_result
					<
						typename 
							multiplies_operator_result
							<
								typename multiplies_operator_result<val_type, OVal1>::type, 
								OVal2
							>::type,
						OVal3
					>::type,
				Array, 
				Base
			> ret_type;

		ret_type mat;
		mat[0][0] = sx;
		mat[1][1] = sy;
		mat[2][2] = sz;
		return mat;
	}

	template<typename OVal1, typename OVal2, typename OVal3,
				template<typename _Val, std::size_t> class OArray1,
				template<typename _Val, std::size_t> class OArray2,
				template<typename _Val, std::size_t> class OArray3> inline
	static matrix3d<typename upper_float<val_type>::type, Array, Base>
		make_reflect(const vector3d<OVal1, OArray1>& pos1,
						const vector3d<OVal2, OArray2>& pos2, 
						const vector3d<OVal3, OArray3>& pos3)
	{
		typedef typename upper_float<val_type>::type float_type;
		typedef matrix3d<float_type, Array, Base> rst_type;
		typedef vector3d<float_type, Array> vec_type;
		typedef geometry::geo_plane<float_type, Array> plane_type;

		plane_type plane(pos1, pos2, pos3);

		return this_type::make_reflect(plane);
	}

	template<typename OVal, template<typename _Val, std::size_t> class OArray> inline
	static matrix3d<typename upper_float<val_type>::type, Array, Base>
		make_reflect(const geometry::geo_plane<OVal, OArray>& plane)
	{
		typedef typename upper_float<val_type>::type float_type;
		typedef matrix3d<float_type, Array, Base> rst_type;
		typedef vector3d<float_type, Array> vec_type;
		typedef geometry::geo_plane<float_type, Array> plane_type;
		typedef flaw_comparer<float_type> cmper_type;

		if(!plane.is_plane())
		{
			return rst_type();
		}

		vec_type vec_n = plane.normal();
		float_type vec_n_len_sq = vec_n.length_sq();
		rst_type mat;
		float_type c0 = float_type(), c1 = float_type(), c2 = float_type(), c3 = float_type();

		c0 = float_type(2.0) * plane.a * vec_n[0];
		c1 = float_type(2.0) * plane.b * vec_n[0];
		c2 = float_type(2.0) * plane.c * vec_n[0];
		c3 = float_type(2.0) * plane.d * vec_n[0];

		mat[0][0] = (vec_n_len_sq - c0) / vec_n_len_sq;
		mat[1][0] = -(c1 / vec_n_len_sq);
		mat[2][0] = -(c2 / vec_n_len_sq);
		mat[3][0] = -(c3 / vec_n_len_sq);

		c0 = float_type(2.0) * plane.a * vec_n[1];
		c1 = float_type(2.0) * plane.b * vec_n[1];
		c2 = float_type(2.0) * plane.c * vec_n[1];
		c3 = float_type(2.0) * plane.d * vec_n[1];

		mat[0][1] = -(c0 / vec_n_len_sq);
		mat[1][1] = (vec_n_len_sq - c1) / vec_n_len_sq;
		mat[2][1] = -(c2 / vec_n_len_sq);
		mat[3][1] = -(c3 / vec_n_len_sq);

		c0 = float_type(2.0) * plane.a * vec_n[2];
		c1 = float_type(2.0) * plane.b * vec_n[2];
		c2 = float_type(2.0) * plane.c * vec_n[2];
		c3 = float_type(2.0) * plane.d * vec_n[2];

		mat[0][2] = -(c0 / vec_n_len_sq);
		mat[1][2] = -(c1 / vec_n_len_sq);
		mat[2][2] = (vec_n_len_sq - c2) / vec_n_len_sq;
		mat[3][2] = -(c3 / vec_n_len_sq);

		return mat;
	}

#ifdef _DEBUG
	//stand adjugate function check used //don't delete this function
	this_type debug_adjugate(void) const
	{
		typedef determinant<val_type, E_ROW_LENGTH - 1, E_COL_LENGTH - 1, Array> det2_type;

		this_type rst(0);
		size_type lose_k = 0;
		size_type lose_l = 0;

		det2_type det_tmp;

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				for(size_type k = 0, ki = 0, ksize = E_ROW_LENGTH; k != ksize; ++k)
				{
					if(k == lose_k)
					{
						continue;
					}
					for(size_type l = 0, lj = 0, lsize = E_COL_LENGTH; l != lsize; ++l)
					{
						if(l == lose_l)
						{
							continue;
						}

						det_tmp[ki][lj] = (*this)[k][l];
						++lj;
					}

					++ki;
				}

				rst[i][j] = det_tmp.det() * this_type::prv_debug_adjugate_i_j_sign(i, j);
				++ lose_k;
			}

			lose_k = 0;
			++lose_l;
		}

		return rst;
	}

private:
	inline typename upper_signed<val_type>::type
		prv_debug_adjugate_i_j_sign(const size_type& i, const size_type& j) const
	{
		// o + o == e == (o - 1) + (o - 1)
		// e + e == e == (e - 1) + (e - 1)
		// o + e == o == (o - 1) + (e - 1)
		// e + o == o == (e - 1) + (o - 1)
		// => sign(idxi + 1, idxj + 1) == sign(idxi, idxj) == sign(idxi - 1, idxj - 1)

		return math::is_even_number(i + j)? val_type(1) : val_type(-1);
	}

#endif // _DEBUG

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"matrix3d_data",
				(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

// non-member foo
// positive
template<typename Val,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
const matrix3d<Val, Array, Base>& 
	operator+(const matrix3d<Val, Array, Base>& val)
{
	return val.positive();
}

//negate
template<typename Val,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
matrix3d<typename negate_operator_result<Val>::type, Array, Base> 
	operator-(const matrix3d<Val, Array, Base>& val)
{
	return val.negate();
}

// plus
template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class ArrayL,
			template<typename _Val, std::size_t> class ArrayR,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
typename
	boost::mpl::if_
	<
		boost::is_same<typename plus_operator_result<ValL, ValR>::type, ValL>,
		matrix3d<typename plus_operator_result<ValL, ValR>::type, ArrayL, BaseL>,
		matrix3d<typename plus_operator_result<ValL, ValR>::type, ArrayR, BaseR>
	>::type
	operator+(const matrix3d<ValL, ArrayL, BaseL>& l, const matrix3d<ValR, ArrayR, BaseR>& r)
{
	return l.plus(r);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class ArrayL,
			template<typename _Val, std::size_t> class ArrayR,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
matrix3d<ValL, ArrayL, BaseL>&
	operator+=(matrix3d<ValL, ArrayL, BaseL>& l, const matrix3d<ValR, ArrayR, BaseR>& r)
{
	return l.plus_set(r);
}

// minus
template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class ArrayL,
			template<typename _Val, std::size_t> class ArrayR,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
typename
	boost::mpl::if_
	<
		boost::is_same<typename minus_operator_result<ValL, ValR>::type, ValL>,
		matrix3d<typename minus_operator_result<ValL, ValR>::type, ArrayL, BaseL>,
		matrix3d<typename minus_operator_result<ValL, ValR>::type, ArrayR, BaseR>
	>::type
	operator-(const matrix3d<ValL, ArrayL, BaseL>& l, const matrix3d<ValR, ArrayR, BaseR>& r)
{
	return l.minus(r);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class ArrayL,
			template<typename _Val, std::size_t> class ArrayR,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
matrix3d<ValL, ArrayL, BaseL>&
	operator-=(matrix3d<ValL, ArrayL, BaseL>& l, const matrix3d<ValR, ArrayR, BaseR>& r)
{
	return l.minus_set(r);
}

// mul

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
matrix3d<typename multiplies_operator_result<ValL, ValR>::type, Array, Base>
	operator*(const ValL& l, const matrix3d<ValR, Array, Base>& r)
{
	return r.multiplies(l);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
matrix3d<typename multiplies_operator_result<ValL, ValR>::type, Array, Base> 
	operator*(const matrix3d<ValL, Array, Base>& l, const ValR& r)
{
	return l.multiplies(r);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
matrix3d<ValL, Array, Base>&
	operator*=(matrix3d<ValL, Array, Base>& l, const ValR& r)
{
	return l.multiplies_set(r);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class ArrayL,
			template<typename _Val, std::size_t> class ArrayR,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
typename
	boost::mpl::if_
	<
		boost::is_same<typename multiplies_operator_result<ValL, ValR>::type, ValL>,
		matrix3d<typename multiplies_operator_result<ValL, ValR>::type, ArrayL, BaseL>,
		matrix3d<typename multiplies_operator_result<ValL, ValR>::type, ArrayR, BaseR>
	>::type
	operator*(const matrix3d<ValL, ArrayL, BaseL>& l, const matrix3d<ValR, ArrayR, BaseR>& r)
{
	return l.multiplies(r);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class ArrayL,
			template<typename _Val, std::size_t> class ArrayR,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
matrix3d<ValL, ArrayL, BaseL>&
	operator*=(matrix3d<ValL, ArrayL, BaseL>& l, const matrix3d<ValR, ArrayR, BaseR>& r)
{
	return l.multiplies_set(r);
}

// div

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
matrix3d<typename divides_operator_result<ValL, ValR>::type, Array, Base> 
	operator/(const matrix3d<ValL, Array, Base>& l, const ValR& r)
{
	return l.divides(r);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
matrix3d<ValL, Array, Base>&
	operator/=(matrix3d<ValL, Array, Base>& l, const ValR& r)
{
	return l.divides_set(r);
}

// mod
template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
matrix3d<typename modulus_operator_result<ValL, ValR>::type, Array, Base> 
	operator%(const matrix3d<ValL, Array, Base>& l, const ValR& r)
{
	return l.modulus(r);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class Base> inline
matrix3d<ValL, Array, Base>&
	operator%=(matrix3d<ValL, Array, Base>& l, const ValR& r)
{
	return l.modulus_set(r);
}

// equal_to

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class ArrayL,
			template<typename _Val, std::size_t> class ArrayR,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
bool operator==(const matrix3d<ValL, ArrayL, BaseL>& l, const matrix3d<ValR, ArrayR, BaseR>& r)
{
	return l.equal_to(r);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class ArrayL,
			template<typename _Val, std::size_t> class ArrayR,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
bool operator!=(const matrix3d<ValL, ArrayL, BaseL>& l, const matrix3d<ValR, ArrayR, BaseR>& r)
{
	return !l.equal_to(r);
}

} // namespace math
} // namespace yggr

namespace yggr
{

template<typename Val,
			template<typename _Val, std::size_t>
			class Array,
			template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t>
				class _Base>
			class Base>
struct upper_float< math::matrix3d<Val, Array, Base> >
{
	typedef typename 
		math::matrix3d
		<
			typename upper_float<Val>::type, Array, Base 
		> type;
};

template<typename Val,
			template<typename _Val, std::size_t>
			class Array,
			template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t>
				class _Base>
			class Base>
struct upper_signed< math::matrix3d< Val, Array, Base > >
{
	typedef typename 
		math::matrix3d
		<
			typename upper_signed< Val >::type, Array, Base 
		> type;
};


} // namespace yggr

namespace yggr
{
namespace math
{
namespace swap_support
{

template<typename Val,
	template<typename _Val, std::size_t> class Array,
	template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t> class _Base> class Base> inline
void swap(matrix3d<Val, Array, Base>& l, matrix3d<Val, Array, Base>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace math
} // namespace yggr

namespace std
{
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
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t> class _Base> class Base>
std::basic_ostream<Char, Traits>&
		operator<<(std::basic_ostream<Char, Traits>& os,
						const matrix3d<Val, Array, Base>& mat)
{
	typedef Val val_type;
	typedef yggr::math::matrix3d<Val> mat_type;


	for(yggr::size_type i = 0, isize = mat_type::row_size(); i != isize; ++i)
	{
		os << "|\t";
		for(yggr::size_type j = 0, jsize = mat_type::col_size(); j != jsize; ++j)
		{
			os << mat[i][j] << "\t";
		}
		os << "|\n";
	}

	return os;
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits, typename Val,
//			template<typename _Val, std::size_t> class Array,
//			template<typename _Val, std::size_t, std::size_t,
//				template<typename __Val, std::size_t> class _Base> class Base,
//			template<typename _Char, typename _Traits> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//		operator<<(Basic_OStream<Char, Traits>& os,
//						const yggr::math::matrix3d<Val, Array, Base>& mat);

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
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
			template<typename __Val, std::size_t> class _Base> class Base> inline
std::size_t hash_value(const yggr::math::matrix3d<Val, Array, Base>& val)
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
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
			template<typename __Val, std::size_t> class _Base> class Base>
struct hash<yggr::math::matrix3d<Val, Array, Base> >
{
	typedef yggr::math::matrix3d<Val, Array, Base> type;

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

#endif // __YGGR_MATH_MATRIX3D_T_HPP__
