////bson_pak_test.cpp
//

//#define MONGO_STATIC_BUILD

#define YGGR_SERIALIZE_SIZE32
#include <iostream>
#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <string>
#include <vector>

#include <yggr/serialization/nvp.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/bson_data_type.hpp>

//BOOST_MPL_ASSERT((yggr::nsql_database_system::is_bson_inside_type<yggr::serialization::collection_size_type>));

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;


#include <yggr/serialization/tuple.hpp>

void test_tuple_of_bson(void)
{
	typedef boost::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(boost::make_tuple(1, 2, 3, 4, 5));
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZE_NAME_NVP("container", t1));

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	assert(boost::get<0>(t1) != boost::get<0>(t2));
	assert(boost::get<1>(t1) != boost::get<1>(t2));
	assert(boost::get<2>(t1) != boost::get<2>(t2));
	assert(boost::get<3>(t1) != boost::get<3>(t2));
	assert(boost::get<4>(t1) != boost::get<4>(t2));

	assert(t1 != t2);

	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZE_NAME_NVP("container", t2));

	assert(boost::get<0>(t1) == boost::get<0>(t2));
	assert(boost::get<1>(t1) == boost::get<1>(t2));
	assert(boost::get<2>(t1) == boost::get<2>(t2));
	assert(boost::get<3>(t1) == boost::get<3>(t2));
	assert(boost::get<4>(t1) == boost::get<4>(t2));
	assert(t1 == t2);
}

void test_tuple_of_network(void)
{
	typedef boost::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(boost::make_tuple(1, 2, 3, 4, 5));
	opacket_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	assert(boost::get<0>(t1) != boost::get<0>(t2));
	assert(boost::get<1>(t1) != boost::get<1>(t2));
	assert(boost::get<2>(t1) != boost::get<2>(t2));
	assert(boost::get<3>(t1) != boost::get<3>(t2));
	assert(boost::get<4>(t1) != boost::get<4>(t2));

	assert(t1 != t2);

	ipacket_type ipak(opak);
	ipak.load(t2);

	assert(boost::get<0>(t1) == boost::get<0>(t2));
	assert(boost::get<1>(t1) == boost::get<1>(t2));
	assert(boost::get<2>(t1) == boost::get<2>(t2));
	assert(boost::get<3>(t1) == boost::get<3>(t2));
	assert(boost::get<4>(t1) == boost::get<4>(t2));
	assert(t1 == t2);
}

void test_tuple_of_container(void)
{
	test_tuple_of_network();
	test_tuple_of_bson();
}

int main(int argc, char* argv[])
{
	test_tuple_of_container();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
