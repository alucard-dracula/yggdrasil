// lua_script_mgr_test_same_id_and_out.cpp

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>
#include <string>

#include <yggr/script/lua_script.hpp>
#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>

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
typedef yggr::script::script_mgr<yggr::u32, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

void exec_test()
{
	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	std::string code1(
					"function calculate(arg)\r\n"
						"print(arg + 100)\r\n"
					"end\r\n"
				);

	std::string code2(
					"function calculate(arg)\r\n"
						"print(arg + 101)\r\n"
						"return arg + 101\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert(1, "calculate", script_mgr_type::string_code_type(code1), 0);
	yggr_test_assert(bright);
	bright = mgr.insert(2, "calculate", script_mgr_type::string_code_type(code2), 0);
	yggr_test_assert(bright);

	{
		bright = mgr.execute_rule(yggr::const_args_anchor(1), 1);
		yggr_test_assert(bright);

		yggr::u32 ra = 0;
		bright = mgr.execute_rule(ra, yggr::const_args_anchor(2), 1);
		yggr_test_assert(bright);
		std::cout << ra << std::endl;
	}

	{
		yggr::u32 arg = 1;

		bright = mgr.execute_rule(yggr::const_args_anchor(1), boost::cref(arg));
		yggr_test_assert(bright);

		yggr::u32 ra = 0;
		bright = mgr.execute_rule(ra, yggr::const_args_anchor(2), boost::cref(arg));
		yggr_test_assert(bright);
		std::cout << ra << std::endl;
	}
}


int main(int argc, char *argv[])
{
	exec_test();

	wait_any_key(argc, argv);
	return 0;
}
