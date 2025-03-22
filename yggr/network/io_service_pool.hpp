//io_service_pool.hpp

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

#ifndef __YGGR_NETWORK_IO_SERVICE_POOL_HPP__
#define __YGGR_NETWORK_IO_SERVICE_POOL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/nonable/noncopyable.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/ptr_val/ptr_val_operator.hpp>

#include <yggr/container/default_allocator.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/exception/exception.hpp>
#include <yggr/system_controller/system_code.hpp>

#include <yggr/network/normal_io_service_selector.hpp>
#include <yggr/network/balance_io_service_selector.hpp>

#include <algorithm>
#include <boost/atomic.hpp>

namespace yggr
{
namespace network
{

template<typename Service,
			typename Thread_Config = thread::boost_thread_config_type,
			template<typename _PTy> class Allocator = YGGR_CONTAINER_DEFAULT_ALLOCATOR(),
			template<typename _PTy, typename _PAx> class Saver = container::vector,
			template<typename _Saver> class Selector = normal_io_service_selector>
class io_service_pool : private nonable::noncopyable
{
public:
	typedef Service service_type;
	typedef typename service_type::work work_type;

	typedef Thread_Config thread_config_type;
	typedef typename thread_config_type::thread_type thread_type;
	typedef typename thread_config_type::thread_group_type thread_group_type;

	typedef ::yggr::shared_ptr<service_type> service_ptr_type;
	typedef ::yggr::shared_ptr<work_type> work_ptr_type;

private:
	typedef Allocator<service_ptr_type> allocator_service_type;
	typedef Allocator<work_ptr_type> allocator_work_type;

	typedef boost::atomic<bool> run_type;

public:
	typedef Saver<service_ptr_type, allocator_service_type> saver_type;
	typedef typename saver_type::iterator saver_val_iter_type;
	typedef typename saver_type::const_iterator saver_val_c_iter_type;

	typedef Saver<work_ptr_type, allocator_work_type> work_saver_type;
	typedef typename work_saver_type::iterator work_saver_iter_type;
	typedef typename work_saver_type::const_iterator work_saver_c_iter_type;

	typedef Selector<saver_type> selector_type;

private:
	typedef io_service_pool this_type;

public:
	io_service_pool(void)
		: _brun(false), _saver(1), _selector(_saver) 
	{
		this_type::prv_init();
	}

	explicit io_service_pool(size_type size)
		:  _brun(false), _saver(size? size : 1), _selector(_saver)
	{
		this_type::prv_init();
	}

	explicit io_service_pool(const Saver<size_type, std::allocator<size_type> >& params)
		: _brun(false)
	{
		size_type size = params.size();
		_saver.resize(size? size : 1);

		if(!size)
		{
			this_type::prv_init();
			return;
		}

		this_type::prv_init(params);
	}

	~io_service_pool(void)
	{
#ifdef _DEBUG
		std::cout << "~io_service_pool" << std::endl;
#endif // _DEBUG
		assert(!_brun);
	}

	inline bool is_running(void) const
	{
		return _brun.load();
	}

	void run(void)
	{
		bool now = false;
		bool next = true; 
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

		for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize; ++i)
		{
			assert(*i);
			_trd_group.create_thread(boost::bind(&this_type::prv_service_run, this, *i));
		}
	}

	void run_one(void)
	{
		bool now = false;
		bool next = true; 
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

		for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize; ++i)
		{
			assert(*i);
			_trd_group.create_thread(boost::bind(&this_type::prv_service_run_one, this, *i));
		}
	}

	//void poll(void)
	//{
	//	for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize; ++i)
	//	{
	//		assert(*i);
	//		_trd_group.create_thread(boost::bind(&this_type::service_poll, this, *i));
	//	}
	//}

	//void poll_one(void)
	//{
	//	for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize; ++i)
	//	{
	//		assert(*i);
	//		_trd_group.create_thread(boost::bind(&this_type::service_poll_one, this, *i));
	//	}
	//}

	inline void join(void)
	{
		_trd_group.join_all();
	}

	inline size_type size(void) const
	{
		return _saver.size();
	}

	void stop(void)
	{
		bool now = true;
		bool next = false; 
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

		for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize; ++i)
		{
			assert(*i);
			if((!(*i)) || (*i)->stopped())
			{
			    continue;
			}
			(*i)->stop();
		}

		join();

		for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize; ++i)
		{
			assert(*i);
			if((!(*i)) && (*i)->stopped())
			{
			   (*i)->reset();
			   (*i)->stop();
			}
		}
	}

	inline service_type& get_service(void)
	{
		service_ptr_type ptr = _selector();
		assert(ptr);
		return *ptr;
	}

private:
	void prv_service_run_one(service_ptr_type ps)
	{
		if(!ps)
		{
			return;
		}

		for(;_brun.load();)
		{
			try
			{
				ps->run_one();
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_IO_SERVICE_POOL));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_IO_SERVICE_POOL));
			}

			ps->stop();
			ps->reset();
		}
	}

	void prv_service_run(service_ptr_type ps)
	{
		if(!ps)
		{
			return;
		}

		for(;_brun.load();)
		{
			try
			{
				ps->run();
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_IO_SERVICE_POOL));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_IO_SERVICE_POOL));
			}

			ps->stop();
			ps->reset();
		}
	}

	//void service_poll_one(service_ptr_type ps)
	//{
	//	if(!ps)
	//	{
	//		return;
	//	}

	//	try
	//	{
	//		ps->poll_one();
	//	}
	//	catch(const boost::system::system_error& e)
	//	{
	//		exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_IO_SERVICE_POOL));
	//	}
	//	catch(const compatibility::stl_exception& e)
	//	{
	//		exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_IO_SERVICE_POOL));
	//	}
	//}

	//void service_poll(service_ptr_type ps)
	//{
	//	if(!ps)
	//	{
	//		return;
	//	}

	//	try
	//	{
	//		for(;;)
	//			ps->poll()
	//	}
	//	catch(const boost::system::system_error& e)
	//	{
	//		exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_IO_SERVICE_POOL));
	//	}
	//	catch(const compatibility::stl_exception& e)
	//	{
	//		exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_IO_SERVICE_POOL));
	//	}
	//}

	void prv_init(void)
	{
		_work_saver.resize(_saver.size());
		work_saver_iter_type wi = _work_saver.begin();
		work_saver_iter_type wisize = _work_saver.end();
		service_ptr_type ptr;
		work_ptr_type wptr;

		for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize && wi != wisize; ++i, ++wi)
		{
			if(!((ptr = ::yggr::make_shared<service_type>())
					&& (wptr = construct_shared<work_type>(yggr_nothrow_new work_type(*ptr))) ))
			{
				assert(false);
				continue;
			}
			
			(*i).swap(ptr);
			(*wi).swap(wptr);
		}
	}

	void prv_init(const Saver<size_type, std::allocator<size_type> >& params)
	{
		typedef Saver<size_type, std::allocator<size_type> > params_type;
		typedef typename params_type::const_iterator c_iter_type;

		assert(params.size() == _saver.size());

		_work_saver.resize(_saver.size());

		c_iter_type isize_params = params.end();
		c_iter_type i_params = params.begin();

		saver_val_iter_type i = _saver.begin();
		saver_val_iter_type isize = _saver.end();

		work_saver_iter_type wi = _work_saver.begin();
		work_saver_iter_type wisize = _work_saver.end();
		service_ptr_type ptr;
		work_ptr_type wptr;

		for(;i_params != isize_params && i != isize && wi != wisize; ++i_params, ++i, ++wi)
		{
			if(!((ptr = construct_shared<service_type>(yggr_nothrow_new service_type(*i_params)))
					&& (wptr = construct_shared<work_type>(yggr_nothrow_new work_type(*ptr))) ) )
			{
				assert(false);
				continue;
			}

			(*i).swap(ptr);
			(*wi).swap(wptr);
		}
	}

private:
	run_type _brun;
	saver_type _saver;
	work_saver_type _work_saver;
	thread_group_type _trd_group;
	selector_type _selector;
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_IO_SERVICE_POOL_HPP__
