//log_file_mgr_op_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>
#include <sstream>

#include <yggr/base/yggrdef.h>

#include <yggr/container/list.hpp>

#include <yggr/log/log_op_def.hpp>
#include <yggr/log/log_file.hpp>
#include <yggr/log/log_file_mgr.hpp>
#include <yggr/log/log_file_op.hpp>

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

void test_log_file_mgr_op_1(void)
{
	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_file_op<log_op_def_type, log_file_mgr_type> log_op_type;

	yggr::list<std::string> init;
	init.push_back("../test_out/log/log_file_mgr11");
	init.push_back("../test_out/log/log_file_mgr12");
	init.push_back("../test_out/log/log_file_mgr13");
	init.push_back("../test_out/log/log_file_mgr14");

	A a;

	log_op_type op(init);

	for(yggr::list<std::string>::const_iterator i = init.begin(), isize = init.end();
			i != isize; ++i)
	{

		op.append((*i), a, true);
		op.append((*i), a);
		op.append((*i), a);
		op.append((*i), a);

		op.clear(*i);

		op.clear_any();
	}

	op.append_any(a);
	op.clear_any();
}

void test_log_file_mgr_op_2(void)
{
	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_file_op<log_op_def_type, log_file_mgr_type> log_op_type;

	yggr::list<std::string> init;
	init.push_back("../test_out/log/log_file_mgr21");
	init.push_back("../test_out/log/log_file_mgr22");
	init.push_back("../test_out/log/log_file_mgr23");
	init.push_back("../test_out/log/log_file_mgr24");

	A a;

	log_op_type op(init);

	for(yggr::list<std::string>::const_iterator i = init.begin(), isize = init.end();
			i != isize; ++i)
	{
		op.append((*i), a, true);
		op.append((*i), a);
		op.append((*i), a);
		op.append((*i), a);
	}

	op.append_any(a, true);
	op.append_any(a);
}

int main(int argc, char* argv[])
{
	test_log_file_mgr_op_1();
	test_log_file_mgr_op_2();

	std::cout << "result in test_out dir" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}
