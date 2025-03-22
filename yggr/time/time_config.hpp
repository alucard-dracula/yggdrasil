//time_config.hpp

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

#ifndef __YGGR_TIME_TIME_CONFIG_HPP__
#define __YGGR_TIME_TIME_CONFIG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/time/time_formatting.hpp>

namespace yggr
{
namespace time
{

struct time_config
{
	struct tag_format{};
	struct tag_charset{};

	YGGR_STATIC_CONSTANT(std::size_t, limit_format_length = 128);
	YGGR_STATIC_CONSTANT(std::size_t, default_string_buffer_length = 255);

	YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC
	const char* default_time_format(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return time_format::tm_fmt_default();
	}

	template<typename Char> YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC
	const Char* default_time_format(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return time_format::tm_fmt_default<Char>();
	}
};

} // nmamespace time
} // namespace yggr

#endif // __YGGR_TIME_TIME_CONFIG_HPP__
