//lua_script_test.cpp

//// if you want to test wrap version using YGGR_LUA_USE_WRAP_VALUE and compare "yggr_lua_base_wrap_type"
//
//#ifndef YGGR_LUA_USE_WRAP_VALUE
//#	define YGGR_LUA_USE_WRAP_VALUE
//#endif // YGGR_LUA_USE_WRAP_VALUE

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/stl_exception.hpp>

#include <yggr/script/lua_script.hpp>
#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>

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
#include YGGR_PP_LINK_LIB(script_lua)


typedef yggr::lua_wrap::wrap_base_t<yggr::u64> lua_u64_type;
typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

struct faile_value {};

void exec_test(void)
{
	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	std::string code("./lua/test.lua");

	std::string code2(
					"require \"yggr_lua_base_type\"\r\n"
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
					"require \"yggr_lua_base_type\"\r\n"
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

	std::string code4(
					"require \"yggr_lua_base_type\"\r\n"
					"function calculate(param)\r\n"
						"local tmp\r\n"
						"tmp.aaa.bbb = 100\r\n"
						"return param\r\n"
					"end\r\n"
				);

	bool bright = false;

	bright = mgr.insert("test", "calculate", script_mgr_type::file_code_type(code), 0);
	yggr_test_assert(bright);
	bright = mgr.insert("test2", "calculate", script_mgr_type::string_code_type(code2), 0);
	yggr_test_assert(bright);
	bright = mgr.insert("test3", "calculate", script_mgr_type::string_code_type(code3), 0);
	yggr_test_assert(!bright);
	bright = mgr.insert("test4", "calculate", script_mgr_type::string_code_type(code4), 0);
	yggr_test_assert(bright);

	yggr::u32 na = 100;
	lua_u64_type a(na);
	lua_u64_type ra;

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

	bright = mgr.execute_rule(ra, yggr::const_args_anchor("test4"), a);
	yggr_test_assert(!bright);

}

int main(int argc, char *argv[])
{
	// !!! MINGW will release the loaded DLL again after the process ends !!!
#if defined(__MINGW32__)
    HMODULE hmod = LoadLibrary("yggr_lua_base_type.dll");
    if(!hmod)
    {
        std::cerr << "load yggr_lua_base_type.dll faied" << std::endl;
        return -1;
    }
#endif // #if defined(__MINGW32__)

	exec_test();

	std::cout << "test end" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
