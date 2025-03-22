//action_table.hpp

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

#ifndef __YGGR_THREAD_ACTION_TABLE_HPP__
#define __YGGR_THREAD_ACTION_TABLE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/tuple/tuple.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/task_center/support/task_real_data_getter.hpp>

#include <yggr/thread/action_helper/action_id_parser.hpp>
#include <yggr/thread/action_helper/make_reg_id_parser.hpp>

#ifdef YGGR_USE_SEH
#	include <yggr/seh/seh.hpp>
#endif // YGGR_USE_SEH

#include <boost/ref.hpp>

#include <algorithm>
#include <cassert>

namespace yggr
{
namespace thread
{
namespace detail
{


template<typename Task, typename WorkRunner>
class action_table_base_calculator
{
public:
	typedef Task this_task_type;
	typedef WorkRunner this_work_runner_type;

private:
	typedef action_table_base_calculator this_type;

public:
	virtual ~action_table_base_calculator(void)
	{
	}

public:
	virtual void run_handler(this_work_runner_type& runner, this_task_type& task) const = 0;

};

template<typename Task, typename WorkRunner,
			typename RealData, typename CallHandler, typename BackHandler, typename SehHandler = int>
class action_table_calculator;

#ifdef YGGR_USE_SEH //seh ver

template<typename Task, typename WorkRunner,
			typename RealData, typename CallHandler, typename BackHandler>
class action_table_calculator<Task, WorkRunner, RealData, CallHandler, BackHandler, int>
	: public action_table_base_calculator<Task, WorkRunner>
{
public:
	typedef Task this_task_type;
	typedef WorkRunner this_work_runner_type;
	typedef action_table_base_calculator<this_task_type, this_work_runner_type> base_type;

private:
	typedef RealData real_data_type;
	typedef CallHandler call_handler_type;
	typedef BackHandler back_handler_type;

private:
	typedef task_center::support::task_real_data_getter<this_task_type> task_real_data_getter_type;

private:
	typedef action_table_calculator this_type;
	
public:
	action_table_calculator(const call_handler_type& call_handler, const back_handler_type& back_handler)
		: _call_handler(call_handler), _back_handler(back_handler)
	{
	}

	~action_table_calculator(void)
	{
	}

public:
	virtual void run_handler(this_work_runner_type& runner, this_task_type& task) const
	{
		seh::seh_type::s_safe_invoke(
			boost::bind(&this_type::prv_run_it_wrap, this, boost::ref(runner), boost::ref(task)));
	}

private:
	inline void prv_run_it_wrap(this_work_runner_type& runner, this_task_type& task) const
	{
		task_real_data_getter_type getter;
#	if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		real_data_type* pdata = getter.operator()<real_data_type>(task);
#	else
		real_data_type* pdata = getter.template operator()<real_data_type>(task);
#	endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)

		if(pdata)
		{
			(_call_handler)(
				task.owner_info(), boost::ref(*pdata), boost::addressof(runner), _back_handler);
		}
	}

private:
	call_handler_type _call_handler;
	back_handler_type _back_handler;
};


template<typename Task, typename WorkRunner,
			typename RealData, typename CallHandler, typename BackHandler, typename SehHandler>
class action_table_calculator
	: public action_table_base_calculator<Task, WorkRunner>
{
public:
	typedef Task this_task_type;
	typedef WorkRunner this_work_runner_type;
	typedef action_table_base_calculator<this_task_type, this_work_runner_type> base_type;

private:
	typedef RealData real_data_type;
	typedef CallHandler call_handler_type;
	typedef BackHandler back_handler_type;
	typedef SehHandler seh_handler_type;

private:
	typedef task_center::support::task_real_data_getter<this_task_type> task_real_data_getter_type;

private:
	typedef action_table_calculator this_type;

public:
	action_table_calculator(const call_handler_type& call_handler,
							const back_handler_type& back_handler,
							const seh_handler_type& seh_handler)
		: _call_handler(call_handler),
			_back_handler(back_handler),
			_seh_handler(seh_handler)
	{
	}

	~action_table_calculator(void)
	{
	}

public:
	virtual void run_handler(this_work_runner_type& runner, this_task_type& task) const
	{
		task_real_data_getter_type getter;
		if(!seh::seh_type::s_safe_invoke(
				boost::bind(&this_type::prv_run_it_wrap, this, boost::ref(runner), boost::ref(task))))
		{
#	if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
			const real_data_type* pdata = getter.operator()<real_data_type>(task);
#	else
			const real_data_type* pdata = getter.template operator()<real_data_type>(task);
#	endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
				
			(_seh_handler)(task.owner_info(), pdata);
		}
	}

private:
	inline void prv_run_it_wrap(this_work_runner_type& runner, this_task_type& task) const
	{
		task_real_data_getter_type getter;
#	if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		real_data_type* pdata = getter.operator()<real_data_type>(task);
#	else
		real_data_type* pdata = getter.template operator()<real_data_type>(task);
#	endif // _MSC_VER
		if(pdata)
		{
			(_call_handler)(
				task.owner_info(), boost::ref(*pdata), boost::addressof(runner), _back_handler);
		}
	}

private:
	call_handler_type _call_handler;
	back_handler_type _back_handler;
	seh_handler_type _seh_handler;
};

#else //non seh_ver

template<typename Task, typename WorkRunner,
			typename RealData, typename CallHandler, typename BackHandler>
class action_table_calculator<Task, WorkRunner, RealData, CallHandler, BackHandler, int>
	: public action_table_base_calculator<Task, WorkRunner>
{
public:
	typedef Task this_task_type;
	typedef WorkRunner this_work_runner_type;
	typedef action_table_base_calculator<this_task_type, this_work_runner_type> base_type;

private:
	typedef RealData real_data_type;
	typedef CallHandler call_handler_type;
	typedef BackHandler back_handler_type;

private:
	typedef task_center::support::task_real_data_getter<this_task_type> task_real_data_getter_type;

private:
	typedef action_table_calculator this_type;

public:
	action_table_calculator(const call_handler_type& call_handler,
							const back_handler_type& back_handler)
		: _call_handler(call_handler),
			_back_handler(back_handler)
	{
	}

	~action_table_calculator(void)
	{
	}

public:
	virtual void run_handler(this_work_runner_type& runner, this_task_type& task) const
	{
		task_real_data_getter_type getter;

#	if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		real_data_type* pdata = getter.operator()<real_data_type>(task);
#	else
		real_data_type* pdata = getter.template operator()<real_data_type>(task);
#	endif // _MSC_VER
		if(pdata)
		{
			(_call_handler)(
				task.owner_info(), boost::ref(*pdata), boost::addressof(runner), _back_handler);
		}
	}

private:
	call_handler_type _call_handler;
	back_handler_type _back_handler;
};

#endif // YGGR_USE_SEH


} // namespace detail
} // namespace thread
} // namespace yggr

namespace yggr
{
namespace thread
{

template<typename Task_Type,
			typename Work_Runner,
			typename ID_Parser,
			typename Mutex = void>
class action_table
{
public:
	typedef Task_Type task_type;
	typedef task_center::support::task_real_data_getter<task_type> task_real_data_getter_type;
	typedef Work_Runner work_runner_type;
	typedef ID_Parser id_parser_type;
	typedef Mutex tpl_arg_mutex_type;

private:
	typedef detail::action_table_base_calculator<task_type, work_runner_type> base_calculate_type;
	typedef shared_ptr<base_calculate_type> calculator_contaniner_type;

	typedef typename id_parser_type::id_type calculator_id_type;

	typedef 
		safe_container::safe_unordered_map
		<
			calculator_id_type,
			calculator_contaniner_type,
			tpl_arg_mutex_type
		> reg_map_type;

	//-------------------------------------------------------------------------------------------------------------
public:

	action_table(void)
	{
	}

	~action_table(void)
	{
	}

public:
	template<typename Real_Data_Type> inline
	bool unregister_calculator(void)
	{
		typedef Real_Data_Type now_real_data_type;
		return _reg_map.erase(id_parser_type::template get_reg_id<now_real_data_type>());
	}

	inline bool unregister_calculator(const calculator_id_type& id)
	{
		return _reg_map.erase(id);
	}

	template<typename Real_Data_Type, typename Recv_Handler, typename Handler> inline
	bool register_calculator(const Recv_Handler& recv_handler, const Handler& handler)
	{
		typedef Real_Data_Type now_real_data_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;

		typedef 
			detail::action_table_calculator
			<
				task_type, 
				work_runner_type,
				now_real_data_type,
				call_handler_type,
				back_handler_type,
				int
			> now_calculator_type;
		
		calculator_contaniner_type pcal = 
			construct_shared<base_calculate_type>(
				yggr_nothrow_new now_calculator_type(handler, recv_handler));

		return pcal && _reg_map.insert(id_parser_type::template get_reg_id<now_real_data_type>(), pcal);
	}

	// fix proxy protocol
	template<typename Real_Data_Type, typename Container, typename Recv_Handler, typename Handler> inline
	bool register_calculator_of_container(const Recv_Handler& recv_handler, const Handler& handler)
	{
		typedef Real_Data_Type now_real_data_type;
		typedef Container now_container_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;

		typedef 
			detail::action_table_calculator
			<
				task_type, 
				work_runner_type,
				now_container_type,
				call_handler_type,
				back_handler_type,
				int
			> now_calculator_type;

		calculator_contaniner_type pcal = 
			construct_shared<base_calculate_type>(
				yggr_nothrow_new now_calculator_type(handler, recv_handler) );

		return pcal && _reg_map.insert(id_parser_type::template get_reg_id<now_real_data_type>(), pcal);
	}

	template<typename Container, typename Recv_Handler, typename Handler> inline
	bool register_calculator(const calculator_id_type& id, const Recv_Handler& recv_handler, const Handler& handler)
	{
		typedef Container now_container_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;

		typedef 
			detail::action_table_calculator
			<
				task_type, 
				work_runner_type,
				now_container_type,
				call_handler_type,
				back_handler_type,
				int
			> now_calculator_type;

		calculator_contaniner_type pcal = 
			construct_shared<base_calculate_type>(
				yggr_nothrow_new now_calculator_type(handler, recv_handler) );

		return pcal && _reg_map.insert(id, pcal);

	}

#ifdef YGGR_USE_SEH
	template<typename Real_Data_Type,
				typename Recv_Handler, typename Handler, typename SEH_Handler> inline
	bool register_calculator(const Recv_Handler& recv_handler,
								const Handler& handler,
								const SEH_Handler& seh_handler)
	{

		typedef Real_Data_Type now_real_data_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;
		typedef SEH_Handler seh_handler_type;

		typedef 
			detail::action_table_calculator
			<
				task_type, 
				work_runner_type,
				now_real_data_type,
				call_handler_type,
				back_handler_type,
				seh_handler_type
			> now_calculator_type;

		calculator_contaniner_type pcal = 
			construct_shared<base_calculate_type>(
				yggr_nothrow_new now_calculator_type(handler, recv_handler, seh_handler) );

		return pcal && _reg_map.insert(id_parser_type::template get_reg_id<now_real_data_type>(), pcal);
	}

	// fix proxy protocol
	template<typename Real_Data_Type, typename Container,
				typename Recv_Handler, typename Handler, typename SEH_Handler> inline
	bool register_calculator_of_container(const Recv_Handler& recv_handler,
											const Handler& handler,
											const SEH_Handler& seh_handler)
	{
		typedef Real_Data_Type now_real_data_type;
		typedef Container now_container_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;
		typedef SEH_Handler seh_handler_type;

		typedef 
			detail::action_table_calculator
			<
				task_type, 
				work_runner_type,
				now_container_type,
				call_handler_type,
				back_handler_type,
				seh_handler_type
			> now_calculator_type;

		calculator_contaniner_type pcal = 
			construct_shared<base_calculate_type>(
				yggr_nothrow_new now_calculator_type(handler, recv_handler, seh_handler) );

		return pcal && _reg_map.insert(id_parser_type::template get_reg_id<now_real_data_type>(), pcal);
	}

	template<typename Container, typename Recv_Handler,
				typename Handler, typename SEH_Handler> inline
	bool register_calculator(const calculator_id_type& id,
								const Recv_Handler& recv_handler,
								const Handler& handler,
								const SEH_Handler& seh_handler)
	{
		typedef Container now_container_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;
		typedef SEH_Handler seh_handler_type;

		typedef 
			detail::action_table_calculator
			<
				task_type, 
				work_runner_type,
				now_container_type,
				call_handler_type,
				back_handler_type,
				seh_handler_type
			> now_calculator_type;

		calculator_contaniner_type pcal = 
			construct_shared<base_calculate_type>(
				yggr_nothrow_new now_calculator_type(handler, recv_handler, seh_handler) );

		return pcal && _reg_map.insert(id, pcal);
	}

#endif // YGGR_USE_SEH

	inline void run_task(work_runner_type& runner, task_type& task) const
	{
		calculator_id_type key(id_parser_type::template get_task_id<task_type>(task));
		calculator_contaniner_type pcont;
		
		bool bget = _reg_map.get_value(key, pcont);

		if(bget && pcont)
		{
			pcont->run_handler(runner, task);
		}
	}

	inline void clear(void)
	{
		_reg_map.clear();
	}

private:
	reg_map_type _reg_map;
};

} // namespace thread
} // namespace yggr

#endif //__YGGR_THREAD_ACTION_TABLE_HPP__
