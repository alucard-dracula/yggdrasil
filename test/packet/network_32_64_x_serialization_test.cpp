// network_32_64_x_serialization_test

#define YGGR_SERIALIZE_SIZE32 // 32 cross 64 bit must defined YGGR_SERIALIZE_SIZE32

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/network/support/data_info_def.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>
#include <yggr/network/network_protocol.hpp>

#include <yggr/task_center/task_center.hpp>

#include <yggr/network/socket_info.hpp>
#include <yggr/p2p/protocol/p2p_packet_def.hpp>
#include <yggr/p2p/protocol/p2p_boot_request.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/serialization/string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

template<typename OPak, typename IPak>
void test_pak_back_id_basic_pack(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type;

	yggr::ids::id64_type id_part_l = static_cast<yggr::ids::id64_type>(0x11223344);
	yggr::ids::id64_type id_part_r = static_cast<yggr::ids::id64_type>(0x55667788);

	test_pak_type in_obj((id_part_l << 32) + id_part_r);

	opak_type o;
	o.save(in_obj);

	yggr_test_assert(o.size());

	yggr::string str_fpath64 = "../test_out/packet/test_pak_back_id_basic_pack_64.bns";
	yggr::string str_fpath32 = "../test_out/packet/test_pak_back_id_basic_pack_32.bns";

#if defined(WIN64) || defined(YGGR_SYSTEM_64)
	const yggr::string& out_str_fpath = str_fpath64;
#else
	const yggr::string& out_str_fpath = str_fpath32;
#endif // WIN64

	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object(out_str_fpath), 
			o.org_buf(), yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

#if defined(WIN64) || defined(YGGR_SYSTEM_64)
	const yggr::string& in_str_fpath = str_fpath32;
#else
	const yggr::string& in_str_fpath = str_fpath64;
#endif // WIN64

	yggr::string in_buf;

	try
	{
		if(yggr::file_system::local_fsys_operators::read_file_of_binary(
				yggr::file_system::local_fsys_operators::make_path_object(in_str_fpath), 
				in_buf) == 0)
		{
			std::cerr << "read file " << in_str_fpath << " failed" << std::endl;
			return;
		}
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	if(in_buf.empty())
	{
		std::cerr << "read file " << in_str_fpath << " failed" << std::endl;
		return;
	}

	ipak_type i(in_buf.begin(), in_buf.end());
	test_pak_type out_obj;
	i.load(out_obj);

	yggr_test_assert(in_obj.id() == out_obj.id());
}

template<typename OPak, typename IPak>
void test_pak_back_id_network_pack(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type;

	yggr::ids::id64_type id_part_l = static_cast<yggr::ids::id64_type>(0x11223344);
	yggr::ids::id64_type id_part_r = static_cast<yggr::ids::id64_type>(0x55667788);

	test_pak_type in_obj((id_part_l << 32) + id_part_r);

	opak_type o;
	o.save(in_obj);

	yggr_test_assert(o.size());

	yggr::string str_fpath64 = "../test_out/packet/test_pak_back_id_network_pack_64.bns";
	yggr::string str_fpath32 = "../test_out/packet/test_pak_back_id_network_pack_32.bns";

#if defined(WIN64) || defined(YGGR_SYSTEM_64)
	const yggr::string& out_str_fpath = str_fpath64;
#else
	const yggr::string& out_str_fpath = str_fpath32;
#endif // WIN64

	try
	{
		yggr::file_system::local_fsys_operators::write_file_of_binary(
			yggr::file_system::local_fsys_operators::make_path_object(out_str_fpath), 
			o.org_buf(), yggr::file_system::local_fsys_operators::options_type::dir_create_if_not_exists);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

#if defined(WIN64) || defined(YGGR_SYSTEM_64)
	const yggr::string& in_str_fpath = str_fpath32;
#else
	const yggr::string& in_str_fpath = str_fpath64;
#endif // WIN64

	yggr::string in_buf;

	try
	{
		if(yggr::file_system::local_fsys_operators::read_file_of_binary(
				yggr::file_system::local_fsys_operators::make_path_object(in_str_fpath), 
				in_buf) == 0)
		{
			std::cerr << "read file " << in_str_fpath << " failed" << std::endl;
			return;
		}
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch(const yggr::compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	if(in_buf.empty())
	{
		std::cerr << "read file " << in_str_fpath << " failed" << std::endl;
		return;
	}

	ipak_type i(o.net_info(), in_buf.begin(), in_buf.end());
	test_pak_type out_obj;
	i.load(out_obj);

	yggr_test_assert(in_obj.id() == out_obj.id());
}

template<typename OPak, typename IPak>
void test_p2p_boot_request(void)
{
	typedef OPak opak_type;
	typedef IPak ipak_type;

	typedef yggr::network::socket_info<yggr::string, yggr::string> socket_info_type;
	typedef yggr::p2p::p2p_boot_request<socket_info_type> p2p_boot_request_type;

	typedef
		yggr::network::network_protocol
		<
			p2p_boot_request_type, 
			yggr::task_center::default_task_data_info_type,
			yggr::p2p::p2p_packet_def::E_id_p2p_boot_request,
			1
		> net_p2p_boot_request_type;


	net_p2p_boot_request_type in_obj;
	in_obj.msg() = p2p_boot_request_type(socket_info_type("192.168.1.101", "5555"));

	opak_type o;
	o.save(in_obj);

	yggr_test_assert(o.size());

	ipak_type i(o);
	net_p2p_boot_request_type out_obj;
	i.load(out_obj);

	yggr_test_assert(in_obj.msg().local_socket_info() == out_obj.msg().local_socket_info());
}

int main(int argc, char* argv[])
{

	typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> opak_type;
	typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> ipak_type;


	{
		
		typedef yggr::network::network_info<yggr::ids::id64_type> network_info_type;

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

		typedef yggr::packet::packet<yggr::archive::archive_partner::network_oarchive_partner> base_net_opak_type;
		typedef yggr::packet::packet<yggr::archive::archive_partner::network_iarchive_partner> base_net_ipak_type;

		test_pak_back_id_basic_pack<base_net_opak_type, base_net_ipak_type>();
		test_pak_back_id_network_pack<net_opak_type, net_ipak_type>();


		test_p2p_boot_request<net_opak_type, net_ipak_type>();
	}

	wait_any_key(argc, argv);
	return 0;
}
