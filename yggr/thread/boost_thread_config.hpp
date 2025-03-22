//boost_thread_config.hpp

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

#ifndef __YGGR_THREAD_BOOST_THREAD_CONFIG_HPP__
#define __YGGR_THREAD_BOOST_THREAD_CONFIG_HPP__

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/thread_config.hpp>
#include <yggr/time/time.hpp>

namespace yggr
{
namespace thread
{
	
#ifndef YGGR_TPL_CONFIG_USING_TPL 

YGGR_PP_THREAD_CONFIG_DECL(boost_thread_config_type, boost::thread, boost::thread_group, time::time);

#else

typedef thread_config<boost::thread, boost::thread_group, time::time> boost_thread_config_type;

#endif // YGGR_TPL_CONFIG_USING_TPL

} // namespace thread
} //namspace yggr

#endif //__YGGR_THREAD_BOOST_THREAD_CONFIG_HPP__