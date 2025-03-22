//log_thread_file_accesser.hpp

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

#ifndef __YGGR_LOG_LOG_THREAD_FILE_ACCESSER_HPP__
#define __YGGR_LOG_LOG_THREAD_FILE_ACCESSER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/thread/boost_thread.hpp>

#include <yggr/move/move.hpp>
#include <yggr/tuple/tuple.hpp>

#include <yggr/log/log_op_def.hpp>
#include <yggr/log/log_thread_file.hpp>
#include <yggr/log/log_file_mgr.hpp>
#include <yggr/log/log_file_op.hpp>
#include <yggr/log/log_accesser.hpp>

#include <yggr/log/log_file_params_maker.hpp>

#include <fstream>

namespace yggr
{
namespace log
{

typedef std::ofstream log_thread_file_ostream_type;

typedef log_op::log_thread_file<log_thread_file_ostream_type, boost::thread::id> log_thread_file_type;
typedef log_op::log_file_mgr<log_thread_file_type> log_thread_file_mgr_type;
typedef log_op::log_file_op<log_op::log_op_def, log_thread_file_mgr_type> log_thread_file_op_type;

typedef log_accesser<u32, log_thread_file_op_type, log_file_params_maker> log_thread_file_accesser_type;

} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOG_THREAD_FILE_ACCESSER_HPP__
