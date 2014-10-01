//task_saver_saver.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_SAVER_HPP__
#define __YGGR_TASK_CENTER_TASK_SAVER_HPP__

#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/task_center/task_store_op.hpp>
#include <yggr/task_center/type_traits/tags.hpp>
#include <yggr/nonable/nonable.hpp>

namespace yggr
{
namespace task_center
{

template<typename Mark,
			typename Task,
			typename Container,
			typename Store_OP>
class task_saver
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Mark mark_type;
	typedef Task task_type;
	typedef Container container_type;
	typedef Store_OP store_op_type;

	typedef struct {} init_type;
	typedef yggr::task_center::type_traits::nonmust_need_init init_tag_type;

private:
	typedef task_saver this_type;

public:

	task_saver(void)
	{
	}

	task_saver(const init_type& init)
	{
	}

	~task_saver(void)
	{
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
	bool pop(const Handler& handler)
	{
		task_type task;
		if(_buf.pop(task))
		{
			handler(task);
			return true;
		}
		return false;
	}

	void push(const task_type& task)
	{
		if(task.empty())
		{
			return;
		}

		store_op_type op;
		_buf.push(op.get_store_data(task));
	}

	bool empty(void) const
	{
		return _buf.empty();
	}

	size_type size(void) const
	{
		return _buf.size();
	}

	template<typename IDs>
	void drop_task(const IDs& ids)
	{
		store_op_type op;
		op.remove(_buf, ids);
	}

	template<typename IDs, typename KeyGetter>
	void drop_task(const IDs& ids, const KeyGetter& key_getter)
	{
		store_op_type op;
		op.remove(_buf, ids, key_getter);
	}

	void clear_tasks(void)
	{
		_buf.clear();
	}

	void clear(void)
	{
		_buf.clear();
	}

private:
	container_type _buf;
};

} //namespace task_center
} //namespace yggr

#endif //__YGGR_TASK_CENTER_TASK_SAVER_HPP__
