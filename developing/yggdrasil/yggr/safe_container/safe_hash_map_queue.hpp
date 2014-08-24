//safe_hash_map_queue.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_HASH_MAP_QUEUE_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_HASH_MAP_QUEUE_HPP__

#include <yggr/base/yggrdef.h>

#include <queue>
#include <algorithm>
#include <cassert>

#include <boost/container/deque.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/swap.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/helper/mutex_def_helper.hpp>


namespace yggr
{
namespace safe_container
{

template<	typename Key, typename Val, 
			typename Mutex = boost::mutex,
			typename Container = boost::container::deque<Val>,
			typename HashFoo = boost::hash<Key>,
			typename Cmp = std::equal_to<Key>,
			typename Alloc = std::allocator< std::pair<const Key, Val> >, 
			template<typename _Val, typename _Container> class Queue_Type = std::queue, 
			template<typename _Key, typename _Node_Val, typename _HahsFoo, typename _Cmp, typename _Alloc> class Hash_Map = boost::unordered_map,
			bool is_only = false
		>
class safe_hash_map_queue;

template<typename Key, typename Val, 
			typename Mutex,
			typename Container,
			typename HashFoo,
			typename Cmp,
			typename Alloc, 
			template<typename _Val, typename _Container> class Queue_Type, 
			template<typename _Key, typename _Node_Val, typename _HahsFoo, typename _Cmp, typename _Alloc> class Hash_Map
		>
class safe_hash_map_queue<Key, Val, Mutex, Container, HashFoo, Cmp, Alloc, Queue_Type, Hash_Map, false>
	: protected Hash_Map<Key, Queue_Type<Val, Container>, HashFoo, Cmp, Alloc>, 
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Key key_type;
	typedef Val val_type;
	typedef Queue_Type<val_type, Container> node_type;

	typedef Hash_Map<key_type, node_type, HashFoo, Cmp, Alloc> base_type;
	typedef typename base_type::const_iterator const_iterator;
	typedef typename base_type::iterator iterator;
	typedef typename base_type::size_type size_type;
	typedef std::pair<key_type, val_type> value_type;

	typedef std::pair<iterator, bool> insert_result_type;

private:
	typedef Mutex mutex_type;
	typedef typename helper::mutex_def_helper<mutex_type>::lock_type lock_type;

	typedef safe_hash_map_queue this_type;

public:
	safe_hash_map_queue(void)
		: _now_iter(base_type::begin()), _size(0)
	{
	}

	~safe_hash_map_queue(void)
	{
		for(iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			for(;!i->second.empty();)
			{
				i->second.pop();
			}
		}
	}

	void push(const key_type& key, const val_type& val)
	{
		lock_type lk(_mutex);
		base_type& base = *this;

		key_type now_key;
		if(base_type::empty())
		{
			now_key = key;
		}
		else
		{
			now_key = _now_iter->first;
		}

		bool bchange = !(exist_key(key));
		
		base[key].push(val);
		++_size;

		if(bchange)
		{
			resetting_now_iter(now_key);
		}
	}

	void push(const value_type& val)
	{
		this_type::push(val.first, val.second);
	}

	bool pop(val_type& val)
	{
		lock_type lk(_mutex);
		if(!_size)
		{
			return false;
		}

		for(;;)
		{
			if(_now_iter->second.empty())
			{
				next();
			}
			else
			{
				break;
			}
		}
		val = _now_iter->second.front();
		_now_iter->second.pop();
		--_size;
		next();

		return true;
	}

	bool front(val_type& val) const
	{
		lock_type lk(_mutex);
		if(!_size)
		{
			return false;
		}

		for(;;)
		{
			if(_now_iter->second.empty())
			{
				next();
			}
			else
			{
				break;
			}
		}
		val = _now_iter->second.front();
		return true;
	}

	void remove(const key_type& key)
	{
		lock_type lk(_mutex);
		
		if(base_type::empty())
		{
			return;
		}

		iterator rm_iter;
		if(_now_iter->first == key)
		{
			rm_iter = _now_iter;
			next();
		}
		else
		{
			rm_iter = base_type::find(key);
		}

		if(rm_iter == base_type::end())
		{
			return;
		}

		key_type now_key = _now_iter->first;
		remove(rm_iter);
	
		if(now_key != key)
		{
			resetting_now_iter(now_key);
		}
	}

	void clear(void)
	{
		base_type tmap;

		{
			lock_type lk(_mutex);
			base_type::swap(tmap);
		}

		iterator isize = base_type::end();
		for(iterator i = base_type::begin(); i != isize; ++i)
		{
			for(;!i->second.empty();)
			{
				i->second.pop();
			}
		}

		tmap.clear();
		_size = 0;
		_now_iter = base_type::begin();
	}
	
	bool empty(void) const
	{
		lock_type lk(_mutex);
		return !_size;
	}

	size_type size(void) const
	{
		lock_type lk(_mutex);
		return _size;
	}

	bool is_only_data(void) const
	{
		return false;
	}

	static bool s_is_only_data(void)
	{
		return false;
	}

private:
	void resetting_now_iter(const key_type& key)
	{
		_now_iter = base_type::find(key);
		assert(_now_iter != base_type::end());
	}

	bool exist_key(const key_type& key) const
	{
		return base_type::find(key) != base_type::end();
	}

	void remove(iterator& iter)
	{
		if(iter == base_type::end())
		{
			return;
		}

		_size -= iter->second.size();
		for(;!iter->second.empty();)
		{
			iter->second.pop();
		}

		base_type::erase(iter);

		_now_iter = base_type::begin();
	}

	void next(void) 
	{
		if(++_now_iter == base_type::end())
		{
			_now_iter = base_type::begin();
		}
	}

private:
	mutable mutex_type _mutex;
	iterator _now_iter;
	size_type _size;
};

//---------------------------------------only_data------------------------------------------------------

template<typename Key, typename Val, 
			typename Mutex,
			typename Container,
			typename HashFoo,
			typename Cmp,
			typename Alloc, 
			template<typename _Val, typename _Container> class Queue_Type, 
			template<typename _Key, typename _Node_Val, typename _HahsFoo, typename _Cmp, typename _Alloc> class Hash_Map
		>
class safe_hash_map_queue<Key, Val, Mutex, Container, HashFoo, Cmp, Alloc, Queue_Type, Hash_Map, true>
	: protected Hash_Map<Key, Queue_Type<Val, Container>, HashFoo, Cmp, Alloc>, private nonable::noncopyable
{
public:
	typedef Key key_type;
	typedef Val val_type;
	typedef Queue_Type<val_type, Container> node_type;

	typedef Hash_Map<key_type, node_type, HashFoo, Cmp, Alloc> base_type;
	typedef typename base_type::const_iterator const_iterator;
	typedef typename base_type::iterator iterator;
	typedef typename base_type::size_type size_type;
	typedef std::pair<key_type, val_type> value_type;
	typedef std::pair<iterator, bool> insert_result_type;

private:
	typedef Mutex mutex_type;
	typedef typename helper::mutex_def_helper<mutex_type>::lock_type lock_type;

	typedef safe_hash_map_queue this_type;

public:
	safe_hash_map_queue(void)
		: _now_iter(base_type::begin()), _size(0)
	{
	}

	~safe_hash_map_queue(void)
	{
		for(iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
		{
			for(;!i->second.empty();)
			{
				i->second.pop();
			}
		}
	}

	void push(const key_type& key, const val_type& val)
	{
		node_type tnode;
		base_type& base = *this;
		key_type now_key;

		{
			lock_type lk(_mutex);
			
			if(base_type::empty())
			{
				now_key = key;
			}
			else
			{
				now_key = _now_iter->first;
			}

			bool bchange = !(exist_key(key));
			
			if(!base[key].empty())
			{
				boost::swap(tnode, base[key]);
			}

			base[key].push(val);
			_size = 1;

			if(bchange)
			{
				resetting_now_iter(now_key);
			}
		}

		for(;!tnode.empty();)
		{
			tnode.pop();
		}
	}

	void push(const value_type& val)
	{
		this_type::push(val.first, val.second);
	}
	
	bool pop(val_type& val)
	{
		lock_type lk(_mutex);
		if(!_size)
		{
			return false;
		}

		for(;;)
		{
			if(_now_iter->second.empty())
			{
				next();
			}
			else
			{
				break;
			}
		}
		val = _now_iter->second.front();
		_now_iter->second.pop();
		--_size;
		next();

		return true;
	}

	bool front(val_type& val) const
	{
		lock_type lk(_mutex);
		if(!_size)
		{
			return false;
		}

		for(;;)
		{
			if(_now_iter->second.empty())
			{
				next();
			}
			else
			{
				break;
			}
		}
		val = _now_iter->second.front();
		return true;
	}

	void remove(const key_type& key)
	{
		lock_type lk(_mutex);
		
		if(base_type::empty())
		{
			return;
		}

		iterator rm_iter;
		if(_now_iter->first == key)
		{
			rm_iter = _now_iter;
			next();
		}
		else
		{
			rm_iter = base_type::find(key);
		}

		if(rm_iter == base_type::end())
		{
			return;
		}

		key_type now_key = _now_iter->first;
		remove(rm_iter);
	
		if(now_key != key)
		{
			resetting_now_iter(now_key);
		}
	}

	void clear(void)
	{
		base_type tmap;

		{
			lock_type lk(_mutex);
			base_type::swap(tmap);
		}

		iterator isize = base_type::end();
		for(iterator i = base_type::begin(); i != isize; ++i)
		{
			for(;!i->second.empty();)
			{
				i->second.pop();
			}
		}

		tmap.clear();
		_size = 0;
		_now_iter = base_type::begin();
	}
	
	bool empty(void) const
	{
		lock_type lk(_mutex);
		return !_size;
	}

	size_type size(void) const
	{
		lock_type lk(_mutex);
		return _size;
	}

	bool is_only_data(void) const
	{
		return true;
	}

	static bool s_is_only_data(void)
	{
		return true;
	}

private:
	void resetting_now_iter(const key_type& key)
	{
		_now_iter = base_type::find(key);
		assert(_now_iter != base_type::end());
	}

	bool exist_key(const key_type& key) const
	{
		return base_type::find(key) != base_type::end();
	}

	void remove(iterator& iter)
	{
		if(iter == base_type::end())
		{
			return;
		}

		_size -= iter->second.size();
		for(;!iter->second.empty();)
		{
			iter->second.pop();
		}

		base_type::erase(iter);

		_now_iter = base_type::begin();
	}

	void next(void)
	{
		if(++_now_iter == base_type::end())
		{
			_now_iter = base_type::begin();
		}
	}

private:
	mutable mutex_type _mutex;
	iterator _now_iter;
	size_type _size;
};

} // namesapce safe_container
} //namesapce yggr

#endif //__YGGR_SAFE_CONTAINER_SAFE_HASH_MAP_QUEUE_HPP__