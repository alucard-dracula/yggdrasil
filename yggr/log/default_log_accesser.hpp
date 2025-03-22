//default_log_accesser.hpp

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

#ifndef __YGGR_LOG_DEFAULT_LOG_ACCESSER_HPP__
#define __YGGR_LOG_DEFAULT_LOG_ACCESSER_HPP__

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/log/log_of_file.hpp>

namespace yggr
{
namespace log
{

typedef 
	log_accesser
	<
		u32, 
		log_file_op
		<
			log_op_def,
			log_op::log_file_mgr
			<
				log_op::log_thread_file<std::ofstream, s64>
			>
		>, 
		log_file_mgr_op_caller
	> default_log_accesser;

typedef ::yggr::shared_ptr<default_log_accesser> default_log_access_ptr;

} // namespace log
} // namespace yggr


#endif // __YGGR_LOG_DEFAULT_LOG_ACCESSER_HPP__
