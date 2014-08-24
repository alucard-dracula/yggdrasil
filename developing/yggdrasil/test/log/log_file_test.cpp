//log_file_test.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/text_archive_partner.hpp>
#include <yggr/log/log_file.hpp>

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

std::ostream& operator<<(std::ostream& os, const A& na)
{
	os << na.to_string();
	return os;
}


int main(int argc, char* argv[])
{
	A a;
	o_log_file_type olf("lf_test.txt");
	olf.append(a);
	olf.append(a);

	olf.clear();

	std::cout << "end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
