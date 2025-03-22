//log_mongodb_op_caller

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

#include <yggr/log/log_mongodb_op_caller.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace log
{

log_mongodb_op_caller::log_mongodb_op_caller(void)
	: _str_db((inner_string_type(this_type::unknown_log_db_name(), 
									YGGR_STR_UTF8_STRING_CHARSET_NAME()))),
		_str_coln((inner_string_type(this_type::unknown_log_coln_name(), 
										YGGR_STR_UTF8_STRING_CHARSET_NAME()))),
		_str_db_coln((inner_string_type(this_type::unknown_log_name(), 
											YGGR_STR_UTF8_STRING_CHARSET_NAME()))),
		_append_opts(bson_type()),
		_search_opts(bson_type()),
		_search_read_prefs(read_prefs_type()),
		_clear_flags(static_cast<u32>(0)),
		_clear_opts(bson_type())
{
}

log_mongodb_op_caller::log_mongodb_op_caller(const this_type& right)
	: _str_db(right._str_db),
		_str_coln(right._str_coln),
		_str_db_coln(right._str_db_coln),
		_append_opts(right._append_opts),
		_search_opts(right._search_opts),
		_search_read_prefs(right._search_read_prefs),
		_clear_flags(right._clear_flags),
		_clear_opts(right._clear_opts)
{
}

log_mongodb_op_caller::~log_mongodb_op_caller(void)
{
}

log_mongodb_op_caller::this_type& 
	log_mongodb_op_caller::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_str_db = right._str_db;
	_str_coln = right._str_coln;
	_str_db_coln = right._str_db_coln;

	_append_opts = right._append_opts;
	
	_search_opts = right._search_opts;
	_search_read_prefs = right._search_read_prefs;

	_clear_flags = right._clear_flags;
	_clear_opts = right._clear_opts;

	return *this;
}

void log_mongodb_op_caller::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	_str_db.swap(right._str_db);
	_str_coln.swap(right._str_coln);
	_str_db_coln.swap(right._str_db_coln);

	_append_opts.swap(right._append_opts);

	_search_opts.swap(right._search_opts);
	_search_read_prefs.swap(right._search_read_prefs);

	_clear_flags.swap(right._clear_flags);
	_clear_opts.swap(right._clear_opts);
}

} // namespace log
} // namespace yggr

