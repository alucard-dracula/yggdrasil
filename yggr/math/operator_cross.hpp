// operator_cross.hpp

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

#ifndef __YGGR_MATH_OPERATOR_CROSS_HPP__
#define __YGGR_MATH_OPERATOR_CROSS_HPP__

#include <yggr/type_traits/operator_result.hpp>
#include <boost/ref.hpp>

namespace yggr
{
namespace math
{

template<typename T>
struct crosser
{
public:
	typedef T value_type;

private:
	typedef typename ::yggr::detail::operator_result_t_getter<value_type>::type chk_type;
	typedef boost::reference_wrapper<T const> val_cref_type;

private:
	typedef crosser this_type;

public:
	crosser(const value_type& val)
		: _val_cref(val)
	{
	}

	~crosser(void)
	{
	}

public:
	operator chk_type(void) const; // fix error c2784 in operator_result

public:
	inline const value_type& get(void) const
	{
		return _val_cref.get();
	}

private:
	val_cref_type _val_cref;
};

template<typename T> inline
crosser<T> cross(const T& val)
{
	typedef crosser<T> crosser_type;
	return crosser_type(val);
}

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_OPERATOR_CROSS_HPP__
