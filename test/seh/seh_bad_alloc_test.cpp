//seh_bad_alloc_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#if defined(_MSC_VER) || defined(YGGR_AT_LINUX)

#if !defined(YGGR_USE_SEH)
#define YGGR_USE_SEH
#endif //YGGR_USE_SEH

#if defined(_MSC_VER) && defined(_DEBUG) && !defined(YGGR_INCLUDE_SEH_DEBUG)
#define YGGR_INCLUDE_SEH_DEBUG
#endif // #if defined(_MSC_VER) && defined(_DEBUG)  && !defined(YGGR_INCLUDE_SEH_DEBUG)


#include <yggr/seh/seh.hpp>
#include <yggr/compile_link/linker.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#include <cstdlib>

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

#define TEST_MEM_LEAK 0

#if defined(YGGR_AT_WINDOWS)
void foo2(void)
{
	unsigned long long* p = new unsigned long long[0x0ffffff0];

	// this code not runed
	yggr_test_assert(false);
	yggr_test_assert(p);
	delete[] p;
}

#else

void foo2(void)
{
 
#define ARR_SIZE 1000
   
    unsigned long long* parr[ARR_SIZE] = {0};
    
    for(std::size_t i = 0, isize = ARR_SIZE; i != isize; ++i)
    {
        parr[i] = new unsigned long long[0x0fffffff];
    }
    
    yggr_test_assert(false);
    for(std::size_t i = 0, isize = ARR_SIZE; i != isize; ++i)
    {
        memset(parr[i], i % 256, 0x0fffffff);
    }
    
    for(std::size_t i = 0, isize = ARR_SIZE; i != isize; ++i)
    {
        delete[] parr[i];
    }
    
}

#endif // #if defined(YGGR_AT_WINDOWS)

void foo1(void)
{
	foo2();
}

void test_normal(void)
{
	foo1();
}

template<typename T>
typename boost::enable_if_c<(sizeof(T*) == 4), void>::type
	test_seh(void)
{
	//yggr_test_assert(sizeof(void*) == 4); // must using 32 system test
#if !(defined(__GNUC__) && defined(__linux__))// Linux GCC will inevitably detect this ASSERT
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(void*) == 4)>));
#endif // _MSC_VER

	yggr::seh::seh_type::init_ins();
	
#if defined(YGGR_AT_WINDOWS)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_access_violation);
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_stack_overflow);
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_cpp_exception);
#elif defined(YGGR_AT_LINUX)
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV); // older linux maybe use this
	yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGABRT);
#else
#	error "please add this plant code.!!!"
#endif // defined(YGGR_AT_WINDOWS)

	bool bright = yggr::seh::seh_type::s_safe_invoke(boost::bind(&foo1));
	yggr_test_assert(!bright);

	//vld will misreport the memory leak, please see windows Task Manager
#if TEST_MEM_LEAK
	for(int i = 0; i != 10000; ++i)
	{
		yggr::seh::seh_type::s_safe_invoke(boost::bind(&foo1));
	}
#endif // TEST_MEM_LEAK

	yggr::seh::seh_type::uninstall();
}

template<typename T>
typename boost::disable_if_c<(sizeof(T*) == 4), void>::type
	test_seh(void)
{
	std::cout << "!!!this test must using 32bit system!!!" << std::endl;
	return;
}

int main(int argc, char* argv[])
{
	test_seh<void>();

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
