//static_work_runner.hpp

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

#ifndef __YGGR_THREAD_STATIC_WORK_RUNNER_HPP__
#define __YGGR_THREAD_STATIC_WORK_RUNNER_HPP__

#include <cassert>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/support/ref.hpp>
#include <yggr/thread/action_table.hpp>
#include <yggr/thread/type_traits/tags.hpp>
#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/mplex/get_container_value_t.hpp>


namespace yggr
{
namespace thread
{

template<	typename Task,
			typename ID_Parser,
			typename Mutex = boost::mutex,
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

	//typedef typename task_type::id_type task_id_type;
	typedef typename task_type::owner_info_type owner_info_type;
	typedef typename task_type::data_info_type data_info_type;
	typedef typename task_type::data_container_type task_data_container_type;

	typedef yggr::thread::type_traits::tag_static_work_runner tag_type;

private:
	typedef Mutex mutex_type;

	typedef static_work_runner this_type;

private:
	template<typename TTask>
	class base_link_handler
	{
	public:
		typedef TTask task_type;

	public:
		virtual ~base_link_handler(void)
		{
		}

		virtual void run_handler(task_type& tk) = 0;
	};

	typedef base_link_handler<task_type> base_link_handler_type;
	typedef interface_ptr<base_link_handler_type> base_link_handler_container_type;
	typedef safe_container::safe_wrap<base_link_handler_container_type, mutex_type> handler_container_wrap_type;
	typedef typename handler_container_wrap_type::error_type handler_continer_wrap_error_type;

	template<typename Handler>
	class link_handler : public base_link_handler_type
	{
	public:
		typedef Handler handler_type;
		typedef base_link_handler_type base_type;
		typedef typename base_type::task_type task_type;


		explicit link_handler(const handler_type& handler)
			: _handler(handler)
		{
		}

		link_handler(const link_handler& h_recver)
			: _handler(h_recver._handler)
		{
		}

		virtual ~link_handler(void)
		{
		}

		virtual void run_handler(task_type& tk)
		{
			_handler(tk);
		}

	private:
		handler_type _handler;
	};
private:

	typedef ActionTable<task_type, this_type, ID_Parser, mutex_type> action_table_type;

public:
	class shared_info
		: private nonable::noncopyable
	{
	public:
		typedef static_work_runner work_runner_type;
		//typedef action_table_type action_table_type;

	public:

		explicit shared_info(void)
		{
		}

		~shared_info(void)
		{
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

	static bool s_need_link(void)
	{
		return true;
	}

	bool need_link(void) const
	{
		return true;
	}

	inline void clear_tasks(void)
	{
	}

	void clear(void)
	{
	    {
            shared_info_ptr_type tmp;
            _shared_info_wrap.swap(tmp);
	    }
		_handler_wrap.clear();
	}

	template<typename Handler>
	void link(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef link_handler<handler_type> link_handler_type;

		base_link_handler_container_type p(new link_handler_type(handler));
		assert(p);
		_handler_wrap = p;
	}

	void drop_link(void)
	{
		_handler_wrap.clear();
	}

	bool operator()(const task_type& task)
	{
		run_task(task);
		return true;
	}

	static shared_info_ptr_type create_work_runner_shared_info(void)
	{
		return shared_info_ptr_type(new shared_info());
	}

	template<typename Owners>
	void fix_result(u32 state, const Owners& owners,
						const data_info_type& data_info,
						u32 cal_type,
						task_data_container_type& rst)
	{
		prv_fix_result(state, owners, data_info, cal_type, rst);
	}

private:

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
		assert(state == yggr::task_center::runtime_task_type::E_CAL_RESULT);

		bool b = (owners.size() == 1);
		for(owners_citer_type i = owners.begin(), isize = owners.end(); i != isize; ++i)
		{
			task_type tk(*i, data_info, cal_type,
							b? yggr::move::move_helper::forced_move(rst) : rst);
			back_task(tk);
		}
	}

private:

	void run_task(const task_type& cdt)
	{
		if(cdt.empty())
		{
			return;
		}

		shared_info_ptr_type ptr = _shared_info_wrap.get_shared_ptr();

		if(ptr)
		{
			ptr->run_task(*this, cdt);
		}
	}

	void back_task(task_type& rst)
	{
		base_link_handler_container_type ptr;
		try
		{
			ptr = _handler_wrap.get_value();
		}
		catch(const handler_continer_wrap_error_type&)
		{
			return;
		}

		assert(ptr);
		ptr->run_handler(rst);
	}

private:
	shared_info_wrap_type _shared_info_wrap;
	handler_container_wrap_type _handler_wrap;
};


} // namespace thread
} // namespace yggr

#endif //__YGGR_THREAD_STATIC_WORK_RUNNER_HPP__
