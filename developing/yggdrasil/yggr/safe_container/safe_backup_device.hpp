//safe_backup_device.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_BACKUP_DEVICE_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_BACKUP_DEVICE_HPP__

#include <boost/bind.hpp>

//#include <yggr/ids/base_ids_def.hpp>
//#include <yggr/ids/id_generator.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

namespace yggr
{
namespace safe_container
{

template<
			typename Key, 
			typename Val, 
			typename KeyGenner,
			typename Mutex =boost::mutex
		>
class safe_backup_device
{
public:

	typedef Key key_type;
	typedef Val val_type;
	typedef val_type value_type;

private:
	typedef KeyGenner key_gen_type;
	typedef Mutex mutex_type;

	typedef safe_container::safe_unordered_map<key_type, val_type, mutex_type> map_type;
	typedef typename map_type::iterator map_iter_type;
	typedef typename map_type::const_iterator map_citer_type;

public:
	typedef typename map_type::base_type container_type;

private:
	typedef safe_backup_device this_type;

public:

	safe_backup_device(void)
	{
	}

	~safe_backup_device(void)
	{
	}

	static bool backup_empty(void)
	{
		return _map.empty();
	}

	static void clear_backup(void)
	{
		container_type container;
		this_type::_map.use_handler(
			boost::bind(&this_type::handler_clear_backup, _1, boost::ref(container)));
	}

	template<typename Handler>
	static typename Handler::result_type clear_backup(const Handler& handler)
	{
		container_type container;
		this_type::_map.use_handler(
			boost::bind(&this_type::handler_clear_backup, _1, boost::ref(container)));

		return handler(container);
	}

	static key_type backup(const value_type& val)
	{
		key_type key = key_gen_type::gen(val);
		if(this_type::_map.insert(key, val))
		{
			return key;
		}

		return key_type();
	}

	static value_type drop_backup(const key_type& key)
	{
		value_type val 
			= this_type::_map.use_handler(boost::bind(&this_type::handler_drop_backup, _1, boost::cref(key)));

		return val;
	}

	static value_type get(const key_type& key)
	{
		value_type val;
		_map.get_value(key, val);
		return val;
	}

private:
	static value_type handler_drop_backup(typename map_type::base_type& base,
												const key_type& key)
	{
		map_iter_type iter = base.find(key);
		if(iter == base.end())
		{
			return value_type();
		}

		value_type val = iter->second;
		base.erase(iter);
		return val;
	}

	static void handler_clear_backup(typename map_type::base_type& base,
										typename map_type::base_type& back)
	{
		back.swap(base);
	}

private:
	static map_type _map;	
};

template<typename Key, typename Val, typename KeyGenner, typename Mutex>
typename safe_backup_device<Key, Val, KeyGenner, Mutex>::map_type 
			safe_backup_device<Key, Val, KeyGenner, Mutex>::_map;

} // namespace safe_container
} // namespace yggr

#endif //__YGGR_SAFE_CONTAINER_SAFE_BACKUP_DEVICE_HPP__