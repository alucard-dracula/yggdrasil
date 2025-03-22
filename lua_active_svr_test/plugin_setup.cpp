//plugin_setup.cpp

#include "plugin_setup.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void plugin_install_handler(runner_shared_info_ptr_type ptr1,
								single_container_type& scont,
								Enter_Handler ptr3)
{
	if(!ptr3)
	{
		return;
	}

	(*ptr3)(ptr1, scont);
}

void plugin_uninstall_handler(runner_shared_info_ptr_type ptr1,
								Leave_Handler ptr3)
{
	if(!ptr3)
	{
		return;
	}

	(*ptr3)(ptr1);
}
