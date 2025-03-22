//network_packet_test.cpp

#define YGGR_SERIALIZE_SIZE32

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>
#include <yggr/network/network_packet.hpp>
#include <yggr/network/support/data_info_def.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

void test0_1(void)
{
	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;
	typedef yggr::network::network_info<yggr::u64> network_info_type;
	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
				> net_opak_type;

	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
				> net_ipak_type;

	int n1 = 100;
	net_opak_type opak(network_info_type(yggr::u64(2)), n1);

	net_ipak_type ipak(opak);
	yggr_test_assert((ipak.data_info() == net_ipak_type::data_info_parser_type::data_info<int>()));

	int n2 = 0;
	ipak.load(n2);
	yggr_test_assert(n1 == n2);
}

void test0_2(void)
{
	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;
	typedef yggr::network::network_info<yggr::u64> network_info_type;
	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
				> net_opak_type;

	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
				> net_ipak_type;

	int n1 = 100;
	net_opak_type opak(network_info_type(yggr::u64(2)), n1);

	net_ipak_type ipak(boost::move(opak));
	yggr_test_assert((ipak.data_info() == net_ipak_type::data_info_parser_type::data_info<int>()));

	int n2 = 0;
	ipak.load(n2);
	yggr_test_assert(n1 == n2);
}

void test1_1(void)
{
	typedef yggr::network::start_data::pak_back_id<int> test_data_type;

	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;
	typedef yggr::network::network_info<yggr::u64> network_info_type;
	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
				> net_opak_type;

	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
				> net_ipak_type;

	test_data_type n1(100);
	net_opak_type opak(network_info_type(yggr::u64(2)));

	opak.save(n1);

	net_ipak_type ipak(opak);
	yggr_test_assert((ipak.data_info() == net_ipak_type::data_info_parser_type::data_info<test_data_type>()));
	yggr_test_assert(opak.net_info() == ipak.net_info());

	test_data_type n2;
	ipak.load(n2);
	yggr_test_assert(n1.id() == n2.id());
}

void test1_2(void)
{
	typedef yggr::network::start_data::pak_back_id<int> test_data_type;

	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;
	typedef yggr::network::network_info<yggr::u64> network_info_type;
	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
				> net_opak_type;

	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
				> net_ipak_type;

	test_data_type n1(100);
	net_opak_type opak(network_info_type(yggr::u64(2)), n1);

	net_ipak_type ipak(boost::move(opak));
	yggr_test_assert((ipak.data_info() == net_ipak_type::data_info_parser_type::data_info<test_data_type>()));

	test_data_type n2;
	ipak.load(n2);
	yggr_test_assert(n1.id() == n2.id());
}

void test1_3(void)
{
	typedef yggr::network::start_data::pak_back_id<int> test_data_type;

	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;
	typedef yggr::network::network_info<yggr::u64> network_info_type;
	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
				> net_opak_type;

	typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
				> net_ipak_type;

	test_data_type n1(100);
	net_opak_type opak(network_info_type(yggr::u64(2)), n1);

	net_ipak_type ipak(boost::move(opak.net_info()), boost::move(opak.org_buf()));
	ipak.restore_info(); // need manual call
	yggr_test_assert((ipak.data_info() == net_ipak_type::data_info_parser_type::data_info<test_data_type>()));

	test_data_type n2;
	ipak.load(n2);
	yggr_test_assert(n1.id() == n2.id());
}

int main(int argc, char* argv[])
{
	test0_1();
	test0_2();
	test1_1();
	test1_2();
	test1_3();

	wait_any_key(argc, argv);
	return 0;
}
