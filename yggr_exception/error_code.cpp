//error_code.cpp

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

#include <yggr/exception/error_code.hpp>
#include <ctime>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace exception
{

error_code::error_code(void)
	: _tm(0),
		_trd_id(-1),
		_code(-1),
		_msg("invlid code", YGGR_STR_UTF8_STRING_CHARSET_NAME())
{
}

error_code::error_code(u32 code, const err_string_type& msg)
	: _tm(std::time(0)),
		_trd_id(0),
		_code(code),
		_msg(msg)
{
	u64 tmp_tid = 0;
	ss_type ss;
	ss << this_thread_type::id();
	ss >> std::hex >> tmp_tid;

	_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
}

error_code::error_code(u32 code,
						const trd_id_type& tid,
						const err_string_type& msg)
	: _tm(std::time(0)),
		_trd_id(0),
		_code(code),
		_msg(msg)
{
	u64 tmp_tid = 0;
	ss_type ss;
	ss << tid;
	ss >> std::hex >> tmp_tid;

	_trd_id = (*(reinterpret_cast<s64*>(&tmp_tid)));
}

error_code::error_code(const this_type& right)
	: _tm(right._tm),
		_trd_id(right._trd_id),
		_code(right._code),
		_msg(right._msg)
{
}

error_code::~error_code(void)
{
}

error_code::this_type& error_code::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_tm = right._tm;
	_trd_id = right._trd_id;
	_code = right._code;
	_msg = right._msg;
	return *this;
}

void error_code::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(_tm, right._tm);
	yggr::swap(_trd_id, right._trd_id);
	yggr::swap(_code, right._code);
	yggr::swap(_msg, right._msg);
}

} // namespace exception
} // namespace yggr
