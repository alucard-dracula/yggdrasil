//mongo_accesser_types_decl.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_TYPES_DECL_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_TYPES_DECL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <yggr/container/resize.hpp>
#include <yggr/type_traits/native_t.hpp>
#include <yggr/type_traits/original_t.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>

#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>
#include <yggr/nsql_database_system/c_mongo_read_prefs.hpp>
#include <yggr/nsql_database_system/c_mongo_index_model.hpp>
#include <yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp>
#include <yggr/nsql_database_system/c_mongo_read_concern.hpp>

#include <yggr/nsql_database_system/mongoc_native_err_ex.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/function.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_reference.hpp>

#include <boost/mpl/if.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <cassert>

namespace yggr
{
namespace nsql_database_system
{

struct mongo_accesser_types_decl
{
public:
	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

	typedef c_bson bson_type;
	typedef bson_type::org_type org_bson_type;

	typedef c_bson_value bson_value_type;
	typedef bson_value_type::org_type org_bson_value_type;

	typedef c_bson_error bson_error_type;
	typedef bson_error_type::org_type org_bson_error_type;

	typedef c_mongo_write_concern write_concern_type;
	typedef write_concern_type::org_type org_write_concern_type;

	typedef c_mongo_read_prefs read_prefs_type;
	typedef read_prefs_type::org_type org_read_prefs_type;

	typedef c_mongo_index_model index_model_type;
	typedef index_model_type::org_type org_index_model_type;

	typedef c_mongo_find_and_modify_opts find_and_modify_opts_type;
	typedef find_and_modify_opts_type::org_type org_find_and_modify_opts_type;

	typedef c_mongo_read_concern read_concern_type;
	typedef read_concern_type::org_type org_read_concern_type;

	typedef mongoc_error_domain_ex_t mongoc_error_domain_type;
	typedef mongoc_error_code_ex_t mongoc_error_code_type;

private:
	typedef mongo_accesser_types_decl this_type;
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_TYPES_DECL_HPP__

