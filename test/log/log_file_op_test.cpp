//log_file_op_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>
#include <sstream>

#include <yggr/base/yggrdef.h>

#include <yggr/container/list.hpp>

#include <yggr/log/log_file.hpp>
#include <yggr/log/log_thread_file.hpp>
#include <yggr/log/log_file_mgr.hpp>
#include <yggr/log/log_file_op.hpp>

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

void test_log_file_op_of_log_file(void)
{
	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_file_op<log_op_def_type, o_log_file_type> log_op_type;

	A a;

	log_op_type op("../test_out/log/log_file");

	op.append(a, true);
	op.append(a);
	op.append(a);
	op.append(a);

	op.clear();
	op.clear_any();
}

void test_log_file_op_of_log_thread_file(void)
{
	typedef yggr::log::log_op::log_thread_file<ostream_type, yggr::u64> o_log_file_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_file_op<log_op_def_type, o_log_file_type> log_op_type;

	TA<10> a10;
	TA<20> a20;

	log_op_type op("../test_out/log/log_thread_file");

	op.append(a10, true);
	op.append(a10);
	op.append(a20, true);
	op.append(a20);

	op.clear(yggr::u64(10));
	//op.clear_any(); // yggr_test_assert(false)
}

void test_log_file_op_of_log_file_mgr(void)
{
	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_file_op<log_op_def_type, log_file_mgr_type> log_op_type;


	yggr::list<std::string> init;
	init.push_back("../test_out/log/log_file_mgr1");
	init.push_back("../test_out/log/log_file_mgr2");
	init.push_back("../test_out/log/log_file_mgr3");
	init.push_back("../test_out/log/log_file_mgr4");

	A a;

	log_op_type op(init);

	for(yggr::list<std::string>::const_iterator i = init.begin(), isize = init.end();
			i != isize; ++i)
	{
		op.append(*i, a, true);
		op.append(*i, a);
		op.append(*i, a);
		op.append(*i, a);
		op.clear(*i);
	}

	op.append_any(a);

	op.clear_any();
}

int main(int argc, char* argv[])
{
	test_log_file_op_of_log_file();
	test_log_file_op_of_log_thread_file();
	test_log_file_op_of_log_file_mgr();

	std::cout << "result in test_out dir" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}
