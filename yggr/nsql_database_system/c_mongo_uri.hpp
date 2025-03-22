//c_mongo_uri.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_URI_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_URI_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/empty.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/type_traits/native_t.hpp>
#include <yggr/type_traits/traits.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/mongoc_uri_native_ex.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_mongo_read_prefs.hpp>
#include <yggr/nsql_database_system/c_mongo_read_concern.hpp>
#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>
#include <yggr/nsql_database_system/c_mongo_host_info.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>

#include <boost/ref.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>

#include <ostream>

/*
https://mongoc.org/libmongoc/current/mongoc_uri_t.html
mongodb[+srv]://                             <1>
   [username:password@]                      <2>
   host1                                     <3>
   [:port1]                                  <4>
   [,host2[:port2],...[,hostN[:portN]]]      <5>
   [/[database]                              <6>
   [?options]]                               <7>

*/

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_uri;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_uri_t {
//   char *str;
//   bool is_srv;
//   char srv[BSON_HOST_NAME_MAX + 1];
//   mongoc_host_list_t *hosts;
//   char *username;
//   char *password;
//   char *database;
//   bson_t raw;     /* Unparsed options, see mongoc_uri_parse_options */
//   bson_t options; /* Type-coerced and canonicalized options */
//   bson_t credentials;
//   bson_t compressors;
//   mongoc_read_prefs_t *read_prefs;
//   mongoc_read_concern_t *read_concern;
//   mongoc_write_concern_t *write_concern;
//};

/*!!! mongoc_uri_parse has unescape, so don't using unescape first !!!*/

class c_mongo_uri
	: protected yggr_mongoc_uri_t
{
public:
	typedef yggr_mongoc_uri_t base_type;
	typedef mongoc_uri_t org_type;

	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

	typedef c_bson bson_type;
	typedef c_bson_error bson_error_type;

	typedef c_mongo_read_prefs read_prefs_type;
	typedef c_mongo_read_concern read_concern_type;
	typedef c_mongo_write_concern write_concern_type;
	typedef c_mongo_host_info mongo_host_info_type;
	typedef container::list<mongo_host_info_type> mongo_host_info_list_type;

private:
	typedef mongo_host_info_type::base_type org_host_list_op_type;
	typedef bson_type::org_type org_bson_type;

private:
	typedef c_mongo_uri this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

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
			inner_string_view_type
		> non_member_string_view_modify_helper_type;

	typedef
		utility::member_var_modify_helper
		<
			this_type,
			org_bson_type,
			inner_string_type,
			const bson_type&
		> member_compressors_modify_helper_type;

public:
	c_mongo_uri(void);
	c_mongo_uri(bson_error_type& outerr);

	template<typename String>
	c_mongo_uri(const String& str_uri,
				typename
					boost::enable_if
					<
						charset::utf8_string_constructable<String>,
						sfinae_type
					>::type sfinae = 0)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type utf8_str_uri(charset::string_charset_helper_data(str_uri));
		base_type tmp_base = {0};

		if(mongoc_uri_native_ex::s_mongoc_uri_init(&tmp_base, utf8_str_uri.c_str(), 0))
		{
			this_type::prv_init(tmp_base);
		}
		else
		{
			this_type::prv_init_default();
		}

		mongoc_uri_native_ex::s_mongoc_uri_clear(&tmp_base);
	}

	template<typename String>
	c_mongo_uri(const String& str_uri,
					bson_error_type& outerr,
					typename
						boost::enable_if
						<
							charset::utf8_string_constructable<String>,
							sfinae_type
						>::type sfinae = 0)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type utf8_str_uri(charset::string_charset_helper_data(str_uri));
		base_type tmp_base = {0};

		if(mongoc_uri_native_ex::s_mongoc_uri_init(
			&tmp_base, utf8_str_uri.c_str(), boost::addressof(outerr)))
		{
			this_type::prv_init(tmp_base);
		}
		else
		{
			this_type::prv_init_default();
		}

		mongoc_uri_native_ex::s_mongoc_uri_clear(&tmp_base);

	}

	template<typename String>
	c_mongo_uri(const String& str_host, u16 port,
				typename
					boost::enable_if
					<
						charset::utf8_string_constructable<String>,
						sfinae_type
					>::type sfinae = 0)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type utf8_str_host(charset::string_charset_helper_data(str_host));
		base_type tmp_base = {0};

		if(mongoc_uri_native_ex::s_mongoc_uri_init(&tmp_base, utf8_str_host.c_str(), port, 0))
		{
			this_type::prv_init(tmp_base);
		}
		else
		{
			this_type::prv_init_default();
		}

		mongoc_uri_native_ex::s_mongoc_uri_clear(&tmp_base);
	}

	template<typename String>
	c_mongo_uri(const String& str_host, u16 port,
				bson_error_type& outerr,
				typename
					boost::enable_if
					<
						charset::utf8_string_constructable<String>,
						sfinae_type
					>::type sfinae = 0)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type utf8_str_host(charset::string_charset_helper_data(str_host));
		base_type tmp_base = {0};

		if(mongoc_uri_native_ex::s_mongoc_uri_init(
			&tmp_base, utf8_str_host.c_str(), port, boost::addressof(outerr)))
		{
			this_type::prv_init(tmp_base);
		}
		else
		{
			this_type::prv_init_default();
		}

		mongoc_uri_native_ex::s_mongoc_uri_clear(&tmp_base);
	}

	explicit c_mongo_uri(const org_type* right);
	c_mongo_uri(const org_type& right);

	c_mongo_uri(BOOST_RV_REF(this_type) right)
	{
		this_type::prv_init_default();

		this_type& right_ref = right;
		this_type::pro_move(right_ref);
	}

	c_mongo_uri(const this_type& right);
	~c_mongo_uri(void);

public:
	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			this_type&
		>::type
		operator=(const String& right)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type utf8_str_uri(charset::string_charset_helper_data(right));
		base_type tmp_base = {0};

		if(mongoc_uri_native_ex::s_mongoc_uri_init(&tmp_base, utf8_str_uri.c_str(), 0))
		{
			this_type::pro_copy(tmp_base);
		}
		else
		{
			this_type::prv_init_default();
		}

		mongoc_uri_native_ex::s_mongoc_uri_clear(&tmp_base);

		return *this;
	}

	this_type& operator=(const org_type& right);

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
	inline bool empty(void) const
	{
		return !(base_type::str && _str.size());
	}

	inline bool failed(void) const
	{
		return this_type::empty();
	}

public:
	inline inner_string_view_type var_scheme(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		if(!base_type::str)
		{
			return inner_string_view_type();
		}

		if(0 == strncmp(base_type::str, mongoc_uri_native_ex::s_mongoc_uri_scheme_srv_record(), 14))
		{
			return inner_string_view_type(base_type::str, 14);
		}

		if(0 == strncmp(base_type::str, mongoc_uri_native_ex::s_mongoc_uri_scheme(), 10))
		{
			return inner_string_view_type(base_type::str, 10);
		}

		return inner_string_view_type();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	var_scheme(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String var_scheme(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	var_scheme(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(var_scheme)

public:
	template<typename Container>
	Container& var_hosts(Container& cont) const
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type chk_type;

		BOOST_MPL_ASSERT((boost::is_same<mongo_host_info_type, chk_type>));

		const mongoc_host_list_t* plist
				= base_type::str? mongoc_uri_native_ex::s_mongoc_uri_get_hosts(this) : 0;

		const org_host_list_op_type* pyggr_list = reinterpret_cast<const org_host_list_op_type*>(plist);

		for(const org_host_list_op_type* i = pyggr_list; i; i = i->next)
		{
			mongo_host_info_type info(reinterpret_cast<const mongoc_host_list_t*>(i));
			iterator_ex::inserter(cont, boost::end(cont)) = boost::move(info);
		}

		return cont;
	}

	template<typename Container> inline
	Container var_hosts(void) const
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type chk_type;

		BOOST_MPL_ASSERT((boost::is_same<mongo_host_info_type, chk_type>));

		cont_type cont;
		this_type::var_hosts(cont);
		return cont;
	}

	template<typename Container, typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			Container
		>::type
		var_hosts(const Alloc& alloc) const
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type chk_type;

		BOOST_MPL_ASSERT((boost::is_same<mongo_host_info_type, chk_type>));

		cont_type cont(alloc);
		this_type::var_hosts(cont);
		return cont;
	}

	inline mongo_host_info_list_type var_hosts(void) const
	{
		mongo_host_info_list_type cont;
		this_type::var_hosts(cont);
		return cont;
	}

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			container::list<mongo_host_info_type, Alloc>
		>::type
		var_hosts(const Alloc& alloc) const
	{
		typedef container::list<mongo_host_info_type, Alloc> cont_type;

		cont_type cont(alloc);
		this_type::var_hosts(cont);
		return cont;
	}

	// var_srv_hostname
	inline inner_string_view_type var_srv_hostname(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return
			base_type::str?
				inner_string_view_type(mongoc_uri_native_ex::s_mongoc_uri_get_srv_hostname(this))
				: inner_string_view_type();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	var_srv_hostname(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String var_srv_hostname(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	var_srv_hostname(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(var_srv_hostname)

public:
	// srv_service_name
	inline inner_string_view_type var_srv_service_name(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		return
			base_type::str?
				inner_string_view_type(mongoc_uri_native_ex::s_mongoc_uri_get_srv_service_name(this))
				: inner_string_view_type();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	var_srv_service_name(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String var_srv_service_name(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	var_srv_service_name(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(var_srv_service_name)

	// var_database
protected:
	inline const inner_string_type& pro_get_var_database(void) const
	{
		return _database;
	}

	inline void pro_set_var_database(const inner_string_type& str_db)
	{
		_database = str_db;
		base_type::database = _database.empty()? 0 : _database.data();
	}

public:
	inline inner_string_member_modify_helper_type var_database(void)
	{
		return
			inner_string_member_modify_helper_type(
				*this, _database,
				&this_type::pro_set_var_database,
				&this_type::pro_get_var_database);
	}

	// inline const inner_string_type& var_database(void) const;

	// template<typename ...Args> inline
	// const inner_string_type& var_database(const Args&& ...args) const;

	// template<typename String, typename ...Args> inline
	// String var_database(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_database(), var_database, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_database(), var_database, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_var_database(), var_database)

	// var_username
protected:
	inline const inner_string_type& pro_get_var_username(void) const
	{
		return _username;
	}

	inline void pro_set_var_username(const inner_string_type& str_uname)
	{
		_username = str_uname;
		base_type::username = _username.empty()? 0 : _username.data();
	}

public:
	// var_username
	inline inner_string_member_modify_helper_type var_username(void)
	{
		return
			inner_string_member_modify_helper_type(
				*this, _username,
				&this_type::pro_set_var_username,
				&this_type::pro_get_var_username);
	}

	// inline const inner_string_type& var_username(void) const;

	// template<typename ...Args> inline
	// const inner_string_type& var_username(const Args&& ...args) const;

	// template<typename String, typename ...Args> inline
	// String var_username(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_username(), var_username, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_username(), var_username, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_var_username(), var_username)

	// var_passwd
protected:
	inline const inner_string_type& pro_get_var_passwd(void) const
	{
		return _password;
	}

	inline void pro_set_var_passwd(const inner_string_type& str_pwd)
	{
		_password = str_pwd;
		base_type::password = _password.empty()? 0 : _password.data();
	}

public:
	inline inner_string_member_modify_helper_type var_passwd(void)
	{
		return
			inner_string_member_modify_helper_type(
				*this, _password,
				&this_type::pro_set_var_passwd,
				&this_type::pro_get_var_passwd);
	}

	// inline const inner_string_type& var_passwd(void) const;

	// template<typename ...Args> inline
	// const inner_string_type& var_passwd(const Args&& ...args) const;

	// template<typename String, typename ...Args> inline
	// String var_passwd(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_passwd(), var_passwd, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_passwd(), var_passwd, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_var_passwd(), var_passwd)

public:
	// var_options
	inline bson_type& var_options(void)
	{
		return reinterpret_cast<bson_type&>(base_type::options);
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			Bson&
		>::type
		var_options(void)
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<now_bson_type&>(base_type::options);
	}

	inline const bson_type& var_options(void) const
	{
		return reinterpret_cast<const bson_type&>(base_type::options);
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			const Bson&
		>::type
		var_options(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type&>(base_type::options);
	}

	// options operator
	//// has_option
	//template<typename OptString> inline
	//bool has_option(const OptString& key) const
	//{
	//	typedef OptString now_opt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

	//	now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(key));

	//	return
	//		!container::empty(mid_opt_str)
	//		&& mongoc_uri_native_ex::s_mongoc_uri_has_option(this, mid_opt_str.c_str());
	//}

	// has_option
	template<typename OptString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		has_option(const OptString& key) const
	{
		const bson_type& opt = this_type::var_options();
		return opt.find(key) != opt.end();
	}

	// static option_is
	// option_is_int32
	template<typename OptString> inline
	static
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		option_is_int32(const OptString& key)
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(key));

		return
			!container::empty(mid_opt_str)
			&& mongoc_uri_native_ex::s_mongoc_uri_option_is_int32(mid_opt_str.c_str());
	}

	// option_is_int64
	template<typename OptString> inline
	static
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		option_is_int64(const OptString& key)
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(key));

		return
			!container::empty(mid_opt_str)
			&& mongoc_uri_native_ex::s_mongoc_uri_option_is_int64(mid_opt_str.c_str());
	}

	// option_is_bool
	template<typename OptString> inline
	static
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		option_is_bool(const OptString& key)
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(key));

		return
			!container::empty(mid_opt_str)
			&& mongoc_uri_native_ex::s_mongoc_uri_option_is_bool(mid_opt_str.c_str());
	}

	// option_is_utf8
	template<typename OptString> inline
	static
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		option_is_utf8(const OptString& key)
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(key));

		return
			!container::empty(mid_opt_str)
			&& mongoc_uri_native_ex::s_mongoc_uri_option_is_utf8(mid_opt_str.c_str());
	}

	// option_as_type
	// get_option_as_int32
	template<typename OptString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, int32_t>::type
		get_option_as_int32(const OptString& option, int32_t fallback) const
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));

		return
			!container::empty(mid_opt_str)?
				mongoc_uri_native_ex::s_mongoc_uri_get_option_as_int32(this, mid_opt_str.c_str(), fallback)
				: fallback;
	}

	// set_option_as_int32
	template<typename OptString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		set_option_as_int32(const OptString& option, int32_t value)
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));

		return
			!container::empty(mid_opt_str)
			&& mongoc_uri_native_ex::s_mongoc_uri_set_option_as_int32(this, mid_opt_str.c_str(), value);
	}

	// get_option_as_int64
	template<typename OptString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, int64_t>::type
		get_option_as_int64(const OptString& option, int64_t fallback) const
	{
		typedef OptString now_opt_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));

		return
			!container::empty(mid_opt_str)?
				mongoc_uri_native_ex::s_mongoc_uri_get_option_as_int64(this, mid_opt_str.c_str(), fallback)
				: fallback;
	}

	// set_option_as_int64
	template<typename OptString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		set_option_as_int64(const OptString& option, int64_t value)
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));

		return
			!container::empty(mid_opt_str)
			&& mongoc_uri_native_ex::s_mongoc_uri_set_option_as_int64(this, mid_opt_str.c_str(), value);
	}

	// get_option_as_bool
	template<typename OptString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		get_option_as_bool(const OptString& option, bool fallback) const
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));

		return
			!container::empty(mid_opt_str)?
				mongoc_uri_native_ex::s_mongoc_uri_get_option_as_bool(this, mid_opt_str.c_str(), fallback)
				: fallback;
	}

	// set_option_as_bool
	template<typename OptString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		set_option_as_bool(const OptString& option, bool value)
	{
		typedef OptString now_opt_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));

		return
			!container::empty(mid_opt_str)
			&& mongoc_uri_native_ex::s_mongoc_uri_set_option_as_bool(this, mid_opt_str.c_str(), value);
	}

	// get_option_as_utf8
	// return inner_string_view_type, fallback const char*
	template<typename OptString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, inner_string_view_type>::type
		get_option_as_utf8(const OptString& option, const inner_string_view_type& fallback,
							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		typedef OptString now_opt_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);

		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));

		const char* ret_str =
			!container::empty(mid_opt_str)?
				mongoc_uri_native_ex::s_mongoc_uri_get_option_as_utf8(this, mid_opt_str.c_str(), 0)
				: 0;

		return ret_str? inner_string_view_type(ret_str) : fallback;
	}

	template<typename OptString, typename FallBackString, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<OptString>,
				container_ex::is_allocator<Alloc>
			>,
			typename detail::make_utf8_string_t_from_allocator<Alloc>::type
		>::type
		get_option_as_utf8(const OptString& option, const FallBackString& fallback, const Alloc& alloc,
							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		typedef typename detail::make_utf8_string_t_from_allocator<Alloc>::type now_ret_string_type;
		typedef OptString now_opt_string_type;
		typedef FallBackString now_fallback_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_fallback_string_type, now_fallback_utf8_string_type);

		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));
		now_fallback_utf8_string_type mid_fallback_str(charset::string_charset_helper_data(fallback));

		return
			this_type::get_option_as_utf8(
				mid_opt_str,
				inner_string_view_type(mid_fallback_str),
				ret_charset_name)
			.template str<now_ret_string_type>(alloc, ret_charset_name);
	}

	template<typename RetString, typename OptString, typename FallBackString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, RetString>::type
		get_option_as_utf8(const OptString& option, const FallBackString& fallback,
							const string& ret_charset_name = charset::charset_name_t<RetString>()) const
	{
		typedef RetString now_ret_string_type;
		typedef OptString now_opt_string_type;
		typedef FallBackString now_fallback_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_fallback_string_type, now_fallback_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));
		now_fallback_utf8_string_type mid_fallback_str(charset::string_charset_helper_data(fallback));

		return
			this_type::get_option_as_utf8(
				mid_opt_str,
				inner_string_view_type(mid_fallback_str),
				YGGR_STR_UTF8_STRING_CHARSET_NAME()).template str<now_ret_string_type>(ret_charset_name);
	}

	template<typename RetString, typename OptString, typename FallBackString, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<OptString>,
				container_ex::is_allocator<Alloc>
			>,
			RetString
		>::type
		get_option_as_utf8(const OptString& option, const FallBackString& fallback, const Alloc& alloc,
							const string& ret_charset_name = charset::charset_name_t<RetString>()) const
	{
		typedef RetString now_ret_string_type;
		typedef OptString now_opt_string_type;
		typedef FallBackString now_fallback_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_fallback_string_type, now_fallback_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));
		now_fallback_utf8_string_type mid_fallback_str(charset::string_charset_helper_data(fallback));

		return
			this_type::get_option_as_utf8(
				mid_opt_str,
				inner_string_view_type(mid_fallback_str),
				YGGR_STR_UTF8_STRING_CHARSET_NAME()).template str<now_ret_string_type>(alloc, ret_charset_name);
	}

	// set_option_as_utf8
	template<typename OptString, typename ValString> inline
	typename boost::enable_if<charset::utf8_string_constructable<OptString>, bool>::type
		set_option_as_utf8(const OptString& option, const ValString& val)
	{
		typedef OptString now_opt_string_type;
		typedef ValString now_val_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_opt_string_type, now_opt_utf8_string_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_val_string_type, now_val_utf8_string_type);

		now_opt_utf8_string_type mid_opt_str(charset::string_charset_helper_data(option));
		now_val_utf8_string_type mid_val_str(charset::string_charset_helper_data(val));

		return
			!container::empty(mid_opt_str)
			&& mongoc_uri_native_ex::s_mongoc_uri_set_option_as_utf8(
					this, mid_opt_str.c_str(), mid_val_str.c_str());
	}

	// var_credentials
	inline bson_type& var_credentials(void)
	{
		return reinterpret_cast<bson_type&>(base_type::credentials);
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			Bson&
		>::type
		var_credentials(void)
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<now_bson_type&>(base_type::credentials);
	}

	inline const bson_type& var_credentials(void) const
	{
		return reinterpret_cast<const bson_type&>(base_type::credentials);
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			const Bson&
		>::type
		var_credentials(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type&>(base_type::credentials);
	}

protected:
	inline inner_string_view_type pro_get_auth_mechanism(void) const
	{
		const char* ptr = base_type::str? mongoc_uri_native_ex::s_mongoc_uri_get_auth_mechanism(this) : 0;
		return inner_string_view_type(ptr);
	}

	inline void pro_set_auth_mechanism(const inner_string_type& val)
	{
		if(!container::empty(val))
		{
			mongoc_uri_native_ex::s_mongoc_uri_set_auth_mechanism(this, val.c_str());
		}
	}

public:
	inline non_member_string_view_modify_helper_type auth_mechanism(void)
	{
		mplex::null_type null_obj;
		return
			non_member_string_view_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_auth_mechanism,
				&this_type::pro_get_auth_mechanism);
	}

	inline inner_string_view_type auth_mechanism(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		return this_type::pro_get_auth_mechanism();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	auth_mechanism(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String auth_mechanism(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	auth_mechanism(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(auth_mechanism)

	// auth_source
protected:
	inline inner_string_view_type pro_get_auth_source(void) const
	{
		const char* ptr = base_type::str? mongoc_uri_native_ex::s_mongoc_uri_get_auth_source(this) : 0;
		return inner_string_view_type(ptr);
	}

	inline void pro_set_auth_source(const inner_string_type& val)
	{
		if(!container::empty(val))
		{
			mongoc_uri_native_ex::s_mongoc_uri_set_auth_source(this, val.c_str());
		}
	}

public:
	public:
	inline non_member_string_view_modify_helper_type auth_source(void)
	{
		mplex::null_type null_obj;
		return
			non_member_string_view_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_auth_source,
				&this_type::pro_get_auth_source);
	}

	inline inner_string_view_type auth_source(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		return this_type::pro_get_auth_source();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	auth_source(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String auth_source(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	auth_source(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(auth_source)

public:
	// var_compressors
	inline bson_type& var_compressors(void)
	{
		return reinterpret_cast<bson_type&>(base_type::compressors);
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			Bson&
		>::type
		var_compressors(void)
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<now_bson_type&>(base_type::compressors);
	}

	inline const bson_type& var_compressors(void) const
	{
		return reinterpret_cast<const bson_type&>(base_type::compressors);
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			const Bson&
		>::type
		var_compressors(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type&>(base_type::compressors);
	}

protected:
	inline const bson_type& pro_get_compressors(void) const
	{
		return this_type::var_compressors();
	}

	inline void pro_set_compressors(const inner_string_type& val)
	{
		if(!container::empty(val))
		{
			mongoc_uri_native_ex::s_mongoc_uri_set_compressors(this, val.c_str());
		}
	}

public:
	inline member_compressors_modify_helper_type modify_compressors(void)
	{
		return
			member_compressors_modify_helper_type(
				*this,
				base_type::compressors,
				&this_type::pro_set_compressors,
				&this_type::pro_get_compressors);
	}

	inline const bson_type& get_compressors(void) const
	{
		return this_type::var_compressors();
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			const Bson&
		>::type
		get_compressors(void) const
	{
		typedef Bson now_bson_type;
		return this_type::var_compressors<now_bson_type>();
	}

	template<typename String> inline
	bool set_compressors(const String& val)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type mid_str(charset::string_charset_helper_data(val));

		return
			(!container::empty(mid_str))
			&& mongoc_uri_native_ex::s_mongoc_uri_set_compressors(this, mid_str.c_str());
	}

public:
	// get_mechanism_properties
	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			Bson&
		>::type
		get_mechanism_properties(Bson& bs) const
	{
		if(!mongoc_uri_native_ex::s_mongoc_uri_get_mechanism_properties(this, &bs))
		{
			reinterpret_cast<bson_type&>(bs).clear();
		}

		return bs;
	}

	inline bson_type get_mechanism_properties(void) const
	{
		bson_type bs;
		this_type::get_mechanism_properties(bs);
		return bs;
	}


	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			bool
		>::type
		set_mechanism_properties(const Bson& val)
	{
		return mongoc_uri_native_ex::s_mongoc_uri_set_mechanism_properties(this, &val);
	}

public:
	// var_read_concern
	inline read_concern_type& var_read_concern(void)
	{
		return _read_concern;
	}

	inline const read_concern_type& var_read_concern(void) const
	{
		return _read_concern;
	}

public:
	// var_read_prefs
	inline read_prefs_type& var_read_prefs(void)
	{
		return _read_prefs;
	}

	inline const read_prefs_type& var_read_prefs(void) const
	{
		return _read_prefs;
	}

	template<typename BsonOrReadPrefs> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, BsonOrReadPrefs>,
			BsonOrReadPrefs&
		>::type
		var_read_prefs(void)
	{
		typedef BsonOrReadPrefs now_bson_type;
		return _read_prefs.var_tags<now_bson_type>();
	}

	template<typename BsonOrReadPrefs> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, BsonOrReadPrefs>,
			const BsonOrReadPrefs&
		>::type
		var_read_prefs(void) const
	{
		typedef BsonOrReadPrefs now_bson_type;
		return _read_prefs.var_tags<now_bson_type>();
	}

	template<typename BsonOrReadPrefs> inline
	typename
		boost::disable_if
		<
			boost::is_base_of<bson_t, BsonOrReadPrefs>,
			BsonOrReadPrefs&
		>::type
		var_read_prefs(void)
	{
		return _read_prefs;
	}

	template<typename BsonOrReadPrefs> inline
	typename
		boost::disable_if
		<
			boost::is_base_of<bson_t, BsonOrReadPrefs>,
			const BsonOrReadPrefs&
		>::type
		var_read_prefs(void) const
	{
		return _read_prefs;
	}

public:
	// var_write_concern
	inline write_concern_type& var_write_concern(void)
	{
		return _write_concern;
	}

	inline const write_concern_type& var_write_concern(void) const
	{
		return _write_concern;
	}


public:
	// get_replica_set
	inline inner_string_view_type get_replica_set(const string& ret_charset_name
													= YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		return
			base_type::str?
				inner_string_view_type(mongoc_uri_native_ex::s_mongoc_uri_get_replica_set(this))
				: inner_string_view_type();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc>
	//	get_replica_set(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String get_replica_set(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	get_replica_set(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(get_replica_set)

public:
	inline bool get_tls(void) const
	{
		return (base_type::str) && (mongoc_uri_native_ex::s_mongoc_uri_get_tls(this));
	}

	inline bool has_tls(void) const
	{
		return this_type::get_tls();
	}

	//inline bool get_ssl(void) const; // discard for mongodbc
	//inline bool has_ssl(void) const; // discard for mongodbc

protected:
	inline const inner_string_type& pro_get_var_uri_string(void) const
	{
		return _str;
	}

	inline void pro_set_var_uri_string(const inner_string_type& uri_str)
	{
		this_type::clear();

		base_type tmp_base = {0};

		if(mongoc_uri_native_ex::s_mongoc_uri_init(&tmp_base, uri_str.c_str(), 0))
		{
			this_type::pro_copy(tmp_base);
		}

		mongoc_uri_native_ex::s_mongoc_uri_clear(&tmp_base);
	}

public:
	inline inner_string_member_modify_helper_type var_uri_string(void)
	{
		return
			inner_string_member_modify_helper_type(
				*this,
				_str,
				&this_type::pro_set_var_uri_string,
				&this_type::pro_get_var_uri_string);
	}

	// inline const inner_string_type& var_uri_string(void) const;

	// template<typename ...Args> inline
	// const inner_string_type& var_uri_string(const Args&& ...args) const;

	// template<typename String, typename ...Args> inline
	// String var_uri_string(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, _str, var_uri_string, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, _str, var_uri_string, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, _str, var_uri_string)


public:
	inline void clear(void)
	{
		this_type tmp;
		tmp.swap(*this);
	}

	/*
		don't implements it.
		because:
			1, can using charset::escape_codes_converter.hpp
			2, mongoc_uri_parse called unescape
	*/
//public:
	//static mongoc_uri_unescape


protected:
	this_type& pro_copy(const base_type& right);
	this_type& pro_copy(const this_type& right);

	this_type& pro_move(this_type& right);

	void pro_swap(this_type& right);

private:
	void prv_init_default(void);
	bool prv_init(base_type& right);
	void prv_destroy(void);

private:
	inner_string_type _str;
	inner_string_type _username;
	inner_string_type _password;
	inner_string_type _database;
	read_prefs_type _read_prefs;
	read_concern_type _read_concern;
	write_concern_type _write_concern;
};

// io
namespace detail
{

template<>
struct bson_operator_outter<c_mongo_uri>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_mongo_uri& val,
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"uri_string\" : \"" << val.var_uri_string() << "\" }";
		return os;
	}
};

} // namespace detial

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_uri& val)
{
	typedef detail::bson_operator_outter<c_mongo_uri> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_mongo_uri) val)
//{
//	const c_mongo_uri& right_cref = val;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_mongo_uri& l, c_mongo_uri& r)
{
	l.swap(r);
}

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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_URI_HPP__
