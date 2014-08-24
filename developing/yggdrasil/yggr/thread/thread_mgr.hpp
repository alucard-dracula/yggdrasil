//thread_mgr.hpp

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

#ifndef __YGGR_THREAD_THREAD_MANAGER_HPP__
#define __YGGR_THREAD_THREAD_MANAGER_HPP__

#include <algorithm>
#include <vector>
#include <iterator>

#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/config/abi_prefix.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/helper/mutex_def_helper.hpp>
#include <yggr/time/time.hpp>
#include <yggr/ppex/foo_params.hpp>


#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif //_MSC_VER

namespace yggr
{
namespace thread
{

template<typename Work_Thread, typename Mutex = boost::shared_mutex>
class thread_mgr : public nonable::noncopyable
{
public:
	typedef Work_Thread work_thread_type;
	typedef typename work_thread_type::thread_config_type thread_config_type;
	typedef typename thread_config_type::time_type time_type;
	typedef typename work_thread_type::id_type id_type;

	typedef boost::shared_ptr<work_thread_type> work_thread_ptr_type;
	typedef std::vector<work_thread_ptr_type> trd_vector_type;

private:
	typedef typename trd_vector_type::iterator trd_vt_iter_type;
	typedef typename trd_vector_type::const_iterator trd_vt_c_iter_type;

	typedef Mutex mutex_type;
	typedef typename helper::mutex_def_helper<mutex_type>::lock_type lock_type;

public:
	thread_mgr(void)
	{
	}

	~thread_mgr(void)
	{
		//_threads.clear();
		//stop_all();
		_threads.clear();
	}

	const id_type get_id(size_type idx) const
	{
		lock_type lk(_mutex);

		if(idx < _threads.size() && _threads[idx])
		{
			return _threads[idx]->get_id();
		}
		return id_type();
	}

	template<typename Container>
	void get_id(Container& container) const
	{
		lock_type lk(_mutex);

		trd_vt_c_iter_type isize = _threads.end();
		for(trd_vt_c_iter_type i = _threads.begin(); i != isize; ++i)
		{
			if(*i)
			{
				//container.push_back((*i)->get_id());
				std::back_inserter(container) = (*i)->get_id();
			}
		}
	}

	thread_mgr& add_thread(void)
	{
		work_thread_ptr_type ptr(new work_thread_type());
		if(!ptr)
		{
			return *this;
		}

		lock_type lk(_mutex);
		_threads.push_back(ptr);
		return *this;
	}

	thread_mgr& add_thread(const time_type& tm_step)
	{
		work_thread_ptr_type ptr(new work_thread_type(tm_step));
		if(!ptr)
		{
			return *this;
		}

		lock_type lk(_mutex);
		_threads.push_back(ptr);
		return *this;
	}

	//template<typename Runner>
	//thread_mgr& add_thread_now_run(typename const Runner::init_type& init, u32 sec = 3, u32 nsec = 0)
	//{
	//	typedef Runner runner_type;

	//	work_thread_ptr_type ptr(new work_thread_type(sec, nsec));

	//	if(!ptr)
	//	{
	//		return *this;
	//	}

	//	lock_type lk(_mutex);
	//	_threads.push_back(work_thread_ptr_type(new work_thread_type(sec, nsec)));
	//	_threads.back()->template start<runner_type>(init);
	//	return *this;
	//}


	size_type size(void) const
	{
		return _threads.size();
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Runner \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > \
	bool startup( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		typedef Runner runner_type; \
		lock_type lk(_mutex); \
		for(size_type i = 0, isize = _threads.size(); i != isize; ++i) { \
			if(_threads[i] && _threads[i]->is_stop()) { \
				_threads[i]->template start< runner_type > \
					(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); \
				return true; } } \
		return false; } \
	\
	template<typename Runner \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > \
	bool startup(size_type idx \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		typedef Runner runner_type; \
		lock_type lk(_mutex); \
		if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop()) { \
			_threads[idx]->template start<runner_type> \
				( YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) ); \
			return true; } \
		return false; } \
	\
	template<typename Runner \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > \
	bool startup(size_type idx, boost::shared_ptr<Runner> prunner \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		typedef Runner runner_type; \
		lock_type lk(_mutex); \
		if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop()) { \
			_threads[idx]->template start_of_runner_ptr<runner_type> \
				(prunner \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) ); \
			return true; } \
		return false; }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()
#undef YGGR_PP_FOO_ARG_NAME


	/*template<typename Runner,typename Init>
	bool startup(const Init& init)
	{
		typedef Runner runner_type;
		typedef Init init_type;

		lock_type lk(_mutex);

		for(size_type i = 0, isize = _threads.size(); i != isize; ++i)
		{
			if(_threads[i] && _threads[i]->is_stop())
			{
				_threads[i]->template start<runner_type>(init);
				return true;
			}
		}

		return false;
	}*/

	template<typename Runner>
	bool startup(boost::shared_ptr<Runner> prunner)
	{
		typedef Runner runner_type;

		lock_type lk(_mutex);

		for(size_type i = 0, isize = _threads.size(); i != isize; ++i)
		{
			if(_threads[i] && _threads[i]->is_stop())
			{
				_threads[i]->template start<runner_type>(prunner);
				return true;
			}
		}

		return false;
	}


	//template<typename Runner>
	//bool startup(size_type idx)
	//{
	//	typedef Runner runner_type;

	//	lock_type lk(_mutex);
	//	if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop())
	//	{
	//		_threads[idx]->template start<runner_type>();
	//		return true;
	//	}
	//	return false;
	//}

	//template<typename Runner, typename Init>
	//bool startup(size_type idx, const Init& init)
	//{
	//	typedef Runner runner_type;
	//	typedef Init init_type;

	//	lock_type lk(_mutex);
	//	if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop())
	//	{
	//		_threads[idx]->template start<runner_type>(init);
	//		return true;
	//	}
	//	return false;
	//}

	template<typename Runner>
	bool startup(size_type idx, boost::shared_ptr<Runner> prunner)
	{
		typedef Runner runner_type;

		lock_type lk(_mutex);
		if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop())
		{
			_threads[idx]->template start_of_runner_ptr<runner_type>(prunner);
			return true;
		}
		return false;
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Runner \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > \
	size_type startup_all( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		typedef Runner runner_type; \
		size_type run_size = 0; \
		lock_type lk(_mutex); \
		for(size_type i = 0, isize = _threads.size() ;i != isize; ++i) { \
			if(_threads[i] && _threads[i]->is_stop()) { \
				_threads[i]->template start<runner_type> \
					(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); } } \
		return run_size; }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()
#undef YGGR_PP_FOO_ARG_NAME

	//template<typename Runner,typename Init>
	//size_type startup_all(const Init& init)
	//{
	//	typedef Runner runner_type;
	//	typedef Init init_type;

	//	size_type run_size = 0;

	//	lock_type lk(_mutex);

	//	for(size_type i = 0, isize = _threads.size() ;i != isize; ++i)
	//	{
	//		if(_threads[i] && _threads[i]->is_stop())
	//		{
	//			_threads[i]->template start<runner_type>(init);
	//		}
	//	}

	//	return run_size;
	//}

	template<typename Runner>
	size_type startup_all(boost::shared_ptr<Runner> prunner)
	{
		typedef Runner runner_type;

		size_type run_size = 0;

		lock_type lk(_mutex);

		for(size_type i = 0, isize = _threads.size() ;i != isize; ++i)
		{
			if(_threads[i] && _threads[i]->is_stop())
			{
				_threads[i]->template start<runner_type>(prunner);
			}
		}

		return run_size;
	}

	void join(size_type idx)
    {
		lock_type lk(_mutex);
        if(idx < _threads.size() && _threads[idx])
		{
			_threads[idx]->join();
		}
    }

	void join_all(void)
	{
		lock_type lk(_mutex);

		size_type isize = _threads.size();
		size_type i = 0;

		for( ;i < isize; ++i)
		{
			if(_threads[i])
			{
				_threads[i]->join();
			}
		}
	}

	void stop(size_type idx)
	{
		lock_type lk(_mutex);
		if(idx < _threads.size())
		{
			if(_threads[idx])
			{
				_threads[idx]->stop();
			}
		}
	}

	void stop(const id_type& id)
	{
		lock_type lk(_mutex);
		trd_vt_iter_type iter = std::find(_threads.begin(), _threads.end(), id);
		if(iter != _threads.end())
		{
			(*iter)->stop();
		}
	}

	void stop_all(void)
	{
		lock_type lk(_mutex);
		size_type isize = _threads.size();
		for(size_type i = 0; i < isize; ++i)
		{
			if(_threads[i])
			{
				//_threads[i]->non_join_stop();
				_threads[i]->stop();
			}
		}

		//prv_join_all();
	}

	void interrupt(size_type idx)
	{
		lock_type lk(_mutex);
		if(idx < _threads.size())
		{
			if(_threads[idx])
			{
				_threads[idx]->interrupt();
			}
		}
	}

	void interrupt_all()
    {
		lock_type lk(_mutex);
		size_type isize = _threads.size();
		for(size_type i = 0; i < isize; ++i)
		{
			if(_threads[i])
			{
				_threads[i]->interrupt();
			}
		}
    }

private:
	void prv_join_all(void)
	{
		size_type isize = _threads.size();
		size_type i = 0;

		for( ;i < isize; ++i)
		{
			if(_threads[i])
			{
				_threads[i]->join();
			}
		}
	}
private:
	trd_vector_type _threads;
	mutable mutex_type _mutex;
};


} // namespace thread
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning(pop)
#endif //_MSC_VER

#include <boost/config/abi_suffix.hpp>

#endif // __YGGR_THREAD_THREAD_MANAGER_HPP__
