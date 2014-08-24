//lua_hello_dll.cpp

//#include "lua_hello_dll.hpp"
#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <lua.hpp>
#include <iostream>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/lua_include.hpp>
#include <yggr/script/base_lua_environment.hpp>

#include <yggr/dll_make/dll_mark_define.hpp>

#include <yggr/dll_make/dll_make_begin.ipp>

YGGR_DLL_API int LUA_MODULE_INIT_FOO(lua_hello_dll)(lua_State* L);

#include <yggr/dll_make/dll_make_end.ipp>

//std::ostream& operator<<(std::ostream& os, const A& oa)
//{
//	os << "to_string " << oa.n;
//	return os;
//}

//std::ostream& operator<<(std::ostream& os, const yggr::u64 val)
//{
//	std::string
//	os << val;
//	return os;
//}

typedef yggr::lua_wrap::wrap_base_t<yggr::u64> lua_u64_type;
//
//std::ostream& operator<<(std::ostream& os, const lua_u64_type& val)
//{
//	os << "12345";
//	return os;
//}

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

