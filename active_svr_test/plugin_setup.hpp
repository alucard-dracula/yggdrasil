// plugin_setup.hpp

#ifndef __ACTIVE_SVR_TEST_PLUGIN_SETUP_HPP__
#define __ACTIVE_SVR_TEST_PLUGIN_SETUP_HPP__

#include "config.hpp"

void plugin_install_handler(runner_shared_info_ptr_type ptr1,
								single_container_type& scont,
								Enter_Handler ptr3);

void plugin_uninstall_handler(runner_shared_info_ptr_type ptr1,
								Leave_Handler ptr3);

#endif // __ACTIVE_SVR_TEST_PLUGIN_SETUP_HPP__