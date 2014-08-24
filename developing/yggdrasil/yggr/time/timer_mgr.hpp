//timer_mgr.hpp

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

#ifndef __YGGR_TIME_TIMER_MGR_HPP__
#define __YGGR_TIME_TIMER_MGR_HPP__

#include <boost/tuple/tuple.hpp>
#include <boost/thread/mutex.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/base/interface_ptr.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/safe_container/safe_map.hpp>
#include <yggr/safe_container/safe_buffered_object.hpp>
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/time/timer_task_state.hpp>

namespace yggr
{
namespace time
{

template<typename Task_ID,
			typename Thread_Config = yggr::thread::boost_thread_config_type>
class timer_mgr
	: private nonable::noncopyable
{
public:
//	enum
//	{
//		E_timer_task_state = 0,
//		E_timer_task_ran,
//		E_timer_task_not_ran,
//		E_timer_task_remove,
//		E_id_compile_u32 = 0xffffffff
//	};

	typedef Task_ID task_id_type;

private:
	typedef timer_mgr this_type;

	typedef Thread_Config thread_config_type;
	typedef typename thread_config_type::thread_type thread_type;
	typedef typename thread_config_type::thread_group_type thread_group_type;
	typedef typename thread_config_type::time_type time_type;

	typedef timer_task_state task_state_type;

	class base_timer_task
	{
	public:
		base_timer_task(const task_id_type& id)
			: _id(id)
		{
		}

		virtual ~base_timer_task(void)
		{
		}

		const task_id_type& id(void) const
		{
			return _id;
		}

		virtual yggr::u32 do_timer(void) = 0;

	private:
		task_id_type _id;
	};

	template<typename Time_Task_Pak>
	class timer_task
		: public base_timer_task,
			private nonable::noncopyable
	{
	public:
		typedef base_timer_task base_type;
		typedef Time_Task_Pak time_task_pak_type;
		//typedef Time_Task_Pak_Init time_task_pak_init_type;
	public:

		timer_task(const task_id_type& id)
			: base_type(id)
		{
		}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		timer_task(const task_id_type& id \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS )) \
			: base_type(id), _time_task_pak(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)) {}

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

		virtual ~timer_task(void)
		{
		}

		virtual yggr::u32 do_timer(void)
		{
			return _time_task_pak();
		}

	private:
		time_task_pak_type _time_task_pak;
	};

	typedef interface_ptr<base_timer_task> i_base_time_task_type;

	typedef safe_container::safe_map<task_id_type, i_base_time_task_type> time_task_map_type;

	template<typename Val>
	class time_obj_container
	{
	public:
		typedef Val val_type;
		typedef std::list<val_type> list_type;

	public:
		time_obj_container(void)
		{
		}

		time_obj_container(const time_obj_container& right)
			: _list(right._list)
		{
		}

		~time_obj_container(void)
		{
			clear();
		}

		time_obj_container& operator=(const time_obj_container& right)
		{
			_list = right._list;
			return *this;
		}

		void splice_to(time_obj_container& right)
		{
			_list.splice(_list.end(), right._list);
			_list.swap(right._list);
		}

		void insert(const val_type& timer_task)
		{
			if(!timer_task)
			{
				return;
			}

			_list.push_back(timer_task);
		}

		template<typename Handler>
		void get(const Handler& handler)
		{
			handler(_list);
		}

		yggr::size_type size(void) const
		{
			return _list.size();
		}

		void clear(void)
		{
			list_type list;
			_list.swap(list);
			list.clear();
		}

		bool empty(void) const
		{
			return _list.empty();
		}

	private:
		list_type _list;
	};

	typedef time_obj_container<i_base_time_task_type> time_task_container_type;
	typedef typename time_task_container_type::list_type task_list_type;
	typedef typename task_list_type::iterator task_list_iter_type;
	typedef typename task_list_type::const_iterator task_list_citer_type;

	typedef yggr::safe_container::safe_buffered_object<time_task_container_type> time_task_queue_type;

	typedef time_obj_container<task_id_type> task_id_container_type;
	typedef typename task_id_container_type::list_type task_id_list_type;
	typedef typename task_id_list_type::iterator task_id_list_iter_type;
	typedef typename task_id_list_type::const_iterator task_id_list_citer_type;

	typedef yggr::safe_container::safe_buffered_object<task_id_container_type> time_task_id_queue_type;

public:
	timer_mgr(void)
		: _brun(false), _tm_step(thread_config_type::E_def_sleep_time_sec, thread_config_type::E_def_sleep_time_nsec)
	{
	}

	timer_mgr(const time_type& tm_step)
		: _brun(false), _tm_step(tm_step)
	{
	}

	~timer_mgr(void)
	{
		assert(!_brun);
	}

	template<typename Time_Task_Pak>
	task_id_type add_task(void)
	{
		typedef Time_Task_Pak task_pak_type;
		typedef timer_task<task_pak_type> time_task_type;

		i_base_time_task_type ptr(new time_task_type(gen_id()));

		return ptr && _task_queue.insert(ptr)? ptr->id() : task_id_type();
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Time_Task_Pak \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	task_id_type add_task(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		typedef Time_Task_Pak task_pak_type; \
		typedef timer_task<task_pak_type> time_task_type; \
		i_base_time_task_type ptr( \
					new time_task_type(gen_id() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA))); \
		return ptr && _task_queue.insert(ptr)? ptr->id() : task_id_type(); }

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

	void start(void)
	{
		if(_brun)
		{
			return;
		}

		_brun = true;

		if(!(_trd_group.create_thread(boost::bind(&this_type::run, this))))
		{
			_brun = false;
		}
	}

	void erase(const task_id_type& id)
	{
		//_task_map.erase(id);
		_rm_task_id_queue.insert(id);

	}

	void join(void)
	{
		_trd_group.join_all();
	}

	void stop(void)
	{
		_brun = false;
		_trd_group.join_all();
	}

	void clear(void)
	{
		_task_queue.clear();
		_rm_task_id_queue.clear();
		_task_map.clear();
	}

private:

	task_id_type gen_id(void) const
	{
		yggr::ids::id_generator<task_id_type> gen;
		return gen();
	}

	void remove_task_of_ids(void)
	{
		remove_task_of_ids_helper(boost::bind(&this_type::handler_remove_task_of_ids, this, _1));
	}

	void handler_remove_task_of_ids(task_id_list_type& list)
	{
		for(task_id_list_iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
		{
			if(*i)
			{
				_task_map.erase(*i);
			}
		}

		list.clear();
	}

	template<typename Handler>
	void remove_task_of_ids_helper(const Handler& handler)

	{
		typedef Handler handler_type;
		_rm_task_id_queue.get(boost::bind(&task_id_container_type::template get<handler_type>, _1, boost::cref(handler)));
	}

	void get_tasks(void)
	{
		get_tasks_helper(boost::bind(&this_type::handler_get_tasks, this, _1));
	}

	template<typename Handler>
	void get_tasks_helper(const Handler& handler)
	{
		typedef Handler handler_type;
		_task_queue.get(boost::bind(&time_task_container_type::template get<handler_type>, _1, boost::cref(handler)));
	}

	void handler_get_tasks(task_list_type& list)
	{
		for(task_list_iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
		{
			if(*i)
			{
				_task_map.insert((*i)->id(), *i);
			}
		}

		list.clear();
	}

	void run(void)
	{
		size_type run_size = 0;
		for(;_brun;)
		{
			get_tasks();
			remove_task_of_ids();
			if(_task_map.empty())
			{
				thread_type::yield();
				time_type tm;
				tm += _tm_step;
				thread_type::sleep(tm);
				continue;
			}

			run_size = 0;
			_task_map.use_handler(boost::bind(&this_type::handler_run_task, this, _1, boost::ref(run_size)));

			if(!run_size)
			{
				thread_type::yield();
				time_type tm;
				tm += _tm_step;
				thread_type::sleep(tm);
			}
		}
	}

	void handler_run_task(typename time_task_map_type::base_type& task_map, size_type& run_size)
	{
		typedef typename time_task_map_type::base_type base_map_type;
		typedef typename base_map_type::iterator base_map_iter_type;

		size_type tmp_run_size = 0;
		for(base_map_iter_type i = task_map.begin(), isize = task_map.end(), rm; _brun && i != isize;)
		{
			if(!(i->second))
			{
			    rm = i;
			    ++i;
				task_map.erase(rm);
				continue;
			}

			yggr::u32 sat = i->second->do_timer();

			switch(sat)
			{
			case task_state_type::E_timer_task_ran:
				++tmp_run_size;
				break;
			case task_state_type::E_timer_task_not_ran:
				break;
			case task_state_type::E_timer_task_remove:
				rm = i;
				++i;
				task_map.erase(rm);
				continue;
			}

			++i;
		}

		run_size = tmp_run_size;
	}


private:
	bool _brun;
	time_task_map_type _task_map;
	time_task_queue_type _task_queue;
	time_type _tm_step;
	time_task_id_queue_type _rm_task_id_queue;
	thread_group_type _trd_group;
};

} // namespace time
} // namespace yggr

#endif //__YGGR_TIME_TIMER_MGR_HPP__
