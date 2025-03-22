//network_packet_task_center_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/task_center/task.hpp>
#include <yggr/task_center/task_creator.hpp>
#include <yggr/task_center/task_center.hpp>
#include <yggr/task_center/task_store_op.hpp>
#include <yggr/task_center/task_saver.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/task_center/recver_handler_mgr.hpp>

#include <yggr/adapter/adapter_mgr.hpp>
#include <yggr/adapter/adapter_parser_helper.hpp>

#include <yggr/ids/uuid.hpp>
#include <yggr/safe_container/safe_hashed_map_queue.hpp>
#include <yggr/adapter/adapter_mgr.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>

#include <yggr/ids/base_ids_def.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>
#include <yggr/network/adapter_helper/io_converter.hpp>
#include <yggr/network/network_info.hpp>

#include <yggr/network/support/data_info_def.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type;


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

typedef yggr::ids::id_generator<yggr::ids::id64_type> task_id_gen_type;

typedef yggr::task_center::task<yggr::ids::id64_type, network_info_type,
									yggr::task_center::default_task_data_info_type> task_type;

typedef yggr::safe_container::safe_hashed_map_queue<network_info_type::owner_id_type, task_type> task_container_type;

typedef yggr::task_center::key_value_store<task_container_type::value_type, task_type::owner_id_getter> task_store_type;

typedef yggr::task_center::task_saver<yggr::task_center::type_traits::mark_saver_condition, task_type,
										task_container_type, task_store_type> cdt_saver_type;

typedef yggr::task_center::recver_handler_mgr<yggr::task_center::type_traits::mark_saver_result, task_type> rst_saver_type;

typedef yggr::task_center::task_creator<task_id_gen_type, task_type> task_creator_type;
typedef yggr::task_center::task_center<task_creator_type, cdt_saver_type, rst_saver_type> task_center_type;

typedef yggr::shared_ptr<task_center_type> task_center_ptr_type;


typedef 
	yggr::adapter::adapter_id_parser_def
	<
		yggr::task_center::task_data_send_typeid_getter
		<
			yggr::task_center::default_task_data_info_type
		>,

		yggr::task_center::task_data_recv_typeid_getter
		<
			yggr::task_center::default_task_data_info_type
		>,

		YGGR_IDS_PARSER_CONST_MEM_FOO(net_ipak_type::base_type,
												const net_ipak_type::data_info_type&,
												data_info),

		YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
											const yggr
													::task_center
														::default_task_data_info_type
															::data_info_type&,
												data_info),

		YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
										yggr
											::task_center
												::default_task_data_info_type
													::class_name_type,
										class_name),
		YGGR_IDS_PARSER_USE_TYPEID_NAME_CONV_RET(yggr::string)
	> adapter_id_parser_def_type;


typedef 
	yggr::adapter::adapter_mgr
	<
		task_center_type,
		net_ipak_type,
		net_opak_type,
		adapter_id_parser_def_type,
		yggr::network::adapter_helper::io_converter
	> adapter_mgr_type;

typedef yggr::shared_ptr<adapter_mgr_type> adapter_mgr_ptr_type;


void handler_recv(const net_opak_type& pak)
{
	test_pak_type data;

	net_ipak_type ipak(pak);
	if(ipak.load(data))
	{
		std::cout << data.id() <<std::endl;
	}
	else
	{
		std::cout << "handler_recv call failed" << std::endl;
	}
}

template<typename Src>
void handler_fix_task(Src& src, const task_type& tk)
{
	const test_pak_type* pval = tk.get<test_pak_type>();
	yggr_test_assert(pval);

	test_pak_type ret((*pval).id() + 1);
	src.template send<rst_saver_type::mark_type>(tk.owner_info(), ret);
}

void test1(void)
{
	task_center_ptr_type ptc(new task_center_type());
	task_center_type& task_center = *ptc;

	adapter_mgr_ptr_type padp(new adapter_mgr_type(task_center));
	adapter_mgr_type& adp_mgr = *padp;

	test_pak_type td1(100);
	test_pak_type td2(200);

	yggr::ids::id64_type id = td1.id();

	net_opak_type opak1(network_info_type(id), td1);
	net_ipak_type ipak1(boost::move(opak1));

	net_opak_type opak2(network_info_type(id), td2);
	net_ipak_type ipak2(boost::move(opak2));

	adp_mgr.register_adapter<test_pak_type>();

	adp_mgr.send(ipak1); // use adapter send cdt_data to task_center
	adp_mgr.send(ipak2);

	adp_mgr.hold_recv(id, boost::bind(&handler_recv, _1));

	task_center.get_task<cdt_saver_type::mark_type>
		( boost::bind(&handler_fix_task<task_center_type>,
						boost::ref(task_center), _1) );

	task_center.get_task<cdt_saver_type::mark_type>
		( boost::bind(&handler_fix_task<task_center_type>,
						boost::ref(task_center), _1) );

	task_center.get_task<cdt_saver_type::mark_type>
		( boost::bind(&handler_fix_task<task_center_type>,
						boost::ref(task_center), _1) ); // empty_task_center_test

	task_center.clear();

}

int main(int argc, char* argv[])
{
	test1();

	wait_any_key(argc, argv);
	return 0;
}
