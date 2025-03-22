// bson_value_conflict_fixer.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BSON_VALUE_CONFILICT_FIXER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BSON_VALUE_CONFILICT_FIXER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/bson_config.hpp>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

class bson_value_conflict_fixer
{
public:
private:
	typedef bson_value_conflict_fixer this_type;

public:
	static bool s_value_cmp_eq_oid(const bson_value_t* l, const bson_oid_t* r);
	static s32 s_value_cmp_oid(const bson_value_t* l, const bson_oid_t* r);
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BSON_VALUE_CONFILICT_FIXER_HPP__
