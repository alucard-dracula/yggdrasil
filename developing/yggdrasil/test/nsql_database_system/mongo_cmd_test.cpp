//mongo_cmd_test.cpp

#include <iostream>
#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#include <string>

#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>


#include <vector>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_WIN32

#ifdef _MSC_VER
#   ifdef _DEBUG
#   pragma comment(lib, "libmongoc-vc100-d.lib")
#   pragma comment(lib, "libbson-vc100-d.lib")

#   else
#   pragma comment(lib, "libmongoc-vc100.lib")
#   pragma comment(lib, "libbson-vc100.lib")
#   endif // _DEBUG
#endif // _MSC_VER

typedef yggr::nsql_database_system::c_mongo_connection connection_type;

int main(int argc, char* argv[])
{
	connection_type::this_ptr_type pconn(new connection_type("127.0.0.1:10098"));
	pconn->connect();

	//yggr::nsql_database_system::mongo_accesser<connection_type> accesser(pconn, "xy", "123456");
	yggr::nsql_database_system::mongo_accesser<connection_type> accesser(pconn);


	typedef yggr::nsql_database_system::mongo_accesser<connection_type>::mongo_index_vt_type mongo_index_vt_type;
	typedef mongo_index_vt_type::value_type mongo_index_item_type;

	mongo_index_vt_type vt;
	vt.push_back(mongo_index_item_type(std::string("a"), yggr::nsql_database_system::mongo_accesser<connection_type>::E_MONGO_SORT_ASC));
	vt.push_back(mongo_index_item_type(std::string("b"), yggr::nsql_database_system::mongo_accesser<connection_type>::E_MONGO_SORT_DESC));

	yggr::nsql_database_system::c_bson out1, tout;

	accesser.drop_all_indexes("test", "foo");
	accesser.create_simple_indexes(std::string("test"), std::string("foo"), vt);


	yggr::nsql_database_system::c_bson out2;
	accesser.drop_index("test", "foo", "b", -1, out2);

	char cc = 0;
	std::cin >> cc;

	vt.clear();

	return 0;
}
