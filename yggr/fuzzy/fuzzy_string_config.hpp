//fuzzy_string_config.hpp

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

#ifndef __YGGR_FUZZY_FUZZY_STRING_CONFIG_HPP__
#define __YGGR_FUZZY_FUZZY_STRING_CONFIG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/charset/const_strings.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4624)
#endif // BOOST_MSVC

namespace yggr
{
namespace fuzzy
{

struct fuzzy_string_config
	: private nonable::noncreateable
{
private:
	typedef fuzzy_string_config this_type;

public:
	template<typename Char> inline
	static const Char* s_fmt(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(str_fmt, "(?1.*)(?2.*)(?3.*)");
		return YGGR_PP_CONST_STRING_LOCAL_GET(str_fmt, Char);
	}

	template<typename Char> inline
	static const Char* s_str_re(void)
	{
		//YGGR_PP_CONST_STRING_LOCAL_DEF(str_regex, "(^)\\W*|\\W+$|($)|(\\W+)");
		YGGR_PP_CONST_STRING_LOCAL_DEF(str_regex, "(^)[\\s`~!@#\\$%\\^&\\*\\-\\+\\\\\\|:;'\\\"<>,\\.?/]*|[\\s`~!@#\\$%\\^&\\*\\-\\+\\\\\\|:;'\\\"<>,\\.\\?/]+$|($)|([\\s`~!@#\\$%\\^&\\*\\-\\+\\\\\\|:;'\\\"<>,\\.\\?/]+)");
		return YGGR_PP_CONST_STRING_LOCAL_GET(str_regex, Char);
	}
};

} // namespace fuzzy
} // namesapce yggr

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

#endif //__YGGR_FUZZY_FUZZY_STRING_CONFIG_HPP__
