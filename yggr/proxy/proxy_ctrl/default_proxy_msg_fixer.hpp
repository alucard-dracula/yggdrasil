//default_proxy_msg_fixer.hpp

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

#ifndef __YGGR_PROXY_PROXY_CTRL_DEFAULT_PROXY_MSG_FIXER_HPP__
#define __YGGR_PROXY_PROXY_CTRL_DEFAULT_PROXY_MSG_FIXER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/args_holder_nv.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/proxy/proxy_ctrl/basic_proxy_msg_fixer.hpp>
#include <yggr/proxy/proxy_mode/proxy_mode_def.hpp>

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
struct default_proxy_msg_fixer
	: public 
		basic_proxy_msg_fixer
		<
			OwnerInfoContainer,
			ProxyRepeater,
			NetInPacket,
			NetOutPacket,
			AdapterMgr,
			TaskDataInfo
		>
{
public:
	ERROR_MAKER_BEGIN("default_proxy_msg_fixer")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef 
		basic_proxy_msg_fixer
		<
			OwnerInfoContainer,
			ProxyRepeater,
			NetInPacket,
			NetOutPacket,
			AdapterMgr,
			TaskDataInfo
		> base_type;

	typedef typename base_type::owner_info_container_type owner_info_container_type;
	typedef typename base_type::proxy_repeater_type proxy_repeater_type;
	typedef typename base_type::net_ipak_type net_ipak_type;
	typedef typename base_type::net_opak_type net_opak_type;
	typedef typename base_type::task_data_info_type task_data_info_type;
	typedef typename base_type::proxy_fix_state_def_type proxy_fix_state_def_type;
	typedef proxy_mode::proxy_mode_def proxy_mode_def_type;

	typedef typename base_type::owner_info_type owner_info_type;

	typedef typename base_type::proxy_router_type proxy_router_type;

	typedef typename base_type::proxy_repeater_wrap_type proxy_repeater_wrap_type;
	typedef typename base_type::proxy_repeater_ptr_type proxy_repeater_ptr_type;

	typedef typename base_type::proxy_router_ptr_type proxy_router_ptr_type;

	typedef typename base_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;

public:
	typedef proxy::proxy_msg::proxy_register_msg<> proxy_register_msg_type;
	typedef proxy::proxy_msg::proxy_register_back_msg<> proxy_register_back_msg_type;
	typedef proxy::proxy_msg::proxy_mode_change_msg<> proxy_mode_change_msg_type;
	typedef proxy::proxy_msg::proxy_mode_change_back_msg<> proxy_mode_change_back_msg_type;
	typedef proxy::proxy_msg::proxy_unregister_msg<> proxy_unregister_msg_type;
	typedef proxy::proxy_msg::proxy_unregister_back_msg<> proxy_unregister_back_msg_type;

protected:
	typedef typename base_type::ref_count_info_type ref_count_info_type;
	typedef typename base_type::ref_count_info_locker_type ref_count_info_locker_type;

	typedef typename base_type::dispatch_id_type dispatch_id_type;
	typedef typename base_type::proxy_container_ptr_type proxy_container_ptr_type;
	typedef typename base_type::proxy_val_type proxy_val_type;
	typedef typename base_type::adapter_mgr_type adapter_mgr_type;
	typedef typename base_type::adapter_mgr_weak_ptr_type adapter_mgr_weak_ptr_type;

private:
	typedef default_proxy_msg_fixer this_type;

protected:
	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename CtrlCenter>
	void register_dispatchers(CtrlCenter& cc)
	{
		ref_count_info_locker_type lk(base_type::_count);
		base_type::template register_dispatchers<Tag>(cc);
	}

public:

	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		ref_count_info_locker_type lk(base_type::_count);

		base_type::template register_repeater_special_data<proxy_register_msg_type>(
			boost::bind(&this_type::cal_proxy_register_msg<owner_info_type,
															runner_type,
															recv_handler_type>,
						boost::shared_from_raw(this), _1, _2, _3, _4));

		base_type::template register_repeater_special_data<proxy_mode_change_msg_type>(
			boost::bind(&this_type::cal_proxy_mode_change_msg<owner_info_type,
																runner_type,
																recv_handler_type>,
						boost::shared_from_raw(this), _1, _2, _3, _4));

		base_type::template register_repeater_special_data<proxy_unregister_msg_type>(
			boost::bind(&this_type::cal_proxy_unregister_msg<owner_info_type,
																runner_type,
																recv_handler_type>,
						boost::shared_from_raw(this), _1, _2, _3, _4));
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		ref_count_info_locker_type lk(base_type::_count);

		base_type::template unregister_repeater_special_data<proxy_register_msg_type>();
		base_type::template unregister_repeater_special_data<proxy_mode_change_msg_type>();
		base_type::template unregister_repeater_special_data<proxy_unregister_msg_type>();
	}

public:
	default_proxy_msg_fixer(const proxy_repeater_ptr_type& prepeater,
							const adapter_mgr_ptr_type& padp_mgr)
		:  base_type(prepeater, padp_mgr)
	{
	}

	~default_proxy_msg_fixer(void)
	{
	}

public:
	using base_type::join;

	inline void clear(void)
	{
		base_type::clear();
		
		proxy_repeater_ptr_type prepeater = base_type::_repeater_wrap.get_shared_ptr();
		if(!prepeater)
		{
			return;
		}

		prepeater->clear();
	}

	template<typename Owner, typename Runner, typename Handler>
	void cal_proxy_register_msg(const Owner& owner,
								const net_ipak_type& cdt,
								Runner* prunner,
								const Handler& handler)
	{
		typedef Owner owner_type;
		typedef Runner runner_type;
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		typedef proxy_register_msg_type::attached_data_type attached_data_type;
		typedef proxy_register_msg_type::data_info_type data_info_type;
		typedef proxy_register_msg_type::reg_map_type reg_map_type;
		typedef reg_map_type::const_iterator reg_map_citer_type;

		ref_count_info_locker_type lk(base_type::_count);

		proxy_register_msg_type reg_msg;
		proxy_register_back_msg_type back_msg;

		bool rst = cdt.load(reg_msg);
		if(!rst)
		{
			return;
		}

		handler_holder_type holder(handler);

		if(!base_type::cal_proxy_register_msg(
			owner, reg_msg, back_msg,
			boost::bind(
				&this_type::handler_mode_change
				<
					owner_type, 
					data_info_type,
					attached_data_type,
					runner_type,
					handler_type
				>, 
				boost::shared_from_raw(this), 
				_1, _2, _3, _4, boost::cref(reg_msg.attached_data()), prunner, 
				boost::cref(utility::args_holder_trans(holder)) ) ) )
		{
			copy_or_move_or_swap(back_msg.reg_back_map(), boost::move(reg_msg.reg_map()));
			back_msg.set_item_state(proxy_mode_def_type::E_proxy_mode_failed);
		}

		copy_or_move_or_swap(back_msg.attached_data(), boost::move(reg_msg.attached_data()));
		net_opak_type opak(owner, back_msg);
		net_opak_type& ref_data = opak;
		owner_info_container_type owners;
		iterator_ex::inserter(owners, boost::end(owners)) = owner;
		handler(prunner, error_maker_type::make_error(0),
					yggr::task_center::runtime_task_type::E_CAL_RESULT,
					owners, proxy_register_back_msg_type::data_info(),
					proxy_register_back_msg_type::cal_type(),
					boost::ref(ref_data.org_buf()));
	}

	template<typename Owner, typename Runner, typename Handler>
	void cal_proxy_mode_change_msg(const Owner& owner,
									const net_ipak_type& cdt,
									Runner* prunner,
									const Handler& handler)
	{
		typedef Owner owner_type;
		typedef Runner runner_type;
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		typedef proxy_mode_change_msg_type::attached_data_type attached_data_type;
		typedef proxy_register_msg_type::data_info_type data_info_type;
		typedef proxy_mode_change_msg_type::chg_map_type chg_map_type;
		typedef chg_map_type::const_iterator chg_map_citer_type;

		ref_count_info_locker_type lk(base_type::_count);

		proxy_mode_change_msg_type chg_msg;
		bool rst = cdt.load(chg_msg);
		if(!rst)
		{
			return;
		}

		handler_holder_type holder(handler);

		base_type::cal_proxy_mode_change_msg(
			owner, chg_msg,
			boost::bind(
				&this_type::handler_mode_change
				<
					owner_type, 
					data_info_type, 
					attached_data_type,
					runner_type,
					handler_type
				>, 
				boost::shared_from_raw(this), 
				_1, _2, _3, _4, boost::cref(chg_msg.attached_data()), prunner, 
				boost::cref(utility::args_holder_trans(holder)) ));
	}

	template<typename Owner, typename Runner, typename Handler>
	void cal_proxy_unregister_msg(const Owner& owner,
									const net_ipak_type& cdt,
									Runner* prunner,
									const Handler& handler)
	{
		typedef proxy_unregister_msg_type unreg_msg_type;
		typedef unreg_msg_type::unreg_set_type unreg_set_type;
		typedef unreg_set_type::const_iterator unreg_set_citer_type;

		ref_count_info_locker_type lk(base_type::_count);

		unreg_msg_type unreg_msg;
		proxy_unregister_back_msg_type unreg_back_msg;
		if(!cdt.load(unreg_msg))
		{
			return;
		}

		base_type::cal_proxy_unregister_msg(owner, unreg_msg, unreg_back_msg);
		copy_or_move_or_swap(unreg_back_msg.attached_data(), boost::move(unreg_msg.attached_data()));
		net_opak_type opak(owner, unreg_back_msg);
		net_opak_type& ref_data = opak;
		owner_info_container_type unreg_owners;
		iterator_ex::inserter(unreg_owners, boost::end(unreg_owners)) = owner;

		handler(prunner, error_maker_type::make_error(0),
					yggr::task_center::runtime_task_type::E_CAL_RESULT,
					unreg_owners, proxy_unregister_back_msg_type::data_info(),
					proxy_unregister_back_msg_type::cal_type(),
					boost::ref(ref_data.org_buf()));
	}

protected:
	template<typename Owner, typename Data_Info, typename Attached_Data, typename Runner, typename Handler>
	void handler_mode_change(const Owner& owner, 
								owner_info_container_type& chg_owners, 
								const Data_Info& data_info, 
								u32 back_mode,
								const Attached_Data& att_data,
								Runner* prunner,
								const utility::basic_args_holder_nv& basic_handler_holder) const
	{
		typedef Handler handler_type;

		ref_count_info_locker_type lk(base_type::_count);

		const handler_type* phandler = utility::args_holder_ref_wrap_static_cast<handler_type const>(basic_handler_holder);
		assert(prunner);
		assert(phandler);

		if(!phandler)
		{
			return;
		}

		if(!boost::empty(chg_owners))
		{
			net_opak_type opak(owner, proxy_mode_change_back_msg_type(data_info, back_mode, att_data));
			net_opak_type& ref_data = opak;
			(*phandler)(prunner, error_maker_type::make_error(0),
							yggr::task_center::runtime_task_type::E_CAL_RESULT,
							chg_owners, proxy_mode_change_back_msg_type::data_info(),
							proxy_mode_change_back_msg_type::cal_type(),
							boost::ref(ref_data.org_buf()));
		}
	}

	using base_type::route;
};

} // namespace proxy_ctrl
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_MSG_FIXER_HPP__


