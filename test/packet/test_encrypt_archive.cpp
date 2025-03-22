//test_encrypt_archive.cpp

#include <test/wait_any_key/wait_any_key.hpp>
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
#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(encryption_tool)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

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
	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary("../test_out/packet/bin.f", opak.org_buf());
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch(const ::yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	std::string buf;
	try
	{
		yggr::file_system::local_fsys_operators::read_file_of_binary("../test_out/packet/bin.f", buf);
	}
	catch(const ::yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	ipak_type ipak(network_info_type(yggr::u64(10)), buf);

	encrypt_tool.decrypt(ipak);
	test_pak_type data2;
	ipak.load(data2);

	std::cout << data2.id() << "," << data2.key() << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
