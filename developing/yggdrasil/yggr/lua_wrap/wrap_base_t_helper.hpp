//wrap_base_t_helper.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_BASE_T_HELPER_HPP__
#define __YGGR_LUA_WRAP_WRAP_BASE_T_HELPER_HPP__

#include <yggr/lua_wrap/wrap_base_t.hpp>

namespace yggr
{
namespace lua_wrap
{

template<typename T>
class wrap_base_t_helper
{
public:
	typedef T val_type;
	typedef val_type wrap_val_type;
};

template<>
class wrap_base_t_helper<u64>
{
public:
	typedef u64 val_type;
	typedef wrap_base_t<val_type> wrap_val_type;
};

template<>
class wrap_base_t_helper<s64>
{
public:
	typedef s64 val_type;
	typedef wrap_base_t<val_type> wrap_val_type;
};

} // namespace lua_wrap
} // namespace yggr

#endif //__YGGR_LUA_WRAP_WRAP_BASE_T_HELPER_HPP__