// bson_value_conflict_fixer.cpp

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

#include <yggr/nsql_database_system/detail/bson_value_conflict_fixer.hpp>
#include <yggr/nsql_database_system/bson_typeid_def.hpp>
#include <yggr/nsql_database_system/bson_error_native_ex.hpp>
#include <yggr/math/sign.hpp>

#include <boost/ref.hpp>

#include <memory>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

/*static*/ 
bool bson_value_conflict_fixer::s_value_cmp_eq_oid(const bson_value_t* l, const bson_oid_t* r)
{
	assert(l && r);
	return (l) && (r)
			&& (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_OID)
			&& (0 == memcmp(boost::addressof(l->value.v_oid), r, sizeof(bson_oid_t)));
}

/*static*/ 
s32 bson_value_conflict_fixer::s_value_cmp_oid(const bson_value_t* l, const bson_oid_t* r)
{
	assert(l && r && (static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_OID));

	if(!(static_cast<u32>(l->value_type) == bson_typeid_def::E_BSON_TYPE_OID))
	{
		bson_error_t bs_err;
		bson_error_native_ex::s_bson_error_init(
			boost::addressof(bs_err), 
			BSON_ERROR_INVALID,
			bson_error_code_ex_def::BSON_VALIDATE_DIFFERENT_TYPE,
			bson_error_code_ex_def::msg_bson_validate_different_type(),
			l->value_type, bson_typeid_def::E_BSON_TYPE_OID);
		throw bs_err;
	}
	return math::sign(memcmp(boost::addressof(l->value.v_oid), r, sizeof(bson_oid_t)));
}

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

