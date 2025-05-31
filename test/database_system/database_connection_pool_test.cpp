//database_connection_pool_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/database_system/dtl_installer.hpp>

#include <yggr/database_system/database_connection_pool.hpp>
#include <yggr/database_system/database_accesser.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(base)

#define USE_MYSQL

//#define MYSQL_ODBC_VERSION 51
//#define MYSQL_ODBC_VERSION 83
#define MYSQL_ODBC_VERSION 92

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

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, const t_s& ts)
{
	os << ts.uid << ","<< ts.uname<< ","<<ts.utm;
	return os;
}

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

} // namespace dtl

typedef yggr::database_system::database_connection_pool db_connection_type;
typedef db_connection_type::conn_type org_conn_type;
typedef yggr::database_system::database_accesser db_accesser_type;
typedef db_accesser_type::error_type db_error_type;

#if defined(USE_MSSQL)
	std::string g_cstr = "DRIVER={SQL Server};Server=.;Database=Game;Uid=sa;Pwd=123456abc;";
#elif defined(USE_MYSQL)
#	if MYSQL_ODBC_VERSION == 51
#        if defined(YGGR_AT_WINDOWS)
            std::string g_cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456abc;OPTION=3;";
#        elif defined(YGGR_AT_LINUX)
             std::string g_cstr = "DRIVER={MySQL-5};SERVER=localhost;DATABASE=Game;UID=root;PASSWORD=123456abc;OPTION=3;";
#        else
#		    error "add connect string"
#        endif // YGGR_AT_WINDOWS
#	elif MYSQL_ODBC_VERSION == 83
#        if defined(YGGR_AT_WINDOWS)
#		    if defined(YGGR_SYSTEM_64)
                std::string g_cstr = "Driver={MySQL ODBC 8.3 ANSI Driver};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#		    else
                std::string g_cstr = "Driver={MySQL ODBC 8.0 ANSI Driver};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#		    endif // YGGR_SYSTEM_64
#       elif defined(YGGR_AT_LINUX)
			// can modify Driver name in /etc/odbcinst.ini, see man odbcinst
            std::string g_cstr = "Driver={MySQL};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#       elif defined(YGGR_AT_DARWIN)
			// can modify Driver name in /Users/[user_name]/Library/ODBC/odbcinst.ini, see man odbcinst
            std::string g_cstr = "Driver={MySQL};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#       else
#           error "add connect string"
#       endif // YGGR_AT_WINDOWS
#	elif MYSQL_ODBC_VERSION == 92
#        if defined(YGGR_AT_WINDOWS)
#		    if defined(YGGR_SYSTEM_64)
                std::string g_cstr = "Driver={MySQL ODBC 9.2 ANSI Driver};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#		    else
                std::string g_cstr = "Driver={MySQL ODBC 8.0 ANSI Driver};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#		    endif // YGGR_SYSTEM_64
#       elif defined(YGGR_AT_LINUX)
			// can modify Driver name in /etc/odbcinst.ini, see man odbcinst
            std::string g_cstr = "Driver={MySQL};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#       elif defined(YGGR_AT_DARWIN)
			// can modify Driver name in /Users/[user_name]/Library/ODBC/odbcinst.ini, see man odbcinst
            std::string g_cstr = "Driver={MySQL};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#       else
#           error "add connect string"
#       endif // YGGR_AT_WINDOWS
#	else
#		error "add connect string"
#	endif // MYSQL_ODBC_VERSION
#else
	std::string g_cstr;
#endif // WIN32

void clean(db_connection_type& client)
{
	yggr_test_assert(client);

	db_error_type err;

	bool bsuccess = db_accesser_type::s_execute_sql(client, "delete from test2", err);

	yggr_test_assert(bsuccess);
}

void select_test(db_connection_type& client)
{
	yggr_test_assert(client);

	std::vector<t_s> rows;
	db_error_type err;

	bool bsuccess = db_accesser_type::s_select_records_of_view(client, "test2", rows, "", err);
	yggr_test_assert(bsuccess);

	for(std::vector<t_s>::const_iterator i = rows.begin(), isize = rows.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}
}

void insert_test(db_connection_type& client)
{
	yggr_test_assert(client);

	std::list<t_s> rows;

	t_s ts;
	ts.uid = 13;
	ts.uname = "ggg";
	ts.utm = 13;
	
	rows.push_back(ts);

	db_error_type err;

	bool bsuccess = db_accesser_type::s_insert_records_of_table(client, "test2", rows, "", err);


	yggr_test_assert(bsuccess);
}

void insert_test_1_2(db_connection_type& client)
{
	yggr_test_assert(client);

	t_s ts;
	ts.uid = 15;
	ts.uname = "ggg";
	ts.utm = 15;
	std::vector<t_s> rows;
	rows.push_back(ts);

	ts.uid = 15;
	ts.uname = "ggg";
	ts.utm = 16;
	rows.push_back(ts);

	db_error_type err;

	bool bsuccess = db_accesser_type::s_insert_records_of_table(client, "test2", rows, "", err);
	yggr_test_assert(bsuccess);
}

void update_test(db_connection_type& client)
{
	yggr_test_assert(client);

	std::list<t_s> rows;

	t_s ts;
	ts.uid = 13;
	ts.uname = "kkk";
	ts.utm = 13;
	
	rows.push_back(ts);

	db_error_type err;
	bool bsuccess = db_accesser_type::s_update_records_of_table(client, "test2", rows, "where uid = (?)", err);

	yggr_test_assert(bsuccess);
}

void delete_test(db_connection_type& client)
{
	yggr_test_assert(client);

	std::list<t_s> rows;

	t_s ts;
	ts.uid = 15;
	ts.uname = "ggg";
	ts.utm = 15;
	
	rows.push_back(ts);

	db_error_type err;
	bool bsuccess = db_accesser_type::s_delete_records_of_table(client, "test2", rows, "", err);

	yggr_test_assert(bsuccess);
}

class t_s2
{
public:

	yggr::s32 uid;
	std::string uname;
	yggr::s32 utm;
};

std::ostream& operator<<(std::ostream& os, const t_s2& ts)
{
	os << ts.uid << ","<< ts.uname << "," << ts.utm;
	return os;
}

namespace dtl
{
template<>
class DefaultBCA<t_s2>
{
public:
	void operator()(dtl::BoundIOs& cols, t_s2& row_buf)
	{
		cols["uid"] == row_buf.uid;
		cols["uname"] == row_buf.uname;
		cols["utm"] == row_buf.utm;
	}
};
} //namespace dtl

void select_test2(db_connection_type& client)
{
	yggr_test_assert(client);

	std::list<t_s2> rows;
	db_error_type err;

	bool bsuccess = db_accesser_type::s_select_records_of_view(client, 
																"(select uname tname, max(utm) mutm from test2 group by uname) tmp, test2", 
																rows,
																"where uname = tname and utm = mutm", err);

	yggr_test_assert(bsuccess);

	for(std::list<t_s2>::const_iterator i = rows.begin(), isize = rows.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}
}

int main(int argc, char* argv[])
{
	yggr::database_system::dtl_installer::install();

	std::string cstr = g_cstr; 

	if(cstr.empty())
	{
		std::cerr << "dsn empty" << std::endl;
		return -1;
	}

	db_connection_type client(cstr, false, true);

	if(!client)
	{
		std::cerr << "failed client" << std::endl;
		return -1;
	}

	clean(client);

	insert_test(client);
	select_test(client);

	std::cout << "-----------" << std::endl;

	update_test(client);
	select_test(client);

	std::cout << "-----------" << std::endl;

	insert_test_1_2(client);
	select_test(client);

	std::cout << "-----------" << std::endl;

	select_test2(client);

	std::cout << "-----------" << std::endl;

	delete_test(client);
	select_test(client);

	yggr::database_system::dtl_installer::uninstall();
	wait_any_key(argc, argv);
	return 0;
}
