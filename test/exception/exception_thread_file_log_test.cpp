//exception_thread_file_log_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/log/log_of_file.hpp>
#include <yggr/exception/exception.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test(void)
{
	typedef std::ofstream ostream_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_op::log_thread_file<ostream_type, yggr::u64> log_file_type;
	typedef yggr::log::log_file_op<log_op_def_type, log_file_type> log_op_type;
	typedef yggr::log::log_file_op_caller caller_type;

	typedef yggr::log::log_accesser<int, log_op_type, caller_type> log_acc_type;


	typedef yggr::shared_ptr<log_acc_type> log_acc_ptr_type;

	typedef yggr::exception::exception exception_type;
	typedef yggr::ptr_single<exception_type> exception_single_type;
	typedef exception_single_type::obj_ptr_type exception_ptr_type;



	log_acc_ptr_type pacc(new log_acc_type("../test_out/exception/log_thread_file_exception_test"));
	yggr_test_assert(pacc);

	caller_type caller("../test_out/exception/log_thread_file_exception_test");

	pacc->register_msg(1, caller);
	pacc->register_msg(2, caller);

	exception_ptr_type pe = exception_single_type::init_ins();
	yggr_test_assert(pe);

	pe->bind_log_accesser(pacc);

	exception_type::throw_error(1, "test_err1");
	exception_type::throw_error(2, "test_err2");
	exception_type::throw_error(3, "test_err3");

	pe->stop();
	pe->join();

	exception_single_type::uninstall();
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}