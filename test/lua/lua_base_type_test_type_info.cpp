// lua_base_type_test_type_info.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/string.hpp>

#include <yggr/script/lua_script.hpp>
#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr_lua_base_type.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(script_lua)
#include YGGR_PP_LINK_LIB(system_controller)

typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

void test_typeinfo_basic(script_mgr_type& mgr)
{
	typedef yggr::lua::wrap_u64_type wrap_u64_type;
	typedef yggr::lua::wrap_s64_type wrap_s64_type;
	yggr::u32 na = 100;
	yggr::u64 a = na;
	yggr::u64 ra;

	bool bright = mgr.execute_rule(ra, yggr::const_args_anchor("lua_base_type_test_typeinfo"), a);
	yggr_test_assert(bright);
	std::cout << "C++ [" << ra << ", " << a << "]" << std::endl;
}

void exec_test()
{
	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code_test_typeinfo("./lua/lua_base_type_test_typeinfo.lua");

    try
	{
		mgr.insert("lua_base_type_test_typeinfo", "calculate", script_mgr_type::file_code_type(code_test_typeinfo), 0);

    }
    catch(const yggr::compatibility::stl_exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

	test_typeinfo_basic(mgr);

	std::cout << "all test end wait free..." << std::endl;
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

	wait_any_key(argc, argv);
	return 0;
}
