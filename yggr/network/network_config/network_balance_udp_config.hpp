//network_balance_udp_config.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_CONFIG_NETWORK_BALANCE_UDP_CONFIG_HPP__
#define __YGGR_NETWORK_NETWORK_CONFIG_NETWORK_BALANCE_UDP_CONFIG_HPP__

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/network/network_config/network_config_template.hpp>
#include <yggr/network/balance_io_service.hpp>
#include <yggr/network/type_traits/tags.hpp>

#include <boost/asio.hpp>

namespace yggr
{
namespace network
{
namespace network_config
{

#ifndef YGGR_TPL_CONFIG_USING_TPL 

YGGR_PP_NETWORK_CONFIG_TEMPLATE_DECL(
	balance_udpv4_config,
	yggr::network::type_traits::tag_udp,
	balance_io_service<boost::asio::io_service>, 
	boost::asio::ip::udp,  
	boost::asio::ip::address,
	boost::asio::deadline_timer,
	ip_ver::E_IPV4 );

YGGR_PP_NETWORK_CONFIG_TEMPLATE_DECL(
	balance_udpv6_config,
	yggr::network::type_traits::tag_udp, 
	balance_io_service<boost::asio::io_service>, 
	boost::asio::ip::udp,  
	boost::asio::ip::address,
	boost::asio::deadline_timer,
	ip_ver::E_IPV6 );


#else

typedef 
	yggr::network::network_config::network_config_template
	< 
		yggr::network::type_traits::tag_udp,
		balance_io_service<boost::asio::io_service>, 
		boost::asio::ip::udp,  
		boost::asio::ip::address,
		boost::asio::deadline_timer,
		ip_ver::E_IPV4
	> balance_udpv4_config;

typedef 
	yggr::network::network_config::network_config_template
	< 
		yggr::network::type_traits::tag_udp, 
		balance_io_service<boost::asio::io_service>, 
		boost::asio::ip::udp,  
		boost::asio::ip::address,
		boost::asio::deadline_timer,
		ip_ver::E_IPV6
	> balance_udpv6_config;

#endif // YGGR_TPL_CONFIG_USING_TPL

} // namespace network_config
} // namespace network_config
} // namespace yggr

#endif //__YGGR_NETWORK_NETWORK_CONFIG_NETWORK_BALANCE_UDP_CONFIG_HPP__