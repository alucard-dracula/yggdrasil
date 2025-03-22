// c_mongo_client_encryption_encrypt_opts.cpp

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

#include <yggr/nsql_database_system/c_mongo_client_encryption_encrypt_opts.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//public:
c_mongo_client_encryption_encrypt_opts::c_mongo_client_encryption_encrypt_opts(void)
{
	this_type::pro_init();
}

c_mongo_client_encryption_encrypt_opts::c_mongo_client_encryption_encrypt_opts(const org_type* ptr)
{
	this_type::pro_init();
	this_type::pro_copy(ptr);
}

c_mongo_client_encryption_encrypt_opts::c_mongo_client_encryption_encrypt_opts(const org_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_encrypt_opts::c_mongo_client_encryption_encrypt_opts(const this_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(right);
}

c_mongo_client_encryption_encrypt_opts::~c_mongo_client_encryption_encrypt_opts(void)
{
	if(base_type::keyid.value_type)
	{
		bson_value_destroy(boost::addressof(base_type::keyid));
	}
}

//public:
c_mongo_client_encryption_encrypt_opts::this_type& 
	c_mongo_client_encryption_encrypt_opts::operator=(const org_type& right)
{
	return this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_encrypt_opts::this_type& 
	c_mongo_client_encryption_encrypt_opts::operator=(const base_type& right)
{
	return this_type::pro_copy(reinterpret_cast<const org_type*>(boost::addressof(right)));
}

c_mongo_client_encryption_encrypt_opts::this_type& 
	c_mongo_client_encryption_encrypt_opts::operator=(const this_type& right)
{
	return this_type::pro_copy(right);
}

//public:
void c_mongo_client_encryption_encrypt_opts::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:
void c_mongo_client_encryption_encrypt_opts::pro_init(void)
{
	mongoc_client_encryption_encrypt_opts_native_ex::s_mongoc_client_encryption_encrypt_opts_init(this);
}

void c_mongo_client_encryption_encrypt_opts::pro_rebuild_base(void)
{
	if(_algorithm.size())
	{
		base_type::algorithm = _algorithm.data();
	}

	if(_keyaltname.size())
	{
		base_type::keyaltname = _keyaltname.data();
	}

	if(_query_mode.size())
	{
		base_type::query_type = _query_mode.data();
	}

	if(_algorithm == mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_range_preview())
	{
		base_type::range_opts = _range_opts.org_pointer();
	}
	else
	{
		_range_opts.clear();
		base_type::range_opts = 0;
	}
}

void c_mongo_client_encryption_encrypt_opts::pro_clear(void)
{
	mongoc_client_encryption_encrypt_opts_native_ex::s_mongoc_client_encryption_encrypt_opts_clear(this);
	_algorithm.clear();
	_keyaltname.clear();
	_query_mode.clear();
	_range_opts.clear();
}

c_mongo_client_encryption_encrypt_opts::this_type& 
	c_mongo_client_encryption_encrypt_opts::pro_copy(const org_type* ptr)
{
	if(!ptr || ptr == reinterpret_cast<org_type*>(this))
	{
		return *this;
	}

	const base_type* prbase = reinterpret_cast<const base_type*>(ptr);

	bson_value_native_ex::s_bson_value_copy(
		boost::addressof(base_type::keyid),
		boost::addressof(prbase->keyid) );

	if(prbase->algorithm)
	{
		_algorithm = prbase->algorithm;
		base_type::algorithm = _algorithm.empty()? 0 : _algorithm.data();
	}

	if(prbase->keyaltname)
	{
		_keyaltname = prbase->keyaltname;
		base_type::keyaltname = _keyaltname.empty()? 0 : _keyaltname.data();
	}

	base_type::contention_factor = prbase->contention_factor;

	if(prbase->query_type)
	{
		_query_mode = prbase->query_type;
		base_type::query_type = _query_mode.empty()? 0 : _query_mode.data();
	}

	if(prbase->range_opts
		&& (_algorithm == mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_range_preview()) )
	{
		_range_opts = *(prbase->range_opts);
		base_type::range_opts = _range_opts.org_pointer();
	}
	else
	{
		_range_opts.clear();
		base_type::range_opts = 0;
	}

	return *this;
}

c_mongo_client_encryption_encrypt_opts::this_type& 
	c_mongo_client_encryption_encrypt_opts::pro_copy(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	bson_value_native_ex::s_bson_value_copy(
		boost::addressof(base_type::keyid),
		boost::addressof(right.keyid) );

	_algorithm = right._algorithm;
	base_type::algorithm = _algorithm.empty()? 0 : _algorithm.data();

	_keyaltname = right._keyaltname;
	base_type::keyaltname = _keyaltname.empty()? 0 : _keyaltname.data();

	base_type::contention_factor = right.contention_factor;

	_query_mode = right._query_mode;
	base_type::query_type = _query_mode.empty()? 0 : _query_mode.data();

	if(_algorithm == mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_range_preview())
	{
		_range_opts = right._range_opts;
		base_type::range_opts = _range_opts.org_pointer();
	}
	else
	{
		_range_opts.clear();
		base_type::range_opts = 0;
	}

	return *this;
}

void c_mongo_client_encryption_encrypt_opts::pro_swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	bson_value_native_ex::s_bson_value_swap(
		boost::addressof(base_type::keyid),
		boost::addressof(right.keyid) );

	_algorithm.swap(right._algorithm);
	_keyaltname.swap(right._keyaltname);
	::yggr::swap(base_type::contention_factor, right.contention_factor);
	_query_mode.swap(right._query_mode);
	_range_opts.swap(right._range_opts);

	this_type::pro_rebuild_base();
	right.pro_rebuild_base();
}

c_mongo_client_encryption_encrypt_opts::this_type& 
	c_mongo_client_encryption_encrypt_opts::pro_move(this_type& right)
{
	this_type& right_ref = right;
		
	if(this == boost::addressof(right_ref))
	{
		return *this;
	}

	bson_value_native_ex::s_bson_value_swap(
		boost::addressof(base_type::keyid),
		boost::addressof(right.keyid) );

	_algorithm = boost::move(right._algorithm);
	_keyaltname = boost::move(right._keyaltname);
	base_type::contention_factor = right.contention_factor;
	_query_mode = boost::move(right._query_mode);
	_range_opts = boost::move(right._range_opts);

	this_type::pro_rebuild_base();

	return *this;
}

} // namespace nsql_database_system
} // namespace yggr
