//range_size_def.hpp

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

#ifndef __YGGR_LUA_WRAP_DETAIL_RANGE_SIZE_DEF_HPP__
#define __YGGR_LUA_WRAP_DETAIL_RANGE_SIZE_DEF_HPP__

#include <yggr/ppex/foo_params.hpp>

#define YGGR_PP_LUA_WRAP_CONTAINER_RANGE_SIZE_SUPPORT( __type_count__, __type__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename _T ) > class Base > \
	typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::size_type \
		range_calculate_size(const YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& c ) { \
		return c.size(); }

namespace yggr
{
namespace lua_wrap
{
namespace detail
{
} // namespace detail
} // namesapce lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_DETAIL_RANGE_SIZE_DEF_HPP__
