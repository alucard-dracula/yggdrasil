//value_miss_comparer.hpp

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

#ifndef __YGGR_MATH_MISS_VALUE_HPP__
#define __YGGR_MATH_MISS_VALUE_HPP__

#include <yggr/base/yggrdef.h>
#include <complex>

namespace yggr
{
namespace math
{

template<typename T>
class gen_miss_val
{
public:
	inline const T operator()(void)
	{
		return T();
	}
};

template<>
class gen_miss_val<f32>
{
public:
	inline const f32 operator()(void)
	{
		return 0.000001f;
		//return 0.001f;
	}
};

template<>
class gen_miss_val<f64>
{
public:
	inline const f64 operator()(void)
	{
		//return 0.001f;
		return 0.00000001;
	}
};

template<typename T> 
class value_miss_comparer
{
private:
	typedef T val_type;
	typedef value_miss_comparer<val_type> this_type;

public:
	value_miss_comparer(const val_type& val)
		:_val(val)
	{
	}
 
	value_miss_comparer(const this_type& right)
		: _val(right._val)
	{
	}

	~value_miss_comparer(void)
	{
	}
	
	// > >= < <= == != opbool

	inline operator const bool(void) const
	{
		return static_cast<const bool>(_val);
	}

	template<typename OVal>
	inline bool operator>(const OVal& val) const
	{
		return _val > val;
	}

	inline bool operator>(const val_type& val) const
	{
		return _val > val;
	}

	inline bool operator>(const this_type& right) const
	{
		return _val > right._val;
	}

	template<typename OVal>
	inline bool operator>=(const OVal& val) const
	{
		return _val >= val;
	}

	inline bool operator>=(const val_type& val) const
	{
		return _val >= val;
	}

	inline bool operator>=(const this_type& right) const
	{
		return _val >= right._val;
	}

	template<typename OVal>
	inline bool operator<(const OVal& val) const
	{
		return _val < val;
	}

	inline bool operator<(const val_type& val) const
	{
		return _val < val;
	}

	inline bool operator<(const this_type& right) const
	{
		return _val < right._val;
	}

	template<typename OVal>
	inline bool operator<=(const OVal& val) const
	{
		return _val <= val;
	}

	inline bool operator<=(const val_type& val) const
	{
		return _val <= val;
	}

	inline bool operator<=(const this_type& right) const
	{
		return _val <= right._val;
	}

	template<typename OVal>
	inline bool operator==(const OVal& val) const
	{
		return _val == val;
	}

	inline bool operator==(const val_type& val) const
	{
		return _val == val;
	}

	inline bool operator==(const this_type& right) const
	{
		return _val == right._val;
	}

	template<typename OVal>
	inline bool operator!=(const OVal& val) const
	{
		return _val != val;
	}

	inline bool operator!=(const val_type& val) const
	{
		return _val != val;
	}

	inline bool operator!=(const this_type& right) const
	{
		return _val != right._val;
	}

private:
	const val_type& _val; 
};

template<> 
class value_miss_comparer<f32>
{
private:
	typedef f32 val_type;
	typedef value_miss_comparer<val_type> this_type;

public:
	value_miss_comparer(const val_type& val)
		:_val(val)
	{
	}
 
	value_miss_comparer(const this_type& right)
		: _val(right._val)
	{
	}

	~value_miss_comparer(void)
	{
	}
	
	// > >= < <= == != opbool

	inline operator const bool(void) const
	{
		return static_cast<const bool>(_val);
	}

	template<typename OVal>
	inline bool operator>(const OVal& val) const
	{
		return cmp_greater(_val, val);
	}

	inline bool operator>(const val_type& val) const
	{
		return cmp_greater(_val, val);
	}

	inline bool operator>(const this_type& right) const
	{
		return cmp_greater(_val, right._val);
	}

	template<typename OVal>
	inline bool operator>=(const OVal& val) const
	{
		return cmp_greater_equal(_val, val);
	}

	inline bool operator>=(const val_type& val) const
	{
		return cmp_greater_equal(_val, val);
	}

	inline bool operator>=(const this_type& right) const
	{
		return cmp_greater_equal(_val, right._val);
	}

	template<typename OVal>
	inline bool operator<(const OVal& val) const
	{
		return cmp_less(_val, val);
	}

	inline bool operator<(const val_type& val) const
	{
		return cmp_less(_val, val);
	}

	inline bool operator<(const this_type& right) const
	{
		return cmp_less(_val, right._val);
	}

	template<typename OVal>
	inline bool operator<=(const OVal& val) const
	{
		return cmp_less_equal(_val, val);
	}

	inline bool operator<=(const val_type& val) const
	{
		return cmp_less_equal(_val, val);
	}

	inline bool operator<=(const this_type& right) const
	{
		return cmp_less_equal(_val, right._val);
	}

	template<typename OVal>
	inline bool operator==(const OVal& val) const
	{
		return cmp_equal(_val, val);
	}

	inline bool operator==(const val_type& val) const
	{
		return cmp_equal(_val, val);
	}

	inline bool operator==(const this_type& right) const
	{
		return cmp_equal(_val, right._val);
	}

	template<typename OVal>
	inline bool operator!=(const OVal& val) const
	{
		return cmp_not_equal(_val, val);
	}

	inline bool operator!=(const val_type& val) const
	{
		return cmp_not_equal(_val, val);
	}

	inline bool operator!=(const this_type& right) const
	{
		return cmp_not_equal(_val, right._val);
	}

	template<typename OVal>
	inline static bool cmp_equal(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return std::abs(val1 - val2) < miss;
	}

	inline static bool cmp_equal(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return std::abs(val1 - val2) < miss;
	}

	template<typename OVal>
	inline static bool cmp_not_equal(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return !cmp_equal(val1, val2, miss);
	}

	inline static bool cmp_not_equal(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return !cmp_equal(val1, val2, miss);
	}

	template<typename OVal>
	inline static bool cmp_less(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return cmp_not_equal(val1, val2, miss) && (val1 < val2);
	}

	inline static bool cmp_less(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return cmp_not_equal(val1, val2, miss) && (val1 < val2);
	}

	template<typename OVal>
	inline static bool cmp_less_equal(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return cmp_equal(val1, val2) || (val1 < val2);
	}

	inline static bool cmp_less_equal(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return cmp_equal(val1, val2) || (val1 < val2);
	}

	template<typename OVal>
	inline static bool cmp_greater(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return cmp_not_equal(val1, val2, miss) && (val1 > val2);
	}

	inline static bool cmp_greater(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return cmp_not_equal(val1, val2, miss) && (val1 > val2);
	}

	template<typename OVal>
	inline static bool cmp_greater_equal(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return cmp_equal(val1, val2) || (val1 > val2);
	}

	inline static bool cmp_greater_equal(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return cmp_equal(val1, val2) || (val1 > val2);
	}

	inline static val_type def_miss_size(void)
	{
		gen_miss_val<val_type> gen;
		return gen();
	}

private:
	const val_type& _val; 
};

template<> 
class value_miss_comparer<f64>
{
private:
	typedef f64 val_type;
	typedef value_miss_comparer<val_type> this_type;

public:
	value_miss_comparer(const val_type& val)
		:_val(val)
	{
	}
 
	value_miss_comparer(const this_type& right)
		: _val(right._val)
	{
	}

	~value_miss_comparer(void)
	{
	}
	
	// > >= < <= == != opbool

	inline operator const bool(void) const
	{
		return static_cast<const bool>(_val);
	}

	template<typename OVal>
	inline bool operator>(const OVal& val) const
	{
		return cmp_greater(_val, val);
	}

	inline bool operator>(const val_type& val) const
	{
		return cmp_greater(_val, val);
	}

	inline bool operator>(const this_type& right) const
	{
		return cmp_greater(_val, right._val);
	}

	template<typename OVal>
	inline bool operator>=(const OVal& val) const
	{
		return cmp_greater_equal(_val, val);
	}

	inline bool operator>=(const val_type& val) const
	{
		return cmp_greater_equal(_val, val);
	}

	inline bool operator>=(const this_type& right) const
	{
		return cmp_greater_equal(_val, right._val);
	}

	template<typename OVal>
	inline bool operator<(const OVal& val) const
	{
		return cmp_less(_val, val);
	}

	inline bool operator<(const val_type& val) const
	{
		return cmp_less(_val, val);
	}

	inline bool operator<(const this_type& right) const
	{
		return cmp_less(_val, right._val);
	}

	template<typename OVal>
	inline bool operator<=(const OVal& val) const
	{
		return cmp_less_equal(_val, val);
	}

	inline bool operator<=(const val_type& val) const
	{
		return cmp_less_equal(_val, val);
	}

	inline bool operator<=(const this_type& right) const
	{
		return cmp_less_equal(_val, right._val);
	}

	template<typename OVal>
	inline bool operator==(const OVal& val) const
	{
		return cmp_equal(_val, val);
	}

	inline bool operator==(const val_type& val) const
	{
		return cmp_equal(_val, val);
	}

	inline bool operator==(const this_type& right) const
	{
		return cmp_equal(_val, right._val);
	}

	template<typename OVal>
	inline bool operator!=(const OVal& val) const
	{
		return cmp_not_equal(_val, val);
	}

	inline bool operator!=(const val_type& val) const
	{
		return cmp_not_equal(_val, val);
	}

	inline bool operator!=(const this_type& right) const
	{
		return cmp_not_equal(_val, right._val);
	}

	template<typename OVal>
	inline static bool cmp_equal(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return std::abs(val1 - val2) < miss;
	}

	inline static bool cmp_equal(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return std::abs(val1 - val2) < miss;
	}

	template<typename OVal>
	inline static bool cmp_not_equal(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return !cmp_equal(val1, val2, miss);
	}

	inline static bool cmp_not_equal(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return !cmp_equal(val1, val2, miss);
	}

	template<typename OVal>
	inline static bool cmp_less(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return cmp_not_equal(val1, val2, miss) && (val1 < val2);
	}

	inline static bool cmp_less(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return cmp_not_equal(val1, val2, miss) && (val1 < val2);
	}

	template<typename OVal>
	inline static bool cmp_less_equal(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return cmp_equal(val1, val2) || (val1 < val2);
	}

	inline static bool cmp_less_equal(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return cmp_equal(val1, val2) || (val1 < val2);
	}

	template<typename OVal>
	inline static bool cmp_greater(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return cmp_not_equal(val1, val2, miss) && (val1 > val2);
	}

	inline static bool cmp_greater(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return cmp_not_equal(val1, val2, miss) && (val1 > val2);
	}

	template<typename OVal>
	inline static bool cmp_greater_equal(const val_type& val1, const OVal& val2, const val_type& miss = def_miss_size())
	{
		return cmp_equal(val1, val2) || (val1 > val2);
	}

	inline static bool cmp_greater_equal(const val_type& val1, const val_type& val2, const val_type& miss = def_miss_size())
	{
		return cmp_equal(val1, val2) || (val1 > val2);
	}

	inline static val_type def_miss_size(void)
	{
		gen_miss_val<val_type> gen;
		return gen();
	}

private:
	const val_type& _val; 
};

} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_MISS_VALUE_HPP__