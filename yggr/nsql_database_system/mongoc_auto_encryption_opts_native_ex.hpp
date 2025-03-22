//mongoc_auto_encryption_opts_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_AUTO_ENCRYPTION_OPTS_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_AUTO_ENCRYPTION_OPTS_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/detail/to_pointer.hpp>

#include <yggr/nsql_database_system/mongoc_mc_kms_credentials_callback_native_ex.hpp>

#include <boost/mpl/bool.hpp>

#include <memory>

namespace yggr
{
namespace nsql_database_system
{

struct yggr_mongoc_auto_encryption_opts 
{
	/* keyvault_client and keyvault_client_pool are not owned and must outlive
	* auto encrypted client/pool. */
	mongoc_client_t *keyvault_client;
	mongoc_client_pool_t *keyvault_client_pool;
	char *keyvault_db;
	char *keyvault_coll;
	bson_t *kms_providers;
	bson_t *tls_opts;
	bson_t *schema_map;
	bson_t *encrypted_fields_map;
	bool bypass_auto_encryption;
	bool bypass_query_analysis;
	yggr_mc_kms_credentials_callback creds_cb;
	bson_t *extra;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{


class mongoc_auto_encryption_opts_native_ex
{
public:
	typedef mongoc_auto_encryption_opts_t org_auto_encryption_opts_type;
	typedef yggr_mongoc_auto_encryption_opts yggr_auto_encryption_opts_type;

	typedef mongoc_kms_credentials_provider_callback_fn kms_credentials_provider_callback_fn_type;

private:
	typedef mongoc_auto_encryption_opts_native_ex this_type;

public:
	inline static bool s_mongoc_auto_encryption_opts_init(void* popts)
	{
		return popts && (memset(popts, 0, sizeof(yggr_auto_encryption_opts_type)), true);
	}

	inline static void s_mongoc_auto_encryption_opts_clear(void* popts)
	{
		this_type::s_mongoc_auto_encryption_opts_init(popts);
	}

	static void s_mongoc_auto_encryption_opts_org_clear(void* popts);
	
public:
	inline static bool s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback(void* popts,
																							kms_credentials_provider_callback_fn_type fn,
																							void* userdata)
	{
		return 
			popts 
			&& (mongoc_auto_encryption_opts_set_kms_credential_provider_callback(
				reinterpret_cast<org_auto_encryption_opts_type*>(popts),
				fn, userdata), true);
	}

	template<typename UserData> inline
	static bool s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback(void* popts,
																					kms_credentials_provider_callback_fn_type fn,
																					UserData& userdata)
	{
		return 
			popts
			&& (mongoc_auto_encryption_opts_set_kms_credential_provider_callback(
				reinterpret_cast<org_auto_encryption_opts_type*>(popts),
				fn, reinterpret_cast<void*>(boost::addressof(userdata)) ), true);
	}

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_AUTO_ENCRYPTION_OPTS_NATIVE_EX_HPP__
