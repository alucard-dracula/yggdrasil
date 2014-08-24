//work_thread.hpp

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

#ifndef __YGGR_THREAD_WORK_THREAD_HPP__
#define __YGGR_THREAD_WORK_THREAD_HPP__

#include <utility>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/swap.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/tss.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/atomic.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/thread/type_traits/tags.hpp>

namespace yggr
{
namespace thread
{

template<typename Thread_Config>
class work_thread : private nonable::noncopyable
{
public:
	typedef Thread_Config thread_config_type;
	typedef typename thread_config_type::thread_type thread_type;
	typedef typename thread_config_type::time_type time_type;
	typedef boost::shared_ptr<thread_type> thread_ptr_type;
	typedef typename thread_config_type::id_type id_type;

private:
	typedef work_thread this_type;

public:
	enum
	{
		E_thread_state_start = 0,
		E_thread_stop,
		E_thread_free,
		E_thread_busy,
		E_compile_u32_1 = 0xffffffff
	};

public:
	work_thread(void)
		: _sat(E_thread_stop), _brun(false),
		_tm_step(thread_config_type::E_def_sleep_time_sec, thread_config_type::E_def_sleep_time_nsec)
	{
	}

	work_thread(const time_type& tm_step)
		: _sat(E_thread_stop), _brun(false),
		_tm_step(tm_step)
	{
	}

	~work_thread(void)
	{
	}

	const std::type_info& get_thread_type(void) const
	{
		return typeid(thread_type);
	}

	work_thread& stop(void)
	{
		if(is_stop())
		{
			return *this;
		}

		_brun = false;
		join();

		return *this;
	}

	work_thread& interrupt(void)
	{
		if(!_brun || !_ptrd)
		{
			return *this;
		}

		_ptrd->interrupt();
		return *this;

	}

	const id_type get_id(void) const
	{
		if(!_ptrd)
		{
			return id_type();
		}

		return _ptrd->get_id();
	}


	bool operator==(const id_type& trd_id) const
	{
		if(!_ptrd)
		{
			return false;
		}
		return _ptrd->get_id() == trd_id;
	}

	operator thread_ptr_type&(void)
	{
		return _ptrd;
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Runner \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > \
	bool start(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS )) { \
		typedef Runner this_runner_type; \
		typedef boost::shared_ptr<this_runner_type> this_runner_ptr_type; \
		u32 sat = E_thread_stop; \
		if(!_sat.compare_exchange_strong(sat, static_cast<u32>(E_thread_free))) { return true; } \
		bool brun = false; \
		if(!_brun.compare_exchange_strong(brun, true)) { return true;} \
		this_runner_ptr_type runner_ptr (new this_runner_type(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA))); \
		thread_ptr_type ptr(new thread_type(boost::bind(&work_thread::run<this_runner_type>, this, runner_ptr))); \
		_ptrd.swap(ptr); \
		return static_cast<bool>(_ptrd); }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()
#undef YGGR_PP_FOO_ARG_NAME


	template<typename Runner>
	bool start(boost::shared_ptr<Runner> runner_ptr)
	{
		typedef Runner this_runner_type;

		u32 sat = E_thread_stop;

		if(!_sat.compare_exchange_strong(sat, static_cast<u32>(E_thread_free)))
		{
			return true;
		}

		bool brun = false;
		if(!_brun.compare_exchange_strong(brun, true))
		{
			return true;
		}

		_ptrd.swap(thread_ptr_type(new thread_type(boost::bind(&work_thread::run<this_runner_type>, this, runner_ptr))));

		return static_cast<bool>(_ptrd);
	}

	void join(void)
    {
        if(!_ptrd->joinable())
        {
            return;
        }

		_ptrd->join();
    }

	bool is_free(void) const
	{
		return _sat.load() == E_thread_free;
	}

	bool is_stop(void) const
	{
		return _sat.load() == E_thread_stop || !_ptrd;
	}

	bool is_busy(void) const
	{
		return _sat.load() == E_thread_busy;
	}

	bool is_running(void) const
	{
		return _sat.load() != E_thread_stop;
	}

private:

	template<typename Runner>
	void run(boost::shared_ptr<Runner> runner_ptr)
	{
		BOOST_MPL_ASSERT((boost::is_same<typename Runner::tag_type,
											yggr::thread::type_traits::tag_dynamic_work_runner>));
		assert(runner_ptr);
		if(!runner_ptr)
		{
			return;
		}

		u32 sat_free = E_thread_free;
		u32 sat_busy = E_thread_busy;
		u32 sat_stop = E_thread_stop;

		for(;_brun.load();)
		{
			_sat.store(static_cast<u32>(E_thread_busy));

			if(!(*runner_ptr)())
			{
				sat_busy = E_thread_busy;
				if(!_sat.compare_exchange_strong(sat_busy,
													static_cast<u32>(E_thread_free)))
				{
					assert(false);
					break;
				}

                thread_type::yield();
				time_type tm;
				tm += _tm_step;
				thread_type::sleep(tm);
            }
		}

		_sat.store(static_cast<u32>(E_thread_stop));
	}

private:
	boost::atomic<u32> _sat;
	thread_ptr_type _ptrd;
	time_type _tm_step;
	boost::atomic<bool> _brun;
};

} // namespace thread
} // namespace yggr

#endif //__YGGR_THREAD_WORK_THREAD_HPP__
