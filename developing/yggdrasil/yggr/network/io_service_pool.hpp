//io_service_pool.hpp

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

#ifndef __YGGR_NETWORK_IO_SERVICE_POOL_HPP__
#define __YGGR_NETWORK_IO_SERVICE_POOL_HPP__

#include <compatibility/stl_exception.hpp>

#include <algorithm>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/ptr_val/ptr_val_operator.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/network/normal_io_service_selector.hpp>
#include <yggr/network/balance_io_service_selector.hpp>
#include <yggr/thread/boost_thread_config.hpp>

namespace yggr
{
namespace network
{

template<typename Service,
			typename Thread_Config = thread::boost_thread_config_type,
			template<typename _PTy> class Allocator = std::allocator,
			template<typename _PTy, typename _PAx> class Saver = std::vector,
			template<typename _Saver> class Selector = normal_io_service_selector>
class io_service_pool : private nonable::noncopyable
{
public:
	typedef Service service_type;
	typedef typename service_type::work work_type;

	typedef Thread_Config thread_config_type;
	typedef typename thread_config_type::thread_type thread_type;
	typedef typename thread_config_type::thread_group_type thread_group_type;

	typedef boost::shared_ptr<service_type> service_ptr_type;
	typedef boost::shared_ptr<work_type> work_ptr_type;

private:
	typedef Allocator<service_ptr_type> allocator_service_type;
	typedef Allocator<work_ptr_type> allocator_work_type;

public:
	typedef Saver<service_ptr_type, allocator_service_type> saver_type;
	typedef typename saver_type::iterator saver_val_iter_type;
	typedef typename saver_type::const_iterator saver_val_c_iter_type;

	typedef Saver<work_ptr_type, allocator_work_type> work_saver_type;
	typedef typename work_saver_type::iterator work_saver_iter_type;
	typedef typename work_saver_type::const_iterator work_saver_c_iter_type;

	typedef Selector<saver_type> selector_type;

	typedef io_service_pool this_type;

public:
	io_service_pool(void)
		: _brun(false), _saver(1), _selector(_saver) 
	{
		init();
	}

	explicit io_service_pool(size_type size)
		:  _brun(false), _saver(size? size : 1), _selector(_saver)
	{
		init();
	}

	//explicit io_service_pool(size_type size, size_type trdsize)
	//	: _saver(size? size : 1), _selector(_saver), _brun(false)
	//{
	//	init(trdsize);
	//}

	explicit io_service_pool(const Saver<size_type, std::allocator<size_type> >& params)
		: _brun(false)
	{
		size_type size = params.size();
		_saver.resize(size? size : 1);

		if(!size)
		{
			init();
			return;
		}

		init(params);
	}

	~io_service_pool(void)
	{
		std::cout << "~io_service_pool" << std::endl;
		assert(!_brun);
		//_work_saver.clear();

		//stop();
		//_work_saver.clear(); //work must delete befor io_service
	}

	bool is_running(void) const
	{
		return _brun;
	}

	void run(void)
	{
		if(_brun)
		{
			return;
		}

		_brun = true;
		for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize; ++i)
		{
			assert(*i);
			_trd_group.create_thread(boost::bind(&this_type::service_run, this, *i));
		}
	}

	void run_one(void)
	{
		if(_brun)
		{
			return;
		}

		_brun = true;
		for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize; ++i)
		{
			assert(*i);
			_trd_group.create_thread(boost::bind(&this_type::service_run_one, this, *i));
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

	void join(void)
	{
		_trd_group.join_all();
	}

	const size_type& size(void) const
	{
		return _saver.size();
	}

	void stop(void)
	{
		if(!_brun)
		{
			return;
		}

		_brun = false;

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

	service_type& get_service(void)
	{
		service_ptr_type ptr = _selector();
		assert(ptr);
		return *ptr;
	}

private:
	void service_run_one(service_ptr_type ps)
	{
		if(!ps)
		{
			return;
		}

		for(;_brun;)
		{
			try
			{
				ps->run_one();
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_IO_SERVICE_POOL);
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_IO_SERVICE_POOL);
			}

			ps->stop();
			ps->reset();
		}
	}

	void service_run(service_ptr_type ps)
	{
		if(!ps)
		{
			return;
		}

		for(;_brun;)
		{
			try
			{
				ps->run();
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_IO_SERVICE_POOL);
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_IO_SERVICE_POOL);
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
	//		exception::exception::throw_error(e, (u32)system_controller::system_code::E_IO_SERVICE_POOL);
	//	}
	//	catch(const compatibility::stl_exception& e)
	//	{
	//		exception::exception::throw_error(e, (u32)system_controller::system_code::E_IO_SERVICE_POOL);
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
	//		exception::exception::throw_error(e, (u32)system_controller::system_code::E_IO_SERVICE_POOL);
	//	}
	//	catch(const compatibility::stl_exception& e)
	//	{
	//		exception::exception::throw_error(e, (u32)system_controller::system_code::E_IO_SERVICE_POOL);
	//	}
	//}

	void init(void)
	{
		_work_saver.resize(_saver.size());
		work_saver_iter_type wi = _work_saver.begin();
		work_saver_iter_type wisize = _work_saver.end();
		for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize && wi != wisize; ++i, ++wi)
		{
			service_ptr_type ptr(new service_type());
			assert(ptr);
			(*i).swap(ptr);
			work_ptr_type wptr(new work_type(*(*i)));
			assert(wptr);
			(*wi).swap(wptr);
		}
	}

	//void init(size_type trd_size)
	//{
	//	_work_saver.resize(_saver.size());
	//	work_saver_iter_type wi = _work_saver.begin();
	//	work_saver_iter_type wisize = _work_saver.end();
	//	for(saver_val_iter_type i = _saver.begin(), isize = _saver.end(); i != isize && wi != wisize; ++i,++wi)
	//	{
	//		service_ptr_type ptr(new service_type(trd_size));
	//		assert(ptr);
	//		work_ptr_type wptr(new work_type(*ptr));
	//		assert(wptr);
	//		(*i).swap(ptr);
	//		(*wi).swap(wptr);
	//	}
	//}

	void init(const Saver<size_type, std::allocator<size_type> >& params)
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
		for(;i_params != isize_params && i != isize && wi != wisize; ++i_params, ++i, ++wi)
		{
			service_ptr_type ptr(new service_type(*i_params));
			assert(ptr);
			work_ptr_type wptr(new work_type(*ptr));
			assert(wptr);
			(*i).swap(ptr);
			(*wi).swap(wptr);
		}
	}

private:
	volatile bool _brun;
	saver_type _saver;
	work_saver_type _work_saver;
	thread_group_type _trd_group;
	selector_type _selector;
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_IO_SERVICE_POOL_HPP__
