//p2p_connect_handler

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

#ifndef __YGGR_P2P_SESSION_HELPER_P2P_CONNECT_HANDLER_HPP__
#define __YGGR_P2P_SESSION_HELPER_P2P_CONNECT_HANDLER_HPP__

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/network/session_helper/udp_connect_handler.hpp>
#include <yggr/network/address_helper.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/mpl/if.hpp>

namespace yggr
{
namespace p2p
{

template<typename ServicePool,
			typename SessionMgrWrap,
			typename OptionsSetter,
			typename Error_Maker>
class p2p_connect_handler
	: public
		network::session_helper::udp_connect_handler
		<
			ServicePool,
			SessionMgrWrap,
			OptionsSetter,
			Error_Maker
		>
{
public:
	typedef
		network::session_helper::udp_connect_handler
		<
			ServicePool,
			SessionMgrWrap,
			OptionsSetter,
			Error_Maker
		> base_type;

public:
	typedef typename base_type::error_maker_type error_maker_type;
	typedef typename base_type::error_type error_type;

protected:
	typedef typename base_type::socket_ptr_type socket_ptr_type;
	typedef typename base_type::conn_ptr_type conn_ptr_type;
	typedef typename base_type::session_mgr_ptr_type session_mgr_ptr_type;
	typedef typename base_type::session_mgr_type session_mgr_type;
	typedef typename base_type::options_setter_type options_setter_type;

	typedef typename base_type::resolver_type resolver_type;
	typedef typename base_type::resolver_query_type resolver_query_type;
	typedef typename base_type::resolver_iter_type resolver_iter_type;
	typedef typename base_type::endpoint_type endpoint_type;

	typedef network::address_helper address_helper_type;
	typedef address_helper_type::address_type address_type;

private:
	typedef p2p_connect_handler this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> explicit
	p2p_connect_handler(BOOST_FWD_REF(Args)... args)
		: base_type(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) \
			BOOST_PP_EXPR_IF( __n__, > ) \
		explicit p2p_connect_handler(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) \
			: base_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)) {}

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~p2p_connect_handler(void)
	{
	}

public:
	template<typename Address, typename Port, typename Start_Mode> inline
	void p2p_connect(const Address& addr, const Port& port, Start_Mode& start_mode)
	{
		typedef typename base_type::conn_type now_conn_type;

		assert(base_type::_pudp_socket_pool);
		socket_ptr_type psocket =
			base_type::_pudp_socket_pool->get(
				base_type::_service_pool.get_service(),
				base_type::pro_s_options_setter());

		assert(psocket);
		if(!psocket)
		{
			return;
		}

		conn_ptr_type pconn =
			construct_shared<now_conn_type>(
				yggr_nothrow_new now_conn_type(*psocket));

		if(!pconn)
		{
			base_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);

			return;
		}

		this_type::pro_s_init_connection(pconn, addr, port);
		session_mgr_ptr_type psmgr = base_type::_smgr_wrap.get_shared_ptr();
		psmgr->insert(pconn, start_mode);

		base_type::recv(psocket);
	}

	template<typename Address, typename Port> inline
	bool p2p_reset_remote_endpoint(const Address& addr, const Port& port)
	{

		address_type now_addr(address_helper_type::address_from_string(addr));
		endpoint_type edpt;
		edpt.address(now_addr);

		{
			u16 now_port = 0;
			std::stringstream ss;
			ss << port;
			ss >> now_port;
			edpt.port(now_port);
		}

		session_mgr_ptr_type psmgr = base_type::_smgr_wrap.get_shared_ptr();
		return
			psmgr->accesse(
				boost::bind(
					&this_type::pro_s_handler_reset_remote_endpoint, _1, boost::cref(edpt)));
	}

protected:
	template<typename Address, typename Port> inline
	static void pro_s_init_connection(conn_ptr_type pconn,
										const Address& addr,
										const Port& port)
	{
		typedef Address address_type;
		typedef Port port_type;

		if(!pconn)
		{
			return;
		}

		resolver_type rslvr(pconn->get_service());
		resolver_query_type query(addr, port);
		resolver_iter_type edpt_iter = rslvr.resolve(query);
		endpoint_type edpt = *edpt_iter;

		pconn->connect(edpt);
	}

	inline static bool pro_s_handler_reset_remote_endpoint(typename session_mgr_type::smap_iter_type& iter,
															const endpoint_type& ep)
	{
		return iter->second && iter->second->update_remote_endpint(ep);
	}
};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_SESSION_HELPER_P2P_CONNECT_HANDLER_HPP__

