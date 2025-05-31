//database_accesser_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>


#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/database_system/dtl_installer.hpp>

#include <yggr/database_system/database_connection_pool.hpp>
#include <yggr/database_system/database_accesser.hpp>
#include <yggr/database_system/database_async_mgr.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(database_system)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

#define USE_MYSQL

//#define MYSQL_ODBC_VERSION 51
//#define MYSQL_ODBC_VERSION 83
#define MYSQL_ODBC_VERSION 92

#define USE_AUTO_COMMIT() 1

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

template<typename Conn, typename Accesser>
bool clean(Conn& client, const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	typedef typename accesser_type::error_type db_error_type;
	
	yggr_test_assert(client);

	db_error_type err;

	bool bsuccess = accesser_type::s_execute_sql(client, "delete from test2", err);

	yggr_test_assert(bsuccess);
	return bsuccess;
}

template<typename Conn, typename Accesser>
bool select_test(Conn& client, const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	typedef typename accesser_type::error_type db_error_type;

	yggr_test_assert(client);

	std::vector<t_s> rows;
	db_error_type err;

	bool bsuccess = accesser_type::s_select_records_of_view(client, "test2", rows, "", err);
	yggr_test_assert(bsuccess);

	for(std::vector<t_s>::const_iterator i = rows.begin(), isize = rows.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	return bsuccess;
}

template<typename Conn, typename Accesser>
bool insert_test(Conn& client, const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	typedef typename accesser_type::error_type db_error_type;

	yggr_test_assert(client);

	std::list<t_s> rows;

	t_s ts;
	ts.uid = 13;
	ts.uname = "ggg";
	ts.utm = 13;
	
	rows.push_back(ts);

	db_error_type err;

	bool bsuccess = accesser_type::s_insert_records_of_table(client, "test2", rows, "", err);

	yggr_test_assert(bsuccess);

	return bsuccess;
}

template<typename Conn, typename Accesser>
bool insert_test_1_2(Conn& client, const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	typedef typename accesser_type::error_type db_error_type;

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

	bool bsuccess = accesser_type::s_insert_records_of_table(client, "test2", rows, "", err);
	yggr_test_assert(bsuccess);

	return bsuccess;
}

template<typename Conn, typename Accesser>
bool update_test(Conn& client, const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	typedef typename accesser_type::error_type db_error_type;

	yggr_test_assert(client);

	std::list<t_s> rows;

	t_s ts;
	ts.uid = 13;
	ts.uname = "kkk";
	ts.utm = 13;
	
	rows.push_back(ts);

	db_error_type err;
	bool bsuccess = accesser_type::s_update_records_of_table(client, "test2", rows, "where uid = (?)", err);

	yggr_test_assert(bsuccess);
	return bsuccess;
}

template<typename Conn, typename Accesser>
bool delete_test(Conn& client, const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	typedef typename accesser_type::error_type db_error_type;

	yggr_test_assert(client);

	std::list<t_s> rows;

	t_s ts;
	ts.uid = 15;
	ts.uname = "ggg";
	ts.utm = 15;
	
	rows.push_back(ts);

	db_error_type err;
	bool bsuccess = accesser_type::s_delete_records_of_table(client, "test2", rows, "", err);

	yggr_test_assert(bsuccess);
	return bsuccess;
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


template<typename Conn, typename Accesser>
bool select_test2(Conn& client, const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	typedef typename accesser_type::error_type db_error_type;

	yggr_test_assert(client);

	std::list<t_s2> rows;
	db_error_type err;

	bool bsuccess = accesser_type::s_select_records_of_view(client, 
																"(select uname tname, max(utm) mutm from test2 group by uname) tmp, test2", 
																rows,
																"where uname = tname and utm = mutm", err);

	yggr_test_assert(bsuccess);

	for(std::list<t_s2>::const_iterator i = rows.begin(), isize = rows.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	return bsuccess;
}

template<typename OutData, typename DB_Mgr>
void stop_test(bool bsuccess, OutData& out, DB_Mgr& db_mgr)
{
	db_mgr.stop();
}

void test_database_mgr_of_connection(void)
{
	typedef yggr::database_system::database_connection db_connection_type;
	typedef db_connection_type::conn_type org_conn_type;

	typedef yggr::database_system::database_accesser db_accesser_type;

	typedef db_accesser_type::error_type db_error_type;

	typedef yggr::database_system::database_async_mgr<std::string, db_connection_type, db_accesser_type> db_mgr_type;

	std::string cstr = g_cstr; 

	if(cstr.empty())
	{
		std::cerr << "dsn empty" << std::endl;
		return;
	}

	db_connection_type client(cstr);

	if(!client)
	{
		std::cerr << "failed client" << std::endl;
		return;
	}

	{
		db_error_type err;
#	if USE_AUTO_COMMIT()
		db_accesser_type::s_connection_using_handler(client, boost::bind(&org_conn_type::SetAutoCommit, _1, true), err);
#	endif // USE_AUTO_COMMIT()
	}

	db_mgr_type db_mgr(cstr);

	db_mgr.register_command("clean", 
								boost::bind(&clean<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("select_test", 
								boost::bind(&select_test<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("insert_test", 
								boost::bind(&insert_test<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("insert_test_1_2", 
								boost::bind(&insert_test_1_2<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("update_test", 
								boost::bind(&update_test<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("delete_test", 
								boost::bind(&delete_test<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("select_test2", 
								boost::bind(&select_test2<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.start(1);

	db_mgr.async_execute("clean");

	db_mgr.async_execute("insert_test");
	db_mgr.async_execute("select_test");

	db_mgr.async_execute("update_test");
	db_mgr.async_execute("select_test");

	db_mgr.async_execute("insert_test_1_2");
	db_mgr.async_execute("select_test");

	db_mgr.async_execute("select_test2");

	db_mgr.async_execute("delete_test");
	db_mgr.async_execute("select_test",
							boost::bind(&stop_test<yggr::any, db_mgr_type>, 
											_1, _2, boost::ref(db_mgr)));

	std::cout << "post end" << std::endl;

	db_mgr.join();
	db_mgr.clear();

	std::cout << "---------test_database_mgr_of_connection end--------------" << std::endl;
}

void test_database_mgr_of_connection_pool(void)
{
	typedef yggr::database_system::database_connection_pool db_connection_type;
	typedef yggr::database_system::database_accesser db_accesser_type;
	typedef yggr::database_system::database_async_mgr<std::string, db_connection_type, db_accesser_type> db_mgr_type;

	std::string cstr = g_cstr; 

	if(cstr.empty())
	{
		std::cerr << "dsn empty" << std::endl;
		return;
	}

	db_connection_type client(cstr, false, USE_AUTO_COMMIT());

	if(!client)
	{
		std::cerr << "failed client" << std::endl;
		return;
	}

	db_mgr_type db_mgr(cstr);

	db_mgr.register_command("clean", 
								boost::bind(&clean<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("select_test", 
								boost::bind(&select_test<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("insert_test", 
								boost::bind(&insert_test<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("insert_test_1_2", 
								boost::bind(&insert_test_1_2<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("update_test", 
								boost::bind(&update_test<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("delete_test", 
								boost::bind(&delete_test<db_connection_type, db_accesser_type>, _1, _2, _3));

	db_mgr.register_command("select_test2", 
								boost::bind(&select_test2<db_connection_type, db_accesser_type>, _1, _2, _3));


	db_mgr.start(1);

	db_mgr.async_execute("clean");

	db_mgr.async_execute("insert_test");
	db_mgr.async_execute("select_test");

	db_mgr.async_execute("update_test");
	db_mgr.async_execute("select_test");

	db_mgr.async_execute("insert_test_1_2");
	db_mgr.async_execute("select_test");

	db_mgr.async_execute("select_test2");

	db_mgr.async_execute("delete_test");
	db_mgr.async_execute("select_test",
							boost::bind(&stop_test<yggr::any, db_mgr_type>, 
											_1, _2, boost::ref(db_mgr)));

	std::cout << "post end" << std::endl;

	db_mgr.join();
	db_mgr.clear();

	std::cout << "---------test_database_mgr_of_connection_pool end--------------" << std::endl;
}

int main(int argc, char* argv[])
{
	yggr::database_system::dtl_installer::install();

	test_database_mgr_of_connection();
	test_database_mgr_of_connection_pool();

	yggr::database_system::dtl_installer::uninstall();
	wait_any_key(argc, argv);
	return 0;
}
