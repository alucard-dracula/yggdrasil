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

//#include <boost/thread.hpp>

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


//typedef yggr::lua_wrap::wrap_base_t<yggr::u64> lua_u64_type;
//typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
//									yggr::script::lua::lua_environment> script_mgr_type;

typedef yggr::script::lua::lua_script script_type;
typedef yggr::script::lua::lua_environment lua_env_type;

void exec_test(void)
{
#if defined(__MINGW32__)
    HMODULE hmod = LoadLibrary("yggr_lua_base_type.dll");
    if(!hmod)
    {
        std::cerr << "load yggr_lua_base_type.dll faied" << std::endl;
        return;
    }

#endif // #if defined(__MINGW32__)

    script_type sp(lua_env_type::E_lib_all);

    std::string code("./lua/test.lua");
	sp.do_script(script_type::file_code_type(code));

	try
	{
		std::cout << sp.call_function<int>("calculate", 100) << std::endl;
	}
	catch(const script_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

}

int main(int argc, char *argv[])
{
	exec_test();

	std::cout << "test end" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
