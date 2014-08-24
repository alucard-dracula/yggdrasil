//yggr_exception_log_accesser_test.cpp

#include <iostream>
#include <boost/thread/thread.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER


#include <yggr/log/yggr_exception_log_accesser.hpp>

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

	boost::thread::id thread_id(void) const
	{
		return boost::this_thread::get_id();
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

#include <yggr/time/time.hpp>

int main(int argc, char* argv[])
{
	yggr::log::yggr_exception_log_accesser_type::init_type init;
	init.push_back("./log/ccc");
	init.push_back("./log/ddd");

	yggr::log::yggr_exception_log_accesser_type acc(init);

	yggr::log::yggr_exception_log_accesser_type::data_creator_type dc1("./log/ccc");
	yggr::log::yggr_exception_log_accesser_type::data_creator_type dc2("./log/ddd");
	acc.register_msg(100, dc1);
	acc.register_msg(101, dc2);

	A a1(100);
	A a2(101);
	A a3(103);
	acc.append(a1);
	acc.append(a2);
	acc.append(a3);

	//std::cout << yggr::time::time().to_local_string("%Y%m%d") << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
