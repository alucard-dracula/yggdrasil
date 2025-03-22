//seh_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#if defined(_MSC_VER) || defined(YGGR_AT_LINUX)

#if !defined(YGGR_USE_SEH)
#define YGGR_USE_SEH
#endif //YGGR_USE_SEH

#if defined(_MSC_VER) && defined(_DEBUG)  && !defined(YGGR_INCLUDE_SEH_DEBUG)
#define YGGR_INCLUDE_SEH_DEBUG
#endif // #if defined(_MSC_VER) && defined(_DEBUG)  && !defined(YGGR_INCLUDE_SEH_DEBUG)

#include <yggr/seh/seh.hpp>
#include <boost/bind.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(seh)
#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)

void fun3(int n)
{
    printf("The %d step begin.\n", n);
    int a[10] = {0};
    for (int i = 0; i< n; i++)
    {
        a[i] = i;
    }
    
    if (n < 20) 
    {
        fun3(n + 1);
    }
    
    printf("The %d step end\n", n);
}

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

void test_final_failed(void)
{
	fun3(8);
}


void test_seh(void)
{
	 yggr::seh::seh_type::init_ins();

#if defined(_MSC_VER)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_access_violation);
#else
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV);
#endif // _MSC_VER


    yggr::seh::seh_type::s_safe_invoke(boost::bind(&foo1));

	yggr::seh::seh_type::uninstall();
}

void test_seh_failed(void)
{
	 yggr::seh::seh_type::init_ins();

#if defined(_MSC_VER)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_access_violation);
#else
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV);
#endif // _MSC_VER

	yggr::seh::seh_type::s_safe_invoke(boost::bind(&test_final_failed));

	yggr::seh::seh_type::uninstall();
}

int main(int argc, char* argv[])
{
	//test_normal(); //crash
	test_seh(); // non crash
	//test_seh_failed(); // crash

	wait_any_key(argc, argv);
    return 0;
}

#else

int main(int argc, char* argv[])
{
	std::cout << "now plantfrom not support seh" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}

#endif // #if defined(_MSC_VER) || defined(YGGR_AT_LINUX)
