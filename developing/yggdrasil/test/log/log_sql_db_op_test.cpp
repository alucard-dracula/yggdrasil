//data_base_system_test.cpp

#include <iostream>
#include <vector>
#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>

#include <yggr/database_system/database_operator.hpp>
#include <yggr/database_system/database_connect.hpp>

#include <yggr/log/log_op_def.hpp>
#include <yggr/log/log_sql_db_op.hpp>

#include <boost/tuple/tuple.hpp>

#define USE_MYSQL

#if defined(_MSC_VER)
#if defined(_DEBUG)
#	pragma comment(lib, "libdtl-vc100-d.lib")
#else
#	pragma comment(lib, "libdtl-vc100.lib")
#endif //_DEBUG
#pragma comment(lib, "odbc32.lib")
#endif // _MSC_VER

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER



class t_s
{
public:
	yggr::s32 uid;
	std::string uname;
	yggr::s32 utm;
};

std::ostream& operator<<(std::ostream& os, const t_s& ts)
{
	os << ts.uid <<","<< ts.uname<<","<<ts.utm;
	return os;
}

typedef std::vector<t_s> t_s_vt_type;

namespace dtl
{

template<>
class DefaultBCA<t_s>
{
public:
	void operator()(dtl::BoundIOs& cols, t_s& row_buf)
	{
		cols["uid"] == row_buf.uid;
		cols["uname"] == row_buf.uname;
		cols["utm"] == row_buf.utm;
	}
};

template<>
class DefaultBPA<t_s>
{
public:
	void operator()(dtl::BoundIOs& cols, t_s& row_buf)
	{
		cols[0] << row_buf.uid;
	}
};
} // namepsace dtl


typedef dtl::DBConnection real_conn_type;
typedef yggr::database_system::database_connect<real_conn_type> conn_type;
typedef boost::shared_ptr<conn_type> conn_ptr_type;
typedef yggr::database_system::database_operator<conn_type> database_operator_type;
typedef yggr::log::log_op::log_op_def<yggr::u32> log_sql_op_def_type;
typedef yggr::log::log_op::log_sql_db_op<log_sql_op_def_type, database_operator_type, t_s> log_sql_db_op_type;


void get_result(t_s_vt_type& vt)
{
	yggr::size_type isize = vt.size();

	for(yggr::size_type i = 0; i != isize; ++i)
	{
		std::cout << vt[i] << std::endl;
	}
}

int main(int argc, char* argv[])
{
#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456;OPTION=3;";
#	else
	std::string cstr;
#	endif // USE_MYSQL

	conn_ptr_type pconn(new conn_type(cstr));
	try
	{
		pconn->Connect();
		pconn->SetAutoCommit(true);
	}
	catch(const dtl::DBException& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	log_sql_db_op_type log_op(pconn);

	t_s ts1;
	ts1.uid = 1;
	ts1.uname = "aaa";
	ts1.utm = 100;
	t_s ts2;
	ts2.uid = 2;
	ts2.uname = "bbb";
	ts2.utm = 101;

	log_op.append(boost::make_tuple(std::string("test2"), ts1, std::string("")));
	log_op.append(boost::make_tuple(std::string("test2"), ts2, std::string("")));
	log_op.search(boost::make_tuple(std::string("test2"), std::string(""), boost::bind(get_result, _1)));
	log_op.clear(boost::make_tuple(std::string("test2")));
	std::cout << "end" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}

