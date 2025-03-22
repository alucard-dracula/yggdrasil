//database_charset_config.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_CHARSET_CONFIG_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_CHARSET_CONFIG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/database_system/dtl_include_helper.hpp>

#ifndef TAG_YGGR_DATABASE_CHARSET_NAME
#	define TAG_YGGR_DATABASE_CHARSET_NAME TAG_YGGR_UTF8_STRING_CHARSET_NAME
#endif // TAG_YGGR_DATABASE_CHARSET_NAME

namespace yggr
{
namespace database_system
{

class database_charset_config
{

private:
	typedef database_charset_config this_type;

public:
	inline static const char* get_default_database_charset_name(void)
	{
		return
			0 == strcmp(TAG_YGGR_DATABASE_CHARSET_NAME(), TAG_YGGR_UTF8_STRING_CHARSET_NAME())?
				charset::default_charset_config::get_inner_string_charset_name()
				: TAG_YGGR_DATABASE_CHARSET_NAME();
	}

	inline static const string& get_str_default_database_charset_name(void)
	{
		static const string ret(TAG_YGGR_DATABASE_CHARSET_NAME());
		return 
			0 == strcmp(TAG_YGGR_DATABASE_CHARSET_NAME(), TAG_YGGR_UTF8_STRING_CHARSET_NAME())?
				charset::default_charset_config::get_str_inner_string_charset_name() : ret;
	}
};

} // namespace database_system
} // namespace yggr

#define YGGR_DATABASE_CHARSET_NAME() \
	::yggr::database_system::database_charset_config::get_default_database_charset_name()

#define YGGR_STR_DATABASE_CHARSET_NAME() \
	::yggr::database_system::database_charset_config::get_str_default_database_charset_name()

#endif // __YGGR_DATABASE_SYSTEM_DATABASE_CHARSET_CONFIG_HPP__
