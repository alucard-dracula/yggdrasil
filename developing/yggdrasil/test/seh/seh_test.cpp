//seh_test.cpp

#define YGGR_USE_SEH

#ifdef _MSC_VER
#define YGGR_INCLUDE_SEH_DEBUG
#endif // _MSC_VER
#include <iostream>

#include <yggr/seh/seh.hpp>
#include <boost/bind.hpp>

void foo2(void)
{
	for(int i = 0, isize = 10; i != isize; ++i)
	{
		std::cout << i << std::endl;

		if(i == 5)
		{
			int* p = 0;
			*p = 100;
		}
	}
}

void foo1(void)
{
    foo2();
}

void test_normal(void)
{
	foo1();
}

void test_seh(void)
{
	 yggr::seh::seh_type::s_init_ins();
	
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_access_violation);
#elif defined(LINUX)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV);
#else
#	error "please add this plant code.!!!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    yggr::seh::seh_type::s_safe_invoke(boost::bind(&foo1));

	yggr::seh::seh_type::s_uninstall();
}

int main(int argc, char* argv[])
{
	//test_normal(); //crash
	test_seh(); // non crash

    char cc = 0;
    std::cin >> cc;
   
    return 0;
}
