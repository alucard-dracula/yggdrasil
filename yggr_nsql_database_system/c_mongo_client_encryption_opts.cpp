//c_mongo_client_encryption_opts.cpp

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

#include <yggr/nsql_database_system/c_mongo_client_encryption_opts.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{


//public:
c_mongo_client_encryption_opts::c_mongo_client_encryption_opts(void)
{
	this_type::pro_init();
}

c_mongo_client_encryption_opts::c_mongo_client_encryption_opts(const org_type* ptr)
{
	this_type::pro_init();
	this_type::pro_copy(ptr);
}

c_mongo_client_encryption_opts::c_mongo_client_encryption_opts(const org_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_opts::c_mongo_client_encryption_opts(const this_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(right);
}

c_mongo_client_encryption_opts::~c_mongo_client_encryption_opts(void)
{
}

//public:
c_mongo_client_encryption_opts::this_type& 
	c_mongo_client_encryption_opts::operator=(const org_type& right)
{
	return this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_opts::this_type& 
	c_mongo_client_encryption_opts::operator=(const base_type& right)
{
	return this_type::pro_copy(reinterpret_cast<const org_type*>(boost::addressof(right)));
}

c_mongo_client_encryption_opts::this_type& 
	c_mongo_client_encryption_opts::operator=(const this_type& right)
{
	return this_type::pro_copy(right);
}

//public:
void c_mongo_client_encryption_opts::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:
void c_mongo_client_encryption_opts::pro_init(void)
{
	base_type& base = *this;
	mongoc_client_encryption_opts_native_ex::s_mongoc_client_encryption_opts_init(boost::addressof(base));
}

void c_mongo_client_encryption_opts::pro_rebuild_base(void)
{
	if(_keyvault_db.size())
	{
		base_type::keyvault_db = _keyvault_db.data();
	}

	if(_keyvault_coll.size())
	{
		base_type::keyvault_coll = _keyvault_coll.data();
	}

	if(!_kms_providers.empty())
	{
		base_type::kms_providers = _kms_providers.org_pointer();
	}

	if(!_tls_opts.empty())
	{
		base_type::tls_opts = _tls_opts.org_pointer();
	}
}

void c_mongo_client_encryption_opts::pro_clear(void)
{
	mongoc_client_encryption_opts_native_ex::s_mongoc_client_encryption_opts_clear(static_cast<base_type*>(this));
	_keyvault_db.clear();
	_keyvault_coll.clear();
	_kms_providers.clear();
	_tls_opts.clear();
}

c_mongo_client_encryption_opts::this_type& 
	c_mongo_client_encryption_opts::pro_copy(const org_type* ptr)
{
	if(!ptr || ptr == reinterpret_cast<org_type*>(this))
	{
		return *this;
	}

	const base_type* prbase = reinterpret_cast<const base_type*>(ptr);

	base_type::keyvault_client = prbase->keyvault_client;

	if(prbase->keyvault_db)
	{
		_keyvault_db.assign(prbase->keyvault_db);
		base_type::keyvault_db = _keyvault_db.data();
	}

	if(prbase->keyvault_coll)
	{
		_keyvault_coll.assign(prbase->keyvault_coll);
		base_type::keyvault_coll = _keyvault_coll.data();
	}

	if(prbase->kms_providers)
	{
		_kms_providers = *(prbase->kms_providers);
		base_type::kms_providers = _kms_providers.org_pointer();
	}

	if(prbase->tls_opts)
	{
		_tls_opts = *(prbase->tls_opts);
		base_type::tls_opts = _tls_opts.org_pointer();
	}

	base_type::creds_cb = prbase->creds_cb;

	return *this;
}

c_mongo_client_encryption_opts::this_type& 
	c_mongo_client_encryption_opts::pro_copy(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	base_type::keyvault_client = right.keyvault_client;

	_keyvault_db = right._keyvault_db;
	_keyvault_coll = right._keyvault_coll;

	_kms_providers = right._kms_providers;
	_tls_opts = right._tls_opts;

	base_type::creds_cb = right.base_type::creds_cb;

	this_type::pro_rebuild_base();

	return *this;
}

void c_mongo_client_encryption_opts::pro_swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	::yggr::swap(base_type::keyvault_client, right.keyvault_client);

	_keyvault_db.swap(right._keyvault_db);
	_keyvault_coll.swap(right._keyvault_coll);

	_kms_providers.swap(right._kms_providers);
	_tls_opts.swap(right._tls_opts);
	
	::yggr::swap(base_type::creds_cb, right.creds_cb);

	this_type::pro_rebuild_base();
	right.pro_rebuild_base();
}

c_mongo_client_encryption_opts::this_type& 
	c_mongo_client_encryption_opts::pro_move(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	base_type::keyvault_client = right.keyvault_client;

	_keyvault_db = boost::move(right._keyvault_db);
	_keyvault_coll = boost::move(right._keyvault_coll);

	_kms_providers = boost::move(right._kms_providers);
	_tls_opts = boost::move(right._tls_opts);

	base_type::creds_cb = right.creds_cb;

	this_type::pro_rebuild_base();

	return *this;
}

} // namespace nsql_database_system
} // namespace yggr
