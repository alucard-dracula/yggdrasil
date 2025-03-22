//log_sql_db_op_caller.cpp

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

#include <yggr/log/log_sql_db_op_caller.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace log
{

log_sql_db_op_caller::log_sql_db_op_caller(void)
	: _db_names((org_string_type(this_type::unknown_log_name()))),
		_bcommit(false),
		_brollback(false)
{
}

log_sql_db_op_caller::log_sql_db_op_caller(const org_string_type& db_names,
											bool bcommit, bool brollback)
	: _db_names(db_names),
		_bcommit(bcommit),
		_brollback(brollback)
{
}

log_sql_db_op_caller::log_sql_db_op_caller(const this_type& right)
	: _db_names(right._db_names),
		_bcommit(right._bcommit),
		_brollback(right._brollback)
{
}

log_sql_db_op_caller::~log_sql_db_op_caller(void)
{
}

log_sql_db_op_caller::this_type& 
	log_sql_db_op_caller::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_db_names = right._db_names;
	_bcommit = right._bcommit;
	_brollback = right._brollback;

	return *this;
}

void log_sql_db_op_caller::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	_db_names.swap(right._db_names);
	_bcommit.swap(right._bcommit);
	_brollback.swap(right._brollback);
}

} // namespace log
} // namespace yggr

