//proxy_dispatcher.hpp

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

#ifndef __YGGR_PROXY_PROXY_DISPATCHER_HPP__
#define __YGGR_PROXY_PROXY_DISPATCHER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/move/move.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/utility/args_holder_nv.hpp>
#include <yggr/task_center/task_info_def.hpp>

#include <yggr/container/unordered_set.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/range/functions.hpp>

namespace yggr
{
namespace proxy
{
namespace detail
{

template<typename ProxyContainerPtr>
struct proxy_dispatcher_tmp_handler
{
	typedef ProxyContainerPtr proxy_container_ptr_type;

	inline void operator()(const proxy_container_ptr_type&) const{}
};

} // namespace detail
} // namespace proxy
} // namespace yggr

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
	typedef ::yggr::shared_ptr<proxy_container_type> proxy_container_ptr_type;

	typedef typename proxy_container_type::task_type task_type;
	typedef typename proxy_container_type::owner_info_container_type owner_info_container_type;
	typedef typename proxy_container_type::owner_info_type owner_info_type;
	typedef typename proxy_container_type::value_type proxy_val_type;
	typedef typename proxy_container_type::fix_state_def_type fix_state_def_type;

	typedef typename id_parser_type::id_type id_type;

protected:

	BOOST_MPL_ASSERT((boost::is_same<id_type, typename proxy_container_type::id_type>));
	typedef typename proxy_container_type::mode_tactics_type mode_tactics_type;
	typedef typename proxy_container_type::mode_creator_type mode_creator_type;

	typedef safe_container::safe_unordered_map<id_type, proxy_container_ptr_type> dispatch_map_type;

	typedef container::unordered_set<proxy_container_ptr_type> dispatch_set_type;
	typedef safe_container::safe_unordered_map<proxy_val_type, dispatch_set_type> proxy_val_map_type;

private:
	typedef detail::proxy_dispatcher_tmp_handler<proxy_container_ptr_type> tmp_handler_type;

	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef proxy_dispatcher this_type;

public:
	proxy_dispatcher(void)
	{
	}

	~proxy_dispatcher(void)
	{
	}

public:
	template<typename RealData> inline
	std::pair<u32, u32> register_dispatch(u32 mode, const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		return this_type::register_dispatch<RealData>(mode, val, tmp_handler);
	}

	inline std::pair<u32, u32> 
		register_dispatch(const id_type& id, u32 mode, const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		return this_type::register_dispatch(id, mode, val, tmp_handler);
	}

	template<typename RealData, typename Change_Handler> inline
	std::pair<u32, u32> register_dispatch(u32 mode, const proxy_val_type& val,
											const Change_Handler& handler)
	{
		typedef RealData real_data_type;

		id_type id = id_parser_type::template get_reg_id<real_data_type>();
		return this_type::register_dispatch(id, mode, val, handler);
	}

	template<typename Change_Handler> inline
	std::pair<u32, u32> register_dispatch(const id_type& id, u32 mode, const proxy_val_type& val,
											const Change_Handler& handler)
	{
		typedef Change_Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		handler_holder_type handler_holder(handler);
		return 
			_dis_map.find(
				id, 
				boost::bind(
					&this_type::handler_register_dispatch<handler_type>, 
					this, _1, _2,  boost::cref(id), mode, 
					boost::cref(val), boost::cref<basic_args_holder_nv_type>(handler_holder) ) );
	}

	template<typename RealData> inline
	std::pair<u32, u32> exchange_mode(u32 mode)
	{
		tmp_handler_type tmp_handler;
		return this_type::exchange_mode<RealData>(mode, tmp_handler);
	}

	inline std::pair<u32, u32> exchange_mode(const id_type& id, u32 mode)
	{
		tmp_handler_type tmp_handler;
		return this_type::exchange_mode(id, mode, tmp_handler);
	}

	template<typename RealData, typename Change_Handler> inline
	std::pair<u32, u32> exchange_mode(u32 mode, const Change_Handler& handler)
	{
		typedef RealData real_data_type;

		id_type id = id_parser_type::template get_reg_id<real_data_type>();
		return this_type::exchange_mode(id, mode, handler);
	}

	template<typename Change_Handler> inline
	std::pair<u32, u32> exchange_mode(const id_type& id, u32 mode, const Change_Handler& handler)
	{
		typedef Change_Handler handler_type;
		typedef std::pair<u32, u32> ret_type;

		proxy_container_ptr_type pcont;
		_dis_map.get_value(id, pcont);

		ret_type ret(0, fix_state_def_type::E_failed);
		if(pcont)
		{
			ret = pcont->exchange_mode(mode);
			if(ret.second == fix_state_def_type::E_changed)
			{
				handler(pcont);
			}
		}

		return ret;
	}

	template<typename RealData> inline
	bool remove_dispatch(const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		return this_type::remove_dispatch(val, tmp_handler);
	}

	inline bool remove_dispatch(const id_type& id, const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		return this_type::remove_dispatch(id, val, tmp_handler);
	}

	template<typename RealData, typename Handler> inline
	bool remove_dispatch(const proxy_val_type& val, const Handler& handler)
	{
		typedef RealData real_data_type;

		id_type id = id_parser_type::template get_reg_id<real_data_type>();
		return this_type::remove_dispatch(id, val, handler);
	}

	template<typename Handler> inline
	bool remove_dispatch(const id_type& id, const proxy_val_type& val, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		handler_holder_type handler_holder(handler);
		return 
			_dis_map.find(
				id,
				boost::bind(
					&this_type::handler_remove_dispatch<handler_type>,
					this, _1, _2, boost::cref(val), 
					boost::cref<basic_args_holder_nv_type>(handler_holder) ) );
	}

	inline void clear_dispatch_of_id(const id_type& id)
	{
		_dis_map.find(boost::bind(&this_type::handler_clear_of_id, this, _1, _2));
	}

	inline void clear_dispatch_of_proxy(const proxy_val_type& val)
	{
		tmp_handler_type tmp_handler;
		clear_dispatch_of_proxy(val, tmp_handler);
	}

	template<typename Handler> inline
	void clear_dispatch_of_proxy(const proxy_val_type& val, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		if(_proxy_val_map.is_exists(val))
		{
			handler_holder_type handler_holder(handler);
			_dis_map.using_handler(
				boost::bind(&this_type::handler_clear_of_proxy<handler_type>,
								this, _1, 
								boost::cref(val), 
								boost::cref<basic_args_holder_nv_type>(handler_holder) ) );
		}
	}
	
	inline void clear(void)
	{
		typedef typename dispatch_map_type::base_type base_dis_map_type;
		typedef typename proxy_val_map_type::base_type base_proxy_val_map_type;

		base_dis_map_type dis_map;
		base_proxy_val_map_type pv_map;

		_dis_map.using_handler(
			boost::bind(
				&this_type::handler_clear, 
					this, _1, boost::ref(dis_map), boost::ref(pv_map) ) );
	}

	template<typename Handler> inline
	void clear(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		typedef typename dispatch_map_type::base_type base_dis_map_type;
		typedef typename proxy_val_map_type::base_type base_proxy_val_map_type;

		base_dis_map_type dis_map;
		base_proxy_val_map_type pv_map;

		handler_holder_type handler_holder(handler);

		_dis_map.using_handler(
			boost::bind(
				&this_type::handler_clear_eins<handler_type>, 
				this, _1, 
				boost::ref(dis_map), boost::ref(pv_map),
				boost::cref<basic_args_holder_nv_type>(handler_holder) ) );
	}

	inline bool is_proxy_data(const owner_info_type& owner) const
	{
		return _proxy_val_map.is_exists(owner.owner_id());
	}

	template<typename Runner, typename Handler> inline
	void send(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type) const
	{
		if(is_proxy_data(owner))
		{
			send_to_back(owner, proxy_data, prunner, handler, cal_type);
		}
		else
		{
			send_to_proxy(owner, proxy_data, prunner, handler, cal_type);
		}
	}

protected:
	template<typename Handler> inline
	void handler_clear_of_proxy(typename dispatch_map_type::base_type& base,
									const proxy_val_type& val,
									const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;

		_proxy_val_map.find(
			val, 
			boost::bind( 
				&this_type::handler_remove_container_proxy<handler_type>,
				this, _1, _2, boost::ref(base), boost::cref(any_handler) ) );
	}

	template<typename Handler> 
	void handler_remove_container_proxy(typename proxy_val_map_type::base_type& base_proxy,
											typename proxy_val_map_type::iterator rst,
											typename dispatch_map_type::base_type& base_dis,
											const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		typedef typename dispatch_set_type::iterator dis_set_iter_type;

		const handler_type* phandler = utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);
		assert(phandler);
		if((rst == base_proxy.end()) || (!phandler))
		{
			return;
		}

		const handler_type& handler = *phandler;

		for(dis_set_iter_type i = rst->second.begin(), isize = rst->second.end();
				i != isize; ++i)
		{
			assert(*i);
			if(!(*i))
			{
				continue;
			}

			(*i)->remove_proxy(rst->first);

			if((*i)->empty_of_proxy())
			{
				id_type id((*i)->id());
				handler(*i);
				base_dis.erase(id);
			}
		}

		base_proxy.erase(rst);
	}

	inline void handler_clear_of_id(typename dispatch_map_type::base_type& base,
								typename dispatch_map_type::iterator rst)
	{
		typedef yggr::vector<proxy_val_type> proxy_vals_type;

		if(rst == base.end())
		{
			return;
		}

		assert(rst->second);
		if(!rst->second)
		{
			base.erase(rst);
			return;
		}

		proxy_vals_type proxy_vals;
		rst->second->swap_proxy_list(proxy_vals);

		if(!proxy_vals.empty())
		{
			_proxy_val_map.using_handler(
				boost::bind(
					&this_type::handler_minus_proxy_ref_of_list<proxy_vals_type>, 
					this, _1, boost::cref(proxy_vals), boost::cref(rst->second) ) );
		}

		base.erase(rst);
	}

	inline void handler_clear(typename dispatch_map_type::base_type& base, 
								typename dispatch_map_type::base_type& out_dis,
								typename proxy_val_map_type::base_type& out_proxy_val)
	{
		_proxy_val_map.swap(out_proxy_val);
		base.swap(out_dis);
	}

	template<typename Handler> inline
	void handler_clear_eins(typename dispatch_map_type::base_type& base, 
							typename dispatch_map_type::base_type& out_dis,
							typename proxy_val_map_type::base_type& out_proxy_val,
							const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;

		_proxy_val_map.using_handler(
			boost::bind(
				&this_type::handler_clear_zwei<handler_type>,
				this, boost::ref(base), _1, 
				boost::ref(out_dis), boost::ref(out_proxy_val),
				boost::cref(any_handler)));
	}

	template<typename Handler>
	void handler_clear_zwei(typename dispatch_map_type::base_type& base1,
								typename proxy_val_map_type::base_type& base, 
								typename dispatch_map_type::base_type& out_dis,
								typename proxy_val_map_type::base_type& out_proxy_val,
								const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		typedef typename dispatch_map_type::iterator iter_type;

		base1.swap(out_dis);
		base.swap(out_proxy_val);

		const handler_type* phandler = utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);
		assert(phandler);

		if(!phandler)
		{
			return;
		}

		const handler_type& handler = *phandler;

		for(iter_type i = out_dis.begin(), isize = out_dis.end(); i != isize; ++i)
		{
			handler(i->second);
		}
	}

	template<typename Runner, typename Handler> inline
	void send_to_back(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type) const
	{

		owner_info_type info(owner);
		info.pop();
		owner_info_container_type owners;
		iterator_ex::inserter(owners, boost::end(owners)) = boost::move(info);

		task_type& ref_data = const_cast<task_type&>(proxy_data);
		handler(
			prunner, error_maker_type::make_error(0),
			yggr::task_center::runtime_task_type::E_CAL_RESULT,
			owners, proxy_data.data_info(), cal_type, boost::ref(ref_data.org_buf()));
	}

	template<typename Runner, typename Handler> inline
	void send_to_proxy(const owner_info_type& owner,
						const task_type& proxy_data,
						Runner* prunner,
						const Handler& handler,
						u32 cal_type) const
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

	template<typename Runner, typename Handler, typename Args> inline
	void send_to_proxy(const owner_info_type& owner,
						const task_type& proxy_data,
						Runner* prunner,
						const Handler& handler,
						u32 cal_type,
						Args& args) const
	{


		id_type id(id_parser_type::get_task_id(proxy_data));

		proxy_container_ptr_type pcont;

		if(!(_dis_map.get_value(id, pcont) && pcont))
		{
			return;
		}

		assert(pcont);
		pcont->send(owner, proxy_data, prunner, handler, cal_type, args);
	}


	inline bool handler_plus_proxy_ref(typename proxy_val_map_type::base_type& base,
										typename proxy_val_map_type::iterator rst,
										const proxy_val_type& val,
										const proxy_container_ptr_type& pcont)
	{
		assert(pcont);
		if(rst == base.end())
		{
			dispatch_set_type tmp;
			tmp.insert(pcont);
			try
			{
				base[val].swap(tmp);
			}
			catch(const ::yggr::stl_exception&)
			{
				return false;
			}
		}
		else
		{
			rst->second.insert(pcont);
		}

		return true;
	}

	template<typename Container> inline
	void handler_minus_proxy_ref_of_list(typename proxy_val_map_type::base_type& base,
											const Container& vals,
											const proxy_container_ptr_type& pcont)
	{
		typedef Container container_type;
		typedef typename container_type::iterator container_iter_type;
		typedef typename proxy_val_map_type::iterator iter_type;

		iter_type iter;
		for(container_iter_type i = vals.begin(), isize = vals.end(); i != isize; ++i)
		{
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

	inline void handler_minus_proxy_ref(typename proxy_val_map_type::base_type& base,
										typename proxy_val_map_type::iterator rst,
										const proxy_container_ptr_type& pcont)
	{
		if(rst == base.end())
		{
			return;
		}

		rst->second.erase(pcont);

		if(rst->second.empty())
		{
			base.erase(rst);
		}
	}

	template<typename Handler> inline
	std::pair<u32, u32> 
		handler_register_dispatch(typename dispatch_map_type::base_type& base,
									typename dispatch_map_type::iterator rst,
									const id_type& id,
									u32 mode,
									const proxy_val_type& proxy_val,
									const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		typedef std::pair<u32, u32> ret_type;

		const handler_type* phandler = utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);
		if(!phandler)
		{
			return ret_type(0, fix_state_def_type::E_failed);
		}

		const handler_type& handler = *phandler;

		proxy_container_ptr_type pcont;
		bool bempty = rst == base.end();
		if(bempty)
		{
			pcont = 
				construct_shared<proxy_container_type>(
					yggr_nothrow_new proxy_container_type( _mode_tactics, _mode_creator, id ) );

			if(!pcont)
			{
				return ret_type(0, fix_state_def_type::E_failed);
			}

			try
			{
				base[id] = pcont;
			}
			catch(const ::yggr::stl_exception&)
			{
				return ret_type(0, fix_state_def_type::E_failed);
			}
		}
		else
		{
			pcont = rst->second;
		}

		assert(pcont);
		if(!_proxy_val_map.find(proxy_val, 
									boost::bind(&this_type::handler_plus_proxy_ref, 
													this, _1, _2,
													boost::cref(proxy_val), boost::cref(pcont) ) ))
		{
			if(bempty)
			{
				base.erase(id);
			}
			return ret_type(0, fix_state_def_type::E_failed);
		}
		else
		{
			ret_type ret = pcont->append_proxy(mode, proxy_val);

			if(ret.second == fix_state_def_type::E_changed)
			{
				handler(pcont);
			}

			return ret;
		}
	}

	template<typename Handler> inline
	bool handler_remove_dispatch(typename dispatch_map_type::base_type& base,
									typename dispatch_map_type::iterator rst,
									const proxy_val_type& proxy_val,
									const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;

		const handler_type* phandler = utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);
		if(!phandler)
		{
			return false;
		}

		const handler_type& handler = *phandler;

		if(rst == base.end())
		{
			return false;
		}

		assert(rst->second);
		if(!rst->second)
		{
			// if using base.erase(rst) error C2668 umap::erase occured,
			// must conv to const_iterator // why???
			container::erase(base, rst);
			return false;
		}

		bool brst = rst->second->remove_proxy(proxy_val);

		if(brst)
		{
			_proxy_val_map.find(proxy_val,
									boost::bind( &this_type::handler_minus_proxy_ref, 
												this, _1, _2, boost::cref(rst->second) ) );
		}

		if(rst->second->empty_of_proxy())
		{
			handler(rst->second);
			container::erase(base, rst);
		}
		return brst;
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
