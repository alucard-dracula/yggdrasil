//init_proxy.cpp

#include "init_proxy.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

proxy_repeater_ptr_type create_proxy_repeater(proxy_runner_shared_info_ptr_type proxy_shared_info_ptr)
{
	proxy_router_ptr_type prouter(new proxy_router_type());
	proxy_args_builder_ptr_type pargs_build(new proxy_args_builder_type());
	proxy_repeater_ptr_type prepeater(
		new proxy_repeater_type(proxy_shared_info_ptr, prouter, pargs_build));

	return prepeater;
}
