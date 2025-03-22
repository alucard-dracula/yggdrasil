//client_tcp_config.hpp

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

#ifndef __YGGR_CLIENT_CLIENT_CONFIG_CLIENT_TCP_CONFIG_HPP__
#define __YGGR_CLIENT_CLIENT_CONFIG_CLIENT_TCP_CONFIG_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/client/client_config/client_config_template.hpp>

#include <boost/asio.hpp>

namespace yggr
{
namespace client
{
namespace client_config
{

#ifndef YGGR_TPL_CONFIG_USING_TPL 

YGGR_PP_CLIENT_CONFIG_TEMPLATE_DECL(
	client_tcp_config_type,
	yggr::network::type_traits::tag_client );

#else

typedef 
	yggr::client::client_config::client_config_template
	<
		yggr::network::type_traits::tag_client
	> client_tcp_config_type;

#endif // YGGR_TPL_CONFIG_USING_TPL

} // namespace client_config
} // namespace client
} // namespace yggr

#endif //__YGGR_CLIENT_CLIENT_CONFIG_CLIENT_TCP_CONFIG_HPP__