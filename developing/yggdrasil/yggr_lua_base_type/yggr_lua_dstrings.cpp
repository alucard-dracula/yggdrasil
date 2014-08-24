//yggr_lua_base_type.cpp

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include "yggr_lua_dstrings.hpp"
#include <luabind/luabind.hpp>
#include <yggr/lua_wrap/lua_export_wrap_vector.hpp>
#include <yggr/lua_wrap/lua_export_wrap_list.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>
#include <yggr/ppex/cast_to_string.hpp>

namespace yggr
{
namespace lua
{

void export_dstrings(lua_State* L)
{
	//luabind::module(L)
	//[
	//	luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
	//	[
	//		luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
	//		[
	//		luabind::class_<utf8_string_type>(YGGR_PP_CAST_TO_STRING(utf8_string))
	//			.def(luabind::constructor<>())
	//			.def(luabind::tostring(luabind::const_self))
	//			.def(luabind::constructor<const utf8_string_type::string_type&>())
	//			.def(luabind::constructor<const utf8_string_type&>())
	//			.def("str", (utf8_string_type::string_type (utf8_string_type::*)(void) const)&utf8_string_type::str)
	//		//	,
	//		//
	//		//EXPORT_WRAP_VECTOR_TO_LUA(utf8_string_vector_type, utf8_string_vector)
	//		//	,
	//		//EXPORT_WRAP_VECTOR_TO_LUA(string_vector_type, string_vector)
	//		//	,
	//		//EXPORT_WRAP_LIST_TO_LUA(utf8_string_list_type, utf8_string_list)
	//		//	,
	//		//EXPORT_WRAP_LIST_TO_LUA(string_list_type, string_list)

	//		]
	//	]
	//];
}

} // namespace lua
} // namespace yggr