// file_sys_nsql_conn_inclue_check

#include <yggr/file_system/local_fsys_operators.hpp> // must after c_mongo_connection.hpp in win32 release


#include <yggr/nsql_database_system/c_bson.hpp>

//#if !defined(BSON_INSIDE)
//#	define BSON_INSIDE
//#	define BSON_COMPILATION
//#endif // BSON_INSIDE
//
//#define BSON_OS_WIN32
//
////#include <bson/bson-compat.h>
//
//#ifdef BSON_OS_WIN32
////#if defined(_WIN32_WINNT) && (_WIN32_WINNT < 0x0600)
////#undef _WIN32_WINNT
////#endif
////#ifndef _WIN32_WINNT
////#define _WIN32_WINNT 0x0600 //出错在这里
////#endif
//#ifndef NOMINMAX
//#define NOMINMAX
//#endif
////#include <winsock2.h>
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
////#include <windows.h>
//#undef WIN32_LEAN_AND_MEAN
//#else
//#include <windows.h>
//#endif
////#include <direct.h>
////#include <io.h>
//#endif
//
//
//#include <inttypes.h>
//#include <stdbool.h>
//
//#undef BSON_INSIDE
//#undef BSON_COMPILATION

#include <yggr/nsql_database_system/c_mongo_connection.hpp>


int main(int argc, char *argv[])
{
	
	return 0;
}