//linux_seh_env.cpp

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

#define YGGR_SEH_INCLUDE
#include <yggr/seh/linux_seh_env.hpp>

namespace yggr
{
namespace seh
{

linux_seh_env::mutex_type linux_seh_env::_jmp_buf_mutex;
linux_seh_env::mutex_type linux_seh_env::_signal_mutex;
linux_seh_env::mutex_type linux_seh_env::_call_stack_mutex;

linux_seh_env::jmp_buf_map_type linux_seh_env::_jmp_buf_map;
linux_seh_env::now_signal_map_type linux_seh_env::_now_sig_map;

//private:
/*static*/ void linux_seh_env::handler_recv_signal(int code)
{
	trd_id_type tid = this_thread_type::id();

	seh_helper_type::dump_call_stack_type call_stack;
	{
		write_lock_type lk(_call_stack_mutex);
		seh_helper_type::dump_call_stack(call_stack);
	}

	std::stringstream ss;

	ss << "os_code = " << code << "\n"
		<< seh_helper_type::format_dump_call_stack_msg(call_stack);

	exception::exception::throw_error(code, ss.str());

	{
		write_lock_type lk(_signal_mutex);
		_now_sig_map[tid] = code;
	}


	{
		write_lock_type lk(_jmp_buf_mutex);
		siglongjmp(_jmp_buf_map[tid].get(), 1);
	}
}

} // namespace seh
} // namespace yggr

