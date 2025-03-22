//c_mongo_connection_pool.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_POOL_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_POOL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/support/operator_t.hpp>
#include <yggr/nonable/nonable.hpp>

#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_mongo_uri.hpp>
#include <yggr/nsql_database_system/c_mongo_ssl_opt.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>

#include <boost/atomic.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_connection_pool;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<typename T>
struct mongoc_client_pool_deleter
{
	typedef void result_type;
	typedef T* argument_type;

	inline void operator()(argument_type x) const
	{
		if(!x)
		{
			return;
		}
		::mongoc_client_pool_destroy(x);
	}
};

template<typename T>
struct pointer_mongoc_client_deleter // to_love-ru style(*^_^*)
{
public:
	typedef void result_type;
	typedef T* argument_type;

private:
	typedef pointer_mongoc_client_deleter this_type;

public:
	pointer_mongoc_client_deleter(void)
		: _bdel(true)
	{
	}

	pointer_mongoc_client_deleter(bool bdel)
		: _bdel(bdel)
	{
	}

	~pointer_mongoc_client_deleter(void)
	{
	}

	inline void operator()(argument_type x) const
	{
		if(_bdel && x)
		{
			::mongoc_client_destroy(x);
		}
	}

	inline void nonfree(void)
	{
		_bdel = false;
	}

private:
	bool _bdel;
};


} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_connection_pool
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef mongoc_client_pool_t org_type;
	typedef org_type* org_ptr_type;

	typedef mongoc_client_t org_client_type;

	typedef c_mongo_connection_init init_type;
	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

	typedef org_client_type* org_mongoc_client_ptr_type;
	typedef const org_client_type* org_mongoc_client_cptr_type;

	typedef org_mongoc_client_ptr_type base_mongoc_client_ptr_type;
	typedef org_mongoc_client_cptr_type base_mongoc_client_cptr_type;

	typedef smart_ptr_ex::shared_ptr<org_client_type> mongoc_client_ptr_type;

	typedef c_bson_error bson_error_type;
	typedef bson_error_type::org_type org_bson_error_type;

	typedef c_mongo_uri uri_type;
	typedef uri_type::inner_string_type uri_inner_string_type;
	typedef uri_type::inner_string_view_type uri_inner_string_view_type;

	typedef c_mongo_apm_callbacks apm_callbacks_type;
	typedef apm_callbacks_type::org_type org_apm_callbacks_type;

	typedef c_mongo_server_api server_api_type;
	typedef server_api_type::org_type org_server_api_type;

	typedef c_mongo_connection connection_type;

#ifdef MONGOC_ENABLE_SSL
	typedef init_type::ssl_opt_type ssl_opt_type;
	typedef ssl_opt_type::path_string_type ssl_opt_path_string_type;
	typedef ssl_opt_type::pwd_string_type ssl_opt_pwd_string_type;
#endif // MONGOC_ENABLE_SSL

	YGGR_STATIC_CONSTANT(u32, E_default_pool_min_size = 0);
	YGGR_STATIC_CONSTANT(u32, E_default_pool_max_size = 100);

public:
	typedef ::yggr::shared_ptr<org_type> mongoc_client_pool_ptr_type;

private:
	typedef mongoc_client_pool_ptr_type client_pool_ptr_type;
	typedef safe_container::safe_wrap<client_pool_ptr_type> client_pool_wrap_type;
	typedef safe_container::safe_wrap<init_type, boost::shared_mutex> init_wrap_type;
	typedef boost::atomic<bool> pop_mode_type;

private:
	typedef c_mongo_connection_pool this_type;

private:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef detail::mongoc_client_pool_deleter<org_type> mongoc_client_pool_deleter_type;

public:
	typedef detail::pointer_mongoc_client_deleter<org_client_type> mongoc_client_deleter_type;

public:
	c_mongo_connection_pool(bool pop_mode = false);

	template<typename String>
	c_mongo_connection_pool(const String& str_uri,
							u32 pool_min_size = 0,
							u32 pool_max_size = 0,
							bool pop_mode = false,
							typename
								boost::enable_if
								<
									charset::utf8_string_constructable<String>,
									sfinae_type
								>::type sfinae = 0)
		: _pop_mode(pop_mode)
	{
		init_type init(str_uri);
		this_type::prv_set_connection_info(init, pool_min_size, pool_max_size);
	}

#ifdef MONGOC_ENABLE_SSL
	template<typename String1,
				typename String2, typename String3,
				typename String4, typename String5,
				typename String6 >
	c_mongo_connection_pool(const String1& str_uri,
							const String2& pem_file,
							const String3& pem_pwd,
							const String4& ca_file,
							const String5& ca_dir,
							const String6& crl_file,
							bool weak_cert_validation = true,
							u32 pool_min_size = 0,
							u32 pool_max_size = 0,
							bool pop_mode = false,
							typename
								boost::enable_if
								<
									boost::mpl::and_
									<
										boost::mpl::and_
										<
											charset::utf8_string_constructable<String1>,
											charset::utf8_string_constructable<String2>,
											charset::utf8_string_constructable<String3>
										>,
										boost::mpl::and_
										<
											charset::utf8_string_constructable<String4>,
											charset::utf8_string_constructable<String5>,
											charset::utf8_string_constructable<String6>
										>
									>,
									sfinae_type
								>::type sfinae = 0)
		: _pop_mode(pop_mode)
	{
		init_type init(str_uri,
						pem_file,
						pem_pwd,
						ca_file,
						ca_dir,
						crl_file,
						weak_cert_validation);

		this_type::prv_set_connection_info(init, pool_min_size, pool_max_size);
	}

#endif // MONGOC_ENABLE_SSL

	c_mongo_connection_pool(const init_type& init,
							u32 pool_min_size = 0,
							u32 pool_max_size = 0,
							bool pop_mode = false);

	~c_mongo_connection_pool(void);

public:
	// don't use swap
	//void swap(this_type& right);

	//Compatible with connection pools
public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		try
		{
			return !!_wrap_client_pool.load();
		}
		catch(const client_pool_wrap_type::error_type&)
		{
			return false;
		}
	}

public:
	inline mongoc_client_ptr_type pop(void)
	{
		bool pop_mode = _pop_mode.load();

		return pop_mode? try_pop() : necessarily_pop();
	}

	bool push(const mongoc_client_ptr_type& ptr); // push foo

	inline org_client_type* revocable(org_client_type* ptr) const
	{
		return ptr;
	}

	inline bool revoke(void)
	{
		assert(false);
		return false;
	}

	inline bool revoke(org_client_type* pclt)
	{
		if(!pclt)
		{
			return false;
		}

		mongoc_client_ptr_type ptr(pclt, mongoc_client_deleter_type());

		return this_type::push(ptr);
	}

public:
	inline mongoc_client_pool_ptr_type client_pool_pointer(void) const
	{
		try
		{
			return _wrap_client_pool.load();
		}
		catch(const client_pool_wrap_type::error_type&)
		{
			return mongoc_client_pool_ptr_type();
		}
	}

public:
	inline uri_type uri(void) const
	{
		try
		{
			return _wrap_init.load().uri();
		}
		catch(const client_pool_wrap_type::error_type&)
		{
			return uri_type();
		}
	}

	inline uri_inner_string_type uri_string(void) const
	{
		try
		{
			return _wrap_init.load().uri().var_uri_string(YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}
		catch(const client_pool_wrap_type::error_type&)
		{
			return inner_string_type();
		}
	}

	template<typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, uri_inner_string_type>::type
		uri_string(const Alloc& alloc) const
	{
		try
		{
			return _wrap_init.load().uri().var_uri_string(alloc);
		}
		catch(const client_pool_wrap_type::error_type&)
		{
			return inner_string_type(alloc);
		}
	}

	template<typename String> inline
	String uri_string(const string& charset_name
						= charset::charset_name_t<String>()) const
	{
		typedef String now_string_type;
		try
		{
			return _wrap_init.load().uri().var_uri_string<now_string_type>(charset_name);
		}
		catch(const init_wrap_type::error_type&)
		{
			return now_string_type();
		}
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		uri_string(const Alloc& alloc,
					const string& charset_name
						= charset::charset_name_t<String>()) const
	{
		typedef String now_string_type;
		try
		{
			return _wrap_init.load().uri().var_uri_string<now_string_type>(alloc, charset_name);
		}
		catch(const init_wrap_type::error_type&)
		{
			return now_string_type(alloc);
		}
	}

public:
	inline void clear(void)
	{
		//free client pool operator must befor ssl_opt
		// and wait inner thread end
		_wrap_client_pool.clear();
		_wrap_init.clear();
	}

	inline bool empty(void) const
	{
		try
		{
			return !(_wrap_client_pool.load());
		}
		catch(const client_pool_wrap_type::error_type&)
		{
			return true;
		}
	}

	inline bool is_poped(void) const
	{
		return false;
	}

#ifdef MONGOC_ENABLE_SSL

public:
	inline ssl_opt_type ssl_opt(void) const
	{
		try
		{
			return _wrap_init.load().ssl_opt();
		}
		catch(const init_wrap_type::error_type&)
		{
			return ssl_opt_type();
		}
	}

	inline bool ssl_opt(const ssl_opt_type& opt)
	{
		return this_type::prv_set_ssl_opt(opt);
	}

	inline bool ssl_opt(const ssl_opt_type::org_type* opt)
	{
		ssl_opt_type tmp(opt);
		return this_type::prv_set_ssl_opt(tmp);
	}

#endif // MONGOC_ENABLE_SSL


public:
	mongoc_client_ptr_type necessarily_pop(void);
	mongoc_client_ptr_type try_pop(void);

	inline bool gen_connection(connection_type& conn) const
	{
		init_type init;
		try
		{
			init = _wrap_init.load();
		}
		catch(const init_wrap_type::error_type&)
		{
			return false;
		}

		return conn.reinit(init);
	}

	inline void min_size(u32 size)
	{
		client_pool_ptr_type ppool;
		try
		{
			ppool = _wrap_client_pool.load();
		}
		catch(const client_pool_wrap_type::error_type&)
		{
			return;
		}

		if(!ppool)
		{
			return;
		}

		mongoc_client_pool_min_size(ppool.get(), size);
	}

	inline void max_size(u32 size)
	{
		client_pool_ptr_type ppool;
		try
		{
			ppool = _wrap_client_pool.load();
		}
		catch(const client_pool_wrap_type::error_type&)
		{
			return;
		}

		if(!ppool)
		{
			return;
		}

		mongoc_client_pool_max_size(ppool.get(), size);
	}

public:
	template<typename String> inline
	bool set_appname(const String& str_appname)
	{
		typedef typename native_t<String>::type string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string_type, utf8_string_type);

		utf8_string_type utf8_str_appname((charset::string_charset_helper_data(str_appname)));

		if(utf8_str_appname.empty())
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

		return mongoc_client_pool_set_appname(ppool.get(), utf8_str_appname.c_str());
	}

	// set_apm_callbacks
protected:
	inline bool pro_set_apm_callbacks(org_apm_callbacks_type* pcbs, void* context)
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

		return mongoc_client_pool_set_apm_callbacks(ppool.get(), pcbs, context);
	}
public:
	template<typename ApmCallbacks> inline
	bool set_apm_callbacks(const ApmCallbacks& cbs, void* context = 0)
	{
		return
			this_type::pro_set_apm_callbacks(
				const_cast<org_apm_callbacks_type*>(detail::to_const_pointer<org_apm_callbacks_type>(cbs)),
				static_cast<void*>(context));
	}

	template<typename ApmCallbacks, typename Context> inline
	bool set_apm_callbacks(const ApmCallbacks& cbs, Context& context)
	{
		return
			this_type::pro_set_apm_callbacks(
				const_cast<org_apm_callbacks_type*>(detail::to_const_pointer<org_apm_callbacks_type>(cbs)),
				static_cast<void*>(boost::addressof(context)));
	}

	// set_error_api
	inline bool set_error_api(s32 version)
	{
		switch(version)
		{
		case MONGOC_ERROR_API_VERSION_LEGACY:
		case MONGOC_ERROR_API_VERSION_2:
			break;
		default:
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

		return mongoc_client_pool_set_error_api(ppool.get(), version);
	}

	// set_server_api
protected:
	inline bool pro_set_server_api(const org_server_api_type* papi, org_bson_error_type* perr)
	{
		assert(papi);
		if(!papi)
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

		return mongoc_client_pool_set_server_api(ppool.get(), papi, perr);
	}

public:
	template<typename ServerApi> inline
	bool set_server_api(const ServerApi& svr_api, org_bson_error_type* perr)
	{
		return
			this_type::pro_set_server_api(
				detail::to_const_pointer<org_server_api_type>(svr_api),
				perr);
	}

	template<typename ServerApi> inline
	bool set_server_api(const ServerApi& svr_api, org_bson_error_type& err)
	{
		return
			this_type::pro_set_server_api(
				detail::to_const_pointer<org_server_api_type>(svr_api),
				boost::addressof(err));
	}

private:
	inline bool prv_set_connection_info(const init_type& init, u32 pool_min_size, u32 pool_max_size)
	{
		return
			_wrap_init.locked_unsafe_using_handler(
				boost::bind(
					&this_type::handler_prv_set_connection_info,
					this, _1, boost::cref(init), pool_min_size, pool_max_size));
	}

	inline bool handler_prv_set_connection_info(init_wrap_type::base_type& base,
												const init_type& init, u32 pool_min_size, u32 pool_max_size)
	{
		return
			_wrap_client_pool.locked_unsafe_using_handler(
				boost::bind(
					&this_type::handler_prv_reset_connection,
					_1, boost::ref(base), boost::cref(init),
					pool_min_size, pool_max_size));
	}

	static bool handler_prv_reset_connection(client_pool_wrap_type::base_type& base,
												init_wrap_type::base_type& base_init,
												const init_type& init,
												u32 pool_min_size, u32 pool_max_size);

#ifdef MONGOC_ENABLE_SSL

	inline bool prv_set_ssl_opt(const ssl_opt_type& opt)
	{
		return
			_wrap_init.locked_unsafe_using_handler(
				boost::bind(&this_type::handler_prv_set_ssl_opt, this, _1, boost::cref(opt)));
	}

	bool handler_prv_set_ssl_opt(init_wrap_type::base_type& base, const ssl_opt_type& opt);

#endif // MONGOC_ENABLE_SSL

private:
	pop_mode_type _pop_mode;
	init_wrap_type _wrap_init;
	client_pool_wrap_type _wrap_client_pool;
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_POOL_HPP__
