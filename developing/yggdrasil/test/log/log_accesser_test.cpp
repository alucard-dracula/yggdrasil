//log_accesser_test.cpp

#include <iostream>

#include <yggr/base/yggrdef.h>
#include <vector>

#include <yggr/log/log_file.hpp>
#include <yggr/log/log_file_mgr.hpp>
#include <yggr/log/log_file_op.hpp>

#include <yggr/log/log_accesser.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <yggr/log/log_op_def.hpp>

#include <boost/tuple/tuple.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef std::ofstream ostream_type;

typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;
typedef yggr::log::log_op::log_op_def<yggr::u32> log_op_def_type;
typedef yggr::log::log_op::log_file_op<log_op_def_type, log_file_mgr_type> log_op_type;
typedef log_op_type::init_type init_type;


class data_creater
{
public:
	typedef std::vector<std::string> init_type;
public:
	data_creater(void)
	{
	}

	data_creater(const std::string& fname)
		: _fname(fname)
	{
	}

	data_creater(const data_creater& right)
		: _fname(right._fname)
	{
	}

	~data_creater(void)
	{
	}

	data_creater& operator=(const data_creater& right)
	{
		_fname = right._fname;
		return *this;
	}

	bool operator==(const data_creater& right) const
	{
		return _fname == right._fname;
	}

	template<typename Value>
	boost::tuple<std::string, Value> create_param(const Value& val) const
	{
		return boost::make_tuple(_fname, val);
	}

private:
	std::string _fname;

};

typedef yggr::log::log_accesser<int, log_op_type, data_creater> log_acc_type;

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


int main(int argc, char* argv[])
{
	log_acc_type::init_type init;
	init.push_back("aaa.txt");
	init.push_back("bbb.txt");
	log_acc_type acc(init);

	data_creater dc1("aaa.txt");
	data_creater dc2("bbb.txt");
	acc.register_msg(100, dc1);
	acc.register_msg(101, dc2);

	A a1(100);
	A a2(101);
	A a3(103);
	acc.append(a1);
	acc.append(a2);
	acc.append(a3);

	char cc = 0;
	std::cin >> cc;
	return 0;
}
