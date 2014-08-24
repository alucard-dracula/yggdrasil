//proxy_container.hpp

#ifndef __YGGR_PROXY_PROXY_CONTAINER_HPP__
#define __YGGR_PROXY_PROXY_CONTAINER_HPP__

#include <algorithm>

#include <yggr/base/interface_ptr.hpp>
#include <yggr/mplex/get_container_value_t.hpp>
#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/safe_container/safe_vector.hpp>
#include <yggr/task_center/task_info_def.hpp>
#include <yggr/proxy/proxy_mode/basic_proxy_mode.hpp>
#include <yggr/proxy/proxy_fix_state_def.hpp>

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
	typedef typename mode_creator_type::owner_id_container_type owner_id_container_type;

	typedef typename mplex::get_container_value_t<owner_info_container_type>::type owner_info_type;
	typedef typename  mplex::get_container_value_t<owner_id_container_type>::type owner_id_type;

	typedef owner_id_type val_type;

public:
	//enum
	//{
	//	E_accepted = 0,
	//	E_changed,
	//	E_refused,
	//	E_existsed,
	//	E_failed,

	//	E_compile_u32 = 0xffffffff
	//};

	typedef proxy_fix_state_def fix_state_def_type;

private:
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

	typedef interface_ptr
			<
				proxy_mode::basic_proxy_mode
				<
					owner_info_container_type
				>
			> interface_proxy_mode_type;

	typedef safe_container::safe_wrap<interface_proxy_mode_type> proxy_mode_wrap_type;
	typedef typename proxy_mode_wrap_type::error_type proxy_mode_wrap_error_type;

	typedef safe_container::safe_vector<owner_id_type> owner_id_vt_type;

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

	id_type id(void) const
	{
		return _id;
	}

	bool empty_of_proxy(void) const
	{
		return _proxy_owner_id_vt.empty();
	}

	template<typename Runner, typename Handler>
	void send(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type) const
	{
		owner_info_container_type owners;
		_proxy_owner_id_vt.use_handler(boost::bind(&this_type::handler_get_owner_info_container,
													this, _1, boost::cref(owner), boost::ref(owners)));

		if(owners.empty())
		{
			return;
		}

		task_type& ref_data = const_cast<task_type&>(proxy_data);
		handler(prunner, error_maker_type::make_error(0),
					yggr::task_center::runtime_task_type::E_CAL_RESULT,
					owners, proxy_data.data_info(), cal_type, boost::ref(ref_data.org_buf()));
	}

	std::pair<u32, u32> append_proxy(u32 mode, const val_type& val)
	{
		return _proxy_owner_id_vt
					.use_handler(
									boost::bind(
													&this_type::handler_append_proxy, this,
													_1, mode, boost::cref(val)
												)
								);
	}

	std::pair<u32, u32> exchange_mode(u32 mode)
	{
		return this_type::create_mode(mode);
	}

	bool remove_proxy(const val_type& val)
	{
		return _proxy_owner_id_vt
					.use_handler(
									boost::bind(
													&this_type::handler_remove_proxy,
													this,
													_1,
													boost::cref(val)
												)
								);
	}

	template<typename Container>
	void get_proxy_list(Container& container) const
	{
		typedef Container container_type;
		_proxy_owner_id_vt.use_handler(
								boost::bind(&this_type::handler_get_proxy_list<container_type>,
												this, _1, boost::ref(container)
											)
										);
	}

private:
	template<typename Container>
	void handler_get_proxy_list(const typename owner_id_vt_type::base_type& base, Container& cont) const
	{
		typedef Container container_type;
		container_type tmp(base.begin(), base.end());
		cont.swap(tmp);
	}

	void handler_get_owner_info_container(const typename owner_id_vt_type::base_type& base,
											const owner_info_type& info,
											owner_info_container_type& cont) const
	{
		interface_proxy_mode_type pimode;

		try
		{
			pimode = _proxy_mode_wrap.get_value();
		}
		catch(const proxy_mode_wrap_error_type&)
		{
			return;
		}

		if(!pimode)
		{
			return;
		}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		cont = boost::move(pimode->get_owner_info_container(info, base));
#else
		cont = pimode->get_owner_info_container(info, base);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	bool handler_remove_proxy(typename owner_id_vt_type::base_type& base,
									const val_type& val)
	{
		typedef typename owner_id_vt_type::iterator iter_type;
		iter_type iter = std::remove(base.begin(), base.end(), val);

		if(iter != base.end())
		{
			base.erase(iter, base.end());
			return true;
		}

		return false;
	}

	std::pair<u32, u32> handler_append_proxy(typename owner_id_vt_type::base_type& base,
												u32 mode, const val_type& val)
	{
		typedef std::pair<u32, u32> ret_type;
		if(std::find(base.begin(), base.end(), val) != base.end())
		{
			return ret_type(mode, fix_state_def_type::E_existsed);
		}

		ret_type cret = create_mode(mode);
		if(cret.second == fix_state_def_type::E_failed
			|| cret.second == fix_state_def_type::E_refused)
		{
			return cret;
		}

		base.push_back(val);
		return cret;
	}

	std::pair<u32, u32> create_mode(u32 mode)
	{
		return _proxy_mode_wrap
				.locked_unsafe_use_handler(
											boost::bind(&this_type::handler_creator_mode,
															this,
															_1,
															boost::cref(mode)
															)
											);
	}


	std::pair<u32, u32> handler_creator_mode(typename proxy_mode_wrap_type::value_ptr_type& ptr,
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
		// pimode.swap(_mode_creator(new_mode));
		{
		    interface_proxy_mode_type tmp(_mode_creator(new_mode));
            pimode.swap(tmp);
		}

		assert(pimode);
		if(!pimode)
		{
			return ret_type(u32(), fix_state_def_type::E_failed);
		}

		//ptr.swap(value_ptr_type(new interface_proxy_mode_type(pimode)));
		{
		    value_ptr_type tmp(new interface_proxy_mode_type(pimode));
		    ptr.swap(tmp);
		}

		return ret_type(new_mode, (old_mode? fix_state_def_type::E_changed : fix_state_def_type::E_accepted));
	}

private:
	mode_tactics_type& _mode_tactics;
	mode_creator_type& _mode_creator;
	id_type _id;
	proxy_mode_wrap_type _proxy_mode_wrap;

	owner_id_vt_type _proxy_owner_id_vt;
};

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_CONTAINER_HPP__
