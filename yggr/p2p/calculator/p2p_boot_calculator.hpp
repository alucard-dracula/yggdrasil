//p2p_boot_calcuator.hpp

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

#ifndef __YGGR_P2P_CALCULATOR_P2P_BOOT_CALCULATOR_HPP__
#define __YGGR_P2P_CALCULATOR_P2P_BOOT_CALCULATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/container/mapped_value.hpp>

#include <yggr/network/network_protocol.hpp>
#include <yggr/task_center/task_info_def.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/p2p/protocol/p2p_packet_def.hpp>
#include <yggr/p2p/session_helper/p2p_peer_start_mode.hpp>
#include <yggr/p2p/p2p_peer_mapping.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

namespace yggr
{
namespace p2p
{

template< typename Server_Single,
			typename Session_Mgr,
			typename Owner_Container,
			typename P2P_Boot_Request,
			typename P2P_Boot_Request_Back,
			typename P2P_Runtime_Info_Mgr
			>
struct p2p_boot_calcuator
	: public boost::enable_shared_from_this
				<
					p2p_boot_calcuator
					<
						Server_Single,
						Session_Mgr,
						Owner_Container,
						P2P_Boot_Request,
						P2P_Boot_Request_Back,
						P2P_Runtime_Info_Mgr
					> 
				>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	ERROR_MAKER_BEGIN("p2p_boot_calcuator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef Server_Single server_single_type;
	typedef Session_Mgr session_mgr_type;

	typedef Owner_Container owner_info_container_type;
	typedef typename container_mapped_value_t<owner_info_container_type>::type owner_type;
	typedef typename owner_type::owner_id_type owner_id_type;

	typedef P2P_Boot_Request p2p_boot_request_type;
	typedef P2P_Boot_Request_Back p2p_boot_request_back_type;

	typedef typename p2p_boot_request_type::socket_info_type socket_info_type;
	
	typedef P2P_Runtime_Info_Mgr p2p_runtime_info_mgr_type;
	typedef typename p2p_runtime_info_mgr_type::p2p_boot_request_back_map_type p2p_boot_request_back_map_type;

	typedef 
		network::network_protocol
		<
			p2p_boot_request_type, 
			yggr::task_center::default_task_data_info_type,
			p2p_packet_def::E_id_p2p_boot_request, 
			1
		> net_p2p_boot_request_type;

	typedef 
		network::network_protocol
		<
			p2p_boot_request_back_type, 
			yggr::task_center::default_task_data_info_type,
			p2p_packet_def::E_id_p2p_boot_request_back, 
			1
		> net_p2p_boot_request_back_type;

private:
	typedef p2p_boot_calcuator this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	p2p_boot_calcuator(BOOST_FWD_REF(Args)... args)
		: _p2p_runtime_info_mgr(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		p2p_boot_calcuator( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: _p2p_runtime_info_mgr(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )) { }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~p2p_boot_calcuator(void)
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
												this_type::shared_from_this(), _1));

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
	}

public:
	template<typename Runner, typename Action_Table, typename Recv_Handler> 
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		at.template register_calculator<net_p2p_boot_request_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_p2p_boot_request<runner_type,
																		recv_handler_type>,
										this_type::shared_from_this(), _1, _2, _3, _4));
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		at.template unregister_calculator<net_p2p_boot_request_type>();
	}

protected:
	template<typename Runner, typename Handler>
	void cal_p2p_boot_request(const owner_type& owner, 
								const net_p2p_boot_request_type& cdt, 
								Runner* prunner, 
								const Handler& handler)
	{
		typedef typename server_single_type::obj_ptr_type svr_ptr_type;

		svr_ptr_type psvr = server_single_type::get_ins();
		if(!psvr)
		{
			return;
		}

		//// test code don't deleted
		//// debug s
		//psvr->access(
		//	owner.owner_id(),
		//	boost::bind(
		//	&this_type::prv_debug_print_socket_info, this_type::shared_from_this(), _1, _2));
		//// debug e

		socket_info_type src_nat_socket_info;

		if(!(psvr->access(
				owner.owner_id(),
				boost::bind(
					&this_type::prv_handler_get_nat_socket_info, 
					this_type::shared_from_this(), _1, _2, boost::ref(src_nat_socket_info)))))
		{
			return;
		}

		if(src_nat_socket_info == socket_info_type())
		{
			return;
		}

		owner_info_container_type owners_now;
		p2p_boot_request_back_map_type owners_others;

		net_p2p_boot_request_back_type net_boot_request_back_now;

		_p2p_runtime_info_mgr.fix_boot_request(owner, 
												src_nat_socket_info, 
												cdt.msg(),
												owners_now, 
												net_boot_request_back_now.msg(), 
												owners_others );

		
		if(!(boost::empty(owners_now) || net_boot_request_back_now.msg().empty()))
		{
			YGGR_PP_TASK_CENTER_BACK_TASK(
				handler, prunner, error_maker_type::make_error(0),
				task_center::runtime_task_type::E_CAL_RESULT,
				owners_now, net_boot_request_back_now);
		}

		if(!boost::empty(owners_others))
		{
			for(typename p2p_boot_request_back_map_type::iterator i = boost::begin(owners_others), 
					isize = boost::end(owners_others); i != isize; ++i)
			{
				owner_info_container_type owners;
				iterator_ex::inserter(owners, boost::end(owners)) = i->first;
				YGGR_PP_TASK_CENTER_BACK_TASK(
					handler, prunner, error_maker_type::make_error(0),
					task_center::runtime_task_type::E_CAL_RESULT,
					owners, net_p2p_boot_request_back_type(boost::move(i->second)));
			}
		}
	}

	void cal_network_dispath(const owner_id_type& owner_id)
	{
		_p2p_runtime_info_mgr.drop_peer(owner_id);
	}

private:
	void prv_handler_get_nat_socket_info( typename session_mgr_type::smap_type::base_type& base,
											typename session_mgr_type::smap_citer_type iter,
											socket_info_type& socket_info) const
	{
		assert(iter->second);
		if(!iter->second)
		{
			return;
		}

		try
		{
			socket_info_type tmp(iter->second->str_remote_address(),
									iter->second->str_remote_port() );
			yggr::swap(socket_info, tmp);
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

	//// test code don't deleted
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

#endif // __YGGR_P2P_CALCULATOR_P2P_BOOT_CALCULATOR_HPP__

