//strand_connection.hpp

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

#ifndef __YGGR_NETWORK_CONNECTION_HPP__
#define __YGGR_NETWORK_CONNECTION_HPP__

#include <boost/enable_shared_from_this.hpp>
#include <boost/atomic.hpp>

#include <yggr/network/detail/tcp_async_strand_io.hpp>
#include <yggr/network/detail/udp_async_strand_io.hpp>

#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace network
{

template< typename Linker, typename OPacket, typename IPacket,
			template<typename _Linker, typename _OPacket, typename _IPacket>
				class NetworkIO = yggr::network::detail::tcp_async_strand_io >
class connection
	: public NetworkIO<Linker, OPacket, IPacket>
{
public:

	typedef Linker linker_type;
	typedef OPacket opacket_type;
	typedef IPacket ipacket_type;

	typedef NetworkIO<linker_type, opacket_type, ipacket_type> base_type;

	typedef typename base_type::service_type service_type;
	typedef typename base_type::link_init_type link_init_type;
	typedef typename base_type::link_type link_type;
	typedef typename base_type::link_address_type link_address_type;
	typedef typename base_type::endpoint_type endpoint_type;
	typedef typename base_type::protocol_type protocol_type;
	typedef typename base_type::protocol_tag_type protocol_tag_type;

	typedef typename base_type::opacket_type send_data_type;
	typedef typename base_type::ipacket_type recv_data_type;

private:

	typedef boost::atomic<bool> gate_type;
	typedef connection this_type;

public:

#define BOOST_PP_LOCAL_MACRO(__n__) \
	template<YGGR_PP_FOO_TYPES_DEF(__n__)> \
	explicit connection( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_REF_PARAMS ) ) \
		: base_type(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)), \
			_gate(true) {}

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

	~connection(void)
	{
	}

	//-----------------send s----------------------------------------

	template<typename NetInfo, typename Data, typename Handler>
	void send_packet(const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		if(!_gate.load())
		{
			return;
		}
		send_data_type pak(netinfo, data);
		base_type::send_packet(pak, handler);
	}

	void next_send(void)
	{
		if(!_gate.load())
		{
			return;
		}
		base_type::next_send();
	}

	template<typename Handler>
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

	template<typename Pak, typename Tuple_Handler>
	void recv_packet(const Tuple_Handler& handler)
	{
		if(!_gate.load())
		{
			return;
		}
		base_type::template recv_packet<Pak>(handler);
	}

	//------------------recv e---------------------------------------

	void close(void)
	{
		//bool b = true;
		_gate.store(false);
		base_type::close();
	}

private:
	gate_type _gate;
};

} //network
} //yggr
#endif //__YGGR_NETWORK_CONNECTION_HPP__
