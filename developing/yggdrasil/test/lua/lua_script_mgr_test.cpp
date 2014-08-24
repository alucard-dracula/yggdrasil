//lua_script_test.cpp


#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

#include <stl_exception.hpp>
#include <yggr/base/yggrdef.h>
#include <iostream>
#include <string>

#include <yggr/script/lua_script.hpp>
#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>


typedef yggr::lua_wrap::wrap_base_t<yggr::u64> lua_u64_type;
typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

void exec_test()
{
	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	std::string code("./test.lua");

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

    try
	{
		//mgr.insert("test", "calculate", script_mgr_type::file_code_type(code));
		mgr.insert("test2","calculate", script_mgr_type::string_code_type(code2));
    }
    catch(const compatibility::stl_exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

	yggr::u32 na = 100;
	lua_u64_type a(na);

	lua_u64_type ra;
	//mgr.execute_rule("test", a, ra);
	//std::cout << ra.n << std::endl;

	mgr.execute_rule("test2", a, ra);
	std::cout << ra << std::endl;

}

int main(int argc, char *argv[])
{
	exec_test();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
