//mongo_connection.cpp

#include <iostream>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_mongo_connection.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

#ifdef _MSC_VER
#ifdef _DEBUG
#   pragma comment(lib, "libmongoc-vc100-d.lib")
#   pragma comment(lib, "libbson-vc100-d.lib")

#else

#   pragma comment(lib, "libmongoc-vc100.lib")
#   pragma comment(lib, "libbson-vc100.lib")

#endif // _DEBUG
#endif // _MSC_VER

typedef yggr::nsql_database_system::c_mongo_connection conn_type;

int main(int argc, char* argv[])
{
	conn_type::this_ptr_type pconn(new conn_type("127.0.0.1:10098"));
	pconn->connect();

	char cc = 0;
	std::cin >> cc;

	return 0;
}
