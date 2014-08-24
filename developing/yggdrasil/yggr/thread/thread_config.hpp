//thread_config.hpp

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

#ifndef __YGGR_THREAD_THREAD_CONFIG_HPP__
#define __YGGR_THREAD_THREAD_CONFIG_HPP__

#include <yggr/nonable/noncreateable.hpp>
#include <yggr/thread/this_thread.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace thread
{

template<typename Thread, typename Thread_Group, typename Time>
class thread_config : private nonable::noncreateable
{
public:
	typedef Thread thread_type;
	typedef typename thread_type::id id_type;
	typedef Thread_Group thread_group_type;
	typedef Time time_type;

	enum
	{
		E_def_sleep_time_sec = 0,
		E_def_sleep_time_nsec = 33000000,
		E_compile_u32 = 0xffffffff
	};

	typedef this_thread<thread_type> this_thread_type;
};

} // namespace thread
} //namspace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_THREAD_THREAD_CONFIG_HPP__