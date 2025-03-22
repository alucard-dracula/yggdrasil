//work_thread.hpp

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

#ifndef __YGGR_THREAD_WORK_THREAD_HPP__
#define __YGGR_THREAD_WORK_THREAD_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/any_val/any.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/thread/type_traits/tags.hpp>

#include <yggr/base/static_constant.hpp>
#include <boost/function.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/tss.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/atomic.hpp>
#include <boost/type_traits/is_same.hpp>

#include <utility>

namespace yggr
{
namespace thread
{

template<typename Thread_Config = yggr::thread::boost_thread_config_type>
class work_thread
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Thread_Config thread_config_type;
	typedef typename thread_config_type::thread_type thread_type;
	typedef typename thread_config_type::time_type time_type;
	typedef ::yggr::shared_ptr<thread_type> thread_ptr_type;
	typedef typename thread_config_type::id_type id_type;

private:
	typedef work_thread this_type;

public:
	YGGR_STATIC_CONSTANT(u32, E_thread_state_start = 0);
	YGGR_STATIC_CONSTANT(u32, E_thread_stop = E_thread_state_start + 1);
	YGGR_STATIC_CONSTANT(u32, E_thread_free = E_thread_state_start + 2);
	YGGR_STATIC_CONSTANT(u32, E_thread_busy = E_thread_state_start + 3);

public:
	work_thread(void)
		: _sat(E_thread_stop), _brun(false),
			_tm_step(thread_config_type::E_def_sleep_time_sec, thread_config_type::E_def_sleep_time_nsec)
	{
	}

	work_thread(const time_type& tm_step)
		: _sat(E_thread_stop), _brun(false), _tm_step(tm_step)
	{
	}

	~work_thread(void)
	{
	}

public:
	inline static const std::type_info& get_thread_type(void)
	{
		return typeid(thread_type);
	}

public:
	inline operator thread_ptr_type&(void)
	{
		return _ptrd;
	}

	inline operator const thread_ptr_type&(void) const
	{
		return _ptrd;
	}

public:
	inline id_type get_id(void) const
	{
		return _ptrd? _ptrd->get_id() : id_type();
	}

public:
	inline bool compare_eq(const id_type& trd_id) const
	{
		return _ptrd && (_ptrd->get_id() == trd_id);
	}

	inline bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (_ptrd == right._ptrd)
			|| this_type::get_id() == right.get_id();
	}

public:
	void stop(void)
	{
		bool bnow = true;
		bool bnext = false;

		if(!_brun.compare_exchange_strong(bnow, bnext))
		{
			return;
		}

		this_type::join();
	}

	void interrupt(void)
	{
		if(!(_brun && _ptrd))
		{
			return;
		}

		_ptrd->interrupt();
	}


#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template<typename Runner, typename ...Args>
	bool start(BOOST_FWD_REF(Args)... args)
	{
		typedef Runner this_runner_type;
		typedef ::yggr::shared_ptr<this_runner_type> this_runner_ptr_type;

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

		this_runner_ptr_type runner_ptr =
			construct_shared<this_runner_type> (
				yggr_nothrow_new this_runner_type(boost::forward<Args>(args)...));

		thread_ptr_type ptr =
			construct_shared<thread_type>(
				yggr_nothrow_new thread_type(
									boost::bind(&work_thread::run<this_runner_type>,
									this, runner_ptr)));
		_ptrd.swap(ptr);
		return !!_ptrd;
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename Runner \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > \
		bool start(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			typedef Runner this_runner_type; \
			typedef ::yggr::shared_ptr<this_runner_type> this_runner_ptr_type; \
			u32 sat = E_thread_stop; \
			if(!_sat.compare_exchange_strong(sat, static_cast<u32>(E_thread_free))) { return true; } \
			bool brun = false; \
			if(!_brun.compare_exchange_strong(brun, true)) { return true;} \
			this_runner_ptr_type runner_ptr = \
				construct_shared<this_runner_type> ( \
					yggr_nothrow_new this_runner_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA))); \
			thread_ptr_type ptr = \
				construct_shared<thread_type>( \
					yggr_nothrow_new thread_type( \
										boost::bind(&work_thread::run<this_runner_type>, \
										this, runner_ptr))); \
			_ptrd.swap(ptr); \
			return !!_ptrd; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Runner>
	bool start(::yggr::shared_ptr<Runner> runner_ptr)
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

		{
			thread_ptr_type tmp =
				construct_shared<thread_type>(
					yggr_nothrow_new thread_type(
						boost::bind(&work_thread::run<this_runner_type>, this, runner_ptr)));

			_ptrd.swap(tmp);
		}

		return !!_ptrd;
	}

	inline void join(void)
    {
        if(!(_ptrd && _ptrd->joinable()))
        {
            return;
        }

		_ptrd->join();
    }

	inline bool is_free(void) const
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
	void run(::yggr::shared_ptr<Runner> runner_ptr)
	{
		BOOST_MPL_ASSERT((boost::is_same<typename Runner::tag_type,
											yggr::thread::type_traits::tag_dynamic_work_runner>));
		assert(runner_ptr);
		if(!runner_ptr)
		{
			return;
		}

		//u32 sat_free = E_thread_free;
		u32 sat_busy = E_thread_busy;
		//u32 sat_stop = E_thread_stop;

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

				time_type tm;
				tm += _tm_step;

                thread_type::yield();
				thread_type::sleep(tm);
            }
		}

		_sat.store(static_cast<u32>(E_thread_stop));
	}

private:
	boost::atomic<u32> _sat;
	boost::atomic<bool> _brun;
	time_type _tm_step;
	thread_ptr_type _ptrd;
};

// non-member foo

template<typename TrdCfg> inline
bool operator==(const typename work_thread<TrdCfg>::id_type& l, const work_thread<TrdCfg>& r)
{
	return r.compare_eq(l);
}

template<typename TrdCfg> inline
bool operator==(const work_thread<TrdCfg>& l, const typename work_thread<TrdCfg>::id_type& r)
{
	return l.compare_eq(r);
}

template<typename TrdCfg> inline
bool operator==(const work_thread<TrdCfg>& l, const work_thread<TrdCfg>& r)
{
	return l.compare_eq(r);
}

template<typename TrdCfg> inline
bool operator!=(const typename work_thread<TrdCfg>::id_type& l, const work_thread<TrdCfg>& r)
{
	return !r.compare_eq(l);
}

template<typename TrdCfg> inline
bool operator!=(const work_thread<TrdCfg>& l, const typename work_thread<TrdCfg>::id_type& r)
{
	return !l.compare_eq(r);
}

template<typename TrdCfg> inline
bool operator!=(const work_thread<TrdCfg>& l, const work_thread<TrdCfg>& r)
{
	return !l.compare_eq(r);
}

} // namespace thread
} // namespace yggr

#endif //__YGGR_THREAD_WORK_THREAD_HPP__
