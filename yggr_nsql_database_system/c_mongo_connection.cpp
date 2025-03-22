//c_mongo_connection.cpp

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

#include <yggr/nsql_database_system/c_mongo_connection.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/utility/swap.hpp>

#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

//-------------------------------------mongo_connection_container------------------------------

mongo_connection_container::mongo_connection_container(void)
	: _bpoped(false)
{
}

mongo_connection_container::mongo_connection_container(const init_type& init)
	: _bpoped(false)
{
	this_type::prv_init(init);
}

mongo_connection_container::mongo_connection_container(const this_type& right)
	: _pclt(right._pclt), _bpoped(false)
{
}

mongo_connection_container::~mongo_connection_container(void)
{
	assert(!_bpoped);
}

mongo_connection_container::this_type& 
	mongo_connection_container::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_pclt = right._pclt;
	_bpoped = right._bpoped;
	return *this;
}

void mongo_connection_container::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	_pclt.swap(right._pclt);
	yggr::swap(_bpoped, right._bpoped);
}

bool mongo_connection_container::prv_init(const init_type& init)
{
	if(init.empty())
	{
		return false;
	}
			
	mongoc_client_ptr_type pclt(mongoc_client_new_from_uri(init.uri()), 
								mongoc_client_deleter_type());
	if(!pclt)
	{
		return false;
	}
			
#ifdef MONGOC_ENABLE_SSL
	if(init.is_using_tls())
	{
		mongoc_client_set_ssl_opts(pclt.get(), (init.ssl_opt()));
	}
#endif // MONGOC_ENABLE_SSL

	_pclt.swap(pclt);

	return true;
}

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

//-------------------------------------c_mongo_connection------------------------------

c_mongo_connection::c_mongo_connection(void)
{
	init_type init;
	this_type::prv_set_connection_info(init);
}

c_mongo_connection::c_mongo_connection(const init_type& init)
{
	this_type::prv_set_connection_info(init);
}

c_mongo_connection::~c_mongo_connection(void)
{
	assert(_wrap_conn.empty());
	assert(_wrap_init.empty());
}

bool c_mongo_connection::handler_prv_set_connection_info(init_wrap_type::base_type& base, const init_type& init)
{
	return 
		_wrap_conn.locked_unsafe_using_handler(
			boost::bind(
				&this_type::handler_prv_reset_connection, 
				_1, boost::ref(base), boost::cref(init)));
}

/*static*/ 
bool c_mongo_connection::handler_prv_reset_connection(connection_container_wrap_type::base_type& base,
														init_wrap_type::base_type& base_init,
														const init_type& init)
{
	// must use area, because free client pool operator must befor ssl_opt
	{
		init_wrap_type::base_type ptr_init = construct_shared<init_type>(yggr_nothrow_new init_type(init));
		if(!ptr_init)
		{
			return false;
		}

		{
			connection_container_wrap_type::base_type ptr = 
				construct_shared<connection_container_wrap_type::value_type>(
					yggr_nothrow_new connection_container_wrap_type::value_type(*ptr_init));
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

/*static*/ 
bool c_mongo_connection::handler_prv_reset_ssl_opt(connection_container_wrap_type::base_type& base,
													init_wrap_type::base_type& base_init,
													const ssl_opt_type& opt)
{
	if(!base)
	{
		return false;
	}

	mongoc_client_ptr_type pclt = base->pop();

	if(!pclt)
	{
		return false;
	}

	base_init->ssl_opt() = opt;
	
	// ssl_opt not copy data to client but memcpy to client, so must use base_init address  
	const init_type& init_cref = *base_init;
	const ssl_opt_type& ssl_opt = init_cref.ssl_opt();

	mongoc_client_set_ssl_opts(pclt.get(), ssl_opt);

	bool bsuccess = base->push(pclt);

	assert(bsuccess);

	return bsuccess;
}

#endif // MONGOC_ENABLE_SSL

/*static*/ 
bool c_mongo_connection::handler_prv_reset_appname(connection_container_wrap_type::base_type& base,
													const inner_string_type& str_appname)
{
	mongoc_client_ptr_type pclt;
	if(!(base && str_appname.size() && (pclt = base->pop())))
	{
		return false;
	}

	mongoc_client_set_appname(pclt.get(), str_appname.c_str());

	bool bsuccess = base->push(pclt);

	assert(bsuccess);

	return bsuccess;
}

/*static*/ 
bool c_mongo_connection::handler_prv_reset_amp_callbacks(connection_container_wrap_type::base_type& base,
															org_apm_callbacks_type* pcallbacks, void* context)
{
	mongoc_client_ptr_type pclt;
	if(!(base && (pclt = base->pop())))
	{
		return false;
	}

	bool bret = mongoc_client_set_apm_callbacks(pclt.get(), pcallbacks, context);

	bool bsuccess = base->push(pclt);

	assert(bsuccess);

	return bret;
}

/*static*/ 
bool c_mongo_connection::handler_prv_set_error_api(connection_container_wrap_type::base_type& base, s32 version)
{
	mongoc_client_ptr_type pclt;
	if(!(base && (pclt = base->pop())))
	{
		return false;
	}

	bool bret = mongoc_client_set_error_api(pclt.get(), version);

	bool bsuccess = base->push(pclt);

	assert(bsuccess);

	return bret;
}

/*static*/ 
bool c_mongo_connection::handler_prv_set_server_api(connection_container_wrap_type::base_type& base,
													const org_server_api_type* papi, org_bson_error_type* perr)
{
	assert(papi);
	mongoc_client_ptr_type pclt;
	if(!(base && papi && (pclt = base->pop())))
	{
		return false;
	}

	bool bret = mongoc_client_set_server_api(pclt.get(), papi, perr);

	bool bsuccess = base->push(pclt);

	assert(bsuccess);

	return bret;
}

} // namespace nsql_database_system
} // namespace yggr


#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER
