// c_mongo_enc_connection_pool.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_ENC_CONNECT_POOL_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_ENC_CONNECT_POOL_HPP__

#include <yggr/nsql_database_system/c_mongo_connection_pool.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_enc_connection_pool
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef c_mongo_connection_pool mongo_connection_pool_type;
	typedef smart_ptr_ex::shared_ptr<mongo_connection_pool_type> mongo_connection_pool_ptr_type;

	typedef mongo_connection_pool_type::org_type org_type;
	typedef mongo_connection_pool_type::org_type mongoc_client_pool_type;
	typedef mongo_connection_pool_type::mongoc_client_pool_ptr_type mongoc_client_pool_ptr_type;

	typedef mongo_connection_pool_type::org_client_type mongoc_client_type;
	typedef mongo_connection_pool_type::mongoc_client_ptr_type mongoc_client_ptr_type;

public:
	typedef mongo_connection_pool_type::uri_type uri_type;
	typedef mongo_connection_pool_type::uri_inner_string_type uri_inner_string_type;

#ifdef MONGOC_ENABLE_SSL
	typedef mongo_connection_pool_type::ssl_opt_type ssl_opt_type;
	typedef mongo_connection_pool_type::ssl_opt_path_string_type ssl_opt_path_string_type;
	typedef mongo_connection_pool_type::ssl_opt_pwd_string_type ssl_opt_pwd_string_type;
#endif // MONGOC_ENABLE_SSL

	typedef mongo_connection_pool_type::inner_string_type inner_string_type;
	typedef mongo_connection_pool_type::inner_string_view_type inner_string_view_type;

	//typedef mongo_connection_pool_type::empty_conn_err_type empty_conn_err_type;

private:
	typedef c_mongo_enc_connection_pool this_type;

public:
	c_mongo_enc_connection_pool(void)
	{
	}

	template<typename Handler>
	c_mongo_enc_connection_pool(const Handler& handler)
	{
		this_type::init(handler);
	}

	~c_mongo_enc_connection_pool(void)
	{
	}

public:
	inline mongoc_client_ptr_type pop(void)
	{
		return _pconn_pool? _pconn_pool->pop() : mongoc_client_ptr_type();
	}

	inline bool push(const mongoc_client_ptr_type pclt)
	{
		return _pconn_pool && _pconn_pool->push(pclt);
	}

public:
	inline bool empty(void) const
	{
		return !_pconn_pool;
	}

	inline bool validate(void) const
	{
		return !!_pconn_pool;
	}

	inline bool is_poped(void)
	{
		return false;
	}

public:
	template<typename Handler> inline
	bool init(const Handler& handler)
	{
		return 
			!(_pconn_pool || _pconn_pool_keyvalut) 
			&& handler(_pconn_pool, _pconn_pool_keyvalut)
			&& this_type::pro_format();
	}

protected:
	inline bool pro_format(void)
	{
		if(_pconn_pool == _pconn_pool_keyvalut)
		{
			mongo_connection_pool_ptr_type tmp;
			_pconn_pool_keyvalut.swap(tmp);
		}

		return true;
	}

private:
	mongo_connection_pool_ptr_type _pconn_pool_keyvalut;
	mongo_connection_pool_ptr_type _pconn_pool;
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_ENC_CONNECT_POOL_HPP__
