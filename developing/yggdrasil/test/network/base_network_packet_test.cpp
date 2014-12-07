//basic_network_packet_test.cpp

#define YGGR_SERIALIZE_SIZE32

#include <iostream>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/network/basic_network_packet.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/network/support/data_info_def.hpp>

void test0_1(void)
{
	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type
				> base_net_opak_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type
				> base_net_ipak_type;

	int n1(100);
	base_net_opak_type opak;
	opak.save(n1);

	base_net_ipak_type ipak(opak);
	assert((ipak.data_info() == base_net_ipak_type::data_info_parser_type::data_info<int>()));

	int n2;
	ipak.load(n2);
	assert(n1 == n2);
}

void test0_2(void)
{
	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type
				> base_net_opak_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type
				> base_net_ipak_type;

	int n1(100);
	base_net_opak_type opak;
	opak.save(n1);

	base_net_ipak_type ipak(opak);
	assert((ipak.data_info() == base_net_ipak_type::data_info_parser_type::data_info<int>()));

	int n2;
	ipak.load(n2);
	assert(n1 == n2);
}

void test1_1(void)
{
	typedef yggr::network::start_data::pak_back_id<int> test_data_type;
	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type
				> base_net_opak_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type
				> base_net_ipak_type;

	test_data_type n1(100);
	base_net_opak_type opak;
	opak.save(n1);

	base_net_ipak_type ipak(opak);
	assert((ipak.data_info() == base_net_ipak_type::data_info_parser_type::data_info<test_data_type>()));

	test_data_type n2;
	ipak.load(n2);
	assert(n1.id() == n2.id());
}

void test1_2(void)
{
	typedef yggr::network::start_data::pak_back_id<int> test_data_type;
	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type
				> base_net_opak_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type
				> base_net_ipak_type;

	test_data_type n1(100);
	base_net_opak_type opak;
	opak.save(n1);

	base_net_ipak_type ipak(opak);
	assert((ipak.data_info() == base_net_ipak_type::data_info_parser_type::data_info<test_data_type>()));

	test_data_type n2;
	ipak.load(n2);
	assert(n1.id() == n2.id());
}

void test2(void)
{
	typedef yggr::network::start_data::pak_back_id<int> test_data_type;
	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type
				> base_net_opak_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type
				> base_net_ipak_type;

	test_data_type n1(100);
	base_net_opak_type opak;
	opak.save(n1);

	base_net_ipak_type ipak(boost::move(opak));

	test_data_type n2;
	ipak.load(n2);
	assert(n1.id() == n2.id());
}

void test3(void)
{
	typedef yggr::network::start_data::pak_back_id<int> test_data_type;
	typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type
				> base_net_opak_type;

	typedef yggr::network::basic_network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type
				> base_net_ipak_type;

	test_data_type n1(100);
	base_net_opak_type opak;
	opak.save(n1);

	base_net_ipak_type ipak(boost::move(opak.org_buf()));

	test_data_type n2;
	ipak.load(n2);
	assert(n1.id() == n2.id());
}

int main(int argc, char* argv[])
{
	test0_1();
	test0_2();
	test1_1();
	test1_2();
	test2();
	test3();

	std::cout << "completed" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
