//data_base_system_test.cpp

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/database_system/database_operator.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <yggr/database_system/database_mgr.hpp>

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
#       include <vld.h>
#endif // _MSC_VER

#define USE_MYSQL

typedef dtl::DBConnection conn_type;
typedef yggr::database_system::database_operator<conn_type> database_operator_type;
typedef yggr::database_system::database_mgr<database_operator_type> data_base_mgr_type;

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
} // namepsace dtl

void out_result_t_s(bool bright, const std::vector<t_s>& rows)
{
	int isize = rows.size();
	for(int i = 0; i != isize; ++i)
	{
		std::cout << rows[i] << std::endl;
	}
}

bool get_insert_rows(std::vector<t_s>& rows)
{
	t_s ts;
	ts.uid = 100;
	ts.uname = "zzz";
	ts.utm = 100;

	rows.push_back(ts);
	return true;
}

bool get_update_rows(std::vector<t_s>& rows)
{
	t_s ts;
	ts.uid = 100;
	ts.uname = "kkk";
	ts.utm = 100;

	rows.push_back(ts);
	return true;
}

bool get_delete_rows(std::vector<t_s>& rows)
{
	t_s ts;
	ts.uid = 100;
	ts.uname = "kkk";
	ts.utm = 100;

	rows.push_back(ts);
	return true;
}

//---------------------------------------------------------------

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
}

void out_result_t_s2(bool bright, const std::vector<t_s2>& rows)
{
	int isize = rows.size();
	for(int i = 0; i != isize; ++i)
	{
		std::cout << rows[i] << std::endl;
	}
}

//--------------------------------------------------------------------------
void data_base_mgr_select_test(void)
{
	data_base_mgr_type::register_select_view<t_s>("get", boost::bind(out_result_t_s, _1, _2));

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
		return;
	}

    std::cout << "select" << std::endl;
	data_base_mgr_type dbm(cstr);
	std::vector<std::string> tb_names;
	tb_names.push_back("test2");
	dbm.execute_view(yggr::database_system::database_check_operator::make_database_command_id<t_s>("get"), tb_names, std::string(""));
	tb_names.clear();
}

void data_base_mgr_insert_test(void)
{
	data_base_mgr_type::register_insert_view<t_s>("insert", boost::bind(get_insert_rows, _1));

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
		return;
	}

	data_base_mgr_type dbm(cstr);
	std::vector<std::string> tb_names;
	tb_names.push_back("test2");
	dbm.execute_view(yggr::database_system::database_check_operator::make_database_command_id<t_s>("insert"), tb_names, std::string(""));
	tb_names.clear();
}

void data_base_mgr_update_test(void)
{
	data_base_mgr_type::register_update_view<t_s>("update", boost::bind(get_update_rows, _1));

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
		return;
	}

	data_base_mgr_type dbm(cstr);
	std::vector<std::string> tb_names;
	tb_names.push_back("test2");
	dbm.execute_view(yggr::database_system::database_check_operator::make_database_command_id<t_s>("update"), tb_names, std::string("where uid = (?)"));
	tb_names.clear();
}

void data_base_mgr_delete_test(void)
{
	data_base_mgr_type::register_delete_view<t_s>("delete", boost::bind(get_delete_rows, _1));

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
		return;
	}

	data_base_mgr_type dbm(cstr);
	std::vector<std::string> tb_names;
	tb_names.push_back("test2");
	dbm.execute_view(yggr::database_system::database_check_operator::make_database_command_id<t_s>("delete"), tb_names, std::string(""));
	tb_names.clear();
}


void data_base_mgr_select_test2(void)
{
	data_base_mgr_type::register_select_view<t_s2>("get2", boost::bind(out_result_t_s2, _1, _2));

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
		return;
	}

	data_base_mgr_type dbm(cstr);
	std::vector<std::string> tb_names;
	tb_names.push_back("test2");
	tb_names.push_back("(select uname tname, max(utm) mutm from test2 group by uname) tmp");

	dbm.execute_view(yggr::database_system::database_check_operator::make_database_command_id<t_s2>("get2"),
					tb_names,
					std::string("where uname = tname and utm = mutm"));
	tb_names.clear();
}

int main(int argc, char* argv[])
{
	data_base_mgr_delete_test();
	data_base_mgr_insert_test();
	data_base_mgr_update_test();
	data_base_mgr_select_test();

	//std::cout << "--------------------------" << std::endl;
	data_base_mgr_select_test2();


	char cc = 0;
	std::cin >> cc;
	return 0;
}
