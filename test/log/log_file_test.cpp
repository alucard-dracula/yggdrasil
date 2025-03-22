//log_file_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>
#include <string>
#include <sstream>

#include <yggr/log/log_file.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef std::ofstream ostream_type;

typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;

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

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, const A& na)
{
	os << na.to_string();
	return os;
}

void test1(void)
{
	A a;
	o_log_file_type olf("../test_out/log/lf_test1.txt");
	olf.append(a);
	olf.append(a, true);
	olf.append(a);

	olf.clear();
}

void test2(void)
{
	A a;
	o_log_file_type olf("../test_out/log/lf_test2.txt");
	olf.append(a);
	olf.append(a, true);
	olf.append(a);
	olf.append(10);

	//olf.clear(); //test2 non clean
}

int main(int argc, char* argv[])
{
	test1();
	test2();

	std::cout << "result in ../test_out/log dir" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}
