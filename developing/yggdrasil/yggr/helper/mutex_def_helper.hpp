//mutex_def_helper.hpp

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

#ifndef __YGGR_HELPER_MUTEX_DEF_HELPER_HPP__
#define __YGGR_HELPER_MUTEX_DEF_HELPER_HPP__

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER
namespace yggr
{
namespace helper
{

template<typename Mutex>
class mutex_def_helper 
	: private nonable::noncreateable
{
public:
	typedef Mutex mutex_type;
	typedef typename mutex_type::scoped_lock lock_type;
	typedef lock_type read_lock_type;
	typedef lock_type write_lock_type;
};

template<>
class mutex_def_helper<boost::shared_mutex> 
	: private nonable::noncreateable
{
public:
	typedef boost::shared_mutex mutex_type;
	typedef boost::shared_lock<mutex_type> lock_type;
	typedef lock_type read_lock_type;
	typedef boost::unique_lock<mutex_type> write_lock_type;
};

template<>
class mutex_def_helper<boost::interprocess::interprocess_mutex>
	: private nonable::noncreateable
{
public:
	typedef boost::interprocess::interprocess_mutex mutex_type;
	typedef boost::interprocess::scoped_lock<mutex_type> lock_type;
	typedef lock_type read_lock_type;
	typedef lock_type write_lock_type;
};

} // namesapce thread
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif //__YGGR_HELPER_MUTEX_DEF_HELPER_HPP__
