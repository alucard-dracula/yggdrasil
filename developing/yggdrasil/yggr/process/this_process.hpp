//this_process.hpp

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

#ifndef __YGGR_PROCESS_THIS_PROCESS_HPP__
#define __YGGR_PROCESS_THIS_PROCESS_HPP__

#include <boost/config.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>

#include <yggr/nonable/noncreateable.hpp>

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4624)
#endif // _MSC_VER

namespace yggr
{
namespace process
{

class this_process : private nonable::noncreateable
{
public:
	typedef boost::interprocess::ipcdetail::OS_process_id_t id_type;

public:
	static const id_type id(void)
	{
		return boost::interprocess::ipcdetail::get_current_process_id();
	}
};

} // namespace process
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning(pop)
#endif // _MSC_VER

#endif //__YGGR_PROCESS_THIS_PROCESS_HPP__