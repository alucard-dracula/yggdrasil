//proxy_netdata_def.hpp

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_NETDATA_DEF_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_NETDATA_DEF_HPP__

#include <yggr/ppex/range_checker.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_msg
{

struct proxy_netdata_def
{
	enum
	{
		E_id_start = 0x00008200,
		E_id_end = 0x00008300,

		E_compile_u32 = 0xffffffff
	};
};

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

#define YGGR_PP_CHECK_PROXY_MSG_ID( __id__ ) \
	YGGR_PP_IS_IN_RANGE(__id__, \
							::yggr::proxy::proxy_msg::proxy_netdata_def::E_id_start, \
							::yggr::proxy::proxy_msg::proxy_netdata_def::E_id_end )

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_NETDATA_DEF_HPP__