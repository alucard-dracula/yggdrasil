// basic_mongo_accesser_client.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_CLIENT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_CLIENT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/nonfree_deleter.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/nsql_database_system/mongoc_command_mode_def.hpp>
#include <yggr/nsql_database_system/mongo_accesser_types_decl.hpp>

#include <yggr/nsql_database_system/c_mongo_uri.hpp>
#include <yggr/nsql_database_system/c_mongo_ssl_opt.hpp>
#include <yggr/nsql_database_system/c_mongo_server_description.hpp>
#include <yggr/nsql_database_system/c_mongo_apm_callbacks.hpp>
#include <yggr/nsql_database_system/c_mongo_server_api.hpp>
#include <yggr/nsql_database_system/c_mongo_auto_encryption_opts.hpp>

#include <yggr/nsql_database_system/detail/get_org_t.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<typename Ret>
struct client_using_handler_helper
{
public:
	typedef Ret ret_type;
	BOOST_MPL_ASSERT_NOT((boost::is_reference<ret_type>));

public:
	typedef mongoc_error_domain_ex_t mongoc_error_domain_type;
	typedef mongoc_error_code_ex_t mongoc_error_code_type;

public:
	template<typename Connection_T, typename Handler>
	ret_type operator()(Connection_T& conn, const Handler& handler, c_bson_error::org_type* perr) const
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		typedef typename connection_type::org_mongoc_client_ptr_type org_mongoc_client_ptr_type;

		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type check_ret_type;

		BOOST_MPL_ASSERT((boost::is_same<ret_type, check_ret_type>));

		bson_error_t err = {0};
		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_using_handler, mongoc_client is not ready");

				return handler(static_cast<org_mongoc_client_ptr_type>(0), perr);
			}
			else
			{
				bson_set_error(
					boost::addressof(err), 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_using_handler, mongoc_client is not ready");

				return handler(static_cast<org_mongoc_client_ptr_type>(0), boost::addressof(err));
			}
		}

		ret_type ret(handler(ptr.get(), perr));

		bool bpush = conn.push(ptr);
		assert(bpush);
		return ret;
	}
};

template<>
struct client_using_handler_helper<void>
{
public:
	typedef void ret_type;

public:
	typedef mongoc_error_domain_ex_t mongoc_error_domain_type;
	typedef mongoc_error_code_ex_t mongoc_error_code_type;

public:
	template<typename Connection_T, typename Handler>
	ret_type operator()(Connection_T& conn, const Handler& handler, c_bson_error::org_type* perr) const
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		typedef typename connection_type::org_mongoc_client_ptr_type org_mongoc_client_ptr_type;

		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type check_ret_type;

		BOOST_MPL_ASSERT((boost::is_same<ret_type, check_ret_type>));
			
		bson_error_t err = {0};
		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			bson_error_t* pout_err = perr? perr : boost::addressof(err);
			assert(pout_err);

			bson_set_error(
				pout_err, 
				mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
				mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
				"mongo_accesser client_using_handler, mongoc_client is not ready");

			handler(static_cast<org_mongoc_client_ptr_type>(0), pout_err);

			return;
		}

		handler(ptr.get(), perr);

		bool bpush = conn.push(ptr);
		assert(bpush);
	}
};

} // namespace detail
} // namespace database_system
} // namespace yggr


namespace yggr
{
namespace nsql_database_system
{

class basic_mongo_accesser_client
{
public:
	typedef mongo_accesser_types_decl types_decl_type;

public:
	typedef types_decl_type::inner_string_type inner_string_type;
	typedef types_decl_type::inner_string_view_type inner_string_view_type;

	typedef types_decl_type::bson_type bson_type;
	typedef types_decl_type::org_bson_type org_bson_type;

	typedef types_decl_type::bson_value_type bson_value_type;
	typedef types_decl_type::org_bson_value_type org_bson_value_type;

	typedef types_decl_type::bson_error_type bson_error_type;
	typedef types_decl_type::org_bson_error_type org_bson_error_type;

	typedef types_decl_type::write_concern_type write_concern_type;
	typedef types_decl_type::org_write_concern_type org_write_concern_type;

	typedef types_decl_type::read_prefs_type read_prefs_type;
	typedef types_decl_type::org_read_prefs_type org_read_prefs_type;

	typedef types_decl_type::index_model_type index_model_type;
	typedef types_decl_type::org_index_model_type org_index_model_type;

	typedef types_decl_type::find_and_modify_opts_type find_and_modify_opts_type;
	typedef types_decl_type::org_find_and_modify_opts_type org_find_and_modify_opts_type;

	typedef types_decl_type::read_concern_type read_concern_type;
	typedef types_decl_type::org_read_concern_type org_read_concern_type;

	typedef types_decl_type::mongoc_error_domain_type mongoc_error_domain_type;
	typedef types_decl_type::mongoc_error_code_type mongoc_error_code_type;

public:
	typedef c_mongo_uri uri_type;
	typedef uri_type::org_type org_uri_type;

	typedef c_mongo_ssl_opt ssl_opt_type;
	typedef ssl_opt_type::org_type org_ssl_opt_type;

	typedef c_mongo_server_description server_description_type;
	typedef server_description_type::org_type org_server_description_type;

	typedef mongoc_stream_initiator_t stream_initiator_type;
	typedef stream_initiator_type org_stream_initiator_type;

	typedef c_mongo_apm_callbacks apm_callbacks_type;
	typedef apm_callbacks_type::org_type org_apm_callbacks_type;

	typedef c_mongo_server_api server_api_type;
	typedef server_api_type::org_type org_server_api_type;

	typedef c_mongo_auto_encryption_opts auto_encryption_opts_type;
	typedef auto_encryption_opts_type::org_type org_auto_encryption_opts_type;

private:
	typedef basic_mongo_accesser_client this_type;

	//------------------------client s---------------------------

public:
	// run_client_command_simple
	template<typename Connection_T>
	static bool s_org_client_command_simple(Connection_T& conn, 
											const char* str_db, 
											const org_bson_type* pcmd,
											const org_read_prefs_type* pread_prefs,
											org_bson_type* preply,
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pcmd))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_command_simple, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_command_simple, mongoc_client is not ready");
			}

			return false;
		}

		bool bsuccess = mongoc_client_command_simple(ptr.get(), str_db, pcmd, pread_prefs, preply, perr);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_client_command_simple_with_server_id(Connection_T& conn, 
															const char* str_db, 
															const org_bson_type* pcmd,
															const org_read_prefs_type* pread_prefs,
															u32 server_id,
															org_bson_type* preply,
															org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pcmd))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_command_simple_with_server_id, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_command_simple_with_server_id, mongoc_client is not ready");
			}

			return false;
		}

		bool bsuccess = 
			mongoc_client_command_simple_with_server_id(
				ptr.get(), str_db, pcmd, pread_prefs, server_id, preply, perr);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// collection_command_with_opts
	template<typename Connection_T>
	static bool s_org_client_command_with_opts(Connection_T& conn, 
												const char* str_db, 
												const org_bson_type* pcmd,
												const org_read_prefs_type* pread_prefs,
												const org_bson_type* popts,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pcmd))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_command_with_opts, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_command_with_opts, mongoc_client is not ready");
			}

			return false;
		}

		bool bsuccess = mongoc_client_command_with_opts(ptr.get(), str_db, pcmd, pread_prefs, popts, preply, perr);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_client_read_command_with_opts(Connection_T& conn, 
													const char* str_db,
													const org_bson_type* pcmd,
													const org_read_prefs_type* pread_prefs,
													const org_bson_type* popts,
													org_bson_type* preply,
													org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pcmd))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_read_command_with_opts, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_read_command_with_opts, mongoc_client is not ready");
			}

			return false;
		}

		bool bsuccess = mongoc_client_read_command_with_opts(ptr.get(), str_db, pcmd, pread_prefs, popts, preply, perr);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_client_read_write_command_with_opts(Connection_T& conn, 
															const char* str_db,
															const org_bson_type* pcmd,
															const org_read_prefs_type* pread_prefs,
															const org_bson_type* popts,
															org_bson_type* preply,
															org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pcmd))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_read_write_command_with_opts, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_read_write_command_with_opts, mongoc_client is not ready");
			}

			return false;
		}

		bool bsuccess =
			mongoc_client_read_write_command_with_opts(
				ptr.get(), str_db, pcmd, pread_prefs, popts, preply, perr);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_client_write_command_with_opts(Connection_T& conn, 
														const char* str_db,
														const org_bson_type* pcmd,
														const org_bson_type* popts,
														org_bson_type* preply,
														org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pcmd))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_write_command_with_opts, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_write_command_with_opts, mongoc_client is not ready");
			}

			return false;
		}

		bool bsuccess = mongoc_client_write_command_with_opts(ptr.get(), str_db, pcmd, popts, preply, perr);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T> inline
	static bool s_org_client_command(Connection_T& conn, 
										const char* str_db,
										u32 flags,
										const org_bson_type* pcmd,
										const org_read_prefs_type* pread_prefs,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		switch(flags)
		{
		case mongoc_command_mode_def::E_MONGOC_CMD_RAW:
			return 
				this_type::s_org_client_command_with_opts(
						conn, str_db, pcmd, pread_prefs, popts, preply, perr);

		case mongoc_command_mode_def::E_MONGOC_CMD_READ:
			return 
				this_type::s_org_client_read_command_with_opts(
					conn, str_db, pcmd, pread_prefs, popts, preply, perr);

		case mongoc_command_mode_def::E_MONGOC_CMD_WRITE:
			return 
				this_type::s_org_client_write_command_with_opts(
					conn, str_db, pcmd, popts, preply, perr);

		case mongoc_command_mode_def::E_MONGOC_CMD_RW:
			return 
				this_type::s_org_client_read_write_command_with_opts(
					conn, str_db, pcmd, 0, popts, preply, perr);

		default:
			{
				if(perr)
				{
					bson_set_error(
						perr, 
						mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
						mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
						"mongo_accesser client_command, command invalid args");
				}

				assert(false);
				return false;
			}
		}
	}

	// s_org_client_get_crypt_shared_version
protected:
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			inner_string_type&
		>::type
		pro_s_org_client_get_crypt_shared_version(inner_string_type& out_str_ver, Connection_T& conn, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_get_crypt_shared_version, mongoc_client is not ready");
			}

			return out_str_ver;
		}

		const char* pstr_ver = mongoc_client_get_crypt_shared_version(ptr.get());

		out_str_ver.assign((pstr_ver? pstr_ver : ""), YGGR_STR_UTF8_STRING_CHARSET_NAME());

		bool bpush = conn.push(ptr);
		assert(bpush);

		return out_str_ver;
	}

public:
	template<typename Connection_T> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			inner_string_type
		>::type
		s_org_client_get_crypt_shared_version(Connection_T& conn, org_bson_error_type* perr)
	{
		inner_string_type str_ver;
		this_type::pro_s_org_client_get_crypt_shared_version(str_ver, conn, perr);
		return str_ver;
	}

	template<typename Connection_T, typename Alloc> inline
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			inner_string_type
		>::type
		s_org_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc, org_bson_error_type* perr)
	{
		inner_string_type str_ver(alloc);
		this_type::pro_s_org_client_get_crypt_shared_version(str_ver, conn, perr);
		return str_ver;
	}

	// s_org_client_is_crypt_enabled
	template<typename Connection_T> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_is_crypt_enabled(Connection_T& conn)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		client_ptr_type ptr = conn.pop();

		bool ret = ptr && mongoc_client_get_crypt_shared_version(ptr.get());

		if(ptr)
		{
			bool bpush = conn.push(ptr);
			assert(bpush);
		}

		return ret;
	}

	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_client_watch(Connection_T& conn, 
									const org_bson_type* ppipeline,
									const org_bson_type* popts,
									org_bson_type* preply,
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(ppipeline))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_watch, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_watch, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_change_stream_t* pchg_stream = mongoc_client_watch(ptr.get(), ppipeline, popts);

		if(!pchg_stream)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY, 
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser client_watch, query failed");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		const org_bson_type* pdoc = 0;
		for(;mongoc_change_stream_next(pchg_stream, boost::addressof(pdoc));) 
		{
			if(!handler_collector(*reinterpret_cast<const bson_type*>(pdoc)))
			{
				break;
			}
		}

		const org_bson_type* ptmp_reply = 0;
		bool bsuccess = !mongoc_change_stream_error_document(pchg_stream, perr, boost::addressof(ptmp_reply));

		if(preply && ptmp_reply)
		{
			bson_native_ex::s_bson_copy(preply, ptmp_reply);
		}

		mongoc_change_stream_destroy(pchg_stream);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// get set foo support connection noly
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_client_get_database_names(Connection_T& conn,
												const org_bson_type* popts,
												org_bson_error_type* perr,
												const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser gridfs_save_file, mongoc_client is not ready");
			}

			return false;
		}

		char** ppstr = mongoc_client_get_database_names_with_opts(ptr.get(), popts, perr);

		if(!ppstr)
		{
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		for(char** i = ppstr; i && (*i); ++i)
		{
			handler_collector(*i);
		}

		bson_strfreev(ppstr);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return true;
	}

	//get_server_description
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_org_client_get_server_description(Connection_T& conn, u32 svr_id, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_server_description, mongoc_client is not ready");
			}

			return server_description_type();
		}

		org_server_description_type* pdesc = mongoc_client_get_server_description(ptr.get(), svr_id);

		if(!pdesc)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_server_description, mongoc_client is not ready");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return server_description_type();
		}

		server_description_type ret(pdesc);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return ret;
	}

	//get_server_descriptions
	// return mongoc_client_get_server_descriptions's 'n', collector return false stop collecting,
	// so container.size() <= return_value 
	template<typename Connection_T, typename HandlerCollector>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			std::size_t
		>::type
		s_org_client_get_server_descriptions(Connection_T& conn, 
												org_bson_error_type* perr,
												const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_server_description, mongoc_client is not ready");
			}

			return 0;
		}

		std::size_t desc_count = 0;
		org_server_description_type** ppdesc = 
			mongoc_client_get_server_descriptions(ptr.get(), boost::addressof(desc_count));

		if(!ppdesc)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_server_description, mongoc_client is not ready");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return 0;
		}

		for(std::size_t i = 0; i != desc_count; ++i)
		{
			server_description_type desc(ppdesc[i]);
			if(!handler_collector(boost::ref(desc)))
			{
				mongoc_server_description_native_ex::s_destroy_all_exclude_outter_array(
					ppdesc + i, desc_count - i);
				break;
			}
		}

		bson_free(ppdesc);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return desc_count;
	}

	// mongoc_client_select_server 
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_org_client_select_server(Connection_T& conn,
									bool for_writes, 
									const org_read_prefs_type* pread_prefs,
									org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser select_server, mongoc_client is not ready");
			}

			return server_description_type();
		}

		org_server_description_type* pdesc = mongoc_client_select_server(ptr.get(), for_writes, pread_prefs, perr);

		if(!pdesc)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser select_server, mongoc_client is not ready");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return server_description_type();
		}

		server_description_type ret(pdesc);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return ret;
	}

	//get_read_concern
	template<typename Connection_T>
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			read_concern_type
		>::type
		s_org_client_get_read_concern(Connection_T& conn, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_read_concern, mongoc_client is not ready");
			}

			return read_concern_type();
		}

		const org_read_concern_type* pread_concern = mongoc_client_get_read_concern(ptr.get());

		if(!pread_concern)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_read_concern, mongoc_client is not ready");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return read_concern_type();
		}

		read_concern_type ret(pread_concern);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return ret;
	}

	//get_read_prefs
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			read_prefs_type
		>::type
		s_org_client_get_read_prefs(Connection_T& conn, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_read_prefs, mongoc_client is not ready");
			}

			return read_prefs_type();
		}

		const org_read_prefs_type* pread_prefs = mongoc_client_get_read_prefs(ptr.get());

		if(!pread_prefs)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_read_prefs, mongoc_client is not ready");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return read_prefs_type();
		}

		read_prefs_type ret(pread_prefs);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return ret;
	}

	template<typename Connection_T> inline
	static uri_type s_org_client_get_uri(Connection_T& conn)
	{
		return conn.uri();
	}

	//get_write_concern
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			write_concern_type
		>::type
		s_org_client_get_write_concern(Connection_T& conn, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_write_concern, mongoc_client is not ready");
			}

			return write_concern_type();
		}

		const org_write_concern_type* pwrite_concern = mongoc_client_get_write_concern(ptr.get());

		if(!pwrite_concern)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_write_concern, mongoc_client is not ready");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return write_concern_type();
		}

		write_concern_type ret(pwrite_concern);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return ret;
	}

	// set_read_concern
	template<typename Connection_T>
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_set_read_concern(Connection_T& conn, 
										const org_read_concern_type* pread_concern,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		assert((pread_concern));
		if(!(pread_concern))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser set_read_concern, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser set_read_concern, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_client_set_read_concern(ptr.get(), pread_concern);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return true;
	}

	// set_read_prefs
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_set_read_prefs(Connection_T& conn, 
									const org_read_prefs_type* pread_prefs,
									org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		assert((pread_prefs));
		if(!(pread_prefs))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser set_read_prefs, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser set_read_prefs, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_client_set_read_prefs(ptr.get(), pread_prefs);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return true;
	}

	//get_set_ssl_opts
#ifdef MONGOC_ENABLE_SSL
	
	template<typename Connection_T> inline
	static ssl_opt_type s_org_client_get_ssl_opts(Connection_T& conn)
	{
		return conn.ssl_opt();
	}

	template<typename Connection_T> inline
	static bool s_org_client_set_ssl_opts(Connection_T& conn, 
											const org_ssl_opt_type* pssl_opt)
	{
		return conn.ssl_opt(pssl_opt);
	}

#endif //MONGOC_ENABLE_SSL

	//set_write_concern
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_set_write_concern(Connection_T& conn, 
										const org_write_concern_type* pwrite_concern,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		assert((pwrite_concern));
		if(!(pwrite_concern))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser set_write_concern, command invalid args");
			}
			return false;
		}

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser set_write_concern, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_client_set_write_concern(ptr.get(), pwrite_concern);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return true;
	}

	// set_stream_initiator
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_set_stream_initiator(Connection_T& conn, 
											org_stream_initiator_type pfoo_initator,
											void* puser_data,
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser set_stream_initiator, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_client_set_stream_initiator(ptr.get(), pfoo_initator, puser_data? puser_data : static_cast<void*>(ptr.get()));

		bool bpush = conn.push(ptr);
		assert(bpush);

		return true;
	}

	template<typename Connection_T, typename Handler>
	static 
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				boost::is_same<void*, typename func::foo_t_info<Handler>::result_type>
			>,
			bool
		>::type
		s_org_client_set_stream_initiator(Connection_T& conn, 
											org_stream_initiator_type pfoo_initator,
											const Handler& handler_user_data,
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser set_stream_initiator, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_client_set_stream_initiator(ptr.get(), pfoo_initator, handler_user_data(ptr));

		bool bpush = conn.push(ptr);
		assert(bpush);

		return true;
	}

	// mongo_client_reset
public:
	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_reset(Connection_T& conn, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser set_stream_initiator, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_client_reset(ptr.get());

		bool bpush = conn.push(ptr);
		assert(bpush);

		return true;
	}

	// client_set_appname
	template<typename Connection_T, typename String> inline
	static bool s_org_client_set_appname(Connection_T& conn, const String& str_appname)
	{
		return conn.set_appname(str_appname);
	}

	template<typename Connection_T>
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_org_client_get_handshake_description(Connection_T& conn,
												u32 server_id,
												const org_bson_type* opt, /*unused*/
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		client_ptr_type ptr = conn.pop();

		if(!ptr)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser get_handshake_description, mongoc_client is not ready");
			}

			return server_description_type();
		}

		server_description_type svr_desc(mongoc_client_get_handshake_description(ptr.get(), server_id, 0, perr));

		bool bpush = conn.push(ptr);
		assert(bpush);

		return svr_desc;
	}

	// client_set_apm_callbacks
	template<typename Connection_T, typename ApmCallBacks> inline
	static bool s_org_client_set_apm_callbacks(Connection_T& conn, const ApmCallBacks& cbs, void* pcontex = 0)
	{
		return conn.set_apm_callbacks(cbs, pcontex);
	}

	template<typename Connection_T, typename ApmCallBacks, typename Context> inline
	static bool s_org_client_set_apm_callbacks(Connection_T& conn, const ApmCallBacks& cbs, Context& contex)
	{
		return conn.set_apm_callbacks(cbs, contex);
	}

	// client_set_error_api
	template<typename Connection_T> inline
	static bool s_org_client_set_error_api(Connection_T& conn, s32 version)
	{
		return conn.set_error_api(version);
	}

	//s_org_client_set_server_api
	template<typename Connection_T, typename ServerApi> inline
	static bool s_org_client_set_server_api(Connection_T& conn, const ServerApi& svr_api, org_bson_error_type* perr = 0)
	{
		return conn.set_server_api(svr_api, perr);
	}

	// s_org_client_enable_auto_encryption 
	// client version
protected:
	template<typename ConnSharedPtr>
	static bool pro_s_org_client_handler_enable_auto_encryption_client(ConnSharedPtr& pconn_out,
																		ConnSharedPtr& pkeyvalut_conn_out,
																		ConnSharedPtr pconn,
																		ConnSharedPtr pkeyvalut_conn,
																		org_auto_encryption_opts_type* popts, 
																		org_bson_error_type* perr)
	{
		typedef ConnSharedPtr connection_ptr_type;
		typedef typename mplex::pointer_to_value_t<connection_ptr_type>::type connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		assert(pconn && popts);
		if(!(pconn && popts))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser enable_auto_encryption, command invalid args");
			}

			return false;
		}

		if((pconn_out || pkeyvalut_conn_out))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser enable_auto_encryption, mongo connection was enabled");
			}

			return false;
		}

		if(pconn == pkeyvalut_conn)
		{
			connection_ptr_type tmp;
			tmp.swap(pkeyvalut_conn);
		}

		// !!!pclt_keyvalut pop must befroe pconn pop !!!
		client_ptr_type pclt_keyvalut;
		
		if(pkeyvalut_conn)
		{
			pclt_keyvalut = pkeyvalut_conn->pop(); // 1

			if(!pclt_keyvalut)
			{
				if(perr)
				{
					bson_set_error(
						perr, 
						mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
						mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
						"mongo_accesser enable_auto_encryption, mongoc_keyvault_client is not ready");
				}

				return false;
			}
		}

		client_ptr_type pclt = pconn->pop(); // 2

		if(!pclt)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser enable_auto_encryption, mongoc_client is not ready");
			}

			if(pclt_keyvalut)
			{
				assert(pkeyvalut_conn);
				bool bpush = pkeyvalut_conn->push(pclt_keyvalut);
				assert(bpush);
			}

			return false;
		}

		if(pclt_keyvalut)
		{
			mongoc_auto_encryption_opts_set_keyvault_client(popts, pclt_keyvalut.get());
		}

		bool bsuccess = mongoc_client_enable_auto_encryption(pclt.get(), popts, perr);
		if(bsuccess)
		{
			pkeyvalut_conn_out = pkeyvalut_conn;
			pconn_out = pconn;
		}

		bool bpush = (!pkeyvalut_conn) || (pkeyvalut_conn->push(pclt_keyvalut));
		assert(bpush);

		bpush = pconn->push(pclt); 
		assert(bpush);

		return bsuccess;
	}

public:
	template<typename EncConnect, typename Connection_T> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_enable_auto_encryption(EncConnect& enc_conn,
											smart_ptr_ex::shared_ptr<Connection_T> pconn,
											smart_ptr_ex::shared_ptr<Connection_T> pkeyvalut_conn,
											org_auto_encryption_opts_type* popts, 
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		assert(pconn && popts);
		if(!(pconn && popts))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser enable_auto_encryption, command invalid args");
			}
			return false;
		}

		return 
			enc_conn.init(
				boost::bind(
					&this_type::pro_s_org_client_handler_enable_auto_encryption_client<connection_ptr_type>,
					_1, _2, pconn, pkeyvalut_conn, popts, perr));
	}

	template<typename EncConnect, typename Connection_T> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_enable_auto_encryption(EncConnect& enc_conn,
											Connection_T& conn,
											smart_ptr_ex::shared_ptr<Connection_T> pkeyvalut_conn,
											org_auto_encryption_opts_type* popts, 
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;
		typedef smart_ptr_ex::nonfree_deleter<connection_type> now_connection_deleter_type;

		connection_ptr_type pconn(boost::addressof(conn), now_connection_deleter_type());
		
		return this_type::s_org_client_enable_auto_encryption(enc_conn, pconn, pkeyvalut_conn, popts, perr);
	}

	template<typename EncConnect, typename Connection_T> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_enable_auto_encryption(EncConnect& enc_conn,
											smart_ptr_ex::shared_ptr<Connection_T> pconn,
											Connection_T& keyvalut_conn,
											org_auto_encryption_opts_type* popts, 
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;
		typedef smart_ptr_ex::nonfree_deleter<connection_type> now_connection_deleter_type;

		connection_ptr_type pkeyvalut_conn(boost::addressof(keyvalut_conn), now_connection_deleter_type());
		
		return this_type::s_org_client_enable_auto_encryption(enc_conn, pconn, pkeyvalut_conn, popts, perr);
	}

	template<typename EncConnect, typename Connection_T> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_org_client_enable_auto_encryption(EncConnect& enc_conn,
											Connection_T& conn,
											Connection_T& keyvalut_conn,
											org_auto_encryption_opts_type* popts, 
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;
		typedef smart_ptr_ex::nonfree_deleter<connection_type> now_connection_deleter_type;

		connection_ptr_type pconn(boost::addressof(conn), now_connection_deleter_type());
		connection_ptr_type pkeyvalut_conn(boost::addressof(keyvalut_conn), now_connection_deleter_type());
		
		return this_type::s_org_client_enable_auto_encryption(enc_conn, pconn, pkeyvalut_conn, popts, perr);
	}

	// client_pool version
protected:
	template<typename ConnPoolSharedPtr>
	static bool pro_s_org_client_handler_enable_auto_encryption_client_pool(ConnPoolSharedPtr& pconn_pool_out,
																			ConnPoolSharedPtr& pkeyvalut_conn_pool_out,
																			ConnPoolSharedPtr pconn_pool,
																			ConnPoolSharedPtr pkeyvalut_conn_pool,
																			org_auto_encryption_opts_type* popts, 
																			org_bson_error_type* perr)
	{
		typedef ConnPoolSharedPtr connection_pool_ptr_type;
		typedef typename mplex::pointer_to_value_t<connection_pool_ptr_type>::type connection_pool_type;
		typedef typename connection_pool_type::mongoc_client_pool_ptr_type client_pool_ptr_type;

		assert(pconn_pool && popts);
		if(!(pconn_pool && popts))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser enable_auto_encryption, command invalid args");
			}

			return false;
		}

		if((pconn_pool_out || pkeyvalut_conn_pool_out))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser enable_auto_encryption, mongo connection pool was enabled");
			}

			return false;
		}

		if(pconn_pool == pkeyvalut_conn_pool)
		{
			connection_pool_ptr_type tmp;
			tmp.swap(pkeyvalut_conn_pool);
		}

		client_pool_ptr_type pclt_pool_keyvalut;
		
		if(pkeyvalut_conn_pool)
		{
			pclt_pool_keyvalut = pkeyvalut_conn_pool->client_pool_pointer();

			if(!pclt_pool_keyvalut)
			{
				if(perr)
				{
					bson_set_error(
						perr, 
						mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
						mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
						"mongo_accesser enable_auto_encryption, mongoc_keyvault_client_pool is not ready");
				}

				return false;
			}
		}

		client_pool_ptr_type pclt_pool = pconn_pool->client_pool_pointer(); 

		if(!pclt_pool)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT, 
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser enable_auto_encryption, mongoc_client_pool is not ready");
			}

			return false;
		}

		if(pclt_pool_keyvalut)
		{
			mongoc_auto_encryption_opts_set_keyvault_client_pool(popts, pclt_pool_keyvalut.get());
		}

		bool bsuccess = mongoc_client_pool_enable_auto_encryption(pclt_pool.get(), popts, perr);
		if(bsuccess)
		{
			pkeyvalut_conn_pool_out = pkeyvalut_conn_pool;
			pconn_pool_out = pconn_pool;
		}

		return bsuccess;
	}

public:
	template<typename EncConnectPool, typename ConnectPool> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_pool_t, typename detail::get_org_t<ConnectPool>::type>,
			bool
		>::type
		s_org_client_enable_auto_encryption(EncConnectPool& enc_conn_pool,
											smart_ptr_ex::shared_ptr<ConnectPool> pconn_pool,
											smart_ptr_ex::shared_ptr<ConnectPool> pkeyvalut_conn_pool,
											org_auto_encryption_opts_type* popts, 
											org_bson_error_type* perr)
	{
		typedef ConnectPool connection_pool_type;
		typedef smart_ptr_ex::shared_ptr<connection_pool_type> connection_pool_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		assert(pconn_pool && popts);
		if(!(pconn_pool && popts))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser enable_auto_encryption, command invalid args");
			}
			return false;
		}

		return 
			enc_conn_pool.init(
				boost::bind(
					&this_type::pro_s_org_client_handler_enable_auto_encryption_client_pool<connection_pool_ptr_type>,
					_1, _2, pconn_pool, pkeyvalut_conn_pool, popts, perr));
	}

	template<typename EncConnectPool, typename ConnectPool> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_pool_t, typename detail::get_org_t<ConnectPool>::type>,
			bool
		>::type
		s_org_client_enable_auto_encryption(EncConnectPool& enc_conn_pool,
											ConnectPool& conn_pool,
											smart_ptr_ex::shared_ptr<ConnectPool> pkeyvalut_conn_pool,
											org_auto_encryption_opts_type* popts, 
											org_bson_error_type* perr)
	{
		typedef ConnectPool connection_pool_type;
		typedef smart_ptr_ex::shared_ptr<connection_pool_type> connection_pool_ptr_type;
		typedef smart_ptr_ex::nonfree_deleter<connection_pool_type> now_connection_pool_deleter_type;

		connection_pool_ptr_type pconn_pool(boost::addressof(conn_pool), now_connection_pool_deleter_type());
		
		return this_type::s_org_client_enable_auto_encryption(enc_conn_pool, pconn_pool, pkeyvalut_conn_pool, popts, perr);
	}

	template<typename EncConnectPool, typename ConnectPool> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_pool_t, typename detail::get_org_t<ConnectPool>::type>,
			bool
		>::type
		s_org_client_enable_auto_encryption(EncConnectPool& enc_conn_pool,
											smart_ptr_ex::shared_ptr<ConnectPool> pconn_pool,
											ConnectPool& keyvalut_conn_pool,
											org_auto_encryption_opts_type* popts, 
											org_bson_error_type* perr)
	{
		typedef ConnectPool connection_pool_type;
		typedef smart_ptr_ex::shared_ptr<connection_pool_type> connection_pool_ptr_type;
		typedef smart_ptr_ex::nonfree_deleter<connection_pool_type> now_connection_pool_deleter_type;

		connection_pool_ptr_type pkeyvalut_conn_pool(boost::addressof(keyvalut_conn_pool), now_connection_pool_deleter_type());
		
		return this_type::s_org_client_enable_auto_encryption(enc_conn_pool, pconn_pool, pkeyvalut_conn_pool, popts, perr);
	}

	template<typename EncConnectPool, typename ConnectPool> inline
	static 
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_pool_t, typename detail::get_org_t<ConnectPool>::type>,
			bool
		>::type
		s_org_client_enable_auto_encryption(EncConnectPool& enc_conn_pool,
											ConnectPool& conn_pool,
											ConnectPool& keyvalut_conn_pool,
											org_auto_encryption_opts_type* popts, 
											org_bson_error_type* perr)
	{
		typedef ConnectPool connection_pool_type;
		typedef smart_ptr_ex::shared_ptr<connection_pool_type> connection_pool_ptr_type;
		typedef smart_ptr_ex::nonfree_deleter<connection_pool_type> now_connection_pool_deleter_type;

		connection_pool_ptr_type pconn_pool(boost::addressof(conn_pool), now_connection_pool_deleter_type());
		connection_pool_ptr_type pkeyvalut_conn_pool(boost::addressof(keyvalut_conn_pool), now_connection_pool_deleter_type());
		
		return this_type::s_org_client_enable_auto_encryption(enc_conn_pool, pconn_pool, pkeyvalut_conn_pool, popts, perr);
	}


	//// !!! don't append mongoc_handshake_data_append, because implement to mongodb_installer !!!

	// client_using_handler
public:
	template<typename Connection_T, typename Handler> inline
	static typename func::foo_t_info<Handler>::result_type 
		s_org_client_using_handler(Connection_T& conn, 
									const Handler& handler, 
									org_bson_error_type* perr)
	{
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;
		
		//BOOST_MPL_ASSERT_NOT((boost::is_reference<ret_type>));

		typedef detail::client_using_handler_helper<ret_type> h_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		h_type h;
		return h(conn, handler, perr);
	}
	
	//------------------------client e---------------------------

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_CLIENT_HPP__
