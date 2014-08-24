//safe_buffered_object.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_BUFFERED_OBJECT_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_BUFFERED_OBJECT_HPP__

#ifdef _MSC_VER
#	include <yggr/support/max_min_undef.ipp>
#endif // _MSC_VER

#include <limits>

#include <boost/thread/mutex.hpp>
#include <boost/ref.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/container/queue.hpp>
#include <yggr/helper/mutex_def_helper.hpp>

namespace yggr
{
namespace safe_container
{

template<typename Object, typename Mutex = boost::mutex, typename Obj_Init = void>
class safe_buffered_object
	: private nonable::noncopyable, private nonable::nonmoveable
{
public:
	typedef Object obj_type;
	typedef Obj_Init obj_init_type;
	typedef boost::shared_ptr<obj_type> obj_ptr_type;
private:
	typedef yggr::container::queue<obj_ptr_type> obj_ptr_queue_type;

	typedef Mutex mutex_type;
	typedef typename helper::mutex_def_helper<mutex_type>::lock_type lock_type;

	typedef safe_buffered_object this_type;

public:

	safe_buffered_object(BOOST_RV_REF(obj_init_type) init)
		: _now_size(0), _init(init), _limit_size(std::numeric_limits<yggr::size_type>::max())

	{
		create_node();
	}

	safe_buffered_object(const obj_init_type& init)
		: _now_size(0), _init(init), _limit_size(std::numeric_limits<yggr::size_type>::max())

	{
		create_node();
	}

	safe_buffered_object(BOOST_RV_REF(obj_init_type) init, yggr::size_type limit_size)
		: _now_size(0), _init(init), _limit_size(limit_size)
	{
		assert(_limit_size);
		create_node();
	}

	safe_buffered_object(const obj_init_type& init, yggr::size_type limit_size)
		: _now_size(0), _init(init), _limit_size(limit_size)
	{
		assert(_limit_size);
		create_node();
	}

	~safe_buffered_object(void)
	{
	}

	void clear(void)
	{
		lock_type lk(_mutex);
		obj_ptr_queue_type tmp(boost::move(_queue));
	}

	template<typename Value>
	bool insert(BOOST_RV_REF(Value) val)
	{
		lock_type lk(_mutex);
		if(_queue.empty())
		{
			if(!create_node())
			{
				return false;
			}
		}

		assert(_queue.front());
		(_queue.front())->insert(val);
		return true;
	}

	template<typename Value>
	bool insert(const Value& val)
	{
		lock_type lk(_mutex);
		if(_queue.empty())
		{
			if(!create_node())
			{
				return false;
			}
		}

		assert(_queue.front());
		(_queue.front())->insert(val);
		return true;
	}

	obj_ptr_type drop(void)
	{
		obj_ptr_type ptr;
		lock_type lk(_mutex);

		if(_queue.empty() || (!_queue.front()))
		{
			return obj_ptr_type();
		}

		if(_queue.size() == 1)
		{
			if(!create_node())
			{
				return obj_ptr_type();
			}
		}
		ptr.swap(_queue.front());
		_queue.pop();
		--_now_size;

		assert(ptr);
		return ptr;
	}

	void back(const obj_ptr_type& ptr)
	{
		lock_type lk(_mutex);
		if(!ptr)
		{
			return;
		}

		_queue.push(ptr);
		++_now_size;
	}

	template<typename Handler>
	bool get(const Handler& handler) const
	{
		obj_ptr_type ptr;
		{
			lock_type lk(_mutex);

			if(_queue.empty() || (!_queue.front()) || _queue.front()->empty())
			{
				return false;
			}

			if(_queue.size() == 1)
			{
				if(!create_node())
				{
					return false;
				}
			}
			ptr.swap(_queue.front());
			_queue.pop();
		}

		assert(ptr);

		handler(boost::ref(*ptr));

		if(!ptr->empty())
		{
			lock_type lk(_mutex);
			ptr->splice_to(*_queue.front());
		}

		ptr->clear();

		{
			lock_type lk(_mutex);
			_queue.push(ptr);
		}

		return true;
	}

	template<typename Handler, typename Return_Handler>
	bool get(const Handler& handler, const Return_Handler& ret) const
	{
		obj_ptr_type ptr;
		{
			lock_type lk(_mutex);

			if(_queue.empty() || (!_queue.front()) || _queue.front()->empty())
			{
				return false;
			}

			if(_queue.size() == 1)
			{
				if(!create_node())
				{
					return false;
				}
			}
			ptr.swap(_queue.front());
			_queue.pop();
		}

		assert(ptr);

		handler(boost::ref(*ptr), ret);

		if(!ptr->empty())
		{
			lock_type lk(_mutex);
			ptr->splice_to(*_queue.front());
		}

		ptr->clear();

		{
			lock_type lk(_mutex);
			_queue.push(ptr);
		}

		return true;
	}

private:
	bool create_node(void) const
	{
		if(!(_now_size < _limit_size))
		{
			return false;
		}

		obj_ptr_type ptr(new obj_type(_init));

		if(!ptr)
		{
			return false;
		}

		_queue.push(ptr);
		++_now_size;
		return true;
	}

private:
	mutable mutex_type _mutex;
	mutable obj_ptr_queue_type _queue;
	mutable yggr::size_type _now_size;
	yggr::size_type _limit_size;
	obj_init_type _init;
};


template<typename Object, typename Mutex>
class safe_buffered_object<Object, Mutex, void> : private nonable::noncopyable
{
public:
	typedef Object obj_type;
	typedef void obj_init_type;
	typedef boost::shared_ptr<obj_type> obj_ptr_type;

private:
	typedef yggr::container::queue<obj_ptr_type> obj_ptr_queue_type;

	typedef safe_buffered_object this_type;

	typedef Mutex mutex_type;
	typedef typename helper::mutex_def_helper<mutex_type>::lock_type lock_type;

public:
	safe_buffered_object(void)
		: _now_size(0), _limit_size(std::numeric_limits<yggr::u16>::max())

	{
		create_node();
	}

	safe_buffered_object(yggr::size_type limit_size)
		: _now_size(0), _limit_size(limit_size)
	{
		assert(_limit_size);
		create_node();
	}

	~safe_buffered_object(void)
	{
	}

	void clear(void)
	{
		lock_type lk(_mutex);
		//obj_ptr_queue_type tmp(boost::move(_queue));
		obj_ptr_queue_type tmp;
		_queue.swap(tmp);
	}

	obj_ptr_type drop(void)
	{
		obj_ptr_type ptr;
		lock_type lk(_mutex);

		if((_queue.empty()) || (!_queue.front()))
		{
			return obj_ptr_type();
		}

		if(_queue.size() == 1)
		{
			if(!create_node())
			{
				return obj_ptr_type();
			}
		}
		ptr.swap(_queue.front());
		_queue.pop();
		--_now_size;

		assert(ptr);
		return ptr;
	}

	void back(const obj_ptr_type& ptr)
	{
		lock_type lk(_mutex);
		if(!ptr)
		{
			return;
		}
		_queue.push(ptr);
		++_now_size;
	}

	template<typename Value>
	bool insert(const Value& val)
	{
		lock_type lk(_mutex);
		if(_queue.empty())
		{
			return false;
		}

		assert(_queue.front());
		(_queue.front())->insert(val);
		return true;
	}

	template<typename Handler>
	bool get(const Handler& handler) const
	{
		obj_ptr_type ptr;
		{
			lock_type lk(_mutex);

			if(_queue.empty() || (!_queue.front()) || _queue.front()->empty())
			{
				return false;
			}

			if(_queue.size() == 1)
			{
				if(!create_node())
				{
					return false;
				}
			}
			ptr.swap(_queue.front());
			_queue.pop();
		}

		assert(ptr);

		handler(boost::ref(*ptr));

		if(!ptr->empty())
		{
			lock_type lk(_mutex);
			ptr->splice_to(*_queue.front());
		}

		ptr->clear();

		{
			lock_type lk(_mutex);
			_queue.push(ptr);
		}

		return true;
	}

	template<typename Handler, typename Return_Handler>
	bool get(const Handler& handler, const Return_Handler& ret) const
	{
		obj_ptr_type ptr;
		{
			lock_type lk(_mutex);

			if(_queue.empty() || (!_queue.front()) || _queue.front()->empty())
			{
				return false;
			}

			if(_queue.size() == 1)
			{
				if(!create_node())
				{
					return false;
				}
			}
			ptr.swap(_queue.front());
			_queue.pop();
		}

		assert(ptr);

		handler(boost::ref(*ptr), ret);

		if(!ptr->empty())
		{
			lock_type lk(_mutex);
			ptr->splice_to(*_queue.front());
		}

		ptr->clear();

		{
			lock_type lk(_mutex);
			_queue.push(ptr);
		}

		return true;
	}

private:

	bool create_node(void) const
	{
		if(!(_now_size < _limit_size))
		{
			return false;
		}

		obj_ptr_type ptr(new obj_type());

		if(!ptr)
		{
			return false;
		}

		_queue.push(ptr);
		++_now_size;
		return true;
	}

private:
	mutable mutex_type _mutex;
	mutable obj_ptr_queue_type _queue;
	mutable yggr::size_type _now_size;
	yggr::size_type _limit_size;
};


} // namespace safe_container
} // namespace yggr

#endif //__YGGR_SAFE_CONTAINER_SAFE_BUFFERED_OBJECT_HPP__
