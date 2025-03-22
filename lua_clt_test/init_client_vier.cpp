//init_server.cpp

#include <cassert>
#include "init_client.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void init_log(void)
{
	yggr::log::default_log_accesser::init_type log_init; //memd to BOOST_PP_LOCAL_MACRO
	log_init.push_back("clt_nt_log_eins");
	log_init.push_back("clt_nt_log_zwei");

	log_accesser_single_type::init_ins(log_init);

	yggr::log::default_log_accesser::op_caller_type dc1("clt_nt_log_eins");
	yggr::log::default_log_accesser::op_caller_type dc2("clt_nt_log_zwei");
	log_accesser_single_type::get_ins()->register_msg(boost::asio::error::connection_refused, dc1);
	log_accesser_single_type::get_ins()->register_msg(boost::asio::error::not_connected, dc2);
}
