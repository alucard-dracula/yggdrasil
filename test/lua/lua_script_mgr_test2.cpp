//lua_script_test.cpp

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/script/lua_script.hpp>
//#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/wrap_vector.hpp>
#include <yggr/lua_wrap/wrap_list.hpp>

#include <yggr/lua_wrap/lua_export_wrap_vector.hpp>
#include <yggr/lua_wrap/lua_export_wrap_list.hpp>

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


typedef yggr::lua_wrap::wrap_base_t<yggr::u64> lua_u64_type;
typedef yggr::lua_wrap::wrap_base_t<yggr::s64> lua_s64_type;

typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

//typedef yggr::lua_wrap::wrap_vector<yggr::s32> s32_vector_type;

//void export_d32_s32_vector(lua_State* L)
//{
//	yggr_test_assert(L);
//	if(!L)
//	{
//		return;
//	}
//
//	luabind::module(L)
//	[
//		luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
//		[
//			luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
//			[
//				EXPORT_WRAP_VECTOR_TO_LUA(s32_vector_type, s32_vector)
//			]
//		]
//	];
//}

void export_d64_s64(lua_State* L)
{
	yggr_test_assert(L);
	if(!L)
	{
		return;
	}

	luabind::module(L)
	[
		luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
		[
			luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
			[

			luabind::class_<lua_s64_type>(YGGR_PP_CAST_TO_STRING(s64))
				.def(luabind::constructor<>())
				.def(luabind::constructor<const lua_s64_type::half_type&>())
				.def(luabind::constructor<const std::string&>())
				.def(luabind::constructor<const lua_s64_type::half_type&, const lua_s64_type::half_type&>())
				.def(luabind::constructor<const lua_s64_type&>())
				.def(luabind::tostring(luabind::const_self))
				.def(-luabind::self)
				.def(luabind::const_self + luabind::const_self)
				.def(luabind::const_self - luabind::const_self)
				.def(luabind::const_self * luabind::const_self)
				.def(luabind::const_self / luabind::const_self)
				.def(luabind::const_self <= luabind::const_self)
				.def(luabind::const_self < luabind::const_self)
				.def(luabind::const_self == luabind::const_self)
				.def(luabind::other<double>() + luabind::const_self)
				.def(luabind::other<double>() - luabind::const_self)
				.def(luabind::other<double>() * luabind::const_self)
				.def(luabind::other<double>() / luabind::const_self)
				.def(luabind::other<double>() <= luabind::const_self)
				.def(luabind::other<double>() < luabind::const_self)
				.def(luabind::const_self + luabind::other<double>())
				.def(luabind::const_self - luabind::other<double>())
				.def(luabind::const_self * luabind::other<double>())
				.def(luabind::const_self / luabind::other<double>())
				.def(luabind::const_self <= luabind::other<double>())
				.def(luabind::const_self < luabind::other<double>())
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

	luabind::module(L)
	[
		luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
		[
			luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
			[
			
			luabind::class_<lua_u64_type>(YGGR_PP_CAST_TO_STRING(u64))
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
				.def(luabind::other<double>() + luabind::const_self)
				.def(luabind::other<double>() - luabind::const_self)
				.def(luabind::other<double>() * luabind::const_self)
				.def(luabind::other<double>() / luabind::const_self)
				.def(luabind::other<double>() <= luabind::const_self)
				.def(luabind::other<double>() < luabind::const_self)
				.def(luabind::const_self + luabind::other<double>())
				.def(luabind::const_self - luabind::other<double>())
				.def(luabind::const_self * luabind::other<double>())
				.def(luabind::const_self / luabind::other<double>())
				.def(luabind::const_self <= luabind::other<double>())
				.def(luabind::const_self < luabind::other<double>())
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

struct faile_value {};

void exec_test()
{
	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	std::string code("./lua/test_non_require.lua");

	std::string code2(
					//"require \"yggr_lua_base_type\"\r\n" // inner_register don't use require
					"function calculate(param)\r\n"
						//"return param\r\n"
						"print(param)\r\n"
						"local b = param + 100\r\n"
						"param = param + 200\r\n"
						"print(param, b)\r\n"
						"local c = yggr.lua.u64(3000)\r\n"
						"print(c)\r\n"
						"local d = yggr.lua.u64(\"12345678901\")\r\n"
						"print(d)\r\n"
						"return b\r\n"
						//"return param\r\n"
					"end\r\n"
				);


	std::string code3(
					"function calculate(param)\r\n"
						"print(param\r\n"				// this line less ')' caused a compile error
						"local b = param + 100\r\n"
						"param = param + 200\r\n"
						"print(param, b)\r\n"
						"local c = yggr.lua.u64(3000)\r\n"
						"print(c)\r\n"
						"local d = yggr.lua.u64(\"12345678901\")\r\n"
						"print(d)\r\n"
						"return b\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::file_code_type(code), 0, 
							boost::bind(&export_all, _1));
	yggr_test_assert(bright);

	bright = mgr.insert("test2","calculate", script_mgr_type::string_code_type(code2), 0,
							boost::bind(&export_all, _1));
	yggr_test_assert(bright);
	bright = mgr.insert("test3","calculate", script_mgr_type::string_code_type(code3), 0,
							boost::bind(&export_all, _1));
	yggr_test_assert(!bright);

	yggr::u32 na = 100;
	lua_u64_type a(na);

	lua_u64_type ra;

#if 1
	// i don't know why the error happend of using rvalue
	// error C2440: 'static_cast' : cannot convert from 'overloaded-function' to 'foo_type'
	// vs2010 is not support foo pointer e.g. foo(rvref, rvref, rvref ...)
	// so using "const type&" fix it
	bright = mgr.execute_rule(ra, yggr::const_args_anchor("test"), lua_u64_type(100));
	yggr_test_assert(bright);
#endif // 0, 1
	
	bright = mgr.execute_rule(yggr::const_args_anchor("test"), lua_u64_type(100)); // test non_ret version
	yggr_test_assert(bright);

	bright = mgr.execute_rule(ra, yggr::const_args_anchor("test"), a);
	yggr_test_assert(bright);
	std::cout << ra << std::endl;

	bright = mgr.execute_rule(ra, yggr::const_args_anchor("test2"), a);
	yggr_test_assert(bright);
	std::cout << ra << std::endl;

	typedef faile_value faile_value_type;
	faile_value_type faile_val;
	bright = mgr.execute_rule(ra, yggr::const_args_anchor("test2"), faile_val);
	yggr_test_assert(!bright);
}

int main(int argc, char *argv[])
{
	exec_test();
	
	wait_any_key(argc, argv);
	return 0;
}
