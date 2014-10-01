//recver_handler_mgr.hpp

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

#ifndef __YGGR_DATA_CENTER_RECVER_HANDLER_MGR_SINGLE_FOO_HPP__
#define __YGGR_DATA_CENTER_RECVER_HANDLER_MGR_SINGLE_FOO_HPP__

#include <cassert>

#include <boost/thread/mutex.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/interface_ptr.hpp>
#include <yggr/nonable/nonable.hpp>

#include <yggr/task_center/type_traits/tags.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>

namespace yggr
{
namespace task_center
{

template<typename Mark, typename Task, typename Mutex = boost::mutex>
class recver_handler_mgr
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Mark mark_type;
	typedef Task task_type;

	typedef typename task_type::owner_id_type owner_id_type;

	typedef struct {} init_type;
	typedef yggr::task_center::type_traits::nonmust_need_init init_tag_type;

private:
	typedef Mutex mutex_type;

	template<typename TTask>
	class base_recver_handler
	{
	public:
		typedef TTask task_type;

	public:
		virtual ~base_recver_handler(void)
		{
		}

		virtual void run_handler(const task_type& tk) = 0;
	};

	typedef base_recver_handler<task_type> base_recv_handler_type;
	typedef interface_ptr<base_recv_handler_type> base_recv_handler_container_type;

	typedef safe_container::safe_unordered_map
			<
				owner_id_type,
				base_recv_handler_container_type,
				mutex_type
			> handler_map_type;
	typedef typename handler_map_type::iterator handler_map_iter_type;
	typedef typename handler_map_type::const_iterator handler_map_citer_type;


	template<typename Handler>
	class recver_handler : public base_recv_handler_type
	{
	public:
		typedef Handler handler_type;
		typedef base_recv_handler_type base_type;
		typedef typename base_type::task_type task_type;


		explicit recver_handler(const handler_type& handler)
			: _handler(handler)
		{
		}

		recver_handler(const recver_handler& h_recver)
			: _handler(h_recver._handler)
		{
		}

		virtual ~recver_handler(void)
		{
		}

		virtual void run_handler(const task_type& tk)
		{
			_handler(true, tk);
		}

	private:
		handler_type _handler;
	};

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

	static bool s_need_link(void)
	{
		return false;
	}

	bool need_link(void) const
	{
		return false;
	}

	template<typename Handler>
	inline void link(const Handler& handler)
	{
		assert(false);
	}

	inline void drop_link(void)
	{
		assert(false);
	}

	template<typename Handler>
	bool hold_recv(const owner_id_type& id, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef recver_handler<handler_type> recver_handler_type;

		assert(id);
		if(!id)
		{
			return false;
		}

		base_recv_handler_container_type p(new recver_handler_type(handler));
		assert(p);
		if(!p)
		{
			return false;
		}
		return _handler_map.insert(id, p);
	}

	bool stop_recv(const owner_id_type& id)
	{
		assert(id);
		if(!id)
		{
			return false;
		}
		return _handler_map.erase(id);
	}

	bool push(const task_type& val) const
	{
		owner_id_type id = val.owner_id();

		base_recv_handler_container_type ptr
			= _handler_map.use_handler(
								boost::bind(
												&this_type::handler_get_recv_handler,
												this, _1, boost::cref(id)
											)
										);

		if(!ptr)
		{
			return false;
		}

		ptr->run_handler(val);
		return true;
	}

	bool empty(void) const
	{
		return _handler_map.empty();
	}

	template<typename IDs>
	inline void drop_task(const IDs& ids)
	{
	}

	template<typename IDs, typename KeyGetter>
	inline void drop_task(const IDs& ids, const KeyGetter& key_getter)
	{
	}

	void clear_tasks(void)
	{
	}

	void clear(void)
	{
		_handler_map.clear();
	}

private:

	base_recv_handler_container_type
		handler_get_recv_handler(const typename handler_map_type::base_type& base,
									const owner_id_type& id) const
	{
		handler_map_citer_type iter = base.find(id);
		if(iter == base.end())
		{
			return base_recv_handler_container_type();
		}

		return iter->second;
	}
private:
	handler_map_type _handler_map;

};
} //namespace task_center
} //namespace yggr

#endif //__YGGR_DATA_CENTER_RECVER_HANDLER_MGR_SINGLE_FOO_HPP__
