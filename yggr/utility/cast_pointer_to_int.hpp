//cast_pointer_to_int.hpp

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

#ifndef __YGGR_UTILITY_CAST_POINTER_TO_INT_HPP__
#define __YGGR_UTILITY_CAST_POINTER_TO_INT_HPP__

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/pointer_int.hpp>

namespace yggr
{
namespace utility
{

template<typename T>
struct pointer_to_int_caster
{
	typedef T result_type;
	typedef mplex::pointer_int::type org_int_type;

	inline result_type operator()(const void* const ptr) const
	{
		return result_type(reinterpret_cast<org_int_type>(ptr));
	}
};

struct cast_pointer_to_int
{
	template<typename T> inline
	static T cast(const void* const ptr)
	{
		typedef T ret_type;
		typedef mplex::pointer_int::type int_type;

		return T(reinterpret_cast<int_type>(ptr));
	}
};

} // namespace utility
} // namespace yggr

#endif // __YGGR_UTILITY_CAST_POINTER_TO_INT_HPP__

