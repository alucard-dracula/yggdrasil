//lua_script_test.cpp

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/script/lua_script.hpp>
//#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr/lua_wrap/lua_export_typeid.hpp>
#include <yggr/lua_wrap/lua_export_wrap_base_t.hpp>
#include <yggr/lua_wrap/lua_export_wrap_pair.hpp>

#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

//#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
//#include <luabind/lua_include.hpp>

#include <string>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)

// Use _TOOL_IGNORE to make the conversion tool ignore

//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_s)
#include YGGR_PP_LINK_LIB(script_lua_ss)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_ss_ts)


typedef yggr::lua_wrap::wrap_base_t<yggr::s64> wrap_s64_type;
typedef yggr::lua_wrap::wrap_base_t<yggr::u64> wrap_u64_type;


typedef yggr::lua_wrap::wrap_pair<yggr::s64, bool> pair_s64_bool_type;
typedef yggr::lua_wrap::wrap_pair<yggr::u64, bool> pair_u64_bool_type;

typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;


void export_d64_s64(lua_State* L)
{
	yggr_test_assert(L);
	if(!L)
	{
		return;
	}

	luabind::class_<wrap_s64_type> s64_cobj("s64");
	luabind::class_<pair_s64_bool_type> s64_bool_pair_cobj("pair_s64_bool_type");

	yggr::lua_wrap::export_wrap_base_t(s64_cobj);

	yggr::lua_wrap::export_wrap_pair(s64_bool_pair_cobj);

	luabind::module(L)
	[
		luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
		[
			luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
			[
				s64_cobj,
				s64_bool_pair_cobj
			]
		]
	];
}

void export_d64_u64(lua_State* L)
{
	yggr_test_assert(L);
	if(!L)
	{
		return;
	}

	luabind::class_<wrap_u64_type> u64_cobj("u64");
	luabind::class_<pair_u64_bool_type> u64_bool_pair_cobj("pair_u64_bool_type");

	yggr::lua_wrap::export_wrap_base_t(u64_cobj);

	yggr::lua_wrap::export_wrap_pair(u64_bool_pair_cobj);

	luabind::module(L)
	[
		luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
		[
			luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
			[
				u64_cobj,
				u64_bool_pair_cobj
			]
		]
	];
}

void export_all(lua_State* L)
{
	yggr_test_assert(L);
	if(!L)
	{
		return;
	}
	
#ifdef _DEBUG
	std::cout << "luaopen_yggr_lua_base_type" << std::endl;
#endif //_DEBUG

	luabind::open(L);

	export_d64_s64(L);
	export_d64_u64(L);
}

void exec_test()
{
	typedef yggr::time::runtime_performance_tester<yggr::time::time> tester_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	pair_u64_bool_type a(10, false);
	pair_u64_bool_type a1(10, false);
	pair_u64_bool_type ra;

	std::string code(
					"function calculate(param)\r\n"
						"param:set_first(param:get_first() + 100)\r\n"
						"param:set_second(not param:get_second())\r\n"
						"return param\r\n"
					"end\r\n"
				);

	bool bright = false;
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0,
							boost::bind(&export_all, _1));
	yggr_test_assert(bright);

	const int test_count = 1000;
	a1 = a;
	{
		
		tester_type tester(test_count);
		for(int i = 0, isize = test_count; i != isize; ++i)
		{
			bright = mgr.execute_rule(ra, yggr::const_args_anchor("test"), boost::ref(a1));
		}
	}

}

int main(int argc, char *argv[])
{
	exec_test();

	wait_any_key(argc, argv);
	return 0;
}
