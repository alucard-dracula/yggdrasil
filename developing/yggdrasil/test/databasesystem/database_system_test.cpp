//data_base_system_test.cpp

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/database_system/database_operator.hpp>
#include <boost/bind.hpp>
#include <vector>

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
#   include<vld.h>
#endif // _MSC_VER

class t_s
{
public:

	static void updata_bpa(dtl::BoundIOs& cols, t_s& row_buf)
	{
		cols[0] << row_buf.uid;
	}

	static void update_bca(dtl::BoundIOs& cols, t_s& row_buf)
	{
		cols["uname"] << row_buf.uname;
		cols["utm"] << row_buf.utm;
	}

	yggr::s32 uid;
	std::string uname;
	yggr::s32 utm;
};

std::ostream& operator<<(std::ostream& os, const t_s& ts)
{
	os << ts.uid <<","<< ts.uname<<","<<ts.utm;
	return os;
}

namespace dtl
{
template<>
class DefaultBCA<t_s>
{
public:
	void operator()(dtl::BoundIOs& cols, t_s& row_buf){
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
} // namespace dtl


typedef yggr::database_system::database_operator<dtl::DBConnection> database_operator_type;

void select_test(void)
{

#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456;OPTION=3;";
#	else
	std::string cstr;
#	endif // WIN32

	if(cstr.empty())
	{
		std::cerr << "dns empty" << std::endl;
	}

	try
	{
		dtl::DBConnection::GetDefaultConnection().Connect(cstr);
		dtl::DBConnection::GetDefaultConnection().SetAutoCommit(true);
	}
	catch(const dtl::DBException& e)
	{
		std::cerr << e.what();
		return;
	}

	std::vector<t_s> rows;
	database_operator_type::select_records_of_view("test2", rows);
	for(int i = 0; i < rows.size(); ++i)
	{
		std::cout << rows[i] << std::endl;
	}
}


void insert_test(void)
{
#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456;OPTION=3;";
#	else
	std::string cstr;
#	endif // WIN32

	if(cstr.empty())
	{
		std::cerr << "dns empty" << std::endl;
	}

	try
	{
		dtl::DBConnection::GetDefaultConnection().Connect(cstr);
		dtl::DBConnection::GetDefaultConnection().SetAutoCommit(true);
	}
	catch(const dtl::DBException& e)
	{
		std::cerr << e.what();
		return;
	}

	t_s ts;
	ts.uid = 13;
	ts.uname = "ggg";
	ts.utm = 13;
	std::vector<t_s> ins_rows;
	ins_rows.push_back(ts);

	database_operator_type::insert_records_of_table("test2", ins_rows);
}

void insert_test_1_2(void)
{
#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456;OPTION=3;";
#	else
	std::string cstr;
#	endif // WIN32

	if(cstr.empty())
	{
		std::cerr << "dns empty" << std::endl;
	}

	try
	{
		dtl::DBConnection::GetDefaultConnection().Connect(cstr);
		dtl::DBConnection::GetDefaultConnection().SetAutoCommit(true);
	}
	catch(const dtl::DBException& e)
	{
		std::cerr << e.what();
		return;
	}

	t_s ts;
	ts.uid = 13;
	ts.uname = "ggg";
	ts.utm = 13;
	std::vector<t_s> ins_rows;
	ins_rows.push_back(ts);

	ts.uid = 16;
	ts.uname = "kkk";
	ts.utm = 16;
	ins_rows.push_back(ts);

	database_operator_type::insert_records_of_table("test2", ins_rows);
}

void update_test(void)
{

#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456;OPTION=3;";
#	else
	std::string cstr;
#	endif // WIN32

	if(cstr.empty())
	{
		std::cerr << "dns empty" << std::endl;
	}

	try
	{
		dtl::DBConnection::GetDefaultConnection().Connect(cstr);
		dtl::DBConnection::GetDefaultConnection().SetAutoCommit(true);
	}
	catch(const dtl::DBException& e)
	{
		std::cerr << e.what();
		return;
	}

	t_s ts;
	ts.uid = 13;
	ts.uname = "kkk";
	ts.utm = 13;
	std::vector<t_s> ins_rows;
	ins_rows.push_back(ts);

	database_operator_type::update_records_of_table("test2", ins_rows, "where uid = (?)");
}

void delete_test(void)
{

#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456;OPTION=3;";
#	else
	std::string cstr;
#	endif // WIN32

	if(cstr.empty())
	{
		std::cerr << "dns empty" << std::endl;
	}

	try
	{
		dtl::DBConnection::GetDefaultConnection().Connect(cstr);
		dtl::DBConnection::GetDefaultConnection().SetAutoCommit(true);
	}
	catch(const dtl::DBException& e)
	{
		std::cerr << e.what();
		return;
	}

	t_s ts;
	ts.uid = 13;
	ts.uname = "kkk";
	ts.utm = 13;
	std::vector<t_s> ins_rows;
	ins_rows.push_back(ts);

	database_operator_type::delete_records_of_table("test2", ins_rows);
}

class t_s2
{
public:

	yggr::s32 uid;
	std::string uname;
	//yggr::s32 utm;
};

std::ostream& operator<<(std::ostream& os, const t_s2& ts)
{
	os << ts.uid <<","<< ts.uname;
	return os;
}

namespace dtl
{
template<>
class DefaultBCA<t_s2>
{
public:
	void operator()(dtl::BoundIOs& cols, t_s2& row_buf){
		cols["uid"] == row_buf.uid;
		cols["uname"] == row_buf.uname;

	}
};
} //namespace dtl

void select_test2(void)
{

#	if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456;";
#	elif defined(USE_MYSQL)
	std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456;OPTION=3;";
#	else
	std::string cstr;
#	endif // WIN32

	if(cstr.empty())
	{
		std::cerr << "dns empty" << std::endl;
	}

	try
	{
		dtl::DBConnection::GetDefaultConnection().Connect(cstr);
		dtl::DBConnection::GetDefaultConnection().SetAutoCommit(true);
	}
	catch(const dtl::DBException& e)
	{
		std::cerr << e.what();
		return;
	}

	std::vector<t_s2> rows;
	database_operator_type::select_records_of_view("(select uname tname, max(utm) mutm from test2 group by uname) tmp,test2", rows,
																	"where uname = tname and utm = mutm");
	for(int i = 0; i < rows.size(); ++i)
	{
		std::cout << rows[i] << std::endl;
	}
}

int main(int argc, char* argv[])
{
	delete_test();
	insert_test();
	update_test();
	insert_test_1_2();
	select_test();

//-----------------------------------------------------------------------
	select_test2();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
