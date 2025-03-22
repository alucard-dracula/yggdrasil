//config.hpp

#ifndef __LUA_ACTIVE_SVR_TEST_CONFIG_HPP__
#define __LUA_ACTIVE_SVR_TEST_CONFIG_HPP__

#include <lua_calculator_impl.hpp>
#include <yggr/base/random.h>

#include <yggr/modules/modules_mgr.hpp>
#include <yggr/plugins/plugins_installer.hpp>

#include <iostream>

typedef yggr::ids::inner_process_id inner_process_id_type;
typedef yggr::ids::id_generator<inner_process_id_type> inner_process_id_gen_type;
typedef yggr::ptr_single<inner_process_id_type> inner_process_id_single_type;

typedef yggr::plugins::plugins_installer<yggr::modules::modules_mgr> plugins_install_type;
typedef yggr::ptr_single<plugins_install_type> plugins_install_single_type;

typedef yggr::modules::modules_accesser::error_type modules_error_type;

typedef yggr::ptr_single<task_center_type> task_center_single_type;

#endif // __LUA_ACTIVE_SVR_TEST_CONFIG_HPP__