//safe_backup_device.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_BACKUP_DEVICE_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_BACKUP_DEVICE_HPP__

#include <yggr/bind/bind.hpp>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <utility>

namespace yggr
{
namespace safe_container
{

template<
			typename Key, 
			typename Val, 
			typename KeyGenner,
			typename Mutex = void,
			typename HashFoo = typename container::unordered_map<Key, Val>::hasher,
			typename Cmp = typename container::unordered_map<Key, Val>::key_equal,
			typename Alloc = typename container::unordered_map<Key, Val>::allocator_type
		>
class safe_backup_device
{
public:

	typedef Key key_type;
	typedef Val mapped_type;

	typedef mapped_type value_type;

private:
	typedef KeyGenner key_gen_type;
	typedef Mutex tpl_arg_mutex_type;
	typedef HashFoo hasher;
	typedef Cmp key_equal;
	typedef Alloc allocator_type;

private:
	typedef 
		safe_container::safe_unordered_map
		<
			key_type, 
			mapped_type,
			tpl_arg_mutex_type,
			hasher, 
			key_equal, 
			allocator_type
		> map_type;
	typedef typename map_type::iterator map_iter_type;
	typedef typename map_type::const_iterator map_citer_type;

public:
	typedef typename map_type::base_type container_type;

private:
	typedef safe_backup_device this_type;

private:
	inline static map_type& _map(void)
	{
		static map_type map;
		return map;
	}

public:

#define _map this_type::_map()
	safe_backup_device(void)
	{
	}

	~safe_backup_device(void)
	{
	}

public:
	inline static bool backup_empty(void)
	{
		return _map.empty();
	}

	inline static void clear_backup(void)
	{
		container_type container;
		_map.swap(container);
	}

	template<typename Handler> inline
	static typename func::foo_t_info<Handler>::result_type 
		clear_backup(const Handler& handler)
	{
		container_type container;
		_map.swap(container);

		return handler(container);
	}

	inline static std::pair<key_type, bool> 
		backup(const value_type& val)
	{
		key_type key = key_gen_type::gen(val);
		std::pair<key_type, bool> ret(key, _map.insert(key, val));
		return ret;
		//return std::make_pair<key_type, bool>(key, _map.insert(key, val)); // msvc cpp14 not support;
	}

	inline static value_type drop_backup(const key_type& key)
	{
		return
			_map.find(
				key,
				boost::bind(&this_type::handler_drop_backup, _1, _2));
	}

	inline static std::size_t remove_backup(const key_type& key)
	{
		return _map.erase(key);
	}

	inline static value_type get(const key_type& key)
	{
		value_type val;
		_map.get_value(key, val);
		return val;
	}

private:
	inline static value_type handler_drop_backup(typename map_type::base_type& base,
													typename map_type::iterator rst)
	{
		if(rst == base.end())
		{
			return value_type();
		}
		else
		{
			value_type val = rst->second;
			base.erase(rst);
			return val;
		}
	}

#undef  _map

};

} // namespace safe_container
} // namespace yggr

#endif //__YGGR_SAFE_CONTAINER_SAFE_BACKUP_DEVICE_HPP__