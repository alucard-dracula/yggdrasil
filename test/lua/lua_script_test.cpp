//lua_script_test.cpp

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


typedef yggr::script::lua::lua_script script_type;
typedef yggr::script::lua::lua_environment lua_env_type;

void exec_test1_string_code()
{
	std::string code("./lua/test_call_function1.lua");

	std::string code2(
					"function hello_string()	\r\n"
					"	return 'hello2'			\r\n"
					"end						\r\n"
					"function minus(l, r)		\r\n"
					"	return l - r			\r\n"
					"end						\r\n"
				);

	script_type sp(lua_env_type::E_lib_all);

	sp.do_script(script_type::file_code_type(code));

	try
	{
		std::cout << sp.call_function<yggr::string>("hello_string") << std::endl;
		std::cout << sp.call_function<int>("plus", 10, 20) << std::endl; 
		int a = 10, b = 20;
		std::cout << sp.call_function<int>("plus", boost::cref(a), boost::cref(b)) << std::endl; 
		std::cout << sp.call_function<int>("minus", 10, 20) << std::endl; 
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 

	try
	{
		int a = 10, b = 20;
		std::cout << sp.call_function<int>("plus", boost::ref(a), boost::ref(b)) << std::endl; 
		yggr_test_assert(false);
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 

	std::cout << "-----------------------------" << std::endl;

	sp.do_script(script_type::string_code_type(code2));

	try
	{
		std::cout << sp.call_function<yggr::string>("hello_string") << std::endl;
		std::cout << sp.call_function<int>("plus", 10, 20) << std::endl; 
		std::cout << sp.call_function<int>("minus", 10, 20) << std::endl; 
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 

	std::cout << "-----------------------------" << std::endl;
	
}

void exec_test1_file_code()
{
	std::string code("./lua/test_call_function1.lua");

	std::string code2("./lua/test_call_function2.lua");

	script_type sp(lua_env_type::E_lib_all);

	sp.do_script(script_type::file_code_type(code));

	try
	{
		std::cout << sp.call_function<yggr::string>("hello_string") << std::endl;
		std::cout << sp.call_function<int>("plus", 10, 20) << std::endl; 
		int a = 10, b = 20;
		std::cout << sp.call_function<int>("plus", boost::cref(a), boost::cref(b)) << std::endl; 
		std::cout << sp.call_function<int>("minus", 10, 20) << std::endl; 
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 

	try
	{
		int a = 10, b = 20;
		std::cout << sp.call_function<int>("plus", boost::ref(a), boost::ref(b)) << std::endl; 
		yggr_test_assert(false);
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 

	std::cout << "-----------------------------" << std::endl;

	sp.do_script(script_type::file_code_type(code2));

	try
	{
		std::cout << sp.call_function<yggr::string>("hello_string") << std::endl;
		std::cout << sp.call_function<int>("plus", 10, 20) << std::endl; 
		std::cout << sp.call_function<int>("minus", 10, 20) << std::endl; 
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 

	std::cout << "-----------------------------" << std::endl;
	
}


#include <yggr/smart_ptr_ex/shared_ptr.hpp>

void exec_test2_string_code()
{
	typedef yggr::shared_ptr<script_type> script_ptr_type;

	std::string code("./lua/test_call_function1.lua");

	std::string code2(
					"test = {}					\r\n"
					"function hello_string()	\r\n"
					"	return 'hello2'			\r\n"
					"end						\r\n"
					"function test.minus(l, r)	\r\n"
					"	return l - r			\r\n"
					"end						\r\n"
				);

	script_ptr_type psp(new script_type(lua_env_type::E_lib_all));

	script_type& sp = *psp;

	sp.do_script(script_type::file_code_type(code));

	try
	{
		std::cout << sp.call_function<yggr::string>("hello_string") << std::endl;
		std::cout << sp.call_function<int>("plus", 10, 20) << std::endl; 
		std::cout << sp.call_function<int>("test.minus", 10, 20) << std::endl; 
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 

	std::cout << "-----------------------------" << std::endl;


	sp.do_script(script_type::string_code_type(code2));

	try
	{
		std::cout << sp.call_function<yggr::string>("hello_string") << std::endl;
		std::cout << sp.call_function<int>("plus", 10, 20) << std::endl; 
		std::cout << sp.call_function<int>("test.minus", 10, 20) << std::endl; 
		std::cout << sp.call_function<int>("aaa.bbb.ccc", 10, 20) << std::endl; 
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 
	
	std::cout << "-----------------------------" << std::endl;
}

void exec_test2_file_code()
{
	typedef yggr::shared_ptr<script_type> script_ptr_type;

	std::string code("./lua/test_call_function1.lua");
	std::string code2("./lua/test_call_function2.lua");

	script_ptr_type psp(new script_type(lua_env_type::E_lib_all));

	script_type& sp = *psp;

	sp.do_script(script_type::file_code_type(code));

	try
	{
		std::cout << sp.call_function<yggr::string>("hello_string") << std::endl;
		std::cout << sp.call_function<int>("plus", 10, 20) << std::endl; 
		std::cout << sp.call_function<int>("test.minus", 10, 20) << std::endl; 
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 

	std::cout << "-----------------------------" << std::endl;

	sp.do_script(script_type::file_code_type(code2));


	try
	{
		std::cout << sp.call_function<yggr::string>("hello_string") << std::endl;
		std::cout << sp.call_function<int>("plus", 10, 20) << std::endl; 
		std::cout << sp.call_function<int>("test.minus", 10, 20) << std::endl; 
		std::cout << sp.call_function<int>("aaa.bbb.ccc", 10, 20) << std::endl; 
	}
	catch(const script_type::error_type& e) 
	{ 
		std::cout << e.what() << std::endl;
	} 
	
	std::cout << "-----------------------------" << std::endl;
}


int main(int argc, char *argv[])
{
	std::cout << std::hex << script_type::error_maker_type::E_lua_code_compile_error << std::endl;
	std::cout << std::hex << script_type::error_maker_type::E_lua_code_runtime_error << std::endl;

	std::cout << std::dec;

	exec_test1_string_code();
	exec_test1_file_code();

	exec_test2_string_code();
	exec_test2_file_code();
	
	wait_any_key(argc, argv);
	return 0;
}
