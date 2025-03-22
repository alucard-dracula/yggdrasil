//basic_proxy_msg_fixer.hpp

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

#ifndef __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_MSG_FIXER_HPP__
#define __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_MSG_FIXER_HPP__

#include <yggr/move/move.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/weak_ptr.hpp>

#include <yggr/container/set.hpp>
#include <yggr/container/clear.hpp>
#include <yggr/container/reserve.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/proxy/proxy_fix_state_def.hpp>
#include <yggr/proxy/proxy_ctrl/basic_proxy_route_fixer.hpp>

#include <yggr/proxy/proxy_msg/proxy_register_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_register_back_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_mode_change_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_mode_change_back_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_unregister_msg.hpp>
#include <yggr/proxy/proxy_msg/proxy_unregister_back_msg.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/functions.hpp>

#include <iterator>

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
			typename AdapterMgr,
			typename TaskDataInfo = task_center::default_task_data_info_type>
struct basic_proxy_msg_fixer
	: public basic_proxy_route_fixer<ProxyRepeater>
{
public:
	ERROR_MAKER_BEGIN("basic_proxy_msg_fixer")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef basic_proxy_route_fixer<ProxyRepeater> base_type;

	typedef OwnerInfoContainer owner_info_container_type;
	typedef typename base_type::proxy_repeater_type proxy_repeater_type;
	typedef typename base_type::proxy_repeater_wrap_type proxy_repeater_wrap_type;
	typedef typename base_type::proxy_repeater_ptr_type proxy_repeater_ptr_type;

	typedef typename base_type::proxy_router_type proxy_router_type;
	typedef typename base_type::proxy_router_ptr_type proxy_router_ptr_type;

	typedef NetInPacket net_ipak_type;
	typedef NetOutPacket net_opak_type;
	typedef TaskDataInfo task_data_info_type;

	typedef proxy_fix_state_def proxy_fix_state_def_type;
	typedef typename boost::range_value<owner_info_container_type>::type owner_info_type;

protected:
	typedef yggr::ref_count_info<> ref_count_info_type;
	typedef yggr::ref_count_info_locker<ref_count_info_type> ref_count_info_locker_type;

	typedef std::pair<u32, u32> mode_change_result_type;

	typedef typename proxy_router_type::dispatch_id_type dispatch_id_type;
	typedef typename proxy_router_type::proxy_container_ptr_type proxy_container_ptr_type;
	typedef typename proxy_router_type::proxy_val_type proxy_val_type;

	typedef AdapterMgr adapter_mgr_type;
	typedef smart_ptr_ex::weak_ptr<adapter_mgr_type> adapter_mgr_weak_ptr_type;

public:
	typedef ::yggr::shared_ptr<adapter_mgr_type> adapter_mgr_ptr_type;

private:
	typedef basic_proxy_msg_fixer this_type;

protected:
	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename CtrlCenter>
	void register_dispatchers(CtrlCenter& cc)
	{
		ref_count_info_locker_type lk(base_type::_count);

		CTRL_HANDLER(Tag, cc, boost::asio::error::invalid_argument,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::message_size,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::name_too_long,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::network_down,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_buffer_space,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_memory,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::shut_down,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::timed_out,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_reset,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
										boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_aborted,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
								boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::operation_aborted,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, 2,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		if(boost::asio::error::connection_reset != 104) // windows not equal see boost asio/erron.h
		{
			CTRL_HANDLER(Tag, cc, 104,
							yggr::system_controller::system_code::E_SESSION_SYSTEM,
							boost::bind(&this_type::handler_fix_network_exception,
										boost::shared_from_raw(this), _1));
		}

		CTRL_HANDLER(Tag, cc, 121,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, 0x60009004,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));

		CTRL_HANDLER(Tag, cc, 0x60020001,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_fix_network_exception,
									boost::shared_from_raw(this), _1));
	}

public:
	basic_proxy_msg_fixer(const proxy_repeater_ptr_type& prepeater,
							const adapter_mgr_ptr_type& padp_mgr)
		:  base_type(prepeater), _wp_adp_mgr(padp_mgr)
	{
	}

	~basic_proxy_msg_fixer(void)
	{
	}

public:
	using base_type::join;

	template<typename Owner,
				typename AttachedData1, typename TaskDataInfo1,
				typename AttachedData2, typename TaskDataInfo2,
				typename Handler_Mode_Change>
	bool cal_proxy_register_msg(const Owner& owner,
								const
									proxy_msg::proxy_register_msg
									<
										AttachedData1,
										TaskDataInfo1
									>& reg_msg,
								proxy_msg::proxy_register_back_msg
								<
									AttachedData2,
									TaskDataInfo2
								>& back_msg,
								const Handler_Mode_Change& handler_mode_chg)
	{
		typedef
			proxy_msg::proxy_register_msg
			<
				AttachedData1,
				TaskDataInfo1
			> proxy_register_msg_type;
		typedef typename proxy_register_msg_type::reg_map_type reg_map_type;
		typedef typename reg_map_type::const_iterator reg_map_citer_type;

		ref_count_info_locker_type lk(base_type::_count);

		adapter_mgr_ptr_type padp_mgr = _wp_adp_mgr.lock();

		if(!padp_mgr)
		{
			return false;
		}

		proxy_repeater_ptr_type prepeater = base_type::_repeater_wrap.get_shared_ptr();
		if(!prepeater)
		{
			return false;
		}

		proxy_router_ptr_type prouter = prepeater->get_router();
		if(!prouter)
		{
			return false;
		}

		back_msg.clear();
		const reg_map_type& cref_reg_map = reg_msg.reg_map();
		owner_info_container_type chg_owners;
		std::pair<u32, u32> dis_rst = std::make_pair(u32(0), u32(0));
		for(reg_map_citer_type i = cref_reg_map.begin(), isize = cref_reg_map.end();
				i != isize; ++i)
		{
			dis_rst
				= prouter->register_dispatch(
						i->first, i->second.first, owner.owner_id(),
						boost::bind(&this_type::handler_get_proxy_list_of_register,
										this, _1, boost::ref(chg_owners), owner.owner_id() ) );

			assert((!chg_owners.empty()? dis_rst.second == proxy_fix_state_def_type::E_changed : true));
			handler_mode_chg(owner, chg_owners, i->first, dis_rst.first);
			container::clear(chg_owners);

			switch(dis_rst.second)
			{
			case proxy_fix_state_def_type::E_accepted:
			case proxy_fix_state_def_type::E_changed:
				prepeater->register_repeater_data(i->first, i->second.second);
				padp_mgr->template register_adapter<net_ipak_type>(i->first, i->first);
				break;
			}

#	ifdef _DEBUG
			if(dis_rst.second == proxy_fix_state_def_type::E_accepted
				|| dis_rst.second == proxy_fix_state_def_type::E_changed)
			{
				std::cout << "proxy_register "
							<< i->first << ","
							<< i->second.first << ", "
							<< i->second.second << std::endl;

			}
#	endif // _DEBUG

			back_msg.add_reg_back_data(i->first, dis_rst);
		}

		return true;
	}

	template<typename Owner,
				typename AttachedData1, typename TaskDataInfo1,
				typename Handler_Mode_Change>
	bool cal_proxy_mode_change_msg(const Owner& owner,
									const
										proxy_msg::proxy_mode_change_msg
										<
											AttachedData1,
											TaskDataInfo1
										>& chg_msg,
									const Handler_Mode_Change& handler_mode_chg)
	{
		typedef
			proxy_msg::proxy_mode_change_msg
			<
				AttachedData1,
				TaskDataInfo1
			> proxy_mode_change_msg_type;
		typedef typename proxy_mode_change_msg_type::chg_map_type chg_map_type;
		typedef typename chg_map_type::const_iterator chg_map_citer_type;

		ref_count_info_locker_type lk(base_type::_count);

		proxy_repeater_ptr_type prepeater = base_type::_repeater_wrap.get_shared_ptr();
		if(!prepeater)
		{
			return false;
		}

		proxy_router_ptr_type prouter = prepeater->get_router();
		if(!prouter)
		{
			return false;
		}

		const chg_map_type& cref_chg_map = chg_msg.chg_map();
		owner_info_container_type chg_owners;
		std::pair<yggr::u32, yggr::u32> dis_rst = std::make_pair(u32(0), u32(0));
		for(chg_map_citer_type i = cref_chg_map.begin(), isize = cref_chg_map.end();
				i != isize; ++i)
		{
			dis_rst
				= prouter->exchange_mode(
					i->first, i->second,
					boost::bind(&this_type::handler_get_proxy_list_of_exchange_mode,
								this, _1, boost::ref(chg_owners) ) );

			assert((!chg_owners.empty()?dis_rst.second == proxy_fix_state_def_type::E_changed : true));

			if(chg_owners.empty())
			{
				iterator_ex::inserter(chg_owners, boost::end(chg_owners)) = owner;
			}

			handler_mode_chg(owner, chg_owners, i->first, dis_rst.first);
			container::clear(chg_owners);

#ifdef _DEBUG
			std::cout << "proxy_mode_change "
						<< i->first << ","
						<< i->second << std::endl;
#endif // _DEBUG
		}

		return true;
	}

	template<typename Owner,
				typename AttachedData1, typename TaskDataInfo1,
				typename AttachedData2, typename TaskDataInfo2>
	bool cal_proxy_unregister_msg(const Owner& owner,
									const
										proxy_msg::proxy_unregister_msg
										<
											AttachedData1,
											TaskDataInfo1
										>& unreg_msg,
									proxy_msg::proxy_unregister_back_msg
									<
										AttachedData2,
										TaskDataInfo2
									>& unreg_back_msg)
	{
		typedef
			proxy_msg::proxy_unregister_msg
			<
				AttachedData1,
				TaskDataInfo1
			> proxy_unregister_msg_type;

		typedef proxy_unregister_msg_type unreg_msg_type;
		typedef typename unreg_msg_type::unreg_set_type unreg_set_type;
		typedef typename unreg_set_type::const_iterator unreg_set_citer_type;

		ref_count_info_locker_type lk(base_type::_count);

		proxy_repeater_ptr_type prepeater = base_type::_repeater_wrap.get_shared_ptr();
		if(!prepeater)
		{
			return false;
		}

		proxy_router_ptr_type prouter = prepeater->get_router();
		if(!prouter)
		{
			return false;
		}

		unreg_back_msg.clear();
		const unreg_set_type& unreg_set = unreg_msg.unreg_set();
		for(unreg_set_citer_type i = unreg_set.begin(), isize = unreg_set.end();
				i != isize; ++i)
		{
			unreg_back_msg.add_unreg_back_data(
				*i,
				prouter->remove_dispatch(
					*i, owner.owner_id(),
					boost::bind(&this_type::handler_remove_proxy_container,
								this, _1 ) ) );
		}

		return true;
	}

public:
	inline void clear(void)
	{
		ref_count_info_locker_type lk(base_type::_count);

		proxy_repeater_ptr_type prepeater = base_type::_repeater_wrap.get_shared_ptr();
		if(!prepeater)
		{
			return;
		}

		proxy_router_ptr_type prouter = prepeater->get_router();
		if(!prouter)
		{
			return;
		}

		prouter->clear(boost::bind(&this_type::handler_clear_proxy_container, this, _1));
	}

protected:
	using base_type::get_proxy_repeater;

	using base_type::register_repeater_data;
	using base_type::register_repeater_special_data;

	using base_type::unregister_repeater_data;
	using base_type::unregister_repeater_special_data;

	using base_type::append_special_data;
	using base_type::remove_special_data;

	using base_type::route;

protected:
	void handler_fix_network_exception(const proxy_val_type& proxy_val)
	{
		ref_count_info_locker_type lk(base_type::_count);

		proxy_repeater_ptr_type prepeater = base_type::_repeater_wrap.get_shared_ptr();
		if(!prepeater)
		{
			return;
		}

		proxy_router_ptr_type prouter = prepeater->get_router();
		if(!prouter)
		{
			return;
		}

		prouter->clear_dispatch_of_proxy(
			proxy_val,
			boost::bind(
				&this_type::handler_remove_proxy_container,
				boost::shared_from_raw(this), _1) );
	}

	void handler_remove_proxy_container(const proxy_container_ptr_type& pcont)
	{
		ref_count_info_locker_type lk(base_type::_count);

		if(!pcont)
		{
			return;
		}
		dispatch_id_type dis_id = pcont->id();
		proxy_repeater_ptr_type prepeater = base_type::_repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->unregister_repeater_data(dis_id);
		}

		adapter_mgr_ptr_type padp_mgr = _wp_adp_mgr.lock();
		if(padp_mgr)
		{
			padp_mgr->unregister_adapter(dis_id);
		}
	}

	void handler_clear_proxy_container(const proxy_container_ptr_type& pcont)
	{
		ref_count_info_locker_type lk(base_type::_count);

		if(!pcont)
		{
			return;
		}
		dispatch_id_type dis_id = pcont->id();
		proxy_repeater_ptr_type prepeater = base_type::_repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			// multi called, but not problem
			prepeater->clear();
		}

		adapter_mgr_ptr_type padp_mgr = _wp_adp_mgr.lock();
		if(padp_mgr)
		{
			padp_mgr->unregister_adapter(dis_id);
		}
	}

	void handler_get_proxy_list_of_register(const proxy_container_ptr_type& pcont,
												owner_info_container_type& cont,
												const proxy_val_type& now_val) const
	{
		ref_count_info_locker_type lk(base_type::_count);

		typedef ::yggr::set<proxy_val_type> proxy_val_set_type;
		typedef typename proxy_val_set_type::const_iterator set_citer_type;
		if(!pcont)
		{
			return;
		}

		proxy_val_set_type proxy_val_set;
		pcont->get_proxy_list(proxy_val_set);

		proxy_val_set.erase(now_val);

		container::reserve(cont, boost::size(proxy_val_set));

		for(set_citer_type i = boost::begin(proxy_val_set), isize = boost::end(proxy_val_set); i != isize; ++i)
		{
			iterator_ex::inserter(cont, boost::end(cont)) = owner_info_type(*i);
		}
	}

	void handler_get_proxy_list_of_exchange_mode(const proxy_container_ptr_type& pcont,
													owner_info_container_type& cont) const
	{
		ref_count_info_locker_type lk(base_type::_count);

		typedef ::yggr::set<proxy_val_type> proxy_val_set_type;
		typedef typename proxy_val_set_type::const_iterator set_citer_type;
		if(!pcont)
		{
			return;
		}

		proxy_val_set_type proxy_val_set;
		pcont->get_proxy_list(proxy_val_set);

		container::reserve(cont, boost::size(proxy_val_set));

		for(set_citer_type i = boost::begin(proxy_val_set), isize = boost::end(proxy_val_set); i != isize; ++i)
		{
			iterator_ex::inserter(cont, boost::end(cont)) = owner_info_type(*i);
		}
	}

protected:
	adapter_mgr_weak_ptr_type _wp_adp_mgr;
};

} // namespace proxy_ctrl
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_MSG_FIXER_HPP__


