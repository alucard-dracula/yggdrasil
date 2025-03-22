//this_thread.hpp

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

#ifndef __YGGR_THREAD_THIS_THREAD_HPP__
#define __YGGR_THREAD_THIS_THREAD_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>

namespace yggr
{
namespace thread
{

template<typename Thread>
class this_thread 
	: private nonable::noncreateable
{
private:
	typedef Thread thread_type;
public:
	typedef typename thread_type::id id_type;
public:
	inline static const id_type id(void)
	{
		return thread_type::this_thread_id();
	}

	template<typename Time> inline
	static void sleep(const Time& step)
	{
		typedef Time time_type;
		time_type tm;
		tm += step;
		thread_type::sleep(tm);
	}

	template<typename Time> inline
	static void sleep(const Time& stm, const Time& step)
	{
		thread_type::sleep(stm + step);
	}

	template<typename Time> inline
	static void sleep_to(const Time& tm)
	{
		thread_type::sleep(tm);
	}

	inline static void yield(void)
	{
		thread_type::yield();
	}
};

} // namesapce thread
} // namespace yggr

#define YGGR_THIS_THREAD_INCLUDE
#	include <yggr/thread/boost_this_thread.hpp>
#undef YGGR_THIS_THREAD_INCLUDE

#endif // __YGGR_THREAD_THIS_THREAD_HPP__