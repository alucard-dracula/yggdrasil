//exception.cpp

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

#include <yggr/exception/exception.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace exception
{

exception::exception(void)
	: _brun(false), _tm_step(0, 33000000)
{
	this_type::init_obj();
}

exception::exception(const exception::time_type& tm_step)
	: _brun(false), _tm_step(tm_step)
{
	this_type::init_obj();
}

exception::~exception(void)
{
	this_type::stop();
}

void exception::init_obj(void)
{

	bool brun_old = false;
	bool brun_new = true;

	if(!_brun.compare_exchange_strong(brun_old, brun_new))
	{
		return;
	}

	if(!(_trd_group.create_thread(boost::bind(&exception::run, this))))
	{
		_brun.store(false);
	}
}

void exception::run(void)
{
	for(;_brun.load();)
	{
		if(message())
		{
			continue;
		}

		time_type tm;
		tm += _tm_step;

		thread_type::yield();
		thread_type::sleep(tm);
	}
}

bool exception::message(void)
{
	typedef err_queue_type::base_type base_queue_type;
	typedef base_queue_type::const_iterator iter_type;

	base_queue_type tmp_queue;
	_err_queue.swap(tmp_queue);

	if(tmp_queue.empty())
	{
		return false;
	}

	interface_log_helper_type plog;
	try
	{
		plog = _log_helper_wrap.load();
	}
	catch(const log_helper_ptr_wrap_type::error_type&)
	{
		interface_log_helper_type tmp;
		tmp.swap(plog);
	}

	if(plog)
	{
		for(iter_type i = tmp_queue.begin(), isize = tmp_queue.end(); i != isize; ++i)
		{
			if(!plog->append(*i))
			{
				std::cerr << (*i) << std::endl;
			}
		}
	}
	else
	{
		for(iter_type i = tmp_queue.begin(), isize = tmp_queue.end(); i != isize; ++i)
		{
			std::cerr << (*i) << std::endl;
		}
	}

	return true;
}


void exception::stop(void)
{
	bool brun_old = true;
	bool brun_new = false;

	if(!_brun.compare_exchange_strong(brun_old, brun_new))
	{
		return;
	}

	this_type::join();
	for(;this_type::message(););
	_log_helper_wrap.clear();
}

} // namespace exception
} // namespace yggr

