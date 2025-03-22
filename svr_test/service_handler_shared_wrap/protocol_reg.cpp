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
	b = psvr->register_network_protocol<test_pak_type>(); // ע���������ݣ�������֮�շ�test_pak_type����ֻע����һ����
#else
	// ����2��ȼ� psvr->register_network_protocol<test_pak_type>()
	b = b && psvr->register_network_protocol<test_pak_type>(network_direct_def_type::E_recv_enable); // ������ȡ
	b = b && psvr->register_network_protocol<test_pak_type>(network_direct_def_type::E_send_enable); // ������
#endif // 0 ,1
	assert(b);
}

void unreg_network_protocol(svr_ptr_type psvr)
{
	assert(psvr);
	bool b = true;
#if 1
	b = psvr->unregister_network_protocol<test_pak_type>(); // ע���������ݣ�������֮�շ�test_pak_type����ֻע����һ����
#else
	// ����2��ȼ� psvr->register_network_protocol<test_pak_type>()
	b = b && psvr->unregister_network_protocol<test_pak_type>(network_direct_def_type::E_recv_enable); // ������ȡ
	b = b && psvr->unregister_network_protocol<test_pak_type>(network_direct_def_type::E_send_enable); // ������
#endif // 0 ,1
	assert(b);
}