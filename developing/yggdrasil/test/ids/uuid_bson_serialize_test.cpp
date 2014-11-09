//bson_test.cpp

#include <iostream>
#include <utility>
#include <string>

#include <yggr/ids/uuid.hpp>

#include <yggr/nsql_database_system/mongodb_config.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

//#include <yggr/archive/bson_oarchive.hpp>
//#include <yggr/archive/bson_iarchive.hpp>

//#include <boost/serialization/shared_ptr.hpp>
//#include <yggr/serialization/bson_data_type.hpp>
//#include <yggr/serialization/array.hpp>
//#include <yggr/serialization/vector.hpp>
//#include <yggr/serialization/list.hpp>
//#include <yggr/serialization/set.hpp>
//#include <yggr/serialization/map.hpp>
//#include <yggr/serialization/unordered_set.hpp>
//#include <yggr/serialization/unordered_map.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

typedef yggr::archive::bson_oarchive<false> oar_type;
typedef yggr::archive::bson_iarchive<false> iar_type;


typedef yggr::ids::uuid uuid_type;

void test1(void)
{
	std::string id = "1000000000000000";

	uuid_type uuid1(id);
	yggr::nsql_database_system::c_bson buf;

	oar_type oar(buf);
	oar & uuid1;
	buf.finish();

	std::cout << buf << std::endl;

	uuid_type uuid2;
	iar_type iar(buf);
	iar & uuid2;
	std::cout << uuid2 << std::endl;

	assert(uuid1 == uuid2);
}

int main(int argc, char* argv[])
{
	test1();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
