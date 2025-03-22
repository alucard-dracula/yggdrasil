//protobuf_packet_test.cpp

#include <iostream>

#include <yggr/archive/net_oarchive.hpp>
#include <yggr/archive/net_iarchive.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/network/support/data_info_def.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/compile_link/linker.hpp>

#include "ffgm_hello.pb.h"

#include "protobuf_network_protocol.hpp"

#ifdef _MSV_VER
#	include <vld.h>
#endif // _MSV_VER

typedef yggr::network::network_info<yggr::ids::uuid> network_info_type;
typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

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

typedef flpx::network::protobuf_network_protocol<1, 0> proto_data_type;

#include YGGR_PP_LINK_LIB(exception)
//#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

#ifdef _MSC_VER
#	if defined(_DEBUG)
#		pragma comment(lib, "libprotobufd.lib")
#		pragma comment(lib, "libprotobuf-lited.lib")
#	else
#		pragma comment(lib, "libprotobuf.lib")
#		pragma comment(lib, "libprotobuf-lite.lib")
#	endif //_DEBUG
#endif // _MSC_VER

void test(void)
{
	typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;

	typedef net_opak_type::size_container_type size_container_type;
	typedef net_opak_type::net_info_type net_info_type;
	{
		ffgm::helloworld msg;
		int n = 0x70000000;
		int m = htonl(n);
		msg.set_id(htonl(n));
		msg.set_str("hello");

		net_opak_type opak;

		opak.save(proto_data_type(msg));

		size_container_type size_cont = yggr::network::hton(size_container_type(opak.net_info_size(), opak.data_size()));
		const net_info_type& net_info = opak.net_info();

		local_fsys_operators_type::append_file_of_binary(
			local_fsys_operators_type::make_path_object("./protobuf_test/test_out/hello_proto"),
			size_cont.buf(), net_opak_type::size_container_size(),
			local_fsys_operators_type::options_type::dir_create_if_not_exists);

		local_fsys_operators_type::append_file_of_binary(
			local_fsys_operators_type::make_path_object("./protobuf_test/test_out/hello_proto"),
			net_info.buf(), net_info.buf_size(),
			local_fsys_operators_type::options_type::dir_create_if_not_exists);

		local_fsys_operators_type::append_file_of_binary(
			local_fsys_operators_type::make_path_object("./protobuf_test/test_out/hello_proto"),
			opak.data_buf(), opak.data_size(),
			local_fsys_operators_type::options_type::dir_create_if_not_exists);
	}

	{
		yggr::string buf;
		local_fsys_operators_type::read_file_of_binary(
			local_fsys_operators_type::make_path_object("./protobuf_test/test_out/hello_proto"),
			buf);

		assert(!buf.empty());

		size_container_type size_cont(yggr::network::ntoh(
										size_container_type(
											buf.begin(), buf.begin() + net_ipak_type::size_container_size())));

		net_info_type net_info;
		net_info.assign(buf.begin() + net_ipak_type::size_container_size(), 
						buf.begin() + (net_ipak_type::size_container_size() + size_cont.info_size()));

		//net_ipak_type ipak(boost::move(net_info),
		//					buf.begin() + (net_ipak_type::size_container_size() + size_cont.info_size()),
		//					buf.begin() + (net_ipak_type::size_container_size() + size_cont.info_size() + size_cont.data_size()));

		net_ipak_type ipak;
		ipak.assign(boost::move(net_info),
					buf.begin() + (net_ipak_type::size_container_size() + size_cont.info_size()),
					buf.begin() + (net_ipak_type::size_container_size() + size_cont.info_size() + size_cont.data_size()));

		proto_data_type proto_data;
		ipak.load(proto_data);

		ffgm::helloworld msg;

		proto_data.load(msg);

		assert(ntohl(msg.id()) == 0x70000000);
		assert(msg.str() == "hello");

		std::cout << std::hex << ntohl(msg.id()) << std::endl;
		std::cout << msg.str() << std::endl;

	}

	google::protobuf::ShutdownProtobufLibrary();
}

int main(int argc, char* argv[])
{
	test();

	std::cout << "all test success" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}