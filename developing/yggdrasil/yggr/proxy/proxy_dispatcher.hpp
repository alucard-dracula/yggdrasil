//proxy_dispatcher.hpp

#ifndef __YGGR_PROXY_PROXY_DISPATCHER_HPP__
#define __YGGR_PROXY_PROXY_DISPATCHER_HPP__

#include <boost/bind.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/unordered_set.hpp>

#include <yggr/task_center/task_info_def.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>


namespace yggr
{
namespace proxy
{

template<	typename ID_Parser,
			typename ProxyContainer>
class proxy_dispatcher
{
protected:

	ERROR_MAKER_BEGIN("proxy_dispatcher")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef ID_Parser id_parser_type;
	typedef ProxyContainer proxy_container_type;
	typedef boost::shared_ptr<proxy_container_type> proxy_container_ptr_type;

	typedef typename proxy_container_type::task_type task_type;
	typedef typename proxy_container_type::owner_info_container_type owner_info_container_type;
	typedef typename proxy_container_type::owner_info_type owner_info_type;
	typedef typename proxy_container_type::val_type proxy_val_type;
	typedef typename proxy_container_type::fix_state_def_type fix_state_def_type;

	typedef typename id_parser_type::id_type id_type;

protected:

	BOOST_MPL_ASSERT((boost::is_same<id_type, typename proxy_container_type::id_type>));
	typedef typename proxy_container_type::mode_tactics_type mode_tactics_type;
	typedef typename proxy_container_type::mode_creator_type mode_creator_type;

	typedef safe_container::safe_unordered_map<id_type, proxy_container_ptr_type> dispatch_map_type;

	typedef boost::unordered_set<proxy_container_ptr_type> dispatch_set_type;
	//typedef safe_container::safe_unordered_map<proxy_val_type, std::size_t> proxy_val_map_type;
	typedef safe_container::safe_unordered_map<proxy_val_type, dispatch_set_type> proxy_val_map_type;

private:
	struct tmp_handler_type
	{
		inline void operator()(const proxy_container_ptr_type& pcont) const{}
	};

	typedef proxy_dispatcher this_type;

public:
	proxy_dispatcher(void)
	{
	}

	~proxy_dispatcher(void)
	{
	}

	template<typename RealData>
	std::pair<u32, u32> register_dispatch(u32 mode, const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		return this_type::register_dispatch<RealData>(mode, val, tmp_handler);
	}

	std::pair<u32, u32> register_dispatch(const id_type& id, u32 mode, const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		return this_type::register_dispatch(id, mode, val, tmp_handler);
	}

	template<typename RealData, typename Change_Handler>
	std::pair<u32, u32> register_dispatch(u32 mode, const proxy_val_type& val,
											const Change_Handler& handler)
	{
		typedef RealData real_data_type;
		typedef Change_Handler handler_type;
		id_type id = id_parser_type::template get_reg_id<real_data_type>();
		return _dis_map.use_handler(
							boost::bind(
											&this_type::template handler_register_dispatch<handler_type>, this, _1,
											boost::cref(id), mode, boost::cref(val), boost::cref(handler)
										)
									);
	}

	template<typename Change_Handler>
	std::pair<u32, u32> register_dispatch(const id_type& id, u32 mode, const proxy_val_type& val,
											const Change_Handler& handler)
	{
		typedef Change_Handler handler_type;
		return _dis_map.use_handler(
								boost::bind(
												&this_type::template handler_register_dispatch<handler_type>, this, _1,
												boost::cref(id), mode, boost::cref(val), boost::cref(handler)
											)
									);
	}

	template<typename RealData>
	std::pair<u32, u32> exchange_mode(u32 mode)
	{
		tmp_handler_type tmp_handler;
		return this_type::exchange_mode<RealData>(mode, tmp_handler);
	}

	std::pair<u32, u32> exchange_mode(const id_type& id, u32 mode)
	{
		tmp_handler_type tmp_handler;
		return this_type::exchange_mode(id, mode, tmp_handler);
	}

	template<typename RealData, typename Change_Handler>
	std::pair<u32, u32> exchange_mode(u32 mode, const Change_Handler& handler)
	{
		typedef RealData real_data_type;
		typedef Change_Handler handler_type;
		id_type id = id_parser_type::template get_reg_id<real_data_type>();
		return _dis_map.use_handler(
							boost::bind(
											&this_type::template handler_exchange_mode<handler_type>, this, _1,
											boost::cref(id), mode, boost::cref(handler)
										)
									);
	}

	template<typename Change_Handler>
	std::pair<u32, u32> exchange_mode(const id_type& id, u32 mode, const Change_Handler& handler)
	{
		typedef Change_Handler handler_type;
		return _dis_map.use_handler(
								boost::bind(
												&this_type::template handler_exchange_mode<handler_type>, this, _1,
												boost::cref(id), mode, boost::cref(handler)
											)
									);
	}

	template<typename RealData>
	bool remove_dispatch(const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		return this_type::remove_dispatch(val, tmp_handler);
	}

	bool remove_dispatch(const id_type& id, const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		return this_type::remove_dispatch(id, val, tmp_handler);
	}

	template<typename RealData, typename Handler>
	bool remove_dispatch(const proxy_val_type& val, const Handler& handler)
	{
		typedef RealData real_data_type;
		typedef Handler handler_type;

		id_type id = id_parser_type::template get_reg_id<real_data_type>();
		return _dis_map.use_handler(
										boost::bind(
														&this_type::template handler_remove_dispatch<handler_type>,
														this, _1,
														boost::cref(id),
														boost::cref(val),
														boost::cref(handler)
													)
									);
	}

	template<typename Handler>
	bool remove_dispatch(const id_type& id, const proxy_val_type& val, const Handler& handler)
	{
		typedef Handler handler_type;
		return _dis_map.use_handler(
										boost::bind(
														&this_type::template handler_remove_dispatch<handler_type>,
														this, _1,
														boost::cref(id),
														boost::cref(val),
														boost::cref(handler)
													)
									);
	}

	void clear_dispatch_of_id(const id_type& id)
	{
		_dis_map.use_handler(boost::bind(&this_type::handler_clear_of_id, this, _1, boost::cref(id)));
	}

	void clear_dispatch_of_proxy(const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		clear_dispatch_of_proxy(val, tmp_handler);
	}

	template<typename Handler>
	void clear_dispatch_of_proxy(const proxy_val_type& val, const Handler& handler)
	{
		typedef Handler handler_type;
		if(!_proxy_val_map.is_exists(val))
		{
			return;
		}
		_dis_map.use_handler(
					boost::bind(
									&this_type::handler_clear_of_proxy<handler_type>,
									this, _1, boost::cref(val), boost::ref(handler)
								)
							);
	}

	void clear(void)
	{
		_dis_map.use_handler(boost::bind(&this_type::handler_clear, this, _1));
	}

	template<typename Runner, typename Handler>
	void send(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type)
	{
		if(is_proxy_data(owner))
		{
			send_to_back(owner, proxy_data, prunner, handler, cal_type);
			return;
		}

		send_to_proxy(owner, proxy_data, prunner, handler, cal_type);
	}

protected:

	template<typename Handler>
	void handler_clear_of_proxy(typename dispatch_map_type::base_type& base,
									const proxy_val_type& val,
									const Handler& handler)
	{
		typedef Handler handler_type;
		_proxy_val_map.use_handler(
						boost::bind(
										&this_type::handler_remove_container_proxy<handler_type>,
										this, _1, boost::ref(base), boost::cref(val), boost::cref(handler)
									)
								);
	}

	template<typename Handler>
	void handler_remove_container_proxy(typename proxy_val_map_type::base_type& base_proxy,
											typename dispatch_map_type::base_type& base_dis,
											const proxy_val_type& val,
											const Handler& handler)
	{
		typedef typename proxy_val_map_type::iterator iter_type;
		typedef typename dispatch_set_type::iterator dis_set_iter_type;
		iter_type iter = base_proxy.find(val);
		if(iter == base_proxy.end())
		{
			return;
		}

		for(dis_set_iter_type i = iter->second.begin(), isize = iter->second.end();
				i != isize; ++i)
		{
			assert(*i);
			if(!(*i))
			{
				continue;
			}
			(*i)->remove_proxy(val);

			if((*i)->empty_of_proxy())
			{
				id_type id((*i)->id());
				handler(*i);
				base_dis.erase(id);
			}
		}

		base_proxy.erase(iter);
	}

	void handler_clear_of_id(typename dispatch_map_type::base_type& base, const id_type& id)
	{
		typedef std::vector<proxy_val_type> proxy_vals_type;
		typedef typename proxy_vals_type::iterator proxy_iter_type;
		typedef typename dispatch_map_type::iterator iter_type;
		iter_type iter = base.find(id);

		if(iter == base.end())
		{
			return;
		}

		assert(iter->second);
		if(!iter->second)
		{
			base.erase(iter);
			return;
		}

		proxy_vals_type proxy_vals;
		iter->second->get_proxy_list(proxy_vals);

		if(!proxy_vals.empty())
		{
			_proxy_val_map.use_handler(
								boost::bind(
												&this_type::template handler_clear_of_id<proxy_vals_type>, this,
												_1, boost::cref(proxy_vals)
											)
										);
		}

		base.erase(iter);
	}

	void handler_clear(typename dispatch_map_type::base_type& base)
	{
		_proxy_val_map.clear();
		base.clear();
	}

	template<typename Runner, typename Handler>
	void send_to_back(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type)
	{

		owner_info_type info(owner);
		info.pop();
		owner_info_container_type owners;
		owners.insert(info);

		task_type& ref_data = const_cast<task_type&>(proxy_data);
		handler(prunner, error_maker_type::make_error(0),
					yggr::task_center::runtime_task_type::E_CAL_RESULT,
					owners, proxy_data.data_info(), cal_type, boost::ref(ref_data.org_buf()));
	}

	template<typename Runner, typename Handler>
	void send_to_proxy(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type)
	{


		id_type id(id_parser_type::get_task_id(proxy_data));

		proxy_container_ptr_type pcont;

		if(!(_dis_map.get_value(id, pcont) && pcont))
		{
			return;
		}

		assert(pcont);
		pcont->send(owner, proxy_data, prunner, handler, cal_type);
	}

	bool is_proxy_data(const owner_info_type& owner)
	{
		return _proxy_val_map.is_exists(owner.owner_id());
	}

	bool handler_plus_proxy_ref(typename proxy_val_map_type::base_type& base,
									const proxy_val_type& val,
									const proxy_container_ptr_type& pcont)
	{
		typedef typename proxy_val_map_type::iterator iter_type;
		typedef typename proxy_val_map_type::value_type ins_value_type;
		typedef std::pair<iter_type, bool> ins_rst_type;
		iter_type iter = base.find(val);
		if(iter == base.end())
		{
			ins_rst_type ins_rst = base.insert(ins_value_type(val, dispatch_set_type()));
			assert(ins_rst.second);
			iter = ins_rst.first;
		}

		iter->second.insert(pcont);

		return true;
	}

	template<typename Container>
	void handler_sub_proxy_ref_of_list(typename proxy_val_map_type::base_type& base,
									const Container& vals,
									const proxy_container_ptr_type& pcont)
	{
		typedef Container container_type;
		typedef typename container_type::iterator container_iter_type;
		typedef typename proxy_val_map_type::iterator iter_type;

		iter_type iter;
		for(container_iter_type i = vals.begin(), isize = vals.end(); i != isize; ++i)
		{
			//iter = base.find(val);
			iter = base.find(*i);
			if(iter == base.end())
			{
				continue;
			}

			iter->second.erase(pcont);

			if(iter->second.empty())
			{
				base.erase(iter);
			}
		}

		return;
	}

	void handler_minus_proxy_ref(typename proxy_val_map_type::base_type& base,
									const proxy_val_type& val,
									const proxy_container_ptr_type& pcont)
	{
		typedef typename proxy_val_map_type::iterator iter_type;

		iter_type iter = base.find(val);
		if(iter == base.end())
		{
			return;
		}

		iter->second.erase(pcont);

		if(iter->second.empty())
		{
			base.erase(iter);
		}

		return;
	}

	template<typename Handler>
	std::pair<u32, u32> handler_register_dispatch(typename dispatch_map_type::base_type& base,
													const id_type& id,
													u32 mode,
													const proxy_val_type& proxy_val,
													const Handler& handler)
	{
		typedef std::pair<u32, u32> ret_type;
		typedef typename dispatch_map_type::iterator iter_type;
		typedef typename dispatch_map_type::value_type ins_value_type;
		typedef std::pair<iter_type, bool> insert_rst_type;

		//return ret_type(0, 0);

		iter_type iter = base.find(id);
		if(iter == base.end())
		{
			insert_rst_type rst = base.insert(
												ins_value_type(
																id,
																proxy_container_ptr_type(
																	new proxy_container_type(
																								_mode_tactics,
																								_mode_creator,
																								id
																							)
																						)
															)
												);
			if(rst.second)
			{
				iter = rst.first;
			}
			else
			{
				return ret_type(u32(), fix_state_def_type::E_failed);
			}
		}

		assert(iter != base.end());
		_proxy_val_map.use_handler(
									boost::bind(
													&this_type::handler_plus_proxy_ref, this,
													_1, boost::cref(proxy_val), boost::cref(iter->second)
												)
								);

		ret_type ret =  iter->second->append_proxy(mode, proxy_val);

		if(ret.second == fix_state_def_type::E_changed)
		{
			handler(iter->second);
		}
		return ret;
	}

	template<typename Handler>
	bool handler_remove_dispatch(typename dispatch_map_type::base_type& base,
									const id_type& id,
									const proxy_val_type& proxy_val,
									const Handler& handler)
	{
		typedef typename dispatch_map_type::const_iterator iter_type;

		iter_type iter = base.find(id);
		if(iter == base.end())
		{
			return false;
		}

		assert(iter->second);
		if(!iter->second)
		{
			return false;
		}

		bool brst = iter->second->remove_proxy(proxy_val);

		if(brst)
		{
			_proxy_val_map.use_handler(
								boost::bind(
												&this_type::handler_minus_proxy_ref, this,
												_1, boost::cref(proxy_val), boost::cref(iter->second)
											)
										);
		}

		if(iter->second->empty_of_proxy())
		{
			handler(iter->second);
			base.erase(iter);
		}
		return brst;
		//return false;
	}

	template<typename Handler>
	std::pair<u32, u32> handler_exchange_mode(typename dispatch_map_type::base_type& base,
													const id_type& id,
													u32 mode,
													const Handler& handler)
	{
		typedef std::pair<u32, u32> ret_type;
		typedef typename dispatch_map_type::iterator iter_type;

		iter_type iter = base.find(id);
		if(iter == base.end())
		{
			return ret_type(u32(), fix_state_def_type::E_failed);
		}

		assert(iter != base.end());

		ret_type ret =  iter->second->exchange_mode(mode);

		if(ret.second == fix_state_def_type::E_changed)
		{
			handler(iter->second);
		}
		return ret;
	}

protected:
	mode_tactics_type _mode_tactics;
	mode_creator_type _mode_creator;
	dispatch_map_type _dis_map;
	proxy_val_map_type _proxy_val_map;
};

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_DISPATCHER_HPP__
