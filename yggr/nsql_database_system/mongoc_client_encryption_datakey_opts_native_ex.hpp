// mongoc_client_encryption_datakey_opts_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_DATAKEY_OPTS_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_DATAKEY_OPTS_NATIVE_EX_HPP__

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <memory>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_client_encryption_datakey_opts_t {
//   bson_t *masterkey;
//   char **keyaltnames;
//   uint32_t keyaltnames_count;
//   uint8_t *keymaterial;
//   uint32_t keymaterial_len;
//};

struct yggr_mongoc_client_encryption_datakey_opts
{
	bson_t* masterkey;
	char** keyaltnames;
	uint32_t keyaltnames_count;
	uint8_t* keymaterial;
	uint32_t keymaterial_len;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class mongoc_client_encryption_datakey_opts_native_ex
{
public:
	typedef mongoc_client_encryption_datakey_opts_t org_client_encryption_datakey_opts_type;
	typedef yggr_mongoc_client_encryption_datakey_opts yggr_client_encryption_datakey_opts_type;

private:
	typedef mongoc_client_encryption_datakey_opts_native_ex this_type;

public:
	inline static bool s_mongoc_client_encryption_datakey_opts_init(void* popts)
	{
		return popts && (memset(popts, 0, sizeof(yggr_client_encryption_datakey_opts_type)), true);
	}

	inline static void s_mongoc_client_encryption_datakey_opts_clear(void* popts)
	{
		this_type::s_mongoc_client_encryption_datakey_opts_init(popts);
	}

	static void s_mongoc_client_encryption_datakey_opts_org_clear(void* popts);

protected:
	static void pro_s__clear_datakey_keyaltnames(void* popts);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_DATAKEY_OPTS_NATIVE_EX_HPP__


