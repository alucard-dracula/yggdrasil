//recver_handler_mgr.hpp

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

#ifndef __YGGR_DATA_CENTER_RECVER_HANDLER_MGR_SINGLE_FOO_HPP__
#define __YGGR_DATA_CENTER_RECVER_HANDLER_MGR_SINGLE_FOO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/nonable.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/task_center/type_traits/tags.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <boost/thread/mutex.hpp>

#include <cassert>

namespace yggr
{
namespace task_center
{
namespace detail
{

struct recver_handler_mgr_tag_init
{
};

template<typename Task>
class base_recver_handler
{
public:
	typedef Task task_type;

private:
	typedef base_recver_handler this_type;

public:
	virtual ~base_recver_handler(void)
	{
	}

public:
	virtual void run_handler(const task_type& tk) const = 0;
};

template<typename Task, typename Handler>
class recver_handler
	: public base_recver_handler<Task>
{
public:
	typedef Task task_type;
	typedef Handler handler_type;
	typedef base_recver_handler<task_type> base_type;

private:
	typedef recver_handler this_type;

public:
	explicit recver_handler(const handler_type& handler)
		: _handler(handler)
	{
	}

	recver_handler(const this_type& right)
		: _handler(right._handler)
	{
	}

	~recver_handler(void)
	{
	}

public:
	virtual void run_handler(const task_type& tk) const
	{
		(_handler)(true, tk);
	}

private:
	handler_type _handler;
};

} // namespace detail
} // namespace task_center
} // namespace yggr

namespace yggr
{
namespace task_center
{

template<typename Mark, typename Task, typename Mutex = void>
class recver_handler_mgr
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Mark mark_type;
	typedef Task task_type;

	typedef typename task_type::owner_id_type owner_id_type;

	typedef detail::recver_handler_mgr_tag_init init_type;
	typedef yggr::task_center::type_traits::nonmust_need_init init_tag_type;

private:
	typedef Mutex tpl_arg_mutex_type;

	typedef detail::base_recver_handler<task_type> base_recv_handler_type;
	typedef shared_ptr<base_recv_handler_type> base_recv_handler_container_type;

	typedef 
		safe_container::safe_unordered_map
		<
			owner_id_type,
			base_recv_handler_container_type,
			tpl_arg_mutex_type
		> handler_map_type;
	typedef typename handler_map_type::iterator handler_map_iter_type;
	typedef typename handler_map_type::const_iterator handler_map_citer_type;

private:
	typedef recver_handler_mgr this_type;

public:
	recver_handler_mgr(void)
	{
	}

	recver_handler_mgr(const init_type& init)
	{
	}

	~recver_handler_mgr(void)
	{
		assert(_handler_map.empty());
	}

public:

	inline static bool need_link(void)
	{
		return false;
	}

	template<typename Handler> inline
	void link(const Handler& handler)
	{
		assert(false);
	}

	inline void drop_link(void)
	{
		assert(false);
	}

	template<typename Handler> inline
	bool hold_recv(const owner_id_type& id, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef detail::recver_handler<task_type, handler_type> recver_handler_type;

		assert(id);
		base_recv_handler_container_type p;

		return 
			(!!id) 
			&& (p = construct_shared<base_recv_handler_type>(
						yggr_nothrow_new recver_handler_type(handler) ))
			&& (_handler_map.insert(id, p));
	}

	inline bool stop_recv(const owner_id_type& id)
	{
		assert(id);
		return (!!id) && _handler_map.erase(id);
	}

	inline bool push(const task_type& val) const
	{
		owner_id_type id = val.owner_id();

		base_recv_handler_container_type ptr;
		_handler_map.get_value(id, ptr);

		return ptr && (ptr->run_handler(val), true);
	}

	inline bool empty(void) const
	{
		return _handler_map.empty();
	}

	template<typename IDs> inline
	void drop_task(const IDs&) {}

	template<typename IDs, typename KeyGetter> inline
	void drop_task(const IDs&, const KeyGetter&){}

	inline void clear_tasks(void){}

	inline void clear(void)
	{
		_handler_map.clear();
	}

private:
	handler_map_type _handler_map;

};
} //namespace task_center
} //namespace yggr

#endif //__YGGR_DATA_CENTER_RECVER_HANDLER_MGR_SINGLE_FOO_HPP__
