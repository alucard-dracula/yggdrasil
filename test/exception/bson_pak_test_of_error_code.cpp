//bson_pak_test_of_error_code.cpp

#define YGGR_SERIALIZE_SIZE32

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/archive/bson_archive_partner.hpp>
#include <yggr/exception/serialization/error_code.hpp>

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


void test_error_code_of_bson(void)
{
	typedef yggr::exception::error_code error_code_type;

	error_code_type t1(1, "test");
	o_bson_packet_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP("test_tt", t1));

	std::cout << opak.org_buf() << std::endl;

	error_code_type t2;

	yggr_test_assert(t1.code() != t2.code());

	i_bson_packet_type ipak(opak);
	ipak.load(YGGR_SERIALIZATION_NAME_NVP("test_tt", t2));

	yggr_test_assert(t1.code() == t2.code());
	yggr_test_assert(t1.time() == t2.time());
	yggr_test_assert(t1.thread_id() == t2.thread_id());
	yggr_test_assert(t1.message() == t2.message());
}

void test_error_code_of_bson2(void)
{
	typedef yggr::exception::error_code error_code_type;

	error_code_type t1(1, "test");
	o_bson_packet_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	error_code_type t2;

	yggr_test_assert(t1.code() != t2.code());

	i_bson_packet_type ipak(opak);
	ipak.load(t2);

	yggr_test_assert(t1.code() == t2.code());
	yggr_test_assert(t1.time() == t2.time());
	yggr_test_assert(t1.thread_id() == t2.thread_id());
	yggr_test_assert(t1.message() == t2.message());
}

void test_error_code_of_network(void)
{
	typedef yggr::exception::error_code error_code_type;

	error_code_type t1(1, "test");
	opacket_type opak;
	opak.save(t1);

	std::cout << opak.org_buf() << std::endl;

	error_code_type t2;

	yggr_test_assert(t1.code() != t2.code());

	ipacket_type ipak(opak);
	ipak.load(t2);

	yggr_test_assert(t1.code() == t2.code());
	yggr_test_assert(t1.time() == t2.time());
	yggr_test_assert(t1.thread_id() == t2.thread_id());
	yggr_test_assert(t1.message() == t2.message());
}

void test_both_of_container(void)
{
	test_error_code_of_network();
	test_error_code_of_bson();
	test_error_code_of_bson2();
}

int main(int argc, char* argv[])
{
	test_both_of_container();

	wait_any_key(argc, argv);
	return 0;
}
