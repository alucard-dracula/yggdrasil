//sync_default_config.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_SYNC_SYSTEM_SYNC_DEFAULT_CONFIG__
#define __YGGR_SYNC_SYSTEM_SYNC_DEFAULT_CONFIG__

#include <yggr/any_val/base_any_type_operator.hpp>
#include <yggr/any_val/any_val_operator.hpp>
#include <yggr/any_val/params.hpp>
#include <yggr/sync_system/sync_helper.hpp>
#include <yggr/sync_system/sync_container_mgr.hpp>
#include <yggr/charset/string.hpp>

namespace yggr
{
namespace sync_system
{

typedef any_val::any_val_operator<any_val::base_any_type_operator> any_val_operator_type;
typedef any_val::params<std::string, any_val_operator_type> default_params_type;

typedef sync_helper<default_params_type> default_sync_helper_type;

typedef sync_container_mgr<std::string, default_params_type> default_container_mgr_type;

}
} // namespace yggr

#endif //__YGGR_SYNC_SYSTEM_SYNC_DEFAULT_CONFIG__