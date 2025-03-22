//strand_connection.hpp

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

#ifndef __YGGR_NETWORK_CONNECTION_HPP__
#define __YGGR_NETWORK_CONNECTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/io/tcp_async_strand_io.hpp>
#include <yggr/network/io/udp_async_strand_io.hpp>

#include <yggr/ppex/foo_params.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/atomic.hpp>

namespace yggr
{
namespace network
{

template< typename Linker, typename OPacket, typename IPacket,
			template<typename _Linker, typename _OPacket, typename _IPacket>
				class NetworkIO = yggr::network::io::tcp_async_strand_io >
class connection
	: public NetworkIO<Linker, OPacket, IPacket>
{
public:

	typedef Linker linker_type;
	typedef OPacket opacket_type;
	typedef IPacket ipacket_type;

	typedef NetworkIO<linker_type, opacket_type, ipacket_type> base_type;

	typedef typename base_type::protocol_tag_type protocol_tag_type;
	typedef typename base_type::service_type service_type;
	typedef typename base_type::protocol_type protocol_type;
	typedef typename base_type::endpoint_type endpoint_type;
	typedef typename base_type::link_address_type link_address_type;
	typedef typename base_type::deadline_timer_type deadline_timer_type;

	typedef typename base_type::link_init_type link_init_type;
	typedef typename base_type::link_type link_type;

	typedef typename base_type::opacket_type send_data_type;
	typedef typename base_type::ipacket_type recv_data_type;

private:
	typedef boost::atomic<bool> gate_type;

private:
	typedef connection this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template<typename ...Args>
	explicit connection(link_init_type& link_init, BOOST_FWD_REF(Args)... args)
		: base_type(link_init, boost::forward<Args>(args)...),
			_gate(true)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, > ) \
		explicit connection( link_init_type& link_init \
								YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: base_type(link_init \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)), \
				_gate(true) {}

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~connection(void)
	{
	}

public:
	//-----------------send s----------------------------------------

	template<typename NetInfo, typename Data, typename Handler> inline
	void send_packet(const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		if(!_gate.load())
		{
			return;
		}
		send_data_type pak(netinfo, data);
		base_type::send_packet(pak, handler);
	}

	inline void next_send(void)
	{
		if(!_gate.load())
		{
			return;
		}
		base_type::next_send();
	}

	template<typename Handler> inline
	void send_packet(send_data_type& pak, const Handler& handler)
	{
		if(!_gate.load())
		{
			return;
		}
		base_type::send_packet(pak, handler);
	}

	//------------------send e---------------------------------------

	//------------------recv s---------------------------------------

	template<typename Pak, typename Tuple_Handler> inline
	void recv_packet(const Tuple_Handler& handler)
	{
		if(!_gate.load())
		{
			return;
		}
		base_type::template recv_packet<Pak>(handler);
	}

	//------------------recv e---------------------------------------

	inline void close(void)
	{
		_gate.store(false);
		base_type::close();
	}

private:
	gate_type _gate;
};

} //network
} //yggr
#endif //__YGGR_NETWORK_CONNECTION_HPP__
