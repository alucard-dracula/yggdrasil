//basic_proxy_msg_fixer.hpp

#ifndef __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_MSG_FIXER_HPP__
#define __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_MSG_FIXER_HPP__

#include <boost/smart_ptr/enable_shared_from_raw.hpp>
#include <boost/unordered_set.hpp>

#include <yggr/proxy/proxy_fix_state_def.hpp>
#include <yggr/mplex/get_container_value_t.hpp>

#include <yggr/proxy/proxy_msg/proxy_register_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_register_back_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_mode_change_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_mode_change_back_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_unregister_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_unregister_back_msg.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_ctrl
{

template<typename OwnerInfoContainer,
			typename ProxyRepeater,
			typename NetInPacket,
			typename NetOutPacket,
			typename Server_Ptr_Single
			>
struct basic_proxy_msg_fixer
	: public boost::enable_shared_from_raw //真正的数据处理部分
{
public:
	ERROR_MAKER_BEGIN("basic_proxy_msg_fixer")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef OwnerInfoContainer owner_info_container_type;
	typedef ProxyRepeater proxy_repeater_type;
	typedef NetInPacket net_ipak_type;
	typedef NetOutPacket net_opak_type;
	typedef proxy_fix_state_def proxy_fix_state_def_type;

	typedef typename mplex::get_container_value_t<owner_info_container_type>::type owner_info_type;

	typedef typename proxy_repeater_type::router_type proxy_router_type;

	typedef yggr::safe_container::safe_wrap<proxy_repeater_type> proxy_repeater_wrap_type;
	typedef typename proxy_repeater_wrap_type::value_ptr_type proxy_repeater_ptr_type;

	typedef yggr::safe_container::safe_wrap<proxy_router_type> proxy_router_wrap_type;
	typedef typename proxy_router_wrap_type::value_ptr_type proxy_router_ptr_type;

	typedef yggr::proxy::proxy_msg::proxy_register_msg<> proxy_register_msg_type;
	typedef yggr::proxy::proxy_msg::proxy_register_back_msg<> proxy_register_back_msg_type;
	typedef yggr::proxy::proxy_msg::proxy_mode_change_msg<> proxy_mode_change_msg_type;
	typedef yggr::proxy::proxy_msg::proxy_mode_change_back_msg<> proxy_mode_change_back_msg_type;
	typedef yggr::proxy::proxy_msg::proxy_unregister_msg<> proxy_unregister_msg_type;
	typedef yggr::proxy::proxy_msg::proxy_unregister_back_msg<> proxy_unregister_back_msg_type;

protected:
	typedef typename proxy_router_type::dispatch_id_type dispatch_id_type;
	typedef typename proxy_router_type::proxy_container_ptr_type proxy_container_ptr_type;
	typedef typename proxy_router_type::proxy_val_type proxy_val_type;
	typedef Server_Ptr_Single server_ptr_single_type;
	typedef basic_proxy_msg_fixer this_type;

protected:
	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename Ctrl_Center>
	void register_dispatchers(Ctrl_Center& cc)
	{
		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::invalid_argument,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::message_size,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::name_too_long,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::network_down,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::no_buffer_space,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::no_memory,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::shut_down,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::timed_out,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::connection_reset,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::connection_aborted,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::operation_aborted,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 2,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 104,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 121,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 0x60009004,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 0x60020001,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));
	}

public:

	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;


		at.template register_calculator_of_container<
														proxy_register_msg_type,
														net_ipak_type,
														Recv_Handler>(
				handler, boost::bind(&this_type::cal_proxy_register_msg_type<
																	owner_info_type,
																	runner_type,
																	recv_handler_type>,
									boost::shared_from_raw(this), _1, _2, _3, _4));

		at.template register_calculator_of_container<
														proxy_mode_change_msg_type,
														net_ipak_type,
														Recv_Handler>(
				handler, boost::bind(&this_type::cal_proxy_mode_change_msg_type<
																	owner_info_type,
																	runner_type,
																	recv_handler_type>,
									boost::shared_from_raw(this), _1, _2, _3, _4));

		at.template register_calculator_of_container<
														proxy_unregister_msg_type,
														net_ipak_type,
														Recv_Handler>(
				handler, boost::bind(&this_type::cal_proxy_unregister_msg_type<
																	owner_info_type,
																	runner_type,
																	recv_handler_type>,
									boost::shared_from_raw(this), _1, _2, _3, _4));
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		at.template unregister_calculator<proxy_register_msg_type>();
		at.template unregister_calculator<proxy_mode_change_msg_type>();
		at.template unregister_calculator<proxy_unregister_msg_type>();
	}

public:
	basic_proxy_msg_fixer(const proxy_repeater_ptr_type& prepeater,
							const proxy_router_ptr_type& prouter)
		:  _repeater_wrap(prepeater), _router_wrap(prouter)
	{
	}

	~basic_proxy_msg_fixer(void)
	{
	}

	template<typename Owner, typename Runner, typename Handler>
	void cal_proxy_register_msg_type(const Owner& owner,
										const net_ipak_type& cdt,
										Runner* prunner,
										const Handler& handler)
	{
		typedef proxy_register_msg_type::reg_map_type reg_map_type;
		typedef reg_map_type::const_iterator reg_map_citer_type;

		proxy_register_msg_type reg_msg;

		bool rst = cdt.load(reg_msg);
		if(!rst)
		{
			return;
		}

		typename server_ptr_single_type::obj_ptr_type psvr = server_ptr_single_type::get_ins();
		proxy_router_ptr_type prouter = _router_wrap.get_shared_ptr();
		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();

		if(!(psvr && prouter && prepeater))
		{
			return;
		}

		proxy_register_back_msg_type back_msg;
		const reg_map_type& cref_reg_map = reg_msg.reg_map();
		owner_info_container_type chg_owners;
		for(reg_map_citer_type i = cref_reg_map.begin(), isize = cref_reg_map.end();
				i != isize; ++i)
		{
			std::pair<yggr::u32, yggr::u32> dis_rst
				= prouter->register_dispatch(
												i->first,
												i->second.first,
												owner.owner_id(),
												boost::bind(
																&this_type::handler_get_proxy_list_of_register,
																this, _1, boost::ref(chg_owners), owner.owner_id()
															)
											);

			if(!chg_owners.empty())
			{
				assert((dis_rst.second == proxy_fix_state_def_type::E_changed));
				net_opak_type opak(owner, proxy_mode_change_back_msg_type(i->first, dis_rst.first));
				net_opak_type& ref_data = opak;
				handler(prunner, error_maker_type::make_error(0),
							yggr::task_center::runtime_task_type::E_CAL_RESULT,
							chg_owners, proxy_mode_change_back_msg_type::s_data_info(),
							proxy_mode_change_back_msg_type::s_cal_type(),
							boost::ref(ref_data.org_buf()));
                {
                    owner_info_container_type tmp;
                    chg_owners.swap(tmp);
                }
			}

			if(dis_rst.second == proxy_fix_state_def_type::E_accepted
				|| dis_rst.second == proxy_fix_state_def_type::E_changed)
			{
				prepeater->register_repeater_data(i->first, i->second.second);
				psvr->template register_network_protocol<net_ipak_type>(i->first, i->first);

				std::cout << "proxy_register " << i->first << ","
							<< i->second.first << ", " << i->second.second << std::endl;
			}

			back_msg.add_reg_back_data(i->first, dis_rst);
		}

		net_opak_type opak(owner, back_msg);
		net_opak_type& ref_data = opak;
		owner_info_container_type owners;
		owners.insert(owner);
		handler(prunner, error_maker_type::make_error(0),
					yggr::task_center::runtime_task_type::E_CAL_RESULT,
					owners, proxy_register_back_msg_type::s_data_info(),
					proxy_register_back_msg_type::s_cal_type(),
					boost::ref(ref_data.org_buf()));
	}

	template<typename Owner, typename Runner, typename Handler>
	void cal_proxy_mode_change_msg_type(const Owner& owner,
											const net_ipak_type& cdt,
											Runner* prunner,
											const Handler& handler)
	{
		typedef proxy_mode_change_msg_type::chg_map_type chg_map_type;
		typedef chg_map_type::const_iterator chg_map_citer_type;

		proxy_mode_change_msg_type chg_msg;
		bool rst = cdt.load(chg_msg);
		if(!rst)
		{
			return;
		}

		proxy_router_ptr_type prouter = _router_wrap.get_shared_ptr();
		if(!prouter)
		{
			return;
		}

		const chg_map_type& cref_chg_map = chg_msg.chg_map();
		owner_info_container_type chg_owners;
		for(chg_map_citer_type i = cref_chg_map.begin(), isize = cref_chg_map.end();
				i != isize; ++i)
		{
			std::pair<yggr::u32, yggr::u32> dis_rst
				= prouter->exchange_mode(
											i->first,
											i->second,
											boost::bind(
															&this_type::handler_get_proxy_list_of_exchange_mode,
															this, _1, boost::ref(chg_owners)
														)
										);

			net_opak_type opak(owner, proxy_mode_change_back_msg_type(i->first, dis_rst.first));
			net_opak_type& ref_data = opak;
			if(!chg_owners.empty())
			{
				assert((dis_rst.second == proxy_fix_state_def_type::E_changed));

				handler(prunner, error_maker_type::make_error(0),
							yggr::task_center::runtime_task_type::E_CAL_RESULT,
							chg_owners, proxy_mode_change_back_msg_type::s_data_info(),
							proxy_mode_change_back_msg_type::s_cal_type(),
							boost::ref(ref_data.org_buf()));
			}
			else
			{
				chg_owners.insert(owner);
				handler(prunner, error_maker_type::make_error(0),
							yggr::task_center::runtime_task_type::E_CAL_RESULT,
							chg_owners, proxy_mode_change_back_msg_type::s_data_info(),
							proxy_mode_change_back_msg_type::s_cal_type(),
							boost::ref(ref_data.org_buf()));
			}

            {
                owner_info_container_type tmp;
                chg_owners.swap(tmp);
            }

			std::cout << "proxy_mode_change " << i->first << ","
						<< i->second << std::endl;
		}
	}

	template<typename Owner, typename Runner, typename Handler>
	void cal_proxy_unregister_msg_type(const Owner& owner,
										const net_ipak_type& cdt,
										Runner* prunner,
										const Handler& handler)
	{
		typedef proxy_unregister_msg_type unreg_msg_type;
		typedef unreg_msg_type::unreg_set_type unreg_set_type;
		typedef unreg_set_type::const_iterator unreg_set_citer_type;

		unreg_msg_type unreg_msg;
		if(!cdt.load(unreg_msg))
		{
			return;
		}

		proxy_router_ptr_type prouter = _router_wrap.get_shared_ptr();

		if(!prouter)
		{
			return;
		}

		const unreg_set_type& unreg_set = unreg_msg.unreg_set();

		proxy_unregister_back_msg_type unreg_back_msg;
		for(unreg_set_citer_type i = unreg_set.begin(), isize = unreg_set.end();
				i != isize; ++i)
		{
			unreg_back_msg.add_unreg_back_data(*i,
								prouter->remove_dispatch(*i,
															owner.owner_id(),
															boost::bind(
																			&this_type::handler_remove_proxy_container,
																			this, _1
																		)
														)
												);
		}

		net_opak_type opak(owner, unreg_back_msg);
		net_opak_type& ref_data = opak;

		owner_info_container_type unreg_owners;
		unreg_owners.insert(owner);

		handler(prunner, error_maker_type::make_error(0),
					yggr::task_center::runtime_task_type::E_CAL_RESULT,
					unreg_owners, proxy_unregister_back_msg_type::s_data_info(),
					proxy_unregister_back_msg_type::s_cal_type(),
					boost::ref(ref_data.org_buf()));
	}

protected:
	void handler_fix_network_exception(const yggr::u64& proxy_val)
	{
		proxy_router_ptr_type prouter = _router_wrap.get_shared_ptr();
		if(!prouter)
		{
			return;
		}
		prouter->clear_dispatch_of_proxy(proxy_val,
											boost::bind(
															&this_type::handler_remove_proxy_container,
															boost::shared_from_raw(this), _1
														)
										);
	}

	void handler_remove_proxy_container(const proxy_container_ptr_type& pcont)
	{
		if(!pcont)
		{
			return;
		}
		dispatch_id_type dis_id = pcont->id();
		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->unregister_repeater_data(dis_id);
		}

		typename server_ptr_single_type::obj_ptr_type psvr = server_ptr_single_type::get_ins();
		if(psvr)
		{
			psvr->unregister_network_protocol(dis_id);
		}
	}

	void handler_get_proxy_list_of_register(const proxy_container_ptr_type& pcont,
												owner_info_container_type& cont,
												const proxy_val_type& now_val) const
	{
		typedef std::set<proxy_val_type> proxy_val_set_type;
		typedef typename proxy_val_set_type::const_iterator set_citer_type;
		if(!pcont)
		{
			return;
		}

		proxy_val_set_type proxy_val_set;
		pcont->get_proxy_list(proxy_val_set);

		proxy_val_set.erase(now_val);

		for(set_citer_type i = proxy_val_set.begin(), isize = proxy_val_set.end(); i != isize; ++i)
		{
			cont.insert(owner_info_type(*i));
		}
	}

	void handler_get_proxy_list_of_exchange_mode(const proxy_container_ptr_type& pcont,
													owner_info_container_type& cont) const
	{
		typedef std::set<proxy_val_type> proxy_val_set_type;
		typedef typename proxy_val_set_type::const_iterator set_citer_type;
		if(!pcont)
		{
			return;
		}

		proxy_val_set_type proxy_val_set;
		pcont->get_proxy_list(proxy_val_set);


		for(set_citer_type i = proxy_val_set.begin(), isize = proxy_val_set.end(); i != isize; ++i)
		{
			cont.insert(owner_info_type(*i));
		}
	}
protected:
	proxy_repeater_wrap_type _repeater_wrap;
	proxy_router_wrap_type _router_wrap;

};

} // namespace proxy_ctrl
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_MSG_FIXER_HPP__


