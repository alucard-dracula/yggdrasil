//bson_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <utility>
#include <string>

#include <yggr/ids/uuid.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/packet/packet.hpp>

#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::archive::bson_oarchive<false> oar_type;
typedef yggr::archive::bson_iarchive<false> iar_type;


typedef yggr::ids::uuid uuid_type;

void test(void)
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

	yggr_test_assert(uuid1 == uuid2);

	std::cout << "uuid_bson_serialize_test success" << std::endl;
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}
