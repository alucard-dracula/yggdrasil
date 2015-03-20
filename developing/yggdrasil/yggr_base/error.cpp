//error.cpp

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

#include <yggr/base/error.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{

error::error(void)
	: _code(this_type::E_unknow_error),
		_msg( "[unknow class] ------  unknow error")
{
}

//error::error(const u32& code)
//	: _code(code)
//{
//}

error::error(const u32& code,
				const err_string_type& class_name,
				const err_string_type& other_msg,
				const err_string_type& msg)
	: _code(code), _msg(class_name + err_string_type(" ") + other_msg + err_string_type(" ") + msg)
{
}

error::error(const u32& code, const err_string_type& msg)
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

error::operator bool(void) const
{
	return _code;
}

yggr::u32 error::code(void) const
{
	return _code;
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

	std::swap(_code, right._code);
	_msg.swap(right._msg);
}

const error::err_string_type& error::message(void) const
{
	return _msg;
}

const error::err_string_type& error::what(void) const
{
	return _msg;
}

} // namespace yggr
