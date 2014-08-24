//lua_export_wrap_pair.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_PAIR_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_PAIR_HPP__

#include < yggr/lua_wrap/wrap_pair.hpp >
#include < yggr/ppex/cast_to_string.hpp >

#define EXPORT_WRAP_PAIR_TO_LUA( __cpp_pair_type__, __lua_pair_type__ ) \
	luabind::class_< __cpp_pair_type__ >(  YGGR_PP_CAST_TO_STRING( __lua_pair_type__ )  ) \
		.def( luabind::constructor<  >(  ) ) \
		.def( luabind::constructor< const __cpp_pair_type__::first_type&, const __cpp_pair_type__::second_type& >(  ) ) \
		.def( luabind::constructor< const __cpp_pair_type__& >(  ) ) \
		.def( luabind::tostring( luabind::const_self ) ) \
		.def( "get_first", &__cpp_pair_type__::get_first ) \
		.def( "set_first", &__cpp_pair_type__::set_first ) \
		.def( "get_second", &__cpp_pair_type__::get_second ) \
		.def( "set_second", &__cpp_pair_type__::set_second )

namespace yggr
{
namespace lua_wrap
{
} // namespace lua_wrap
} // namespace yggr

#endif //__YGGR_LUA_WRAP_LUA_EXPORT_WRAP_PAIR_HPP__