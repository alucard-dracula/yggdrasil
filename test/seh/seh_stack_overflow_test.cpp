//seh_stack_overflow_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#if defined(_MSC_VER)

#pragma warning (disable : 4717) //disable stack overflow warning

#define YGGR_WARNING_NOT_TO_ERROR

#if !defined(YGGR_USE_SEH)
#define YGGR_USE_SEH
#endif //YGGR_USE_SEH

#if defined(_MSC_VER) && defined(_DEBUG)  && !defined(YGGR_INCLUDE_SEH_DEBUG)
#define YGGR_INCLUDE_SEH_DEBUG
#endif // #if defined(_MSC_VER) && defined(_DEBUG)  && !defined(YGGR_INCLUDE_SEH_DEBUG)

#include <yggr/seh/seh.hpp>
//#include <yggr/thread/boost_thread.hpp>

#include <boost/bind.hpp>

#include <cstdlib>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(seh)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)

void test_normal(void);

struct A
{
	A(void) { ++n; };
	~A() { --n; };

	static unsigned int n;

	char a[1000];

private:
	A(const A &);
};

unsigned int A::n = 0;


void foo1(void)
{
	A a;
	test_normal();
}

void test_normal(void)
{
	A a;
	foo1();
}

void test_seh(void)
{
	yggr::seh::seh_type::init_ins();
	
#if defined(YGGR_AT_WINDOWS)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_access_violation);
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_stack_overflow);
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_cpp_exception);
	//yggr::seh::seh_type::s_register_code(STATUS_NO_MEMORY);
#elif defined(YGGR_AT_LINUX)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV);
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGILL);
#else
#	error "please add this plant code.!!!"
#endif // YGGR_AT_WINDOWS

	bool bright = yggr::seh::seh_type::s_safe_invoke(boost::bind(&foo1));
	yggr_test_assert(!bright);

	std::cout << "A::n is " << (A::n) << std::endl;

#if defined(_MSC_VER)
	yggr_test_assert((A::n == 0));
#endif // _MSC_VER

	yggr::seh::seh_type::uninstall();
}

int main(int argc, char* argv[])
{
#if 0
	test_normal(); // crash
#else
	test_seh();
#endif // 0, 1

	wait_any_key(argc, argv);
	return 0;
}

#else

int main(int argc, char* argv[])
{
	std::cout << "now plantfrom not support seh stack overflow" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}

#endif // #if defined(_MSC_VER) || defined(YGGR_AT_LINUX)
