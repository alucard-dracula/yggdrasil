// exception_serialization_dtl_error_code.hpp

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

#ifndef __YGGR_EXCEPTION_SERIALIZATION_DTL_ERROR_CODE_HPP__
#define __YGGR_EXCEPTION_SERIALIZATION_DTL_ERROR_CODE_HPP__

#error "!!!If you need this feature, please modify the DTL first!!!"

#include <yggr/database_system/dtl_include_helper.hpp>
#include <yggr/exception/error_code.hpp>

namespace dtl
{

template<>
class DefaultBCA<yggr::exception::error_code>
{
public:
	inline void operator()(BoundIOs& cols, yggr::exception::error_code& row_buf)
	{
		cols["code"] == row_buf.code();
		cols["time"] == row_buf.time();
		cols["thread_id"] == row_buf.thread_id();
		cols["message"] == row_buf.message();
	}
};

} // namepsace dtl

#endif // __YGGR_EXCEPTION_SERIALIZATION_DTL_ERROR_CODE_HPP__
