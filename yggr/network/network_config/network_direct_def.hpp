//network_direct_def.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

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

#ifndef __YGGR_NETWORK_NETWORK_DIRECT_DEF_HPP__
#define __YGGR_NETWORK_NETWORK_DIRECT_DEF_HPP__

#include <yggr/base/static_constant.hpp>

#include <yggr/adapter/adapter_register_type_def.hpp>

namespace yggr
{
namespace network
{
namespace network_config
{

struct network_direct_def
{
	YGGR_STATIC_CONSTANT(u32, E_send_enable =  adapter::adapter_register_type_def::E_from_src);
	YGGR_STATIC_CONSTANT(u32, E_recv_enable =  adapter::adapter_register_type_def::E_to_src);
	YGGR_STATIC_CONSTANT(u32, E_both_enable =  adapter::adapter_register_type_def::E_both);
};

} // namespace network_config

typedef network_config::network_direct_def network_direct_def_type;

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_NETWORK_DIRECT_DEF_HPP__