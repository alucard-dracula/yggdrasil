//log_file_test.cpp

#include <iostream>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include <yggr/log/log_file.hpp>
#include <yggr/log/log_file_mgr.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/serialization/access.hpp>

typedef std::ofstream ostream_type;

typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;
typedef log_file_mgr_type::init_type init_type;


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

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & a;
		ar & f;
		ar & str;
	}

	int a;
	float f;
	std::string str;

};

std::ostream& operator<<(std::ostream& os, const A& a)
{
	os << a.to_string();
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

	log_file_mgr_type lfm(init);

	init_type::iterator isize = init.end();

	for(init_type::iterator i = init.begin(); i != isize; ++i)
	{
		lfm.append(*i, a);
		lfm.append(*i, a);
		lfm.append(*i, a);
		lfm.append(*i, a);
		//lfm.clear((*i));
	}

	lfm.append_all(a);
	lfm.clear_all();


	std::cout << "end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
