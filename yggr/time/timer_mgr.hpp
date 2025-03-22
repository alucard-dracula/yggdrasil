//timer_mgr.hpp

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

#ifndef __YGGR_TIME_TIMER_MGR_HPP__
#define __YGGR_TIME_TIMER_MGR_HPP__

#include <yggr/base/yggrdef.h>

// <yggr/thread/boost_thread_config.hpp> (<yggr/network/socket_conflict_fixer.hpp>)
// must after <boost/thread/mutex.hpp> start boost 106600
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/safe_container/safe_unordered_set.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/time/timer_task_state.hpp>
#include <yggr/time/detail/base_timer_mgr.hpp>

#ifdef YGGR_USE_SEH
#	include <yggr/seh/seh.hpp>
#endif // YGGR_USE_SEH

#include <boost/thread/mutex.hpp>

namespace yggr
{
namespace time
{

template<typename Task_ID,
			typename Thread_Config = yggr::thread::boost_thread_config_type,
			 typename Inner_Process_ID = void, std::size_t nid = 0>
class timer_mgr;

} // namespace time
} // namespace yggr

namespace yggr
{
namespace time
{
namespace detail
{

template<typename TimerMgr>
class timer_mgr_delegate;

} // namespace detail
} // namespace time
} // namespace yggr

namespace yggr
{
namespace time
{

// ------------------------inner sharedable version-----------------------------
template<typename Task_ID,
			typename Thread_Config,
			typename Inner_Process_ID,
			std::size_t nid>
class timer_mgr
	: public detail::base_timer_mgr<Task_ID, Thread_Config>
{
public:
	typedef detail::base_timer_mgr<Task_ID, Thread_Config> base_type;

public:
	typedef Inner_Process_ID inner_process_id_type;
	typedef timer_task_state task_state_type;

	typedef typename base_type::task_id_type task_id_type;
	typedef typename base_type::time_type time_type;
	typedef typename base_type::base_timer_task_type base_timer_task_type;

private:
	typedef typename base_type::i_base_timer_task_type i_base_timer_task_type;

	typedef typename base_type::task_id_container_type task_id_container_type;
	typedef typename base_type::task_id_list_type task_id_list_type;
	typedef typename base_type::timer_task_id_queue_type timer_task_id_queue_type;

	typedef typename base_type::timer_task_container_type timer_task_container_type;
	typedef typename base_type::timer_task_list_type timer_task_list_type;
	typedef typename base_type::timer_task_queue_type timer_task_queue_type;

	typedef typename base_type::timer_task_map_type timer_task_map_type;

	typedef typename base_type::this_thread_type this_thread_type;

	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef timer_mgr this_type;

private:
	template<typename TimerMgr>
	friend class detail::timer_mgr_delegate;

//	// don't move to outside
//	class timer_mgr_delegate;
//	YGGR_PP_FRIEND_CLASS(timer_mgr_delegate);
//
//	class timer_mgr_delegate
//	{
//	private:
//		typedef timer_mgr outside_type;
//		YGGR_PP_FRIEND_TYPENAME(outside_type);
//
//		typedef safe_container::safe_unordered_set<task_id_type> task_id_set_type;
//		typedef safe_container::safe_wrap<inner_process_id_type> inner_process_id_wrap_type;
//		typedef typename task_id_set_type::iterator task_id_set_iter_type;
//		typedef typename task_id_set_type::const_iterator task_id_set_citer_type;
//
//	private:
//		typedef timer_mgr_delegate this_type;
//
//	public:
//		timer_mgr_delegate(outside_type& outside, const inner_process_id_type& pid)
//			: _outside(outside), _pid(pid)
//		{
//		}
//
//		~timer_mgr_delegate(void)
//		{
//		}
//
//	public:
//
//#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
//
//		template< typename Timer_Task_Pak, typename ...Args>  inline
//		task_id_type add_task(BOOST_FWD_REF(Args)... args)
//		{
//			typedef Timer_Task_Pak task_pak_type;
//			typedef detail::timer_task<task_id_type, task_pak_type> timer_task_type;
//
//			i_base_timer_task_type ptr =
//				construct_shared<base_timer_task_type>(
//					yggr_nothrow_new timer_task_type(outside_type::gen_id(), boost::forward<Args>(args)...) );
//
//			const i_base_timer_task_type& ptr_cref = ptr;
//			return
//				(ptr_cref
//					&& _task_queue.insert(ptr_cref)
//					&& _ids_set.insert(ptr_cref->id()))?
//				ptr_cref->id() : task_id_type();
//		}
//
//#else
//
//#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
//		template< typename Timer_Task_Pak \
//					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
//					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
//		task_id_type add_task(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
//			typedef Timer_Task_Pak task_pak_type; \
//			typedef detail::timer_task<task_id_type, task_pak_type> timer_task_type; \
//			i_base_timer_task_type ptr = \
//				construct_shared<base_timer_task_type>( \
//					yggr_nothrow_new timer_task_type( \
//						outside_type::gen_id() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
//						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA) )  ); \
//			const i_base_timer_task_type& ptr_cref = ptr; \
//			return (ptr_cref && _task_queue.insert(ptr_cref) && _ids_set.insert(ptr_cref->id()))? \
//						ptr_cref->id() : task_id_type(); }
//
//#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
//#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
//#	include BOOST_PP_LOCAL_ITERATE(  )
//#	undef YGGR_PP_FOO_ARG_NAME
//
//#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES
//
//		template<typename Timer_Task_Pak> inline
//		task_id_type add_task(BOOST_RV_REF(Timer_Task_Pak) pak)
//		{
//			typedef Timer_Task_Pak task_pak_type;
//			typedef timer_task<task_pak_type> timer_task_type;
//
//			i_base_timer_task_type ptr =
//				construct_shared<base_timer_task_type>(
//					yggr_nothrow_new timer_task_type(outside_type::gen_id(), boost::move(pak)) );
//
//			const i_base_timer_task_type& ptr_cref = ptr;
//			return (ptr_cref && _task_queue.insert(ptr_cref) && _ids_set.insert(ptr_cref->id()))?
//					ptr_cref->id() : task_id_type();
//		}
//
//		template<typename Timer_Task_Pak> inline
//		task_id_type add_task(const Timer_Task_Pak& pak)
//		{
//			typedef Timer_Task_Pak task_pak_type;
//			typedef timer_task<task_pak_type> timer_task_type;
//
//			i_base_timer_task_type ptr =
//				construct_shared<base_timer_task_type>(
//					yggr_nothrow_new timer_task_type(outside_type::gen_id(), pak) );
//
//			const i_base_timer_task_type& ptr_cref = ptr;
//			return (ptr_cref && _task_queue.insert(ptr_cref) && _ids_set.insert(ptr_cref->id()))?
//					ptr_cref->id() : task_id_type();
//		}
//
//		inline void remove_task(const task_id_type& id)
//		{
//			_rm_task_id_queue.insert(id);
//		}
//
//		inline void exit_task(const task_id_type& id)
//		{
//			_task_queue.remove(boost::bind(&outside_type::base_type::handler_equal_id_of_task,
//												_1, boost::cref(id)));
//			_rm_task_id_queue.remove(boost::bind(&outside_type::base_type::handler_equal_id_of_task_id,
//													_1, boost::cref(id)));
//			_outside.exit_task(id);
//		}
//
//		void clear(void)
//		{
//			typedef typename task_id_set_type::base_type base_task_id_set_type;
//
//			_outside.exit_delegate(_pid.load());
//
//			_task_queue.clear();
//			_rm_task_id_queue.clear();
//
//			base_task_id_set_type tmp_set;
//			_ids_set.swap(tmp_set);
//
//			for(task_id_set_iter_type i = tmp_set.begin(), isize = tmp_set.end(); i != isize; ++i)
//			{
//				_outside.exit_task(*i);
//			}
//		}
//
//		template<typename Handler> inline
//		void get_task(const basic_args_holder_nv_type& any_handler)
//		{
//			typedef Handler handler_type;
//
//			_task_queue.get(
//				boost::bind(
//					&timer_task_container_type::template get_of_any_handler<handler_type>,
//					_1, boost::cref(any_handler)));
//		}
//
//		template<typename Handler> inline
//		void get_task_ids(const basic_args_holder_nv_type& any_handler)
//		{
//			typedef Handler handler_type;
//
//			_rm_task_id_queue.get(
//				boost::bind(
//					&task_id_container_type::template get_of_any_handler<handler_type>,
//					_1, boost::cref(any_handler)));
//		}
//
//	private:
//		outside_type& _outside;
//		inner_process_id_wrap_type _pid;
//		timer_task_queue_type _task_queue;
//		timer_task_id_queue_type _rm_task_id_queue;
//		task_id_set_type _ids_set;
//	};

public:
	typedef detail::timer_mgr_delegate<this_type> timer_mgr_delegate_type;
	typedef ::yggr::shared_ptr<timer_mgr_delegate_type> timer_mgr_delegate_ptr_type;
	typedef timer_mgr_delegate_type delegate_type;
	typedef timer_mgr_delegate_ptr_type delegate_ptr_type;

private:
	typedef
		safe_container::safe_unordered_map
		<
			inner_process_id_type,
			timer_mgr_delegate_ptr_type
		> delegate_map_type;

public:
	timer_mgr(void)
	{
	}

	timer_mgr(const time_type& tm_step)
		: base_type(tm_step)
	{
	}

	~timer_mgr(void)
	{
		// if stop here, don't forgot call delegate.clear in modules exit
		assert(base_type::_task_map.empty());
	}

public:
	inline void start(void)
	{
		bool brun_old = false;
		bool brun_new = true;

		if(!base_type::_brun.compare_exchange_strong(brun_old, brun_new))
		{
			return;
		}

		if(!(base_type::_trd_group.create_thread(boost::bind(&this_type::run, this))))
		{
			base_type::_brun.store(false);
		}
	}

	inline timer_mgr_delegate_ptr_type get_delegate(const inner_process_id_type& id)
	{
		return
			_delegate_map.find(
				id,
				boost::bind(
					&this_type::handler_get_delegate,
					this, _1, _2, boost::cref(id)));
	}

private:

	inline timer_mgr_delegate_ptr_type handler_get_delegate(typename delegate_map_type::base_type& base,
																typename delegate_map_type::iterator rst,
																const inner_process_id_type& id)
	{
		if(rst == base.end())
		{
			timer_mgr_delegate_ptr_type ptr(yggr_nothrow_new timer_mgr_delegate_type(*this, id));
			try
			{
				base[id] = ptr;
			}
			catch(const ::yggr::stl_exception&)
			{
				return timer_mgr_delegate_ptr_type();
			}
			return ptr;

		}
		else
		{
			return rst->second;
		}
	}


	inline void get_timer_objects(timer_task_list_type& task_list, task_id_list_type& id_list)
	{
		typedef typename delegate_map_type::base_type map_type;

		map_type tmp = _delegate_map.load();

		this_type::get_timer_objects_helper(
			tmp,
			boost::bind(
				&this_type::handler_get_timer_object_list<timer_task_list_type>,
				_1, boost::ref(task_list)),
			boost::bind(
				&this_type::handler_get_timer_object_list<task_id_list_type>,
				_1, boost::ref(id_list)) );
	}

	template<typename List> inline
	static void handler_get_timer_object_list(List& list, List& out_list)
	{
		out_list.splice(out_list.end(), list);
	}

	template<typename Task_Handler, typename ID_Handler>
	void get_timer_objects_helper(typename delegate_map_type::base_type& map,
									const Task_Handler& task_handler,
									const ID_Handler& id_handler)
	{
		typedef typename delegate_map_type::iterator map_iter_type;
		typedef Task_Handler task_handler_type;
		typedef ID_Handler id_handler_type;

		typedef utility::args_holder_nv_ref_wrap<task_handler_type const> task_handler_holder_type;
		typedef utility::args_holder_nv_ref_wrap<id_handler_type const> id_handler_holder_type;

		task_handler_holder_type any_task_handler(task_handler);
		id_handler_holder_type any_id_handler(id_handler);

		const basic_args_holder_nv_type& base_any_task_handler = utility::args_holder_trans(any_task_handler);
		const basic_args_holder_nv_type& base_any_id_handler = utility::args_holder_trans(any_id_handler);

		for(map_iter_type i = map.begin(), isize = map.end(); i != isize; ++i)
		{
			if(i->second)
			{
				(i->second)->template get_task<task_handler_type>(base_any_task_handler);
				(i->second)->template get_task_ids<id_handler_type>(base_any_id_handler);
			}
		}
	}

	inline void exit_delegate(const inner_process_id_type& pid)
	{
		_delegate_map.erase(pid);
	}

	inline void exit_task(const task_id_type& id)
	{
		base_type::_task_map.erase(id);
	}

	void adjust_task_map(void)
	{
		typedef typename timer_task_list_type::iterator task_iter_type;
		typedef typename task_id_list_type::iterator id_iter_type;

		timer_task_list_type task_list;
		task_id_list_type id_list;

		get_timer_objects(task_list, id_list);

		{
			for(task_iter_type i = task_list.begin(), isize = task_list.end(); i != isize; ++i)
			{
				base_type::pro_add_task(*i);
			}
		}

		{
			for(id_iter_type i = id_list.begin(), isize = id_list.end(); i != isize; ++i)
			{
				base_type::pro_remove_task(*i);
			}
		}

	}

	size_type run_tasks(typename timer_task_map_type::base_type& task_map)
	{
		typedef typename timer_task_map_type::iterator map_iter_type;
		typedef ::yggr::list<task_id_type> task_id_list_type;
		typedef typename task_id_list_type::const_iterator list_iter_type;

		size_type run_size = 0;
		task_id_list_type id_list;
		u32 sat = 0;

		{
			for(map_iter_type i = task_map.begin(), isize = task_map.end(); base_type::_brun.load() && i != isize; ++i)
			{
				if(!(i->second))
				{
					id_list.push_back(i->first);
				}
				else
				{
					sat = i->second->do_timer();

					switch(sat)
					{
					case task_state_type::E_timer_task_ran:
						++run_size;
						break;
					case task_state_type::E_timer_task_not_ran:
						break;
					case task_state_type::E_timer_task_remove:
						id_list.push_back(i->first);
						break;
					default:
						assert(false);
						id_list.push_back(i->first);
					}
				}
			}
		}

		{
			for(list_iter_type i = id_list.begin(), isize = id_list.end(); i != isize; ++i)
			{
				base_type::pro_remove_task(*i);
			}
		}

		return run_size;
	}

#if defined(YGGR_USE_SEH) && !defined(YGGR_TIMER_TASK_NOT_USE_SEH)
	inline size_type safe_run_tasks(typename timer_task_map_type::base_type& task_map)
	{
		size_type ret = 0;
		return
			yggr::seh::seh_type::s_safe_invoke(
				ret,
				boost::bind(&this_type::run_tasks, this, boost::ref(task_map)))?
			ret : 0;
	}
#endif // defined(YGGR_USE_SEH) && !defined(YGGR_TIMER_TASK_NOT_USE_SEH)

	void run(void)
	{
		typedef typename timer_task_map_type::base_type base_map_type;

		base_map_type timer_task_map;
		size_type run_size = 0;
		for(;base_type::_brun.load();)
		{
			adjust_task_map();

			timer_task_map = base_type::_task_map.load();

#if defined(YGGR_USE_SEH) && !defined(YGGR_TIMER_TASK_NOT_USE_SEH)
			run_size = timer_task_map.empty()? 0 : safe_run_tasks(timer_task_map);
#else
			run_size = timer_task_map.empty()? 0 : run_tasks(timer_task_map);
#endif // defined(YGGR_USE_SEH) && !defined(YGGR_TIMER_TASK_NOT_USE_SEH)

			timer_task_map.clear();

			if(!run_size)
			{
				this_thread_type::yield();
				this_thread_type::sleep(base_type::_tm_step.load());
			}
		}
	}

private:
	delegate_map_type _delegate_map;
};

// ------------------------------------------normal version-------------------------------------------
template<typename Task_ID,
			typename Thread_Config,
			std::size_t nid>
class timer_mgr<Task_ID, Thread_Config, void, nid>
	: public detail::base_timer_mgr<Task_ID, Thread_Config>
{

public:
	typedef detail::base_timer_mgr<Task_ID, Thread_Config> base_type;

public:
	typedef typename base_type::task_id_type task_id_type;
	typedef typename base_type::time_type time_type;
	typedef timer_task_state task_state_type;

private:
	typedef timer_mgr this_type;

private:
	typedef typename base_type::i_base_timer_task_type i_base_timer_task_type;

	typedef typename base_type::task_id_container_type task_id_container_type;
	typedef typename base_type::task_id_list_type task_id_list_type;
	typedef typename base_type::timer_task_id_queue_type timer_task_id_queue_type;

	typedef typename base_type::timer_task_container_type timer_task_container_type;
	typedef typename base_type::timer_task_list_type timer_task_list_type;
	typedef typename base_type::timer_task_queue_type timer_task_queue_type;

	typedef typename base_type::timer_task_map_type timer_task_map_type;
	typedef typename base_type::base_timer_task_type base_timer_task_type;

	typedef typename base_type::this_thread_type this_thread_type;

public:
	timer_mgr(void)
	{
	}

	timer_mgr(const time_type& tm_step)
		: base_type(tm_step)
	{
	}

	~timer_mgr(void)
	{
	}

public:
	using base_type::is_running;
	using base_type::join;
	using base_type::stop;

protected:
	using base_type::gen_id;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Timer_Task_Pak, typename ...Args> inline
	task_id_type add_task(BOOST_FWD_REF(Args)... args)
	{
		typedef Timer_Task_Pak task_pak_type;
		typedef detail::timer_task<task_id_type, task_pak_type> timer_task_type;

		i_base_timer_task_type ptr =
			construct_shared<base_timer_task_type>(
				yggr_nothrow_new timer_task_type(base_type::gen_id(), boost::forward<Args>(args)...) );

		const i_base_timer_task_type& ptr_cref = ptr;
		return ptr_cref && _task_queue.insert(ptr_cref)? ptr_cref->id() : task_id_type();
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Timer_Task_Pak \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		task_id_type add_task(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef Timer_Task_Pak task_pak_type; \
			typedef detail::timer_task<task_id_type, task_pak_type> timer_task_type; \
			i_base_timer_task_type ptr = \
				construct_shared<base_timer_task_type>( \
					yggr_nothrow_new timer_task_type( \
						base_type::gen_id() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)) ); \
			const i_base_timer_task_type& ptr_cref = ptr; \
			return ptr_cref && _task_queue.insert(ptr_cref)? ptr_cref->id() : task_id_type(); }

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Timer_Task_Pak> inline
	task_id_type add_task(BOOST_RV_REF(Timer_Task_Pak) pak)
	{
		typedef Timer_Task_Pak task_pak_type;
		typedef detail::timer_task<task_id_type, task_pak_type> timer_task_type;

		i_base_timer_task_type ptr =
			construct_shared<base_timer_task_type>(
				yggr_nothrow_new timer_task_type(base_type::gen_id(), boost::move(pak)) );

		const i_base_timer_task_type& ptr_cref = ptr;
		return ptr_cref && _task_queue.insert(ptr_cref)? ptr_cref->id() : task_id_type();
	}

	template<typename Timer_Task_Pak> inline
	task_id_type add_task(const Timer_Task_Pak& pak)
	{
		typedef Timer_Task_Pak task_pak_type;
		typedef detail::timer_task<task_id_type, task_pak_type> timer_task_type;

		i_base_timer_task_type ptr =
			construct_shared<base_timer_task_type>(
				yggr_nothrow_new timer_task_type(base_type::gen_id(), pak) );

		const i_base_timer_task_type& ptr_cref = ptr;
		return ptr_cref && _task_queue.insert(ptr_cref)? ptr_cref->id() : task_id_type();
	}

	inline void remove_task(const task_id_type& id)
	{
		_rm_task_id_queue.insert(id);
	}

	inline void exit_task(const task_id_type& id)
	{
		_task_queue.remove(
			boost::bind(
				&base_type::handler_equal_id_of_task,
				_1, boost::cref(id)));

		_rm_task_id_queue.remove(
			boost::bind(
				&base_type::handler_equal_id_of_task_id,
				_1, boost::cref(id)));

		base_type::_task_map.erase(id);
	}

	inline void clear(void)
	{
		_task_queue.clear();
		_rm_task_id_queue.clear();
		base_type::_task_map.clear();
	}

	inline this_type* get_delegate(void)
	{
		return this;
	}

	template<typename T> inline
	this_type* get_delegate(const T&)
	{
		return this;
	}

	inline void start(void)
	{
		bool brun_old = false;
		bool brun_new = true;

		if(!base_type::_brun.compare_exchange_strong(brun_old, brun_new))
		{
			return;
		}

		if(!(base_type::_trd_group.create_thread(boost::bind(&this_type::run, this))))
		{
			base_type::_brun.store(false);
		}
	}

private:
	inline void get_timer_objects(timer_task_list_type& task_list, task_id_list_type& id_list)
	{
		this_type::get_timer_objects_helper(
			boost::bind(
				&this_type::handler_get_timer_object_list<timer_task_list_type>,
				_1, boost::ref(task_list)),
			boost::bind(
				&this_type::handler_get_timer_object_list<task_id_list_type>,
				_1, boost::ref(id_list)));
	}

	template<typename Task_Handler, typename ID_Handler> inline
	void get_timer_objects_helper(const Task_Handler& task_handler, const ID_Handler& id_handler)
	{
		typedef Task_Handler task_handler_type;
		typedef ID_Handler id_handler_type;

		typedef utility::basic_args_holder_nv basic_args_holder_nv_type;
		typedef utility::args_holder_nv_ref_wrap<task_handler_type const> holder_task_handler_type;
		typedef utility::args_holder_nv_ref_wrap<id_handler_type const> holder_id_handler_type;

		holder_task_handler_type holder_task_handler(task_handler);
		holder_id_handler_type holder_id_handler(id_handler);

		_task_queue.get(
			boost::bind(
				&timer_task_container_type::template get_of_any_handler<task_handler_type>,
				_1, boost::cref<basic_args_holder_nv_type>(holder_task_handler)));

		_rm_task_id_queue.get(
			boost::bind(
				&task_id_container_type::template get_of_any_handler<id_handler_type>,
				_1, boost::cref<basic_args_holder_nv_type>(holder_id_handler)));
	}

	template<typename List> inline
	static void handler_get_timer_object_list(List& list, List& out_list)
	{
		out_list.splice(out_list.end(), list);
	}


	void adjust_task_map(void)
	{
		typedef typename timer_task_list_type::iterator task_iter_type;
		typedef typename task_id_list_type::iterator id_iter_type;

		timer_task_list_type task_list;
		task_id_list_type id_list;

		get_timer_objects(task_list, id_list);

		{
			for(task_iter_type i = task_list.begin(), isize = task_list.end(); i != isize; ++i)
			{
				base_type::pro_add_task(*i);
			}
		}

		{
			for(id_iter_type i = id_list.begin(), isize = id_list.end(); i != isize; ++i)
			{
				base_type::pro_remove_task(*i);
			}
		}

	}

	size_type run_tasks(typename timer_task_map_type::base_type& task_map)
	{
		typedef typename timer_task_map_type::iterator map_iter_type;
		typedef ::yggr::list<task_id_type> task_id_list_type;
		typedef typename task_id_list_type::const_iterator list_iter_type;

		size_type run_size = 0;
		task_id_list_type id_list;
		u32 sat = 0;

		{
			for(map_iter_type i = task_map.begin(), isize = task_map.end(); base_type::_brun.load() && i != isize; ++i)
			{
				if(!(i->second))
				{
					id_list.push_back(i->first);
				}
				else
				{
					sat = i->second->do_timer();

					switch(sat)
					{
					case task_state_type::E_timer_task_ran:
						++run_size;
						break;
					case task_state_type::E_timer_task_not_ran:
						break;
					case task_state_type::E_timer_task_remove:
						id_list.push_back(i->first);
						break;
					default:
						assert(false);
						id_list.push_back(i->first);
					}
				}
			}
		}

		{
			for(list_iter_type i = id_list.begin(), isize = id_list.end(); i != isize; ++i)
			{
				base_type::pro_remove_task(*i);
			}
		}

		return run_size;
	}

#if defined(YGGR_USE_SEH) && !defined(YGGR_TIMER_TASK_NOT_USE_SEH)

	inline size_type safe_run_tasks(typename timer_task_map_type::base_type& task_map)
	{
		size_type ret = 0;
		return
			yggr::seh::seh_type::s_safe_invoke(
				ret,
				boost::bind(&this_type::run_tasks, this, boost::ref(task_map)))?
			ret : 0;
	}

#endif // defined(YGGR_USE_SEH) && !defined(YGGR_TIMER_TASK_NOT_USE_SEH)

	void run(void)
	{
		typedef typename timer_task_map_type::base_type base_map_type;

		base_map_type timer_task_map;
		size_type run_size = 0;
		for(;base_type::_brun.load();)
		{
			adjust_task_map();

			timer_task_map = base_type::_task_map.load();

#if defined(YGGR_USE_SEH) && !defined(YGGR_TIMER_TASK_NOT_USE_SEH)
			run_size = timer_task_map.empty()? 0 : safe_run_tasks(timer_task_map);
#else
			run_size = timer_task_map.empty()? 0 : run_tasks(timer_task_map);
#endif // defined(YGGR_USE_SEH) && !defined(YGGR_TIMER_TASK_NOT_USE_SEH)

			timer_task_map.clear();

			if(!run_size)
			{
				this_thread_type::yield();
				this_thread_type::sleep(base_type::_tm_step.load());
			}
		}
	}

private:
	timer_task_queue_type _task_queue;
	timer_task_id_queue_type _rm_task_id_queue;
};

} // namespace time
} // namespace yggr

namespace yggr
{
namespace time
{
namespace detail
{

template<typename TimerMgr>
class timer_mgr_delegate
{
private:
	typedef TimerMgr timer_mgr_type;
	YGGR_PP_FRIEND_TYPENAME(timer_mgr_type);

	typedef typename timer_mgr_type::task_id_type task_id_type;
	typedef typename timer_mgr_type::inner_process_id_type inner_process_id_type;

	typedef typename timer_mgr_type::timer_task_queue_type timer_task_queue_type;
	typedef typename timer_mgr_type::timer_task_id_queue_type timer_task_id_queue_type;

	typedef typename timer_mgr_type::i_base_timer_task_type i_base_timer_task_type;
	typedef typename timer_mgr_type::base_timer_task_type base_timer_task_type;

	typedef typename timer_mgr_type::basic_args_holder_nv_type basic_args_holder_nv_type;

	typedef typename timer_mgr_type::timer_task_container_type timer_task_container_type;
	typedef typename timer_mgr_type::task_id_container_type task_id_container_type;

	typedef safe_container::safe_unordered_set<task_id_type> task_id_set_type;
	typedef safe_container::safe_wrap<inner_process_id_type> inner_process_id_wrap_type;
	typedef typename task_id_set_type::iterator task_id_set_iter_type;
	typedef typename task_id_set_type::const_iterator task_id_set_citer_type;


private:
	typedef timer_mgr_delegate this_type;

public:
	timer_mgr_delegate(timer_mgr_type& timer_mgr, const inner_process_id_type& pid)
		: _tm_mgr(timer_mgr), _pid(pid)
	{
	}

	~timer_mgr_delegate(void)
	{
	}

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename Timer_Task_Pak, typename ...Args>  inline
	task_id_type add_task(BOOST_FWD_REF(Args)... args)
	{
		typedef Timer_Task_Pak task_pak_type;
		typedef detail::timer_task<task_id_type, task_pak_type> timer_task_type;

		i_base_timer_task_type ptr =
			construct_shared<base_timer_task_type>(
				yggr_nothrow_new timer_task_type(timer_mgr_type::gen_id(), boost::forward<Args>(args)...) );

		const i_base_timer_task_type& ptr_cref = ptr;
		return
			(ptr_cref
				&& _task_queue.insert(ptr_cref)
				&& _ids_set.insert(ptr_cref->id()))?
			ptr_cref->id() : task_id_type();
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Timer_Task_Pak \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	task_id_type add_task(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		typedef Timer_Task_Pak task_pak_type; \
		typedef detail::timer_task<task_id_type, task_pak_type> timer_task_type; \
		i_base_timer_task_type ptr = \
			construct_shared<base_timer_task_type>( \
				yggr_nothrow_new timer_task_type( \
					timer_mgr_type::gen_id() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA) )  ); \
		const i_base_timer_task_type& ptr_cref = ptr; \
		return (ptr_cref && _task_queue.insert(ptr_cref) && _ids_set.insert(ptr_cref->id()))? \
					ptr_cref->id() : task_id_type(); }

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Timer_Task_Pak> inline
	task_id_type add_task(BOOST_RV_REF(Timer_Task_Pak) pak)
	{
		typedef Timer_Task_Pak task_pak_type;
		typedef timer_task<task_pak_type, task_pak_type> timer_task_type;

		i_base_timer_task_type ptr =
			construct_shared<base_timer_task_type>(
				yggr_nothrow_new timer_task_type(timer_mgr_type::gen_id(), boost::move(pak)) );

		const i_base_timer_task_type& ptr_cref = ptr;
		return (ptr_cref && _task_queue.insert(ptr_cref) && _ids_set.insert(ptr_cref->id()))?
				ptr_cref->id() : task_id_type();
	}

	template<typename Timer_Task_Pak> inline
	task_id_type add_task(const Timer_Task_Pak& pak)
	{
		typedef Timer_Task_Pak task_pak_type;
		typedef timer_task<task_pak_type, task_pak_type> timer_task_type;

		i_base_timer_task_type ptr =
			construct_shared<base_timer_task_type>(
				yggr_nothrow_new timer_task_type(timer_mgr_type::gen_id(), pak) );

		const i_base_timer_task_type& ptr_cref = ptr;
		return (ptr_cref && _task_queue.insert(ptr_cref) && _ids_set.insert(ptr_cref->id()))?
				ptr_cref->id() : task_id_type();
	}

	inline void remove_task(const task_id_type& id)
	{
		_rm_task_id_queue.insert(id);
	}

	inline void exit_task(const task_id_type& id)
	{
		_task_queue.remove(boost::bind(&timer_mgr_type::base_type::handler_equal_id_of_task,
											_1, boost::cref(id)));
		_rm_task_id_queue.remove(boost::bind(&timer_mgr_type::base_type::handler_equal_id_of_task_id,
												_1, boost::cref(id)));
		_tm_mgr.exit_task(id);
	}

	void clear(void)
	{
		typedef typename task_id_set_type::base_type base_task_id_set_type;

		_tm_mgr.exit_delegate(_pid.load());

		_task_queue.clear();
		_rm_task_id_queue.clear();

		base_task_id_set_type tmp_set;
		_ids_set.swap(tmp_set);

		for(task_id_set_iter_type i = tmp_set.begin(), isize = tmp_set.end(); i != isize; ++i)
		{
			_tm_mgr.exit_task(*i);
		}
	}

	template<typename Handler> inline
	void get_task(const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;

		_task_queue.get(
			boost::bind(
				&timer_task_container_type::template get_of_any_handler<handler_type>,
				_1, boost::cref(any_handler)));
	}

	template<typename Handler> inline
	void get_task_ids(const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;

		_rm_task_id_queue.get(
			boost::bind(
				&task_id_container_type::template get_of_any_handler<handler_type>,
				_1, boost::cref(any_handler)));
	}

private:
	timer_mgr_type& _tm_mgr;
	inner_process_id_wrap_type _pid;
	timer_task_queue_type _task_queue;
	timer_task_id_queue_type _rm_task_id_queue;
	task_id_set_type _ids_set;
};

} // namespace detail
} // namespace time
} // namespace yggr

#endif //__YGGR_TIME_TIMER_MGR_HPP__
