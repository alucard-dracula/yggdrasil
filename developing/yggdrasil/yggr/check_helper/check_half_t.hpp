//check_half_t.hpp

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

#ifndef __YGGR_CHECK_HELPER_CHECK_HALF_T_HPP__
#define __YGGR_CHECK_HELPER_CHECK_HALF_T_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace check_helper
{

template<typename T>
class half_t;

#define HALF_T_DEF(__type__, __half_type__) \
	template<> class half_t<__type__> { public: typedef __half_type__ half_type;};

HALF_T_DEF(u64, u32)
HALF_T_DEF(s64, u32)
HALF_T_DEF(u32, u16)
HALF_T_DEF(s32, u16)
HALF_T_DEF(u16, u8)
HALF_T_DEF(s16, u8)
HALF_T_DEF(u8, u8)
HALF_T_DEF(s8, u8)

template<typename T, 
			typename Half_T = half_t<T> >
class check_half_t
{
private:
	typedef T now_type;
	typedef Half_T half_t_type;
public:
	typedef typename half_t_type::half_type half_type;
	enum
	{
		half_bit_size = (sizeof(now_type) * 4),
		E_compile_u32 = 0xffffffff
	};
};

} // namespace check_helper
} // namespace yggr

#endif //__YGGR_CHECK_HELPER_CHECK_HALF_T_HPP__