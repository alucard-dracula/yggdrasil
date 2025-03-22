//error.hpp

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

#ifndef __YGGR_ERROR_HPP__
#define __YGGR_ERROR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/support/operator_t.hpp>

#include <ostream>
#include <string> // don't using //#include <yggr/charset/string.hpp>, because loop include

namespace yggr
{

class error
{
public:
	typedef std::string err_string_type;

	YGGR_STATIC_CONSTANT(u32, E_unknown_error = 0xffffffff);

private:
	typedef error this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	error(void);
	error(u32 code);
	error(u32 code, const err_string_type& msg);
	error(u32 code,
			const err_string_type& class_name,
			const err_string_type& other_msg,
			const err_string_type& msg);

	error(BOOST_RV_REF(this_type) right) // if defined BOOST_COPYABLE_AND_MOVABLE, this foo must use inline
        : _code(right._code),
            _msg(boost::move(right._msg))
    {
    }

	error(const this_type& right);
	~error(void);

public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !!_code;
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
    {
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_code, boost::move(right._code));
		copy_or_move_or_swap(_msg, boost::move(right._msg));

		return *this;
    }

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline u32 code(void) const
	{
		return _code;
	}

	inline const err_string_type& message(void) const
	{
		return _msg;
	}

	inline const err_string_type& what(void) const
	{
		return _msg;
	}

private:
	u32 _code;
	err_string_type _msg;
};

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, const error& err)
{
	os << "code: " << err.code() << "\n"
		<< "msg: " << err.what().c_str();

	return os;
}

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(error) err)
{
	const error& right_cref = err;
	return operator<<(os, right_cref);
}

} // namespace yggr

#endif // __YGGR_ERROR_HPP__
