//script_code.hpp

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

#ifndef __YGGR_SCRIPT_CODE_HPP__
#define __YGGR_SCRIPT_CODE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <boost/ref.hpp>
#include <cassert>


namespace yggr
{
namespace script
{

struct tag_file_code {};
struct tag_string_code {};

template<typename Tag>
class script_code
{
public:
	typedef charset::utf8_string string_type;

private:
	typedef script_code this_type;

private:
	this_type& operator=(BOOST_RV_REF(this_type));
	this_type& operator=(const this_type&);

public:
	template<typename String>
	script_code(BOOST_RV_REF(String) code)
		: _str_code(code), _pstr_code(boost::addressof(_str_code))
	{
		assert(_pstr_code);
	}

	template<typename String>
	script_code(const String& code)
		: _str_code(code), _pstr_code(boost::addressof(_str_code))
	{
		assert(_pstr_code);
	}

	script_code(BOOST_RV_REF(string_type) code)
		: _str_code(boost::move(code)), _pstr_code(boost::addressof(_str_code))
	{
		assert(_pstr_code);
	}

	script_code(const string_type& code)
		: _pstr_code(boost::addressof(code))
	{
		assert(_pstr_code);
	}

	script_code(BOOST_RV_REF(this_type) right)
		: _str_code(boost::move(right._str_code)),
			_pstr_code(right.is_inner_link()?
						boost::addressof(_str_code) : right._pstr_code)
	{
		assert(_pstr_code);
	}

	script_code(const this_type& right)
		: _str_code(right._str_code),
			_pstr_code(right.is_inner_link()?
						boost::addressof(_str_code) : right._pstr_code)
	{
		assert(_pstr_code);
	}

	~script_code(void)
	{
	}

public:

	inline operator const string_type&(void) const
	{
		assert(_pstr_code);
		return *_pstr_code;
	}

	inline const string_type& code(void) const
	{
		assert(_pstr_code);
		return *_pstr_code;
	}

private:
	inline bool is_inner_link(void) const
	{
		return _pstr_code == boost::addressof(_str_code);
	}

private:
	string_type _str_code;
	const string_type* _pstr_code;
};

typedef script_code<tag_file_code> file_code;
typedef script_code<tag_string_code> string_code;

} // namespace script
} // namespace yggr

#endif // __YGGR_SCRIPT_CODE_HPP__
