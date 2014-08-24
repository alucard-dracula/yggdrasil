//yggr_exception_log_accesser.hpp

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

#ifndef __YGGR_LOG_YGGR_EXCEPTION_LOG_ACCESSER_HPP__
#define __YGGR_LOG_YGGR_EXCEPTION_LOG_ACCESSER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/log/log_op_def.hpp>
//#include <yggr/log/log_file.hpp>
#include <yggr/log/log_thread_file.hpp>
#include <yggr/log/log_file_mgr.hpp>
#include <yggr/log/log_file_op.hpp>
#include <yggr/log/log_accesser.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/thread/thread.hpp>

#include <yggr/charset/string.hpp>
#include <fstream>

namespace yggr
{
namespace log
{

class yggr_file_log_data_creator
{
public:
	typedef std::string fname_string_type;
	typedef fname_string_type init_type;

public:
	yggr_file_log_data_creator(void)
		: _fname("unknow_log")
	{
	}

	yggr_file_log_data_creator(const fname_string_type& fname)
		: _fname(fname)
	{
	}

	yggr_file_log_data_creator(const yggr_file_log_data_creator& right)
		: _fname(right._fname)
	{
	}

	~yggr_file_log_data_creator(void)
	{
	}

	yggr_file_log_data_creator& operator=(const yggr_file_log_data_creator& right)
	{
		_fname = right._fname;
		return *this;
	}

	bool operator==(const yggr_file_log_data_creator& right) const
	{
		return _fname == right._fname;
	}

	template<typename Value>
	boost::tuple<fname_string_type, Value> create_param(const Value& val) const
	{
		return boost::make_tuple(_fname, val);
	}

private:
	fname_string_type _fname;
};

typedef std::ofstream log_file_ostream_type;

//typedef log_op::log_file<log_file_ostream_type> yggr_exception_log_file_type;
typedef log_op::log_thread_file<log_file_ostream_type, boost::thread::id> yggr_exception_log_file_type;
typedef log_op::log_file_mgr<yggr_exception_log_file_type> yggr_exception_log_file_mgr_type;
typedef log_op::log_op_def<u32> yggr_exception_log_op_def_type;
typedef log_op::log_file_op<yggr_exception_log_op_def_type, yggr_exception_log_file_mgr_type> yggr_exception_log_file_op_type;
typedef log_accesser<u32, yggr_exception_log_file_op_type, yggr_file_log_data_creator> yggr_exception_log_accesser_type;

} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_YGGR_EXCEPTION_LOG_ACCESSER_HPP__