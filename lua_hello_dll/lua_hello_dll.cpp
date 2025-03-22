//lua_hello_dll.cpp

//#include "lua_hello_dll.hpp"
#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/script/lua_base_environment.hpp>
#include <iostream>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/lua_include.hpp>

#include <yggr/compile_link/linker.hpp>

#if defined(_MSC_VER)
#   include <vld.h>
#endif // defined(_MSC_VER) 

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(script_lua)

#include <yggr/dll_make/dll_mark_define.hpp>

#include <yggr/dll_make/dll_make_begin.ipp>

YGGR_DLL_API int LUA_MODULE_INIT_FOO(lua_hello_dll)(lua_State* L);

#include <yggr/dll_make/dll_make_end.ipp>

typedef yggr::lua_wrap::wrap_base_t<yggr::u64> lua_u64_type;

int LUA_MODULE_INIT_FOO(lua_hello_dll)(lua_State* L)
{
	std::cout << "luaopen_lua_hello_dll" << std::endl;
	luabind::open(L);

	luabind::module(L)
	[
		luabind::namespace_("test")
		[
		luabind::class_<lua_u64_type>("uint64")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const lua_u64_type::half_type&>())
			.def(luabind::constructor<const std::string&>())
			.def(luabind::constructor<const lua_u64_type::half_type&, const lua_u64_type::half_type&>())
			.def(luabind::constructor<const lua_u64_type&>())
			.def(luabind::tostring(luabind::const_self))
			.def(-luabind::self)
			.def(luabind::const_self + luabind::const_self)
			.def(luabind::const_self - luabind::const_self)
			.def(luabind::const_self * luabind::const_self)
			.def(luabind::const_self / luabind::const_self)
			.def(luabind::const_self <= luabind::const_self)
			.def(luabind::const_self < luabind::const_self)
			.def(luabind::const_self == luabind::const_self)
			.def(luabind::other<yggr::u32>() + luabind::const_self)
			.def(luabind::other<yggr::u32>() - luabind::const_self)
			.def(luabind::other<yggr::u32>() * luabind::const_self)
			.def(luabind::other<yggr::u32>() / luabind::const_self)
			.def(luabind::other<yggr::u32>() <= luabind::const_self)
			.def(luabind::other<yggr::u32>() < luabind::const_self)
			.def(luabind::const_self + luabind::other<yggr::u32>())
			.def(luabind::const_self - luabind::other<yggr::u32>())
			.def(luabind::const_self * luabind::other<yggr::u32>())
			.def(luabind::const_self / luabind::other<yggr::u32>())
			.def(luabind::const_self <= luabind::other<yggr::u32>())
			.def(luabind::const_self < luabind::other<yggr::u32>())
		]
	];

	return 0;
}

