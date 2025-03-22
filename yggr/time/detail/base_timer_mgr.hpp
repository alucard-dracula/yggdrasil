//base_timer_mgr.hpp

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

#ifndef __YGGR_TIME_DETAIL_BASE_TIMER_MGR_HPP__
#define __YGGR_TIME_DETAIL_BASE_TIMER_MGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/thread/this_thread.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/safe_container/safe_buffered_object.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/time/detail/timer_task.hpp>
#include <yggr/time/detail/timer_object_container.hpp>
#include <yggr/time/detail/timer_task_state.hpp>

#include <boost/atomic.hpp>

namespace yggr
{
namespace time
{
namespace detail
{

template<typename Task_ID, typename Thread_Config>
class base_timer_mgr
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
protected:
	typedef Thread_Config thread_config_type;

public:
	typedef Task_ID task_id_type;
	typedef timer_task_state task_state_type;
	typedef typename thread_config_type::time_type time_type;

protected:
	typedef boost::atomic<bool> run_state_type;

	typedef typename thread_config_type::thread_type thread_type;
	typedef typename thread_config_type::thread_group_type thread_group_type;

	typedef thread::this_thread<thread_type> this_thread_type;

protected:
	typedef safe_container::safe_wrap<time_type> time_wrap_type;


protected:
	typedef timer_object_container<task_id_type> task_id_container_type;
	typedef typename task_id_container_type::list_type task_id_list_type;
	typedef safe_container::safe_buffered_object<task_id_container_type> timer_task_id_queue_type;

	typedef base_timer_task<task_id_type> base_timer_task_type;
	typedef shared_ptr<base_timer_task_type> i_base_timer_task_type;

	typedef timer_object_container<i_base_timer_task_type> timer_task_container_type;
	typedef typename timer_task_container_type::list_type timer_task_list_type;
	typedef safe_container::safe_buffered_object<timer_task_container_type> timer_task_queue_type;

	typedef safe_container::safe_unordered_map<task_id_type, i_base_timer_task_type> timer_task_map_type;

private:
	typedef base_timer_mgr this_type;

public:
	base_timer_mgr(void)
		: _brun(false),
			_tm_step(time_type(thread_config_type::E_def_sleep_time_sec,
								thread_config_type::E_def_sleep_time_nsec))
	{
	}

	base_timer_mgr(const time_type& tm_step)
		: _brun(false), _tm_step(tm_step)
	{
	}

	~base_timer_mgr(void)
	{
		assert(!_brun.load());
	}

public:
	inline bool is_running(void) const
	{
		return _brun.load();
	}

	inline void join(void)
	{
		_trd_group.join_all();
	}

	inline void stop(void)
	{
		bool brun_old = true;
		bool brun_new = false;

		if(!_brun.compare_exchange_strong(brun_old, brun_new))
		{
			return;
		}

		_trd_group.join_all();
	}

protected:
	inline bool pro_add_task(const i_base_timer_task_type& ptask)
	{
		return _task_map.insert(ptask->id(), ptask);
	}

	inline bool pro_remove_task(const task_id_type& id)
	{
		return _task_map.erase(id);
	}

protected:

	inline static task_id_type gen_id(void)
	{
		static ids::id_generator<task_id_type> gen;
		return gen();
	}

	inline static bool handler_equal_id_of_task(const i_base_timer_task_type& ptm_task, const task_id_type& id)
	{
		return ptm_task && ptm_task->id() == id;
	}

	inline static bool handler_equal_id_of_task_id(const task_id_type& task_id, const task_id_type& id)
	{
		return task_id == id;
	}

protected:
	run_state_type _brun;
	timer_task_map_type _task_map;
	time_wrap_type _tm_step;
	thread_group_type _trd_group;

};

} // namespace detail
} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_DETAIL_BASE_TIMER_MGR_HPP__
