//log_file_accesser_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>
#include <string>

#include <yggr/base/yggrdef.h>

#include <yggr/log/log_op_def.hpp>
#include <yggr/log/log_file.hpp>
#include <yggr/log/log_thread_file.hpp>
#include <yggr/log/log_file_op.hpp>
#include <yggr/log/log_file_op_caller.hpp>
#include <yggr/log/log_accesser.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(ids)
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

	A(int na)
		:a(na), f(123.5f), str("Hello")
	{
	}

	int code(void) const
	{
		return a;
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

	TA(int n)
		: A(n)
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

void test_accesser_of_log_file(void)
{
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_op::log_file<ostream_type> log_file_type;
	typedef yggr::log::log_file_op<log_op_def_type, log_file_type> log_op_type;
	typedef yggr::log::log_file_op_caller caller_type;

	typedef yggr::log::log_accesser<int, log_op_type, caller_type> log_acc_type;

	log_acc_type acc("../test_out/log/log_file_acc_test");

	caller_type caller("../test_out/log/log_file_acc_test");

	acc.register_msg(100, caller);
	acc.register_msg(101, caller);

	A a1(100);
	A a2(101);
	A a3(103);

	acc.append(a1.code(), a1, true);
	acc.append(a2.code(), a2);
	acc.append(a3.code(), a3);

	yggr_test_assert(!acc.search(100));
	yggr_test_assert(!acc.search_any());

	acc.clear_any();

	acc.append_any(a1);
	acc.append_any(a2);
	acc.append_any(a3);

	acc.clear(100); // same clear any

	acc.append(a1.code(), a1, true);
	acc.append(a2.code(), a2);
	acc.append(a3.code(), a3);
}

void test_accesser_of_log_thread_file(void)
{
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_op::log_thread_file<ostream_type, yggr::u64> log_file_type;
	typedef yggr::log::log_file_op<log_op_def_type, log_file_type> log_op_type;
	typedef yggr::log::log_file_op_caller caller_type;

	typedef yggr::log::log_accesser<int, log_op_type, caller_type> log_acc_type;

	log_acc_type acc("../test_out/log/log_thread_file_acc_test");
	caller_type caller("../test_out/log/log_thread_file_acc_test");

	/*acc.register_msg(100, "../test_out/log/log_thread_file_acc_test");
	acc.register_msg(101, "../test_out/log/log_thread_file_acc_test");*/

	acc.register_msg(100, caller);
	acc.register_msg(101, caller);

	TA<10> a1(100);
	TA<20> a2(101);
	TA<30> a3(103);

	acc.append(a1.code(), a1, true);
	acc.append(a2.code(), a2);
	acc.append(a3.code(), a3);

	yggr_test_assert(!acc.search(100));
	yggr_test_assert(!acc.search_any());
	//acc.clear_any(); //log_thread_file don't use clear_any, because not suppor and yggr_test_assert(false)

	acc.append_any(a1, true);
	acc.append_any(a2);
	acc.append_any(a3);

	//acc.clear(100); // same clear any // log_thread_file don't use clear_any, because not suppor and yggr_test_assert(false)

	acc.append(a1.code(), a1, true);
	acc.append(a2.code(), a2);
	acc.append(a3.code(), a3);

	acc.clear(100, yggr::u64(10));

	// result:
	//	a1 = 0 // last call acc.clear(100, yggr::u64(10));
	//	a2 = 3 // a2 is not has clear operator
	//	a3 = 1 // acc.append_any(a3) can append a3
}

int main(int argc, char* argv[])
{
	test_accesser_of_log_file();
	test_accesser_of_log_thread_file();

	std::cout << "result in test_out dir" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}
