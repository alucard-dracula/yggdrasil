//md5_tool.hpp

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

#ifndef __YGGR_ENCRYPTION_TOOL_MD5_TOOL_HPP__
#define __YGGR_ENCRYPTION_TOOL_MD5_TOOL_HPP__

//#include <iostream>
#include <md5.h>
#include <memory>

#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>

namespace yggr
{
namespace encryption_tool
{

class md5_tool
{
public:
	enum
	{
		E_MD5_SIZE = MD5_SIZE,
		//E_MD5_SIZE = 16,
		E_MD5_STRING_LEAST_SIZE = 33,

		E_compile_size_u32 = 0xffffffff
	};

	enum
	{
		E_TYPE_SIG,
		E_TYPE_CODE,
		E_TYPE_BUF,

		E_compile_type_u32 = 0xffffffff
	};

	typedef boost::array<char, E_MD5_SIZE> sig_type;
	typedef sig_type check_type;

private:
	typedef md5_tool this_type;

public:
	md5_tool(void)
	{
	}
	
	~md5_tool(void)
	{
	}

	inline size_type md5_size(void) const
	{
		return E_MD5_SIZE;
	}

	inline size_type sig_size(void) const
	{
		return E_MD5_SIZE;
	}

	inline size_type check_size(void) const
	{
		return E_MD5_SIZE;
	}

	inline static size_type s_check_size(void)
	{
		return E_MD5_SIZE;
	}

	inline static size_type s_sig_size(void)
	{
		return E_MD5_SIZE;
	}

	inline static u32 s_type_sig(void)
	{
		return E_TYPE_SIG;
	}

	inline static u32 s_type_buf(void)
	{
		return E_TYPE_BUF;
	}

	inline static u32 s_type_code(void)
	{
		return E_TYPE_CODE;
	}

	template<typename Code>
	static Code s_sig_to_code(const sig_type& sig)
	{
		typedef Code code_type;
		typedef typename code_type::value_type code_value_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<code_type>));

		yggr::string tcode(E_MD5_STRING_LEAST_SIZE, 0);
		md5_sig_to_string((void*)&sig[0], &tcode[0], E_MD5_STRING_LEAST_SIZE);

		return YGGR_PP_CHARSET_FOO_XCHG(code_type, tcode, 
											charset::charset_name_t<yggr::string::value_type>(),
											charset::charset_name_t<code_value_type>());

	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static sig_type s_code_to_sig(const Basic_String<Char, Traits, Alloc>& code)
	{
		typedef Basic_String<Char, Traits, Alloc> code_type;

		BOOST_MPL_ASSERT((charset::is_not_utf8_string<code_type>));

		sig_type sig = {0};
		if(code.empty())
		{
			return sig;
		}

		yggr::string tcode(YGGR_PP_CHARSET_FOO_XCHG(yggr::string, code, 
														charset::charset_name_t<Char>(),
														charset::charset_name_t<yggr::string::value_type>()));

		yggr::string use_code(E_MD5_STRING_LEAST_SIZE, 0);
		memcpy(&use_code[0], &tcode[0], sizeof(yggr::string::value_type) * tcode.size());

		md5_sig_from_string(&sig[0], &tcode[0]);
		return sig;
	}

	template<typename Code>
	Code sig_to_code(const sig_type& sig) const
	{
		return this_type::s_sig_to_code<Code>(sig);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	sig_type code_to_sig(const Basic_String<Char, Traits, Alloc>& code) const
	{
		return this_type::s_code_to_sig<Char, Traits, Alloc, Basic_String>(code);
	}

	template<typename Buffer>
	sig_type encrypt(const Buffer& buf) const
	{
		typedef Buffer buf_type;
		typedef typename buf_type::value_type buf_val_type;

		sig_type sig = {0};
		md5_buffer((const char*)&buf[0], buf.size() * sizeof(buf_val_type), &sig[0]);
		return sig;
	}
};


} // namespace encryption_tool
} // namespace yggr

#endif //__YGGR_ENCRYPTION_TOOL_MD5_TOOL_HPP__
