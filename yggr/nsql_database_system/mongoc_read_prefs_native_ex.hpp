//mongoc_read_prefs_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_READ_PREFS_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_READ_PREFS_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/type_traits/number_t.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_read_prefs_t {
//   mongoc_read_mode_t mode;
//   bson_t tags;
//   int64_t max_staleness_seconds;
//   bson_t hedge;
//};

struct yggr_mongoc_read_prefs
{
	//mongoc_read_mode_t mode;
	YGGR_PP_UINTEGER_T_TYPE(sizeof(mongoc_read_mode_t)) mode;
	bson_t             tags;
	int64_t max_staleness_seconds;
	bson_t hedge;
};

class mongoc_read_prefs_native_ex
{
public:
	typedef YGGR_PP_UINTEGER_T_TYPE(sizeof(mongoc_read_mode_t)) mode_type;

private:
	typedef mongoc_read_prefs_native_ex this_type;

public:
	YGGR_STATIC_CONSTANT(s64, E_MONGOC_NO_MAX_STALENESS = -1); // #define MONGOC_NO_MAX_STALENESS -1
	YGGR_STATIC_CONSTANT(s64, E_MONGOC_SMALLEST_MAX_STALENESS_SECONDS = 90); // #define MONGOC_SMALLEST_MAX_STALENESS_SECONDS 90

public:
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_INVALIDATE = 0); // if destroy mode is E_MONGOC_READ_INVALIDATE
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_PRIMARY = MONGOC_READ_PRIMARY);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_SECONDARY = MONGOC_READ_SECONDARY);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_PRIMARY_PREFERRED = MONGOC_READ_PRIMARY_PREFERRED);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_SECONDARY_PREFERRED = MONGOC_READ_SECONDARY_PREFERRED);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_NEAREST = MONGOC_READ_NEAREST);

public:
	static void* s_mongoc_read_prefs_init(void* pmrp, mode_type mode = this_type::E_MONGOC_READ_PRIMARY);
	static bool s_mongoc_read_prefs_static_destroy(void* pmrp);
	static void* s_mongoc_read_prefs_move(void* pdst, void* psrc);
	static void* s_mongoc_read_prefs_copy(void* pdst, const void* psrc);
	static void s_mongoc_read_prefs_swap(void* pl, void* pr);

	static bool s_mongoc_read_prefs_add_tag(void* pmrp, const bson_t* tag);

	static bool s_mongoc_read_prefs_is_valid(const void* pmrp);

	static u32 s_mongoc_read_prefs_get_mode(const void* pmrp);
	static bool s_mongoc_read_prefs_set_mode(void* pmrp, mode_type mode);

	static const bson_t* s_mongoc_read_prefs_get_tags(const void* pmrp);
	static bool s_mongoc_read_prefs_set_tags(void* pmrp, const bson_t* ptags);

	static s64 s_mongoc_read_prefs_get_max_staleness_seconds(const void* pmrp);
	static bool s_mongoc_read_prefs_set_max_staleness_seconds(void* pmrp, s64 max_staleness_seconds);

	static const bson_t* s_mongoc_read_prefs_get_hedge(const void* pmrp);
	static bool s_mongoc_read_prefs_set_hedge(void* pmrp, const bson_t* phedge);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_READ_PREFS_NATIVE_EX_HPP__