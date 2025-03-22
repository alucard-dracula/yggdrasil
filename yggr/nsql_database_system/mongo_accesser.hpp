//mongo_accesser.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_HPP__

#include <yggr/nsql_database_system/mongo_accesser_client.hpp>
#include <yggr/nsql_database_system/mongo_accesser_client_encryption.hpp>
#include <yggr/nsql_database_system/mongo_accesser_database.hpp>
#include <yggr/nsql_database_system/mongo_accesser_collection.hpp>
#include <yggr/nsql_database_system/mongo_accesser_gridfs.hpp>
#include <yggr/nsql_database_system/mongo_accesser_thread.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongo_accesser
	: public mongo_accesser_client,
		public mongo_accesser_client_encryption,
		public mongo_accesser_database,
		public mongo_accesser_collection,
		public mongo_accesser_gridfs,
		public mongo_accesser_thread
{

public:
	typedef mongo_accesser_client base_accesser_client_type;
	typedef mongo_accesser_client_encryption base_accesser_encryption_type;
	typedef mongo_accesser_database base_accesser_database_type;
	typedef mongo_accesser_collection base_accesser_collection_type;
	typedef mongo_accesser_gridfs base_accesser_gridfs_type;
	typedef mongo_accesser_thread base_accesser_thread_type;

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

	// client special
public:
	typedef base_accesser_client_type::uri_type uri_type;
	typedef base_accesser_client_type::org_uri_type org_uri_type;

	typedef base_accesser_client_type::ssl_opt_type ssl_opt_type;
	typedef base_accesser_client_type::org_ssl_opt_type org_ssl_opt_type;

	typedef base_accesser_client_type::server_description_type server_description_type;
	typedef base_accesser_client_type::org_server_description_type org_server_description_type;

	typedef base_accesser_client_type::stream_initiator_type stream_initiator_type;
	typedef base_accesser_client_type::org_stream_initiator_type org_stream_initiator_type;

	typedef base_accesser_client_type::apm_callbacks_type apm_callbacks_type;
	typedef base_accesser_client_type::org_apm_callbacks_type org_apm_callbacks_type;

	typedef base_accesser_client_type::server_api_type server_api_type;
	typedef base_accesser_client_type::org_server_api_type org_server_api_type;

	typedef base_accesser_client_type::auto_encryption_opts_type auto_encryption_opts_type;
	typedef base_accesser_client_type::org_auto_encryption_opts_type org_auto_encryption_opts_type;

	// encryption special
public:
	typedef base_accesser_encryption_type::client_encryption_opts_type client_encryption_opts_type;
	typedef base_accesser_encryption_type::org_client_encryption_opts_type org_client_encryption_opts_type;

	typedef base_accesser_encryption_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;
	typedef base_accesser_encryption_type::org_client_encryption_datakey_opts_type org_client_encryption_datakey_opts_type;

	typedef base_accesser_encryption_type::client_encryption_rewrap_many_datakey_result_type client_encryption_rewrap_many_datakey_result_type;
	typedef base_accesser_encryption_type::org_client_encryption_rewrap_many_datakey_result_type org_client_encryption_rewrap_many_datakey_result_type;

	typedef base_accesser_encryption_type::client_encryption_encrypt_opts_type client_encryption_encrypt_opts_type;
	typedef base_accesser_encryption_type::org_client_encryption_encrypt_opts_type org_client_encryption_encrypt_opts_type;

	// gridfs special
public:
	typedef base_accesser_gridfs_type::gridfs_file_opt_type gridfs_file_opt_type;
	typedef base_accesser_gridfs_type::org_gridfs_file_opt_type org_gridfs_file_opt_type;

	typedef base_accesser_gridfs_type::gridfs_file_type gridfs_file_type;
	typedef base_accesser_gridfs_type::org_gridfs_file_type org_gridfs_file_type;

private:
	typedef mongo_accesser this_type;

public:
	// combo s_watch
	using base_accesser_client_type::s_watch;
	using base_accesser_database_type::s_watch;
	using base_accesser_collection_type::s_watch;

	// combo s_using_handler
	using base_accesser_database_type::s_using_handler;
	using base_accesser_collection_type::s_using_handler;

};

} // namespace nsql_database_system
} // namespace yggr


#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_HPP__

