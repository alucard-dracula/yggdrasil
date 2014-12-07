//sync_container_mgr.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_SYNC_SYSTEM_SYNC_CONTAINER_MGR_HPP__
#define __YGGR_SYNC_SYSTEM_SYNC_CONTAINER_MGR_HPP__

#include <map>
//#include <boost/unordered_map.hpp>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>

#include <yggr/base/interface_ptr.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/sync_system/sync_helper.hpp>

namespace yggr
{
namespace sync_system
{

template<typename Key, typename Params,
		template<typename _Params> class Helper = sync_helper>
class sync_container_mgr : private nonable::noncopyable
{
public:
	typedef Key key_type;
	typedef Params params_type;
	typedef Helper<params_type> helper_type;

private:
	class base_sync_container
	{
	public:
		virtual ~base_sync_container(void)
		{
		}

		virtual bool recv(helper_type& helper) = 0;
		//virtual bool recv(helper_type& helper) const = 0;
		virtual bool send(helper_type& helper) = 0;
		//virtual bool send(helper_type& helper) const = 0;
		virtual bool execute(const helper_type& in_helper, helper_type& out_helper) = 0;
		//virtual bool execute(const helper_type& in_helper, helper_type& out_helper) const = 0;
	};

	typedef interface_ptr<base_sync_container> interface_sync_container_ptr_type;
	typedef safe_container::safe_wrap<base_sync_container,
										boost::mutex,
										interface_sync_container_ptr_type> base_sync_container_wrap_type;
	typedef typename base_sync_container_wrap_type::error_type base_sync_container_wrap_error_type;
	typedef boost::shared_ptr<base_sync_container_wrap_type> base_sync_container_wrap_ptr_type;

	template<typename Val>
	class sync_container : public base_sync_container
	{
	public:
		typedef Val val_type;
		typedef boost::shared_ptr<val_type> val_ptr_type;
	public:

		sync_container(void)
			: _ptr(new val_type())
		{
		}

		//template<typename Init>
		//sync_container(const Init& init)
		//	: _ptr(new val_type(init))
		//{
		//}



		sync_container(const val_ptr_type& ptr)
			: _ptr(ptr)
		{
		}

		virtual ~sync_container(void)
		{
			val_ptr_type().swap(_ptr);
		}

		virtual bool recv(helper_type& helper)
		{
			if(!_ptr)
			{
				return false;
			}
			val_type& val = *_ptr;
			return helper.recv(val);
		}

		//virtual bool recv(helper_type& helper) const
		//{
		//	if(!_ptr)
		//	{
		//		return false;
		//	}
		//	const val_type& val = *_ptr;
		//	return helper.recv(val);
		//}

		virtual bool send(helper_type& helper)
		{
			return _ptr && (*_ptr << helper);
		}

		//virtual bool send(helper_type& helper) const
		//{
		//	return _ptr && (*_ptr << helper);
		//}

		virtual bool execute(const helper_type& in_helper, helper_type& out_helper)
		{
			return _ptr && _ptr->execute(in_helper, out_helper);
		}

		//virtual bool execute(const helper_type& in_helper, helper_type& out_helper) const
		//{
		//	return _ptr && _ptr->execute(in_helper, out_helper);
		//}

		/*virtual bool execute(void)
		{
			return _ptr && _ptr->execute();
		}

		virtual bool execute(void) const
		{
			return _ptr && _ptr->execute();
		}*/

	private:
		val_ptr_type _ptr;
	};

	//typedef boost::unordered_map<key_type, interface_sync_container_ptr_type> reg_map_type;
	typedef safe_container::safe_unordered_map<key_type, base_sync_container_wrap_ptr_type> reg_map_type;
	typedef typename reg_map_type::iterator reg_map_iter_type;
	typedef typename reg_map_type::const_iterator reg_map_citer_type;

	typedef sync_container_mgr this_type;

public:
	sync_container_mgr(void)
	{
	}

	~sync_container_mgr(void)
	{
		_reg_map.clear();
	}

	template<typename T>
	bool reg_container(void)
	{
		typedef T val_type;
		return reg_container<val_type>(val_type::type_id());
	}

	template<typename T>
	bool reg_container(const key_type& key)
	{
		typedef T val_type;
		typedef sync_container<val_type> sync_container_type;

		interface_sync_container_ptr_type iptr(new sync_container_type());
		base_sync_container_wrap_ptr_type wptr(new base_sync_container_wrap_type(iptr));

		return iptr && wptr && _reg_map.insert(key, wptr);
	}

	/*template<typename T>
	bool reg_container(const key_type& key, const boost::shared_ptr<T>& ptr)
	{
		typedef T val_type;
		typedef sync_container<val_type> sync_container_type;

		if(!ptr || _reg_map.find(key) != base.end())
		{
			return false;
		}

		interface_sync_container_ptr_type iptr(new sync_container_type(ptr));
		if(!iptr)
		{
			return false;
		}

		_reg_map[key] = iptr;
		return true;
	}

	template<typename T>
	bool reg_container(const boost::shared_ptr<T>& ptr)
	{
		typedef T val_type;
		return reg_container<val_type>(val_type::type_id(), ptr);
	}*/

	template<typename T, typename Init>
	bool reg_container(const Init& init)
	{
		typedef T val_type;
		typedef Init init_type;
		return reg_container<val_type, init_type>(val_type::type_id(), init);
	}

	template<typename T, typename Init>
	bool reg_container(const key_type& key, const Init& init)
	{
		typedef T val_type;
		typedef sync_container<val_type> sync_container_type;

		interface_sync_container_ptr_type iptr(new sync_container_type(init));
		base_sync_container_wrap_ptr_type wptr(new base_sync_container_wrap_type(iptr));
		return iptr && wptr && _reg_map.insert(key, wptr);
	}

	bool unreg_container(const key_type& key)
	{
		return _reg_map.erase(key);
	}

	//template<typename Val>
	//bool send(const key_type& key, const Val& val)
	//{
	//	helper_type helper(params_type(), val);
	//	return send(key, helper);
	//}

	template<typename Val>
	bool send(const key_type& key, const Val& val) const
	{
		helper_type helper(params_type(), val);
		return send(key, helper);
	}

	//template<typename Val>
	//bool const_send(const key_type& key, const Val& val) const
	//{
	//	return send(key, val);
	//}

	//template<typename Cdt, typename Val>
	//bool send(const key_type& key, const Cdt& cdt, const Val& val)
	//{
	//	helper_type helper(cdt, val);
	//	return send(key, helper);
	//}

	template<typename Cdt, typename Val>
	bool send(const key_type& key, const Cdt& cdt, const Val& val) const
	{
		helper_type helper(cdt, val);
		return send(key, helper);
	}

	//template<typename Cdt, typename Val>
	//bool const_send(const key_type& key, const Cdt& cdt, const Val& val) const
	//{
	//	return send(key, cdt, val);
	//}

	//template<typename Val>
	//bool send_all(const Val& val)
	//{
	//	helper_type helper(params_type(), val);
	//	return send_all(helper);
	//}

	template<typename Val>
	bool send_all(const Val& val) const
	{
		helper_type helper(params_type(), val);
		return send_all(helper);
	}

	//template<typename Val>
	//bool const_send_all(const Val& val) const
	//{
	//	return send_all(val);
	//}

	//template<typename Cdt, typename Val>
	//bool send_all(const Cdt& cdt, const Val& val)
	//{
	//	helper_type helper(cdt, val);
	//	return send(helper);
	//}

	template<typename Cdt, typename Val>
	bool send_all(const Cdt& cdt, const Val& val) const
	{
		helper_type helper(cdt, val);
		return send(helper);
	}

	//template<typename Cdt, typename Val>
	//bool const_send_all(const Cdt& cdt, const Val& val) const
	//{
	//	return send_all(cdt, val);
	//}

	//template<typename Val>
	//bool recv(const key_type& key, Val& val)
	//{
	//	helper_type helper(params_type(), val);
	//	return recv(key, helper) && (val << helper);
	//}

	template<typename Val>
	bool recv(const key_type& key, Val& val) const
	{
		helper_type helper(params_type(), val);
		return recv(key, helper) && (val << helper);
	}

	//template<typename Val>
	//bool const_recv(const key_type& key, Val& val) const
	//{
	//	return recv(key, val);
	//}

	//template<typename Cdt, typename Val>
	//bool recv(const key_type& key, const Cdt& cdt, Val& val)
	//{
	//	helper_type helper(cdt, val);
	//	return recv(key, helper) && (val << helper);
	//}

	template<typename Cdt, typename Val>
	bool recv(const key_type& key, const Cdt& cdt, Val& val) const
	{
		helper_type helper(cdt, val);
		return recv(key, helper) && (val << helper);
	}

	//template<typename Cdt, typename Val>
	//bool const_recv(const key_type& key, const Cdt& cdt, Val& val) const
	//{
	//	return recv(key, cdt, val);
	//}

	//template<typename Container>
	//size_type recv_all(Container& container)
	//{
	//	return recv_all_of_map(container.map_container());
	//	//return recv_all(container.map_container());
	//}

	template<typename Container>
	size_type recv_all(Container& container) const
	{
		return recv_all_of_map(container.map_container());
		//return recv_all(container.map_container());
	}

	//template<typename Container>
	//size_type const_recv_all(Container& container) const
	//{
	//	return recv_all_of_map(container.map_container());
	//	//return recv_all(container.map_container());
	//}

	//bool execute(const key_type& key)
	//{
	//    helper_type in_helper, out_helper;
	//	return execute(key, in_helper, out_helper);
	//}

	bool execute(const key_type& key) const
	{
		helper_type rst;
		return execute(key, helper_type(), rst);
	}

	//bool const_execute(const key_type& key) const
	//{
	//	return execute(key);
	//}

	//template<typename Condition>
	//bool execute(const key_type& key, const Condition& cdt)
	//{
	//	helper_type helper(cdt);
	//	return execute(key, helper, helper_type());

	//}

	template<typename Condition>
	bool execute(const key_type& key, const Condition& cdt) const
	{
		helper_type helper(cdt);
		return execute(key, helper, helper_type());
	}

	//template<typename Condition>
	//bool const_execute(const key_type& key, const Condition& cdt) const
	//{
	//	return execute(key, cdt);
	//}

	//template<typename Condition, typename Result>
	//bool execute(const key_type& key, const Condition& cdt, Result& rst)
	//{
	//	helper_type in_helper(cdt), out_helper;
	//	return (execute(key, in_helper, out_helper)) && (rst << out_helper);
	//}

	template<typename Condition, typename Result>
	bool execute(const key_type& key, const Condition& cdt, Result& rst) const
	{
		helper_type in_helper(cdt), out_helper;
		return (execute(key, in_helper, out_helper)) && (rst << out_helper);
	}

	//template<typename Condition, typename Result>
	//bool const_execute(const key_type& key, const Condition& cdt, Result& rst) const
	//{
	//	return execute(key, cdt);
	//}

	//size_type execute_all(void)
	//{
	//    helper_type in_helper, out_helper;
	//	return execute_all(in_helper, out_helper);
	//}

	size_type execute_all(void) const
	{
	    helper_type in_helper, out_helper;
		return execute_all(in_helper, out_helper);
	}

	//size_type const_execute_all(void) const
	//{
	//	return execute_all();
	//}

	//template<typename Condition>
	//size_type execute_all(const Condition& cdt)
	//{
	//	helper_type in_helper(cdt), out_helpr;
	//	return execute_all(in_helper, out_helpr);
	//}

	template<typename Condition>
	size_type execute_all(const Condition& cdt) const
	{
		helper_type helper(cdt);
		return execute_all(helper, helper_type());
	}

	//template<typename Condition>
	//size_type const_execute_all(const Condition& cdt) const
	//{
	//	return execute_all(cdt);
	//}

	//template<typename Condition, typename Result>
	//size_type execute_all(const Condition& cdt, Result& rst)
	//{
	//	helper_type in_helper(cdt), out_helper;
	//	return (execute_all(in_helper, out_helper)) && (rst << out_helper);
	//}

	template<typename Condition, typename Result>
	size_type execute_all(const Condition& cdt, Result& rst) const
	{
		helper_type in_helper(cdt), out_helper;
		return (execute_all(in_helper, out_helper)) && (rst << out_helper);
		//return 0;
	}

	//template<typename Condition, typename Result>
	//size_type const_execute_all(const Condition& cdt, Result& rst) const
	//{
	//	return execute_all(cdt, rst);
	//}

	void clear(void)
	{
		_reg_map.clear();
	}

protected:

	base_sync_container_wrap_ptr_type get_wrap(const key_type& key) const
	{
		base_sync_container_wrap_ptr_type ptr;
		_reg_map.use_handler(key, boost::bind(&this_type::handler_get_wrap, this, _1, boost::ref(ptr)));
		return ptr;
	}

	void handler_get_wrap(reg_map_citer_type& iter, base_sync_container_wrap_ptr_type& ptr) const
	{
		ptr = iter->second;
	}

	bool send(const key_type& key, helper_type& helper)
	{
		base_sync_container_wrap_ptr_type ptr(get_wrap(key));
		try
		{
			return ptr && ptr->use_handler(boost::bind((bool (base_sync_container::*)(helper_type&))
																&base_sync_container::send, _1, boost::ref(helper)));
		}
		catch(const base_sync_container_wrap_error_type&)
		{
		}

		return false;
	}

	bool send(const key_type& key, helper_type& helper) const
	{
		base_sync_container_wrap_ptr_type ptr(get_wrap(key));
		try
		{
			return ptr && ptr->use_handler(boost::bind((bool (base_sync_container::*)(helper_type&) const)
																&base_sync_container::send, _1, boost::ref(helper)));
		}
		catch(const base_sync_container_wrap_error_type&)
		{
		}

		return false;
	}

	size_type send_all(helper_type& helper)
	{
		size_type send_size = 0;
//		_reg_map.use_handler(boost::bind((void (this_type::*)(typename reg_map_type::base_type&, helper_type&, size_type&))
//											&this_type::handler_send_all, this, _1, boost::ref(helper), boost::ref(send_size)));

		_reg_map.use_handler(boost::bind(&this_type::handler_send_all_of_non_const, this, _1, boost::ref(helper), boost::ref(send_size)));

		return send_size;
	}

	void handler_send_all_of_non_const(typename reg_map_type::base_type& base, helper_type& helper, size_type& size)
	{
		size_type send_size = 0;
		bool bret = false;
		for(reg_map_iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			bret = false;
			if(i->second && i->second->use_handler(boost::bind((bool (base_sync_container::*)(helper_type& helper))
																		&base_sync_container::send, _1, boost::ref(helper))))
			{
				++send_size;
			}
		}

		size = send_size;
	}

	size_type send_all(helper_type& helper) const
	{
		size_type send_size = 0;
//		_reg_map.use_handler(boost::bind((void (this_type::*)( const typename reg_map_type::base_type& base,
//																helper_type&, size_type&) const)
//											&this_type::handler_send_all, this, _1, boost::ref(helper), boost::ref(send_size)));
		_reg_map.use_handler(boost::bind(&this_type::handler_send_all_of_const, this, _1, boost::ref(helper), boost::ref(send_size)));
		return send_size;
	}

	void handler_send_all_of_const(const typename reg_map_type::base_type& base, helper_type& helper, size_type& size) const
	{
		size_type send_size = 0;
		bool bret = false;
		for(reg_map_citer_type i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			try
			{
				if(i->second && i->second->use_handler(boost::bind((bool (base_sync_container::*)(helper_type&) const)
																		&base_sync_container::send, _1, boost::ref(helper))))
				{
					++send_size;
				}
			}
			catch(const base_sync_container_wrap_error_type&)
			{
			}
		}

		size = send_size;
	}

	bool recv(const key_type& key, helper_type& helper)
	{
		//reg_map_iter_type iter = _reg_map.find(key);
		//return (iter != base.end()) && iter->second && iter->second->recv(helper);
		base_sync_container_wrap_ptr_type ptr(get_wrap(key));
		//bool bret = false;
		try
		{
			return ptr && ptr->use_handler(boost::bind((bool (base_sync_container::*)(helper_type&))
																&base_sync_container::recv, _1, boost::ref(helper)));
		}
		catch(const base_sync_container_wrap_error_type&)
		{
		}

		return false;

	}

	bool recv(const key_type& key, helper_type& helper) const
	{
		/*reg_map_citer_type iter = _reg_map.find(key);
		return (iter != base.end()) && iter->second && boost::cref(*iter->second).get().recv(helper);*/

		base_sync_container_wrap_ptr_type ptr(get_wrap(key));
		try
		{
			return ptr && ptr->use_handler(boost::bind((bool (base_sync_container::*)(helper_type&) const)
																&base_sync_container::recv, _1, boost::ref(helper)));
		}
		catch(const base_sync_container_wrap_error_type&)
		{
		}

		return false;
	}

	template<typename Map>
	size_type recv_all_of_map(Map& helpers)
	{
		typedef Map map_type;
		size_type recv_size = 0;
//		_reg_map.use_handler(boost::bind((void (this_type::*)(typename reg_map_type::base_type&, map_type&, size_type&))
//											&this_type::handler_recv_all<map_type>, this, _1, boost::ref(helpers), boost::ref(recv_size)));
		_reg_map.use_handler(boost::bind(&this_type::handler_recv_all_of_non_const<map_type>,
																			this, _1, boost::ref(helpers), boost::ref(recv_size)));
		return recv_size;
	}

	template<typename Map>
	void handler_recv_all_of_non_const(typename reg_map_type::base_type& base, Map& helpers, size_type& size)
	{
		size_type recv_size = 0;
		bool bret = false;
		for(reg_map_iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			try
			{
				if(i->second && i->second->use_handler(boost::bind((bool (base_sync_container::*)(helper_type&))
																		&base_sync_container::recv, _1, boost::ref(helpers[i->first]))))
				{
					++recv_size;
				}
			}
			catch(const base_sync_container_wrap_error_type&)
			{
			}
		}

		size = recv_size;
	}

	template<typename Map>
	size_type recv_all_of_map(Map& helpers) const
	{
		typedef Map map_type;
		size_type recv_size = 0;
//		_reg_map.use_handler(boost::bind(((void (this_type::*)( const typename reg_map_type::base_type&,
//																helper_type&, size_type&) const)
//											&this_type::handler_recv_all<map_type>), this, _1, boost::ref(helpers), boost::ref(recv_size)));
		_reg_map.use_handler(boost::bind(&this_type::handler_recv_all_of_const<map_type>, this, _1, boost::ref(helpers), boost::ref(recv_size)));
		return recv_size;
	}

	template<typename Map>
	void handler_recv_all_of_const(const typename reg_map_type::base_type& base, Map& helpers, size_type& size) const
	{
		size_type recv_size = 0;
		for(reg_map_citer_type i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			try
			{
				if(i->second && i->second->use_handler(boost::bind((bool (base_sync_container::*)(helper_type&) const)
																			&base_sync_container::recv, _1, boost::ref(helpers[i->first]))))
				{
					++recv_size;
				}
			}
			catch(const base_sync_container_wrap_error_type&)
			{
			}
		}

		size = recv_size;
	}

	bool execute(const key_type& key, const helper_type& in_helper, helper_type& out_helper)
	{
		base_sync_container_wrap_ptr_type ptr(get_wrap(key));
		try
		{
			return ptr && ptr->use_handler(boost::bind((bool (base_sync_container::*)(const helper_type&, helper_type&))
																&base_sync_container::execute,
																_1, boost::cref(in_helper), boost::ref(out_helper)));
		}
		catch(const base_sync_container_wrap_error_type&)
		{
		}

		return false;
	}

	bool execute(const key_type& key, const helper_type& in_helper, helper_type& out_helper) const
	{
		base_sync_container_wrap_ptr_type ptr(get_wrap(key));
		try
		{
			return ptr && ptr->use_handler(boost::bind((bool (base_sync_container::*)(const helper_type&, helper_type&) const)
															&base_sync_container::execute,
															_1, boost::cref(in_helper), boost::ref(out_helper)));
		}
		catch(const base_sync_container_wrap_error_type&)
		{
		}

		return false;
	}

	size_type execute_all(const helper_type& in_helper, helper_type& out_helper)
	{
		size_type ex_size = 0;
		_reg_map.use_handler(boost::bind((void (this_type::*)(typename reg_map_type::base_type&,
																const helper_type&, helper_type&,
																size_type&))
											&this_type::handler_execute_all, this, _1,
											boost::cref(in_helper),
											boost::ref(out_helper),
											boost::ref(ex_size)));
		return ex_size;
	}

	void handler_execute_all(typename reg_map_type::base_type& base,
							const helper_type& in_helper,
							helper_type& out_helper,
							size_type& size)
	{
		size_type ex_size = 0;
		for(reg_map_iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			try
			{
				if(i->second && i->second->use_handler(boost::bind((bool (base_sync_container::*)(const helper_type&, helper_type&))
																		&base_sync_container::execute, _1,
																		boost::cref(in_helper),
																		boost::ref(out_helper))))
				{
					++ex_size;
				}
			}
			catch(const base_sync_container_wrap_error_type&)
			{
			}
		}

		size = ex_size;
	}

	size_type execute_all(const helper_type& in_helper, helper_type& out_helper) const
	{
		size_type ex_size = 0;
		_reg_map.use_handler(boost::bind(((void (this_type::*)(const typename reg_map_type::base_type&,
																const helper_type&, helper_type&, size_type&) const)
											&this_type::handler_execute_all), this, _1,
											boost::cref(in_helper),
											boost::ref(out_helper),
											boost::ref(ex_size)));
		return ex_size;
	}

	void handler_execute_all(const typename reg_map_type::base_type& base,
								const helper_type& in_helper,
								helper_type& out_helper,
								size_type& size) const
	{
		size_type ex_size = 0;
		for(reg_map_citer_type i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			try
			{
				if(i->second && i->second->use_handler(boost::bind((bool (base_sync_container::*)(const helper_type&, helper_type&) const)
																		&base_sync_container::execute, _1,
																		boost::cref(in_helper),
																		boost::ref(out_helper))))
				{
					++ex_size;
				}
			}
			catch(const base_sync_container_wrap_error_type&)
			{
			}
		}

		size = ex_size;
	}


private:
	reg_map_type _reg_map;
};

} // sync_system
} // yggr

#endif //__YGGR_SYNC_SYSTEM_SYNC_CONTAINER_MGR_HPP__
