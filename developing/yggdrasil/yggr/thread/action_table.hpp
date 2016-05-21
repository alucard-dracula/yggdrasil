//action_table.hpp

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

#ifndef __YGGR_THREAD_ACTION_TABLE_HPP__
#define __YGGR_THREAD_ACTION_TABLE_HPP__

#include <algorithm>
#include <cassert>

#include <boost/tuple/tuple.hpp>

#include <yggr/base/interface_ptr.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/task_center/support/task_real_data_getter.hpp>

#include <yggr/thread/action_helper/action_id_parser.hpp>
#include <yggr/thread/action_helper/make_reg_id_parser.hpp>

#ifdef YGGR_USE_SEH
#	include <yggr/seh/seh.hpp>
#endif // YGGR_USE_SEH

namespace yggr
{
namespace thread
{

template<typename Task_Type,
			typename Work_Runner,
			typename ID_Parser,
			typename Mutex = boost::mutex>
class action_table
{
	typedef Task_Type task_type;
	typedef task_center::support::task_real_data_getter<task_type> task_real_data_getter_type;
	typedef Work_Runner work_runner_type;

private:
	typedef Mutex mutex_type;
	//typedef ID_Parser id_parser_type;
	//typedef typename task_type::class_name_getter class_name_getter_type;
	typedef ID_Parser id_parser_type;

	//-------------------------------------------------------------------------
	template<typename _Task_Type, typename _Work_Runner>
	class base_calculator
	{
	public:
		typedef _Task_Type this_task_type;
		typedef _Work_Runner this_work_runner_type;
	public:
		virtual ~base_calculator(void)
		{
		}

		virtual void run_handler(this_work_runner_type&, const this_task_type&) const = 0;
	};

	typedef base_calculator<task_type, work_runner_type> base_calculate_type;


	typedef interface_ptr<base_calculate_type> calculator_contaniner_type;

	//template<typename Real_Data_Type, typename Tuple_Handler, u32 tuple_len>
	//class calculator;

	template<typename Real_Data_Type, typename Call_Handler, typename Back_Handler, typename SEH_Handler = int>
	class calculator;

#ifdef YGGR_USE_SEH //seh ver
	template<typename Real_Data_Type, typename Call_Handler, typename Back_Handler>
	class calculator<Real_Data_Type, Call_Handler, Back_Handler, int>
		: public base_calculate_type
	{
		typedef base_calculate_type base_type;
		typedef typename base_type::this_task_type this_task_type;
		typedef Real_Data_Type real_data_type;
		typedef Call_Handler call_handler_type;
		typedef Back_Handler back_handler_type;
		//typedef Tuple_Handler handler_type;
		typedef calculator this_type;
	public:
		calculator(const call_handler_type& call_handler, const back_handler_type& back_handler)
			: _call_handler(call_handler), _back_handler(back_handler)
		{
		}

		virtual ~calculator(void)
		{
		}

		virtual void run_handler(typename base_type::this_work_runner_type& runner, const this_task_type& task) const
		{
			seh::seh_type::s_safe_invoke(boost::bind(&this_type::run_it_wrap, this, boost::ref(runner), boost::cref(task)));
		}

	private:
		void run_it_wrap(typename base_type::this_work_runner_type& runner, const this_task_type& task) const
		{
			task_real_data_getter_type getter;
#		ifdef _MSC_VER
			const real_data_type* pdata = getter.operator()<real_data_type>(task);
#		else
			const real_data_type* pdata = getter.template operator()<real_data_type>(task);
#		endif // _MSC_VER

			if(!pdata)
			{
				return;
			}
			_call_handler(task.owner_info(), *pdata, &runner, _back_handler);
		}

	private:
		call_handler_type _call_handler;
		back_handler_type _back_handler;
	};

	template<typename Real_Data_Type, typename Call_Handler, typename Back_Handler, typename SEH_Handler>
	class calculator
		: public base_calculate_type
	{
		typedef base_calculate_type base_type;
		typedef typename base_type::this_task_type this_task_type;
		typedef Real_Data_Type real_data_type;
		typedef Call_Handler call_handler_type;
		typedef Back_Handler back_handler_type;
		typedef SEH_Handler seh_handler_type;
		//typedef Tuple_Handler handler_type;
		typedef calculator this_type;
	public:
		calculator(const call_handler_type& call_handler,
					const back_handler_type& back_handler,
					const seh_handler_type& seh_handler)
			: _call_handler(call_handler),
				_back_handler(back_handler),
				_seh_handler(seh_handler)
		{
		}

		virtual ~calculator(void)
		{
		}

		virtual void run_handler(typename base_type::this_work_runner_type& runner, const this_task_type& task) const
		{
			task_real_data_getter_type getter;
			if(!seh::seh_type::s_safe_invoke(boost::bind(&this_type::run_it_wrap, this, boost::ref(runner), boost::cref(task))))
			{
#			ifdef _MSC_VER
				const real_data_type* pdata = getter.operator()<real_data_type>(task);
#			else
				const real_data_type* pdata = getter.template operator()<real_data_type>(task);
#			endif // _MSC_VER
				
				_seh_handler(task.owner_info(), pdata);
			}
		}

	private:
		void run_it_wrap(typename base_type::this_work_runner_type& runner, const this_task_type& task) const
		{
			task_real_data_getter_type getter;
#		ifdef _MSC_VER
				const real_data_type* pdata = getter.operator()<real_data_type>(task);
#		else
				const real_data_type* pdata = getter.template operator()<real_data_type>(task);
#		endif // _MSC_VER
			if(!pdata)
			{
				return;
			}
			_call_handler(task.owner_info(), *pdata, &runner, _back_handler);
		}

	private:
		call_handler_type _call_handler;
		back_handler_type _back_handler;
		seh_handler_type _seh_handler;
	};
#else //non seh_ver
	template<typename Real_Data_Type, typename Call_Handler, typename Back_Handler>
	class calculator<Real_Data_Type, Call_Handler, Back_Handler, int>
		: public base_calculate_type
	{
		typedef base_calculate_type base_type;
		typedef typename base_type::this_task_type this_task_type;
		typedef Real_Data_Type real_data_type;
		//typedef Tuple_Handler handler_type;
		typedef Call_Handler call_handler_type;
		typedef Back_Handler back_handler_type;

	private:
		typedef calculator this_type;

	public:
		calculator(const call_handler_type& call_handler,
					const back_handler_type& back_handler)
			: _call_handler(call_handler), _back_handler(back_handler)
		{
		}

		virtual ~calculator(void)
		{
		}

		virtual void run_handler(typename base_type::this_work_runner_type& runner, const this_task_type& task) const
		{
			task_real_data_getter_type getter;

#ifdef _MSC_VER
			const real_data_type* pdata = getter.operator()<real_data_type>(task);
#else
			const real_data_type* pdata = getter.template operator()<real_data_type>(task);
#endif // _MSC_VER
			if(!pdata)
			{
				return;
			}
			_call_handler(task.owner_info(), *pdata, &runner,_back_handler);
		}

	private:
		call_handler_type _call_handler;
		back_handler_type _back_handler;
	};
#endif // YGGR_USE_SEH

	typedef typename id_parser_type::id_type calculator_id_type;

	typedef yggr::safe_container::safe_unordered_map<calculator_id_type,
														calculator_contaniner_type,
														mutex_type> reg_map_type;

	//-------------------------------------------------------------------------------------------------------------
public:

	action_table(void)
	{
	}

	~action_table(void)
	{
	}

	template<typename Real_Data_Type>
	bool unregister_calculator(void)
	{
		typedef Real_Data_Type now_real_data_type;
		return _reg_map.erase(id_parser_type::template get_reg_id<now_real_data_type>());
	}

	bool unregister_calculator(const calculator_id_type& id)
	{
		return _reg_map.erase(id);
	}

	template<typename Real_Data_Type,
				typename Recv_Handler, typename Handler>
	bool register_calculator(const Recv_Handler& recv_handler, const Handler& handler)
	{
		typedef Real_Data_Type now_real_data_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;

		typedef calculator<now_real_data_type,
							call_handler_type,
							back_handler_type,
							int> now_calculator_type;

		//class_name_getter_type name_getter;
		return _reg_map.insert(id_parser_type::template get_reg_id<now_real_data_type>(),
								calculator_contaniner_type(new now_calculator_type(handler, recv_handler)));


	}

	template<typename Real_Data_Type, typename Container,
				typename Recv_Handler, typename Handler>
	bool register_calculator_of_container(const Recv_Handler& recv_handler, const Handler& handler)
	{
		typedef Real_Data_Type now_real_data_type;
		typedef Container now_container_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;

		typedef calculator<now_container_type,
							call_handler_type,
							back_handler_type,
							int> now_calculator_type;

		return _reg_map.insert(id_parser_type::template get_reg_id<now_real_data_type>(),
								calculator_contaniner_type(new now_calculator_type(handler, recv_handler)));

	}

	template<typename Container,
				typename Recv_Handler, typename Handler>
	bool register_calculator(const calculator_id_type& id,
								const Recv_Handler& recv_handler, const Handler& handler)
	{
		typedef Container now_container_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;
		typedef calculator<now_container_type,
							call_handler_type,
							back_handler_type,
							int> now_calculator_type;

		return _reg_map.insert(id,
								calculator_contaniner_type(new now_calculator_type(handler, recv_handler)));

	}

#ifdef YGGR_USE_SEH
	template<typename Real_Data_Type,
				typename Recv_Handler, typename Handler, typename SEH_Handler>
	bool register_calculator(const Recv_Handler& recv_handler,
								const Handler& handler,
								const SEH_Handler& seh_handler)
	{

		typedef Real_Data_Type now_real_data_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;
		typedef SEH_Handler seh_handler_type;

		typedef calculator<now_real_data_type,
							call_handler_type,
							back_handler_type,
							seh_handler_type> now_calculator_type;

		//class_name_getter_type name_getter;
		return _reg_map.insert(id_parser_type::template get_reg_id<now_real_data_type>(),
								calculator_contaniner_type(new now_calculator_type(handler, recv_handler, seh_handler)));
	}

	template<typename Real_Data_Type, typename Container,
							typename Recv_Handler, typename Handler, typename SEH_Handler>
	bool register_calculator_of_container(const Recv_Handler& recv_handler,
											const Handler& handler,
											const SEH_Handler& seh_handler)
	{
		typedef Real_Data_Type now_real_data_type;
		typedef Container now_container_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;
		typedef SEH_Handler seh_handler_type;

		typedef calculator<now_container_type,
							call_handler_type,
							back_handler_type,
							seh_handler_type> now_calculator_type;

		return _reg_map.insert(id_parser_type::template get_reg_id<now_real_data_type>(),
								calculator_contaniner_type(new now_calculator_type(handler, recv_handler, seh_handler)));
	}

	template<typename Container, typename Recv_Handler,
				typename Handler, typename SEH_Handler>
	bool register_calculator(const calculator_id_type& id,
								const Recv_Handler& recv_handler,
								const Handler& handler,
								const SEH_Handler& seh_handler)
	{
		typedef Container now_container_type;
		typedef Handler call_handler_type;
		typedef Recv_Handler back_handler_type;
		typedef SEH_Handler seh_handler_type;
		typedef calculator<now_container_type,
							call_handler_type,
							back_handler_type,
							int> now_calculator_type;

		return _reg_map.insert(id,
								calculator_contaniner_type(new now_calculator_type(handler, recv_handler, seh_handler)));
	}

#endif // YGGR_USE_SEH


	//void run_task(work_runner_type& runner, const task_type& task) const
	//{
	//	//class_name_getter_type name_getter;
	//	calculator_id_type key(id_parser_type::template get_task_id<task_type>(task));
	//	calculator_contaniner_type pcont(
	//								_reg_map.use_handler(
	//											boost::bind(
	//															&action_table::handler_find_calculator,
	//															this, _1, boost::cref(key))));
	//	if(pcont)
	//	{
	//		pcont->run_handler(runner, task);
	//	}
	//}

	void run_task(work_runner_type& runner, const task_type& task) const
	{
		//class_name_getter_type name_getter;
		calculator_id_type key(id_parser_type::template get_task_id<task_type>(task));
#if defined(_MSC_VER) && defined(_DEBUG)
		//{
		//if(!key.empty())
		//{
		//	if(key[0] == 0xdd)
		//	{
		//		assert(false);
		//	}
		//}
		//}
#endif // defined(_MSC_VER) && defined(_DEBUG)
		calculator_contaniner_type pcont;
		
		bool bget = _reg_map.get_value(key, pcont);

		if(bget && pcont)
		{
			pcont->run_handler(runner, task);
		}

#if defined(_MSC_VER) && defined(_DEBUG)
		//{
		//if(!key.empty())
		//{
		//	if(key[0] == 0xdd)
		//	{
		//		assert(false);
		//	}
		//}
		//}
#endif // defined(_MSC_VER) && defined(_DEBUG)
	}

	void clear(void)
	{
		_reg_map.clear();
	}

private:
	//calculator_contaniner_type
	//	handler_find_calculator(const typename reg_map_type::base_type& base,
	//								const calculator_id_type& cmp) const
	//{
	//	typedef typename reg_map_type::const_iterator iter_type;
	//	iter_type iter = base.find(cmp);
	//	if(iter == base.end())
	//	{
	//		return calculator_contaniner_type();
	//	}

	//	return iter->second;
	//}

	reg_map_type _reg_map;
};

} // namespace thread
} // namespace yggr

#endif //__YGGR_THREAD_ACTION_TABLE_HPP__
