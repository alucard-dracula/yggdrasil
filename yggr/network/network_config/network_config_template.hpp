//network_config_template.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_CONFIG_NETWORK_CONFIG_TEMPLATE_HPP__
#define __YGGR_NETWORK_NETWORK_CONFIG_NETWORK_CONFIG_TEMPLATE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/network/network_config/network_direct_def.hpp>
#include <yggr/network/type_traits/socket_type_checker.hpp>

#include <boost/asio.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace network
{
namespace network_config
{

struct ip_ver
{
public:
	YGGR_STATIC_CONSTANT(u32, E_IPV4 = PF_INET);
	YGGR_STATIC_CONSTANT(u32, E_IPV6 = PF_INET6);

private:
	typedef ip_ver this_type;

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 v4(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return E_IPV4;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 v6(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return E_IPV6;
	}
};


// fix fatal error C1060 compiler is out of heap space
#define YGGR_PP_NETWORK_CONFIG_TEMPLATE_DECL(__name__, Protocol_Tag, Service, Protocol, Address, DeadLineTimer, V) \
	class __name__ { \
	public: \
		YGGR_STATIC_CONSTANT(u32, E_NOW_IP_VERSION = V); \
		\
		typedef Protocol_Tag protocol_tag_type; \
		typedef Service service_type; \
		typedef Protocol protocol_type; \
		typedef protocol_type::socket socket_type; \
		typedef protocol_type::endpoint endpoint_type; \
		typedef Address address_type; \
		typedef DeadLineTimer deadline_timer_type; \
		\
		typedef boost::mpl::bool_<(E_NOW_IP_VERSION == ip_ver::E_IPV4)> is_v4_type; \
		typedef boost::mpl::bool_<(E_NOW_IP_VERSION == ip_ver::E_IPV6)> is_v6_type; \
		\
	private: \
		typedef __name__ this_type; \
		\
	public: \
		YGGR_CONSTEXPR_OR_INLINE static bool is_v4(void) YGGR_NOEXCEPT_OR_NOTHROW { \
			return this_type::is_v4_type::value; } \
		YGGR_CONSTEXPR_OR_INLINE static bool is_v6(void) YGGR_NOEXCEPT_OR_NOTHROW { \
			return this_type::is_v6_type::value; } };

template<typename Protocol_Tag, 
			typename Service, 
			typename Protocol, 
			typename Address,
			typename DeadLineTimer,
			u32 V>
class network_config_template
{
public:
	YGGR_STATIC_CONSTANT(u32, E_NOW_IP_VERSION = V);
	
	typedef Protocol_Tag protocol_tag_type;
	typedef Service service_type;
	typedef Protocol protocol_type;
	typedef typename protocol_type::socket socket_type;
	typedef typename protocol_type::endpoint endpoint_type;
	typedef Address address_type;
	typedef DeadLineTimer deadline_timer_type;

	typedef boost::mpl::bool_<(E_NOW_IP_VERSION == ip_ver::E_IPV4)> is_v4_type;
	typedef boost::mpl::bool_<(E_NOW_IP_VERSION == ip_ver::E_IPV6)> is_v6_type;

private:
	typedef network_config_template this_type;

public:
	YGGR_CONSTEXPR_OR_INLINE static bool is_v4(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::is_v4_type::value;
	}

	YGGR_CONSTEXPR_OR_INLINE static bool is_v6(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return this_type::is_v6_type::value;
	}
};

//#endif // YGGR_TPL_CONFIG_USING_TPL

} // namespace network_config
} // namespace network
} // namespace yggr

YGGR_PP_SOCKET_TYPE_CHECKER_DEF(boost::asio::ip::tcp::socket, yggr::network::type_traits::tag_tcp)
YGGR_PP_SOCKET_TYPE_CHECKER_DEF(boost::asio::ip::udp::socket, yggr::network::type_traits::tag_udp)

#endif //__YGGR_NETWORK_NETWORK_CONFIG_NETWORK_CONFIG_TEMPLATE_HPP__