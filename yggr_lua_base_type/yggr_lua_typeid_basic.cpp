// yggr_lua_typeid_basic.cpp

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

#include "yggr_lua_typeid_basic.hpp"

#include <yggr/lua_wrap/lua_export_typeid.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace lua
{

void export_typeid_basic(lua_State* L)
{
	luabind::module(L)
	[
		luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
		[
			luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
			[
				lua_wrap::export_typeid<bool>(),
				lua_wrap::export_typeid<double>(),
				lua_wrap::export_typeid<std::string>()
			]
		]
	];
}

} // namespace lua
} // namespace yggr
