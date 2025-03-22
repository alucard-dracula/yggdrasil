//log_file_mgr_accesser_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>
#include <sstream>
#include <list>

#include <yggr/base/yggrdef.h>

#include <yggr/log/log_op_def.hpp>
#include <yggr/log/log_file.hpp>
#include <yggr/log/log_file_mgr.hpp>
#include <yggr/log/log_file_op.hpp>
#include <yggr/log/log_file_mgr_op_caller.hpp>

#include <yggr/log/log_accesser.hpp>

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

std::ostream& operator<<(std::ostream& os, const A& na)
{
    os << na.to_string();
    return os;
}

void test_access_of_log_file_mgr(void)
{
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;
	typedef yggr::log::log_file_op<log_op_def_type, log_file_mgr_type> log_op_type;

	typedef yggr::log::log_file_mgr_op_caller caller_type;
	typedef yggr::log::log_accesser<int, log_op_type, caller_type> log_acc_type;

	std::list<std::string> fnames;
	fnames.push_back("../test_out/log/test_access_of_log_file_mgr1");
	fnames.push_back("../test_out/log/test_access_of_log_file_mgr2");

	log_acc_type acc(fnames);

	acc.register_msg(100, "../test_out/log/test_access_of_log_file_mgr1");
	acc.register_msg(101, "../test_out/log/test_access_of_log_file_mgr2");

	A a1(100);
	A a2(101);
	A a3(103);

	acc.append(a1.code(), a1, true);
	acc.append(a2.code(), a2, true);
	acc.append(a3.code(), a3, true);

	yggr_test_assert(!acc.search(100));
	yggr_test_assert(!acc.search_any());

	acc.clear_any();

	acc.append_any(a1, true);
	acc.append_any(a2);
	acc.append_any(a3);

	acc.clear(a1.code());

	acc.append(a1.code(), a1);
	acc.append(a2.code(), a2, true);
	acc.append(a3.code(), a3, true);
}

int main(int argc, char* argv[])
{
	test_access_of_log_file_mgr();

	std::cout << "result in test_out dir" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
