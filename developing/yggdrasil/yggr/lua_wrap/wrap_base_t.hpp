//wrap_base_t.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_BASE_T_HPP__
#define __YGGR_LUA_WRAP_WRAP_BASE_T_HPP__

#include <iostream>
#include <sstream>
#include <yggr/check_helper/check_half_t.hpp>
#include <math.h>

namespace yggr
{
namespace lua_wrap
{

template<typename T>
class mod
{
private:
    typedef T val_type;
public:
    template<typename Right>
    inline val_type operator()(const val_type& left, const Right& right) const
    {
        return left % (val_type)right;
    }

    inline val_type operator()(const val_type& left, const val_type& right) const
    {
        return left % right;
    }
};

template<typename T>
class mod_equal
{
private:
    typedef T val_type;
public:
    template<typename Right>
    inline void operator()(val_type& left, const Right& right) const
    {
        left %= (val_type)right;
    }

    inline void operator()(val_type& left, const val_type& right) const
    {
        left %= right;
    }
};

template<>
class mod<float>
{
private:
    typedef float val_type;
public:
    template<typename Right>
    inline val_type operator()(const val_type& left, const Right& right) const
    {
        return fmod(left, (val_type)right);
    }

    inline val_type operator()(const val_type& left, const val_type& right) const
    {
        return fmod(left, right);
    }
};

template<>
class mod_equal<float>
{
private:
    typedef float val_type;
public:
    template<typename Right>
    inline void operator()(val_type& left, const Right& right) const
    {
        left = fmod(left, (val_type)right);
    }

    inline void operator()(val_type& left, const val_type& right) const
    {
        left = fmod(left, right);
    }
};

template<>
class mod<double>
{
private:
    typedef double val_type;
public:
    template<typename Right>
    inline val_type operator()(const val_type& left, const Right& right) const
    {
        return fmod(left, (val_type)right);
    }

    inline val_type operator()(const val_type& left, const val_type& right) const
    {
        return fmod(left, right);
    }
};

template<>
class mod_equal<double>
{
private:
    typedef double val_type;
public:
    template<typename Right>
    inline void operator()(val_type& left, const Right& right) const
    {
        left = fmod(left, (val_type)right);
    }

    inline void operator()(val_type& left, const val_type& right) const
    {
        left = fmod(left, right);
    }
};

template<typename Val>
class wrap_base_t
{
public:
	typedef Val val_type;
private:
	typedef check_helper::check_half_t<val_type> check_half_t_type;
	enum
	{
		half_bit_size = check_half_t_type::half_bit_size,
		E_compile_u32 = 0xffffffff
	};
public:
	typedef typename check_half_t_type::half_type half_type;
private:

	typedef wrap_base_t this_type;

public:
	wrap_base_t(void)
		: _val()
	{
	}

	//template<typename T>
	//wrap_base_t(const T val)
	//	: _val(val)
	//{
	//	//_val = val;
	//}

	wrap_base_t(const val_type& val)
		: _val(val)
	{
	}

	//wrap_base_t(const half_type& val)
	//	: _val(val)
	//{
	//}

	wrap_base_t(const std::string& val)
	{
		std::stringstream ss;
		ss << val;
		ss >> _val;
	}

	//wrap_base_t(val_type val)
	//	: _val(val)
	//{
	//}

	//wrap_base_t(half_type val)
	//	: _val(val)
	//{
	//}

	wrap_base_t(const half_type& h, const half_type& l)
	{
		val_type valh = h;
		val_type vall = l;
		valh << half_bit_size;
		_val = valh | vall;
	}

	wrap_base_t(const this_type& right)
		: _val(right._val)
	{
	}

	~wrap_base_t(void)
	{
	}

	operator val_type&(void)
	{
		return _val;
	}

	operator const val_type&(void) const
	{
		return _val;
	}

	const this_type operator-(void) const
	{
		return this_type(-_val);
	}

	template<typename T>
	const this_type operator+(const T& right) const
	{
		return this_type((val_type)(_val + right));
	}

	const this_type operator+(const this_type& right) const
	{
		return this_type(_val + right._val);
	}

	template<typename T>
	this_type& operator+=(const T& right)
	{
		_val += (val_type)right;
		return *this;
	}

	this_type& operator+=(const this_type& right)
	{
		_val += right._val;
		return *this;
	}

	this_type& operator++(void)
	{
		_val += val_type(1);
		return _val;
	}

	const this_type operator++(int)
	{
		this_type ret(_val);
		_val += val_type(1);
		return ret;
	}

	template<typename T>
	const this_type operator-(const T& right) const
	{
		return this_type(_val - (val_type)right);
	}

	const this_type operator-(const this_type& right) const
	{
		return this_type(_val - right._val);
	}

	template<typename T>
	this_type& operator-=(const T& right)
	{
		_val -= (val_type)right;
		return *this;
	}

	template<typename T>
	this_type& operator-=(const this_type& right)
	{
		_val -= right._val;
		return *this;
	}

	this_type& operator--(void)
	{
		_val -= val_type(1);
		return _val;
	}

	const this_type operator--(int)
	{
		this_type ret(_val);
		_val -= val_type(1);
		return ret;
	}

	template<typename T>
	const this_type operator*(const T& right) const
	{
		return this_type(val_type(_val * right));
	}

	const this_type operator*(const this_type& right) const
	{
		return this_type(_val * right._val);
	}

	template<typename T>
	this_type& operator*=(const T& right)
	{
		_val *= (val_type)right;
		return *this;
	}

	this_type& operator*=(const this_type& right)
	{
		_val *= right._val;
		return *this;
	}

	template<typename T>
	const this_type operator/(const T& right) const
	{
		return this_type((val_type)(_val / right));
	}

	const this_type operator/(const this_type& right) const
	{
		return this_type(_val / right._val);
	}

	template<typename T>
	this_type& operator/=(const T& right)
	{
		_val /= (val_type)right;
		return *this;
	}

	this_type& operator/=(const this_type& right)
	{
		_val /= right._val;
		return *this;
	}

	template<typename T>
	const this_type operator%(const T& right) const
	{
		return this_type(mod<val_type>()(_val, right));
	}

	const this_type operator%(const this_type& right) const
	{
		return this_type(mod<val_type>()(_val, right._val));
	}

	template<typename T>
	this_type& operator%=(const T& right)
	{
		mod_equal<val_type>()(_val, right);
		return *this;
	}

	this_type& operator%=(const this_type& right)
	{
		mod_equal<val_type>()(_val, right._val);
		return *this;
	}

	template<typename T>
	this_type& operator=(const T& right)
	{
		_val = right;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_val = right._val;
		return *this;
	}

	template<typename T>
	bool operator&&(const T& right) const
	{
		return _val && right;
	}

	bool operator&&(const this_type& right) const
	{
		return _val && right._val;
	}

	template<typename T>
	bool operator||(const T& right) const
	{
		return _val || right;
	}

	bool operator||(const this_type& right) const
	{
		return _val || right._val;
	}

	bool operator!(void) const
	{
		return !_val;
	}

	template<typename T>
	bool operator==(const T& right) const
	{
		return _val == right;
	}

	bool operator==(const this_type& right) const
	{
		return _val == right._val;
	}

	template<typename T>
	bool operator!=(const T& right) const
	{
		return _val != right;
	}

	bool operator!=(const this_type& right) const
	{
		return _val != right._val;
	}

	template<typename T>
	bool operator<(const T& right) const
	{
		return _val < right;
	}

	bool operator<(const this_type& right) const
	{
		return _val < right._val;
	}

	template<typename T>
	bool operator<=(const T& right) const
	{
		return _val <= right;
	}

	bool operator<=(const this_type& right) const
	{
		return _val <= right._val;
	}

	template<typename T>
	bool operator>(const T& right) const
	{
		return _val > right._val;
	}

	bool operator>(const this_type& right) const
	{
		return _val > right._val;
	}

	template<typename T>
	bool operator>=(const T& right) const
	{
		return _val >= right;
	}

	bool operator>=(const this_type& right) const
	{
		return _val >= right._val;
	}

	template<typename T>
	const this_type operator<<(const T& right) const
	{
		val_type val = _val << right;
		return this_type(val);
	}

	const this_type operator<<(const this_type& right) const
	{
		val_type val = _val << right._val;
		return this_type(val);
	}

	template<typename T>
	const this_type& operator<<=(const T& right)
	{
		_val <<= right;
		return *this;
	}

	const this_type& operator<<=(const this_type& right)
	{
		_val <<= right._val;
		return *this;
	}

	template<typename T>
	const this_type operator>>(const T& right) const
	{
		val_type val = _val >> right;
		return this_type(val);
	}

	const this_type operator>>(const this_type& right) const
	{
		val_type val = _val >> right;
		return this_type(val);
	}

	template<typename T>
	const this_type& operator>>=(const T& right)
	{
		_val >>= right;
		return *this;
	}

	const this_type& operator>>=(const this_type& right)
	{
		_val >>= right._val;
		return *this;
	}

	const this_type operator~(void) const
	{
		return this_type(~_val);
	}

	template<typename T>
	const this_type operator&(const T& right) const
	{
		val_type val(right);
		return this_type(_val & val);
	}

	const this_type operator&(const this_type& right) const
	{
		return this_type(_val & right._val);
	}

	template<typename T>
	this_type& operator&=(const T& right)
	{
		val_type val(right);
		_val &= val;
		return *this;
	}

	this_type& operator&=(const this_type& right)
	{
		_val &= right._val;
		return *this;
	}

	template<typename T>
	const this_type operator|(const T& right) const
	{
		val_type val(right);
		return this_type(_val | val);
	}

	const this_type operator|(const this_type& right) const
	{
		return this_type(_val | right._val);
	}

	template<typename T>
	this_type& operator|=(const T& right)
	{
		val_type val(right);
		_val |= val;
		return *this;
	}

	this_type& operator|=(const this_type& right)
	{
		_val |= right._val;
		return *this;
	}

private:
	val_type _val;
};

} // namespace lua_wrap
} //namespace yggr

template<typename T>
std::ostream& operator<<(std::ostream& os, const yggr::lua_wrap::wrap_base_t<T>& v)
{
	typedef T value_type;
	typedef yggr::lua_wrap::wrap_base_t<value_type> wrap_type;
	typedef typename wrap_type::val_type val_type;
	const val_type& val = v;
	os << val;
	return os;
}

template<typename T>
std::stringstream& operator<<(std::stringstream& ss, const yggr::lua_wrap::wrap_base_t<T>& v)
{
	typedef T value_type;
	typedef yggr::lua_wrap::wrap_base_t<value_type> wrap_type;
	typedef typename wrap_type::val_type val_type;
	const val_type& val = v;
	ss << val;
	return ss;
}

template<typename Left, typename Right>
const Left operator+(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	return left + r;
}

template<typename Left, typename Right>
Left& operator+=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	left += r;
	return left;
}

template<typename Left, typename Right>
const Left operator-(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	return left - r;
}

template<typename Left, typename Right>
Left& operator-=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	left -= r;
	return left;
}

template<typename Left, typename Right>
const Left operator*(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	return left * r;
}

template<typename Left, typename Right>
Left& operator*=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	left *= r;
	return left;
}

template<typename Left, typename Right>
const Left operator/(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	return left / r;
}

template<typename Left, typename Right>
Left& operator/=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	left /= r;
	return left;
}

template<typename Left, typename Right>
const Left operator%(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	typedef yggr::lua_wrap::mod<left_type> mod_type;
	//typedef yggr::lua_wrap::wrap_base_t<Right> wrap_base_t_type;

	left_type r = right;
	return mod_type()(left, r);
}

template<typename Left, typename Right>
Left& operator%=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	typedef yggr::lua_wrap::mod_equal<left_type> mod_equal_type;
	//typedef yggr::lua_wrap::wrap_base_t<Right> wrap_base_t_type;
	left_type r = right;
	mod_equal_type()(left, r);
	return left;
}

//template<typename Left, typename Right>
//Left& operator=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
//{
//	typedef Left left_type;
//	left_type r = right;
//	left = r;
//	return left;
//}

template<typename Left, typename Right>
bool operator==(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Right right_type;
	const right_type& r = right;
	return left == r;
}

template<typename Left, typename Right>
bool operator!=(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Right right_type;
	const right_type& r = right;
	return left != r;
}

template<typename Left, typename Right>
bool operator<(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Right right_type;
	const right_type& r = right;
	return left < r;
}

template<typename Left, typename Right>
bool operator<=(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Right right_type;
	const right_type& r = right;
	return left <= r;
}

template<typename Left, typename Right>
bool operator>(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Right right_type;
	const right_type& r = right;
	return left > r;
}

template<typename Left, typename Right>
bool operator>=(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Right right_type;
	const right_type& r = right;
	return left >= r;
}

template<typename Left, typename Right>
bool operator&&(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Right right_type;
	const right_type& r = right;
	return left && r;
}

template<typename Left, typename Right>
bool operator||(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Right right_type;
	const right_type& r = right;
	return left || r;
}

template<typename Left, typename Right>
Left operator&(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	return left & r;
}

template<typename Left, typename Right>
Left& operator&=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	left &= r;
	return left;
}

template<typename Left, typename Right>
Left operator|(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	return left | r;
}

template<typename Left, typename Right>
Left& operator|=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	left |= r;
	return left;
}

template<typename Left, typename Right>
Left operator<<(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	return left << r;
}

template<typename Left, typename Right>
Left& operator<<=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	left <<= r;
	return left;
}

template<typename Left, typename Right>
Left operator>>(const Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	return left >> r;
}

template<typename Left, typename Right>
Left& operator>>=(Left& left, const yggr::lua_wrap::wrap_base_t<Right>& right)
{
	typedef Left left_type;
	left_type r = right;
	left >>= r;
	return left;
}

#endif //__YGGR_LUA_WRAP_WRAP_BASE_T_HPP__
