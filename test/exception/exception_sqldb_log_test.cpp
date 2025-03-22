//exception_sqldb_log_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/database_system/database_connection.hpp>
#include <yggr/log/log_of_sql_db.hpp>
#include <yggr/exception/exception.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(base)

#define USE_MYSQL

//#define MYSQL_ODBC_VERSION 51
//#define MYSQL_ODBC_VERSION 83
#define MYSQL_ODBC_VERSION 92

//bool create_table()
//{
//	db_error_type err;
//
//	std::string sql =
//		"CREATE TABLE test2"
//		"("
//			"uid int(4) not null,"
//			"uname varchar(255) not null,"
//			"utm int(4) not null,"
//			"primary key(uid, utm)"
//		")";
//
//	{
//		tester_type test;
//		bool bsuccess = db_accesser_type::s_execute_sql(client, sql, err);
//
//		std::cout << err.what() << std::endl;
//		yggr_test_assert(bsuccess);
//	}
//}

void test(void)
{
	typedef yggr::database_system::database_connection db_connection_type;
	typedef yggr::log::log_op_def log_op_def_type;
	typedef yggr::log::log_op::log_sql_db_op<log_op_def_type, db_connection_type> log_sql_db_op_type;
	typedef yggr::log::log_sql_db_op_caller caller_type;
	typedef log_sql_db_op_type::error_type error_type;
	typedef yggr::log::log_accesser<yggr::u32, log_sql_db_op_type, caller_type> log_acc_type;
	typedef yggr::shared_ptr<log_acc_type> log_acc_ptr_type;

	typedef yggr::exception::exception exception_type;
	typedef yggr::ptr_single<exception_type> exception_single_type;
	typedef exception_single_type::obj_ptr_type exception_ptr_type;

	typedef log_sql_db_op_type::real_op_type db_acc_type;


	/*
	create database log_test;

	create table log_data (
		id int(8) unsigned auto_increment, 
		code int(4) unsigned not null, 
		time int(8) unsigned not null, 
		thread_id int(8) unsigned not null, 
		message varchar(255), 
		primary key (id) );
	*/


#if defined(USE_MSSQL)
	std::string cstr = "DRIVER={SQL Server};Server=.;Database=log_test;Uid=sa;Pwd=123456abc;";
#elif defined(USE_MYSQL)
#	if MYSQL_ODBC_VERSION == 51
#        if defined(YGGR_AT_WINDOWS)
            std::string cstr = "DRIVER={MySQL ODBC 5.1 Driver};SERVER=localhost;DATABASE=log_test;UID=root;PASSWORD=123456abc;OPTION=3;";
#        elif defined(YGGR_AT_LINUX)
             std::string cstr = "DRIVER={MySQL-5};SERVER=localhost;DATABASE=log_test;UID=root;PASSWORD=123456abc;OPTION=3;";
#        else
#		    error "add connect string"
#        endif // YGGR_AT_WINDOWS
#	elif MYSQL_ODBC_VERSION == 83
#        if defined(YGGR_AT_WINDOWS)
#		    if defined(YGGR_SYSTEM_64)
                std::string cstr = "Driver={MySQL ODBC 8.3 ANSI Driver};Server=localhost;Database=log_test;User=root;Password=123456abc;Option=3;";
#		    else
                std::string cstr = "Driver={MySQL ODBC 8.0 ANSI Driver};Server=localhost;Database=log_test;User=root;Password=123456abc;Option=3;";
#		    endif // YGGR_SYSTEM_64
#       elif defined(YGGR_AT_LINUX)
			// can modify Driver name in /etc/odbcinst.ini, see man odbcinst
            std::string cstr = "Driver={MySQL};Server=localhost;Database=log_test;User=root;Password=123456abc;Option=3;";
#       elif defined(YGGR_AT_DARWIN)
			// can modify Driver name in /Users/[user_name]/Library/ODBC/odbcinst.ini, see man odbcinst
            std::string cstr = "Driver={MySQL};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#       else
#           error "add connect string"
#       endif // YGGR_AT_WINDOWS
#	elif MYSQL_ODBC_VERSION == 92
#        if defined(YGGR_AT_WINDOWS)
#		    if defined(YGGR_SYSTEM_64)
                std::string cstr = "Driver={MySQL ODBC 9.2 ANSI Driver};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#		    else
                std::string cstr = "Driver={MySQL ODBC 8.0 ANSI Driver};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#		    endif // YGGR_SYSTEM_64
#       elif defined(YGGR_AT_LINUX)
			// can modify Driver name in /etc/odbcinst.ini, see man odbcinst
            std::string cstr = "Driver={MySQL};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#       elif defined(YGGR_AT_DARWIN)
			// can modify Driver name in /Users/[user_name]/Library/ODBC/odbcinst.ini, see man odbcinst
            std::string cstr = "Driver={MySQL};Server=localhost;Database=Game;User=root;Password=123456abc;Option=3;";
#       else
#           error "add connect string"
#       endif // YGGR_AT_WINDOWS
#	else
#		error "add connect string"
#	endif // MYSQL_ODBC_VERSION
#else
	std::string cstr;
#endif // WIN32

	log_acc_ptr_type pacc(new log_acc_type(cstr, false, true));
	yggr_test_assert(pacc);

	caller_type caller("log_data", true, true);

	pacc->register_msg(1, caller);
	pacc->register_msg(2, caller);

	exception_ptr_type pe = exception_single_type::init_ins();

	yggr_test_assert(pe);

	pe->bind_log_accesser(pacc);

	exception_type::throw_error(1, "test_err1");
	exception_type::throw_error(2, "test_err2");
	exception_type::throw_error(3, "test_err3");

	pe->stop();
	pe->join();

	exception_single_type::uninstall();
}


int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}