//mongo_connection_only_test.cpp

#include <iostream>
#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_mongo_connection.hpp>

#include <string>

#ifdef WIN32
#   ifdef _DEBUG
#       include <vld.h>
#   endif //_DEBUG
#endif //WIN32

#ifdef _DEBUG
#   pragma comment(lib, "libmongoc-vc100-d.lib")
#else
#   pragma comment(lib, "libmongoc-vc100.lib")
#endif // _DEBUG


int main(int argc, char* argv[])
{
	typedef yggr::nsql_database_system::c_mongo_connection conn_type;
	typedef conn_type::this_ptr_type conn_ptr_type;
	typedef conn_type::init_type conn_init_type;

	conn_init_type init("rs0");
	init << "127.0.0.1:10099"
		<< "127.0.0.1:10100"
		<< "127.0.0.1:10101";

	//conn_type::this_ptr_type pconn(new conn_type("127.0.0.1:10098"));
	conn_type::this_ptr_type pconn(new conn_type(init));
	pconn->connect();

	char cc = 0;
	std::cin >> cc;

	return 0;
}
