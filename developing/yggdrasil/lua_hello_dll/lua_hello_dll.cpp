//lua_hello_dll.cpp

#include "lua_hello_dll.hpp"
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

std::ostream& operator<<(std::ostream& os, const A& oa)
{
	os << "to_string " << oa.n;
	return os;
}

//std::ostream& operator<<(std::ostream& os, const yggr::u64 val)
//{
//	std::string
//	os << val;
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
		/*luabind::class_<yggr::u64>("int64")
			.def(luabind::constructor<>())
			.def(luabind::constructor<yggr::u64>())
		,*/

		luabind::class_<A>("A")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const A&>())
			.def(luabind::constructor<A::use_val_type>())
			.def("print", &A::print)
			.def("test_u64", (void (A::*)(yggr::s64))&A::test_u64)
			.def(luabind::const_self + A::use_val_type())
			.def(luabind::tostring(luabind::const_self))
			,

		luabind::class_<int_list_type>("int_list_type")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const int_list_type&>())
			.def("push_back", (void (int_list_type::*)(const int&))&int_list_type::push_back)
			.def("pop_back", &int_list_type::pop_back)
			.def("clear", &int_list_type::clear)
			.def("empty", &int_list_type::empty)
			//.def("size", &int_list_type::size)
			//.def("back", &int_list_type::back)
			//.def("front", &int_list_type::front)
		]

	];

	return 0;
}
