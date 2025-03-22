//determinant.hpp

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

#ifndef __YGGR_MATH_DETERMINANT_HPP__
#define __YGGR_MATH_DETERMINANT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/type_traits/upper_types.hpp>
#include <yggr/type_traits/operator_result.hpp>
#include <yggr/math/array_indexes_permutation_parity.hpp>
#include <yggr/math/flaw_comparer.hpp>
#include <yggr/math/odd_even_number.hpp>

#include <yggr/container/array.hpp>
#include <yggr/serialization/access.hpp>
#include <yggr/serialization/array.hpp>

#include <boost/ref.hpp>
#include <boost/functional/hash/hash.hpp>

#include <memory>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4244)
#endif //_MSC_VER

namespace yggr
{
namespace math
{

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base = yggr::array>
class determinant;

} // namespace math
} // namespace yggr

namespace yggr
{
namespace detail
{

// fix error c2784 in operator_result
template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base>
struct operator_result_t_getter< math::determinant<Val, ROW, COL, Base> >
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

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base>
class determinant
: public Base< Base<typename upper_signed<Val>::type, COL>, ROW>
{
public:
	YGGR_STATIC_CONSTANT(std::size_t, E_ROW_LENGTH = ROW);
	YGGR_STATIC_CONSTANT(std::size_t, E_COL_LENGTH = COL);

public:
	typedef typename upper_signed<Val>::type value_type;

private:
	typedef value_type val_type;

public:
	typedef Base<val_type, E_COL_LENGTH> row_type;

private:
	typedef Base<size_type, E_COL_LENGTH> index_array_type;
	typedef Base< row_type, E_ROW_LENGTH> base_type;

private:
	typedef determinant this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	determinant(void)
	{
		this_type::zero();
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	determinant(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		this_type::assign(right);
	}

	determinant(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	determinant(const base_type& right)
		: base_type(right)
	{
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	determinant(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		this_type::assign(right);
	}

	determinant(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	determinant(const this_type& right)
		: base_type(right)
	{
	}

	~determinant(void)
	{
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	this_type& operator=(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		this_type::assign(right);

		return *this;
	}

	this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type& base = *this;
		base = boost::move(right_ref);
		return *this;
	}

	inline this_type& operator=(const base_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& base = *this;
		base = right;
		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	this_type& operator=(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		this_type::assign(right);

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
		if(this == &right) 
		{
			return;
		}

		base_type::swap(right);
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	bool equal_to(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				if(!(cmper_type((*this)[i][j]) == right[i][j]))
				{
					return false;
				}
			}
		}

		return true;
	}

	bool equal_to(const this_type& right) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		if(this == boost::addressof(right))
		{
			return true;
		}

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				if(!(cmper_type((*this)[i][j]) == right[i][j]))
				{
					return false;
				}
			}
		}

		return true;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool not_equal_to(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right) const
	{
		return !this_type::equal_to(right);
	}

public:
	inline static size_type row_size(void)
	{
		return E_ROW_LENGTH;
	}

	inline static size_type col_size(void)
	{
		return E_COL_LENGTH;
	}

public:
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
		typedef flaw_comparer<val_type> cmper_type;

		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				if(!(cmper_type((*this)[i][j]) == val_type()))
				{
					return false;
				}
			}
		}

		return true;
	}

public:
	using base_type::operator[];

	inline val_type& operator()(const size_type& i, const size_type& j)
	{
		assert(i < E_ROW_LENGTH && j < E_COL_LENGTH);
		return (*this)[i][j];
	}

	inline const val_type& operator()(const size_type& i, const size_type& j) const
	{
		assert(i < E_ROW_LENGTH && j < E_COL_LENGTH);
		return (*this)[i][j];
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	void assign(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] = right[i * E_COL_LENGTH + j];
			}
		}
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	void assign(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		for(size_type i = 0, isize = E_ROW_LENGTH; i != isize; ++i)
		{
			for(size_type j = 0, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				(*this)[i][j] = right[i][j];
			}
		}
	}

public:
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

				lr += this_type::prv_s_item_sign(idx_arr) * tmp;
			}

			{
				tmp = 1;
				for(size_type k = 0, l = j, ksize = E_ROW_LENGTH; k != ksize; ++k, l = (l < 1)? E_COL_LENGTH - 1 : l - 1)
				{
					idx_arr[k] = l;
					tmp *= (*this)[k][l];
				}

				rl += this_type::prv_s_item_sign(idx_arr) * tmp;
			}
		}

		return lr + rl;
	}

	inline std::size_t hash(void) const
	{
		const base_type& base = *this;
		std::size_t seed = 0;
		boost::hash_combine(seed, "determinant");
		boost::hash_combine(seed, base);
		return seed;
	}

private:
	static val_type prv_s_item_sign(const index_array_type& indexes)
	{
		size_type inv_num = size_type();
		for(size_type i = 0, isize = E_COL_LENGTH - 1; i != isize; ++i)
		{
			for(size_type j = i + 1, jsize = E_COL_LENGTH; j != jsize; ++j)
			{
				inv_num += static_cast<size_type>(indexes[j] < indexes[i]);
			}
		}

		return math::is_even_number(inv_num)? val_type(1) : val_type(-1);
	}

protected:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"determinant_data",
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
class determinant< Val, 0, 2, Base >
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
class determinant< Val, 1, 2, Base >
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
	: public Base< Base<typename upper_signed<Val>::type, 2>, 2>
{
public:
	YGGR_STATIC_CONSTANT(std::size_t, E_ROW_LENGTH = 2);
	YGGR_STATIC_CONSTANT(std::size_t, E_COL_LENGTH = 2);
	
public:
	typedef typename upper_signed<Val>::type val_type;
	typedef Base<val_type, E_COL_LENGTH> row_type;

private:
	typedef Base<size_type, E_COL_LENGTH> index_array_type;

	typedef Base< row_type, E_ROW_LENGTH> base_type;
	typedef determinant this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	determinant(void)
	{
		this_type::zero();
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	determinant(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		this_type::assign(right);
	}

	determinant(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	determinant(const base_type& right)
		: base_type(right)
	{
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	determinant(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		this_type::assign(right);
	}

	determinant(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	determinant(const this_type& right)
		: base_type(right)
	{
	}

	~determinant(void)
	{
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& operator=(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		this_type::assign(right);

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type& base = *this;
		base = boost::move(right_ref);
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& base = *this;
		base = right;
		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& operator=(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		this_type::assign(right);

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
		if(this == &right) 
		{
			return;
		}

		base_type::swap(right);
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool equal_to(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		return 
			cmper_type((*this)[0][0]) == right[0][0] 
			&& cmper_type((*this)[0][1]) == (*this)[0][1] 

			&& cmper_type((*this)[1][0]) == (*this)[1][0] 
			&& cmper_type((*this)[1][1]) == (*this)[1][1];
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool not_equal_to(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right) const
	{
		return !this_type::equal_to(right);
	}

public:
	inline static size_type row_size(void)
	{
		return E_ROW_LENGTH;
	}

	inline static size_type col_size(void)
	{
		return E_COL_LENGTH;
	}

	inline this_type& zero(void)
	{
		(*this)[0][0] = (*this)[0][1] = (*this)[1][0] = (*this)[1][1] = 0;
		return *this;
	}

	inline bool is_zero(void) const
	{
		typedef flaw_comparer<val_type> cmper_type;
		cmper_type cmper((*this)[0][0]);
		return (cmper == 0 && cmper == (*this)[0][1] && cmper == (*this)[1][0] && cmper == (*this)[1][1]);
	}

public:
	using base_type::operator[];

	inline val_type& operator()(const size_type& i, const size_type& j)
	{
		assert(i < E_ROW_LENGTH && j < E_COL_LENGTH);
		return (*this)[i][j];
	}

	inline const val_type& operator()(const size_type& i, const size_type& j) const
	{
		assert(i < E_ROW_LENGTH && j < E_COL_LENGTH);
		return (*this)[i][j];
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	void assign(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		(*this)[0][0] = right[0];
		(*this)[0][1] = right[1];
		(*this)[1][0] = right[2];
		(*this)[1][1] = right[3];
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	void assign(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		(*this)[0][0] = right[0][0];
		(*this)[0][1] = right[0][1];
		(*this)[1][0] = right[1][0];
		(*this)[1][1] = right[1][1];
	}

public:
	inline val_type det(void) const
	{
		return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
	}

public:

	inline std::size_t hash(void) const
	{
		const base_type& base = *this;
		std::size_t seed = 0;
		boost::hash_combine(seed, "determinant");
		boost::hash_combine(seed, base);
		return seed;
	}

protected:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"determinant_data",
				(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant<Val, 3, 3, Base>
	: public Base< Base<typename upper_signed<Val>::type, 3>, 3>
{
public:
	YGGR_STATIC_CONSTANT(std::size_t, E_ROW_LENGTH = 3);
	YGGR_STATIC_CONSTANT(std::size_t, E_COL_LENGTH = 3);

public:
	typedef typename upper_signed<Val>::type val_type;
	typedef Base<val_type, E_COL_LENGTH> row_type;

private:
	typedef Base<size_type, E_COL_LENGTH> index_array_type;

	typedef Base< row_type, E_ROW_LENGTH> base_type;
	typedef determinant this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	determinant(void)
	{
		this_type::zero();
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	determinant(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH> right)
	{
		this_type::assign(right);
	}

	determinant(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	determinant(const base_type& right)
		: base_type(right)
	{
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	determinant(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		this_type::assign(right);
	}

	determinant(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	determinant(const this_type& right)
		: base_type(right)
	{
	}

	~determinant(void)
	{
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& operator=(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		this_type::assign(right);

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type& base = *this;
		base = boost::move(right_ref);
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& base = *this;
		base = right;
		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& operator=(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		this_type::assign(right);

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
		if(this == &right) 
		{
			return;
		}

		base_type::swap(right);
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool equal_to(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		return 
			cmper_type((*this)[0][0]) == right[0][0] 
			&& cmper_type((*this)[0][1]) == (*this)[0][1] 
			&& cmper_type((*this)[0][2]) == (*this)[0][2] 

			&& cmper_type((*this)[1][0]) == (*this)[1][0] 
			&& cmper_type((*this)[1][1]) == (*this)[1][1]
			&& cmper_type((*this)[1][2]) == (*this)[1][2]
			
			&& cmper_type((*this)[2][0]) == (*this)[2][0] 
			&& cmper_type((*this)[2][1]) == (*this)[2][1]
			&& cmper_type((*this)[2][2]) == (*this)[2][2];
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool not_equal_to(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right) const
	{
		return !this_type::equal_to(right);
	}

public:
	inline static size_type row_size(void)
	{
		return E_ROW_LENGTH;
	}

	inline static size_type col_size(void)
	{
		return E_COL_LENGTH;
	}

	inline this_type& zero(void)
	{
		(*this)[0][0] = (*this)[0][1] = (*this)[0][2]
		= (*this)[1][0] = (*this)[1][1] = (*this)[1][2]
		= (*this)[2][0] = (*this)[2][1] = (*this)[2][2] = val_type();

		return *this;
	}

	inline bool is_zero(void) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		cmper_type cmper((*this)[0][0]);

		return 
			(cmper == val_type() && cmper == (*this)[0][1] && cmper == (*this)[0][2]
				&& cmper == (*this)[1][0] && cmper == (*this)[1][1] && cmper == (*this)[1][2]
				&& cmper == (*this)[2][0] && cmper == (*this)[2][1] && cmper == (*this)[2][2]);
	}

public:
	using base_type::operator[];

	inline val_type& operator()(const size_type& i, const size_type& j)
	{
		assert(i < E_ROW_LENGTH && j < E_COL_LENGTH);
		return (*this)[i][j];
	}

	inline const val_type& operator()(const size_type& i, const size_type& j) const
	{
		assert(i < E_ROW_LENGTH && j < E_COL_LENGTH);
		return (*this)[i][j];
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	void assign(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
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
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	void assign(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
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
	}

public:
	inline val_type det(void) const
	{
		return 
			(*this)[0][0] * (*this)[1][1] * (*this)[2][2]
			+ (*this)[0][1] * (*this)[1][2] * (*this)[2][0]
			+ (*this)[0][2] * (*this)[1][0] * (*this)[2][1]

			- (*this)[0][2] * (*this)[1][1] * (*this)[2][0]
			- (*this)[0][1] * (*this)[1][0] * (*this)[2][2]
			- (*this)[0][0] * (*this)[1][2] * (*this)[2][1];
	}

public:
	inline std::size_t hash(void) const
	{
		const base_type& base = *this;
		std::size_t seed = 0;
		boost::hash_combine(seed, "determinant");
		boost::hash_combine(seed, base);
		return seed;
	}

protected:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"determinant_data",
				(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
class determinant<Val, 4, 4, Base>
	: public Base< Base<typename upper_signed<Val>::type, 4>, 4>
{
public:
	YGGR_STATIC_CONSTANT(std::size_t, E_ROW_LENGTH = 4);
	YGGR_STATIC_CONSTANT(std::size_t, E_COL_LENGTH = 4);

public:
	typedef typename upper_signed<Val>::type val_type;
	typedef Base<val_type, E_COL_LENGTH> row_type;

private:
	typedef Base<size_type, E_COL_LENGTH> index_array_type;

	typedef Base< row_type, E_ROW_LENGTH> base_type;
	typedef determinant this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	determinant(void)
	{
		this_type::zero();
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	determinant(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		this_type::assign(right);
	}

	determinant(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	determinant(const base_type& right)
		: base_type(right)
	{
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase>
	determinant(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		this_type::assign(right);
	}

	determinant(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	determinant(const this_type& right)
		: base_type(right)
	{
	}

	~determinant(void)
	{
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& operator=(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
	{
		this_type::assign(right);

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type& base = *this;
		base = boost::move(right_ref);
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& base = *this;
		base = right;
		return *this;
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	this_type& operator=(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
	{
		this_type::assign(right);

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
		if(this == &right) 
		{
			return;
		}

		base_type::swap(right);
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool equal_to(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right) const
	{
		typedef flaw_comparer<val_type> cmper_type;

		return 
			cmper_type((*this)[0][0]) == right[0][0] 
			&& cmper_type((*this)[0][1]) == (*this)[0][1] 
			&& cmper_type((*this)[0][2]) == (*this)[0][2] 
			&& cmper_type((*this)[0][3]) == (*this)[0][3] 

			&& cmper_type((*this)[1][0]) == (*this)[1][0] 
			&& cmper_type((*this)[1][1]) == (*this)[1][1]
			&& cmper_type((*this)[1][2]) == (*this)[1][2]
			&& cmper_type((*this)[1][3]) == (*this)[1][3]
			
			&& cmper_type((*this)[2][0]) == (*this)[2][0] 
			&& cmper_type((*this)[2][1]) == (*this)[2][1]
			&& cmper_type((*this)[2][2]) == (*this)[2][2]
			&& cmper_type((*this)[2][3]) == (*this)[2][3]

			&& cmper_type((*this)[3][0]) == (*this)[3][0] 
			&& cmper_type((*this)[3][1]) == (*this)[3][1]
			&& cmper_type((*this)[3][2]) == (*this)[3][2]
			&& cmper_type((*this)[3][3]) == (*this)[3][3];
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	bool not_equal_to(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right) const
	{
		return !this_type::equal_to(right);
	}

public:
	inline static size_type row_size(void)
	{
		return E_ROW_LENGTH;
	}

	inline static size_type col_size(void)
	{
		return E_COL_LENGTH;
	}

	inline this_type& zero(void)
	{
		(*this)[0][0] = (*this)[0][1] = (*this)[0][2] = (*this)[0][3]
		= (*this)[1][0] = (*this)[1][1] = (*this)[1][2] = (*this)[1][3]
		= (*this)[2][0] = (*this)[2][1] = (*this)[2][2] = (*this)[2][3]
		= (*this)[3][0] = (*this)[3][1] = (*this)[3][2] = (*this)[3][3]
		= val_type();

		return *this;
	}

	inline bool is_zero(void) const
	{

		typedef flaw_comparer<val_type> cmper_type;

		cmper_type cmper((*this)[0][0]);

		return 
			(cmper == val_type() && cmper == (*this)[0][1] && cmper == (*this)[0][2] && cmper == (*this)[0][3]
				&& cmper == (*this)[1][0] && cmper == (*this)[1][1] && cmper == (*this)[1][2] && cmper == (*this)[1][3]
				&& cmper == (*this)[2][0] && cmper == (*this)[2][1] && cmper == (*this)[2][2] && cmper == (*this)[2][3]
				&& cmper == (*this)[3][0] && cmper == (*this)[3][1] && cmper == (*this)[3][2] && cmper == (*this)[3][3]);
	}

public:
	using base_type::operator[];

	inline val_type& operator()(const size_type& i, const size_type& j)
	{
		assert(i < E_ROW_LENGTH && j < E_COL_LENGTH);
		return (*this)[i][j];
	}

	inline const val_type& operator()(const size_type& i, const size_type& j) const
	{
		assert(i < E_ROW_LENGTH && j < E_COL_LENGTH);
		return (*this)[i][j];
	}

public:
	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	void assign(const OBase<OVal, E_ROW_LENGTH * E_COL_LENGTH>& right)
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
	}

	template<typename OVal, template<typename _Val, std::size_t> class OBase> inline
	void assign(const determinant<OVal, E_ROW_LENGTH, E_COL_LENGTH, OBase>& right)
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
	}

public:
	inline val_type det(void) const
	{
		return	
			(*this)[0][0] * (*this)[1][1] * (*this)[2][2] * (*this)[3][3] * sign_array_indexes<0, 4>::value
			+ (*this)[0][1] * (*this)[1][2] * (*this)[2][3] * (*this)[3][0] * sign_array_indexes<1, 4>::value
			+ (*this)[0][2] * (*this)[1][3] * (*this)[2][0] * (*this)[3][1] * sign_array_indexes<2, 4>::value
			+ (*this)[0][3] * (*this)[1][0] * (*this)[2][1] * (*this)[3][2] * sign_array_indexes<3, 4>::value

			+ (*this)[0][3] * (*this)[1][2] * (*this)[2][1] * (*this)[3][0] * sign_array_indexes<3, 4, false>::value
			+ (*this)[0][2] * (*this)[1][1] * (*this)[2][0] * (*this)[3][3] * sign_array_indexes<2, 4, false>::value
			+ (*this)[0][1] * (*this)[1][0] * (*this)[2][3] * (*this)[3][2] * sign_array_indexes<1, 4, false>::value
			+ (*this)[0][0] * (*this)[1][3] * (*this)[2][2] * (*this)[3][1] * sign_array_indexes<0, 4, false>::value;
	}

public:
	inline std::size_t hash(void) const
	{
		const base_type& base = *this;
		std::size_t seed = 0;
		boost::hash_combine(seed, "determinant");
		boost::hash_combine(seed, base);
		return seed;
	}

protected:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"determinant_data",
				(boost::serialization::base_object<base_type, this_type>(*this)));
	}
};

} // namespace math
} // namespace yggr


namespace yggr
{
namespace math
{

template<typename ValL, typename ValR, 
			std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class BaseR> inline
bool operator==(const determinant<ValL, ROW, COL, BaseL>& l, const determinant<ValR, ROW, COL, BaseR>& r)
{
	return l.equal_to(r);
}

template<typename ValL, typename ValR, 
			std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class BaseR> inline
bool operator!=(const determinant<ValL, ROW, COL, BaseL>& l, const determinant<ValR, ROW, COL, BaseR>& r)
{
	return !l.equal_to(r);
}

} // namespace math
} // namespace yggr

namespace yggr
{
template<typename Val, std::size_t ROW, std::size_t COL,
			template <typename _Val, std::size_t>
			class Base >
struct upper_float< math::determinant<Val, ROW, COL, Base> >
{
	typedef typename 
		math::determinant
		<
			typename upper_float< Val >::type, ROW, COL, Base 
		> type;
};

template<typename Val, std::size_t ROW, std::size_t COL,
			template <typename _Val, std::size_t>
			class Base >
struct upper_signed< math::determinant<Val, ROW, COL, Base> >
{
	typedef typename 
		math::determinant
		<
			typename upper_signed< Val >::type, ROW, COL, Base 
		> type;
};

} // namespace yggr

// abs support
namespace yggr
{
namespace math
{
namespace math_support
{

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base> inline
determinant<Val, ROW, COL, Base>
	abs(const determinant<Val, ROW, COL, Base>& x)
{
	return x.det();
}

} // namespace math_support

using math_support::abs;

} // namespace math
} // namespace yggr

// swap support
namespace yggr
{
namespace math
{
namespace swap_support
{

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base> inline
void swap(determinant<Val, ROW, COL, Base>& l, determinant<Val, ROW, COL, Base>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

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

template<typename Char, typename Traits,
			typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const yggr::math::determinant<Val, ROW, COL, Base>& det)
{
	typedef Val val_type;
	typedef yggr::math::determinant<Val, ROW, COL, Base> det_type;


	for(yggr::size_type i = 0, isize = det_type::row_size(); i != isize; ++i)
	{
		os << "|\t";
		for(yggr::size_type j = 0, jsize = det_type::col_size(); j != jsize; ++j)
		{
			os << det[i][j] << "\t";
		}

		os << "|\n";
	}

	return os;
}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			typename Val, std::size_t ROW, std::size_t COL,
//			template<typename _Char, typename _Traits> class Basic_OStream,
//			template<typename _Val, std::size_t> class Base>
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os,
//				const yggr::math::determinant<Val, ROW, COL, Base>& det);

} //namespace math
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

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base> inline
std::size_t hash_value(const yggr::math::determinant<Val, ROW, COL, Base>& val)
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

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base>
struct hash< yggr::math::determinant<Val, ROW, COL, Base> >
{
	typedef yggr::math::determinant<Val, ROW, COL, Base> type;

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

#endif // __YGGR_MATH_DETERMINANT_HPP__
