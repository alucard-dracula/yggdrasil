//charset_converter.hpp

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

#ifndef __YGGR_CHARSET_CHARSET_CONVERTER_HPP__
#define __YGGR_CHARSET_CHARSET_CONVERTER_HPP__

#include <iconv.h>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/charset_base_foo.hpp>


namespace yggr
{
namespace charset
{

class charset_converter : private nonable::noncopyable
{
private:
	typedef charset_converter this_type;

//-------------------------------------------------------------------------
public:
	charset_converter(const char* src_charset, const char* dst_charset);

	charset_converter(const string& src_charset, const string& dst_charset)
		: _cd((iconv_t) -1)
	{
		_cd = iconv_open(dst_charset.c_str(), src_charset.c_str());

		assert(_cd != (iconv_t) -1);
	}

	~charset_converter(void);

	template<typename InString, typename OutString>
	bool operator()(const InString& input, OutString& output) const
	{
		typedef InString in_string_type;
		typedef OutString out_string_type;
		size_t olen = charset_base_foo::xchglen(input);
		return this_type::operator()(input, output, olen);
	}

	template<typename InString, typename OutString>
	bool operator()(const InString& input, OutString& output, size_t olen) const
	{
		typedef InString in_string_type;
		typedef OutString out_string_type;
		typedef typename in_string_type::value_type val_type;

		if(!olen)
		{
			return true;
		}
		out_string_type str(olen, 0);
		if(this_type::operator()((const char*)(&input[0]), input.size() * sizeof(val_type), (char*)&str[0], olen))
		{
			output.swap(str);
			return true;
		}

		return false;
	}

	bool operator()(const char* input, size_t ilen, char* output, size_t olen) const;

private:
	iconv_t _cd;
};

} // charset
} // namespace yggr

#endif //__YGGR_CHARSET_CHARSET_CONVERTER_HPP__
