//p2p_peer_handler.hpp

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

#ifndef __YGGR_P2P_P2P_PEER_HANDLER_HPP__
#define __YGGR_P2P_P2P_PEER_HANDLER_HPP__

#include <yggr/ppex/friend.hpp>
#include <yggr/client/basic_clt_handler.hpp>
#include <yggr/p2p/session_helper/p2p_connect_handler.hpp>

namespace yggr
{
namespace p2p
{

template<
			typename ClientConfig,
			typename Service_Handler,
			typename SessionMgr,
			typename OptionsSetter = network::default_options_setter_type,
			template<typename _ServicePool,
						typename _SessionMgrWrap,
						typename _OptionsSetter,
						typename _Error_Maker>
				class Connect_Handler = p2p_connect_handler
		>
class p2p_peer_handler
	: public 
		client::basic_clt_handler
		<
			ClientConfig,
			Service_Handler,
			SessionMgr,
			OptionsSetter,
			Connect_Handler
		>
{
public:
	typedef ClientConfig client_config_type;
	typedef Service_Handler service_handler_type;
	typedef SessionMgr session_mgr_type;
	typedef OptionsSetter options_setter_type;

	typedef 
		client::basic_clt_handler
		<
			ClientConfig,
			Service_Handler,
			SessionMgr,
			OptionsSetter,
			Connect_Handler
		> base_type;

	typedef typename base_type::session_mgr_ptr_type session_mgr_ptr_type;
	typedef typename base_type::protocol_tag_type protocol_tag_type;
	typedef typename base_type::link_handler_init_type link_handler_init_type;
	typedef typename base_type::tag_type tag_type;
	typedef typename base_type::init_type init_type;
	typedef typename base_type::adapter_mgr_reg_def_type adapter_mgr_reg_def_type;

private:
	typedef p2p_peer_handler this_type;
	YGGR_PP_FRIEND_CLASS(network::network_handler<this_type>);

public:
	template<typename Init>
	p2p_peer_handler(service_handler_type& sh, const Init& init)
		: base_type(sh, init)
	{
	}

	~p2p_peer_handler(void)
	{
	}

public:
	template<typename Address, typename Port, typename Start_Mode> inline
	void p2p_connect(const Address& addr, const Port& port, Start_Mode& smode)
	{
		base_type::_connect_handler.p2p_connect(addr, port, smode);
	}

public:
	template<typename Address, typename Port, typename Owner, typename Data> inline
	void p2p_nat_reduce(const Address& addr, const Port& port, const Owner& owner, const Data& data)
	{
		base_type::_connect_handler.p2p_nat_reduce(addr, port, owner, data);
	}

public:
	using base_type::connect;
	using base_type::register_network_protocol;
	using base_type::register_network_protocol_of_container;
	using base_type::unregister_network_protocol;

	using base_type::run;
	using base_type::get_session_mgr;
	using base_type::stop;
	using base_type::online_size;
	using base_type::clear_sessions;
	using base_type::clear;
	using base_type::check_state;
	
	using base_type::send_packet;
	using base_type::access;
	using base_type::access_of_all;
};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_P2P_PEER_HANDLER_HPP__
