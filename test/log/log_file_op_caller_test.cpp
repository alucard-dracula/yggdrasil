//log_file_op_caller_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>
#include <sstream>

#include <yggr/base/yggrdef.h>

#include <yggr/container/list.hpp>

#include <yggr/log/log_file.hpp>
#include <yggr/log/log_thread_file.hpp>
#include <yggr/log/log_file_op.hpp>
#include <yggr/log/log_file_op_caller.hpp>

#include <yggr/log/log_op_def.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef std::ofstream ostream_type;

struct A
{
	A(void)
		:a(100), f(123.5f), str("Hello")
	{
	}

	const std::string to_string(void) const
	{
		std::stringstream ss;
		ss << a << "," << f << "," << str;
		return ss.str();
	}

	int a;
	float f;
	std::string str;

};

template<yggr::u64 tid>
struct TA
	: public A
{
	TA(void)
	{
	}

	yggr::u64 thread_id(void) const
	{
		return tid;
	}
};


std::ostream& operator<<(std::ostream& os, const A& na)
{
    os << na.to_string();
	return os;
}

void test_log_file_op_caller_of_log_file(void)
{
	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_file_op<log_op_def_type, o_log_file_type> log_op_type;
	typedef yggr::log::log_file_op_caller caller_type;

	A a;

	caller_type caller("../test_out/log/log_file_caller");
	log_op_type op("../test_out/log/log_file_caller");

	caller.call_clear(op);
	caller.call_append(op, a, true);
	caller.call_append(op, a);
	caller.call_append(op, a);
	caller.call_append(op, a);
	
	caller.call_clear_any(op);
}

void test_log_file_op_of_caller_log_thread_file(void)
{
	typedef yggr::log::log_op::log_thread_file<ostream_type, yggr::u64> o_log_file_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_file_op<log_op_def_type, o_log_file_type> log_op_type;
	typedef yggr::log::log_file_op_caller caller_type;

	TA<10> a10;
	TA<20> a20;

	caller_type caller("../test_out/log/log_thread_file_caller");
	log_op_type op("../test_out/log/log_thread_file_caller");

	caller.call_append(op, a10, true);
	caller.call_append(op, a10);
	caller.call_append(op, a20, true);
	caller.call_append(op, a20);

	caller.call_clear(op, yggr::u64(10));
}

int main(int argc, char* argv[])
{
	test_log_file_op_caller_of_log_file();
	test_log_file_op_of_caller_log_thread_file();

	std::cout << "result in test_out dir" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}
