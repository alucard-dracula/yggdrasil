//elo_rating.hpp

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

#ifndef __YGGR_MATH_ELO_RATING_HPP__
#define __YGGR_MATH_ELO_RATING_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/type_traits/upper_types.hpp>
#include <yggr/mplex/limits.hpp>

#include <complex>

namespace yggr
{
namespace math
{

// fast
class fast_elo_rating
{
public:
	YGGR_STATIC_CONSTANT(u32, E_normal_k = 16);
	YGGR_STATIC_CONSTANT(u32, E_profession_k = 16);
	
private:
	typedef fast_elo_rating this_type;

public:
	template<typename T> inline
	typename upper_unsigned<T>::type
		delta_rating(const T& ra, const T& rb) const
	{
		return this_type::s_delta_rating(ra, rb);
	}

	template<typename S, typename T> inline
	typename upper_unsigned<T>::type
		rating(const S& s, const T& ra, const T& rb) const
	{
		return this_type::s_rating(s, ra, rb);
	}

	template<typename S, typename T> inline
	typename upper_unsigned<T>::type
		operator()(const S& s, const T& ra, const T& rb) const
	{
		return this_type::s_rating(s, ra, rb);
	}
public:
	// fast
	template<typename T> inline
	static typename upper_unsigned<T>::type
		s_delta_rating(const T& ra, const T& rb)
	{
		// this foo k = 16
		typedef typename upper_signed<T>::type val_type;
		val_type ret = 16 + (val_type(rb) - val_type(ra)) * 4 / 100;
		return ret > 31? 31 : ret < 1? 0 : ret;
	}

	template<typename S, typename T> inline
	static typename upper_unsigned<T>::type
		s_rating(const S& s, const T& ra, const T& rb)
	{
		typedef typename upper_unsigned<T>::type rst_type;
		
		if(s >= S(0.5f))
		{
			rst_type delta = this_type::s_delta_rating(ra, rb);
			return mplex::numeric_limits<rst_type>::max_type::value - ra < delta 
						? mplex::numeric_limits<rst_type>::max_type::value : ra + delta;
		}

		rst_type delta = this_type::s_delta_rating(rb, ra);
		return ra < delta? rst_type() : ra - delta;
	}
};

// normal
class elo_rating
{
public:
	YGGR_STATIC_CONSTANT(u32, E_normal_k = 32);
	YGGR_STATIC_CONSTANT(u32, E_profession_k = 16);
	
private:
	typedef elo_rating this_type;

public:
	
	template<typename S, typename T> inline
	typename upper_float<T>::type
		delta_rating(const S& s, const T& k, const T& ra, const T& rb) const
	{
		return this_type::s_delta_rating(s, k, ra, rb);
	}

	template<typename S, typename T> inline
	typename upper_unsigned<T>::type
		rating(const S& s, const T& k, const T& ra, const T& rb) const
	{
		return this_type::s_rating(s, k, ra, rb);
	}

	template<typename S, typename T> inline
	typename upper_unsigned<T>::type
		operator()(const S& s, const T& k, const T& ra, const T& rb) const
	{
		return this_type::s_rating(s, k, ra, rb);
	}

public:
	template<typename S, typename T> inline
	static typename upper_float<T>::type
		s_delta_rating(const S& s, const T& k, const T& ra, const T& rb)
	{
		typedef typename upper_float<T>::type value_type;
		return value_type(k) * (value_type(s) - this_type::s_elo(ra, rb));
	}

	template<typename S, typename T> inline
	static typename upper_unsigned<T>::type
		s_rating(const S& s, const T& k, const T& ra, const T& rb)
	{
		typedef typename upper_float<T>::type fvalue_type;
		typedef typename upper_unsigned<T>::type uvalue_type;
		uvalue_type rst = uvalue_type(fvalue_type(ra) + fvalue_type(this_type::s_delta_rating(s, k, ra, rb)) + fvalue_type(0.5));
		return rst < uvalue_type()? uvalue_type() : rst;
	}

private:
	template<typename T> inline
	static typename upper_float<T>::type
		s_elo(const T& ra, const T& rb)
	{
		typedef typename upper_float<T>::type value_type;
		value_type elo_param = (value_type(rb) - value_type(ra)) / value_type(400);
		return value_type(1) / (value_type(1) + std::pow(value_type(10), elo_param));
	}

};

} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_ELO_RATING_HPP__