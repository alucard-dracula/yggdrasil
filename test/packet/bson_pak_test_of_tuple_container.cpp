// bson_pak_test_of_tuple_container.cpp

//#define MONGO_STATIC_BUILD

#define YGGR_SERIALIZE_SIZE32
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <string>
#include <vector>

#include <yggr/serialization/nvp.hpp>
#include <yggr/archive/bson_archive_partner.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/bson_inner_data.hpp>
#include <yggr/serialization/tuple.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::bson_oarchive_partner> o_bson_packet_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::bson_iarchive_partner> i_bson_packet_type;

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

// test std tuple
void test_std_tuple_of_bson(void)
{
	typedef std::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(std::make_tuple(1, 2, 3, 4, 5));
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", t1));

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(yggr::get<0>(t1) != yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) != yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) != yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) != yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) != yggr::get<4>(t2));

	yggr_test_assert(t1 != t2);

	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", t2));

	yggr_test_assert(yggr::get<0>(t1) == yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) == yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) == yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) == yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) == yggr::get<4>(t2));
	yggr_test_assert(t1 == t2);
}


void test_std_tuple_of_bson2(void)
{
	typedef std::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(std::make_tuple(1, 2, 3, 4, 5));
	o_bson_packet_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(yggr::get<0>(t1) != yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) != yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) != yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) != yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) != yggr::get<4>(t2));

	yggr_test_assert(t1 != t2);

	i_bson_packet_type ipak(opak);
	ipak.load(t2);

	yggr_test_assert(yggr::get<0>(t1) == yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) == yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) == yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) == yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) == yggr::get<4>(t2));
	yggr_test_assert(t1 == t2);
}

void test_std_tuple_of_network(void)
{
	typedef std::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(std::make_tuple(1, 2, 3, 4, 5));
	opacket_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(yggr::get<0>(t1) != yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) != yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) != yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) != yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) != yggr::get<4>(t2));

	yggr_test_assert(t1 != t2);

	ipacket_type ipak(opak);
	ipak.load(t2);

	yggr_test_assert(yggr::get<0>(t1) == yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) == yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) == yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) == yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) == yggr::get<4>(t2));
	yggr_test_assert(t1 == t2);
}

void test_std_tuple_of_container(void)
{
	test_std_tuple_of_network();
	test_std_tuple_of_bson();
	test_std_tuple_of_bson2();
}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

// test boost tuple
void test_boost_tuple_of_bson(void)
{
	typedef boost::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(boost::make_tuple(1, 2, 3, 4, 5));
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", t1));

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(boost::get<0>(t1) != boost::get<0>(t2));
	yggr_test_assert(boost::get<1>(t1) != boost::get<1>(t2));
	yggr_test_assert(boost::get<2>(t1) != boost::get<2>(t2));
	yggr_test_assert(boost::get<3>(t1) != boost::get<3>(t2));
	yggr_test_assert(boost::get<4>(t1) != boost::get<4>(t2));

	yggr_test_assert(t1 != t2);

	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", t2));

	yggr_test_assert(boost::get<0>(t1) == boost::get<0>(t2));
	yggr_test_assert(boost::get<1>(t1) == boost::get<1>(t2));
	yggr_test_assert(boost::get<2>(t1) == boost::get<2>(t2));
	yggr_test_assert(boost::get<3>(t1) == boost::get<3>(t2));
	yggr_test_assert(boost::get<4>(t1) == boost::get<4>(t2));
	yggr_test_assert(t1 == t2);
}


void test_boost_tuple_of_bson2(void)
{
	typedef boost::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(boost::make_tuple(1, 2, 3, 4, 5));
	o_bson_packet_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(boost::get<0>(t1) != boost::get<0>(t2));
	yggr_test_assert(boost::get<1>(t1) != boost::get<1>(t2));
	yggr_test_assert(boost::get<2>(t1) != boost::get<2>(t2));
	yggr_test_assert(boost::get<3>(t1) != boost::get<3>(t2));
	yggr_test_assert(boost::get<4>(t1) != boost::get<4>(t2));

	yggr_test_assert(t1 != t2);

	i_bson_packet_type ipak(opak);
	ipak.load(t2);

	yggr_test_assert(boost::get<0>(t1) == boost::get<0>(t2));
	yggr_test_assert(boost::get<1>(t1) == boost::get<1>(t2));
	yggr_test_assert(boost::get<2>(t1) == boost::get<2>(t2));
	yggr_test_assert(boost::get<3>(t1) == boost::get<3>(t2));
	yggr_test_assert(boost::get<4>(t1) == boost::get<4>(t2));
	yggr_test_assert(t1 == t2);
}

void test_boost_tuple_of_network(void)
{
	typedef boost::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(boost::make_tuple(1, 2, 3, 4, 5));
	opacket_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(boost::get<0>(t1) != boost::get<0>(t2));
	yggr_test_assert(boost::get<1>(t1) != boost::get<1>(t2));
	yggr_test_assert(boost::get<2>(t1) != boost::get<2>(t2));
	yggr_test_assert(boost::get<3>(t1) != boost::get<3>(t2));
	yggr_test_assert(boost::get<4>(t1) != boost::get<4>(t2));

	yggr_test_assert(t1 != t2);

	ipacket_type ipak(opak);
	ipak.load(t2);

	yggr_test_assert(boost::get<0>(t1) == boost::get<0>(t2));
	yggr_test_assert(boost::get<1>(t1) == boost::get<1>(t2));
	yggr_test_assert(boost::get<2>(t1) == boost::get<2>(t2));
	yggr_test_assert(boost::get<3>(t1) == boost::get<3>(t2));
	yggr_test_assert(boost::get<4>(t1) == boost::get<4>(t2));
	yggr_test_assert(t1 == t2);
}

void test_boost_tuple_of_container(void)
{
	test_boost_tuple_of_network();
	test_boost_tuple_of_bson();
	test_boost_tuple_of_bson2();
}

// test yggr tuple
void test_yggr_tuple_of_bson(void)
{
	typedef yggr::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(yggr::make_tuple(1, 2, 3, 4, 5));
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("container", t1));

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(yggr::get<0>(t1) != yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) != yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) != yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) != yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) != yggr::get<4>(t2));

	yggr_test_assert(t1 != t2);

	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("container", t2));

	yggr_test_assert(yggr::get<0>(t1) == yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) == yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) == yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) == yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) == yggr::get<4>(t2));
	yggr_test_assert(t1 == t2);
}


void test_yggr_tuple_of_bson2(void)
{
	typedef yggr::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(yggr::make_tuple(1, 2, 3, 4, 5));
	o_bson_packet_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(yggr::get<0>(t1) != yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) != yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) != yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) != yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) != yggr::get<4>(t2));

	yggr_test_assert(t1 != t2);

	i_bson_packet_type ipak(opak);
	ipak.load(t2);

	yggr_test_assert(yggr::get<0>(t1) == yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) == yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) == yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) == yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) == yggr::get<4>(t2));
	yggr_test_assert(t1 == t2);
}

void test_yggr_tuple_of_network(void)
{
	typedef yggr::tuple<int, int, int, int, int> tuple_type;
	tuple_type t1(yggr::make_tuple(1, 2, 3, 4, 5));
	opacket_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	tuple_type t2;

	yggr_test_assert(yggr::get<0>(t1) != yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) != yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) != yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) != yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) != yggr::get<4>(t2));

	yggr_test_assert(t1 != t2);

	ipacket_type ipak(opak);
	ipak.load(t2);

	yggr_test_assert(yggr::get<0>(t1) == yggr::get<0>(t2));
	yggr_test_assert(yggr::get<1>(t1) == yggr::get<1>(t2));
	yggr_test_assert(yggr::get<2>(t1) == yggr::get<2>(t2));
	yggr_test_assert(yggr::get<3>(t1) == yggr::get<3>(t2));
	yggr_test_assert(yggr::get<4>(t1) == yggr::get<4>(t2));
	yggr_test_assert(t1 == t2);
}

void test_yggr_tuple_of_container(void)
{
	test_yggr_tuple_of_network();
	test_yggr_tuple_of_bson();
	test_yggr_tuple_of_bson2();
}

int main(int argc, char* argv[])
{
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	std::cout << "----------------test std tuple-------------" << std::endl;
	test_std_tuple_of_container();
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	std::cout << "----------------test boost tuple-------------" << std::endl;
	test_boost_tuple_of_container();
	std::cout << "----------------test yggr tuple-------------" << std::endl;
	test_yggr_tuple_of_container();

	wait_any_key(argc, argv);
	return 0;
}
