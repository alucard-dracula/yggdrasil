// mongoc_client_encryption_opts_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_OPTS_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_OPTS_NATIVE_EX_HPP__

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/mongoc_mc_kms_credentials_callback_native_ex.hpp>

#include <memory>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_client_encryption_opts_t {
//   mongoc_client_t *keyvault_client;
//   char *keyvault_db;
//   char *keyvault_coll;
//   bson_t *kms_providers;
//   bson_t *tls_opts;
//   mc_kms_credentials_callback creds_cb;
//};

struct yggr_mongoc_client_encryption_opts
{
	mongoc_client_t *keyvault_client;
	char *keyvault_db;
	char *keyvault_coll;
	bson_t *kms_providers;
	bson_t *tls_opts;
	yggr_mc_kms_credentials_callback creds_cb;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class mongoc_client_encryption_opts_native_ex
{
public:
	typedef mongoc_client_encryption_opts_t org_client_encryption_opts_type;
	typedef yggr_mongoc_client_encryption_opts yggr_client_encryption_opts_type;

	typedef mongoc_kms_credentials_provider_callback_fn kms_credentials_provider_callback_fn_type;

private:
	typedef mongoc_client_encryption_opts_native_ex this_type;

public:
	inline static bool s_mongoc_client_encryption_opts_init(void* popts)
	{
		return popts && (memset(popts, 0, sizeof(yggr_client_encryption_opts_type)), true);
	}

	inline static void s_mongoc_client_encryption_opts_clear(void* popts)
	{
		this_type::s_mongoc_client_encryption_opts_init(popts);
	}

	static void s_mongoc_client_encryption_opts_org_clear(void* popts);

public:
	inline static bool s_mongoc_client_encryption_opts_set_kms_credential_provider_callback(void* popts,
																							kms_credentials_provider_callback_fn_type fn,
																							void* userdata)
	{
		return 
			popts 
			&& (mongoc_client_encryption_opts_set_kms_credential_provider_callback (
				reinterpret_cast<org_client_encryption_opts_type*>(popts),
				fn, userdata), true);
	}

	template<typename UserData> inline
	static bool s_mongoc_client_encryption_opts_set_kms_credential_provider_callback(void* popts,
																					kms_credentials_provider_callback_fn_type fn,
																					UserData& userdata)
	{
		return 
			popts
			&& (mongoc_client_encryption_opts_set_kms_credential_provider_callback (
				reinterpret_cast<org_client_encryption_opts_type*>(popts),
				fn, reinterpret_cast<void*>(boost::addressof(userdata)) ), true);
	}

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_OPTS_NATIVE_EX_HPP__


