//basic_mongo_accesser.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_DATABASE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_DATABASE_HPP__

#include <yggr/nsql_database_system/mongo_accesser_types_decl.hpp>

namespace yggr
{
namespace nsql_database_system
{

class basic_mongo_accesser_database
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

private:
	typedef basic_mongo_accesser_database this_type;

	//------------------------database s------------------------
public:
	// database_aggregate
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_database_aggregate(Connection_T& conn, 
											const char* str_db,
											const org_bson_type* ppipeline, 
											const org_bson_type* popts,
											const org_read_prefs_type* pread_prefs,
											org_bson_error_type* perr, 
											const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && ppipeline))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser database_aggregate, command invalid args");
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
					"mongo_accesser database_aggregate, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser database_aggregate, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		mongoc_cursor_t* pcur = 
			mongoc_database_aggregate(
				pdb, ppipeline, popts, pread_prefs);

		if(!pcur)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser database_aggregate, query failed");
			}

			mongoc_database_destroy(pdb);
			
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		const org_bson_type* pdoc = 0;
		for(;mongoc_cursor_next(pcur, boost::addressof(pdoc));) 
		{
			if(!handler_collector(*reinterpret_cast<const bson_type*>(pdoc)))
			{
				break;
			}
		}

		bool bsuccess = !mongoc_cursor_error(pcur, perr);

		mongoc_cursor_destroy(pcur);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// create_collection
	template<typename Connection_T>
	static bool s_org_database_create_collecion(Connection_T& conn, 
												const char* str_db, 
												const char* str_coln, const org_bson_type* popts, 
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser create_collecion, command invalid args");
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
					"mongo_accesser create_collecion, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser create_collecion, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		mongoc_collection_t* pcoln = mongoc_database_create_collection(pdb, str_coln, popts, perr);
		bool bsuccess = !!pcoln;

		if(pcoln)
		{
			mongoc_collection_destroy(pcoln);
		}

		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// find_collections
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_database_find_collections(Connection_T& conn, const char* str_db,
												const org_bson_type* popts, 
												org_bson_error_type* perr,
												const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser find_collections, command invalid args");
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
					"mongo_accesser find_collections, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser find_collections, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);

			return false;
		}

		mongoc_cursor_t* pcur = mongoc_database_find_collections_with_opts(pdb, popts);

		if(!pcur)
		{
			mongoc_database_destroy(pdb);

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		const org_bson_type* pdoc = 0;
		for(;mongoc_cursor_next(pcur, boost::addressof(pdoc));) 
		{
			if(!handler_collector(*reinterpret_cast<const bson_type*>(pdoc)))
			{
				break;
			}
		}

		bool bsuccess = !mongoc_cursor_error(pcur, perr);

		mongoc_cursor_destroy(pcur);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// has_collection
	template<typename Connection_T>
	static bool s_org_database_has_collection(Connection_T& conn, 
												const char* str_db, 
												const char* str_coln,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser has_collection, command invalid args");
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
					"mongo_accesser has_collection, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser has_collection, not found %s", str_db);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_database_has_collection(pdb, str_coln, perr);

		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// get_collection_name
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_database_get_collection_names(Connection_T& conn,
													const char* str_db,
													const org_bson_type* popts,
													org_bson_error_type* perr,
													const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser get_collection_names, command invalid args");
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
					"mongo_accesser get_collection_names, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser get_collection_names, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		char** ppstr = mongoc_database_get_collection_names_with_opts(pdb, popts, perr);
		bool bsuccess = ppstr;

		if(!ppstr)
		{
			mongoc_database_destroy(pdb);
			
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		for(char** i = ppstr; i && (*i); ++i)
		{
			handler_collector(*i);
		}

		bson_strfreev(ppstr);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// database_drop
	template<typename Connection_T>
	static bool s_org_database_drop(Connection_T& conn, 
									const char* str_db,
									const org_bson_type* popts,
									org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser database_drop, command invalid args");
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
					"mongo_accesser database_drop, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser database_drop, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_database_drop_with_opts(pdb, popts, perr);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// database_add_user
	template<typename Connection_T>
	static bool s_org_database_add_user(Connection_T& conn, 
										const char* str_db, 
										const char* str_uname, const char* str_pwd, 
										const org_bson_type* proles, const org_bson_type* pcustom_data,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_uname && str_pwd && str_db))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser add_user, command invalid args");
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
					"mongo_accesser add_user, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser add_user, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_database_add_user(pdb, str_uname, str_pwd, proles, pcustom_data, perr);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// remove_user
	template<typename Connection_T>
	static bool s_org_database_remove_user(Connection_T& conn, 
											const char* str_db, 
											const char* str_uname, 
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_uname && str_db))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser database_remove_user, command invalid args");
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
					"mongo_accesser database_remove_user, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser database_remove_user, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);

			return false;
		}

		bool bsuccess = mongoc_database_remove_user(pdb, str_uname, perr);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// remove_all_users
	template<typename Connection_T>
	static bool s_org_database_remove_all_users(Connection_T& conn, const char* str_db, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!str_db)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser database_remove_all_users, command invalid args");
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
					"mongo_accesser database_remove_all_users, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser database_remove_all_users, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_database_remove_all_users(pdb, perr);

		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);
		
		return bsuccess;
	}

	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_database_watch(Connection_T& conn,
										const char* str_db,
										const org_bson_type* ppipeline,
										const org_bson_type* popts, 
										org_bson_type* preply,
										org_bson_error_type* perr,
										const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && ppipeline))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser database_watch, command invalid args");
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
					"mongo_accesser database_watch, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser database_watch, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);

			return false;
		}

		mongoc_change_stream_t* pchg_stream = mongoc_database_watch(pdb, ppipeline, popts);

		if(!pchg_stream)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser database_watch, query failed");
			}

			mongoc_database_destroy(pdb);

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
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// database_using_handler
	template<typename Connection_T, typename Handler>
	static
	typename
		boost::enable_if
		<
			boost::is_same<typename func::foo_t_info<Handler>::result_type, void>,
			void
		>::type
		s_org_database_using_handler(Connection_T& conn, const char* str_db, 
										org_bson_error_type* perr, const Handler& handler)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!str_db)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser database_using_handler, command invalid args");
			}
			return handler(static_cast<mongoc_database_t*>(0), perr);
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
					"mongo_accesser database_using_handler, mongoc_client is not ready");
			}

			return handler(static_cast<mongoc_database_t*>(0), perr);
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser database_using_handler, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);

			return handler(static_cast<mongoc_database_t*>(0), perr);
		}

		handler(pdb, perr);

		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);
		
	}

	template<typename Connection_T, typename Handler>
	static
	typename
		boost::disable_if
		<
			boost::is_same<typename func::foo_t_info<Handler>::result_type, void>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_org_database_using_handler(Connection_T& conn, const char* str_db, 
										org_bson_error_type* perr, const Handler& handler)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type result_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!str_db)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser database_using_handler, command invalid args");
			}
			return handler(static_cast<mongoc_database_t*>(0), perr);
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
					"mongo_accesser database_using_handler, mongoc_client is not ready");
			}

			return handler(static_cast<mongoc_database_t*>(0), perr);
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser database_using_handler, not found %s", str_db);
			}
			
			bool bpush = conn.push(ptr);
			assert(bpush);

			return handler(static_cast<mongoc_database_t*>(0), perr);
		}

		result_type rst(handler(pdb, perr));

		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);
		
		return rst;
	}

	//------------------------database e------------------------

};

} // namespace nsql_database_system
} // namespace yggr

//#ifdef _MSC_VER
//#	pragma warning( pop )
//#endif //_MSC_VER

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_DATABASE_HPP__
