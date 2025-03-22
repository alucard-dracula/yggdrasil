// basic_mongo_accesser_collection.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_COLLECTION_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_COLLECTION_HPP__

#include <yggr/nsql_database_system/mongo_accesser_types_decl.hpp>
#include <yggr/nsql_database_system/mongoc_collection_native_ex.hpp>

#include <yggr/nsql_database_system/mongoc_command_mode_def.hpp>

#include <yggr/nsql_database_system/mongo_const_strings_def.hpp>
#include <yggr/nsql_database_system/mongo_operators_dollar_cmd_def.hpp>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

//upsert select copy check

struct exclude_upsert_checker
{
public:
	inline bool operator()(const char* name) const
	{
		return !(0 == strcmp(name, mongo_const_strings_def::upsert().c_str()));
	}
};

} // namespace detail
} // namespace database_system
} // namespace yggr


namespace yggr
{
namespace nsql_database_system
{

class basic_mongo_accesser_collection
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
	typedef basic_mongo_accesser_collection this_type;

	//------------------------collection s------------------------

public:
	// insert
	template<typename Connection_T>
	static bool s_org_collection_insert_one(Connection_T& conn,
												const char* str_db, const char* str_coln,
												const org_bson_type* pdoc,
												const org_bson_type* popt,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pdoc))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_insert, command invalid args");
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
					"mongo_accesser collection_insert, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_INSERT,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_insert, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_insert_one(pcoln, pdoc, popt, preply, perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_collection_insert_many(Connection_T& conn,
												const char* str_db, const char* str_coln,
												const org_bson_type** ppbs, u32 n_documents,
												const org_bson_type* popts,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && ppbs && n_documents))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_insert, command invalid args");
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
					"mongo_accesser collection_insert, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_INSERT,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_insert, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_insert_many(pcoln, ppbs, n_documents, popts, preply, perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// remove
	template<typename Connection_T>
	static bool s_org_collection_delete_one(Connection_T& conn,
											const char* str_db, const char* str_coln,
											const org_bson_type* psel, const org_bson_type* popts,
											org_bson_type* preply, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && psel))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_delete_one, command invalid args");
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
					"mongo_accesser collection_delete_one, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_delete_one, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_delete_one(pcoln, psel, popts, preply, perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_collection_delete_many(Connection_T& conn,
												const char* str_db, const char* str_coln,
												const org_bson_type* psel, const org_bson_type* popts,
												org_bson_type* preply, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && psel))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_delete_many, command invalid args");
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
					"mongo_accesser collection_delete_many, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_delete_many, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_delete_many(pcoln, psel, popts, preply, perr);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T> inline
	static bool s_org_collection_remove(Connection_T& conn,
										const char* str_db, const char* str_coln,
										u32 flags, const org_bson_type* psel, const org_bson_type* popts,
										org_bson_type* preply, org_bson_error_type* perr)
	{
		switch(flags)
		{
		case MONGOC_DELETE_SINGLE_REMOVE:
			return s_org_collection_delete_one(conn, str_db, str_coln, psel, popts, preply, perr);
		case MONGOC_REMOVE_NONE:
			return s_org_collection_delete_many(conn, str_db, str_coln, psel, popts, preply, perr);
		default:
			{
				if(perr)
				{
					bson_set_error(
						perr,
						mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
						mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
						"mongo_accesser collection_remove, command invalid args");
				}

				assert(false);
				return false;
			}
		}
	}

	// find
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_collection_find(Connection_T& conn,
										const char* str_db, const char* str_coln,
										const org_bson_type* pfilter,
										const org_bson_type* popts,
										const org_read_prefs_type* pread_prefs,
										org_bson_error_type* perr,
										const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pfilter))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_find, command invalid args");
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
					"mongo_accesser collection_find, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_find, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		mongoc_cursor_t* pcur = mongoc_collection_find_with_opts(pcoln, pfilter, popts, pread_prefs);

		if(!pcur)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser collection_find, query failed");
			}

			mongoc_collection_destroy(pcoln);

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
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// aggregate
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_collection_aggregate(Connection_T& conn,
											const char* str_db, const char* str_coln,
											u32 flags,
											const org_bson_type* ppipeline,
											const org_bson_type* popts,
											const org_read_prefs_type* pread_prefs,
											org_bson_error_type* perr,
											const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && ppipeline))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_aggregate, command invalid args");
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
					"mongo_accesser collection_aggregate, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_aggregate, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		mongoc_cursor_t* pcur =
			mongoc_collection_aggregate(
				pcoln, static_cast<mongoc_query_flags_t>(flags),
				ppipeline, popts, pread_prefs);

		if(!pcur)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser collection_aggregate, query failed");
			}

			mongoc_collection_destroy(pcoln);

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
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// update
	template<typename Connection_T>
	static bool s_org_collection_replace_one(Connection_T& conn,
												const char* str_db, const char* str_coln,
												const org_bson_type* pselector,
												const org_bson_type* preplacement,
												const org_bson_type* popts,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pselector && preplacement))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_replace_one, command invalid args");
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
					"mongo_accesser collection_replace_one, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_replace_one, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_replace_one(pcoln, pselector, preplacement, popts, preply, perr);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_collection_update_one(Connection_T& conn,
											const char* str_db, const char* str_coln,
											const org_bson_type* pselector,
											const org_bson_type* pupdate,
											const org_bson_type* popts,
											org_bson_type* preply,
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pselector && pupdate))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_update_one, command invalid args");
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
					"mongo_accesser collection_update_one, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_update_one, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_update_one(pcoln, pselector, pupdate, popts, preply, perr);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_collection_update_many(Connection_T& conn,
												const char* str_db, const char* str_coln,
												const org_bson_type* pselector,
												const org_bson_type* pupdate,
												const org_bson_type* popts,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pselector && pupdate))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_update_many, command invalid args");
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
					"mongo_accesser collection_update_many, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_update_many, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_update_many(pcoln, pselector, pupdate, popts, preply, perr);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_collection_update(Connection_T& conn,
										const char* str_db, const char* str_coln,
										u32 flags,
										const org_bson_type* pselector,
										const org_bson_type* pupdate,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		if(!pupdate)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_update, command invalid args");
			}

			return false;
		}

		switch(flags)
		{
		case MONGOC_UPDATE_NONE:
			{
				const bson_type* pudt = reinterpret_cast<const bson_type*>(pupdate);
				const char* pkey = pudt->begin().get_key_pointer();

				if(!pkey)
				{
					if(perr)
					{
						bson_set_error(
							perr,
							mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
							mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
							"mongo_accesser collection_update, command invalid args");
					}

					return false;
				}

				return
					(*pkey == '$')?
						this_type::s_org_collection_update_one(
							conn, str_db, str_coln, pselector, pupdate, popts, preply, perr)
						: this_type::s_org_collection_replace_one(
							conn, str_db, str_coln, pselector, pupdate, popts, preply, perr);
			}
		default:
			{
				if(flags & MONGOC_UPDATE_MULTI_UPDATE)
				{
					return s_org_collection_update_many(conn, str_db, str_coln, pselector, pupdate, popts, preply, perr);
				}
				else
				{
					if(perr)
					{
						bson_set_error(
							perr,
							mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
							mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
							"mongo_accesser collection_update, command invalid args");
					}

					assert(false);
					return false;
				}
			}
		}
	}

	// count
	template<typename Connection_T>
	static s64 s_org_collection_count(Connection_T& conn,
										const char* str_db, const char* str_coln,
										const org_bson_type* pfilter,
										const org_bson_type* popts,
										const org_read_prefs_type* pread_prefs,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pfilter))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_count, command invalid args");
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
					"mongo_accesser collection_count, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_count, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		s64 size =
			mongoc_collection_count_documents(
				pcoln,
				pfilter, popts, pread_prefs,
				preply, perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return size;
	}

	//estimated_count
	template<typename Connection_T>
	static s64 s_org_collection_estimated_count(Connection_T& conn,
												const char* str_db, const char* str_coln,
												const org_bson_type* popts,
												const org_read_prefs_type* pread_prefs,
												org_bson_type* preply,
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
					"mongo_accesser collection_count, command invalid args");
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
					"mongo_accesser collection_count, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_count, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		s64 size =
			mongoc_collection_estimated_document_count(
				pcoln,
				popts, pread_prefs,
				preply, perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return size;
	}

	// drop
	template<typename Connection_T>
	static bool s_org_collecion_drop(Connection_T& conn,
										const char* str_db, const char* str_coln,
										const org_bson_type* popts,
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
					"mongo_accesser collecion_drop, command invalid args");
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
					"mongo_accesser collecion_drop, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collecion_drop, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_drop_with_opts(pcoln, popts, perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// rename
	template<typename Connection_T>
	static bool s_org_collection_rename(Connection_T& conn,
											const char* str_db, const char* str_coln,
											const char* new_str_db, const char* new_str_coln,
											bool drop_taget_before_rename,
											const org_bson_type* popts,
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && new_str_db && new_str_coln))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_rename, command invalid args");
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
					"mongo_accesser collection_rename, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_rename, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess =
			mongoc_collection_rename_with_opts(
				pcoln, new_str_db, new_str_coln,
				drop_taget_before_rename, popts,
				perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// save
	template<typename Connection_T>
	static bool s_org_collection_save(Connection_T& conn,
										const char* str_db, const char* str_coln,
										const org_bson_type* pdoc,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		typedef bson_type::const_iterator bson_iter_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pdoc))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_save, command invalid args");
			}
			return false;
		}

		const bson_type* pbs_doc = reinterpret_cast<const bson_type*>(pdoc);
		assert(pbs_doc);

		bson_iter_type iter_id = pbs_doc->find(mongo_const_strings_def::_id());

		if(iter_id == pbs_doc->end())
		{
			return
				this_type::s_org_collection_insert_one(
					conn, str_db, str_coln, pdoc, popts, preply, perr);
		}
		else
		{
			bson_type new_opts;
			const bson_type* pbs_opts = reinterpret_cast<const bson_type*>(popts);
			if(pbs_opts && pbs_opts->is_finished() && !(pbs_opts->empty()))
			{
				detail::exclude_upsert_checker chk;
				pbs_opts->select(new_opts, chk);
			}

			new_opts.save(mongo_const_strings_def::upsert(), true);

			bson_type sel;
			sel.save(iter_id.get_key(), iter_id.get_value());

			return
				this_type::s_org_collection_replace_one(
					conn, str_db, str_coln,
					boost::addressof(sel), pdoc, boost::addressof(new_opts),
					preply, perr);
		}

	}

	// s_states
	template<typename Connection_T>
	static bool s_org_collection_stats(Connection_T& conn,
										const char* str_db, const char* str_coln,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && preply))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_stats, command invalid args");
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
					"mongo_accesser collection_stats, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_stats, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_native_ex::s_mongoc_collection_stats(pcoln, popts, preply, perr);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	//s_validate
	template<typename Connection_T>
	static bool s_org_collections_validate(Connection_T& conn,
											const char* str_db, const char* str_coln,
											const org_bson_type* popts,
											org_bson_type* preply,
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
					"mongo_accesser collection_validate, command invalid args");
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
					"mongo_accesser collection_validate, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_validate, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_native_ex::s_mongoc_collection_validate(pcoln, popts, preply, perr);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// index
	// create_index

	// args detail see
	// https://www.mongodb.com/docs/manual/reference/command/createIndexes/
	// https://mongoc.org/libmongoc/current/mongoc_collection_create_indexes_with_opts.html
	template<typename Connection_T>
	static bool s_org_collection_create_indexes(Connection_T& conn,
												const char* str_db, const char* str_coln,
												org_index_model_type* const* pmodels,
												size_t n_models,
												const org_bson_type* popts,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pmodels && n_models))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_create_index, command invalid args");
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
					"mongo_accesser collection_create_index, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_create_index, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess =
			mongoc_collection_create_indexes_with_opts(
				pcoln, pmodels, n_models, popts, preply, perr);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// drop_index
	template<typename Connection_T>
	static bool s_org_collection_drop_index(Connection_T& conn,
											const char* str_db, const char* str_coln,
											const char* str_idx_name, const org_bson_type* popts,
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && str_idx_name))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_drop_index, command invalid args");
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
					"mongo_accesser collection_drop_index, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_drop_index, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_drop_index_with_opts(pcoln, str_idx_name, popts, perr);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	//find_indexes
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_collection_find_indexes(Connection_T& conn,
												const char* str_db, const char* str_coln,
												const org_bson_type* opts,
												org_bson_error_type* perr, const HandlerCollector& handler_collector)
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
					"mongo_accesser collection_find_indexes, command invalid args");
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
					"mongo_accesser collection_find_indexes, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_find_indexes, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		mongoc_cursor_t* pcur = mongoc_collection_find_indexes_with_opts(pcoln, opts);

		if(!pcur)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser collection_find_indexes, query failed");
			}

			mongoc_collection_destroy(pcoln);

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

		bson_error_type tmp_err;
		const org_bson_type* porg_reply = 0;

		if(mongoc_cursor_error_document(pcur, boost::addressof(tmp_err), boost::addressof(porg_reply)))
		{
			if(perr)
			{
				const bson_type* preply = reinterpret_cast<const bson_type*>(porg_reply);
				bson_set_error(
					perr,
					tmp_err.domain(), tmp_err.code(),
					"Cursor Failure: %s\nReply: %s",
					tmp_err.what(), preply? preply->to_json<std::string>().c_str(): "");
			}


			mongoc_cursor_destroy(pcur);
			mongoc_collection_destroy(pcoln);
			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = !mongoc_cursor_error(pcur, perr);

		mongoc_cursor_destroy(pcur);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	//find_and_modify
	template<typename Connection_T>
	static bool s_org_collection_find_and_modify(Connection_T& conn,
													const char* str_db, const char* str_coln,
													const org_bson_type* pquery, const org_bson_type* psort,
													const org_bson_type* pupdate, const org_bson_type* pfields,
													bool bremove, bool bupsert, bool bnew,
													org_bson_type* preply, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pquery && pupdate))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_find_and_modify, command invalid args");
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
					"mongo_accesser collection_find_and_modify, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_find_and_modify, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess =
			mongoc_collection_find_and_modify(
				pcoln, pquery, psort, pupdate, pfields,
				bremove, bupsert, bnew, preply, perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// find_and_modify_with_opt
	template<typename Connection_T>
	static bool s_org_collection_find_and_modify_with_opt(Connection_T& conn,
															const char* str_db, const char* str_coln,
															const org_bson_type* pquery,
															const org_find_and_modify_opts_type* popts,
															org_bson_type* preply, org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && pquery))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_find_and_modify, command invalid args");
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
					"mongo_accesser collection_find_and_modify, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_find_and_modify, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		bool bsuccess = mongoc_collection_find_and_modify_with_opts(pcoln, pquery, popts, preply, perr);

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// bulk_execute
	template<typename Connection_T, typename HandlerBulkOptMaker>
	static u32 s_org_collection_bulk_execute(Connection_T& conn,
												const char* str_db,
												const char* str_coln,
												const org_bson_type* popts,
												const HandlerBulkOptMaker& handler_bulk_opt_maker, // commends maker
												org_bson_type* preply,
												org_bson_error_type* perr) // bulk execute params
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
					"mongo_accesser collection_bulk_execute, command invalid args");
			}
			return 0;
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
					"mongo_accesser collection_bulk_execute, mongoc_client is not ready");
			}

			return 0;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_bulk_execute, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return 0;
		}

		mongoc_bulk_operation_t* pbulk_opt
			= mongoc_collection_create_bulk_operation_with_opts(pcoln, popts);

		if(!pbulk_opt)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_bulk_execute, bulk_operation create failed");
			}

			mongoc_collection_destroy(pcoln);
			bool bpush = conn.push(ptr);
			assert(bpush);
			return 0;
		}

		u32 rst = 0;
		if(!handler_bulk_opt_maker(pbulk_opt))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_bulk_execute, bulk_operation make failed");
			}
		}
		else
		{
			rst = mongoc_bulk_operation_execute(pbulk_opt, preply, perr);
		}

		mongoc_bulk_operation_destroy(pbulk_opt);
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return rst;
	}

	// collection_watch
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_collection_watch(Connection_T& conn,
										const char* str_db,
										const char* str_coln,
										const org_bson_type* ppipeline,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr,
										const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && ppipeline))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_watch, command invalid args");
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
					"mongo_accesser collection_watch, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_watch, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		mongoc_change_stream_t* pchg_stream = mongoc_collection_watch(pcoln, ppipeline, popts);

		if(!pchg_stream)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser collection_watch, query failed");
			}

			mongoc_collection_destroy(pcoln);

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
		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// using_handler
	template<typename Connection_T, typename Handler>
	static
	typename
		boost::enable_if
		<
			boost::is_same<typename func::foo_t_info<Handler>::result_type, void>,
			void
		>::type
		s_org_collection_using_handler(Connection_T& conn,
										const char* str_db, const char* str_coln,
										org_bson_error_type* perr, const Handler& handler)
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
					"mongo_accesser collection_using_handler, command invalid args");
			}
			return handler(static_cast<mongoc_collection_t*>(0), perr);
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
					"mongo_accesser collection_using_handler, mongoc_client is not ready");
			}

			return handler(static_cast<mongoc_collection_t*>(0), perr);
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_using_handler, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return handler(static_cast<mongoc_collection_t*>(0), perr);
		}

		handler(pcoln, perr);

		mongoc_collection_destroy(pcoln);

		//bool bpush = conn.push(ptr);
		conn.push(ptr);
	}

	template<typename Connection_T, typename Handler>
	static
	typename
		boost::disable_if
		<
			boost::is_same<typename func::foo_t_info<Handler>::result_type, void>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_org_collection_using_handler(Connection_T& conn,
										const char* str_db, const char* str_coln,
										org_bson_error_type* perr, const Handler& handler)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type result_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser collection_using_handler, command invalid args");
			}
			return handler(static_cast<mongoc_collection_t*>(0), perr);
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
					"mongo_accesser collection_using_handler, mongoc_client is not ready");
			}

			return handler(static_cast<mongoc_collection_t*>(0), perr);
		}

		mongoc_collection_t* pcoln = mongoc_client_get_collection(ptr.get(), str_db, str_coln);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser collection_using_handler, not found %s.%s", str_db, str_coln);
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return handler(static_cast<mongoc_collection_t*>(0), perr);
		}

		result_type rst(handler(pcoln, perr));

		mongoc_collection_destroy(pcoln);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return rst;
	}

	//------------------------collection e------------------------

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_COLLECTION_HPP__
