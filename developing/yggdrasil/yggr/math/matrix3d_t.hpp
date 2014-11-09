//matrix3d_t.hpp

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

#ifndef __YGGR_MATH_MATRIX3D_T_HPP__
#define __YGGR_MATH_MATRIX3D_T_HPP__

#include <algorithm>
#include <boost/array.hpp>
#include <boost/serialization/access.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/math/determinant.hpp>
#include <yggr/math/math.hpp>
#include <yggr/geometry/geo_plane.hpp>

#include <yggr/serialization/array.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4244)
#endif //_MSC_VER

namespace yggr
{
//namespace math
//{
//
//template<typename Val,
//			template <typename _Val, std::size_t> class Base>
//class vector3d;
//
//} // namespace math

//namespace geometry
//{
//template<typename Val,
//			template <typename _Val, std::size_t> class Base>
//class geo_plane;
//
//} // namesapce geometry

namespace math
{
template<typename Val,
	template<typename _Val, std::size_t> class Array,
	template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t> class _Base> class Base>
class matrix3d;

} // namespace math

namespace math
{

template<typename Val,
	template<typename _Val, std::size_t> class Array = boost::array,
	template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t> class _Base> class Base = determinant>
class matrix3d : public Base< Val, 4, 4, Array>
{
public:
	enum
	{
		E_ROW_LENGTH = 4,
		E_COL_LENGTH = 4,
		E_MAX_RANK = 4,
		E_compile_u32 = 0xffffffff
	};
public:
	typedef Val val_type;
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

	template<typename OVal>
	matrix3d(const Array<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
		: base_type(right)
	{
	}

	matrix3d(const Array<val_type, E_ROW_LENGTH * E_COL_LENGTH>& right)
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

	template<typename OVal>
	matrix3d(const matrix3d<OVal, Array, Base>& right)
	{
		(*this) = right;
	}

	matrix3d(const base_type& right)
		: base_type(right)
	{
	}

	matrix3d(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}

	matrix3d(const this_type& right)
		: base_type(right)
	{
	}

	~matrix3d(void)
	{
	}

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
		typedef math::value_miss_comparer<val_type> cmper_type;

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

	const size_type rank(void) const
	{
		typedef value_miss_comparer<val_type> cmper_type;
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

		return std::min(r, l);
	}

	const size_type max_rank(void) const
	{
		return E_MAX_RANK;
	}

	static const size_type s_max_rank(void)
	{
		return E_MAX_RANK;
	}

	this_type operator-(void) const
	{
		this_type tmp(val_type(0));

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				tmp[i][j] = -((*this)[i][j]);
			}
		}

		return tmp;
	}

	template<typename OVal>
	this_type operator+(const matrix3d<OVal, Array, Base>& right) const
	{
		this_type tmp(val_type(0));

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				tmp[i][j] = (*this)[i][j] + right[i][j];
			}
		}

		return tmp;
	}

	template<typename OVal>
	this_type& operator+=(const matrix3d<OVal, Array, Base>& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] += right[i][j];
			}
		}

		return *this;
	}

	template<typename OVal>
	this_type operator-(const matrix3d<OVal, Array, Base>& right) const
	{
		this_type tmp(val_type(0));

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				tmp[i][j] = (*this)[i][j] - right[i][j];
			}
		}

		return tmp;
	}

	template<typename OVal>
	this_type& operator-=(const matrix3d<OVal, Array, Base>& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] -= right[i][j];
			}
		}

		return *this;
	}

	template<typename OVal>
	this_type operator*(const OVal& right) const
	{
		this_type tmp(val_type(0));

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				tmp[i][j] = (*this)[i][j] * right;
			}
		}

		return tmp;
	}

	template<typename OVal>
	this_type& operator*=(const OVal& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] *= right;
			}
		}

		return *this;
	}

	template<typename OVal>
	this_type operator/(const OVal& right) const
	{
		this_type tmp(val_type(0));

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				tmp[i][j] = (*this)[i][j] / right;
			}
		}

		return tmp;
	}

	template<typename OVal>
	this_type& operator/=(const OVal& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] /= right;
			}
		}

		return *this;
	}

	template<typename OVal>
	this_type& operator=(const Array<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		base_type& base = *this;
		base = right;
		return *this;
	}

	template<typename OVal>
	this_type& operator=(const matrix3d<OVal, Array, Base>& right)
	{
		base_type& base = *this;
		base = right;

		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		base_type::operator=(boost::forward<base_type>(right));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	void swap(this_type& right)
	{
		base_type::swap(right);
	}

	template<typename OVal>
	this_type operator*(const matrix3d<OVal, Array, Base>& right) const
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

		return tmp;
	}

	template<typename OVal>
	this_type& operator*=(const matrix3d<OVal, Array, Base>& right)
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

	const matrix3d<typename upper_float<val_type>::value_type, Array, Base> inverse(void) const
	{
		typedef typename upper_float<val_type>::value_type float_type;
		typedef matrix3d<float_type> ret_type;
		typedef value_miss_comparer<float_type> float_cmper_type;

		float_type d = (*this).det();

		if(float_cmper_type(d) == float_type())
		{
			return ret_type(float_type(0));
		}

		ret_type ret(adjugate());

		if(ret.rank() != E_COL_LENGTH)
		{
			return ret_type(float_type(0));
		}

		ret /= d;

		return ret;
	}

	const this_type adjugate(void) const
	{
		this_type rst(0);

		rst[0][0] =   (*this)[1][1] * (*this)[2][2] * (*this)[3][3]
					+ (*this)[1][2] * (*this)[2][3] * (*this)[3][1]
					+ (*this)[1][3] * (*this)[2][1] * (*this)[3][2]
					- (*this)[1][3] * (*this)[2][2] * (*this)[3][1]
					- (*this)[1][2] * (*this)[2][1] * (*this)[3][3]
					- (*this)[1][1] * (*this)[2][3] * (*this)[3][2];

		rst[0][1] = -((*this)[0][1] * (*this)[2][2] * (*this)[3][3]
					+ (*this)[0][2] * (*this)[2][3] * (*this)[3][1]
					+ (*this)[0][3] * (*this)[2][1] * (*this)[3][2]
					- (*this)[0][3] * (*this)[2][2] * (*this)[3][1]
					- (*this)[0][2] * (*this)[2][1] * (*this)[3][3]
					- (*this)[0][1] * (*this)[2][3] * (*this)[3][2]);

		rst[0][2] =   (*this)[0][1] * (*this)[1][2] * (*this)[3][3]
					+ (*this)[0][2] * (*this)[1][3] * (*this)[3][1]
					+ (*this)[0][3] * (*this)[1][1] * (*this)[3][2]
					- (*this)[0][3] * (*this)[1][2] * (*this)[3][1]
					- (*this)[0][2] * (*this)[1][1] * (*this)[3][3]
					- (*this)[0][1] * (*this)[1][3] * (*this)[3][2];

		rst[0][3] = -((*this)[0][1] * (*this)[1][2] * (*this)[2][3]
					+ (*this)[0][2] * (*this)[1][3] * (*this)[2][1]
					+ (*this)[0][3] * (*this)[1][1] * (*this)[2][2]
					- (*this)[0][3] * (*this)[1][2] * (*this)[2][1]
					- (*this)[0][2] * (*this)[1][1] * (*this)[2][3]
					- (*this)[0][1] * (*this)[1][3] * (*this)[2][2]);
		//---------------------------------------------------------------------

		rst[1][0] = -((*this)[1][0] * (*this)[2][2] * (*this)[3][3]
					+ (*this)[1][2] * (*this)[2][3] * (*this)[3][0]
					+ (*this)[1][3] * (*this)[2][0] * (*this)[3][2]
					- (*this)[1][3] * (*this)[2][2] * (*this)[3][0]
					- (*this)[1][2] * (*this)[2][0] * (*this)[3][3]
					- (*this)[1][0] * (*this)[2][3] * (*this)[3][2]);

		rst[1][1] =   (*this)[0][0] * (*this)[2][2] * (*this)[3][3]
					+ (*this)[0][2] * (*this)[2][3] * (*this)[3][0]
					+ (*this)[0][3] * (*this)[2][0] * (*this)[3][2]
					- (*this)[0][3] * (*this)[2][2] * (*this)[3][0]
					- (*this)[0][2] * (*this)[2][0] * (*this)[3][3]
					- (*this)[0][0] * (*this)[2][3] * (*this)[3][2];

		rst[1][2] = -((*this)[0][0] * (*this)[1][2] * (*this)[3][3]
					+ (*this)[0][2] * (*this)[1][3] * (*this)[3][0]
					+ (*this)[0][3] * (*this)[1][0] * (*this)[3][2]
					- (*this)[0][3] * (*this)[1][2] * (*this)[3][0]
					- (*this)[0][2] * (*this)[1][0] * (*this)[3][3]
					- (*this)[0][0] * (*this)[1][3] * (*this)[3][2]);

		rst[1][3] =   (*this)[0][0] * (*this)[1][2] * (*this)[2][3]
					+ (*this)[0][2] * (*this)[1][3] * (*this)[2][0]
					+ (*this)[0][3] * (*this)[1][0] * (*this)[2][2]
					- (*this)[0][3] * (*this)[1][2] * (*this)[2][0]
					- (*this)[0][2] * (*this)[1][0] * (*this)[2][3]
					- (*this)[0][0] * (*this)[1][3] * (*this)[2][2];
		//--------------------------------------------------------------------

		rst[2][0] =   (*this)[1][0] * (*this)[2][1] * (*this)[3][3]
					+ (*this)[1][1] * (*this)[2][3] * (*this)[3][0]
					+ (*this)[1][3] * (*this)[2][0] * (*this)[3][1]
					- (*this)[1][3] * (*this)[2][1] * (*this)[3][0]
					- (*this)[1][1] * (*this)[2][0] * (*this)[3][3]
					- (*this)[1][0] * (*this)[2][3] * (*this)[3][1];

		rst[2][1] = -((*this)[0][0] * (*this)[2][1] * (*this)[3][3]
					+ (*this)[0][1] * (*this)[2][3] * (*this)[3][0]
					+ (*this)[0][3] * (*this)[2][0] * (*this)[3][1]
					- (*this)[0][3] * (*this)[2][1] * (*this)[3][0]
					- (*this)[0][1] * (*this)[2][0] * (*this)[3][3]
					- (*this)[0][0] * (*this)[2][3] * (*this)[3][1]);

		rst[2][2] =   (*this)[0][0] * (*this)[1][1] * (*this)[3][3]
					+ (*this)[0][1] * (*this)[1][3] * (*this)[3][0]
					+ (*this)[0][3] * (*this)[1][0] * (*this)[3][1]
					- (*this)[0][3] * (*this)[1][1] * (*this)[3][0]
					- (*this)[0][1] * (*this)[1][0] * (*this)[3][3]
					- (*this)[0][0] * (*this)[1][3] * (*this)[3][1];

		rst[2][3] = -((*this)[0][0] * (*this)[1][1] * (*this)[2][3]
					+ (*this)[0][1] * (*this)[1][3] * (*this)[2][0]
					+ (*this)[0][3] * (*this)[1][0] * (*this)[2][1]
					- (*this)[0][3] * (*this)[1][1] * (*this)[2][0]
					- (*this)[0][1] * (*this)[1][0] * (*this)[2][3]
					- (*this)[0][0] * (*this)[1][3] * (*this)[2][1]);
		//-----------------------------------------------------------------

		rst[3][0] = -((*this)[1][0] * (*this)[2][1] * (*this)[3][2]
					+ (*this)[1][1] * (*this)[2][2] * (*this)[3][0]
					+ (*this)[1][2] * (*this)[2][0] * (*this)[3][1]
					- (*this)[1][2] * (*this)[2][1] * (*this)[3][0]
					- (*this)[1][1] * (*this)[2][0] * (*this)[3][2]
					- (*this)[1][0] * (*this)[2][2] * (*this)[3][1]);

		rst[3][1] =   (*this)[0][0] * (*this)[2][1] * (*this)[3][2]
					+ (*this)[0][1] * (*this)[2][2] * (*this)[3][0]
					+ (*this)[0][2] * (*this)[2][0] * (*this)[3][1]
					- (*this)[0][2] * (*this)[2][1] * (*this)[3][0]
					- (*this)[0][1] * (*this)[2][0] * (*this)[3][2]
					- (*this)[0][0] * (*this)[2][2] * (*this)[3][1];

		rst[3][2] = -((*this)[0][0] * (*this)[1][1] * (*this)[3][2]
					+ (*this)[0][1] * (*this)[1][2] * (*this)[3][0]
					+ (*this)[0][2] * (*this)[1][0] * (*this)[3][1]
					- (*this)[0][2] * (*this)[1][1] * (*this)[3][0]
					- (*this)[0][1] * (*this)[1][0] * (*this)[3][2]
					- (*this)[0][0] * (*this)[1][2] * (*this)[3][1]);

		rst[3][3] = (*this)[0][0] * (*this)[1][1] * (*this)[2][2]
					+ (*this)[0][1] * (*this)[1][2] * (*this)[2][0]
					+ (*this)[0][2] * (*this)[1][0] * (*this)[2][1]
					- (*this)[0][2] * (*this)[1][1] * (*this)[2][0]
					- (*this)[0][1] * (*this)[1][0] * (*this)[2][2]
					- (*this)[0][0] * (*this)[1][2] * (*this)[2][1];

		return rst;

	}

	const this_type transpose(void) const
	{
		this_type tmp(*this);

		//std::swap(tmp[0][0], tmp[0][0]);
		//std::swap(tmp[1][1], tmp[1][1]);
		//std::swap(tmp[2][2], tmp[2][2]);
		//std::swap(tmp[3][3], tmp[3][3]);

		std::swap(tmp[0][1], tmp[1][0]);
		std::swap(tmp[0][2], tmp[2][0]);
		std::swap(tmp[0][3], tmp[3][0]);
		std::swap(tmp[1][2], tmp[2][1]);
		std::swap(tmp[1][3], tmp[3][1]);
		std::swap(tmp[2][3], tmp[3][2]);

		return tmp;
	}

	template<typename OVal>
	static const this_type make_translate(const OVal& x, const OVal& y, const OVal& z)
	{
		this_type mat;
		mat[3][0] = x;
		mat[3][1] = y;
		mat[3][2] = z;
		return mat;
	}

	static const matrix3d<typename upper_float<val_type>::value_type, Array, Base>
		make_rotate_x(typename upper_float<val_type>::value_type& rad)
	{
		typedef matrix3d<typename upper_float<val_type>::value_type, Array, Base> rst_type;
		rst_type mat;
		mat[1][1] = std::cos(rad);
		mat[1][2] = std::sin(rad);
		mat[2][1] = -mat[1][2];
		mat[2][2] = mat[1][1];

		return mat;
	}

	static const matrix3d<typename upper_float<val_type>::value_type, Array, Base>
		make_rotate_y(typename upper_float<val_type>::value_type& rad)
	{
		typedef matrix3d<typename upper_float<val_type>::value_type, Array, Base> rst_type;
		rst_type mat;
		mat[0][0] = std::cos(rad);
		mat[0][2] = -std::sin(rad);
		mat[2][0] = -mat[0][2];
		mat[2][2] = mat[0][0];

		return mat;
	}

	static const matrix3d<typename upper_float<val_type>::value_type, Array, Base>
		make_rotate_z(typename upper_float<val_type>::value_type& rad)
	{
		typedef matrix3d<typename upper_float<val_type>::value_type, Array, Base> rst_type;
		rst_type mat;
		mat[0][0] = std::cos(rad);
		mat[0][1] = std::sin(rad);
		mat[1][0] = -mat[0][0];
		mat[1][1] = mat[0][0];

		return mat;
	}

	static const matrix3d<typename upper_float<val_type>::value_type, Array, Base>
		make_rotate_yxz(typename upper_float<val_type>::value_type& xrad,
							typename upper_float<val_type>::value_type& yrad,
							typename upper_float<val_type>::value_type& zrad)
	{
		return make_rotate_y(yrad) * make_rotate_x(xrad) * make_rotate_z(zrad);
	}

	template<typename OVal>
	static const this_type make_scale(const OVal& sx, const OVal& sy, const OVal& sz)
	{
		this_type mat;
		mat[0][0] = sx;
		mat[1][1] = sy;
		mat[2][2] = sz;
		return mat;
	}

	template<typename OVal>
	static const matrix3d<typename upper_float<val_type>::value_type, Array, Base>
		make_reflect(const vector3d<OVal, Array>& pos1,
						const vector3d<OVal, Array>& pos2, const vector3d<OVal, Array>& pos3)
	{
		typedef typename upper_float<val_type>::value_type float_type;
		typedef matrix3d<float_type, Array, Base> rst_type;
		typedef vector3d<OVal, Array> vec_type;
		typedef geometry::geo_plane<OVal, Array> plane_type;

		plane_type plane(pos1, pos2, pos3);

		return this_type::make_reflect(plane);
	}

	template<typename OVal>
	static const matrix3d<typename upper_float<val_type>::value_type, Array, Base>
		make_reflect(const geometry::geo_plane<OVal, Array>& plane)
	{
		typedef typename upper_float<val_type>::value_type float_type;
		typedef matrix3d<float_type, Array, Base> rst_type;
		typedef vector3d<OVal, Array> vec_type;
		typedef geometry::geo_plane<OVal, Array> plane_type;
		typedef math::value_miss_comparer<float_type> cmper_type;

		if(!plane.is_plane())
		{
			//return rst_type(float_type(0));
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
	const this_type debug_adjugate(void) const
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

				rst[i][j] = det_tmp.det() * debug_adjugate_i_j_sign(i, j);
				++ lose_k;
			}

			lose_k = 0;
			++lose_l;
		}

		return rst;
	}

private:
	inline typename upper_signed<val_type>::value_type
			debug_adjugate_i_j_sign(const size_type& i, const size_type& j) const
	{
		// o + o == e == (o - 1) + (o - 1)
		// e + e == e == (e - 1) + (e - 1)
		// o + e == o == (o - 1) + (e - 1)
		// e + o == o == (e - 1) + (o - 1)
		// => sign(idxi + 1, idxj + 1) == sign(idxi, idxj) == sign(idxi - 1, idxj - 1)

		return (i + j) % 2 == val_type(0)? val_type(1) : val_type(-1);
	}

#endif // _DEBUG

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		base_type::serialize(ar, version);
	}
};

} // namespace math

template<typename Val,
			template<typename _Val, std::size_t>
			class Array,
			template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t>
				class _Base>
			class Base>
struct upper_float< math::matrix3d<Val, Array, Base> >
{
	typedef typename math::matrix3d<
				typename upper_float<Val>::value_type, Array, Base > value_type;
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
	typedef typename math::matrix3d<
				typename upper_signed< Val >::value_type, Array, Base > value_type;
};


} // namespace yggr

//template<typename ValL, typename ValR,
//		template<typename _Val, std::size_t> class Array,
//		template<typename _Val, std::size_t, std::size_t,
//					template<typename __Val, std::size_t> class _Base> class Base>
//yggr::math::matrix3d<ValR, Array, Base> operator*(const ValL& l, const yggr::math::matrix3d<ValR, Array, Base>& r)
//{
//	return r * l;
//}

namespace std
{

template<typename Val,
	template<typename _Val, std::size_t> class Array,
	template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t> class _Base> class Base>
void swap(yggr::math::matrix3d<Val, Array, Base>& l, 
			yggr::math::matrix3d<Val, Array, Base>& r)
{
	l.swap(r);
}

} // namespace std

namespace boost
{

template<typename Val,
	template<typename _Val, std::size_t> class Array,
	template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t> class _Base> class Base>
void swap(yggr::math::matrix3d<Val, Array, Base>& l, 
			yggr::math::matrix3d<Val, Array, Base>& r)
{
	l.swap(r);
}

} // namespace boost

template<typename Char, typename Traits, typename Val,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t> class _Base> class Base>
std::basic_ostream<Char, Traits>&
		operator<<(std::basic_ostream<Char, Traits>& os,
						const yggr::math::matrix3d<Val, Array, Base>& mat)
{
	typedef Val val_type;
	typedef yggr::math::matrix3d<Val> mat_type;


	for(yggr::size_type i = 0, isize = mat_type::s_row_size(); i != isize; ++i)
	{
		os << "|\t";
		for(yggr::size_type j = 0, jsize = mat_type::s_col_size(); j != jsize; ++j)
		{
			os << mat[i][j] << "\t";
		}
		os << "|\n";
	}

	return os;
}

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif // __YGGR_MATH_MATRIX3D_T_HPP__
