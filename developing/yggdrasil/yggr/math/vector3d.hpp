//vector3d.hpp

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

#ifndef __YGGR_MATH_VECTOR3D_HPP__
#define __YGGR_MATH_VECTOR3D_HPP__

#include <ostream>
#include <boost/array.hpp>
#include <boost/serialization/access.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/math/math.hpp>
#include <yggr/utility/value_swap.hpp>
#include <yggr/utility/value_copy.hpp>

#include <yggr/serialization/array.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4244)
#endif //_MSC_VER

namespace yggr
{
namespace math
{

template<typename Val,
			template <typename _Val, std::size_t> class Base = boost::array>
class vector3d;

template<typename Val, std::size_t ROW, std::size_t COL,
			template<typename _Val, std::size_t> class Base>
class determinant;

template<typename Val,
	template<typename _Val, std::size_t> class Array,
	template<typename _Val, std::size_t, std::size_t,
				template<typename __Val, std::size_t> class _Base> class Base>
class matrix3d;

} // namesapce math

namespace math
{
template<typename Val,
			template <typename _Val, std::size_t> class Base>
class vector3d : public Base<typename upper_signed<Val>::value_type, 4>
{
public:
	typedef typename upper_signed<Val>::value_type val_type;
	typedef vector3d cross_result_type;

public:
	enum
	{
		E_LENGTH = 3,
		E_ARRAY_LENGTH = E_LENGTH + 1,
		E_compile_u32 = 0xffffffff
	};

	typedef Base<val_type, E_ARRAY_LENGTH> base_type;

private:
	typedef vector3d this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	vector3d(void)
		: x(base_type::operator[](0)), y(base_type::operator[](1)),
			z(base_type::operator[](2)), w(base_type::operator[](3))
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}

	template<typename OVal1, typename OVal2, typename OVal3>
	vector3d(const OVal1& val1, const OVal2& val2, const OVal3& val3)
		: x(base_type::operator[](0)), y(base_type::operator[](1)),
			z(base_type::operator[](2)), w(base_type::operator[](3))
	{
		x = static_cast<val_type>(val1);
		y = static_cast<val_type>(val2);
		z = static_cast<val_type>(val3);
		w = static_cast<val_type>(1);
	}

	vector3d(const val_type& val1, const val_type& val2, const val_type& val3)
		: x(base_type::operator[](0)), y(base_type::operator[](1)),
			z(base_type::operator[](2)), w(base_type::operator[](3))
	{
		x = val1;
		y = val2;
		z = val3;
		w = static_cast<val_type>(1);
	}

	vector3d(const base_type& right)
		: base_type(right),
			x(base_type::operator[](0)), y(base_type::operator[](1)),
			z(base_type::operator[](2)), w(base_type::operator[](3))
	{
		w = 1;
	}

	template<typename OVal>
	vector3d(const vector3d<OVal, Base>& right)
		: x(base_type::operator[](0)), y(base_type::operator[](1)),
			z(base_type::operator[](2)), w(base_type::operator[](3))
	{
		(*this)[0] = static_cast<val_type>(right[0]);
		(*this)[1] = static_cast<val_type>(right[1]);
		(*this)[2] = static_cast<val_type>(right[2]);
		(*this)[3] = static_cast<val_type>(right[3]);
	}

	vector3d(BOOST_RV_REF(this_type) right)
		: base_type(right),
			x(base_type::operator[](0)), y(base_type::operator[](1)),
			z(base_type::operator[](2)), w(base_type::operator[](3))
	{
	}

	vector3d(const this_type& right)
		: base_type(right),
			x(base_type::operator[](0)), y(base_type::operator[](1)),
			z(base_type::operator[](2)), w(base_type::operator[](3))
	{
	}

	~vector3d(void)
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

	inline const size_type array_size(void) const
	{
		return E_ARRAY_LENGTH;
	}

	inline static const size_type s_array_size(void)
	{
		return E_ARRAY_LENGTH;
	}

	void clear(void)
	{
		(*this)[0] = 0;
		(*this)[1] = 0;
		(*this)[2] = 0;
		(*this)[3] = 1;
	}

	operator const bool(void) const
	{
		return (*this) != this_type();
	}

	template<typename OVal>
	operator const vector3d<OVal, Base>(void) const
	{
		return vector3d<OVal, Base>(*this);
	}

	const bool is_zero(void) const
	{
		typedef math::value_miss_comparer<val_type> cmper_type;

		cmper_type cmper((*this)[0]);

		return cmper == 0 && cmper == (*this)[1] && cmper == (*this)[2];
	}

	const size_type zero_count(void) const
	{
		typedef math::value_miss_comparer<val_type> cmper_type;

		return static_cast<size_type>(cmper_type((*this)[0]) == 0)
				+ static_cast<size_type>(cmper_type((*this)[1]) == 0)
				+ static_cast<size_type>(cmper_type((*this)[2]) == 0);
	}

	const this_type operator-(void) const
	{
		this_type tmp;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tmp[i] = -(*this)[i];
		}

		return tmp;
	}

	template<typename OVal>
	const this_type operator+(const vector3d<OVal, Base>& right) const
	{
		this_type tmp;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tmp[i] = (*this)[i] + right[i];
		}

		return tmp;
	}

	template<typename OVal>
	this_type& operator+=(const vector3d<OVal, Base>& right)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] += right[i];
		}

		return *this;
	}

	template<typename OVal>
	const this_type operator-(const vector3d<OVal, Base>& right) const
	{
		this_type tmp;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tmp[i] = (*this)[i] - right[i];
		}

		return tmp;
	}

	template<typename OVal>
	this_type& operator-=(const vector3d<OVal, Base>& right)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] -= right[i];
		}

		return *this;
	}

	template<typename TVal>
	const this_type operator*(const TVal& right) const
	{
		this_type tmp;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tmp[i] = (*this)[i] * right;
		}

		return tmp;
	}

	template<typename OVal>
	const this_type operator*(const vector3d<OVal, Base>& right) const
	{
		this_type tmp;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tmp[i] = (*this)[i] * right[i];
		}

		return tmp;
	}

	template<typename OVal,
				template<typename _Val, std::size_t, std::size_t,
					template<typename __Val, std::size_t> class _Base> class Det>
	const this_type operator*(const matrix3d<OVal, Base, Det>& right) const
	{
		this_type rst((*this)[0] * right[0][0] + (*this)[1] * right[1][0] + (*this)[2] * right[2][0] + (*this)[3] * right[3][0],
						(*this)[0] * right[0][1] + (*this)[1] * right[1][1] + (*this)[2] * right[2][1] + (*this)[3] * right[3][1],
						(*this)[0] * right[0][2] + (*this)[1] * right[1][2] + (*this)[2] * right[2][2] + (*this)[3] * right[3][2]);
		rst[3] = (*this)[0] * right[0][3] + (*this)[1] * right[1][3] + (*this)[2] * right[2][3] + (*this)[3] * right[3][3];
		return rst;
	}

	template<typename TVal>
	this_type& operator*=(const TVal& right)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] *= right;
		}
		return *this;
	}

	template<typename OVal>
	this_type& operator*=(const vector3d<OVal, Base>& right)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] *= right[i];
		}
		return *this;
	}

	template<typename OVal,
				template<typename _Val, std::size_t, std::size_t,
					template<typename __Val, std::size_t> class _Base> class Det>
	this_type operator*=(const matrix3d<OVal, Base, Det>& right)
	{
		this_type rst((*this)[0] * right[0][0] + (*this)[1] * right[1][0] + (*this)[2] * right[2][0] + (*this)[3] * right[3][0],
						(*this)[0] * right[0][1] + (*this)[1] * right[1][1] + (*this)[2] * right[2][1] + (*this)[3] * right[3][1],
						(*this)[0] * right[0][2] + (*this)[1] * right[1][2] + (*this)[2] * right[2][2] + (*this)[3] * right[3][2]);
		rst[3] = (*this)[0] * right[0][3] + (*this)[1] * right[1][3] + (*this)[2] * right[2][3] + (*this)[3] * right[3][3];
		*this = rst;
		return *this;
	}

	template<typename TVal>
	const this_type operator/(const TVal& right) const
	{
		this_type tmp;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tmp[i] = (*this)[i] / right;
		}

		return tmp;
	}

	template<typename OVal>
	const this_type operator/(const vector3d<OVal, Base>& right) const
	{
		this_type tmp;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tmp[i] = (*this)[i] / right[i];
		}

		return tmp;
	}

	template<typename TVal>
	this_type& operator/=(const TVal& right)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] /= right;
		}
		return *this;
	}

	template<typename OVal>
	this_type& operator/=(const vector3d<OVal, Base>& right)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] /= right[i];
		}
		return *this;
	}

	template<typename TVal>
	this_type& operator=(const TVal& right)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] = static_cast<val_type>(right);
		}
		return *this;
	}

	template<typename OVal>
	this_type& operator=(const vector3d<OVal, Base>& right)
	{
		for(size_type i = 0; i != E_ARRAY_LENGTH; ++i)
		{
			(*this)[i] = static_cast<val_type>(right[i]);
		}

		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		base_type& base = *this;
		if(&base == &right) {return *this;}

#ifdef _DEBUG
		value_type* p = &base[0];
#endif //_DEBUG

		utility::value_copy<base_type>(base, right);

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		this_type& right_ref = right;

#ifdef _DEBUG
		value_type* p = &base[0];
#endif //_DEBUG

		utility::value_copy<base_type>(base, right_ref);

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;

#ifdef _DEBUG
		value_type* p = &base[0];
#endif //_DEBUG

		utility::value_copy<base_type>(base, right);

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
		return *this;
	}

	void swap(base_type& base)
	{
		base_type& base = *this;
		if(&base == &right) {return;}

#ifdef _DEBUG
		value_type* p = &base[0];
#endif //_DEBUG
		
		utility::value_swap<base_type>(base, right);

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG
	}

	void swap(this_type& right)
	{
		if(this == &right) { return; }
		base_type& base =*this;

#ifdef _DEBUG
		value_type* p = &base[0];
#endif //_DEBUG
		
		utility::value_swap<base_type>(base, right);

#ifdef _DEBUG
		assert((p == &base[0]));
#endif //_DEBUG

	}

	template<typename OVal>
	bool operator==(const vector3d<OVal, Base>& right) const
	{
		typedef math::value_miss_comparer<val_type> val_cmper_type;

		for(size_type i = 0; i !=  E_ARRAY_LENGTH; ++i)
		{
			if(val_cmper_type((*this)[i]) != right[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename OVal>
	bool operator!=(const vector3d<OVal, Base>& right) const
	{
		return ! this_type::operator==<OVal>(right);
	}

	this_type& normalize(void)
	{
		typedef math::value_miss_comparer<val_type> val_cmper_type;
		typedef typename upper_float<val_type>::value_type float_type;

		val_type len_sq = length_sq();
		if(val_cmper_type(len_sq) == val_type())
		{
			return *this;
		}

		float_type tval = float_type();
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			tval = (*this)[i];
			(*this)[i] = math::sign(tval) * math::sqrt((tval * tval) / len_sq);
		}

		return *this;
	}

	vector3d<typename upper_float<val_type>::value_type> get_normalized_vector(void) const
	{
		typedef vector3d<typename upper_float<val_type>::value_type> ret_type;
		ret_type ret(*this);
		return ret.normalize();
	}

	this_type& round(void)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] = std::floor(0.5 + (*this)[i]);
		}

		return *this;
	}

	this_type& floor(void)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] = std::floor((*this)[i]);
		}

		return *this;
	}

	this_type& ceil(void)
	{
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			(*this)[i] = std::ceil((*this)[i]);
		}

		return *this;
	}

	val_type sum(void) const
	{
		return x + y + z;
	}

	typename upper_float<val_type>::value_type length(void) const
	{
		typedef typename upper_float<val_type>::value_type rst_type;
		rst_type rst = 0;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			rst += (*this)[i] * (*this)[i];
		}
		return yggr::math::sqrt(rst);
	}

	val_type length_sq(void) const
	{
		val_type rst = 0.0;
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			rst += (*this)[i] * (*this)[i];
		}

		return rst;
	}

	template<typename OVal>
	typename upper_float<val_type>::value_type dot(const vector3d<OVal, Base>& right) const
	{
		typedef typename upper_float<val_type>::value_type rst_type;
		rst_type rst = val_type();
		for(size_type i = 0; i != E_LENGTH; ++i)
		{
			rst += (*this)[i] * right[i];
		}

		return rst;
	}

	template<typename OVal>
	this_type cross(const vector3d<OVal, Base>& right)
	{
		return this_type(
							(*this)[1] * right[2] - (*this)[2] * right[1],
							(*this)[2] * right[0] - (*this)[0] * right[2],
							(*this)[0] * right[1] - (*this)[1] * right[0]
						);
	}

	this_type cross(const this_type& right) const
	{
		return this_type(
							(*this)[1] * right[2] - (*this)[2] * right[1],
							(*this)[2] * right[0] - (*this)[0] * right[2],
							(*this)[0] * right[1] - (*this)[1] * right[0]
						);
	}

	template<typename OVal>
	vector3d<OVal, Base> type_cast(void) const
	{
		typedef OVal oval_type;
		typedef vector3d<oval_type> rst_type;
		rst_type rst;
		for(size_type i = 0; i != E_ARRAY_LENGTH; ++i)
		{
			rst[i] = static_cast<oval_type>((*this)[i]);
		}

		return rst;
	}

	template<typename Ret, typename Handler>
	Ret type_cast(const Handler& handler) const
	{
		return handler(*this);
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("vector3d_data",
										(boost::serialization::base_object<base_type, this_type>(*this)));
		//ar & boost::serialization::base_object<base_type, this_type>(*this);
		//base_type& base = *this;
		//ar & base;
	}
public:
	val_type& x;
	val_type& y;
	val_type& z;
	val_type& w;
};

template<typename Val,
			template<typename _Val, std::size_t> class Base>
const vector3d<typename upper_float<Val>::value_type, Base> sign(const vector3d<Val, Base>& x)
{
	return x.get_normalized_vector();
}

} // namespace math

template<typename Val,
			template <typename _Val, std::size_t>
				class Base >
struct upper_float< math::vector3d<Val, Base> >
{
	typedef typename math::vector3d< typename upper_float<Val>::value_type, Base > value_type;
};

template<typename Val,
			template <typename _Val, std::size_t>
				class Base >
struct upper_signed< math::vector3d<Val, Base> >
{
	typedef typename math::vector3d< typename upper_signed< Val >::value_type, Base > value_type;
};

} // namespace yggr

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t>
				class Base>
yggr::math::vector3d<ValR, Base> operator*(const ValL& l, const yggr::math::vector3d<ValR, Base>& r)
{
	return r * l;
}

namespace std
{

template<typename Val,
			template<typename _Val, std::size_t> class Base>
typename yggr::upper_float<Val>::value_type abs(const yggr::math::vector3d<Val, Base>& x)
{
	return x.length();
}

template<typename Val,
			template<typename _Val, std::size_t> class Base>
void swap(yggr::math::vector3d<Val, Base>& l, yggr::math::vector3d<Val, Base>& r)
{
	l.swap(r);
}

template<typename Val,
			template<typename _Val, std::size_t> class Base>
void swap(typename yggr::math::vector3d<Val, Base>::base_type& l, 
			yggr::math::vector3d<Val, Base>& r)
{
	r.swap(l);
}

} // namespace std

template<typename Char, typename Traits, typename Val,
			template<typename _Val, std::size_t> class Base>
std::basic_ostream<Char, Traits>&
		operator<<(std::basic_ostream<Char, Traits>& os,
					const yggr::math::vector3d<Val, Base>& vec)
{
	typedef Val val_type;
	typedef yggr::math::vector3d<Val> vec_type;
	typedef typename vec_type::const_iterator vec_c_iter_type;
	os << "[ ";
	for(vec_c_iter_type i = vec.begin(), isize = vec.end(); i != isize; ++i)
	{
		os << *i << " ";
	}
	os << "]";
	return os;
}

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif //__YGGR_MATH_VECTOR3D_HPP__
