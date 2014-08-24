//error.hpp

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

#ifndef __YGGR_ERROR_HPP__
#define __YGGR_ERROR_HPP__

#include <string>
#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

namespace yggr
{

class error
{
public:
	typedef std::string err_string_type;

	enum
	{
		E_unknow_error = 0xffffffff
	};
private:
	typedef error this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	error(void);
	//error(const u32& code);
	error(const u32& code, const err_string_type& msg);
	error(const u32& code,
			const err_string_type& class_name,
			const err_string_type& other_msg,
			const err_string_type& msg);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	error(BOOST_RV_REF(this_type) right)
        : _code(boost::forward<u32>(right._code)),
            _msg(boost::forward<err_string_type>(right._msg))
    {
    }
#else
	error(BOOST_RV_REF(this_type) right) // if defined BOOST_COPYABLE_AND_MOVABLE, this foo must use inline
        : _code(this_type::E_unknow_error),
            _msg( "[unknow class] ------  unknow error")
    {
        this_type& right_ref = right;
        std::swap(_code, right_ref._code);
        _msg.swap(right_ref._msg);
    }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	error(const this_type& right);
	~error(void);

	operator bool(void) const;
	yggr::u32 code(void) const;

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
    {
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_code = boost::forward<u32>(right._code);
		_msg = boost::forward<err_string_type>(right._msg);
#else
		this_type& right_ref = right;
        std::swap(_code, right_ref._code);
        _msg.swap(right_ref._msg);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
    }
	this_type& operator=(const this_type& right);
	const err_string_type& message(void) const;
	const err_string_type& what(void) const;

private:
	u32 _code;
	err_string_type _msg;
};

} // namespace yggr

#endif // __YGGR_ERROR_HPP__
