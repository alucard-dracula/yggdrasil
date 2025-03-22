//proxy_container.hpp

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

#ifndef __YGGR_PROXY_PROXY_CONTAINER_HPP__
#define __YGGR_PROXY_PROXY_CONTAINER_HPP__

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/args_holder.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/safe_container/safe_multi_index_random_access_index.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>

#include <yggr/task_center/task_info_def.hpp>
#include <yggr/proxy/proxy_fix_state_def.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <boost/range/functions.hpp>

#include <boost/range/value_type.hpp>
#include <boost/multi_index/identity.hpp>

#include <algorithm>


namespace yggr
{
namespace proxy
{

template<	typename ID,
			typename Task,
			typename ModeTactics,
			typename ModeCreator
			>
class proxy_container
{
private:
	ERROR_MAKER_BEGIN("proxy_container")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()
public:
	typedef ID id_type;
	typedef Task task_type;
	typedef ModeTactics mode_tactics_type;
	typedef ModeCreator mode_creator_type;

	typedef typename mode_creator_type::owner_info_container_type owner_info_container_type;
	typedef typename mode_creator_type::owner_info_type owner_info_type;
	typedef typename mode_creator_type::owner_id_type owner_id_type;
	typedef typename mode_creator_type::mode_data_store_type mode_data_store_type;

	typedef owner_id_type val_type;
	typedef val_type value_type;

public:
	// view only don't uncomment
	//YGGR_STATIC_CONSTANT(u32, E_failed = 0);
	//YGGR_STATIC_CONSTANT(u32, E_accepted = 1);
	//YGGR_STATIC_CONSTANT(u32, E_changed = 2);
	//YGGR_STATIC_CONSTANT(u32, E_refused = 3);
	//YGGR_STATIC_CONSTANT(u32, E_existsed = 4);

	typedef proxy_fix_state_def fix_state_def_type;

private:
	typedef safe_container::safe_wrap<id_type> id_wrap_type;

	BOOST_MPL_ASSERT((boost::is_same
						<
							typename task_type::owner_info_type,
							owner_info_type
						>));

	BOOST_MPL_ASSERT((boost::is_same
						<
							typename owner_info_type::owner_id_type,
							owner_id_type
						>));

	typedef typename mode_creator_type::base_proxy_mode_type base_proxy_mode_type;
	typedef typename mode_creator_type::store_type store_type;

	typedef shared_ptr<base_proxy_mode_type> interface_proxy_mode_type;
	typedef safe_container::safe_wrap<interface_proxy_mode_type> proxy_mode_wrap_type;
	typedef typename proxy_mode_wrap_type::error_type proxy_mode_wrap_error_type;

private:
	typedef proxy_container this_type;

public:
	proxy_container(mode_tactics_type& tactics,
						mode_creator_type& creator,
						const id_type& id)
		: _mode_tactics(tactics),
			_mode_creator(creator),
			_id(id)
	{
	}

	~proxy_container(void)
	{
	}

public:
	inline id_type id(void) const
	{
		return _id.load();
	}

	inline bool empty_of_proxy(void) const
	{
		return _mode_data_store.empty();
	}

	template<typename Runner, typename Handler> inline
	void send(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type) const
	{
		owner_info_container_type owners;
		interface_proxy_mode_type pimode;

		try
		{
			pimode = _proxy_mode_wrap.load();
		}
		catch(const proxy_mode_wrap_error_type&)
		{
			return;
		}

		_mode_data_store.get_owner_info_container(owners, pimode, owner);

		if(boost::empty(owners))
		{
			return;
		}

		task_type& ref_data = const_cast<task_type&>(proxy_data);
		handler(prunner, error_maker_type::make_error(0),
					yggr::task_center::runtime_task_type::E_CAL_RESULT,
					owners, proxy_data.data_info(), cal_type, boost::ref(ref_data.org_buf()));
	}

	template<typename Runner, typename Handler, typename Args> inline
	void send(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type,
				Args& args) const
	{

		typedef Args args_type;
		typedef utility::args_holder_nv_ref_wrap<args_type> args_holder_type;

		owner_info_container_type owners;
		interface_proxy_mode_type pimode;

		try
		{
			pimode = _proxy_mode_wrap.load();
		}
		catch(const proxy_mode_wrap_error_type&)
		{
			return;
		}

		args_holder_type args_holder_ref_wrap(args);
		_mode_data_store.get_owner_info_container(owners, pimode, owner, args);

		if(boost::empty(owners))
		{
			return;
		}

		task_type& ref_data = const_cast<task_type&>(proxy_data);
		handler(prunner, error_maker_type::make_error(0),
					yggr::task_center::runtime_task_type::E_CAL_RESULT,
					owners, proxy_data.data_info(), cal_type, boost::ref(ref_data.org_buf()));
	}

	std::pair<u32, u32> 
		append_proxy(u32 mode, const owner_id_type& val)
	{
		return 
			_mode_data_store.using_handler(
				boost::bind(&this_type::handler_append_proxy, this,
								_1, mode, boost::cref(val) ));
	}

	inline std::pair<u32, u32> exchange_mode(u32 mode)
	{
		return this_type::create_mode(mode);
	}

	inline bool remove_proxy(const owner_id_type& id)
	{
		// if you forgot to write a "return", there will be a poem -_-!!!
		return _mode_data_store.erase(id);
	}

	template<typename Container> inline
	void get_proxy_list(Container& cont) const
	{
		_mode_data_store.get_datas(cont);
	}

	template<typename Container> inline
	void swap_proxy_list(Container& cont)
	{
		_mode_data_store.swap_datas(cont);
	}

private:
	inline std::pair<u32, u32> 
		handler_append_proxy(store_type& cont, u32 mode, const owner_id_type& val)
	{
		typedef std::pair<u32, u32> ret_type;

		if(mode_data_store_type::s_is_exists_of_base(cont, val))
		{
			return ret_type(mode, fix_state_def_type::E_existsed);
		}

		ret_type cret = create_mode(mode);
		if(!(cret.second == fix_state_def_type::E_failed
				|| cret.second == fix_state_def_type::E_refused))
		{
			mode_data_store_type::s_push_data_of_base(cont, val);
		}

		return cret;
	}

	inline std::pair<u32, u32> create_mode(u32 mode)
	{
		return 
			_proxy_mode_wrap.locked_unsafe_using_handler(
				boost::bind(
					&this_type::handler_creator_mode,
					this, _1, boost::cref(mode) ) );
	}


	inline std::pair<u32, u32> 
		handler_creator_mode(typename proxy_mode_wrap_type::value_ptr_type& ptr,
								const u32 mode)
	{
		typedef std::pair<u32, u32> ret_type;
		typedef typename proxy_mode_wrap_type::value_ptr_type value_ptr_type;

		u32 old_mode = 0;
		u32 new_mode = 0;
		if(ptr)
		{
			if(!*ptr)
			{
				return ret_type(u32(), fix_state_def_type::E_failed);
			}
			old_mode = (*ptr)->now_mode();
			new_mode = _mode_tactics(old_mode, mode);
		}
		else
		{
			new_mode = mode;
		}

		if(!new_mode)
		{
			return ret_type(old_mode, fix_state_def_type::E_refused);
		}

		if(old_mode == new_mode)
		{
			return ret_type(old_mode, fix_state_def_type::E_accepted);
		}

		interface_proxy_mode_type pimode;
		{
		    interface_proxy_mode_type tmp(_mode_creator(new_mode));
            pimode.swap(tmp);
		}

		assert(pimode);
		if(!pimode)
		{
			return ret_type(u32(), fix_state_def_type::E_failed);
		}

		{
			value_ptr_type tmp;
			try
			{
				tmp = value_ptr_type(yggr_nothrow_new interface_proxy_mode_type(pimode));
			}
			catch(...)
			{
				return ret_type(u32(), fix_state_def_type::E_failed);
			}

		    ptr.swap(tmp);
		}

		return ret_type(new_mode, (old_mode? fix_state_def_type::E_changed : fix_state_def_type::E_accepted));
	}

private:
	mode_tactics_type& _mode_tactics;
	mode_creator_type& _mode_creator;
	id_wrap_type _id;
	proxy_mode_wrap_type _proxy_mode_wrap;
	mode_data_store_type _mode_data_store;
};

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_CONTAINER_HPP__
