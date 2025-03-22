//log_file_mgr_op_caller.cpp

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

#include <yggr/log/log_file_mgr_op_caller.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace log
{

log_file_mgr_op_caller::log_file_mgr_op_caller(void)
	: _fname_wrap(charset::string_converter::s_conv<fname_string_type>("unknown_log_file"))
{
}

log_file_mgr_op_caller::log_file_mgr_op_caller(const fname_string_type& fname)
	: _fname_wrap(fname)
{
}

log_file_mgr_op_caller::log_file_mgr_op_caller(const this_type& right)
	: _fname_wrap(right._fname_wrap)
{
}

log_file_mgr_op_caller::~log_file_mgr_op_caller(void)
{
}

log_file_mgr_op_caller::this_type& 
	log_file_mgr_op_caller::operator=(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	_fname_wrap = right._fname_wrap;
	return *this;
}

void log_file_mgr_op_caller::swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	_fname_wrap.swap(right._fname_wrap);
}

} // namespace log
} // namespace yggr


