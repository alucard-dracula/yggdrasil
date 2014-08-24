//test_encrypt_archive.cpp

#include <iostream>
#include <string>
#include <algorithm>

#include <boost/serialization/access.hpp>
#include <boost/move/move.hpp>
#include <boost/lexical_cast.hpp>

#include <yggr/base/random.h>
#include <yggr/network/network_config/network_balance_tcp_config.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/archive/net_oarchive.hpp>
#include <yggr/archive/net_iarchive.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/network/support/data_info_def.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>

#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/list.hpp>

#include <yggr/network/start_data/encryption_pak_back_id.hpp>

#include <yggr/encryption_tool/blowfish_tool.hpp>
#include <yggr/encryption_tool/packet_cypher_tool.hpp>
#include <yggr/file_system/local_file_operator.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

typedef yggr::encryption_tool::blowfish_tool<> encrypt_tool_type;
typedef yggr::encryption_tool::packet_cypher_tool<encrypt_tool_type> packet_cypher_tool_type;

typedef yggr::network::start_data::encryption_pak_back_id<yggr::ids::id64_type, std::string, 8> test_pak_type;

typedef yggr::network::network_info<yggr::ids::id64_type> network_info_type;
typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
			> opak_type;

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
			> ipak_type;


int main(int argc, char* argv[])
{
	packet_cypher_tool_type encrypt_tool;
	encrypt_tool.init(std::string("abcdabcd"));
	test_pak_type data(1, "abcdabcd");

	std::cout << data.id() << "," << data.key() << std::endl;
    opak_type opak(network_info_type(yggr::u64(10)), data);

	encrypt_tool.encrypt(opak);

	yggr::file_system::local_file_operator_type::write_file_of_binary("bin.f", opak.org_buf());

	std::string buf;
	yggr::file_system::local_file_operator_type::read_file_of_binary("bin.f", buf);
	ipak_type ipak(network_info_type(yggr::u64(10)), buf);

	encrypt_tool.decrypt(ipak);
	test_pak_type data2;
	ipak.load(data2);

	std::cout << data2.id() << "," << data2.key() << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
