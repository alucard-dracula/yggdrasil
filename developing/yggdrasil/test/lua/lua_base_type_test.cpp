//lua_script_test.cpp


//#if defined(WIN32) && defined(_DEBUG)
//#   include <vld.h>
//#endif // _DEBUG
//
//#include <stl_exception.hpp>
//#include <yggr/base/yggrdef.h>
#include <iostream>
//#include <string>
//
//#include <yggr/script/lua_script.hpp>
//#include <yggr/script/lua_environment.hpp>
//#include <yggr/script/script_mgr.hpp>
//
//#include <yggr_lua_base_type.hpp>

//typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
//									yggr::script::lua::lua_environment> script_mgr_type;
//
//
//void test_u64(script_mgr_type& mgr)
//{
//	typedef yggr::lua::u64_type u64_type;
//	yggr::u32 na = 100;
//	u64_type a(na);
//	u64_type ra;
//
//	mgr.execute_rule("lua_base_type_test", a, ra);
//	std::cout << "C++ " << ra << std::endl;
//}
//
//void test_u64_vector(script_mgr_type& mgr)
//{
//	typedef yggr::lua::u64_vector_type u64_vector_type;
//
//	u64_vector_type vt;
//	for(int i = 0; i != 10; ++i)
//	{
//		vt.push_back((yggr::u64)i + 1);
//	}
//
//	u64_vector_type rvt;
//	mgr.execute_rule("lua_base_type_test", vt, rvt);
//	std::cout << "C++ " << rvt << std::endl;
//}
//
//void test_u32_vector(script_mgr_type& mgr)
//{
//	yggr::u32 tb(100);
//	yggr::lua::u64_type b(tb);
//	int a = 10 + b;
//
//	std::cout << a << "," << typeid(a).name() << std::endl;
//	typedef yggr::lua::u32_vector_type u32_vector_type;
//
//	u32_vector_type vt;
//	for(int i = 0; i != 10; ++i)
//	{
//		vt.push_back(i + 1);
//	}
//
//	u32_vector_type rvt;
//	mgr.execute_rule("lua_base_type_test", vt, rvt);
//	std::cout << "C++ " << rvt << std::endl;
//}
//
//void test_u64_list(script_mgr_type& mgr)
//{
//	typedef yggr::lua::u64_list_type u64_list_type;
//
//	u64_list_type vt;
//	for(int i = 0; i != 10; ++i)
//	{
//		vt.push_back((yggr::u64)i + 1);
//	}
//
//	u64_list_type rvt;
//	mgr.execute_rule("lua_base_type_test", vt, rvt);
//	std::cout << "C++ " << rvt << std::endl;
//}
//
//void exec_test()
//{
//	script_mgr_type mgr;
//
//	mgr.append_module(script_mgr_type::E_lib_all);
//
//	std::string code("./lua/lua_base_type_test.lua");
//
//    try
//	{
//		mgr.insert("lua_base_type_test", "calculate", script_mgr_type::file_code_type(code));
//    }
//    catch(const compatibility::stl_exception& e)
//    {
//        std::cerr << e.what() << std::endl;
//    }
//
//	//test_u64(mgr);
//	//test_u64_vector(mgr);
//	test_u32_vector(mgr);
//	//test_u64_list(mgr);
//}
//

#include <typeinfo>
#include <yggr/lua_wrap/wrap_vector.hpp>
#include <yggr/lua_wrap/wrap_allocator.hpp>
#include <yggr/lua_wrap/lua_export_wrap_allocator.hpp>
#include <yggr/mplex/foo_type_info.hpp>

struct A
{
	void out(void)
	{
	}

	void out(void) const
	{
	}
};

void otest(void)
{
	A a;

	yggr::mplex::member_foo_type_info_0<void, A, false>::foo_type fp = &A::out;
	yggr::mplex::member_foo_type_info_0<void, A, true>::foo_type fp2 = &A::out;
	typedef yggr::mplex::member_foo_type_info_1<void, A, int, false>::foo_type foo3_type;
	typedef yggr::mplex::member_foo_type_info_2<void, A, int, int, true>::foo_type foo4_type;
	std::cout << typeid(fp).name() << std::endl;
	std::cout << typeid(fp2).name() << std::endl;

	std::cout << typeid(foo3_type).name() << std::endl;
	std::cout << typeid(foo4_type).name() << std::endl;
	(a.*fp)();
}

int main(int argc, char *argv[])
{
	otest();
	//std::vector<int> ivt;
	//ivt.push_back(10);

	//std::cout << ivt.begin() << std::endl;
	//exec_test();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
