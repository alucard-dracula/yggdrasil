// mongoc_client_encryption_encrypt_range_opts_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{

#if YGGR_MONGOC_HEX() < 0x011C0000

//struct _mongoc_client_encryption_encrypt_range_opts_t {
//   struct {
//      bson_value_t value;
//      bool set;
//   } min;
//   struct {
//      bson_value_t value;
//      bool set;
//   } max;
//   int64_t sparsity;
//   struct {
//      int32_t value;
//      bool set;
//   } precision;
//};

struct yggr_mongoc_client_encryption_encrypt_range_opts
{
public:
	typedef struct 
	{
		bson_value_t value;
		bool set;
	} min_max_type;

	typedef struct 
	{
		int32_t value;
		bool set;
	} precision_type;

	min_max_type min;
	min_max_type max;
	int64_t sparsity;
	precision_type precision;
};

#else

//struct _mongoc_client_encryption_encrypt_range_opts_t {
//   struct {
//      bson_value_t value;
//      bool set;
//   } min;
//   struct {
//      bson_value_t value;
//      bool set;
//   } max;
//   struct {
//      int32_t value;
//      bool set;
//   } trim_factor;
//   struct {
//      int64_t value;
//      bool set;
//   } sparsity;
//   struct {
//      int32_t value;
//      bool set;
//   } precision;
//};

struct yggr_mongoc_client_encryption_encrypt_range_opts 
{
	typedef struct 
	{
		bson_value_t value;
		bool set;
	} min_max_type;

	typedef struct 
	{
		int32_t value;
		bool set;
	} int32_val_type;
	typedef int32_val_type trim_factor_type;
	typedef int32_val_type precision_type;

	typedef struct 
	{
		int64_t value;
		bool set;
	} sparsity_type;

	min_max_type min;
	min_max_type max;
	trim_factor_type trim_factor;
	sparsity_type sparsity;
	precision_type precision;
};

#endif // #if YGGR_MONGOC_HEX() < 0x011C0000

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class mongoc_client_encryption_encrypt_range_opts_native_ex
{
public:
	typedef mongoc_client_encryption_encrypt_range_opts_t org_client_encryption_encrypt_range_opts_type;
	typedef yggr_mongoc_client_encryption_encrypt_range_opts yggr_client_encryption_encrypt_range_opts_type;

private:
	typedef mongoc_client_encryption_encrypt_range_opts_native_ex this_type;
	
public:
	inline static bool s_mongoc_client_encryption_encrypt_range_opts_init(void* popts)
	{
		return popts && (memset(popts, 0, sizeof(yggr_client_encryption_encrypt_range_opts_type)), true);
	}

	static void s_mongoc_client_encryption_encrypt_range_opts_clear(void* popts);
	static bool s_mongoc_client_encryption_encrypt_range_opts_copy(void* pdst_opts, const void* psrc_opts);
	static void s_mongoc_client_encryption_encrypt_range_opts_swap(void* pdst_opts, void* psrc_opts);
	static void s_mongoc_client_encryption_encrypt_range_opts_move(void* pdst_opts, void* psrc_opts);
};

} // namespace nsql_database_system
} // namespace yggr


#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_ENCRYPT_RANGE_OPTS_NATIVE_EX_HPP__
