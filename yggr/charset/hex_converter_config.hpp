// hex_converter_config.hpp

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

#ifndef __YGGR_CHARSET_HEX_CONVERTER_CONFIG_HPP__
#define __YGGR_CHARSET_HEX_CONVERTER_CONFIG_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace charset
{

struct hex_converter_config
{
public:
	YGGR_STATIC_CONSTANT(u32, E_Mode_Low = 0);
	YGGR_STATIC_CONSTANT(u32, E_Mode_Upper = 1);

#if (!defined(YGGR_HEX_CONV_DEFAULT_MODE)) || (!(YGGR_HEX_CONV_DEFAULT_MODE < 2))
	YGGR_STATIC_CONSTANT(u32, E_Mode_Default = E_Mode_Low);
#else
	YGGR_STATIC_CONSTANT(u32, E_Mode_Default = YGGR_HEX_CONV_DEFAULT_MODE);
#endif // YGGR_HEX_CONV_DEFAULT_MODE

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE static const char* s_hex_to_char_table(u32 mode = E_Mode_Default)
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC const char* const tabs[2] = {"0123456789abcdef", "0123456789ABCDEF"};
		assert(mode < 2);
		return tabs[mode];
	}

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_CHARSET_HEX_CONVERTER_CONFIG_HPP__
