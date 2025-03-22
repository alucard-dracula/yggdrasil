// c_mongo_connection_encryption.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_ENCRYPTION_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_ENCRYPTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_opts.hpp>

#include <yggr/nsql_database_system/detail/mongoc_client_encryption_deleter.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_connection_encryption
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef c_mongo_connection connection_type;
	typedef connection_type::id_type connection_id_type;
	typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;
	
	typedef mongoc_client_encryption_t org_type;
	typedef org_type* org_mongoc_client_encryption_ptr_type;
	typedef const org_type* org_mongoc_client_encryption_cptr_type;

	typedef smart_ptr_ex::shared_ptr<org_type> mongoc_client_encryption_ptr_type;
	typedef detail::mongoc_client_encryption_deleter<org_type> mongoc_client_encryption_deleter_type;

	typedef c_mongo_client_encryption_opts client_encryption_opts_type;

	typedef connection_type::mongoc_client_ptr_type mongoc_client_ptr_type;
	typedef safe_container::safe_wrap<connection_type::org_type> mongoc_client_wrap_type;

private:
	typedef c_mongo_connection_encryption this_type;

public:
	c_mongo_connection_encryption(void);

	template<typename Handler>
	explicit c_mongo_connection_encryption(const Handler& handler)
	{
		this_type::init(handler);
	}

	~c_mongo_connection_encryption(void);

public:
	mongoc_client_encryption_ptr_type pop(void);
	bool push(const mongoc_client_encryption_ptr_type& ptr);

public:
	inline bool validate(void) const
	{
		return _pconn && _penc;
	}

	inline bool is_poped(void) const
	{
		return _pconn && _pconn->is_poped();
	}

public:
	inline connection_ptr_type get_connection_shared_ptr(void) const
	{
		return _pconn;
	}

	inline mongoc_client_ptr_type get_client_shared_ptr(const mongoc_client_encryption_ptr_type& penc) const
	{
		return 
			(_pconn && _pconn->is_poped() && (penc == _penc) && _poped_pclt_wrap)?
				_poped_pclt_wrap.get_shared_ptr() : mongoc_client_ptr_type();
	}

	inline connection_id_type connection_id(void) const
	{
		return _pconn? _pconn->id() : connection_id_type();
	}

public:
	template<typename Handler> inline
	bool init(const Handler& handler)
	{
		return (!this_type::validate()) && handler(_pconn, _penc);
	}

private:
	connection_ptr_type _pconn;
	mongoc_client_encryption_ptr_type _penc;
	mongoc_client_wrap_type _poped_pclt_wrap;
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_ENCRYPTION_HPP__
