//c_mongo_connection.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/support/operator_t.hpp>

#include <yggr/smart_ptr_ex/interface_ptr.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/args_holder_nv.hpp>
#include <yggr/utility/cast_pointer_to_int.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/container/size.hpp>
#include <yggr/container/reserve.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/mplex/pointer_int.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/mongoc_client_native_ex.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>

#include <yggr/nsql_database_system/c_mongo_server_description.hpp>
#include <yggr/nsql_database_system/c_mongo_connection_init.hpp>
#include <yggr/nsql_database_system/c_mongo_read_concern.hpp>
#include <yggr/nsql_database_system/c_mongo_read_prefs.hpp>
#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>

#include <yggr/nsql_database_system/c_mongo_apm_callbacks.hpp>
#include <yggr/nsql_database_system/c_mongo_server_api.hpp>
#include <yggr/nsql_database_system/c_mongo_auto_encryption_opts.hpp>

#include <yggr/nsql_database_system/detail/to_pointer.hpp>

#include <boost/thread/shared_mutex.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <algorithm>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_connection;

namespace detail
{

template<typename T>
struct mongoc_client_deleter
{
	typedef void result_type;
	typedef T* argument_type;

	inline void operator()(argument_type x) const
	{
		if(x) 
		{
			::mongoc_client_destroy(x);
		}
	}

	inline void nonfree(void)
	{
	}
};

// mongo_connection_container
struct mongo_connection_container
{
private:
	friend class c_mongo_connection;

public:
	typedef c_mongo_connection_init init_type;

	typedef mongoc_client_t org_type;
	typedef org_type* org_mongoc_client_ptr_type;
	typedef const org_type* org_mongoc_client_cptr_type;

	typedef org_mongoc_client_ptr_type base_mongoc_client_ptr_type;
	typedef org_mongoc_client_cptr_type base_mongoc_client_cptr_type;
	
	typedef smart_ptr_ex::shared_ptr<org_type> mongoc_client_ptr_type;
	typedef mongoc_client_deleter<org_type> mongoc_client_deleter_type;

public:
	typedef c_bson_error bson_error_type;
	typedef bson_error_type::org_type org_bson_error_type;

	typedef c_mongo_read_prefs read_prefs_type;
	typedef read_prefs_type::org_type org_read_prefs_type;

	typedef c_mongo_read_concern read_concern_type;
	typedef read_concern_type::org_type org_read_concern_type;

	typedef c_mongo_write_concern write_concern_type;
	typedef write_concern_type::org_type org_write_concern_type;

	typedef c_mongo_server_description server_description_type;
	typedef server_description_type::org_type org_server_description_type;

	typedef c_mongo_apm_callbacks apm_callbacks_type;
	typedef apm_callbacks_type::org_type org_apm_callbacks_type;

	typedef c_mongo_server_api server_api_type;
	typedef server_api_type::org_type org_server_api_type;

protected:
	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef mongo_connection_container this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	mongo_connection_container(void);
	mongo_connection_container(const init_type& init);

	mongo_connection_container(BOOST_RV_REF(this_type) right)
		: _pclt(boost::move(right._pclt)),
			_bpoped(boost::move(right._bpoped))
	{
	}

	mongo_connection_container(const this_type& right);
	~mongo_connection_container(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_pclt, boost::move(right._pclt));
		copy_or_move_or_swap(_bpoped, boost::move(right._bpoped));

		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);


	//Compatible with connection pools
public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !!_pclt;
	}

public:
	inline mongoc_client_ptr_type pop(void)
	{
		if(_bpoped || !_pclt)
		{
			return mongoc_client_ptr_type();
		}

		assert(_bpoped == false);
		_bpoped = !_bpoped;
		return _pclt;
	}

	inline bool push(const mongoc_client_ptr_type& ptr)
	{
		if(!_bpoped || ptr != _pclt)
		{
			return false;
		}

		assert(_bpoped == true);
		_bpoped = !_bpoped;
		return true;
	}

	inline base_mongoc_client_ptr_type revocable(base_mongoc_client_ptr_type ptr) const
	{
		return (_bpoped && (ptr == _pclt.get()))? ptr : 0;
	}

	inline bool revoke(void)
	{
		return _bpoped && (_bpoped = !_bpoped, true);
	}

	inline bool check_revoke(base_mongoc_client_cptr_type ptr)
	{
		return 
			_bpoped 
			&& (ptr == _pclt.get())
			&& (_bpoped = !_bpoped, true);
	}

	inline bool empty(void) const
	{
		//return _pclt && !_bpoped; // ?
		return !_pclt;
	}

	inline bool is_poped(void) const
	{
		return _bpoped;
	}

//public:
//	inline mongoc_client_ptr_type clinet_pointer(void) const
//	{
//		return _pclt;
//	}

public:
	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		using_handler(const Handler& handler)
	{
		return handler(_pclt, _bpoped);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		using_handler(const Handler& handler) const
	{
		return handler(_pclt, _bpoped);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		using_any_handler(const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		const handler_type* phandler = utility::args_holder_static_cast<handler_type>(any_handler);
		assert(phandler);

		return (*phandler)(_pclt, _bpoped);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		using_any_handler_const(const basic_args_holder_nv_type& any_handler) const
	{
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		const handler_type* phandler = utility::args_holder_static_cast<handler_type>(any_handler);
		assert(phandler);

		return (*phandler)(_pclt, _bpoped);
	}

private:
	bool prv_init(const init_type& init);

private:
	mongoc_client_ptr_type _pclt;
	bool _bpoped;
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_connection
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef mplex::pointer_int::type id_type;

	typedef mongoc_client_t org_type;
	typedef org_type* org_mongoc_client_ptr_type;
	typedef const org_type* org_mongoc_client_cptr_type;

	typedef org_mongoc_client_ptr_type base_mongoc_client_ptr_type;
	typedef org_mongoc_client_cptr_type base_mongoc_client_cptr_type;

	typedef smart_ptr_ex::shared_ptr<org_type> mongoc_client_ptr_type;

	typedef c_mongo_connection_init init_type;

	typedef init_type::uri_type uri_type;
	typedef uri_type::inner_string_type uri_inner_string_type;

#ifdef MONGOC_ENABLE_SSL
	typedef init_type::ssl_opt_type ssl_opt_type;
	typedef ssl_opt_type::path_string_type ssl_opt_path_string_type;
	typedef ssl_opt_type::pwd_string_type ssl_opt_pwd_string_type;
#endif // MONGOC_ENABLE_SSL

	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

private:
	typedef safe_container::safe_wrap<init_type, boost::shared_mutex> init_wrap_type;

public:
	typedef init_wrap_type::error_type empty_conn_err_type;

//---------------------------------------------------------

//typedef enum mongo_error_t {
//    MONGO_CONN_SUCCESS = 0,  /**< Connection success! */
//    MONGO_CONN_NO_SOCKET,    /**< Could not create a socket. */
//    MONGO_CONN_FAIL,         /**< An error occured while calling connect(). */
//    MONGO_CONN_ADDR_FAIL,    /**< An error occured while calling getaddrinfo(). */
//    MONGO_CONN_NOT_MASTER,   /**< Warning: connected to a non-master node (read-only). */
//    MONGO_CONN_BAD_SET_NAME, /**< Given rs name doesn't match this replica set. */
//    MONGO_CONN_NO_PRIMARY,   /**< Can't find primary in replica set. Connection closed. */
//
//    MONGO_IO_ERROR,          /**< An error occurred while reading or writing on the socket. */
//    MONGO_SOCKET_ERROR,      /**< Other socket error. */
//    MONGO_READ_SIZE_ERROR,   /**< The response is not the expected length. */
//    MONGO_COMMAND_FAILED,    /**< The command returned with 'ok' value of 0. */
//    MONGO_WRITE_ERROR,       /**< Write with given write_concern returned an error. */
//    MONGO_NS_INVALID,        /**< The name for the ns (database or collection) is invalid. */
//    MONGO_BSON_INVALID,      /**< BSON not valid for the specified op. */
//    MONGO_BSON_NOT_FINISHED, /**< BSON object has not been finished. */
//    MONGO_BSON_TOO_LARGE,    /**< BSON object exceeds max BSON size. */
//    MONGO_WRITE_CONCERN_INVALID /**< Supplied write concern object is invalid. */
//} mongo_error_t;

public:
	typedef detail::mongoc_client_deleter<org_type> mongoc_client_deleter_type;

public:
	typedef detail::mongo_connection_container connection_container_type;

private:
	typedef safe_container::safe_wrap<connection_container_type> connection_container_wrap_type;

public:
	typedef connection_container_type::bson_error_type bson_error_type;
	typedef connection_container_type::org_bson_error_type org_bson_error_type;

	typedef connection_container_type::read_prefs_type read_prefs_type;
	typedef connection_container_type::org_read_prefs_type org_read_prefs_type;

	typedef connection_container_type::read_concern_type read_concern_type;
	typedef connection_container_type::org_read_concern_type org_read_concern_type;

	typedef connection_container_type::write_concern_type write_concern_type;
	typedef connection_container_type::org_write_concern_type org_write_concern_type;

	typedef connection_container_type::server_description_type server_description_type;
	typedef connection_container_type::org_server_description_type org_server_description_type;

	typedef connection_container_type::apm_callbacks_type apm_callbacks_type;
	typedef connection_container_type::org_apm_callbacks_type org_apm_callbacks_type;

	typedef connection_container_type::server_api_type server_api_type;
	typedef connection_container_type::org_server_api_type org_server_api_type;

private:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef c_mongo_connection this_type;

#ifdef MONGOC_ENABLE_SSL

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			mplex::null_type,
			ssl_opt_type,
			ssl_opt_type
		> ssl_opt_modify_helper_type;

#endif // MONGOC_ENABLE_SSL

public:
	c_mongo_connection(void);
	
	template<typename String>
	c_mongo_connection(const String& str_uri,
						typename 
							boost::enable_if
							<
								charset::utf8_string_constructable<String>,
								sfinae_type
							>::type sfinae = 0)
	{
		init_type init(str_uri);
		this_type::prv_set_connection_info(init);
	}

#ifdef MONGOC_ENABLE_SSL
	template<typename String1, 
				typename String2, typename String3,
				typename String4, typename String5,
				typename String6 >
	c_mongo_connection(const String1& str_uri,
						const String2& pem_file,
						const String3& pem_pwd,
						const String4& ca_file,
						const String5& ca_dir,
						const String6& crl_file,
						bool weak_cert_validation = true,
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
	{
		init_type init(str_uri,
						pem_file,
						pem_pwd,
						ca_file,
						ca_dir,
						crl_file,
						weak_cert_validation);

		this_type::prv_set_connection_info(init);
	}

#endif // MONGOC_ENABLE_SSL

	explicit c_mongo_connection(const init_type& init);
	~c_mongo_connection(void);

public:
	// don't use swap
	//void swap(this_type& right);

	//Compatible with connection pools
public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		try
		{
			return _wrap_conn.call_function(&connection_container_type::operator bool);
		}
		catch(const connection_container_wrap_type::error_type&)
		{
			return false;
		}
	}

	inline bool push(const mongoc_client_ptr_type& ptr)
	{
		try
		{
			return _wrap_conn.call_function(&connection_container_type::push, ptr);
		}
		catch(const connection_container_wrap_type::error_type&)
		{
			return false;
		}
	}

	inline mongoc_client_ptr_type pop(void)
	{
		try
		{
			return _wrap_conn.call_function(&connection_container_type::pop);
		}
		catch(const connection_container_wrap_type::error_type&)
		{
			return mongoc_client_ptr_type();
		}
	}

	inline base_mongoc_client_ptr_type revocable(base_mongoc_client_ptr_type ptr) const
	{
		try
		{
			return _wrap_conn.call_function(&connection_container_type::revocable, ptr);
		}
		catch(const connection_container_wrap_type::error_type&)
		{
			return 0;
		}
	}

	inline bool revoke(void)
	{
		try
		{
			return _wrap_conn.call_function(&connection_container_type::revoke);
		}
		catch(const connection_container_wrap_type::error_type&)
		{
			return false;
		}
	}

	inline bool revoke(base_mongoc_client_cptr_type ptr)
	{
		try
		{
			return _wrap_conn.call_function(&connection_container_type::check_revoke, ptr);
		}
		catch(const connection_container_wrap_type::error_type&)
		{
			return false;
		}
	}

public:
	inline uri_type uri(void) const
	{
		try
		{
			return _wrap_init.load().uri();
		}
		catch(const init_wrap_type::error_type&)
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
		catch(const init_wrap_type::error_type&)
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
		catch(const init_wrap_type::error_type&)
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
		//free client operator must befor ssl_opt
		_wrap_conn.clear();
		_wrap_init.clear();
	}

	inline bool empty(void) const
	{
		try
		{
			return _wrap_conn.call_function(&connection_container_type::empty);
		}
		catch(connection_container_wrap_type::error_type&)
		{
			return true;
		}
	}

	inline bool is_poped(void) const
	{
		try
		{
			return _wrap_conn.call_function(&connection_container_type::is_poped);
		}
		catch(const connection_container_wrap_type::error_type&)
		{
			return false;
		}
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
	inline id_type id(void) const
	{
		return utility::cast_pointer_to_int::cast<id_type>(this);
	}

	inline bool reinit(const init_type& init)
	{
		return this_type::prv_set_connection_info(init);
	}

public:
	// set_appname
	template<typename String> inline
	bool set_appname(const String& str_appname)
	{
		return this_type::prv_set_appname(str_appname);
	}

	// set_apm_callbacks
	template<typename ApmCallbacks> inline
	bool set_apm_callbacks(const ApmCallbacks& cbs, void* context = 0)
	{
		return 
			this_type::prv_set_amp_callbacks(
				const_cast<org_apm_callbacks_type*>(detail::to_const_pointer<org_apm_callbacks_type>(cbs)),
				context);
	}

	template<typename ApmCallbacks, typename Context> inline
	bool set_apm_callbacks(const ApmCallbacks& cbs, Context& context)
	{

		return 
			this_type::prv_set_amp_callbacks(
				const_cast<org_apm_callbacks_type*>(detail::to_const_pointer<org_apm_callbacks_type>(cbs)), 
				static_cast<void*>(boost::addressof(context)));
	}

	//set_error_api
	bool set_error_api(s32 version)
	{
		return this_type::prv_set_error_api(version);
	}

	// set_server_api
	template<typename ServerApi> inline
	bool set_server_api(const ServerApi& svr_api, org_bson_error_type* perr = 0)
	{
		return 
			this_type::prv_set_server_api(
				detail::to_const_pointer<org_server_api_type>(svr_api),
				perr);
	}

	template<typename ServerApi> inline
	bool set_server_api(const ServerApi& svr_api, org_bson_error_type& err)
	{
		return 
			this_type::prv_set_server_api(
				detail::to_const_pointer<org_server_api_type>(svr_api),
				boost::addressof(err));
	}

public:
	// throw empty_conn_err_type
	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		using_handler(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		handler_holder_type handler_holder(handler);
		return 
			_wrap_conn.call_function(
				&connection_container_type::using_any_handler<handler_type>, 
				utility::args_holder_trans(handler_holder));
	}

	// throw empty_conn_err_type
	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		using_handler(const Handler& handler) const
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		handler_holder_type handler_holder(handler);
		return 
			_wrap_conn.call_function(
				&connection_container_type::using_any_handler_const<handler_type>, 
				utility::args_holder_trans(handler_holder));
	}

private:
	inline bool prv_set_connection_info(const init_type& init)
	{
		return 
			_wrap_init.locked_unsafe_using_handler(
				boost::bind(&this_type::handler_prv_set_connection_info, this, _1, boost::cref(init)));
	}

	bool handler_prv_set_connection_info(init_wrap_type::base_type& base, const init_type& init);

	static bool handler_prv_reset_connection(connection_container_wrap_type::base_type& base,
												init_wrap_type::base_type& base_init,
												const init_type& init);

#ifdef MONGOC_ENABLE_SSL

	inline bool prv_set_ssl_opt(const ssl_opt_type& opt)
	{
		return 
			_wrap_init.locked_unsafe_using_handler(
				boost::bind(&this_type::handler_prv_set_ssl_opt, this, _1, boost::cref(opt)));
	}

	inline bool handler_prv_set_ssl_opt(init_wrap_type::base_type& base, const ssl_opt_type& opt)
	{
		return 
			(!!base)
			&& _wrap_conn.locked_unsafe_using_handler(
					boost::bind(
						&this_type::handler_prv_reset_ssl_opt, 
						_1, boost::ref(base),
						boost::cref(opt)));
	}

	static bool handler_prv_reset_ssl_opt(connection_container_wrap_type::base_type& base,
											init_wrap_type::base_type& base_init,
											const ssl_opt_type& opt);
#endif // MONGOC_ENABLE_SSL

	// appname
	template<typename String> inline
	bool prv_set_appname(const String& str_appname)
	{
		typedef typename native_t<String>::type string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string_type, utf8_string_type);

		utf8_string_type utf8_str_appname((charset::string_charset_helper_data(str_appname)));

		return 
			_wrap_conn.locked_unsafe_using_handler(
				boost::bind(&this_type::handler_prv_reset_appname, _1, boost::cref(utf8_str_appname)));
	}

	static bool handler_prv_reset_appname(connection_container_wrap_type::base_type& base,
											const inner_string_type& str_appname);


	// apm_callbacks
	inline bool prv_set_amp_callbacks(org_apm_callbacks_type* pcallbacks, void* context)
	{
		return
			_wrap_conn.locked_unsafe_using_handler(
				boost::bind(
					&this_type::handler_prv_reset_amp_callbacks, 
					_1, pcallbacks, context));
	}

	static bool handler_prv_reset_amp_callbacks(connection_container_wrap_type::base_type& base,
												org_apm_callbacks_type* pcallbacks, void* context);

	// set_error_api
	inline bool prv_set_error_api(s32 version)
	{
		switch(version)
		{
		case MONGOC_ERROR_API_VERSION_LEGACY:
		case MONGOC_ERROR_API_VERSION_2:
			break;
		default:
			return false;
		}

		return
			_wrap_conn.locked_unsafe_using_handler(
				boost::bind(
					&this_type::handler_prv_set_error_api, 
					_1, version));
	}

	static bool handler_prv_set_error_api(connection_container_wrap_type::base_type& base, s32 version);

	// set_server_api
	inline bool prv_set_server_api(const org_server_api_type* papi, org_bson_error_type* perr)
	{
		assert(papi);
		return
			papi
			&& _wrap_conn.locked_unsafe_using_handler(
				boost::bind(
					&this_type::handler_prv_set_server_api, 
					_1, papi, perr));
	}

	static bool handler_prv_set_server_api(connection_container_wrap_type::base_type& base, 
											const org_server_api_type* papi, org_bson_error_type* perr);

private:
	// don't change the following variable sequence
	init_wrap_type _wrap_init;
	connection_container_wrap_type _wrap_conn;
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECT_HPP__
