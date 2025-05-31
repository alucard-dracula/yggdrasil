//database_connection_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/database_system/dtl_installer.hpp>

#include <yggr/database_system/database_connection_pool.hpp>
#include <yggr/database_system/database_accesser.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/this_thread.hpp>

#include <boost/bind.hpp>
#include <boost/atomic.hpp>

#include <typeinfo>

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

#define USE_AUTO_COMMIT() 0

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
typedef boost::atomic<bool> run_state_type;

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

run_state_type g_is_run(false);

void clean(db_connection_type& client)
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;
	yggr_test_assert(client);

	for(;g_is_run.load();)
	{
		db_error_type err;
		bool bsuccess = db_accesser_type::s_execute_sql(client, "delete from test2", err);

		if(bsuccess)
		{
			std::cout << "cleaned" << std::endl;
		}

		this_thread_type::yield();
		this_thread_type::sleep(thread_config_type::time_type(2, 0));
	}
}

void select_test(db_connection_type& client)
{

	yggr_test_assert(client);

	for(;g_is_run.load();)
	{
		std::vector<t_s> rows;
		db_error_type err;

		bool bsuccess = db_accesser_type::s_select_records_of_view(client, "test2", rows, "", err);

		if(bsuccess)
		{
			std::cout << "selected" << std::endl;
			for(std::vector<t_s>::const_iterator i = rows.begin(), isize = rows.end(); i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
	}
}

void insert_test(db_connection_type& client, int count)
{
	yggr_test_assert(client);

	int num = 0;

	for(;g_is_run.load() && num < count; ++num)
	{
		std::list<t_s> rows;

		t_s ts;
		ts.uid = num;
		ts.uname = "ggg";
		ts.utm = num;
	
		rows.push_back(ts);

		db_error_type err;

		bool bsuccess = db_accesser_type::s_insert_records_of_table(client, "test2", rows, "", err);

		if(bsuccess)
		{
			std::cout << "inserted" << std::endl;
		}
	}

	g_is_run = false;

	std::cout << "insert run end" << std::endl;
}

int main(int argc, char* argv[])
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;

	yggr::database_system::dtl_installer::install();

	thread_config_type::thread_group_type trd_group;

	std::cout << typeid(thread_config_type::id_type).name() << std::endl;

	thread_config_type::id_type id;

	std::string cstr = g_cstr; 

	if(cstr.empty())
	{
		std::cerr << "dsn empty" << std::endl;
		return -1;
	}

	db_connection_type client(cstr);

	if(!client)
	{
		std::cerr << "failed client" << std::endl;
		return -1;
	}

	{
		db_error_type err;
#	if USE_AUTO_COMMIT()
		db_accesser_type::s_connection_using_handler(client, boost::bind(&org_conn_type::SetAutoCommit, _1, true), err);
#	endif // USE_AUTO_COMMIT()
	}

	clean(client);
	g_is_run = true;

	trd_group.create_thread(boost::bind(&clean, boost::ref(client)));
	trd_group.create_thread(boost::bind(&select_test, boost::ref(client)));
	trd_group.create_thread(boost::bind(&insert_test, boost::ref(client), 100000));

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}

	g_is_run = false;
	trd_group.join_all();

	yggr::database_system::dtl_installer::uninstall();
	wait_any_key(argc, argv);
	return 0;
}
