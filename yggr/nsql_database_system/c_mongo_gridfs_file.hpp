//c_mongo_gridfs_file.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_GRIDFS_FILE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_GRIDFS_FILE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/support/operator_t.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp>
#include <yggr/nsql_database_system/c_mongo_iovec.hpp>
#include <yggr/nsql_database_system/mongoc_native_err_ex.hpp>

#include <yggr/nsql_database_system/c_bson_oid.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>
#include <yggr/nsql_database_system/detail/to_pointer.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_gridfs_file;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

class mongoc_gridfs_file_t_shared_ptr_deleter
{
public:
	inline void operator()(mongoc_gridfs_file_t* ptr) const
	{
		if(ptr)
		{
			mongoc_gridfs_file_destroy(ptr);
		}
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_gridfs_file
{
public:
	typedef mongoc_error_domain_ex_t mongoc_error_domain_type;
	typedef mongoc_error_code_ex_t mongoc_error_code_type;

public:
	typedef mongoc_gridfs_file_t org_type;
	typedef ::yggr::shared_ptr<org_type> gridfs_file_ptr_type;
	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

	typedef c_mongo_gridfs_file_opt file_opt_type;
	typedef file_opt_type::org_type org_file_opt_type;

	typedef c_bson bson_type;
	typedef c_bson_value bson_value_type;
	typedef c_bson_oid bson_oid_type;
	typedef c_bson_error bson_error_type;

	YGGR_STATIC_CONSTANT(u32, E_seek_pos_set = SEEK_SET);
	YGGR_STATIC_CONSTANT(u32, E_seek_pos_cur = SEEK_CUR);
	YGGR_STATIC_CONSTANT(u32, E_seek_pos_end = SEEK_END);

protected:
	typedef bson_type::org_type org_bson_type;
	typedef bson_error_type::org_type org_bson_error_type;

private:
	typedef c_mongo_gridfs_file this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef detail::mongoc_gridfs_file_t_shared_ptr_deleter shared_ptr_deleter_type;

public:
	typedef
		utility::member_var_modify_helper
		<
			this_type,
			mplex::null_type,
			inner_string_type,
			inner_string_view_type
		> inner_string_member_modify_helper_type;

	typedef
		utility::member_var_modify_helper
		<
			this_type,
			mplex::null_type,
			bson_t,
			bson_type
		> bson_member_modify_helper_type;

	typedef
		utility::member_var_modify_helper
		<
			this_type,
			mplex::null_type,
			bson_value_t,
			bson_value_type
		> bson_value_member_modify_helper_type;

	typedef
		utility::member_var_modify_helper
		<
			this_type,
			mplex::null_type,
			bson_oid_t,
			bson_oid_type
		> bson_oid_member_modify_helper_type;

public:
	c_mongo_gridfs_file(void);

	template<typename Opts>
	c_mongo_gridfs_file(mongoc_gridfs_t* pgridfs, const Opts& opts)
	{
		const org_file_opt_type* popts = detail::to_const_pointer<org_file_opt_type>(opts);
		if(!(pgridfs && popts))
		{
			return;
		}

		this_type::prv_init(pgridfs, popts);
	}

	template<typename Opts>
	c_mongo_gridfs_file(mongoc_gridfs_t* pgridfs, mongoc_stream_t* pstream, const Opts& opts)
	{
		const org_file_opt_type* popts = detail::to_const_pointer<org_file_opt_type>(opts);
		if(!(pgridfs && pstream && popts && popts->filename))
		{
			return;
		}

		this_type::prv_init(pgridfs, pstream, popts);
	}

	template<typename Stream, typename Opts>
	c_mongo_gridfs_file(mongoc_gridfs_t* pgridfs, Stream& stream, const Opts& opts)
	{
		mongoc_stream_t* pstream = stream;
		stream.release();
		const org_file_opt_type* popts = detail::to_const_pointer<org_file_opt_type>(opts);

		if(!(pgridfs && pstream && popts && popts->filename))
		{
			return;
		}

		this_type::prv_init(pgridfs, pstream, popts);
	}

	c_mongo_gridfs_file(org_type* pfile); // !!! don't wrapper twice!!!


	c_mongo_gridfs_file(BOOST_RV_REF(this_type) right)
		: _pfile(boost::move(right._pfile))
	{
	}

	c_mongo_gridfs_file(const this_type& right);

	~c_mongo_gridfs_file(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if((this == &right_ref)
			|| _pfile == right_ref._pfile)
		{
			return *this;
		}

		copy_or_move_or_swap(_pfile, boost::move(right_ref._pfile));
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

public:
	inline org_type* org_pointer(void)
	{
		return _pfile.get();
	}

	inline const org_type* org_pointer(void) const
	{
		return _pfile.get();
	}

public:
	inline operator org_type*(void)
	{
		return this_type::org_pointer();
	}

	inline operator const org_type*(void) const
	{
		return this_type::org_pointer();
	}

	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return !!_pfile;
	}

public:
	inline const bson_type* aliases_pointer(void) const
	{
		const bson_type::base_type* ptr
			= _pfile? mongoc_gridfs_file_get_aliases(_pfile.get()) : 0;

		return reinterpret_cast<const bson_type*>(ptr);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson*>::type
		aliases_pointer(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type*>(this_type::aliases_pointer());
	}

protected:
	inline bson_type pro_get_aliases(void) const
	{
		const bson_type::org_type* ptr
			= _pfile? mongoc_gridfs_file_get_aliases(_pfile.get()) : 0;
		return ptr? bson_type(*ptr) : bson_type();
	}

	inline void pro_set_aliases(const bson_t& val)
	{
		if(!_pfile)
		{
			return;
		}

		mongoc_gridfs_file_set_aliases(_pfile.get(), boost::addressof(val));
	}

public:
	inline bson_member_modify_helper_type aliases(void)
	{
		mplex::null_type null_obj;
		return
			bson_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_aliases,
				&this_type::pro_get_aliases);
	}

	inline bson_type aliases(void) const
	{
		const bson_type::org_type* ptr
			= _pfile? mongoc_gridfs_file_get_aliases(_pfile.get()) : 0;
		return ptr? bson_type(*ptr) : bson_type();
	}

public:
	inline const bson_type* metadata_pointer(void) const
	{
		const bson_type::org_type* ptr
			= _pfile? mongoc_gridfs_file_get_metadata(_pfile.get()) : 0;
		return reinterpret_cast<const bson_type*>(ptr);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson*>::type
		metadata_pointer(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type*>(this_type::metadata_pointer());
	}

protected:
	inline bson_type pro_get_metadata(void) const
	{
		const bson_type::org_type* ptr
			= _pfile? mongoc_gridfs_file_get_metadata(_pfile.get()) : 0;
		return ptr? bson_type(*ptr) : bson_type();
	}

	inline void pro_set_metadata(const bson_t& val)
	{
		if(!_pfile)
		{
			return;
		}

		mongoc_gridfs_file_set_metadata(_pfile.get(), boost::addressof(val));
	}

public:
	inline bson_member_modify_helper_type metadata(void)
	{
		mplex::null_type null_obj;
		return
			bson_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_metadata,
				&this_type::pro_get_metadata);
	}

	inline bson_type metadata(void) const
	{
		const bson_type::base_type* ptr
			= _pfile? mongoc_gridfs_file_get_metadata(_pfile.get()) : 0;
		return ptr? bson_type(*ptr) : bson_type();
	}

public:
	inline const char* content_pointer(void) const
	{
		return
			_pfile?
				mongoc_gridfs_file_get_content_type(_pfile.get())
				: static_cast<const char*>(0);
	}

protected:
	inline inner_string_view_type pro_get_content(void) const
	{
		return
			_pfile?
				inner_string_view_type(mongoc_gridfs_file_get_content_type(_pfile.get()))
				: inner_string_view_type();
	}

	inline void pro_set_content(const inner_string_type& val)
	{
		if(!_pfile)
		{
			return;
		}

		mongoc_gridfs_file_set_content_type(_pfile.get(), val.data());
	}

public:
	inline inner_string_member_modify_helper_type content(void)
	{
		mplex::null_type null_obj;
		return
			inner_string_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_content,
				&this_type::pro_get_content);
	}

	inline inner_string_view_type content(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_get_content();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	content(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String content(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	content(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(content)

public:
	inline const char* file_name_pointer(void) const
	{
		return
			_pfile?
				mongoc_gridfs_file_get_filename(_pfile.get())
				: static_cast<const char*>(0);
	}

protected:
	inline inner_string_view_type pro_get_file_name(void) const
	{
		return
			_pfile?
				inner_string_view_type(mongoc_gridfs_file_get_filename(_pfile.get()))
				: inner_string_view_type();
	}

	inline void pro_set_file_name(const inner_string_type& val)
	{
		if(!_pfile)
		{
			return;
		}

		mongoc_gridfs_file_set_filename(_pfile.get(), val.data());
	}

public:
	inline inner_string_member_modify_helper_type file_name(void)
	{
		mplex::null_type null_obj;
		return
			inner_string_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_file_name,
				&this_type::pro_get_file_name);
	}

	inline inner_string_view_type file_name(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_get_file_name();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	file_name(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String file_name(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	file_name(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(file_name)

public:
	inline const char* md5_pointer(void) const
	{
		return
			_pfile?
				mongoc_gridfs_file_get_md5(_pfile.get())
				: static_cast<const char*>(0);
	}

protected:
	inline inner_string_view_type pro_get_md5(void) const
	{
		return
			_pfile?
				inner_string_view_type(mongoc_gridfs_file_get_md5(_pfile.get()))
				: inner_string_view_type();
	}

	inline void pro_set_md5(const inner_string_type& val)
	{
		if(!_pfile)
		{
			return;
		}

		mongoc_gridfs_file_set_md5(_pfile.get(), val.data());
	}

public:
	inline inner_string_member_modify_helper_type md5(void)
	{
		mplex::null_type null_obj;
		return
			inner_string_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_md5,
				&this_type::pro_get_md5);
	}

	inline inner_string_view_type md5(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_get_md5();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	md5(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String md5(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	md5(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(md5)

	inline int32_t chunk_size(void) const
	{
		return _pfile? mongoc_gridfs_file_get_chunk_size(_pfile.get()) : 0;
	}

	org_file_opt_type file_opt_reference(void) const
	{
		org_file_opt_type ret
			= { this_type::md5_pointer(),
				this_type::file_name_pointer(),
				this_type::content_pointer(),
				this_type::aliases_pointer(),
				this_type::metadata_pointer(),
				static_cast<u32>(this_type::chunk_size()) };

		return ret;
	}

	inline file_opt_type file_opt(void) const
	{
		return
			file_opt_type(
				this_type::md5(),
				this_type::file_name(),
				this_type::content(),
				this_type::aliases(),
				this_type::metadata(),
				this_type::chunk_size());
	}


public:
	inline void clear(void)
	{
		gridfs_file_ptr_type tmp(static_cast<org_type*>(0), shared_ptr_deleter_type());
		tmp.swap(_pfile);
	}

	inline bool empty(void) const
	{
		return !_pfile;
	}

	inline int64_t size(void) const
	{
		return _pfile? mongoc_gridfs_file_get_length(_pfile.get()) : 0;
	}

	inline int64_t length(void) const
	{
		return this_type::size();
	}

	// default id real type is oid

public:
	inline const bson_value_t* var_id_pointer(void) const
	{
		return
			_pfile?
				mongoc_gridfs_file_get_id(_pfile.get())
				: static_cast<const bson_value_t*>(0);
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, const BsonValue*>::type
		var_id_pointer(void) const
	{
		typedef BsonValue now_bson_value_type;

		return
			_pfile?
				reinterpret_cast<const now_bson_value_type*>(mongoc_gridfs_file_get_id(_pfile.get()))
				: static_cast<const now_bson_value_type*>(0);
	}

protected:
	inline bson_value_type pro_get_var_id(void) const
	{
		const bson_value_type* pval = this_type::var_id_pointer<bson_value_type>();
		return pval? bson_value_type(*pval) : bson_value_type();
	}

	inline void pro_set_var_id(const bson_value_t& val)
	{
		if(!_pfile)
		{
			return;
		}

		bson_error_type err;

		bool bsuccess = mongoc_gridfs_file_set_id(_pfile.get(), boost::addressof(val), boost::addressof(err));

		if(!bsuccess)
		{
			throw err;
		}
	}

public:
	inline bson_value_member_modify_helper_type var_id(void)
	{
		mplex::null_type null_obj;
		return
			bson_value_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_var_id,
				&this_type::pro_get_var_id);
	}

	inline bson_value_type var_id(void) const
	{
		return this_type::pro_get_var_id();
	}

protected:
	inline bson_oid_type pro_get_id(void) const
	{
		const bson_value_type* pval = this_type::var_id_pointer<bson_value_type>();

		bson_oid_type bs_oid(bson_oid_type::s_gen_empty_oid());
		return (pval && pval->get_value(YGGR_BSON_VALUE_TAG_OBJECT(oid), bs_oid), bs_oid);
	}

	inline void pro_set_id(const bson_oid_t& val)
	{
		if(!_pfile)
		{
			return;
		}

		bson_value_type bs_val(val);
		this_type::pro_set_var_id(bs_val);
	}

public:
	inline bson_oid_member_modify_helper_type id(void)
	{
		mplex::null_type null_obj;
		return
			bson_oid_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_id,
				&this_type::pro_get_id);
	}

	inline bson_oid_type id(void) const
	{
		return this_type::pro_get_id();
	}

	inline int64_t upload_date(void) const
	{
		return _pfile? mongoc_gridfs_file_get_upload_date(_pfile.get()) : 0;
	}

	inline bson_error_type error(void) const
	{
		bson_error_type err;
		if(!_pfile)
		{
			bson_set_error(boost::addressof(err),
							mongoc_error_domain_ex_t::E_MONGOC_ERROR_GRIDFS,
							mongoc_error_code_ex_t::E_MONGOC_ERROR_EMPTY_OBJECT,
							"c_mongo_gridfs_file empty");
			return err;
		}

		return mongoc_gridfs_file_error(_pfile.get(), boost::addressof(err))? err : bson_error_type();
	}

	template<typename BsonErr> inline
	typename boost::enable_if< boost::is_base_of<bson_error_t, BsonErr>, BsonErr>::type
		error(void) const
	{
		BsonErr err;
		bson_error_t* perr = bson_error_native_ex::s_bson_error_init(boost::addressof(err));

		if(!_pfile)
		{
			bson_set_error(perr,
							mongoc_error_domain_ex_t::E_MONGOC_ERROR_GRIDFS,
							mongoc_error_code_ex_t::E_MONGOC_ERROR_EMPTY_OBJECT,
							"c_mongo_gridfs_file empty");
			return err;
		}

		mongoc_gridfs_file_error(_pfile.get(), perr);
		return err;
	}

	inline bool error(org_bson_error_type& err) const
	{
		return mongoc_gridfs_file_error(_pfile.get(), &err);
	}

	inline bool error(org_bson_error_type* perr) const
	{
		return mongoc_gridfs_file_error(_pfile.get(), perr);
	}

	inline bson_error_type remove(void)
	{
		bson_error_type err;
		if(!_pfile)
		{
			bson_set_error(
				&err,
				mongoc_error_domain_ex_t::E_MONGOC_ERROR_GRIDFS,
				mongoc_error_code_ex_t::E_MONGOC_ERROR_EMPTY_OBJECT,
				"c_mongo_gridfs_file empty");
			return err;
		}

		bool bhas_err = mongoc_gridfs_file_remove(_pfile.get(), &err);

		return bhas_err? err : bson_error_type();
	}

	template<typename BsonErr> inline
	typename boost::enable_if< boost::is_base_of<bson_error_t, BsonErr>, BsonErr>::type
		remove(void)
	{
		BsonErr err;
		bson_error_t* perr = bson_error_native_ex::s_bson_error_init(&err);

		if(!_pfile)
		{
			bson_set_error(
				perr,
				mongoc_error_domain_ex_t::E_MONGOC_ERROR_GRIDFS,
				mongoc_error_code_ex_t::E_MONGOC_ERROR_EMPTY_OBJECT,
				"c_mongo_gridfs_file empty");
			return err;
		}

		bool bhas_err = mongoc_gridfs_file_remove(_pfile.get(), perr);

		return err;
	}

	inline bool remove(org_bson_error_type* perr)
	{
		assert(perr);

		perr = bson_error_native_ex::s_bson_error_init(perr);

		if(!_pfile)
		{
			bson_set_error(
				perr,
				mongoc_error_domain_ex_t::E_MONGOC_ERROR_GRIDFS,
				mongoc_error_code_ex_t::E_MONGOC_ERROR_EMPTY_OBJECT,
				"c_mongo_gridfs_file empty");
			return false;
		}

		return mongoc_gridfs_file_remove(_pfile.get(), perr);
	}

	inline bool remove(org_bson_error_type& err)
	{
		return this_type::remove(boost::addressof(err));
	}

	inline bool seek(int64_t delta, int whence)
	{
		return _pfile && 0 == mongoc_gridfs_file_seek(_pfile.get(), delta, whence);
	}

	inline uint64_t tell(void) const
	{
		return _pfile? mongoc_gridfs_file_tell(_pfile.get()) : 0;
	}

public:
	inline bool save(void)
	{
		return _pfile && mongoc_gridfs_file_save(_pfile.get());
	}

	inline bool save(org_bson_error_type* perr)
	{
		assert(perr);
		perr = bson_error_native_ex::s_bson_error_init(perr);

		if(!_pfile)
		{
			bson_set_error(
				perr,
				mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
				mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
				"mongo_accesser insert, command invalid args");
			return false;
		}

		bool bsuccess = mongoc_gridfs_file_save(_pfile.get());

		if(!bsuccess)
		{
			mongoc_gridfs_file_error(_pfile.get(), perr);
		}

		return bsuccess;
	}

	inline bool save(org_bson_error_type& err)
	{
		return this_type::save(boost::addressof(err));
	}

	template<typename Container> inline
	ssize_t read(Container& cont, size_t min_bytes = -1, int32_t timeout_msec = 0) const
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type value_type;

		BOOST_MPL_ASSERT((boost::is_base_of<mongoc_iovec_t, value_type>));

		return
			_pfile?
				mongoc_gridfs_file_readv(
					_pfile.get(),
					&(*boost::begin(cont)),
					boost::size(cont),
					min_bytes, timeout_msec)
				: -1;
	}


	inline ssize_t read(mongoc_iovec_t* iov, size_t iovcnt, size_t min_bytes = -1, int32_t timeout_msec = 0) const
	{
		assert(iov);

		return
			(iov && _pfile)?
				mongoc_gridfs_file_readv(
					_pfile.get(),
					iov, iovcnt,
					min_bytes, timeout_msec)
				: -1;
	}

	inline ssize_t read(mongoc_iovec_t& iov, size_t min_bytes = -1, int32_t timeout_msec = 0) const
	{
		return
			(iov.iov_len && iov.iov_base)?
				this_type::read(&iov, 1, min_bytes, timeout_msec)
				: -1;
	}

	inline ssize_t read(c_mongo_iovec& iov, size_t min_bytes = -1, int32_t timeout_msec = 0) const
	{
		return
			(iov.iov_len && iov.iov_base)?
				this_type::read(&iov, 1, min_bytes, timeout_msec)
				: -1;
	}

	template<typename Container> inline
	ssize_t write(Container& cont, int32_t timeout_msec = 0) const
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type value_type;

		BOOST_MPL_ASSERT((boost::is_base_of<mongoc_iovec_t, value_type>));

		return
			_pfile?
				mongoc_gridfs_file_writev(
					_pfile.get(),
					&(*boost::begin(cont)),
					boost::size(cont),
					timeout_msec)
				: -1;
	}

	inline ssize_t write(mongoc_iovec_t* iov, size_t iovcnt, int32_t timeout_msec = 0)
	{
		assert(iov);
		return iov && _pfile? mongoc_gridfs_file_writev(_pfile.get(), iov, iovcnt, timeout_msec) : -1;
	}

	inline ssize_t write(mongoc_iovec_t& iov, int32_t timeout_msec = 0)
	{
		return (iov.iov_len && iov.iov_base)? this_type::write(&iov, 1, timeout_msec) : -1;
	}

	inline ssize_t write(c_mongo_iovec& iov, int32_t timeout_msec = 0)
	{
		return (iov.iov_len && iov.iov_base)? this_type::write(&iov, 1, timeout_msec) : -1;
	}

private:
	void prv_init(mongoc_gridfs_t* pgridfs, const mongoc_gridfs_file_opt_t* popt);
	void prv_init(mongoc_gridfs_t* pgridfs, mongoc_stream_t* pstream, const mongoc_gridfs_file_opt_t* popt);

private:
	gridfs_file_ptr_type _pfile;
};

// non-member foo
inline bool operator<(const c_mongo_gridfs_file& l, const c_mongo_gridfs_file& r)
{
	return l.file_name() < r.file_name();
}

inline bool operator<=(const c_mongo_gridfs_file& l, const c_mongo_gridfs_file& r)
{
	return l.file_name() <= r.file_name();
}

inline bool operator>(const c_mongo_gridfs_file& l, const c_mongo_gridfs_file& r)
{
	return l.file_name() > r.file_name();
}

inline bool operator>=(const c_mongo_gridfs_file& l, const c_mongo_gridfs_file& r)
{
	return l.file_name() >= r.file_name();
}

inline bool operator==(const c_mongo_gridfs_file& l, const c_mongo_gridfs_file& r)
{
	const c_mongo_gridfs_file::org_type* pl = l.org_pointer();
	const c_mongo_gridfs_file::org_type* pr = r.org_pointer();
	return (pl == pr) || (l.id() == r.id());
}

inline bool operator!=(const c_mongo_gridfs_file& l, const c_mongo_gridfs_file& r)
{
	const c_mongo_gridfs_file::org_type* pl = l.org_pointer();
	const c_mongo_gridfs_file::org_type* pr = r.org_pointer();
	return !((pl == pr) || (l.id() == r.id()));
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_gridfs_file)
} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_GRIDFS_FILE_HPP__
