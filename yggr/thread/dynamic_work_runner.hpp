//dynamic_work_runner.hpp

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

#ifndef __YGGR_THREAD_DYNAMIC_WORK_RUNNER_HPP__
#define __YGGR_THREAD_DYNAMIC_WORK_RUNNER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/to_ref_helper.hpp>
#include <yggr/move/move.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_sequenced_index.hpp>

#include <yggr/safe_container/safe_deque.hpp>

#include <yggr/thread/action_table.hpp>
#include <yggr/thread/type_traits/tags.hpp>

#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index_container.hpp>

#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/range/value_type.hpp>

#include <cassert>

namespace yggr
{
namespace thread
{
namespace dynamic_work_runner_detail
{

// fix msvc c1001
struct tag_task_seq {};
struct tag_task_id {};
struct tag_task_owner_info {};
struct tag_task_data_info {};

} // namespace dynamic_work_runner_detail
} // namespace thread
} // namespace yggr

namespace yggr
{
namespace thread
{

template<	typename Task_Center,
			typename ID_Parser,
			typename Mutex = void,
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
	typedef ID_Parser id_parser_type;

	typedef typename task_center_type::task_type task_type;
	typedef typename task_type::data_container_type task_data_container_type;

	typedef typename task_type::id_type task_id_type;
	typedef typename task_type::owner_info_type owner_info_type;
	typedef typename task_type::data_info_type data_info_type;

	typedef yggr::thread::type_traits::tag_dynamic_work_runner tag_type;

private:
	typedef Mutex tpl_arg_mutex_type;

private:
	typedef dynamic_work_runner this_type;

private:
	typedef ActionTable<task_type, this_type, id_parser_type, tpl_arg_mutex_type> action_table_type;

public:
	// don't move to outside
	class shared_info
		: private nonable::noncopyable
	{
	public:
		typedef dynamic_work_runner work_runner_type;

	private:
		typedef boost::atomic<std::size_t> count_type;

	public:
		shared_info(void)
			:_slow_runner_size(0),
                _slow_runner(0)

		{
		}

		shared_info(size_type slow_runner_size)
			:_slow_runner_size(slow_runner_size),
                _slow_runner(0)

		{
		}

		~shared_info(void)
		{
		}

	public:
		inline bool slow_runner_free(void) const
		{
			return _slow_runner.load() < _slow_runner_size.load();
		}

		bool plus_slow_runner(void)
		{
			std::size_t now_count = 0;
			do
			{
				now_count = _slow_runner.load();
				if(_slow_runner_size.load() <= now_count)
				{
					return false;
				}

			} while(_slow_runner.compare_exchange_weak(now_count, now_count + 1));

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
					return;
				}

			} while(_slow_runner.compare_exchange_weak(now_count, now_count - 1));
		}

		inline bool slow_task_enable(void) const
		{
			return !!(_slow_runner_size.load());
		}

		inline void run_task(work_runner_type& runner, task_type& task) const
		{
			tab.run_task(runner, task);
		}

		template<typename Calculator> inline
		void register_calculator(Calculator& cal)
		{
			typedef Calculator now_calculator_type;
			typedef typename now_calculator_type::error_type now_error_type;
			typedef typename now_calculator_type::owner_info_container_type owner_info_container_type;
			typedef typename boost::range_value<owner_info_container_type>::type cal_owner_info_type;
			BOOST_MPL_ASSERT((boost::is_same<owner_info_type, cal_owner_info_type>));

			cal.template register_cal_object<dynamic_work_runner>(
					tab,
					boost::bind(
						&work_runner_type::template prv_handler_result
						<
							now_error_type,
							owner_info_container_type
						>,
						_1, _2, _3, _4, _5, _6, _7 ) );

			return;
		}

		template<typename Calculator> inline
		void unregister_calculator(Calculator& cal)
		{
			cal.unregister_cal_object(tab);
		}

		inline void clear(void)
		{
			tab.clear();
		}

	public:
		action_table_type tab;
		count_type _slow_runner_size;
		count_type _slow_runner;
	};

public:
	typedef ::yggr::shared_ptr<shared_info> shared_info_ptr_type;

private:
	typedef safe_container::safe_deque<task_id_type, tpl_arg_mutex_type> task_id_queue_type;
	typedef safe_container::safe_deque<owner_info_type, tpl_arg_mutex_type> task_owner_info_queue_type;
	typedef safe_container::safe_deque<data_info_type, tpl_arg_mutex_type> task_data_info_queue_type;

public:
	typedef dynamic_work_runner_detail::tag_task_seq tag_task_seq_type;
	typedef dynamic_work_runner_detail::tag_task_id tag_task_id_type;
	typedef dynamic_work_runner_detail::tag_task_owner_info tag_task_owner_info_type;
	typedef dynamic_work_runner_detail::tag_task_data_info tag_task_data_info_type;

private:
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			task_type,
			boost::multi_index::indexed_by
			<
				boost::multi_index::sequenced
				<
					boost::multi_index::tag<tag_task_seq_type>
				>,

				boost::multi_index::hashed_unique
				<
					boost::multi_index::tag<tag_task_id_type>,
					boost::multi_index::const_mem_fun<task_type,
														task_id_type,
														&task_type::copy_task_id >
				>,

				boost::multi_index::hashed_non_unique
				<
					boost::multi_index::tag<tag_task_owner_info_type>,
					boost::multi_index::const_mem_fun<task_type,
														owner_info_type,
														&task_type::copy_owner_info >
				>,

				boost::multi_index::hashed_non_unique
				<
					boost::multi_index::tag<tag_task_data_info_type>,
					boost::multi_index::const_mem_fun<task_type,
														data_info_type,
														&task_type::copy_data_info >
				>
			>
		> task_list_type;

public:
	dynamic_work_runner(const task_center_type& tc, const shared_info_ptr_type& ptr)
		: _tc(yggr::to_ref_helper::to_ref(tc)), _p_shared_info(ptr)
	{
	}

	~dynamic_work_runner(void)
	{
		this_type::clear();
	}

public:
	inline void clear_tasks(void)
	{
		typename task_list_type::base_type tmp_task_list;
		typename task_id_queue_type::base_type tmp_task_id_queue;
		typename task_owner_info_queue_type::base_type tmp_owner_info_queue;
		typename task_data_info_queue_type::base_type tmp_task_data_info_queue;


		_task_list.using_handler(
			boost::bind(
				&this_type::prv_handler_swap_all,
				this, _1,
				boost::ref(tmp_task_list),
				boost::ref(tmp_task_id_queue),
				boost::ref(tmp_owner_info_queue),
				boost::ref(tmp_task_data_info_queue) ));
	}

	inline void clear(void)
	{
	    shared_info_ptr_type tmp;
		_p_shared_info.swap(tmp);

		this_type::clear_tasks();
	}

	inline bool operator()(void)
	{
		return this_type::prv_execute_fixing();
	}

	template<typename Tag, typename ID> inline
	void post_kill_task(BOOST_RV_REF(ID) id)
	{
		this_type::prv_post_kill_task(boost::move(id), static_cast<const Tag*>(0));
	}

	template<typename Tag, typename ID> inline
	void post_kill_task(const ID& id)
	{
		this_type::prv_post_kill_task(id, static_cast<const Tag*>(0));
	}

	template<typename Owners> inline
	void fix_result(u32 state,
						const Owners& owners,
						const data_info_type& data_info,
						u32 cal_type,
						task_data_container_type& rst)
	{
		this_type::prv_fixing_result(state, owners, data_info, cal_type, rst);
	}

	inline static shared_info_ptr_type create_work_runner_shared_info(size_type slow_runner_size = 0)
	{
		return construct_shared<shared_info>(yggr_nothrow_new shared_info(slow_runner_size));
	}

private:
	inline bool prv_execute_fixing(void)
	{
		this_type::prv_execute_kill_tasks();
		task_type task;
		if(this_type::prv_slow_runner_free()
			&& this_type::prv_plus_slow_runner()
			&& _task_list.pop_front(task))
		{
			this_type::prv_run_task(task);
			return true;
		}

		return
			_tc.template get_task<typename task_center_type::cdt_mark_type>(
				boost::bind(&this_type::prv_handler_condition, this, _1));
	}

	inline void prv_execute_kill_tasks(void)
	{
		typename task_list_type::base_type tmp_task_list;
		typename task_id_queue_type::base_type tmp_task_id_queue;
		typename task_owner_info_queue_type::base_type tmp_owner_info_queue;
		typename task_data_info_queue_type::base_type tmp_task_data_info_queue;

		_task_list.using_handler(
			boost::bind(
				&this_type::prv_handler_kill_tasks,
				this, _1,
				boost::ref(tmp_task_list),
				boost::ref(tmp_task_id_queue),
				boost::ref(tmp_owner_info_queue),
				boost::ref(tmp_task_data_info_queue) ));
	}

	inline void prv_handler_kill_tasks(typename task_list_type::base_type& base,
										typename task_list_type::base_type& out_base,
										typename task_id_queue_type::base_type& task_id_queue,
										typename task_owner_info_queue_type::base_type& owner_info_queue,
										typename task_data_info_queue_type::base_type& task_data_info_queue)
	{

		this_type::prv_handler_swap_all_queue(base, task_id_queue, owner_info_queue, task_data_info_queue);

		this_type::prv_s_kill_task<tag_task_id_type>(base, out_base, task_id_queue);
		this_type::prv_s_kill_task<tag_task_owner_info_type>(base, out_base, owner_info_queue);
		this_type::prv_s_kill_task<tag_task_data_info_type>(base, out_base, task_data_info_queue);
	}


	template<typename Tag, typename BaseQueue>
	static void prv_s_kill_task(typename task_list_type::base_type& base,
								typename task_list_type::base_type& out_base,
								const BaseQueue& queue)
	{
		typedef typename task_list_type::base_type base_task_list_type;
		typedef typename boost::multi_index::index<base_task_list_type, Tag>::type now_index_cont_type;
		typedef typename now_index_cont_type::iterator now_index_iter_type;

		typedef typename boost::multi_index::nth_index<base_task_list_type, 0>::type rm_index_cont_type;
		typedef typename rm_index_cont_type::iterator rm_index_iter_type;

		typedef typename BaseQueue::const_iterator queue_iter_type;

		if(base.empty())
		{
			return;
		}

		now_index_cont_type& find_index_cont = base.template get<Tag>();
		rm_index_cont_type& src_index_cont = base.template get<0>();
		//rm_index_cont_type& dst_index_cont = out_base.template get<0>();

		now_index_iter_type find_iter;
		rm_index_iter_type rm_iter;

		for(queue_iter_type i = queue.begin(), isize = queue.end(); i != isize; ++i)
		{
			if(((find_iter = find_index_cont.find(*i)) != find_index_cont.end())
				&& ((rm_iter = src_index_cont.iterator_to(*find_iter)) != src_index_cont.begin()))
			{
				out_base.splice(out_base.end(), src_index_cont, rm_iter);
			}
		}
	}

	inline void prv_run_task(task_type& cdt)
	{
		if(cdt.empty() || !_p_shared_info)
		{
			return;
		}

		_p_shared_info->run_task(*this, cdt);
	}

	inline void prv_handler_condition(task_type& cdt)
	{
		assert((cdt.cal_type() == yggr::task_center::task_cal_type::E_CAL_FAST
				|| cdt.cal_type() == yggr::task_center::task_cal_type::E_CAL_SLOW));

		if(cdt.cal_type() == task_center::task_cal_type::E_CAL_SLOW
			&& !this_type::prv_plus_slow_runner())
		{
			_task_list.push_back(boost::move(cdt));
		}
		else
		{
			this_type::prv_run_task(cdt);
		}
	}

	template<typename Error, typename Owners> inline
	void prv_handler_result(const Error& e, u32 state,
							const Owners& owners,
							const data_info_type& data_info,
							u32 cal_type,
							task_data_container_type& rst)
	{
		typedef Error error_type;
		if(!e)
		{
			this_type::prv_fixing_result(state, owners, data_info, cal_type, rst);
		}
		else
		{
			exception::exception::throw_error(e);
		}
	}

	template<typename Owners>
	void prv_fixing_result(u32 state, const Owners& owners,
							const data_info_type& data_info,
							u32 cal_type,
							task_data_container_type& rst)
	{
		typedef Owners owners_type;
		typedef typename owners_type::const_iterator owners_citer_type;

		if(owners.empty())
		{
			return;
		}

		switch(state)
		{
		case yggr::task_center::runtime_task_type::E_CAL_CONDITION:
			{
				for(owners_citer_type i = owners.begin(), isize = owners.end(); i != isize; ++i)
				{
					_tc.template send<typename task_center_type::cdt_mark_type>(*i, data_info, cal_type, rst);
				}
			}
			break;

		case yggr::task_center::runtime_task_type::E_CAL_RESULT:
			{
				for(owners_citer_type i = owners.begin(), isize = owners.end(); i != isize; ++i)
				{
					_tc.template send<typename task_center_type::rst_mark_type>(*i, data_info, cal_type, rst);
				}
			}
			break;
		}
	}

	inline void prv_handler_swap_all_queue(typename task_list_type::base_type&,
											typename task_id_queue_type::base_type& task_id_queue,
											typename task_owner_info_queue_type::base_type& owner_info_queue,
											typename task_data_info_queue_type::base_type& task_data_info_queue)
	{
		_kill_task_id_queue.swap(task_id_queue);
		_kill_task_owner_info_queue.swap(owner_info_queue);
		_kill_task_data_info_queue.swap(task_data_info_queue);
	}

	inline void prv_handler_swap_all(typename task_list_type::base_type& base,
										typename task_list_type::base_type& out_base,
										typename task_id_queue_type::base_type& task_id_queue,
										typename task_owner_info_queue_type::base_type& owner_info_queue,
										typename task_data_info_queue_type::base_type& task_data_info_queue)
	{
		this_type::prv_handler_swap_all_queue(base, task_id_queue, owner_info_queue, task_data_info_queue);
		out_base.swap(base);
	}

	inline bool prv_slow_runner_free(void) const
	{
		return _p_shared_info && _p_shared_info->slow_runner_free();
	}

	inline bool prv_plus_slow_runner(void)
	{
		return _p_shared_info && _p_shared_info->plus_slow_runner();
	}

	inline void prv_minus_slow_runner(void)
	{
		return _p_shared_info && _p_shared_info->minus_slow_runner();
	}

	inline bool prv_slow_task_enable(void) const
	{
		return _p_shared_info && _p_shared_info->slow_task_enable();
	}

	inline void prv_post_kill_task(BOOST_RV_REF(task_id_type) id,
									const tag_task_id_type*)
	{
		_kill_task_id_queue.push_back(boost::move(id));
	}

	inline void prv_post_kill_task(const task_id_type& id,
									const tag_task_id_type*)
	{
		_kill_task_id_queue.push_back(id);
	}

	inline void prv_post_kill_task(BOOST_RV_REF(owner_info_type) id,
									const tag_task_owner_info_type*)
	{
		_kill_task_owner_info_queue.push_back(boost::move(id));
	}

	inline void prv_post_kill_task(const owner_info_type& id,
									const tag_task_owner_info_type*)
	{
		_kill_task_owner_info_queue.push_back(id);
	}

	inline void prv_post_kill_task(BOOST_RV_REF(task_id_type) id,
									const tag_task_data_info_type*)
	{
		_kill_task_data_info_queue.push_back(boost::move(id));
	}

	inline void prv_post_kill_task(const task_id_type& id,
									const tag_task_data_info_type*)
	{
		_kill_task_data_info_queue.push_back(id);
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
