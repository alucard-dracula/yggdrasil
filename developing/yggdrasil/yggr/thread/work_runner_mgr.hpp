//work_runner_mgr.hpp
#ifndef __YGGR_THREAD_WORK_RUNNER_MANAGER_HPP__
#define __YGGR_THREAD_WORK_RUNNER_MANAGER_HPP__

#include <lockfree/CAS.h>
#include <base/yggrdef.h>
#include <boost/any.hpp>
#include <list>
#include <lockfree/lf_queue.hpp>
#include <algorithm>
#include <base/Exception.hpp>
#include <boost/noncopyable.hpp>



namespace yggr
{
namespace thread
{
template<class Data_Center>
class work_runner 
	: private boost::noncopyable
{
public:
	typedef Data_Center data_center_type;
	typedef typename data_center_type::task_id_type task_id_type;
	typedef typename data_center_type::task_type task_type;
	typedef typename task_type::id_type id_type;
	typedef typename task_type::data_type data_type;
	typedef typename task_type::user_id_type user_id_type;
	

	typedef typename base_runner_type::Error error_type;
	
private:
	typedef std::list<task_type> task_list_type;
	typedef typename task_list_type::iterator task_list_iter;
	typedef typename task_list_type::const_iterator task_list_c_iter;

	typedef lf::lf_queue<task_id_type> task_id_queue;

public:
	
	
public:
	work_runner(data_center_type& dc)
		:_dc(dc)
	{
	}

	~work_runner(void)
	{
		clear();
	}

	void clear()
	{
		_task_list.clear();
		_kill_task_id_queue.clear();
	}

	bool operator(void)
	{
		return handler_fix();
	}

	void post_kill_task_id(const task_id_type& task_id)
	{
		_kill_task_id_queue.push(task_id);
	}

	static void init_slow_runner(u32 size)
	{
		while(!CAS::cas32(&_slow_runner_size,  _slow_runner_size, size));
	}

private:
	bool handler_fix(void)
	{
		handler_kill_task();
		if(!_task_list.empty() && work_runner::slow_runner_free())
		{

			task_type task(_task_list.front());
			_task_list.pop_front();
			return handler_slow_condition(task);
		}

		_dc.pop_condition(boost::bind(&work_runner::handler_condition, this, _1, _2, _3));

		return false;
	}

	bool handler_slow_condition(const task_type& cdt)
	{
		if(work_runner::plus_slow_runner())
		{
			base_runner_type::calculate(cdt.get(), boost::bind(&work_runner::handler_result, this, _1, _2, _3, _4, _5));
			return true;
		}

		return false;
	}

	bool handler_condition(bool geted, u32 state, const task_type& cdt)
	{
		if(!geted)
		{
			return false;
		}

		if(state == task_type::E_CONDITION_SLOW)
		{
			if(work_runner::plus_slow_runner())
			{
				base_runner_type::calculate(cdt.get(), boost::bind(&work_runner::handler_result, this, _1, _2, _3, _4, _5));
			}
			else
			{
				_task_list.push_back(cdt);
			}

			return true;
		}

		if(state == task_type::E_CONDITION_FAST)
		{
			base_runner_type::calculate(cdt.get(), boost::bind(&work_runner::handler_result, this,  _1, _2, _3, _4, _5));
			return true;
		}


		return true;
	}


	void handler_result(const error_type& e, u32 state, task_id_type task_id, user_id_type uid, const boost::any& rst)
	{
		if(!e)
		{
			switch(state)
			{
			case base_runner_type::E_CAL_CONDITION:
				{
					_dc.push_condition(task_id, uid, state, rst);
				}
				break;

			case base_runner_type::E_CAL_RESULT:
				{
					_dc.push_result(task_id, uid,state, rst);
				}
				break;
			}

			return;
		}

		exception::Exception* pe = obj_single<exception::Exception>::get_ins();

		if(pe)
		{
			pe->post(exception::Exception::make_error(e));
		}
	}

	bool handler_kill_task(void)
	{
		bool brun = false;
		task_id_type task_id;
		for(;!_kill_task_id_queue.empty();)
		{
			if(_kill_task_id_queue.pop(task_id))
			{
				task_list_iter iter = std::find(_task_list.begin(), _task_list.end(), task_id);
				if(iter == _task_list.end())
				{
					continue;
				}

				if(iter == _task_list.begin())
				{
					brun = true;
				}

				_task_list.erase(iter);
			}
		}
		return brun;
	}

	static bool slow_runner_free(void)
	{
		return _slow_runner < _slow_runner_size;
	}

	static bool plus_slow_runner(void)
	{
		if(_slow_runner < _slow_runner_size)
		{
			return CAS::cas32(&_slow_runner,  _slow_runner, _slow_runner + 1);
		}
		return false;
	}

	static void minus_slow_runner(void)
	{
		if(_slow_runner > 0)
		{
			while(!CAS::cas32(&_slow_runner,  _slow_runner, _slow_runner - 1));
			return true;
		}
		return false;
	}

	static bool slow_task_enable(void)
	{
		return _slow_runner_size > 0;
	}

private:
	data_center_type& _dc;
	task_list_type _task_list;
	task_id_queue _kill_task_id_queue;

	static u32 _slow_runner_size;
	static u32 _slow_runner;
};

template<typename Data_Center, typename Base_Runner>
u32 work_runner<Data_Center, Base_Runner>::_slow_runner_size = 0;

template<typename Data_Center, typename Base_Runner>
u32 work_runner<Data_Center, Base_Runner>::_slow_runner = 0; 
	
} // namespace thread
} // namespace yggr

#endif //__YGGR_THREAD_WORK_RUNNER_MANAGER_HPP__