// protocol_reg.cpp

#include "protocol_reg.hpp"

#if defined(YGGR_USE_SEH)

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_network_protocol(svr_ptr_type psvr)
{
	assert(psvr);
	bool b = true;

	b = b && psvr->register_network_protocol<test_pak_type>(); // 注册网络数据（该例题之收发test_pak_type所以只注册这一个）

	assert(b);
}

#endif // #if defined(YGGR_USE_SEH)