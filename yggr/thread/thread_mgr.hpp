//thread_mgr.hpp

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

#ifndef __YGGR_THREAD_THREAD_MANAGER_HPP__
#define __YGGR_THREAD_THREAD_MANAGER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/time/time.hpp>

#include <yggr/thread/mutex_def_helper.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/thread/shared_mutex.hpp>
#include <boost/config/abi_prefix.hpp>

#include <algorithm>

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif //_MSC_VER

namespace yggr
{
namespace thread
{

template<typename Work_Thread, typename Mutex = boost::shared_mutex>
class thread_mgr
	: public nonable::noncopyable
{
public:
	typedef Work_Thread work_thread_type;
	typedef typename work_thread_type::thread_config_type thread_config_type;
	typedef typename thread_config_type::time_type time_type;
	typedef typename work_thread_type::id_type id_type;

	typedef ::yggr::shared_ptr<work_thread_type> work_thread_ptr_type;
	typedef ::yggr::vector<work_thread_ptr_type> trd_vector_type;

private:
	typedef typename trd_vector_type::iterator trd_vt_iter_type;
	typedef typename trd_vector_type::const_iterator trd_vt_c_iter_type;

	typedef Mutex arg_mutex_type;
	typedef thread::mutex_def_helper<arg_mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

	typedef typename mutex_def_helper_type::lock_type lock_type; // debug end rm

private:
	typedef thread_mgr this_type;

public:
	thread_mgr(void)
	{
	}

	~thread_mgr(void)
	{
		_threads.clear();
	}

public:
	id_type get_id(size_type idx) const
	{
		work_thread_ptr_type ptr;
		{
			read_lock_type lk(_mutex);
			if(idx < _threads.size())
			{
				ptr =  _threads[idx];
			}
		}

		return ptr? ptr->get_id() : id_type();
	}

	template<typename Container>
	Container& get_id(Container& cont) const
	{
		trd_vector_type tmp;
		{
			read_lock_type lk(_mutex);
			tmp = _threads;
		}

		for(trd_vt_c_iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
		{
			if(*i)
			{
				iterator_ex::inserter(cont, boost::end(cont)) = (*i)->get_id();
			}
		}

		return cont;
	}

	this_type& add_thread(void)
	{
		work_thread_ptr_type ptr = ::yggr::make_shared<work_thread_type>();

		if(!ptr)
		{
			return *this;
		}

		{
			write_lock_type lk(_mutex);
			_threads.push_back(ptr);
		}
		return *this;
	}

	this_type& add_thread(const time_type& tm_step)
	{
		work_thread_ptr_type ptr = ::yggr::make_shared<work_thread_type>(tm_step);

		if(!ptr)
		{
			return *this;
		}

		{
			write_lock_type lk(_mutex);
			_threads.push_back(ptr);
		}
		return *this;
	}

	size_type size(void) const
	{
		read_lock_type lk(_mutex);
		return _threads.size();
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Runner, typename ...Args> 
	size_type startup(BOOST_FWD_REF(Args)... args) 
	{ 
		typedef Runner runner_type;
		write_lock_type lk(_mutex);
		for(size_type i = 0, isize = _threads.size(); i != isize; ++i) 
		{
			if(_threads[i] && _threads[i]->is_stop()) 
			{ 
				_threads[i]->template start<runner_type>(boost::forward<Args>(args)...);
				return 1; 
			} 
		} 
		return 0; 
	}
	
	template<typename Runner, typename ...Args> 
	size_type startup(size_type idx, BOOST_FWD_REF(Args)... args)
	{ 
		typedef Runner runner_type; 
		write_lock_type lk(_mutex); 
		if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop()) 
		{ 
			_threads[idx]->template start<runner_type>(boost::forward<Args>(args)...);
			return 1; 
		}
		return 0; 
	}
	
	template<typename Runner, typename ...Args> 
	size_type startup(size_type idx, 
						::yggr::shared_ptr<Runner> prunner,
						BOOST_FWD_REF(Args)... args) 
	{ 
		typedef Runner runner_type;
		write_lock_type lk(_mutex);
		if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop())
		{
			_threads[idx]->template start_of_runner_ptr<runner_type>(
				prunner, boost::forward<Args>(args)...);
			return 1; 
		}
		return 0; 
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename Runner \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > \
		size_type startup( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef Runner runner_type; \
			write_lock_type lk(_mutex); \
			for(size_type i = 0, isize = _threads.size(); i != isize; ++i) { \
				if(_threads[i] && _threads[i]->is_stop()) { \
					_threads[i]->template start< runner_type >( \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)); \
					return 1; } } \
			return 0; } \
		\
		template<typename Runner \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > \
		size_type startup(size_type idx \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef Runner runner_type; \
			write_lock_type lk(_mutex); \
			if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop()) { \
				_threads[idx]->template start<runner_type>( \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA) ); \
				return 1; } \
			return 0; } \
		\
		template<typename Runner \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > \
		size_type startup(size_type idx, ::yggr::shared_ptr<Runner> prunner \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef Runner runner_type; \
			write_lock_type lk(_mutex); \
			if(idx < _threads.size() && _threads[idx] && _threads[idx]->is_stop()) { \
				_threads[idx]->template start_of_runner_ptr<runner_type>( \
					prunner YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA) ); \
				return 1; } \
			return 0; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Runner>
	size_type startup(::yggr::shared_ptr<Runner> prunner)
	{
		typedef Runner runner_type;

		write_lock_type lk(_mutex);
		for(size_type i = 0, isize = _threads.size(); i != isize; ++i)
		{
			if(_threads[i] && _threads[i]->is_stop())
			{
				_threads[i]->template start<runner_type>(prunner);
				return 1;
			}
		}

		return 0;
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Runner, typename ...Args>
	size_type startup_all(BOOST_FWD_REF(Args)... args)
	{
		typedef Runner runner_type;
		size_type run_size = 0;
		write_lock_type lk(_mutex);
		for(size_type i = 0, isize = _threads.size(); i != isize; ++i)
		{ 
			if(_threads[i] && _threads[i]->is_stop()) 
			{ 
				_threads[i]->template start<runner_type>(boost::forward<Args>(args)...); 
				++run_size; 
			} 
		} 
		return run_size; 
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename Runner \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > \
		size_type startup_all( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef Runner runner_type; \
			size_type run_size = 0; \
			write_lock_type lk(_mutex); \
			for(size_type i = 0, isize = _threads.size() ;i != isize; ++i) { \
				if(_threads[i] && _threads[i]->is_stop()) { \
					_threads[i]->template start<runner_type>( \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)); \
					++run_size; } } \
			return run_size; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Runner>
	size_type startup_all(::yggr::shared_ptr<Runner> prunner)
	{
		typedef Runner runner_type;

		size_type run_size = 0;

		write_lock_type lk(_mutex);
		for(size_type i = 0, isize = _threads.size(); i != isize; ++i)
		{
			if(_threads[i] && _threads[i]->is_stop())
			{
				_threads[i]->template start<runner_type>(prunner);
				++run_size;
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
		write_lock_type lk(_mutex);

		for(size_type i = 0, isize = _threads.size(); i != isize; ++i)
		{
			if(_threads[i])
			{
				_threads[i]->join();
			}
		}
	}

	void stop(size_type idx)
	{
		write_lock_type lk(_mutex);
		if(idx < _threads.size() &&_threads[idx] )
		{
			_threads[idx]->stop();
		}
	}

	void stop(const id_type& id)
	{
		write_lock_type lk(_mutex);
		trd_vt_iter_type iter = std::find(_threads.begin(), _threads.end(), id);
		if((iter != _threads.end()) && (*iter))
		{
			(*iter)->stop();
		}
	}

	void stop_all(void)
	{
		write_lock_type lk(_mutex);
		for(size_type i = 0, isize = _threads.size(); i != isize; ++i)
		{
			if(_threads[i])
			{
				_threads[i]->stop();
			}
		}
	}

	void interrupt(size_type idx)
	{
		write_lock_type lk(_mutex);
		if(idx < _threads.size() && _threads[idx])
		{
			_threads[idx]->interrupt();
		}
	}

	void interrupt_all(void)
    {
		write_lock_type lk(_mutex);
		for(size_type i = 0, isize = _threads.size(); i < isize; ++i)
		{
			if(_threads[i])
			{
				_threads[i]->interrupt();
			}
		}
    }

private:
	mutable mutex_type _mutex;
	trd_vector_type _threads;
	
};


} // namespace thread
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning(pop)
#endif //_MSC_VER

#include <boost/config/abi_suffix.hpp>

#endif // __YGGR_THREAD_THREAD_MANAGER_HPP__
