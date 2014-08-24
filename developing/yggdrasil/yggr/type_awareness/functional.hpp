//functional.hpp

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

#ifndef __YGGR_TYPE_AWARENESS_FUNCTIONAL_HPP__
#define __YGGR_TYPE_AWARENESS_FUNCTIONAL_HPP__

#include <functional>

namespace yggr
{
namespace type_awareness
{

template<typename T>
struct negate
	: public std::unary_function<T, T>
{
	typedef T val_type;
	val_type operator()(const T& l) const
	{
		return std::negate<val_type>(l);
	}
};

template<typename T1, typename T2 = T1>
struct plus 
	: public std::binary_function<T1, T2, T1 >
{
	typedef T1 val_type;
	val_type operator()(const T1& l, const T2& r) const
	{	
		return l + r;
	}
};

template<typename T1, typename T2 = T1>
struct minus
	: public std::binary_function<T1, T2, T1 >
{
	typedef T1 val_type;
	val_type operator()(const T1& l, const T2& r) const
	{	
		return l - r;
	}
};

template<typename T1, typename T2 = T1>
struct multiplies
	: public std::binary_function<T1, T2, T1 >
{
	typedef T1 val_type;
	val_type operator()(const T1& l, const T2& r) const
	{	
		return l * r;
	}
};

template<typename T1, typename T2 = T1>
struct divides
	: public std::binary_function<T1, T2, T1 >
{
	typedef T1 val_type;
	val_type operator()(const T1& l, const T2& r) const
	{	
		return l / r;
	}
};

template<typename T>
struct modulus
	: public std::binary_function<T, T, T >
{
	typedef T val_type;
	val_type operator()(const T& l, const T& r) const
	{	
		return std::modulus<val_type>()(l, r);
	}
};

template<typename T1, typename T2 = T1>
struct less
	: public std::binary_function<T1, T2, bool>
{
	bool operator()(const T1& l, const T2& r) const
	{	
		return l < r;
	}
};

template<typename T1, typename T2 = T1>
struct less_equal 
	: public std::binary_function<T1, T2, bool>
{
	bool operator()(const T1& l, const T2& r) const
	{
		return l <= r;
	}
};

template<typename T1, typename T2 = T1>
struct greater 
	: public std::binary_function<T1, T2, bool>
{
	bool operator()(const T1& l, const T2& r) const
	{
		return l > r;
	}
};

template<typename T1, typename T2 = T1>
struct greater_equal
	: public std::binary_function<T1, T2, bool>
{
	bool operator()(const T1& l, const T2& r) const
	{
		return l >= r;
	}
};

template<typename T1, typename T2 = T1>
struct equal_to
	: public std::binary_function<T1, T2, bool>
{
	bool operator()(const T1& l, const T2& r) const
	{
		return l == r;
	}
};

template<typename T1, typename T2 = T1>
struct not_equal_to 
	: public std::binary_function<T1, T2, bool>
{
	bool operator()(const T1& l, const T2& r) const
	{
		return l != r;
	}
};

template<typename T1, typename T2 = T1>
struct logical_and 
	: public std::binary_function<T1, T2, bool>
{
	bool operator()(const T1& l, const T2& r) const
	{
		return l && r;
	}
};

template<typename T1, typename T2 = T1>
struct logical_or 
	: public std::binary_function<T1, T2, bool>
{
	bool operator()(const T1& l, const T2& r) const
	{
		return l || r;
	}
};

template<typename T>
struct logical_not 
	: public std::unary_function<T, bool>
{
	bool operator()(const T& l) const
	{
		return !l;
	}
};

} // type_awareness
} // namespace yggr


#endif // __YGGR_TYPE_AWARENESS_FUNCTIONAL_HPP__