//log_sql_db_accesser_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>

#include <boost/array.hpp>
#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>

#include <yggr/database_system/database_connection.hpp>

#include <yggr/log/log_op_def.hpp>
#include <yggr/log/log_sql_db_op.hpp>
#include <yggr/log/log_sql_db_op_caller.hpp>
#include <yggr/log/log_accesser.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(database_system)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

#define USE_MYSQL

//#define MYSQL_ODBC_VERSION 51
//#define MYSQL_ODBC_VERSION 83
#define MYSQL_ODBC_VERSION 92

class t_s
{
public:

	yggr::u64 code(void) const
	{
		yggr::u64 code = uid;
		return ((code << 32) | utm);
	}

	yggr::s32 uid;
	std::string uname;
	yggr::s32 utm;
};

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const t_s& ts)
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


typedef yggr::database_system::database_connection db_connection_type;
typedef yggr::log::log_op_def log_op_def_type;
typedef yggr::log::log_op::log_sql_db_op<log_op_def_type, db_connection_type> log_sql_db_op_type;
typedef yggr::log::log_sql_db_op_caller caller_type;
typedef log_sql_db_op_type::error_type error_type;
typedef yggr::log::log_accesser<yggr::u64, log_sql_db_op_type, caller_type> log_acc_type;

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

void test_log_sql_accesser(void)
{
	std::string cstr = g_cstr;

	log_acc_type acc(cstr, false, true);

	caller_type caller("test2", true, true);

	t_s ts1;
	ts1.uid = 1;
	ts1.uname = "aaa";
	ts1.utm = 101;

	t_s ts2;
	ts2.uid = 2;
	ts2.uname = "bbb";
	ts2.utm = 102;

	t_s ts3;
	ts3.uid = 3;
	ts3.uname = "ccc";
	ts3.utm = 103;

	t_s ts4;
	ts4.uid = 4;
	ts4.uname = "ddd";
	ts4.utm = 104;

	acc.register_msg(ts1.code(), caller);
	acc.register_msg(ts2.code(), caller);
	acc.register_msg(ts3.code(), caller);

	acc.clear<log_acc_type::tag_op_caller_id>(caller.id());

	acc.append(ts1.code(), ts1);
	acc.append(ts2.code(), ts2);
	acc.append(ts3.code(), ts3);
	acc.append(ts4.code(), ts4);

	{
		std::vector<t_s> vt;
		acc.search<log_acc_type::tag_op_caller_id>(caller.id(), vt);

		yggr_test_assert(!vt.empty());

		for(std::vector<t_s>::const_iterator i = vt.begin(), isize = vt.end();
				i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(vt.size() == 3);
	}

	acc.clear<log_acc_type::tag_op_caller_id>(caller.id());
}

int main(int argc, char* argv[])
{
	test_log_sql_accesser();

	wait_any_key(argc, argv);
	return 0;
}

