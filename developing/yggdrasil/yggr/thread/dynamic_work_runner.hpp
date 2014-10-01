//dynamic_work_runner.hpp

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

#ifndef __YGGR_THREAD_DYNAMIC_WORK_RUNNER_HPP__
#define __YGGR_THREAD_DYNAMIC_WORK_RUNNER_HPP__

#include <boost/any.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/atomic.hpp>
#include <yggr/move/move.hpp>
#include <boost/thread/mutex.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>
#include <yggr/container/multi_index_container.hpp>
#include <yggr/safe_container/safe_queue.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/thread/action_table.hpp>
#include <yggr/support/ref.hpp>
#include <yggr/thread/type_traits/tags.hpp>

#include <yggr/mplex/get_container_value_t.hpp>

#include <cassert>


namespace yggr
{
namespace thread
{
template<	typename Task_Center,
			typename ID_Parser,
			typename Mutex = boost::mutex,
			template<	typename _Task,
						typename _WorkRunner,
						typename _ID_Parser,
						typename _Mutex >
				class ActionTable = yggr::thread::action_table >
class dynamic_work_runner
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Task_Center task_center_type;

	typedef typename task_center_type::task_type task_type;
	typedef typename task_type::data_container_type task_data_container_type;

	typedef typename task_type::id_type task_id_type;
	typedef typename task_type::owner_info_type owner_info_type;
	typedef typename task_type::data_info_type data_info_type;

	typedef yggr::thread::type_traits::tag_dynamic_work_runner tag_type;

private:
	typedef Mutex mutex_type;

	typedef dynamic_work_runner this_type;

private:

	template<u32 runtim_tag, typename Nil_T = int>
	struct task_runtime_tag_switch;

	template<typename Nil_T>
	struct task_runtime_tag_switch<task_center::runtime_task_type::E_CAL_CONDITION, Nil_T>
	{
		typedef typename task_center_type::cdt_mark_type tag_type;
	};

	template<typename Nil_T>
	struct task_runtime_tag_switch<task_center::runtime_task_type::E_CAL_RESULT, Nil_T>
	{
		typedef typename task_center_type::rst_mark_type tag_type;
	};

	typedef safe_container::safe_queue<task_id_type, mutex_type> task_id_queue_type;
	typedef safe_container::safe_queue<owner_info_type, mutex_type> task_owner_info_queue_type;
	typedef safe_container::safe_queue<data_info_type, mutex_type> task_data_info_queue_type;


	struct tag_task_seq {};
	struct tag_task_id
	{
		typedef task_id_type id_type;
		typedef typename task_type::task_id_getter getter_type;
	};

	struct tag_task_owner_info
	{
		typedef owner_info_type id_type;
		typedef typename task_type::owner_info_getter getter_type;
	};

	struct tag_task_data_info
	{
		typedef data_info_type id_type;
		typedef typename task_type::data_info_getter getter_type;
	};

	typedef boost::multi_index::multi_index_container
			<
				task_type,
				boost::multi_index::indexed_by
				<
					boost::multi_index::sequenced
					<
						boost::multi_index::tag<tag_task_seq>
					>,

					boost::multi_index::hashed_unique
					<
						boost::multi_index::tag<tag_task_id>,
						boost::multi_index::const_mem_fun<task_type,
															task_id_type,
															&task_type::copy_task_id >
					>,

					boost::multi_index::hashed_non_unique
					<
						boost::multi_index::tag<tag_task_owner_info>,
						boost::multi_index::const_mem_fun<task_type,
															owner_info_type,
															&task_type::copy_owner_info >
					>,

					boost::multi_index::hashed_non_unique
					<
						boost::multi_index::tag<tag_task_data_info>,
						boost::multi_index::const_mem_fun<task_type,
															data_info_type,
															&task_type::copy_data_info >
					>
				>
			> task_list_type;


	typedef ActionTable<task_type, this_type, ID_Parser, mutex_type> action_table_type;

public:
	class shared_info
		: private nonable::noncopyable
	{
	public:
		typedef dynamic_work_runner work_runner_type;
		//typedef action_table_type action_table_type;

	private:
		typedef boost::atomic<std::size_t> count_type;

	public:
		explicit shared_info(size_type slow_runner_size = 0)
			:_slow_runner(0), _slow_runner_size(slow_runner_size)
		{
		}

		~shared_info(void)
		{
		}

		bool slow_runner_free(void) const
		{
			return _slow_runner.load() < _slow_runner_size;
		}

		bool plus_slow_runner(void)
		{
			std::size_t now_count = 0;
			do
			{
				now_count = _slow_runner.load();
				if(_slow_runner_size <= now_count)
				{
					return false;
				}

			} while(_slow_runner.compare_exchange_strong(now_count, now_count + 1));

			return true;
		}

		void minus_slow_runner(void)
		{
			std::size_t now_count = 0;

			do
			{
				now_count = _slow_runner.load();
				if(now_count == 0)
				{
					return false;
				}

			} while(_slow_runner.compare_exchange_strong(now_count, now_count - 1));

			return true;
		}

		bool slow_task_enable(void)
		{
			return _slow_runner_size;
		}

		void run_task(work_runner_type& runner, const task_type& task) const
		{
			tab.run_task(runner, task);
		}

		template<typename Calculator>
		void register_calculator(Calculator& cal)
		{
			typedef Calculator now_calculator_type;
			typedef typename now_calculator_type::error_type now_error_type;
			typedef typename now_calculator_type::owner_info_container_type owner_info_container_type;
			typedef typename mplex::get_container_value_t<owner_info_container_type>::type cal_owner_info_type;
			BOOST_MPL_ASSERT((boost::is_same<owner_info_type, cal_owner_info_type>));

			cal.template register_cal_object<this_type>(
					tab,
					boost::bind(
									&work_runner_type::template handler_result<
											now_error_type, owner_info_container_type>,
											_1, _2, _3, _4, _5, _6, _7
						)
				);
		}

		template<typename Calculator>
		void unregister_calculator(Calculator& cal)
		{
			cal.unregister_cal_object(tab);
		}

		void clear(void)
		{
			tab.clear();
		}

	public:
		action_table_type tab;
		const std::size_t _slow_runner_size;
		count_type _slow_runner;
	};

public:
	typedef boost::shared_ptr<shared_info> shared_info_ptr_type;

public:
	dynamic_work_runner(const task_center_type& tc, const shared_info_ptr_type& ptr)
		: _tc(yggr::support::ref::to_ref(tc)), _p_shared_info(ptr)
	{
	}

	~dynamic_work_runner(void)
	{
		clear();
	}

	void clear_tasks(void)
	{
		_task_list.clear();
		_kill_task_id_queue.clear();
		_kill_task_owner_info_queue.clear();
		_kill_task_data_info_queue.clear();
	}

	void clear(void)
	{
	    shared_info_ptr_type tmp;
		_p_shared_info.swap(tmp);
		_task_list.clear();
		_kill_task_id_queue.clear();
		_kill_task_owner_info_queue.clear();
		_kill_task_data_info_queue.clear();
	}

	bool operator()(void)
	{
		return handler_fix();
	}

	template<typename Tag>
	void post_kill_task(const typename Tag::id_type& id)
	{
		this_type::post_kill_task(id, (const Tag*)0);
	}

	template<typename Owners>
	void fix_result(u32 state, const Owners& owners,
						const data_info_type& data_info,
						u32 cal_type,
						task_data_container_type& rst)
	{
		prv_fix_result(state, owners, data_info, cal_type, rst);
	}

	static shared_info_ptr_type create_work_runner_shared_info(size_type slow_runner_size = 0)
	{
		shared_info_ptr_type ptr(new shared_info(slow_runner_size));
		return ptr;
	}

private:

	bool handler_fix(void)
	{
		handler_kill_task();
		if(!_task_list.empty() && slow_runner_free())
		{
			task_type task(_task_list.front());
			_task_list.pop_front();
			return handler_slow_condition(task);
		}

		return _tc.template get_task<typename task_center_type::cdt_mark_type>(
                                        boost::bind(&this_type::handler_condition, this, _1));
	}


	bool handler_slow_condition(const task_type& cdt)
	{
		if(!plus_slow_runner())
		{
			return false;
		}

		run_task(cdt);
		return true;
	}

	void handler_condition(const task_type& cdt)
	{
		assert((cdt.cal_type() == yggr::task_center::task_cal_type::E_CAL_FAST
				|| cdt.cal_type() == yggr::task_center::task_cal_type::E_CAL_SLOW));

		if(cdt.cal_type() == yggr::task_center::task_cal_type::E_CAL_SLOW)
		{
			if(!plus_slow_runner())
			{
				_task_list.push_back(cdt);
				return;
			}
		}

		run_task(cdt);
	}


	template<typename Error, typename Owners>
	void handler_result(const Error& e, u32 state,
							const Owners& owners,
							const data_info_type& data_info,
							u32 cal_type,
							task_data_container_type& rst)
	{
		typedef Error error_type;
		if(!e)
		{
			if(owners.empty())
			{
				return;
			}
			prv_fix_result(state, owners, data_info, cal_type, rst);
			return;
		}

		exception::exception::throw_error(e);
	}

	template<typename Owners>
	void prv_fix_result(u32 state, const Owners& owners,
							const data_info_type& data_info,
							u32 cal_type,
							task_data_container_type& rst)
	{
		typedef Owners owners_type;
		typedef typename owners_type::const_iterator owners_citer_type;

		assert(!owners.empty());

		bool b = (owners.size() == 1);
		switch(state)
		{
		case yggr::task_center::runtime_task_type::E_CAL_CONDITION:
			{
				for(owners_citer_type i = owners.begin(), isize = owners.end(); i != isize; ++i)
				{
					_tc.template send<
                            typename task_center_type::cdt_mark_type>(
                                        *i, data_info, cal_type,
                                        b? yggr::move::move_helper::forced_move(rst) : rst);
				}
			}
			break;

		case yggr::task_center::runtime_task_type::E_CAL_RESULT:
			{
				for(owners_citer_type i = owners.begin(), isize = owners.end(); i != isize; ++i)
				{
					_tc.template send<
                        typename task_center_type::rst_mark_type>(
                            *i, data_info, cal_type,
							b? yggr::move::move_helper::forced_move(rst) : rst);
				}
			}
			break;
		}
	}

	void handler_kill_task(void)
	{
		prv_handler_kill_task<tag_task_id>(_kill_task_id_queue);
		prv_handler_kill_task<tag_task_owner_info>(_kill_task_owner_info_queue);
		prv_handler_kill_task<tag_task_data_info>(_kill_task_data_info_queue);
	}

	bool slow_runner_free(void)
	{
		return _p_shared_info? _p_shared_info->slow_runner_free() : false;
	}

	bool plus_slow_runner(void)
	{
		return _p_shared_info? _p_shared_info->plus_slow_runner() : false;
	}

	void minus_slow_runner(void)
	{
		return _p_shared_info? _p_shared_info->minus_slow_runner() : false;
	}

	bool slow_task_enable(void)
	{
		return _p_shared_info? _p_shared_info->slow_task_enable() : false;
	}

private:

	void post_kill_task(const task_id_type& id,
							const tag_task_id*)
	{
		_kill_task_id_queue.push(id);
	}

	void post_kill_task(const owner_info_type& id,
							const tag_task_owner_info*)
	{
		_kill_task_owner_info_queue.push(id);
	}

	void post_kill_task(const task_id_type& id,
							const tag_task_data_info*)
	{
		_kill_task_data_info_queue.push(id);
	}

	template<typename Tag, typename Queue>
	void prv_handler_kill_task(Queue& queue)
	{
		typedef typename Tag::id_type now_id_type;
		typedef typename Tag::getter_type now_getter_type;
		if(queue.empty())
		{
			return;
		}

		if(_task_list.empty())
		{
			queue.clear();
		}

		now_id_type rm_id;
		for(;!queue.empty();)
		{
			if(queue.pop(rm_id))
			{
				now_getter_type getter;
				if(getter(_task_list.front()) == rm_id)
				{
					continue;
				}

				boost::multi_index::get<Tag>(_task_list).erase(rm_id);
			}
		}
	}

	void run_task(const task_type& cdt)
	{
		if(cdt.empty() || !_p_shared_info)
		{
			return;
		}

		_p_shared_info->run_task(*this, cdt);
	}

private:
	task_center_type& _tc;
	task_list_type _task_list;

	task_id_queue_type _kill_task_id_queue;
	task_owner_info_queue_type _kill_task_owner_info_queue;
	task_data_info_queue_type _kill_task_data_info_queue;

	shared_info_ptr_type _p_shared_info;
};

} // namespace thread
} // namespace yggr

#endif //__YGGR_THREAD_DYNAMIC_WORK_RUNNER_HPP__
