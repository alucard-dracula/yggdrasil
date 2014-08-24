//determinant.hpp

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

#ifndef __YGGR_MATH_DETERMINANT_HPP__
#define __YGGR_MATH_DETERMINANT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/upper_types.hpp>
#include <boost/array.hpp>
#include <yggr/math/array_indexes_permutation_parity.hpp>

#include <boost/serialization/access.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/math/value_miss_comparer.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4244)
#endif //_MSC_VER

namespace yggr
{
namespace math
{

template<typename Val, std::size_t ROW, std::size_t COL,
		template<typename _Val, std::size_t> class Base = boost::array>
class determinant;

} // namespace math
namespace math
{

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base>
class determinant
: public Base< Base<typename upper_signed<Val>::value_type, COL>, ROW>
{
public:
	enum
	{
		E_ROW_LENGTH = ROW,
		E_COL_LENGTH = COL,
		E_compile_u32 = 0xffffffff
	};

public:
	typedef typename upper_signed<Val>::value_type val_type;
	typedef Base<val_type, E_COL_LENGTH> row_type;

private:
	typedef Base<size_type, E_COL_LENGTH> index_array_type;

	typedef Base< row_type, E_ROW_LENGTH> base_type;
	typedef determinant this_type;

public:
	determinant(void)
	{
		base_type::zero();
	}

	template<typename OVal>
	determinant(const Base<OVal, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		(*this) = right;
	}

	determinant(const Base<val_type, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		(*this) = right;
	}

	determinant(const base_type& right)
		: base_type(right)
	{
	}

	template<typename OVal>
	determinant(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, Base>& right)
	{
		(*this) = right;
	}

	determinant(const this_type& right)
		: base_type(right)
	{
	}

	~determinant(void)
	{
	}

	inline const size_type row_size(void) const
	{
		return E_ROW_LENGTH;
	}

	inline static const size_type s_row_size(void)
	{
		return E_ROW_LENGTH;
	}

	inline const size_type col_size(void) const
	{
		return E_COL_LENGTH;
	}

	inline static const size_type s_col_size(void)
	{
		return E_COL_LENGTH;
	}

	this_type& zero(void)
	{
		typedef typename base_type::iterator iter_type;

		for(iter_type i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			std::fill((*i).begin(), (*i).end(), val_type());
		}

		return *this;
	}

	bool is_zero(void) const
	{
		typedef math::value_miss_comparer<val_type> cmper_type;

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				if(cmper_type((*this)[i][j]) != val_type())
				{
					return false;
				}
			}
		}

		return true;
	}

	inline val_type& operator()(const size_type& i, const size_type& j)
	{
		assert(i < 4 && j < 4);
		return (*this)[i][j];
	}

	inline const val_type& operator()(const size_type& i, const size_type& j) const
	{
		assert(i < 4 && j < 4);
		return (*this)[i][j];
	}

	template<typename OVal>
	this_type& operator=(const Base<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] = right[i * E_COL_LENGTH + j];
			}
		}

		return *this;
	}

	this_type& operator=(const Base<val_type, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] = right[i * E_COL_LENGTH + j];
			}
		}

		return *this;
	}

	template<typename OVal>
	this_type& operator=(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, Base>& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] = right[i][j];
			}
		}

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base = right;
		return *this;
	}

	val_type det(void) const
	{
		index_array_type idx_arr;
		val_type lr = val_type();
		val_type rl = val_type();
		val_type tmp = val_type(1);
		for(size_type i = 0, j = E_COL_LENGTH - 1, isize = E_COL_LENGTH; i != isize; ++i, --j)
		{
			{
				tmp = 1;
				for(size_type k = 0, l = i, ksize = E_ROW_LENGTH; k != ksize; ++k, l = (l + 1) % E_COL_LENGTH)
				{
					idx_arr[k] = l;
					tmp *= (*this)[k][l];
				}

				lr += item_sign(idx_arr) * tmp;
			}

			{
				tmp = 1;
				for(size_type k = 0, l = j, ksize = E_ROW_LENGTH; k != ksize; ++k, l = (l < 1)? E_COL_LENGTH - 1 : l - 1)
				{
					idx_arr[k] = l;
					tmp *= (*this)[k][l];
				}

				rl += item_sign(idx_arr) * tmp;
			}
		}

		return lr + rl;
	}

private:
	const val_type item_sign(const index_array_type& indexes) const
	{
		size_type inv_num = size_type();
		for(size_type i = 0, isize = E_COL_LENGTH - 1; i != isize; ++i)
		{
			for(size_type j = i + 1, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				inv_num += static_cast<size_type>(indexes[j] < indexes[i]);
			}
		}

		return inv_num % 2 == size_type()? val_type(1) : val_type(-1);
	}

protected:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("determinant_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

//------------------error determinant S-------------------------------
template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 0, 0, Base >
{
private:
	determinant(void){};
	~determinant(void){};

	Val _elems[0][0];
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 0, 1, Base >
{
private:
	determinant(void){};
	~determinant(void){};

	Val _elems[0][0];
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 1, 0, Base >
{
private:
	determinant(void){};
	~determinant(void){};

	Val _elems[0][0];
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 1, 1, Base >
{
private:
	determinant(void){};
	~determinant(void){};

	Val _elems[0][0];
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 0, 2, Base >
{
private:
	determinant(void){};
	~determinant(void){};

	Val _elems[0][0];
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 2, 0, Base >
{
private:
	determinant(void){};
	~determinant(void){};

	Val _elems[0][0];
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 1, 2, Base >
{
private:
	determinant(void){};
	~determinant(void){};

	Val _elems[0][0];
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 2, 1, Base >
{
private:
	determinant(void){};
	~determinant(void){};

	Val _elems[0][0];
};

//------------------error determinant E-------------------------------

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant< Val, 2, 2, Base >
	: public Base< Base<typename upper_signed<Val>::value_type, 2>, 2>
{
public:
	enum
	{
		E_ROW_LENGTH = 2,
		E_COL_LENGTH = 2,
		E_compile_u32 = 0xffffffff
	};

public:
	typedef typename upper_signed<Val>::value_type val_type;
	typedef Base<val_type, E_COL_LENGTH> row_type;

private:
	typedef Base<size_type, E_COL_LENGTH> index_array_type;

	typedef Base< row_type, E_ROW_LENGTH> base_type;
	typedef determinant this_type;

public:
	determinant(void)
	{
		zero();
	}

	template<typename OVal>
	determinant(const Base<OVal, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		(*this) = right;
	}

	determinant(const Base<val_type, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		(*this) = right;
	}

	determinant(const base_type& right)
		: base_type(right)
	{
	}

	template<typename OVal>
	determinant(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, Base>& right)
	{
		(*this) = right;
	}

	determinant(const this_type& right)
		: base_type(right)
	{
	}

	~determinant(void)
	{
	}

	inline const size_type row_size(void) const
	{
		return E_ROW_LENGTH;
	}

	inline static const size_type s_row_size(void)
	{
		return E_ROW_LENGTH;
	}

	inline const size_type col_size(void) const
	{
		return E_COL_LENGTH;
	}

	inline static const size_type s_col_size(void)
	{
		return E_COL_LENGTH;
	}

	this_type& zero(void)
	{
		(*this)[0][0] = (*this)[0][1] = (*this)[1][0] = (*this)[1][1] = 0;
		return *this;
	}

	bool is_zero(void) const
	{
		typedef value_miss_comparer<val_type> cmper_type;
		cmper_type cmper((*this)[0][0]);
		return !(cmper != 0 || cmper != (*this)[0][1] || cmper != (*this)[1][0] || cmper != (*this)[1][1]);
	}

	val_type det(void) const
	{
		return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
	}

	template<typename OVal>
	this_type& operator=(const Base<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		(*this)[0][0] = right[0];
		(*this)[0][1] = right[1];
		(*this)[1][0] = right[2];
		(*this)[1][1] = right[3];

		return *this;
	}

	this_type& operator=(const Base<val_type, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		(*this)[0][0] = right[0];
		(*this)[0][1] = right[1];
		(*this)[1][0] = right[2];
		(*this)[1][1] = right[3];

		return *this;
	}

	template<typename OVal>
	this_type& operator=(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, Base>& right)
	{
		(*this)[0][0] = right[0][0];
		(*this)[0][1] = right[0][1];
		(*this)[1][0] = right[1][0];
		(*this)[1][1] = right[1][1];

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base = right;
		return *this;
	}

protected:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("determinant_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
		//for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		//{
		//	ar & (*this)[i];
		//}
	}
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant<Val, 3, 3, Base>
	: public Base< Base<typename upper_signed<Val>::value_type, 3>, 3>
{
public:
	enum
	{
		E_ROW_LENGTH = 3,
		E_COL_LENGTH = 3,
		E_compile_u32 = 0xffffffff
	};

public:
	typedef typename upper_signed<Val>::value_type val_type;
	typedef Base<val_type, E_COL_LENGTH> row_type;

private:
	typedef Base<size_type, E_COL_LENGTH> index_array_type;

	typedef Base< row_type, E_ROW_LENGTH> base_type;
	typedef determinant this_type;

public:
	determinant(void)
	{
		zero();
	}

	template<typename OVal>
	determinant(const Base<OVal, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		(*this) = right;
	}

	determinant(const Base<val_type, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		(*this) = right;
	}

	determinant(const base_type& right)
		: base_type(right)
	{
	}

	template<typename OVal>
	determinant(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, Base>& right)
	{
		(*this) = right;
	}

	determinant(const this_type& right)
		: base_type(right)
	{
	}

	~determinant(void)
	{
	}

	inline const size_type row_size(void) const
	{
		return E_ROW_LENGTH;
	}

	inline static const size_type s_row_size(void)
	{
		return E_ROW_LENGTH;
	}

	inline const size_type col_size(void) const
	{
		return E_COL_LENGTH;
	}

	inline static const size_type s_col_size(void)
	{
		return E_COL_LENGTH;
	}

	this_type& zero(void)
	{
		(*this)[0][0] = (*this)[0][1] = (*this)[0][2]
		= (*this)[1][0] = (*this)[1][1] = (*this)[1][2]
		= (*this)[2][0] = (*this)[2][1] = (*this)[2][2] = val_type();
		return *this;
	}

	bool is_zero(void) const
	{
		typedef math::value_miss_comparer<val_type> cmper_type;

		cmper_type cmper((*this)[0][0]);
		return ! (cmper != val_type() || cmper != (*this)[0][1] || cmper != (*this)[0][2]
					|| cmper != (*this)[1][0] || cmper != (*this)[1][1] || cmper != (*this)[1][2]
					|| cmper != (*this)[2][0] || cmper != (*this)[2][1] || cmper != (*this)[2][2]);
	}

	val_type det(void) const
	{
		return (*this)[0][0] * (*this)[1][1] * (*this)[2][2]
				+ (*this)[0][1] * (*this)[1][2] * (*this)[2][0]
				+ (*this)[0][2] * (*this)[1][0] * (*this)[2][1]

				- (*this)[0][2] * (*this)[1][1] * (*this)[2][0]
				- (*this)[0][1] * (*this)[1][0] * (*this)[2][2]
				- (*this)[0][0] * (*this)[1][2] * (*this)[2][1];
	}

	template<typename OVal>
	this_type& operator=(const Base<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		(*this)[0][0] = right[0];
		(*this)[0][1] = right[1];
		(*this)[0][2] = right[2];

		(*this)[1][0] = right[3];
		(*this)[1][1] = right[4];
		(*this)[1][2] = right[5];

		(*this)[2][0] = right[6];
		(*this)[2][1] = right[7];
		(*this)[2][2] = right[8];

		return *this;
	}

	this_type& operator=(const Base<val_type, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		(*this)[0][0] = right[0];
		(*this)[0][1] = right[1];
		(*this)[0][2] = right[2];

		(*this)[1][0] = right[3];
		(*this)[1][1] = right[4];
		(*this)[1][2] = right[5];

		(*this)[2][0] = right[6];
		(*this)[2][1] = right[7];
		(*this)[2][2] = right[8];

		return *this;
	}

	template<typename OVal>
	this_type& operator=(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, Base>& right)
	{
		(*this)[0][0] = right[0][0];
		(*this)[0][1] = right[0][1];
		(*this)[0][2] = right[0][2];

		(*this)[1][0] = right[1][0];
		(*this)[1][1] = right[1][1];
		(*this)[1][2] = right[1][2];

		(*this)[2][0] = right[2][0];
		(*this)[2][1] = right[2][1];
		(*this)[2][2] = right[2][2];

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base = right;
		return *this;
	}

protected:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("determinant_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
		//for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		//{
		//	ar & (*this)[i];
		//}
	}
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant<Val, 4, 4, Base>
	: public Base< Base<typename upper_signed<Val>::value_type, 4>, 4>
{
public:
	enum
	{
		E_ROW_LENGTH = 4,
		E_COL_LENGTH = 4,
		E_compile_u32 = 0xffffffff
	};

public:
	typedef typename upper_signed<Val>::value_type val_type;
	typedef Base<val_type, E_COL_LENGTH> row_type;

private:
	typedef Base<size_type, E_COL_LENGTH> index_array_type;

	typedef Base< row_type, E_ROW_LENGTH> base_type;
	typedef determinant this_type;

public:
	determinant(void)
	{
		zero();
	}

	template<typename OVal>
	determinant(const Base<OVal, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		(*this) = right;
	}

	determinant(const Base<val_type, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		(*this) = right;
	}

	determinant(base_type& right)
		: base_type(right)
	{
	}

	template<typename OVal>
	determinant(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, Base>& right)
	{
		(*this) = right;
	}

	determinant(const this_type& right)
		: base_type(right)
	{
	}

	~determinant(void)
	{
	}

	inline const size_type row_size(void) const
	{
		return E_ROW_LENGTH;
	}

	inline static const size_type s_row_size(void)
	{
		return E_ROW_LENGTH;
	}

	inline const size_type col_size(void) const
	{
		return E_COL_LENGTH;
	}

	inline static const size_type s_col_size(void)
	{
		return E_COL_LENGTH;
	}

	this_type& zero(void)
	{
		(*this)[0][0] = (*this)[0][1] = (*this)[0][2] = (*this)[0][3]
		= (*this)[1][0] = (*this)[1][1] = (*this)[1][2] = (*this)[1][3]
		= (*this)[2][0] = (*this)[2][1] = (*this)[2][2] = (*this)[2][3]
		= (*this)[3][0] = (*this)[3][1] = (*this)[3][2] = (*this)[3][3]
		= val_type();
		return *this;
	}

	bool is_zero(void) const
	{

		typedef math::value_miss_comparer<val_type> cmper_type;

		cmper_type cmper((*this)[0][0]);
		return ! (cmper != val_type() || cmper != (*this)[0][1] || cmper != (*this)[0][2] || cmper != (*this)[0][3]
					|| cmper != (*this)[1][0] || cmper != (*this)[1][1] || cmper != (*this)[1][2] || cmper != (*this)[1][3]
					|| cmper != (*this)[2][0] || cmper != (*this)[2][1] || cmper != (*this)[2][2] || cmper != (*this)[2][3]
					|| cmper != (*this)[3][0] || cmper != (*this)[3][1] || cmper != (*this)[3][2] || cmper != (*this)[3][3]);
	}

	val_type det(void) const
	{
		return	(*this)[0][0] * (*this)[1][1] * (*this)[2][2] * (*this)[3][3] * sign_array_indexes<0, 4>::value
				+ (*this)[0][1] * (*this)[1][2] * (*this)[2][3] * (*this)[3][0] * sign_array_indexes<1, 4>::value
				+ (*this)[0][2] * (*this)[1][3] * (*this)[2][0] * (*this)[3][1] * sign_array_indexes<2, 4>::value
				+ (*this)[0][3] * (*this)[1][0] * (*this)[2][1] * (*this)[3][2] * sign_array_indexes<3, 4>::value

				+ (*this)[0][3] * (*this)[1][2] * (*this)[2][1] * (*this)[3][0] * sign_array_indexes<3, 4, false>::value
				+ (*this)[0][2] * (*this)[1][1] * (*this)[2][0] * (*this)[3][3] * sign_array_indexes<2, 4, false>::value
				+ (*this)[0][1] * (*this)[1][0] * (*this)[2][3] * (*this)[3][2] * sign_array_indexes<1, 4, false>::value
				+ (*this)[0][0] * (*this)[1][3] * (*this)[2][2] * (*this)[3][1] * sign_array_indexes<0, 4, false>::value;
	}

	template<typename OVal>
	this_type& operator=(const Base<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		(*this)[0][0] = right[0];
		(*this)[0][1] = right[1];
		(*this)[0][2] = right[2];
		(*this)[0][3] = right[3];

		(*this)[1][0] = right[4];
		(*this)[1][1] = right[5];
		(*this)[1][2] = right[6];
		(*this)[1][3] = right[7];

		(*this)[2][0] = right[8];
		(*this)[2][1] = right[9];
		(*this)[2][2] = right[10];
		(*this)[2][3] = right[11];

		(*this)[3][0] = right[12];
		(*this)[3][1] = right[13];
		(*this)[3][2] = right[14];
		(*this)[3][3] = right[15];

		return *this;
	}

	this_type& operator=(const Base<val_type, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		(*this)[0][0] = right[0];
		(*this)[0][1] = right[1];
		(*this)[0][2] = right[2];
		(*this)[0][3] = right[3];

		(*this)[1][0] = right[4];
		(*this)[1][1] = right[5];
		(*this)[1][2] = right[6];
		(*this)[1][3] = right[7];

		(*this)[2][0] = right[8];
		(*this)[2][1] = right[9];
		(*this)[2][2] = right[10];
		(*this)[2][3] = right[11];

		(*this)[3][0] = right[12];
		(*this)[3][1] = right[13];
		(*this)[3][2] = right[14];
		(*this)[3][3] = right[15];

		return *this;
	}

	template<typename OVal>
	this_type& operator=(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, Base>& right)
	{
		(*this)[0][0] = right[0][0];
		(*this)[0][1] = right[0][1];
		(*this)[0][2] = right[0][2];
		(*this)[0][3] = right[0][3];

		(*this)[1][0] = right[1][0];
		(*this)[1][1] = right[1][1];
		(*this)[1][2] = right[1][2];
		(*this)[1][3] = right[1][3];

		(*this)[2][0] = right[2][0];
		(*this)[2][1] = right[2][1];
		(*this)[2][2] = right[2][2];
		(*this)[2][3] = right[2][3];

		(*this)[3][0] = right[3][0];
		(*this)[3][1] = right[3][1];
		(*this)[3][2] = right[3][2];
		(*this)[3][3] = right[3][3];

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base = right;
		return *this;
	}

protected:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("determinant_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
		//for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		//{
		//	ar & (*this)[i];
		//}
	}
};

} // namespace math

template<typename Val, std::size_t ROW, std::size_t COL,
			template <typename _Val, std::size_t>
			class Base >
struct upper_float< math::determinant<Val, ROW, COL, Base> >
{
	typedef typename math::determinant<
				typename upper_float< Val >::value_type, ROW, COL, Base > value_type;
};

template<typename Val, std::size_t ROW, std::size_t COL,
			template <typename _Val, std::size_t>
			class Base >
struct upper_signed< math::determinant<Val, ROW, COL, Base> >
{
	typedef typename math::determinant<
				typename upper_signed< Val >::value_type, ROW, COL, Base > value_type;
};

} // namespace yggr

namespace std
{

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base>
const yggr::math::determinant<Val, ROW, COL, Base>
	abs(const yggr::math::determinant<Val, ROW, COL, Base>& x)
{
	return x.det();
}

} // namespace std

template<typename Char, typename Traits,
			typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const yggr::math::determinant<Val, ROW, COL, Base>& det)
{
	typedef Val val_type;
	typedef yggr::math::determinant<Val, ROW, COL, Base> det_type;


	for(yggr::size_type i = 0, isize = det_type::s_row_size(); i != isize; ++i)
	{
		os << "|\t";
		for(yggr::size_type j = 0, jsize = det_type::s_col_size(); j != jsize; ++j)
		{
			os << det[i][j] << "\t";
		}

		os << "|\n";
	}

	return os;
}

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif // __YGGR_MATH_DETERMINANT_HPP__
