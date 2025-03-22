// protocol_reg.cpp

#include "protocol_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_network_protocol(svr_ptr_type psvr)
{
	assert(psvr);
	bool b = true;
#if 1
	b = psvr->register_network_protocol<test_pak_type>(); // 注册网络数据（该例题之收发test_pak_type所以只注册这一个）
#else
	// 下面2句等价 psvr->register_network_protocol<test_pak_type>()
	b = b && psvr->register_network_protocol<test_pak_type>(network_direct_def_type::E_recv_enable); // 允许收取
	b = b && psvr->register_network_protocol<test_pak_type>(network_direct_def_type::E_send_enable); // 允许发送
#endif // 0 ,1
	assert(b);
}

void unreg_network_protocol(svr_ptr_type psvr)
{
	assert(psvr);
	bool b = true;
#if 1
	b = psvr->unregister_network_protocol<test_pak_type>(); // 注册网络数据（该例题之收发test_pak_type所以只注册这一个）
#else
	// 下面2句等价 psvr->register_network_protocol<test_pak_type>()
	b = b && psvr->unregister_network_protocol<test_pak_type>(network_direct_def_type::E_recv_enable); // 允许收取
	b = b && psvr->unregister_network_protocol<test_pak_type>(network_direct_def_type::E_send_enable); // 允许发送
#endif // 0 ,1
	assert(b);
}