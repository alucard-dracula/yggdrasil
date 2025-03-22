//mongoc_server_description_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_SERVER_DESCRIPTION_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_SERVER_DESCRIPTION_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/type_traits/number_t.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

//typedef enum {
//   MONGOC_SERVER_UNKNOWN,
//   MONGOC_SERVER_STANDALONE,
//   MONGOC_SERVER_MONGOS,
//   MONGOC_SERVER_POSSIBLE_PRIMARY,
//   MONGOC_SERVER_RS_PRIMARY,
//   MONGOC_SERVER_RS_SECONDARY,
//   MONGOC_SERVER_RS_ARBITER,
//   MONGOC_SERVER_RS_OTHER,
//   MONGOC_SERVER_RS_GHOST,
//   MONGOC_SERVER_LOAD_BALANCER,
//   MONGOC_SERVER_DESCRIPTION_TYPES,
//} mongoc_server_description_type_t;

enum mongoc_server_description_type_t_mark
{
	E_MARK_1,
	E_MARK_2
};

} // namespace detail
} // namespcae nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class mongoc_server_description_native_ex
{
public:
	typedef YGGR_PP_UINTEGER_T_TYPE(sizeof(detail::mongoc_server_description_type_t_mark)) typeid_type;

private:
	typedef mongoc_server_description_native_ex this_type;

public:
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_UNKNOWN = 0);			// MONGOC_SERVER_UNKNOWN
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_STANDALONE = 1);			// MONGOC_SERVER_STANDALONE
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_MONGOS = 2);				// MONGOC_SERVER_MONGOS
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_POSSIBLE_PRIMARY = 3);	// MONGOC_SERVER_POSSIBLE_PRIMARY
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_RS_PRIMARY = 4);			// MONGOC_SERVER_RS_PRIMARY
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_RS_SECONDARY = 5);		// MONGOC_SERVER_RS_SECONDARY
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_RS_ARBITER = 6);			// MONGOC_SERVER_RS_ARBITER
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_RS_OTHER = 7);			// MONGOC_SERVER_RS_OTHER
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_RS_GHOST = 8);			// MONGOC_SERVER_RS_GHOST
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_LOAD_BALANCER = 9);		// MONGOC_SERVER_LOAD_BALANCER
	YGGR_STATIC_CONSTANT(typeid_type, E_MONGOC_SERVER_DESCRIPTION_TYPES = 10);	// MONGOC_SERVER_DESCRIPTION_TYPES

public:
	static typeid_type s_typeid(const mongoc_server_description_t* pdesc);
	static void s_destroy_all_exclude_outter_array(mongoc_server_description_t **sds, std::size_t n);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_SERVER_DESCRIPTION_NATIVE_EX_HPP__