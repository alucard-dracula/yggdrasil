//c_mongo_auto_encryption_opts.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_AUTO_ENCRYPTION_OPTS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_AUTO_ENCRYPTION_OPTS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_mongo_mc_kms_credentials_callback.hpp>
#include <yggr/nsql_database_system/mongoc_auto_encryption_opts_native_ex.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>
#include <yggr/nsql_database_system/detail/mongo_encryption_bson_member_get_set_macro.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/preprocessor/cat.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct yggr_mc_kms_credentials_callback 
//{
//	mongoc_kms_credentials_provider_callback_fn fn;
//	void *userdata;
//};

//struct _mongoc_auto_encryption_opts_t {
//	/* keyvault_client and keyvault_client_pool are not owned and must outlive
//	* auto encrypted client/pool. */
//	mongoc_client_t *keyvault_client;
//	mongoc_client_pool_t *keyvault_client_pool;
//	char *keyvault_db;
//	char *keyvault_coll;
//	bson_t *kms_providers;
//	bson_t *tls_opts;
//	bson_t *schema_map;
//	bson_t *encrypted_fields_map;
//	bool bypass_auto_encryption;
//	bool bypass_query_analysis;
//	mc_kms_credentials_callback creds_cb;
//	bson_t *extra;
//};

class c_mongo_auto_encryption_opts
	: protected yggr_mongoc_auto_encryption_opts
{
public:
	typedef yggr_mongoc_auto_encryption_opts base_type;
	typedef mongoc_auto_encryption_opts_t org_type;

	typedef c_bson bson_type;
	typedef charset::utf8_string inner_string_type;
	typedef charset::utf8_string_view inner_string_view_type;

	typedef c_mc_kms_credentials_callback mc_kms_credentials_callback_type;
	typedef mongoc_kms_credentials_provider_callback_fn kms_credentials_provider_callback_fn_type;
	
private:
	typedef c_mongo_auto_encryption_opts this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			inner_string_type,
			inner_string_type,
			const inner_string_type&
		> inner_string_member_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			mplex::null_type,
			inner_string_type,
			inner_string_type
		> keyvault_namespace_member_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			bson_type,
			bson_t,
			const bson_type&
		> bson_member_modify_helper_type;

public:
	c_mongo_auto_encryption_opts(void);

	template<typename StrKeyvaultDB,
				typename StrKeyvaultColl, 
				typename BsonKmsProviders,
				typename BsonTlsOpts,
				typename BsonSchemaMap>
	c_mongo_auto_encryption_opts(const StrKeyvaultDB& str_keyvault_db,
									const StrKeyvaultColl& str_keyvault_coll,
									const BsonKmsProviders& bs_kms_providers,
									const BsonTlsOpts& bs_tls_opts,
									const BsonSchemaMap& bs_schema_map)
		: _keyvault_db(str_keyvault_db), _keyvault_coll(str_keyvault_coll),
			_kms_providers(bs_kms_providers), _tls_opts(bs_tls_opts),
			_schema_map(bs_schema_map)
	{
		this_type::pro_init();
		this_type::pro_rebuild_base();
	}

	template<typename StrKeyvaultDB, typename StrKeyvaultColl, 
				typename BsonKmsProviders,
				typename BsonTlsOpts,
				typename BsonSchemaMap,
				typename BsonEncryptedFieldsMap,
				typename BsonExtra>
	c_mongo_auto_encryption_opts(const StrKeyvaultDB& str_keyvault_db, 
									const StrKeyvaultColl& str_keyvault_coll,
									const BsonKmsProviders& bs_kms_providers,
									const BsonTlsOpts& bs_tls_opts,
									const BsonSchemaMap& bs_schema_map,
									const BsonEncryptedFieldsMap& bs_encrypted_map,
									bool b_bypass_auto_encryption,
									bool b_bypass_query_analysis,
									const BsonExtra& bs_extra,
									kms_credentials_provider_callback_fn_type callback_fn = 0,
									void* userdata = 0)
		: _keyvault_db(str_keyvault_db), _keyvault_coll(str_keyvault_coll),
			_kms_providers(bs_kms_providers), _tls_opts(bs_tls_opts),
			_schema_map(bs_schema_map), _encrypted_fields_map(bs_encrypted_map),
			_extra(bs_extra)
	{
		this_type::pro_init();
		base_type::bypass_auto_encryption = b_bypass_auto_encryption;
		base_type::bypass_query_analysis = b_bypass_query_analysis;
		this_type::pro_set_kms_credential_provider_callback(callback_fn, userdata);
		this_type::pro_rebuild_base();
	}

	template<typename StrKeyvaultDB,
				typename StrKeyvaultColl, 
				typename BsonKmsProviders,
				typename BsonTlsOpts,
				typename BsonSchemaMap,
				typename BsonEncryptedFieldsMap,
				typename BsonExtra,
				typename UserData>
	c_mongo_auto_encryption_opts(const StrKeyvaultDB& str_keyvault_db,
									const StrKeyvaultColl& str_keyvault_coll,
									const BsonKmsProviders& bs_kms_providers,
									const BsonTlsOpts& bs_tls_opts,
									const BsonSchemaMap& bs_schema_map,
									const BsonEncryptedFieldsMap& bs_encrypted_map,
									bool b_bypass_auto_encryption,
									bool b_bypass_query_analysis,
									const BsonExtra& bs_extra,
									kms_credentials_provider_callback_fn_type callback_fn,
									UserData& userdata)
		: _keyvault_db(str_keyvault_db), _keyvault_coll(str_keyvault_coll),
			_kms_providers(bs_kms_providers), _tls_opts(bs_tls_opts),
			_schema_map(bs_schema_map), _encrypted_fields_map(bs_encrypted_map),
			_extra(bs_extra)
	{
		this_type::pro_init();
		base_type::bypass_auto_encryption = b_bypass_auto_encryption;
		base_type::bypass_query_analysis = b_bypass_query_analysis;
		this_type::pro_set_kms_credential_provider_callback(callback_fn, userdata);
		this_type::pro_rebuild_base();
	}

	c_mongo_auto_encryption_opts(const org_type* ptr);
	c_mongo_auto_encryption_opts(const org_type& right);

	c_mongo_auto_encryption_opts(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_init();
		this_type::pro_move(right_ref);
	}

	c_mongo_auto_encryption_opts(const this_type& right);
	~c_mongo_auto_encryption_opts(void);

public:
	this_type& operator=(const org_type& right);
	this_type& operator=(const base_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		return this_type::pro_move(right_ref);
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
		base_type& base = *this;
		return reinterpret_cast<org_type*>(boost::addressof(base));
	}

	inline const org_type* org_pointer(void) const
	{
		const base_type& base = *this;
		return reinterpret_cast<const org_type*>(boost::addressof(base));
	}

public:
	inline operator org_type&(void)
	{
		return *(this_type::org_pointer());
	}

	inline operator const org_type&(void) const
	{
		return *(this_type::org_pointer());
	}

	inline operator org_type*(void)
	{
		return this_type::org_pointer();
	}

	inline operator const org_type*(void) const
	{
		return this_type::org_pointer();
	}

public:
	/* 
		not support get set keyvault_client and keyvault_client_pool,
		because fixed in mongo_accesss
	 */

	// var_keyvault_db
protected:
	inline const inner_string_type& pro_get_var_keyvault_db(void) const
	{
		return _keyvault_db;
	}

	inline void pro_set_var_keyvault_db(const inner_string_type& str_db)
	{
		_keyvault_db = str_db;
		base_type::keyvault_db = _keyvault_db.empty()? 0 : _keyvault_db.data();
	}

public:
	inline inner_string_member_modify_helper_type var_keyvault_db(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _keyvault_db,
				&this_type::pro_set_var_keyvault_db,
				&this_type::pro_get_var_keyvault_db);
	}

	// inline const inner_string_type& var_keyvault_db(void) const;
	
	// template<typename ...Args> inline 
	// const inner_string_type& var_keyvault_db(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String var_keyvault_db(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_keyvault_db(), var_keyvault_db, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_keyvault_db(), var_keyvault_db, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_var_keyvault_db(), var_keyvault_db)

	// var_keyvault_coll
protected:
	inline const inner_string_type& pro_get_var_keyvault_coll(void) const
	{
		return _keyvault_coll;
	}

	inline void pro_set_var_keyvault_coll(const inner_string_type& str_col)
	{
		_keyvault_coll = str_col;
		base_type::keyvault_coll = _keyvault_coll.empty()? 0 : _keyvault_coll.data();
	}

public:
	inline inner_string_member_modify_helper_type var_keyvault_coll(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _keyvault_coll,
				&this_type::pro_set_var_keyvault_coll,
				&this_type::pro_get_var_keyvault_coll);
	}

	// inline const inner_string_type& var_keyvault_coll(void) const;
	
	// template<typename ...Args> inline 
	// const inner_string_type& var_keyvault_coll(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String var_keyvault_coll(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_keyvault_coll(), var_keyvault_coll, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_keyvault_coll(), var_keyvault_coll, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_var_keyvault_coll(), var_keyvault_coll)

	// keyvault_namespace
protected:
	inline inner_string_type pro_get_keyvault_namespace(void) const
	{
		static const inner_string_type str_dot(".", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		
		return 
			base_type::keyvault_db && base_type::keyvault_coll?
			(_keyvault_db + str_dot + _keyvault_coll)
			: inner_string_type();
	}

	inline void pro_set_keyvault_namespace(const inner_string_type& str_namespace)
	{
		static const inner_string_type str_dot(".", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		std::size_t dot_pos = str_namespace.find(str_dot);
	

		if((dot_pos == inner_string_type::npos) || (dot_pos == str_namespace.size() - 1))
		{
			_keyvault_db.clear();
			base_type::keyvault_db = 0;

			_keyvault_coll.clear();
			base_type::keyvault_coll = 0;
		}
		else
		{
			_keyvault_db = str_namespace.substr(0, dot_pos);
			base_type::keyvault_db = _keyvault_db.empty()? 0 : _keyvault_db.data();

			_keyvault_coll = str_namespace.substr(dot_pos + 1, str_namespace.size() - dot_pos - 1);
			base_type::keyvault_coll = _keyvault_coll.empty()? 0 : _keyvault_coll.data();
		}
	}

public:
	inline keyvault_namespace_member_modify_helper_type keyvault_namespace(void)
	{
		mplex::null_type null_obj;
		return 
			keyvault_namespace_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_keyvault_namespace,
				&this_type::pro_get_keyvault_namespace);
	}

	// inline inner_string_type keyvault_namespace(void) const;
	
	// template<typename ...Args> inline 
	// inner_string_type keyvault_namespace(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String keyvault_namespace(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline 
	inner_string_type keyvault_namespace(BOOST_FWD_REF(Args)... args) const 
	{ 
		return this_type::pro_get_keyvault_namespace(); 
	} 
	
	template<typename String, typename ...Args> inline 
	typename boost::enable_if<boost::is_same<String, inner_string_type>, String>::type 
		keyvault_namespace(BOOST_FWD_REF(Args)... args) const 
	{
		return this_type::pro_get_keyvault_namespace(); 
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		inner_string_type keyvault_namespace( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return this_type::pro_get_keyvault_namespace(); } \
		\
		template< typename String \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if<boost::is_same<String, inner_string_type>, String>::type \
			keyvault_namespace( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return this_type::pro_get_keyvault_namespace(); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename String> inline
	typename boost::disable_if<boost::is_same<String, inner_string_type>, String>::type
		keyvault_namespace(const string& charset_name = charset::charset_name_t<String>()) const
	{
		typedef String now_string_type;
		return 
			charset::string_converter::s_conv<now_string_type>(
				this_type::pro_get_keyvault_namespace(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), charset_name);
	}
	
	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_same<String, inner_string_type> >,
				container_ex::is_allocator<Alloc> 
			>,
			String
		>::type
		keyvault_namespace(const Alloc& alloc,
							const string& charset_name
								= charset::charset_name_t<String>()) const
	{
		typedef String now_string_type;
		return 
			charset::string_converter::s_conv<now_string_type>(
				this_type::pro_get_keyvault_namespace(), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), charset_name);
	}

	// var_bson_member

	// var_kms_providers

	//inline bson_member_modify_helper_type var_kms_providers(void);

	//template<typename Bson> inline 
	//typename
	//	boost::enable_if< 
	//		boost::is_base_of<bson_t, Bson>,
	//		utility::member_var_modify_helper< 
	//			this_type, 
	//			bson_type, 
	//			bson_t, 
	//			const Bson& > >::type 
	//	var_kms_providers(void);

	//inline const bson_type& var_kms_providers(void) const;
	
	//template<typename Bson> inline 
	//typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
	//	var_kms_providers(void) const;

	YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET(kms_providers)

	// var_tls_opts

	//inline bson_member_modify_helper_type var_tls_opts(void);

	//template<typename Bson> inline 
	//typename
	//	boost::enable_if< 
	//		boost::is_base_of<bson_t, Bson>,
	//		utility::member_var_modify_helper< 
	//			this_type, 
	//			bson_type, 
	//			bson_t, 
	//			const Bson& > >::type 
	//	var_tls_opts(void);

	//inline const bson_type& var_tls_opts(void) const;
	
	//template<typename Bson> inline 
	//typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
	//	var_tls_opts(void) const;

	YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET(tls_opts)

	// var_schema_map

	//inline bson_member_modify_helper_type var_schema_map(void);

	//template<typename Bson> inline 
	//typename
	//	boost::enable_if< 
	//		boost::is_base_of<bson_t, Bson>,
	//		utility::member_var_modify_helper< 
	//			this_type, 
	//			bson_type, 
	//			bson_t, 
	//			const Bson& > >::type 
	//	var_schema_map(void);

	//inline const bson_type& var_schema_map(void) const;
	
	//template<typename Bson> inline 
	//typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
	//	var_schema_map(void) const;

	YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET(schema_map)

	// var_encrypted_fields_map

	//inline bson_member_modify_helper_type var_encrypted_fields_map(void);

	//template<typename Bson> inline 
	//typename
	//	boost::enable_if< 
	//		boost::is_base_of<bson_t, Bson>,
	//		utility::member_var_modify_helper< 
	//			this_type, 
	//			bson_type, 
	//			bson_t, 
	//			const Bson& > >::type 
	//	var_encrypted_fields_map(void);

	//inline const bson_type& var_encrypted_fields_map(void) const;
	
	//template<typename Bson> inline 
	//typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
	//	var_encrypted_fields_map(void) const;

	YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET(encrypted_fields_map)

	// var_extra

	//inline bson_member_modify_helper_type var_extra(void);

	//template<typename Bson> inline 
	//typename
	//	boost::enable_if< 
	//		boost::is_base_of<bson_t, Bson>,
	//		utility::member_var_modify_helper< 
	//			this_type, 
	//			bson_type, 
	//			bson_t, 
	//			const Bson& > >::type 
	//	var_extra(void);

	//inline const bson_type& var_extra(void) const;
	
	//template<typename Bson> inline 
	//typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
	//	var_extra(void) const;

	YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET(extra)

public:
	inline bool& var_bypass_auto_encryption(void)
	{
		return base_type::bypass_auto_encryption;
	}

	inline bool var_bypass_auto_encryption(void) const
	{
		return base_type::bypass_auto_encryption;
	}

	inline bool& var_bypass_query_analysis(void)
	{
		return base_type::bypass_query_analysis;
	}

	inline bool var_bypass_query_analysis(void) const
	{
		return base_type::bypass_query_analysis;
	}

public:
	inline mc_kms_credentials_callback_type& var_creds_cb(void)
	{
		mc_kms_credentials_callback_type* ptr = 
			reinterpret_cast<mc_kms_credentials_callback_type*>(boost::addressof(base_type::creds_cb));
		assert(ptr);
		return *ptr;
	}

	inline const mc_kms_credentials_callback_type& var_creds_cb(void) const
	{
		const mc_kms_credentials_callback_type* ptr = 
			reinterpret_cast<const mc_kms_credentials_callback_type*>(boost::addressof(base_type::creds_cb));
		assert(ptr);
		return *ptr;
	}

	inline kms_credentials_provider_callback_fn_type& creds_cb_fn(void)
	{
		return base_type::creds_cb.fn;
	}

	inline kms_credentials_provider_callback_fn_type creds_cb_fn(void) const
	{
		return base_type::creds_cb.fn;
	}

	inline void*& creds_cb_userdata(void)
	{
		return base_type::creds_cb.userdata;
	}

	template<typename UserData> inline
	UserData*& creds_cb_userdata(void)
	{
		typedef UserData userdata_type;
		void*& userdata_ref = base_type::creds_cb.userdata;
		return reinterpret_cast<userdata_type*&>(userdata_ref);
	}

	inline void* creds_cb_userdata(void) const
	{
		return base_type::creds_cb.userdata;
	}

	template<typename UserData> inline
	UserData* creds_cb_userdata(void) const
	{
		typedef UserData userdata_type;
		return reinterpret_cast<userdata_type*>(base_type::creds_cb.userdata);
	}

public:
	inline bool validate(void) const
	{
		return 
			base_type::keyvault_db 
			&& base_type::keyvault_coll
			&& base_type::kms_providers
			&& base_type::schema_map;
	}

protected:
	void pro_init(void);
	void pro_rebuild_base(void);
	void pro_clear(void);
	this_type& pro_copy(const org_type* ptr);
	this_type& pro_copy(const this_type& right);
	void pro_swap(this_type& right);
	this_type& pro_move(this_type& right);

protected:
	inline void pro_set_kms_credential_provider_callback(kms_credentials_provider_callback_fn_type fn, void* userdata)
	{
		base_type* pbase = this;
		mongoc_auto_encryption_opts_native_ex::s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback(
			static_cast<void*>(pbase), fn, userdata);
	}

	template<typename UserData> inline
	void pro_set_kms_credential_provider_callback(kms_credentials_provider_callback_fn_type fn, UserData& userdata)
	{
		base_type* pbase = this;
		mongoc_auto_encryption_opts_native_ex::s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback(
			static_cast<void*>(pbase), fn, userdata);
	}

private:
	//mongoc_client_t* keyvault_client; // using native
	//mongoc_client_pool_t* keyvault_client_pool; // using_native
	inner_string_type _keyvault_db;
	inner_string_type _keyvault_coll;
	bson_type _kms_providers;
	bson_type _tls_opts;
	bson_type _schema_map;
	bson_type _encrypted_fields_map;
	//bool bypass_auto_encryption; // using native
	//bool bypass_query_analysis; // using native
	//mc_kms_credentials_callback creds_cb; // using native
	bson_type _extra;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_auto_encryption_opts)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_AUTO_ENCRYPTION_OPTS_HPP__
