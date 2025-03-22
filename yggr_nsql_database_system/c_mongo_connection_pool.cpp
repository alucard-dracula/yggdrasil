//c_mongo_connection_pool.cpp

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

#include <yggr/nsql_database_system/c_mongo_connection_pool.hpp>
#include <yggr/bind/bind.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_connection_pool::c_mongo_connection_pool(bool pop_mode)
	: _pop_mode(pop_mode)
{
	init_type init;
	this_type::prv_set_connection_info(init, 0, 0);
}

c_mongo_connection_pool::c_mongo_connection_pool(const init_type& init,
						u32 pool_min_size,
						u32 pool_max_size,
						bool pop_mode)
	: _pop_mode(pop_mode)
{
	this_type::prv_set_connection_info(init, pool_min_size, pool_max_size);
}

c_mongo_connection_pool::~c_mongo_connection_pool(void)
{
	assert(_wrap_init.empty());
	assert(_wrap_client_pool.empty());
}

bool c_mongo_connection_pool::push(const mongoc_client_ptr_type& ptr)
{
	client_pool_ptr_type ppool;

	try
	{
		ppool = _wrap_client_pool.load();
	}
	catch(const client_pool_wrap_type::error_type&)
	{
		return false;
	}
		
	if(!ppool)
	{
		return false;
	}

	mongoc_client_pool_push(ppool.get(), ptr.get());

	mongoc_client_deleter_type* pdel
		= boost::get_deleter<mongoc_client_deleter_type>(ptr);

	assert(pdel);
	if(pdel)
	{
		pdel->nonfree();
	}

	return !!pdel;
}

c_mongo_connection_pool::mongoc_client_ptr_type 
	c_mongo_connection_pool::necessarily_pop(void)
{
	client_pool_ptr_type ppool;
	try
	{
		ppool = _wrap_client_pool.load();
	}
	catch(const client_pool_wrap_type::error_type&)
	{
		return mongoc_client_ptr_type();
	}
		
	if(!ppool)
	{
		return mongoc_client_ptr_type();
	}

	org_client_type* pclt = mongoc_client_pool_pop(ppool.get());

	assert(pclt);

	return mongoc_client_ptr_type(pclt, mongoc_client_deleter_type());
}

c_mongo_connection_pool::mongoc_client_ptr_type 
	c_mongo_connection_pool::try_pop(void)
{
	client_pool_ptr_type ppool;
	try
	{
		ppool = _wrap_client_pool.load();
	}
	catch(const client_pool_wrap_type::error_type&)
	{
		return mongoc_client_ptr_type();
	}
		
	if(!ppool)
	{
		return mongoc_client_ptr_type();
	}

	org_client_type* pclt = mongoc_client_pool_try_pop(ppool.get());

	return pclt? mongoc_client_ptr_type(pclt, mongoc_client_deleter_type())
					: mongoc_client_ptr_type();
}

/*static*/ 
bool c_mongo_connection_pool::handler_prv_reset_connection(client_pool_wrap_type::base_type& base,
															init_wrap_type::base_type& base_init,
															const init_type& init, u32 pool_min_size, u32 pool_max_size)
{
	// must use area, because free client pool operator must befor ssl_opt
	{
		init_wrap_type::base_type ptr_init = construct_shared<init_type>(yggr_nothrow_new init_type(init));
		if(!ptr_init)
		{
			return false;
		}

		{
			client_pool_ptr_type ptr_pool = 
				construct_shared<org_type>(
					mongoc_client_pool_new(ptr_init->uri()), 
					mongoc_client_pool_deleter_type());

			if(!ptr_pool)
			{
				return false;
			}

#		ifdef MONGOC_ENABLE_SSL
			const ssl_opt_type& ssl_opt = ptr_init->ssl_opt();
			
			if(ptr_init->uri().get_tls())
			{
				mongoc_client_pool_set_ssl_opts(ptr_pool.get(), ssl_opt);
			}
#		endif // MONGOC_ENABLE_SSL

			if(pool_min_size < pool_max_size)
			{
				mongoc_client_pool_min_size(ptr_pool.get(), pool_min_size);
				mongoc_client_pool_max_size(ptr_pool.get(), pool_max_size);
			}
		
			client_pool_wrap_type::base_type ptr(yggr_nothrow_new client_pool_wrap_type::value_type(ptr_pool));
			if(!ptr)
			{
				return false;
			}
			base.swap(ptr);
		}
		base_init.swap(ptr_init);
	}
	return true;
}

#ifdef MONGOC_ENABLE_SSL

bool c_mongo_connection_pool::handler_prv_set_ssl_opt(init_wrap_type::base_type& base, const ssl_opt_type& opt)
{
	if(!base)
	{
		return false;
	}

	client_pool_ptr_type ppool;
	try
	{
		ppool = _wrap_client_pool.load();
	}
	catch(const client_pool_wrap_type::error_type&)
	{
		return false;
	}
			
	if(!ppool)
	{
		return false;
	}

	base->ssl_opt() = opt;
	const init_type& init_cref = *base;
	const ssl_opt_type& ssl_opt = init_cref.ssl_opt();

	mongoc_client_pool_set_ssl_opts(ppool.get(), ssl_opt);

	return true;
}

#endif // MONGOC_ENABLE_SSL

} // namespace nsql_database_system
} // namespace yggr
