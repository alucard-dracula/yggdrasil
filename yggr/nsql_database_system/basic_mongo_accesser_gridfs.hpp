//basic_mongo_accesser_gridfs.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_GRIDFS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_GRIDFS_HPP__

#include <yggr/nsql_database_system/mongo_accesser_types_decl.hpp>

#include <yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp>
#include <yggr/nsql_database_system/c_mongo_gridfs_file.hpp>

#include <yggr/nsql_database_system/mongoc_gridfs_file_native_ex.hpp>
#include <yggr/nsql_database_system/mongoc_gridfs_bucket_native_ex.hpp>
#include <yggr/nsql_database_system/mongoc_gridfs_bucket_t_native_ex.hpp>
#include <yggr/nsql_database_system/mongoc_gridfs_bucket_file_t_native_ex.hpp>

namespace yggr
{
namespace nsql_database_system
{

class basic_mongo_accesser_gridfs
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
	typedef c_mongo_gridfs_file_opt gridfs_file_opt_type;
	typedef gridfs_file_opt_type::org_type org_gridfs_file_opt_type;

	typedef c_mongo_gridfs_file gridfs_file_type;
	typedef gridfs_file_type::org_type org_gridfs_file_type;

private:
	typedef basic_mongo_accesser_gridfs this_type;

	//------------------------gridfs s----------------------------

public:
	// gridfs save_file
	template<typename Connection_T>
	static bool s_org_gridfs_save_file(Connection_T& conn, const char* str_db, const char* str_gridfs,
										const char* str_fpath, s32 flag, s32 mode,
										org_gridfs_file_opt_type* popt,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && str_fpath))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_save_file, command invalid args");
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
					"mongo_accesser gridfs_save_file, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		mongoc_stream_t* pstream = mongoc_stream_file_new_for_path(str_fpath, flag? flag : O_RDONLY, mode);
		if(!pstream)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_save_file, create file stream failed");
			}
			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return false;
		}


		org_gridfs_file_type* pfile = mongoc_gridfs_create_file_from_stream(pgridfs, pstream, popt);


		if(!pfile)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_GRIDFS_INVALID_FILENAME,
					"mongo_accesser gridfs_save_file, create gridfs file failed");
			}
			mongoc_stream_destroy(pstream);
			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return false;
		}

		bool bsuccess = mongoc_gridfs_file_save(pfile); // 1 stream, 2 stram to file 3 save

		mongoc_gridfs_file_destroy(pfile);
		// if mongoc_gridfs_create_file_from_stream success stream is freed
		//mongoc_stream_destroy(stream); // don't destroy stream
		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// save_of_buffer
	template<typename Connection_T>
	static bool s_org_gridfs_save_file_of_buffer(Connection_T& conn, const char* str_db, const char* str_gridfs,
													u32 tmout_msec, const u8* buffer, u64 len,
													org_gridfs_file_opt_type* popt,
													org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && buffer && len))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_save_file, command invalid args");
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
					"mongo_accesser gridfs_save_file, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		org_gridfs_file_type* pfile = mongoc_gridfs_create_file(pgridfs, popt);

		if(!pfile)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_GRIDFS_INVALID_FILENAME,
					"mongo_accesser gridfs_save_file, create gridfs file failed");
			}
			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return false;
		}

		mongoc_iovec_t iovec = {0};
		iovec.iov_base = reinterpret_cast<char*>(const_cast<u8*>(buffer));
		iovec.iov_len = len;

		bool bsuccess
			= (len == mongoc_gridfs_file_writev(pfile, &iovec, 1, tmout_msec))
				&& mongoc_gridfs_file_save(pfile);

		mongoc_gridfs_file_destroy(pfile);
		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// gridfs_load_file
	template<typename Connection_T, typename Buffer>
	static u64 s_org_gridfs_load_file(Connection_T& conn, const char* str_db, const char* str_gridfs,
										const char* str_fname, s32 tmout_msec, Buffer& out_buf,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		typedef Buffer buffer_type;
		typedef typename boost::range_value<buffer_type>::type buf_val_type;
		typedef typename boost::range_iterator<buffer_type>::type buf_iter_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<buf_iter_type>));

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && str_fname))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_load_file, command invalid args");
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
					"mongo_accesser gridfs_load_file, mongoc_client is not ready");
			}

			return 0;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return 0;
		}

		org_gridfs_file_type* pfile = mongoc_gridfs_find_one_by_filename(pgridfs, str_fname, perr);

		if(!pfile)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_GRIDFS_INVALID_FILENAME,
					"mongo_accesser gridfs_load_file, file not found");
			}

			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return 0;
		}

		mongoc_stream_t* pstream = mongoc_stream_gridfs_new(pfile); // stream

		if(!pstream)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_load_file, create file stream failed");
			}
			mongoc_gridfs_file_destroy(pfile);
			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return 0;
		}

		s64 buf_byte_size = mongoc_gridfs_file_get_length(pfile);

		if(buf_byte_size <= 0)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_load_file, invalidate file");
			}
			mongoc_gridfs_file_destroy(pfile);
			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return 0;
		}

		s64 buf_size = (buf_byte_size + sizeof(buf_val_type) -1) / sizeof(buf_val_type);

		yggr::resize(out_buf, buf_size);
		memset(&(*boost::begin(out_buf)), 0, buf_size * sizeof(buf_val_type));

		mongoc_iovec_t iov = {0};
		iov.iov_base = reinterpret_cast<char*>(&(*boost::begin(out_buf)));
		iov.iov_len = buf_byte_size;

		s64 read_byte_size = mongoc_stream_readv(pstream, &iov, 1, -1, tmout_msec);

		if(buf_byte_size != read_byte_size)
		{
			read_byte_size = 0;
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_load_file, file read failed");
			}
		}

		mongoc_stream_destroy(pstream);
		mongoc_gridfs_file_destroy(pfile);
		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);
		return static_cast<u64>(read_byte_size);
	}

	// gridfs_find
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_gridfs_find(Connection_T& conn,
									const char* str_db, const char* str_gridfs,
									const org_bson_type* pquery,
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && pquery))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_find, command invalid args");
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
					"mongo_accesser gridfs_find, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		mongoc_gridfs_file_list_t* plist = mongoc_gridfs_find(pgridfs, pquery);

		if(!plist)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_find, query failure");
			}

			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return false;
		}

		for(org_gridfs_file_type* pfile = 0; pfile = mongoc_gridfs_file_list_next(plist);)
		{
			gridfs_file_type tmp(pfile);
			if(!handler_collector(tmp))
			{
				break;
			}
		}

		mongoc_gridfs_file_list_destroy(plist);
		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);
		return true;
	}

	// gridfs_find_one
	template<typename Connection_T, typename Handler>
	static bool s_org_gridfs_find_one(Connection_T& conn,
										const char* str_db, const char* str_gridfs,
										const org_bson_type* pquery,
										org_bson_error_type* perr,
										const Handler& handler)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && pquery))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_find_one, command invalid args");
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
					"mongo_accesser gridfs_find_one, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		org_gridfs_file_type* pfile = mongoc_gridfs_find_one(pgridfs, pquery, perr);

		if(!pfile)
		{
			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return false;
		}

		{
			gridfs_file_type tmp(pfile);
			handler(tmp);
		}

		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);
		return true;
	}

	// gridfs_find_one_by_filename
	template<typename Connection_T, typename Handler>
	static bool s_org_gridfs_find_one_by_filename(Connection_T& conn,
													const char* str_db, const char* str_gridfs,
													const char* str_fname,
													org_bson_error_type* perr,
													const Handler& handler)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && str_fname))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_find_one, command invalid args");
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
					"mongo_accesser gridfs_find_one, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		org_gridfs_file_type* pfile = mongoc_gridfs_find_one_by_filename(pgridfs, str_fname, perr);

		if(!pfile)
		{
			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return false;
		}

		{
			gridfs_file_type tmp(pfile);
			handler(tmp);
		}

		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);
		return true;
	}

	// gridfs_chunks_find
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_gridfs_chunks_find(Connection_T& conn,
											const char* str_db, const char* str_gridfs,
											u32 flags, u32 skip, u32 limit, u32 batch_size,
											const org_bson_type* pquery, const org_bson_type* pfields,
											const org_read_prefs_type* pread_prefs,
											org_bson_error_type* perr,
											const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && pquery))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_chunks_find, command invalid args");
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
					"mongo_accesser gridfs_chunks_find, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		mongoc_collection_t* pcoln = mongoc_gridfs_get_chunks(pgridfs);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser gridfs_chunks_find, collections not exist");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		mongoc_cursor_t* pcur = mongoc_collection_find(pcoln, static_cast<mongoc_query_flags_t>(flags),
														skip, limit, batch_size, pquery, pfields,
														pread_prefs);

		if(!pcur)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_chunks_find, query failed");
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

		//bool bsuccess = !mongoc_cursor_error(pcur, perr);
		mongoc_cursor_error(pcur, perr);

		mongoc_cursor_destroy(pcur);
		//mongoc_collection_destroy(pcoln); // don't destroy pcoln, it's free at mongoc_gridfs_destroy

		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);
		return true;
	}

	// gridfs_files_find
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_gridfs_files_find(Connection_T& conn,
											const char* str_db, const char* str_gridfs,
											u32 flags, u32 skip, u32 limit, u32 batch_size,
											const org_bson_type* pquery, const org_bson_type* pfields,
											const org_read_prefs_type* pread_prefs,
											org_bson_error_type* perr,
											const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && pquery))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_chunks_find, command invalid args");
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
					"mongo_accesser gridfs_chunks_find, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		mongoc_collection_t* pcoln = mongoc_gridfs_get_files(pgridfs);

		if(!pcoln)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST,
					"mongo_accesser gridfs_chunks_find, collections not exist");
			}

			bool bpush = conn.push(ptr);
			assert(bpush);
			return false;
		}

		mongoc_cursor_t* pcur = mongoc_collection_find(pcoln, static_cast<mongoc_query_flags_t>(flags),
														skip, limit, batch_size, pquery, pfields,
														pread_prefs);

		if(!pcur)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_chunks_find, query failed");
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
		//mongoc_collection_destroy(pcoln); // don't destroy pcoln, it's free at mongoc_gridfs_destroy

		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);

		//return true;
		return bsuccess;
	}

	// gridfs_remove_file
	template<typename Connection_T>
	static bool s_org_gridfs_remove_file(Connection_T& conn,
											const char* str_db, const char* str_gridfs,
											const char* str_fname,
											org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && str_fname))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_find_one, command invalid args");
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
					"mongo_accesser gridfs_find_one, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		bool bsuccess = mongoc_gridfs_remove_by_filename(pgridfs, str_fname, perr);

		mongoc_gridfs_destroy(pgridfs);

		bool bpush = conn.push(ptr);
		assert(bpush);
		return bsuccess;
	}

	// gridfs_files_begin
	// like s_org_gridfs_find, but copy gridfs files is enable,
	// need use gridfs_end revoke connection and gridfs files
	// !!!!!!!! a gridfs container's elements must use a same connection and don't splice it !!!!!!

	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_gridfs_files_begin(Connection_T& conn,
											const char* str_db, const char* str_gridfs,
											const org_bson_type* pquery,
											org_bson_error_type* perr,
											const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		typedef typename connection_type::mongoc_client_deleter_type mongoc_client_deleter_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_gridfs && pquery))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_files_begin, command invalid args");
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
					"mongo_accesser gridfs_files_begin, mongoc_client is not ready");
			}

			return false;
		}

		mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(ptr.get(), str_db, str_gridfs, perr);
		if(!pgridfs)
		{
			conn.push(ptr);
			return false;
		}

		mongoc_gridfs_file_list_t* plist = mongoc_gridfs_find(pgridfs, pquery);

		if(!plist)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_files_begin, query failure");
			}

			mongoc_gridfs_destroy(pgridfs);
			conn.push(ptr);
			return false;
		}

		for(org_gridfs_file_type* pfile = 0; pfile = mongoc_gridfs_file_list_next(plist);)
		{
			gridfs_file_type tmp(pfile);
			if(!handler_collector(tmp))
			{
				break;
			}
		}

		mongoc_gridfs_file_list_destroy(plist);

		// gridfs_files need connection, so not revoke it

		{
			mongoc_client_deleter_type* pdel = boost::get_deleter<mongoc_client_deleter_type>(ptr);
			assert(pdel);
			if(!pdel)
			{
				if(perr)
				{
					bson_set_error(
						perr,
						mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
						mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
						"mongo_accesser gridfs_files_begin, mongoc_client deleter not supported nonfree");
				}
			}

			pdel->nonfree();
		}

		return true;
	}

	// gridfs_files_end
	// !!!!!!!! a gridfs container's elements must use a same connection and don't splice it !!!!!!
	template<typename Connection_T, typename Files_Container>
	static bool s_org_gridfs_files_end(Connection_T& conn,
										Files_Container& files_cont,
										org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::base_mongoc_client_ptr_type client_ptr_type;

		typedef Files_Container files_cont_type;
		typedef typename boost::range_iterator<files_cont_type>::type files_cont_iter_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		// don't check is_poped, because revocable is checked it

		bool brevoke = true;
		client_ptr_type pclient = 0;
		client_ptr_type pchk_client = 0;
		const org_gridfs_file_type* pfile = 0;
		mongoc_gridfs_t* pgridfs = 0;
		{
			files_cont_type tmp;
			tmp.swap(files_cont);

			for(files_cont_iter_type i = boost::begin(tmp), isize = boost::end(tmp); brevoke && (i != isize); ++i)
			{
				pfile = *i;
				pgridfs = pgridfs? pgridfs : mongoc_gridfs_file_native_ex::s_get_mongoc_gridfs(pfile);
				pclient = mongoc_gridfs_file_native_ex::s_get_mongoc_client(pfile);
				pchk_client = pchk_client? pchk_client : conn.revocable(pclient);
				brevoke = (brevoke) && (pchk_client) && (pclient == pchk_client)
							&& (mongoc_gridfs_file_native_ex::s_get_mongoc_gridfs(pfile) == pgridfs);
			}

			if(!brevoke)
			{
				files_cont.swap(tmp);
			}
		}

		if(brevoke)
		{
			if(pgridfs)
			{
				mongoc_gridfs_destroy(pgridfs);
			}
			conn.revoke(pchk_client);
		}
		else
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_files_end, revoke failure");
			}
		}

		return brevoke;
	}

	template<typename Connection_T, typename Files_Container, typename Gridfs_File_Getter>
	static bool s_org_gridfs_files_end(Connection_T& conn,
										Files_Container& files_cont,
										org_bson_error_type* perr,
										const Gridfs_File_Getter& file_getter)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::base_mongoc_client_cptr_type client_ptr_type;

		typedef Files_Container files_cont_type;
		typedef typename boost::range_iterator<files_cont_type>::type files_cont_iter_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!conn.is_poped())
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_files_end, connection is poped");
			}
			return false;
		}

		bool brevoke = true;
		client_ptr_type pclient = 0;
		org_gridfs_file_type* pfile = 0;
		mongoc_gridfs_t* pgridfs = 0;
		{
			files_cont_type tmp;
			tmp.swap(files_cont);

			for(files_cont_iter_type i = boost::begin(tmp), isize = boost::end(tmp); brevoke && (i != isize); ++i)
			{
				pfile = file_getter(*i);
				pgridfs = pgridfs? pgridfs : mongoc_gridfs_file_native_ex::s_get_mongoc_gridfs(pfile);
				pclient = mongoc_gridfs_file_native_ex::s_get_mongoc_client(pfile);
				brevoke = brevoke && conn.revocable(pclient)
							&& mongoc_gridfs_file_native_ex::s_get_mongoc_gridfs(pfile) == pgridfs;
			}

			if(!brevoke)
			{
				files_cont.swap(tmp);
			}
		}

		if(brevoke)
		{
			if(pgridfs)
			{
				mongoc_gridfs_destroy(pgridfs);
			}
			conn.revoke();
		}
		else
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_files_end, revoke failure");
			}
		}

		return brevoke;
	}

	//------------------------gridfs e----------------------------

	//------------------------gridfs_bucket s----------------------------

	// gridfs_bucket save_file
	template<typename Connection_T>
	static bool s_org_gridfs_bucket_save_file(Connection_T& conn, const char* str_db, const char* str_fname,
												const char* str_fpath, s32 flag, s32 mode,
												const org_bson_type* pbucket_opts,
												const org_read_prefs_type* pbucket_read_prefs,
												const org_bson_type* pstream_opts,
												org_bson_value_type *pfile_id,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_fname && str_fpath))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_bucket_save_file, command invalid args");
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
					"mongo_accesser gridfs_bucket_save_file, mongoc_client is not ready");
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
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_save_file, mongoc_database is not exists");
			}

			conn.push(ptr);
			return false;
		}

		mongoc_gridfs_bucket_t* pbucket = mongoc_gridfs_bucket_new(pdb, pbucket_opts, pbucket_read_prefs, perr);

		if(!pbucket)
		{
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		mongoc_stream_t* pfile_stream = mongoc_stream_file_new_for_path(str_fpath, flag? flag : O_RDONLY, mode);
		if(!pfile_stream)
		{
			if(perr)
			{
				bson_set_error(
					perr, mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_bucket_save_file, create file stream failed");
			}
			mongoc_gridfs_bucket_destroy(pbucket);
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		bool bsuccess =
			mongoc_gridfs_bucket_upload_from_stream(
				pbucket, str_fname, pfile_stream,
				pstream_opts, pfile_id, perr);

		mongoc_stream_close(pfile_stream);
		mongoc_stream_destroy(pfile_stream);

		mongoc_gridfs_bucket_destroy(pbucket);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// gridfs_bucket save_file
	template<typename Connection_T>
	static bool s_org_gridfs_bucket_save_file_with_id(Connection_T& conn, const char* str_db, const char* str_fname,
														const char* str_fpath, s32 flag, s32 mode,
														const org_bson_type* pbucket_opts,
														const org_read_prefs_type* pbucket_read_prefs,
														const org_bson_type* pstream_opts,
														const org_bson_value_type *pfile_id,
														org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_fname && str_fpath))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_bucket_save_file, command invalid args");
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
					"mongo_accesser gridfs_bucket_save_file, mongoc_client is not ready");
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
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_save_file, mongoc_database is not exists");
			}

			conn.push(ptr);
			return false;
		}

		mongoc_gridfs_bucket_t* pbucket = mongoc_gridfs_bucket_new(pdb, pbucket_opts, pbucket_read_prefs, perr);

		if(!pbucket)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_save_file, create gridfs bucket failed");
			}

			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		mongoc_stream_t* pfile_stream = mongoc_stream_file_new_for_path(str_fpath, flag? flag : O_RDONLY, mode);
		if(!pfile_stream)
		{
			if(perr)
			{
				bson_set_error(
					perr, mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_bucket_save_file, create file stream failed");
			}
			mongoc_gridfs_bucket_destroy(pbucket);
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		bool bsuccess =
			mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_upload_from_stream_with_id(
				pbucket, pfile_id,
				str_fname, pfile_stream,
				pstream_opts, perr);

		mongoc_stream_close(pfile_stream);
		mongoc_stream_destroy(pfile_stream);

		mongoc_gridfs_bucket_destroy(pbucket);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// bucket_save_file_of_buffer
	template<typename Connection_T>
	static bool s_org_gridfs_bucket_save_file_of_buffer(Connection_T& conn, const char* str_db, const char* str_fname,
														u32 tmout_msec, const u8* buffer, u64 len,
														const org_bson_type* pbucket_opts,
														const org_read_prefs_type* pbucket_read_prefs,
														const org_bson_type* pstream_opts,
														org_bson_value_type *pfile_id,
														org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_fname && buffer && len))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_bucket_save_file, command invalid args");
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
					"mongo_accesser gridfs_bucket_save_file, mongoc_client is not ready");
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
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_save_file, mongoc_database is not exists");
			}

			conn.push(ptr);
			return false;
		}

		mongoc_gridfs_bucket_t* pbucket = mongoc_gridfs_bucket_new(pdb, pbucket_opts, pbucket_read_prefs, perr);

		if(!pbucket)
		{
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		mongoc_stream_t* pfile_stream =
			mongoc_gridfs_bucket_open_upload_stream(
				pbucket, str_fname, pstream_opts, pfile_id, perr);

		if(!pfile_stream)
		{
			if(perr)
			{
				bson_set_error(
					perr, mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_bucket_save_file, create file stream failed");
			}
			mongoc_gridfs_bucket_destroy(pbucket);
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		mongoc_iovec_t iovec = {0};
		iovec.iov_base = reinterpret_cast<char*>(const_cast<u8*>(buffer));
		iovec.iov_len = len;

		bool bsuccess = (len == mongoc_stream_writev(pfile_stream, &iovec, 1, tmout_msec));

		mongoc_stream_close(pfile_stream);
		mongoc_stream_destroy(pfile_stream);

		mongoc_gridfs_bucket_destroy(pbucket);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	template<typename Connection_T>
	static bool s_org_gridfs_bucket_save_file_of_buffer_with_id(Connection_T& conn, const char* str_db, const char* str_fname,
																u32 tmout_msec, const u8* buffer, u64 len,
																const org_bson_type* pbucket_opts,
																const org_read_prefs_type* pbucket_read_prefs,
																const org_bson_type* pstream_opts,
																const org_bson_value_type *pfile_id,
																org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_fname && buffer && len))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_bucket_save_file, command invalid args");
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
					"mongo_accesser gridfs_bucket_save_file, mongoc_client is not ready");
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
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_save_file, mongoc_database is not exists");
			}

			conn.push(ptr);
			return false;
		}

		mongoc_gridfs_bucket_t* pbucket = mongoc_gridfs_bucket_new(pdb, pbucket_opts, pbucket_read_prefs, perr);

		if(!pbucket)
		{
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		mongoc_stream_t* pfile_stream =
			mongoc_gridfs_bucket_open_upload_stream_with_id(
				pbucket, pfile_id, str_fname, pstream_opts, perr);

		if(!pfile_stream)
		{
			if(perr)
			{
				bson_set_error(
					perr, mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_bucket_save_file, create file stream failed");
			}
			mongoc_gridfs_bucket_destroy(pbucket);
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		mongoc_iovec_t iovec = {0};
		iovec.iov_base = reinterpret_cast<char*>(const_cast<u8*>(buffer));
		iovec.iov_len = len;

		bool bsuccess = (len == mongoc_stream_writev(pfile_stream, &iovec, 1, tmout_msec));

		if(0 != mongoc_stream_close(pfile_stream))
		{
			bsuccess = !mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_upload_get_error(pfile_stream, perr);
		}

		mongoc_stream_destroy(pfile_stream);

		mongoc_gridfs_bucket_destroy(pbucket);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// gridfs_bucket_load_file
	template<typename Connection_T, typename Buffer>
	static u64 s_org_gridfs_bucket_load_file(Connection_T& conn,
												const char* str_db,
												const org_bson_type* pbucket_opts,
												const org_read_prefs_type* pbucket_read_prefs,
												const org_bson_value_type *pfile_id,
												s32 tmout_msec,
												Buffer& out_buf,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		typedef Buffer buffer_type;
		typedef typename boost::range_value<buffer_type>::type buf_val_type;
		typedef typename boost::range_iterator<buffer_type>::type buf_iter_type;

		BOOST_MPL_ASSERT((is_random_access_iterator<buf_iter_type>));

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pfile_id))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_bucket_load_file, command invalid args");
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
					"mongo_accesser gridfs_bucket_load_file, mongoc_client is not ready");
			}

			return 0;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);
		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_load_file, mongoc_database is not exists");
			}

			conn.push(ptr);
			return 0;
		}

		mongoc_gridfs_bucket_t* pbucket = mongoc_gridfs_bucket_new(pdb, pbucket_opts, pbucket_read_prefs, perr);

		if(!pbucket)
		{
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return 0;
		}

		mongoc_stream_t* pfile_stream =
			mongoc_gridfs_bucket_open_download_stream(
				pbucket, pfile_id, perr);

		if(!pfile_stream)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_bucket_load_file, create file stream failed");
			}
			mongoc_gridfs_bucket_destroy(pbucket);
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return 0;
		}

		void* pfile = mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_download_get_file(pfile_stream);

		if(!pfile)
		{
			if(perr)
			{
				bson_set_error(
					perr, mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_bucket_load_file, create file stream failed");
			}
			mongoc_stream_destroy(pfile_stream);
			mongoc_gridfs_bucket_destroy(pbucket);
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return 0;
		}

		s64 buf_byte_size = mongoc_gridfs_bucket_file_t_native_ex::var_length(pfile);

		if(buf_byte_size <= 0)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_bucket_load_file, invalidate file");
			}
			mongoc_stream_destroy(pfile_stream);
			mongoc_gridfs_bucket_destroy(pbucket);
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return 0;
		}

		s64 buf_size = (buf_byte_size + sizeof(buf_val_type) -1) / sizeof(buf_val_type);

		yggr::resize(out_buf, buf_size);
		memset(&(*boost::begin(out_buf)), 0, buf_size * sizeof(buf_val_type));
		
		mongoc_iovec_t iov = {0};
		iov.iov_base = reinterpret_cast<char*>(&(*boost::begin(out_buf)));
		iov.iov_len = buf_byte_size;

		s64 read_byte_size = mongoc_stream_readv(pfile_stream, &iov, 1, -1, tmout_msec);

		if(buf_byte_size != read_byte_size)
		{
			read_byte_size = 0;
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_GRIDFS,
					mongoc_error_code_type::E_MONGOC_ERROR_STREAM_INVALID_TYPE,
					"mongo_accesser gridfs_load_file, file read failed");
			}
		}

		mongoc_stream_destroy(pfile_stream);
		mongoc_gridfs_bucket_destroy(pbucket);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);
		return static_cast<u64>(read_byte_size);
	}

	// gridfs_bucket_find
	template<typename Connection_T, typename HandlerCollector>
	static bool s_org_gridfs_bucket_find(Connection_T& conn,
											const char* str_db,
											const org_bson_type* pbucket_opts,
											const org_read_prefs_type* pbucket_read_prefs,
											const org_bson_type* pfilter,
											const org_bson_type* popts,
											org_bson_error_type* perr,
											const HandlerCollector& handler_collector)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pfilter))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_bucket_find, command invalid args");
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
					"mongo_accesser gridfs_bucket_find, mongoc_client is not ready");
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
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_find, mongoc_database is not exists");
			}

			conn.push(ptr);
			return false;
		}

		mongoc_gridfs_bucket_t* pbucket = mongoc_gridfs_bucket_new(pdb, pbucket_opts, pbucket_read_prefs, perr);

		if(!pbucket)
		{
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return false;
		}

		mongoc_cursor_t* pcur = mongoc_gridfs_bucket_find(pbucket, pfilter, popts);

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
		mongoc_gridfs_bucket_destroy(pbucket);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return bsuccess;
	}

	// gridfs_bucket_remove_file
	template<typename Connection_T>
	static s64 s_org_gridfs_bucket_remove_file_by_id(Connection_T& conn,
														const char* str_db,
														const org_bson_type* pbucket_opts,
														const org_read_prefs_type* pbucket_read_prefs,
														const org_bson_value_type* pfile_id,
														org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pfile_id))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_bucket_remove_file_by_id, command invalid args");
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
					"mongo_accesser gridfs_bucket_remove_file_by_id, mongoc_client is not ready");
			}

			return 0;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);
		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_remove_file_by_id, mongoc_database is not exists");
			}

			conn.push(ptr);
			return 0;
		}

		mongoc_gridfs_bucket_t* pbucket = mongoc_gridfs_bucket_new(pdb, pbucket_opts, pbucket_read_prefs, perr);

		if(!pbucket)
		{
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return 0;
		}

		bool bsuccess = mongoc_gridfs_bucket_delete_by_id(pbucket, pfile_id, perr);

		mongoc_gridfs_bucket_destroy(pbucket);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);
		return bsuccess? 1 : 0;
	}

	// gridfs_bucket_remove_filess
	template<typename Connection_T>
	static s64 s_org_gridfs_bucket_remove_files(Connection_T& conn,
												const char* str_db,
												const org_bson_type* pbucket_opts,
												const org_read_prefs_type* pbucket_read_prefs,
												const org_bson_type* pfilter,
												const org_bson_type* popts,
												org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		typedef c_bson_oid bson_oid_type;

		typedef bson_type::const_iterator bson_citer_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && pfilter))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser gridfs_bucket_remove_files, command invalid args");
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
					"mongo_accesser gridfs_bucket_remove_files, mongoc_client is not ready");
			}

			return 0;
		}

		mongoc_database_t* pdb = mongoc_client_get_database(ptr.get(), str_db);
		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_remove_files, mongoc_database is not exists");
			}

			conn.push(ptr);
			return 0;
		}

		mongoc_gridfs_bucket_t* pbucket = mongoc_gridfs_bucket_new(pdb, pbucket_opts, pbucket_read_prefs, perr);

		if(!pbucket)
		{
			mongoc_database_destroy(pdb);
			conn.push(ptr);
			return 0;
		}

		mongoc_cursor_t* pcur = mongoc_gridfs_bucket_find(pbucket, pfilter, popts);

		const org_bson_type* porg_doc = 0;
		const bson_type* pdoc = 0;
		bson_oid_type file_id(bson_oid_type::s_gen_empty_oid());
		bson_citer_type bs_iter;

		bson_type query_data;
		s64 rm_count = 0;

		yggr::nsql_database_system::bson_mark_splice sp;
		query_data.save_array_start(mongo_operators_dollar_cmd_def::dollar_in(), sp);
		for(;mongoc_cursor_next(pcur, boost::addressof(porg_doc));)
		{
			pdoc = reinterpret_cast<const bson_type*>(porg_doc);

			if(pdoc)
			{
				bs_iter = pdoc->find(mongo_const_strings_def::_id());
				if(bs_iter != pdoc->end() && bs_iter.typeid_of_value() == bson_typeid_def::E_BSON_TYPE_OID)
				{
					std::stringstream ss;
					ss << rm_count;
					query_data.save(
						charset::make_string_charset_helper(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
						bs_iter.get_value(file_id));
					++rm_count;
				}
			}
		}

		query_data.save_array_end(sp);

		if(rm_count)
		{
			bson_type files_query;
			bson_type chunks_query;
			files_query.save(mongo_const_strings_def::_id(), query_data);
			chunks_query.save(mongo_const_strings_def::files_id(), query_data);

			mongoc_collection_t* pcol_files = mongoc_gridfs_bucket_t_native_ex::var_files(pbucket);
			mongoc_collection_t* pcol_chunks = mongoc_gridfs_bucket_t_native_ex::var_chunks(pbucket);

			if(!(pcol_files && pcol_chunks))
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser gridfs_bucket_remove_files, mongoc_gridfs_bucket_t is failed");
				rm_count = 0;
			}
			else
			{
				if(!(mongoc_collection_remove(pcol_chunks, MONGOC_REMOVE_NONE, boost::addressof(chunks_query), 0, perr)
						&& mongoc_collection_remove(pcol_files, MONGOC_REMOVE_NONE, boost::addressof(files_query), 0, perr)))
				{
					rm_count = 0;
				}
			}
		}

		mongoc_cursor_destroy(pcur);

		mongoc_gridfs_bucket_destroy(pbucket);
		mongoc_database_destroy(pdb);

		bool bpush = conn.push(ptr);
		assert(bpush);

		return rm_count;
	}

	//------------------------gridfs_bucket e----------------------------
};

} // namespace nsql_database_system
} // namespace yggr

//#ifdef _MSC_VER
//#	pragma warning( pop )
//#endif //_MSC_VER

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_GRIDFS_HPP__
