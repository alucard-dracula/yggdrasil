//exception_test.cpp

#include<test/wait_any_key/wait_any_key.hpp>
#include <yggr/exception/exception.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_static_using(void)
{
	typedef yggr::exception::exception exception_type;

	exception_type::throw_error(1, "test_err1");
	exception_type::throw_error(2, "test_err2");

	std::cout << "---------test_static_using end -------" << std::endl;

}

void test_ptr_single_using(void)
{
	typedef yggr::exception::exception exception_type;
	typedef yggr::ptr_single<exception_type> exception_single_type;
	typedef exception_single_type::obj_ptr_type exception_ptr_type;

	exception_ptr_type pe = exception_single_type::init_ins();

	yggr_test_assert(pe);

	exception_type::throw_error(1, "test_err1");
	exception_type::throw_error(2, "test_err2");

	pe->stop();
	pe->join();

	exception_single_type::uninstall();
}


int main(int argc, char* argv[])
{
	test_static_using();
	test_ptr_single_using();

	wait_any_key(argc, argv);
	return 0;
}