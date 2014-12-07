//network_packet_id_def.hpp

#ifndef __YGGR_NETWORK_NETWORK_DATA_ID_DEF_HPP__
#define __YGGR_NETWORK_NETWORK_DATA_ID_DEF_HPP__

namespace yggr
{
namespace network
{

struct network_data_def
{
	enum
	{
		E_user_id_start = 0x10000000,
		E_compile_u32 = 0xffffffff
	};
};

} // namesapce network
} // namespace yggr

#endif //__YGGR_NETWORK_NETWORK_DATA_ID_DEF_HPP__
