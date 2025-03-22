#include <cstdio>
#include <cstdlib>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/ppex/cast_to_string.hpp>
#include <yggr/any_val/any.hpp>

#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/operator.hpp>
#include <luabind/scope.hpp>

//#include <lua.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

// Use _TOOL_IGNORE to make the conversion tool ignore

#include YGGR_PP_LINK_LIB(script_lua)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_s)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_ss)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_ss_ts)


luabind::scope lua2bind_any_type()
{
	typedef yggr::any any_type;

	luabind::class_<any_type> my_class("any_type");
	my_class
		.def( luabind::constructor<>() )
		.def( luabind::constructor<yggr::s32>() )
		.def( luabind::constructor<const std::string>() )

		.def( "swap", static_cast<void(any_type::*)(any_type&)>(&any_type::swap) )

		.def( "empty", &any_type::empty )
		.def( "type", &any_type::type ) //lnk err
		;

	return my_class;
}

void lua_export_caller(lua_State * L)
{
	luabind::open(L);

	luabind::module(L)
	[
		luabind::namespace_(YGGR_PP_CAST_TO_STRING(flpx))
		[
			luabind::namespace_(YGGR_PP_CAST_TO_STRING(battle))
			[
				lua2bind_any_type()
			]
		]
	];
}

int main(int argc, char *argv[])
{
	//lua_State* L= lua_open();
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);
	lua_export_caller(L);

	lua_close(L);

	wait_any_key(argc, argv);

    return EXIT_SUCCESS;
}
