//charset_converter.cpp

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include <yggr/charset/charset_converter.hpp>
#include <cassert>

namespace yggr
{
namespace charset
{

//-------------------------------------------------------------------------

charset_converter::charset_converter(const char* src_charset, const char* dst_charset)
	: _cd((iconv_t)-1)
{
	_cd = iconv_open(dst_charset, src_charset);

	assert(_cd != (iconv_t) -1);
}


charset_converter::~charset_converter(void)
{
	if(_cd == (iconv_t)-1)
	{
		return;
	}
	iconv_close(_cd);
}

bool charset_converter::operator()(const char* input, size_t ilen, char* output, size_t olen) const
{
	if(_cd == (iconv_t)-1)
	{
		return false;
	}

	if(!olen)
	{
		return true;
	}
#if _LIBICONV_VERSION <= 0x0109
	const char **pin = &input;
#else
	char **pin = const_cast<char**>(&input);
#endif //_LIBICONV_VERSION

	char **pout = &output;
	memset(output, 0, sizeof(char) * olen);

	if(-1 == iconv(_cd, pin, &ilen, pout, &olen))
	{
		return false;
	}

	return true;
}


} // charset
} // namespace yggr
