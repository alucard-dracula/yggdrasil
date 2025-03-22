//static_work_runner.hpp

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

#ifndef __YGGR_THREAD_STATIC_WORK_RUNNER_HPP__
#define __YGGR_THREAD_STATIC_WORK_RUNNER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/thread/action_table.hpp>
#include <yggr/thread/type_traits/tags.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <boost/range/value_type.hpp>

#include <cassert>

namespace yggr
{
namespace thread
{
namespace detail
{

template<typename Task>
class static_work_runner_base_link_handler
{
public:
	typedef Task task_type;

private:
	typedef static_work_runner_base_link_handler this_type;

public:
	virtual ~static_work_runner_base_link_handler(void)
	{
	}

public:
	virtual void run_handler(task_type& tk) const = 0;
};

template<typename Task, typename Handler>
class static_work_runner_link_handler 
	: public static_work_runner_base_link_handler<Task>
{
public:
	typedef Task task_type;
	typedef Handler handler_type;
	typedef static_work_runner_base_link_handler<task_type> base_type;

private:
	typedef static_work_runner_link_handler this_type;

public:
	explicit static_work_runner_link_handler(const handler_type& handler)
		: _handler(handler)
	{
	}

	static_work_runner_link_handler(const this_type& right)
		: _handler(right._handler)
	{
	}

	~static_work_runner_link_handler(void)
	{
	}

public:
	virtual void run_handler(task_type& tk) const
	{
		(_handler)(tk);
	}

private:
	handler_type _handler;
};

} // namespace detail
} // namespace thread
} // namespace yggr

namespace yggr
{
namespace thread
{

template<	typename Task,
			typename ID_Parser,
			typename Mutex = void,
			template<	typename _Task,
						typename _WorkRunner,
						typename _ID_Parser,
						typename _Mutex
					>
				class ActionTable = yggr::thread::action_table
		>
class static_work_runner
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Task task_type;
	typedef ID_Parser id_parser_type;

	typedef typename task_type::owner_info_type owner_info_type;
	typedef typename task_type::data_info_type data_info_type;
	typedef typename task_type::data_container_type task_data_container_type;

	typedef yggr::thread::type_traits::tag_static_work_runner tag_type;

private:
	typedef Mutex tpl_arg_mutex_type;

	typedef static_work_runner this_type;

private:
	typedef detail::static_work_runner_base_link_handler<task_type> base_link_handler_type;
	typedef shared_ptr<base_link_handler_type> base_link_handler_container_type;
	typedef safe_container::safe_wrap<base_link_handler_container_type, tpl_arg_mutex_type> handler_container_wrap_type;
	typedef typename handler_container_wrap_type::error_type handler_continer_wrap_error_type;

private:
	typedef ActionTable<task_type, this_type, id_parser_type, tpl_arg_mutex_type> action_table_type;

public:
	// don't move to outside
	class shared_info
		: private nonable::noncopyable
	{
	public:
		typedef static_work_runner work_runner_type;

	public:
		shared_info(void)
		{
		}

		~shared_info(void)
		{
		}

	public:
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

			cal.template register_cal_object<static_work_runner>(
					tab,
					boost::bind(
						&work_runner_type::template prv_handler_result
						<
							now_error_type,
							owner_info_container_type
						>,
						_1, _2, _3, _4, _5, _6, _7 ) );
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
	};

public:
	typedef yggr::safe_container::safe_wrap<shared_info> shared_info_wrap_type;
	typedef typename shared_info_wrap_type::error_type shared_info_wrap_error_type;
	typedef typename shared_info_wrap_type::base_type shared_info_ptr_type;
	typedef shared_info_ptr_type init_type;

	//----------------------------------------------------------------------

public:
	static_work_runner(const init_type& ptr)
		: _shared_info_wrap(ptr)
	{
		assert(ptr);
	}

	~static_work_runner(void)
	{
	}

public:
	inline static bool need_link(void)
	{
		return true;
	}

	inline void clear_tasks(void) const
	{
	}

	inline void clear(void)
	{
	    {
            shared_info_ptr_type tmp;
            _shared_info_wrap.swap(tmp);
	    }
		_handler_wrap.clear();
	}

	template<typename Handler> inline
	void link(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef 
			detail::static_work_runner_link_handler
			<
				task_type, 
				handler_type
			> link_handler_type;

		base_link_handler_container_type p =
			construct_shared<base_link_handler_type>(
				yggr_nothrow_new link_handler_type(handler) );

		assert(p);
		_handler_wrap = p;
	}

	inline void drop_link(void)
	{
		_handler_wrap.clear();
	}

	inline bool operator()(task_type& task)
	{
		this_type::prv_run_task(task);
		return true;
	}

	inline static shared_info_ptr_type create_work_runner_shared_info(void)
	{
		return ::yggr::make_shared<shared_info>();
	}

	template<typename Owners> inline 
	void fix_result(u32 state, const Owners& owners,
						const data_info_type& data_info,
						u32 cal_type,
						task_data_container_type& rst)
	{
		this_type::prv_fixing_result(state, owners, data_info, cal_type, rst);
	}

private:
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
			if(owners.empty())
			{
				return;
			}
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

		assert(!owners.empty());
		assert(state == yggr::task_center::runtime_task_type::E_CAL_RESULT);

		base_link_handler_container_type ptr = this_type::prv_get_handler_container();
		assert(ptr);
		if(!ptr)
		{
			return;
		}

		for(owners_citer_type i = owners.begin(), isize = owners.end(); i != isize; ++i)
		{
			task_type tk(*i, data_info, cal_type, rst);
			ptr->run_handler(tk);
		}
	}

private:
	inline void prv_run_task(task_type& cdt)
	{
		if(!cdt.empty())
		{
			shared_info_ptr_type ptr = _shared_info_wrap.get_shared_ptr();

			if(ptr)
			{
				ptr->run_task(*this, cdt);
			}
		}
	}

	inline base_link_handler_container_type prv_get_handler_container(void) const
	{
		try
		{
			return _handler_wrap.load();
		}
		catch(const handler_continer_wrap_error_type&)
		{
			return base_link_handler_container_type();
		}
	}

private:
	shared_info_wrap_type _shared_info_wrap;
	handler_container_wrap_type _handler_wrap;
};


} // namespace thread
} // namespace yggr

#endif //__YGGR_THREAD_STATIC_WORK_RUNNER_HPP__
