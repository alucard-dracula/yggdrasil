//error.cpp

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

#include <yggr/base/error.hpp>
#include <yggr/utility/swap.hpp>

#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{

error::error(void)
	: _code(this_type::E_unknown_error),
		_msg( "[unknown class] ------  unknown error")
{
}

error::error(u32 code)
	: _code(0),
		_msg( "success" )
{
	assert(code == 0);
}

error::error(u32 code,
				const err_string_type& class_name,
				const err_string_type& other_msg,
				const err_string_type& msg)
	: _code(code), 
		_msg(class_name + err_string_type(" ") + other_msg + err_string_type(" ") + msg)
{
}

error::error(u32 code, const err_string_type& msg)
	: _code(code), _msg(msg)
{
}

error::error(const this_type& right)
	: _code(right._code), _msg(right._msg)
{
}

error::~error(void)
{
}

error::this_type& error::operator=(const this_type& right)
{
	if(this == &right) 
	{
		return *this;
	}
	_code = right._code;
	_msg = right._msg;
	return *this;
}

void error::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(_code, right._code);
	_msg.swap(right._msg);
}

} // namespace yggr
