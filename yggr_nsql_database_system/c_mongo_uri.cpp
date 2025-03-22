//c_mongo_uri.cpp

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

#include <yggr/nsql_database_system/c_mongo_uri.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/mongoc_host_list_native_ex.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_uri::c_mongo_uri(void)
{
	base_type tmp_base = {0};

	if(mongoc_uri_native_ex::s_mongoc_uri_init(&tmp_base, 0))
	{
		this_type::prv_init(tmp_base);
	}
	else
	{
		this_type::prv_init_default();
	}

	mongoc_uri_native_ex::s_mongoc_uri_clear(&tmp_base);
}

c_mongo_uri::c_mongo_uri(bson_error_type& outerr)
{
	base_type tmp_base = {0};

	if(mongoc_uri_native_ex::s_mongoc_uri_init(
		&tmp_base, boost::addressof(outerr)))
	{
		this_type::prv_init(tmp_base);
	}
	else
	{
		this_type::prv_init_default();
	}

	mongoc_uri_native_ex::s_mongoc_uri_clear(&tmp_base);
}

c_mongo_uri::c_mongo_uri(const org_type* right)
{
	this_type::prv_init_default();
	if(right)
	{
		this_type::pro_copy(reinterpret_cast<const base_type&>(*right));
	}
}

c_mongo_uri::c_mongo_uri(const org_type& right)
{
	this_type::prv_init_default();
	this_type::pro_copy(reinterpret_cast<const base_type&>(right));
}

c_mongo_uri::c_mongo_uri(const this_type& right)
{
	this_type::prv_init_default();
	this_type::pro_copy(right);
}

c_mongo_uri::~c_mongo_uri(void)
{
	this_type::prv_destroy();
}

c_mongo_uri::this_type& c_mongo_uri::operator=(const org_type& right)
{
	return this_type::pro_copy(reinterpret_cast<const base_type&>(right));
}

c_mongo_uri::this_type& c_mongo_uri::operator=(const this_type& right)
{
	return this_type::pro_copy(right);
}

void c_mongo_uri::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:

c_mongo_uri::this_type& c_mongo_uri::pro_copy(const base_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	_str.assign(right.str? right.str : "", YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_username.assign(right.username? right.username : "" , YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_password.assign(right.password? right.password : "" , YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_database.assign(right.database? right.database : "" , YGGR_STR_UTF8_STRING_CHARSET_NAME());
	
	if(right.read_prefs)
	{
		_read_prefs = *right.read_prefs;
	}

	if(right.read_concern)
	{
		_read_concern = *right.read_concern;
	}

	if(right.write_concern)
	{
		_write_concern = *right.write_concern;
	}

	base_type::is_srv = right.is_srv;
	std::strcpy(base_type::srv, right.srv);

	if(base_type::hosts)
	{
		mongoc_host_list_native_ex::s_mongoc_host_list_destroy_all(base_type::hosts);
	}
	base_type::hosts = mongoc_host_list_native_ex::s_mongoc_host_list_copy_all(right.hosts);

	bson_native_ex::s_bson_copy(&(base_type::raw), &(right.raw));
	bson_native_ex::s_bson_copy(&(base_type::options), &(right.options));
	bson_native_ex::s_bson_copy(&(base_type::credentials), &(right.credentials));
	bson_native_ex::s_bson_copy(&(base_type::compressors), &(right.compressors));

	base_type::str = _str.empty()? 0 : _str.data();
	base_type::username = _username.empty()? 0 : _username.data();
	base_type::password = _password.empty()? 0 : _password.data();
	base_type::database = _database.empty()? 0 : _database.data();

	// read_prefs don't need fix, because see assert(base_type::read_prefs == &_read_prefs);
	// read_concern don't need fix, because see assert(base_type::read_concern == &_read_concern);
	// write_concern don't need fix, because see assert(base_type::write_concern == &_write_concern);

	return *this;
}

c_mongo_uri::this_type& c_mongo_uri::pro_copy(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	assert(base_type::read_prefs == _read_prefs);
	assert(base_type::read_concern == _read_concern);
	assert(base_type::write_concern == _write_concern);

	_str = right._str;
	_username = right._username;
	_password = right._password;
	_database = right._database;
	_read_prefs = right._read_prefs;
	_read_concern = right._read_concern;
	_write_concern = right._write_concern;

	base_type::is_srv = right.is_srv;
	std::strcpy(base_type::srv, right.srv);

	if(base_type::hosts)
	{
		mongoc_host_list_native_ex::s_mongoc_host_list_destroy_all(base_type::hosts);
	}
	base_type::hosts = mongoc_host_list_native_ex::s_mongoc_host_list_copy_all(right.hosts);

	bson_native_ex::s_bson_copy(&(base_type::raw), &(right.raw));
	bson_native_ex::s_bson_copy(&(base_type::options), &(right.options));
	bson_native_ex::s_bson_copy(&(base_type::credentials), &(right.credentials));
	bson_native_ex::s_bson_copy(&(base_type::compressors), &(right.compressors));

	base_type::str = _str.empty()? 0 : _str.data();
	base_type::username = _username.empty()? 0 : _username.data();
	base_type::password = _password.empty()? 0 : _password.data();
	base_type::database = _database.empty()? 0 : _database.data();

	// read_prefs don't need fix, because see assert(base_type::read_prefs == &_read_prefs);
	// read_concern don't need fix, because see assert(base_type::read_concern == &_read_concern);
	// write_concern don't need fix, because see assert(base_type::write_concern == &_write_concern);

	return *this;
}

c_mongo_uri::this_type& c_mongo_uri::pro_move(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	assert(base_type::read_prefs == _read_prefs);
	assert(base_type::read_concern == _read_concern);
	assert(base_type::write_concern == _write_concern);

	_str = boost::move(right._str);
	_username = boost::move(right._username);
	_password = boost::move(right._password);
	_database = boost::move(right._database);

	_read_prefs = boost::move(right._read_prefs);
	_read_concern = boost::move(right._read_concern);
	_write_concern = boost::move(right._write_concern);

	base_type::is_srv = boost::move(right.is_srv);
	std::strcpy(base_type::srv, right.srv);

	yggr::swap(base_type::hosts, right.hosts);

	bson_native_ex::s_bson_swap(&(base_type::raw), &(right.raw));
	bson_native_ex::s_bson_swap(&(base_type::options), &(right.options));
	bson_native_ex::s_bson_swap(&(base_type::credentials), &(right.credentials));
	bson_native_ex::s_bson_swap(&(base_type::compressors), &(right.compressors));

	base_type::str = _str.empty()? 0 : _str.data();
	base_type::username = _username.empty()? 0 : _username.data();
	base_type::password = _password.empty()? 0 : _password.data();
	base_type::database = _database.empty()? 0 : _database.data();

	// read_prefs don't need fix, because see assert(base_type::read_prefs == &_read_prefs);
	// read_concern don't need fix, because see assert(base_type::read_concern == &_read_concern);
	// write_concern don't need fix, because see assert(base_type::write_concern == &_write_concern);

	return *this;
}

void c_mongo_uri::pro_swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	assert(base_type::read_prefs == _read_prefs);
	assert(base_type::read_concern == _read_concern);
	assert(base_type::write_concern == _write_concern);

	_str.swap(right._str);
	_username.swap(right._username);
	_password.swap(right._password);
	_database.swap(right._database);

	_read_prefs.swap(right._read_prefs);
	_read_concern.swap(right._read_concern);
	_write_concern.swap(right._write_concern);

	yggr::swap(base_type::is_srv, right.is_srv);
	std::strcpy(base_type::srv, right.srv);

	yggr::swap(base_type::hosts, right.hosts);

	bson_native_ex::s_bson_swap(&(base_type::raw), &(right.raw));
	bson_native_ex::s_bson_swap(&(base_type::options), &(right.options));
	bson_native_ex::s_bson_swap(&(base_type::credentials), &(right.credentials));
	bson_native_ex::s_bson_swap(&(base_type::compressors), &(right.compressors));

	base_type::str = _str.empty()? 0 : _str.data();
	base_type::username = _username.empty()? 0 : _username.data();
	base_type::password = _password.empty()? 0 : _password.data();
	base_type::database = _database.empty()? 0 : _database.data();

	right.str = right._str.empty()? 0 : right._str.data();
	right.username = right._username.empty()? 0 : right._username.data();
	right.password = right._password.empty()? 0 : right._password.data();
	right.database = right._database.empty()? 0 : right._database.data();

	// read_prefs don't need fix, because see assert(base_type::read_prefs == &_read_prefs);
	// read_concern don't need fix, because see assert(base_type::read_concern == &_read_concern);
	// write_concern don't need fix, because see assert(base_type::write_concern == &_write_concern);

	return;
}

//private:

void c_mongo_uri::prv_init_default(void)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));

	bson_init(boost::addressof(base_type::raw));
	bson_init(boost::addressof(base_type::options));
	bson_init(boost::addressof(base_type::credentials));
	bson_init(boost::addressof(base_type::compressors));

	base_type::read_prefs = _read_prefs;
	base_type::read_concern = _read_concern;
	base_type::write_concern = _write_concern;
}

bool c_mongo_uri::prv_init(base_type& right)
{
	assert((this != &right));

	if(mongoc_uri_native_ex::s_mongoc_uri_empty(&right))
	{
		this_type::prv_init_default();
		return false;
	}

	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));

	_str.assign(right.str? right.str : "", YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_username.assign(right.username? right.username : "" , YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_password.assign(right.password? right.password : "" , YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_database.assign(right.database? right.database : "" , YGGR_STR_UTF8_STRING_CHARSET_NAME());
	
	if(right.read_prefs)
	{
		_read_prefs = *right.read_prefs;
	}

	if(right.read_concern)
	{
		_read_concern = *right.read_concern;
	}

	if(right.write_concern)
	{
		_write_concern = *right.write_concern;
	}

	base_type::is_srv = right.is_srv;
	
	assert(!(*base_type::srv));
	std::strcpy(base_type::srv, right.srv);

	assert(base_type::hosts == 0);
	base_type::hosts = right.hosts;
	right.hosts = 0;

	bson_native_ex::s_bson_copy(&(base_type::raw), &(right.raw));
	bson_native_ex::s_bson_copy(&(base_type::options), &(right.options));
	bson_native_ex::s_bson_copy(&(base_type::credentials), &(right.credentials));
	bson_native_ex::s_bson_copy(&(base_type::compressors), &(right.compressors));

	base_type::str = _str.empty()? 0 : _str.data();
	base_type::username = _username.empty()? 0 : _username.data();
	base_type::password = _password.empty()? 0 : _password.data();
	base_type::database = _database.empty()? 0 : _database.data();

	base_type::read_prefs = _read_prefs;
	base_type::read_concern = _read_concern;
	base_type::write_concern = _write_concern;

	return true;
}

void c_mongo_uri::prv_destroy(void)
{
	if(base_type::hosts)
	{
		mongoc_host_list_native_ex::s_mongoc_host_list_destroy_all(base_type::hosts);
	}

	if(base_type::raw.len)
	{
		bson_destroy(&(base_type::raw));
	}

	if(base_type::options.len)
	{
		bson_destroy(&(base_type::options));
	}

	if(base_type::credentials.len)
	{
		bson_destroy(&(base_type::credentials));
	}

	if(base_type::compressors.len)
	{
		bson_destroy(&(base_type::compressors));
	}

	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
}

} // namespace nsql_database_system
} // namespace yggr
