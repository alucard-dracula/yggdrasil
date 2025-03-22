//log_thread_file_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>
#include <string>
#include <sstream>

#include <yggr/log/log_thread_file.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef std::ofstream ostream_type;

typedef yggr::log::log_op::log_thread_file<ostream_type, yggr::u64> o_log_file_type;

struct A
{
	A(yggr::u64 ntid)
		:tid(ntid), a(100), f(123.5f), str("Hello")
	{
	}

	const std::string to_string(void) const
	{
		std::stringstream ss;
		ss << a << "," << f << "," << str;
		return ss.str();
	}

	yggr::u64 thread_id(void) const
	{
		return tid;
	}

	yggr::u64 tid;
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


int main(int argc, char* argv[])
{
	o_log_file_type olf("../test_out/log/ltf_test.txt");
	olf.append(A(10));
	
	olf.clear(10);
	
	olf.append(A(20));
	olf.append(A(20));
	olf.append(A(20), true);
	olf.append(A(20));

	std::cout << "result in ../test_out/log dir" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}
