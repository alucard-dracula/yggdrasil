//log_file_test.cpp

#include <iostream>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include <yggr/base/yggrdef.h>

#include <yggr/log/log_file.hpp>
#include <yggr/log/log_file_mgr.hpp>
#include <yggr/log/log_file_op.hpp>
#include <fstream>
#include <string>
#include <sstream>

#include <yggr/log/log_op_def.hpp>

#include <boost/tuple/tuple.hpp>

typedef std::ofstream ostream_type;

typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;
typedef yggr::log::log_op::log_op_def<yggr::u32> log_op_def_type;
typedef yggr::log::log_op::log_file_op<log_op_def_type, log_file_mgr_type> log_op_type;
typedef log_op_type::init_type init_type;


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

std::ostream& operator<<(std::ostream& os, const A& na)
{
	os << na.to_string();
	return os;
}


int main(int argc, char* argv[])
{
	init_type init;
	init.push_back("lf_test1.txt");
	init.push_back("lf_test2.txt");
	init.push_back("lf_test3.txt");
	init.push_back("lf_test4.txt");
	A a;

	log_op_type op(init);

	op.append(boost::make_tuple(init.front(), a));
	op.append(boost::make_tuple(init.back(), a));

	op.clear(boost::make_tuple(init.front()));
	op.clear(boost::make_tuple(init.back()));




	std::cout << "end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
