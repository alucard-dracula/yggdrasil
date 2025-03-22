//user_calculator.hpp

#ifndef __USER_CALCULATOR_HPP__
#define __USER_CALCULATOR_HPP__

#include <iterator>

#include "p2p_peer_def.hpp"
#include "p2p_peer_calculator_def.hpp"

struct user_calculator
	: public boost::enable_shared_from_this<user_calculator>
{
private:
	typedef user_calculator this_type;

public:
	ERROR_MAKE_NON("user_calculator")

	typedef network_info_type owner_info_type;
	typedef ::owner_info_container_type owner_info_container_type;

	typedef
		yggr::network::network_protocol
		<
			p2p_boot_request_type, 
			yggr::task_center::default_task_data_info_type,
			yggr::p2p::p2p_packet_def::E_id_p2p_boot_request,
			1
		> net_p2p_boot_request_type;

	typedef p2p_peer_calculator_type::net_p2p_boot_start_type net_p2p_boot_start_type;

	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		at.template register_calculator<test_pak_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_start_pak<runner_type,
																recv_handler_type>,
									this_type::shared_from_this(), _1, _2, _3, _4));

		at.template register_calculator<net_p2p_boot_start_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_p2p_boot_start<runner_type,
																	recv_handler_type>,
									this_type::shared_from_this(), _1, _2, _3, _4));
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		at.template unregister_calculator<test_pak_type>();
		at.template unregister_calculator<net_p2p_boot_start_type>();
	}


private:
	template<typename Runner, typename Handler>
	void cal_start_pak(const owner_info_type& owner, const test_pak_type& cdt, Runner* prunner, const Handler& handler)
	{
		std::cout << "cal_start_pak" << std::endl;
		clt_ptr_type pclt = clt_ptr_single_type::get_ins();
		if(!pclt)
		{
			return;
		}

		pclt->access(owner.owner_id(),
							boost::bind(&this_type::print_socket_info, 
											this_type::shared_from_this(), _1, _2));

		socket_info_type localhost_socket_info;

		if(!(pclt->access(owner.owner_id(),
							boost::bind(&this_type::prv_handler_get_localhost_socket_info, 
											this_type::shared_from_this(), _1, _2, boost::ref(localhost_socket_info)))))
		{
			return;
		}

		if(localhost_socket_info == socket_info_type())
		{
			return;
		}

		owner_info_container_type owners;
		std::inserter(owners, boost::end(owners)) = owner;

		net_p2p_boot_request_type back_msg;
		p2p_boot_request_type p2p_boot_request(boost::move(localhost_socket_info));
		back_msg.msg().swap(p2p_boot_request);

		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
										yggr::task_center::runtime_task_type::E_CAL_RESULT,
										owners, back_msg);
	}

	template<typename Runner, typename Handler>
	void cal_p2p_boot_start(const owner_info_type& owner, const net_p2p_boot_start_type& cdt, Runner* prunner, const Handler& handler)
	{
		std::cout << "cal_p2p_boot_start" << std::endl;
		clt_ptr_type pclt = clt_ptr_single_type::get_ins();
		if(!pclt)
		{
			return;
		}

		socket_info_type remote_socket_info;

		if(!(pclt->access(owner.owner_id(),
							boost::bind(&this_type::prv_handler_get_remote_socket_info, 
											this_type::shared_from_this(), _1, _2, boost::ref(remote_socket_info)))))
		{
			return;
		}

		if(remote_socket_info == socket_info_type())
		{
			return;
		}

		std::cout << "hello world " << remote_socket_info.host() << ":" << remote_socket_info.port() << std::endl;
	}

	void prv_handler_get_localhost_socket_info(const session_mgr_type::smap_type::base_type& base,
												session_mgr_type::smap_citer_type iter,
												socket_info_type& back_msg) const;

	void prv_handler_get_remote_socket_info(const session_mgr_type::smap_type::base_type& base,
											session_mgr_type::smap_citer_type iter,
											socket_info_type& back_msg) const;

	void print_socket_info(const session_mgr_type::smap_type::base_type& base,
							session_mgr_type::smap_citer_type iter) const;

};

#endif // __USER_CALCULATOR_HPP__
