//p2p_peer_calculator.hpp

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

#ifndef __YGGR_P2P_CALCULATOR_P2P_PEER_CALCULATOR_HPP__
#define __YGGR_P2P_CALCULATOR_P2P_PEER_CALCULATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/network/network_protocol.hpp>

#include <yggr/p2p/protocol/p2p_packet_def.hpp>
#include <yggr/p2p/session_helper/p2p_peer_start_mode.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <cassert>

namespace yggr
{
namespace p2p
{

template< typename Client_Single,
			typename Owner_Container,
			typename P2P_Boot_Request_Back, 
			typename P2P_Boot_Start,
			typename P2P_Runtime_Info_Mgr
			>
class p2p_peer_calculator
	: public 
		boost::enable_shared_from_this
		<
			p2p_peer_calculator
			<
				Client_Single, 
				Owner_Container,
				P2P_Boot_Request_Back,
				P2P_Boot_Start,
				P2P_Runtime_Info_Mgr
			> 
		>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{

public:
	ERROR_MAKER_BEGIN("p2p_peer_calculator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef Client_Single client_single_type;
	typedef Owner_Container owner_info_container_type;
	typedef typename container_mapped_value_t<owner_info_container_type>::type owner_type;
	typedef typename owner_type::owner_id_type owner_id_type;

	typedef P2P_Boot_Request_Back p2p_boot_request_back_type;
	typedef P2P_Boot_Start p2p_boot_start_type;
	typedef P2P_Runtime_Info_Mgr p2p_runtime_info_mgr_type;

protected:
	typedef typename p2p_boot_request_back_type::peer_boot_info_type peer_boot_info_type;
	typedef typename peer_boot_info_type::socket_info_type socket_info_type;
	typedef typename p2p_runtime_info_mgr_type::p2p_session_creator_type p2p_session_creator_type;
	typedef p2p_peer_start_mode<peer_boot_info_type, p2p_session_creator_type> p2p_peer_start_mode_type;

public:
	typedef 
		network::network_protocol
		<
			p2p_boot_request_back_type, 
			yggr::task_center::default_task_data_info_type,
			p2p_packet_def::E_id_p2p_boot_request_back, 
			1
		> net_p2p_boot_request_back_type;
	
	typedef 
		network::network_protocol
		<
			p2p_boot_start_type, 
			yggr::task_center::default_task_data_info_type,
			p2p_packet_def::E_id_p2p_boot_start, 
			1
		> net_p2p_boot_start_type;

	typedef 
		network::network_protocol
		<
			socket_info_type,
			yggr::task_center::default_task_data_info_type,
			p2p_packet_def::E_id_p2p_nat_reduce, 
			1
		> net_p2p_nat_reduce_type;

private:
	typedef p2p_peer_calculator this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	p2p_peer_calculator(BOOST_FWD_REF(Args)... args)
		: _p2p_runtime_info_mgr(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) \
			BOOST_PP_EXPR_IF( __n__, > ) \
		p2p_peer_calculator( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: _p2p_runtime_info_mgr(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )) { }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~p2p_peer_calculator(void)
	{
	}

public:
	inline p2p_runtime_info_mgr_type& p2p_runtim_info_mgt(void)
	{
		return _p2p_runtime_info_mgr;
	}

	inline const p2p_runtime_info_mgr_type& p2p_runtim_info_mgt(void) const
	{
		return _p2p_runtime_info_mgr;
	}

protected:
	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename CtrlCenter>
	void register_dispatchers(CtrlCenter& cc)
	{
		CTRL_HANDLER(Tag, cc, boost::asio::error::invalid_argument,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::message_size,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::name_too_long,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::network_down,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_buffer_space,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_memory,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::shut_down,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::timed_out,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_reset,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_aborted,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::operation_aborted,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 2,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		if(boost::asio::error::connection_reset != 104) // windows not equal see boost asio/erron.h
		{
			CTRL_HANDLER(Tag, cc, 104,
									yggr::system_controller::system_code::E_SESSION_SYSTEM,
									boost::bind(&this_type::cal_network_dispath,
													this_type::shared_from_this(), _1));
		}

		CTRL_HANDLER(Tag, cc, 121,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 0x60009004,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 0x60020001,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, session_type::close_code_type::value,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this(), _1));
	}

public:
	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		at.template register_calculator<net_p2p_boot_request_back_type, recv_handler_type>(
				handler, boost::bind(&this_type::cal_p2p_boot_request_back<runner_type,
																			recv_handler_type>,
									this_type::shared_from_this(), _1, _2, _3, _4));

		at.template register_calculator<net_p2p_nat_reduce_type, recv_handler_type>(
				handler, boost::bind(&this_type::cal_p2p_nat_reduce<runner_type,
																	recv_handler_type>,
									this_type::shared_from_this(), _1, _2, _3, _4));
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		at.template unregister_calculator<net_p2p_boot_request_back_type>();
		at.template unregister_calculator<net_p2p_nat_reduce_type>();
	}

protected:
	template<typename Runner, typename Handler>
	void cal_p2p_boot_request_back(const owner_type& owner, 
									const net_p2p_boot_request_back_type& cdt, 
									Runner* prunner, 
									const Handler& handler)
	{
		typedef typename p2p_boot_request_back_type::peer_boot_info_cont_type peer_boot_info_cont_type;
		typedef typename p2p_boot_request_back_type::peer_boot_info_type peer_boot_info_type;

		owner_info_container_type owners;

		typename client_single_type::obj_ptr_type ptr = client_single_type::get_ins();
		if(!ptr)
		{
			return;
		}

		//// debug s
		//ptr->access(
		//	owner.owner_id(),
		//	boost::bind(
		//		&this_type::prv_debug_print_socket_info, this_type::shared_from_this(), _1, _2));
		//// debug e

		peer_boot_info_cont_type peer_boot_info_cont;
		net_p2p_boot_start_type net_boot_start;

		_p2p_runtime_info_mgr.fix_boot_request_back(owner, cdt.msg(), peer_boot_info_cont, net_boot_start.msg());
		
		if(boost::empty(peer_boot_info_cont))
		{
			return;
		}

		socket_info_type localhost_socket_info;
		this_type::pro_s_get_localhost_socket_info(localhost_socket_info, ptr, owner);

		this_type::pro_s_connect_and_update(owners, ptr, peer_boot_info_cont, localhost_socket_info, net_boot_start);

		if(!boost::empty(owners))
		{
			YGGR_PP_TASK_CENTER_BACK_TASK(
				handler, prunner, error_maker_type::make_error(0),
				yggr::task_center::runtime_task_type::E_CAL_RESULT,
				owners, net_boot_start);
		}
	}

	template<typename Runner, typename Handler>
	void cal_p2p_nat_reduce(const owner_type& owner, 
							const net_p2p_nat_reduce_type& cdt, 
							Runner* prunner, 
							const Handler& handler)
	{
		typename client_single_type::obj_ptr_type ptr = client_single_type::get_ins();
		if(!ptr)
		{
			return;
		}

		ptr->access(
			owner.owner_id(),
			boost::bind(
				&this_type::pro_s_handler_update_remote_socket_info, 
				_1, _2, boost::cref(cdt.msg())));
	}

	// test code // don't deleted
	//template<typename Owner, typename Runner, typename Handler>
	//void cal_p2p_nat_reduce(const Owner& owner, 
	//							const net_p2p_nat_reduce_type& cdt, 
	//							Runner* prunner, 
	//							const Handler& handler) const
	//{

	//	//test code
	//	static int n = 0;

	//	std::cout << "cal_p2p_nat_reduce" << std::endl;
	//	client_single_type::obj_ptr_type ptr = client_single_type::get_ins();
	//	if(!ptr)
	//	{
	//		return;
	//	}

	//	// debug s
	//	ptr->access(owner.owner_id(),
	//					boost::bind(&this_type::prv_debug_print_socket_info, 
	//									this_type::shared_from_this(), _1, _2));
	//	// debug e
	//
	//	// test code
	//	if(!n)
	//	{
	//		ptr->access(owner.owner_id(),
	//						boost::bind(&this_type::handler_update_remote_socket_info, 
	//										this_type::shared_from_this(), _1, _2, boost::cref(cdt.msg()))); 		
	//	}
	//	
	//	// test code
	//	if(n < 10)
	//	{
	//		++n;
	//		owner_info_container_type owners;
	//		iterator_ex::inserter(owners, owners.end()) = owner;
	//		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
	//										yggr::task_center::runtime_task_type::E_CAL_RESULT,
	//										owners, cdt);
	//	}
	//}

protected:
	void cal_network_dispath(const owner_id_type& owner_id)
	{
		_p2p_runtime_info_mgr.drop_peer(owner_id);
	}

protected:
	static socket_info_type& 
		pro_s_get_localhost_socket_info(socket_info_type& info, 
										typename client_single_type::obj_ptr_type pclt,
										const owner_type& owner)
	{
		assert(pclt);
		
		pclt->access(
			owner.owner_id(),
			boost::bind(
				&this_type::pro_s_handler_get_localhost_socket_info, 
				_1, _2, boost::ref(info)));

		return info;
	}

	static owner_info_container_type&
		pro_s_connect_and_update(owner_info_container_type& owners,
									typename client_single_type::obj_ptr_type pclt,
									const typename p2p_boot_request_back_type::peer_boot_info_cont_type& peer_boot_info_cont,
									const socket_info_type& localhost_socket_info,
									const net_p2p_boot_start_type& net_boot_start)
	{
		typedef typename p2p_boot_request_back_type::peer_boot_info_cont_type peer_boot_info_cont_type;
		typedef typename p2p_boot_request_back_type::peer_boot_info_type peer_boot_info_type;

		for(typename peer_boot_info_cont_type::const_iterator i = boost::begin(peer_boot_info_cont), isize = boost::end(peer_boot_info_cont);
				i != isize ; ++i)
		{
			const peer_boot_info_type& boot_info = yggr::mapped_value<peer_boot_info_cont_type>(*i);
			p2p_peer_start_mode_type start_mode(boot_info);

			if(boot_info.is_try_reduce_link_enable())
			{
				pclt->op_handler().p2p_connect(
					boot_info.dst_local_socket_info().host(), 
					boot_info.dst_local_socket_info().port(), 
					start_mode);

				net_p2p_nat_reduce_type net_nat_reduce(localhost_socket_info);
				
				pclt->access(
					boot_info.owner_id(),
					boost::bind(
						&this_type::pro_s_handler_send_nat_reduce,  
						_1, _2,
						owner_type(boot_info.owner_id()),
						boost::cref(net_nat_reduce),
						boost::cref(boot_info.dst_nat_socket_info()),
						boost::cref(net_boot_start)));
			}
			else
			{
				pclt->op_handler().p2p_connect(
					boot_info.dst_nat_socket_info().host(), 
					boot_info.dst_nat_socket_info().port(), 
					start_mode);

				iterator_ex::inserter(owners, boost::end(owners)) = owner_type(boot_info.owner_id());
			}
		}

		return owners;
	}

	static void pro_s_handler_get_localhost_socket_info(typename session_mgr_type::smap_type::base_type& base,
														typename session_mgr_type::smap_citer_type iter,
														socket_info_type& localhost_socket_info)
	{
		try
		{
			localhost_socket_info = iter->second->localhost_socket_info<socket_info_type>();
		}
		catch(const boost::system::error_code& e)
		{
			std::cerr << e.message() << std::endl;
		}
		catch(const yggr::stl_exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	static void pro_s_handler_send_nat_reduce(typename session_mgr_type::smap_type::base_type& base,
												typename session_mgr_type::smap_iter_type iter,
												const owner_type& owner, 
												const net_p2p_nat_reduce_type& msg,
												const socket_info_type& boot_remote_socket_info,
												const net_p2p_boot_start_type& boot_start)
	{
		assert(iter->second);
		if(!iter->second)
		{
			return;
		}

		iter->second->send_packet(
			owner, msg, 
			boost::bind(
				&this_type::pro_s_handler_send_nat_reduce_end,
				_1, owner, iter->second, boot_remote_socket_info, boot_start));

	}

	static void pro_s_handler_send_nat_reduce_end(const boost::system::error_code& e, 
													const owner_type& owner,
													typename session_mgr_type::session_ptr_type sptr,
													const socket_info_type& boot_remote_socket_info,
													const net_p2p_boot_start_type& boot_start)
	{
		assert(sptr);
		if(!sptr)
		{
			return;
		}

		sptr->update_remote_endpoint(boot_remote_socket_info.host(), boot_remote_socket_info.port());
		sptr->send_packet(owner, boot_start);
	}

	static void pro_s_handler_update_remote_socket_info(typename session_mgr_type::smap_type::base_type& base,
												typename session_mgr_type::smap_iter_type& iter,
												const socket_info_type& remote_socket_info)
	{
		assert(iter->second);
		if(!iter->second)
		{
			return;
		}
		
		iter->second->update_remote_endpoint(remote_socket_info.host(), remote_socket_info.port());
	}

	// test code don't delete
	//void prv_debug_print_socket_info(typename session_mgr_type::smap_type::base_type& base,
	//									typename session_mgr_type::smap_citer_type iter) const
	//{
	//	assert(iter->second);
	//	if(!iter->second)
	//	{
	//		return;
	//	}

	//	try
	//	{
	//		std::cout << "local_socket " << iter->second->str_local_socket_info() << std::endl;
	//		std::cout << "localhost_socket " << iter->second->str_localhost_socket_info() << std::endl;
	//		std::cout << "remote_socket " << iter->second->str_remote_socket_info() << std::endl;
	//	}
	//	catch(const yggr::stl_exception& e)
	//	{
	//		std::cerr << e.what() << std::endl;
	//	}
	//	catch(const boost::system::error_code& e)
	//	{
	//		std::cerr << e.message() << std::endl;
	//	}
	//}

protected:
	p2p_runtime_info_mgr_type _p2p_runtime_info_mgr;
};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_CALCULATOR_P2P_PEER_CALCULATOR_HPP__
