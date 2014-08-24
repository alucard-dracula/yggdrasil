//network_config_template.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_CONFIG_NETWORK_CONFIG_TEMPLATE_HPP__
#define __YGGR_NETWORK_NETWORK_CONFIG_NETWORK_CONFIG_TEMPLATE_HPP__

#include <boost/asio.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/network/network_config/network_direct_def.hpp>

#include <yggr/network/type_traits/socket_type_checker.hpp>

namespace yggr
{
namespace network
{
namespace network_config
{

struct ip_ver
{
public:
	enum
	{
		E_IPV4 = PF_INET,
		E_IPV6 = PF_INET6,
		E_compile_u32 = 0xffffffff
	};

	static const u32 v4(void)
	{
		return E_IPV4;
	}

	static const u32 v6(void)
	{
		return E_IPV6;
	}
};

template<typename Protocol_Tag, 
			typename Service, 
			typename Protocol, 
			typename Address,
			u32 V>
class network_config_template
{
public:

	enum
	{
		E_NOW_IP_VERSION = V,
		E_compile_u32 = 0xffffffff
	};

	typedef Protocol_Tag protocol_tag_type;
	typedef Service service_type;
	typedef Protocol protocol_type;
	typedef typename protocol_type::socket socket_type;
	typedef typename protocol_type::endpoint endpoint_type;
	typedef Address address_type;

	static bool is_v4(void)
	{
		return E_NOW_IP_VERSION == ip_ver::v4();
	}

	static bool is_v6(void)
	{
		return E_NOW_IP_VERSION == ip_ver::v6();
	}
};

} // namespace network_config
} // namespace network
} // namespace yggr

YGGR_PP_SOCKET_TYPE_CHECKER_DEF(boost::asio::ip::tcp::socket, yggr::network::type_traits::tag_tcp)
YGGR_PP_SOCKET_TYPE_CHECKER_DEF(boost::asio::ip::udp::socket, yggr::network::type_traits::tag_udp)

#endif //__YGGR_NETWORK_NETWORK_CONFIG_NETWORK_CONFIG_TEMPLATE_HPP__