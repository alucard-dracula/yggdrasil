//charset_foo.hpp

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

#ifndef __YGGR_CHARSET_CHARSET_BASE_FOO_HPP__
#define __YGGR_CHARSET_CHARSET_BASE_FOO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncreateable.hpp>
//#include <utility>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace charset
{

class charset_base_foo : private nonable::noncreateable
{
private:
	typedef charset_base_foo this_type;
public:
	
	static size_type xchglen(const char* str);
	static size_type xchglen(const wchar_t* str);

	template<typename String>
	static size_type xchglen(const String& str)
	{
		return str.size() * 4;
	}

	template<typename String>
	static String& nptr_rst_norm_lenth(String& str)
	{
		typedef String string_type;
		string_type tstr(str.c_str());
		std::swap(tstr, str);
		return str;
	}
};

} // namespace charset
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_CHARSET_BASE_FOO_HPP__