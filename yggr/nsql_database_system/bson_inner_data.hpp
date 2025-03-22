//bson_inner_data.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/nsql_database_system/basic_bson_inner_data.hpp>

#include <yggr/nsql_database_system/c_bson_null.hpp>
#include <yggr/nsql_database_system/c_bson_undefined.hpp>

#include <yggr/nsql_database_system/c_bson_minkey.hpp>
#include <yggr/nsql_database_system/c_bson_maxkey.hpp>

#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_bson_time.hpp>
#include <yggr/nsql_database_system/c_bson_timeval.hpp>

#include <yggr/nsql_database_system/c_bson_oid.hpp>

#include <yggr/nsql_database_system/c_bson_binary_buffer.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer_ref.hpp>

#include <yggr/nsql_database_system/c_bson_regex.hpp>
#include <yggr/nsql_database_system/c_bson_code.hpp>
#include <yggr/nsql_database_system/c_bson_symbol.hpp>
#include <yggr/nsql_database_system/c_bson_code_w_scope.hpp>

#include <yggr/nsql_database_system/c_bson_timestamp.hpp>
#include <yggr/nsql_database_system/c_bson_decimal128.hpp>
#include <yggr/nsql_database_system/c_bson_dbpointer.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>

#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace nsql_database_system
{
} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_INNER_DATA_HPP__