// c_mongo_ssl_opt.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SSL_OPT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SSL_OPT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/type_traits/native_t.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_mongo_internal_tls_opts.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_ssl_opt_t {
//   const char *pem_file;
//   const char *pem_pwd;
//   const char *ca_file;
//   const char *ca_dir;
//   const char *crl_file;
//   bool weak_cert_validation;
//   bool allow_invalid_hostname;
//   void *internal;
//   void *padding[6];
//};

struct yggr_mongoc_ssl_opt_t
{
	const char *pem_file;
	const char *pem_pwd;
	const char *ca_file;
	const char *ca_dir;
	const char *crl_file;
	bool weak_cert_validation;
	bool allow_invalid_hostname;
	void *internal;
	void *padding[6];
};

class c_mongo_ssl_opt
	: protected yggr_mongoc_ssl_opt_t
{
public:
	typedef yggr_mongoc_ssl_opt_t base_type;
	typedef mongoc_ssl_opt_t org_type;

	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

#ifdef YGGR_AT_WINDOWS
	typedef yggr::string path_string_type;
#else
	typedef inner_string_type path_string_type;
#endif // YGGR_AT_WINDOWS
	typedef yggr::utf8_string pwd_string_type;

	typedef c_mongo_internal_tls_opts internal_tls_opts_type;

private:
	YGGR_STATIC_CONSTANT(u32, E_compare_size = sizeof(const char*) * 5);

private:
	typedef c_mongo_ssl_opt this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	typedef
		utility::member_var_modify_helper
		<
			this_type,
			path_string_type,
			path_string_type,
			const path_string_type&
		> path_string_member_modify_helper_type;

	typedef
		utility::member_var_modify_helper
		<
			this_type,
			pwd_string_type,
			pwd_string_type,
			const pwd_string_type&
		> pwd_string_member_modify_helper_type;

public:
	c_mongo_ssl_opt(void);

	template<typename String1, typename String2,
				typename String3, typename String4,
				typename String5 >
	c_mongo_ssl_opt(const String1& pem_file_name, const String2& pem_passwd,
					const String3& ca_file_name, const String4& ca_dir_name,
					const String5& crl_file_name,
					bool bweak_cert_validation = false,
					bool ballow_invalid_hostname = false,
					const internal_tls_opts_type& internal_tls_opts = internal_tls_opts_type(false, false),
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
									charset::utf8_string_constructable<String5>
								>
							>,
							sfinae_type
						>::type sfinae = 0)
	{
		this_type::pro_init(
			pem_file_name, pem_passwd, ca_file_name, ca_dir_name, crl_file_name,
			bweak_cert_validation, ballow_invalid_hostname, internal_tls_opts);
	}

	c_mongo_ssl_opt(const org_type* right);
	c_mongo_ssl_opt(const org_type& right);

	c_mongo_ssl_opt(BOOST_RV_REF(this_type) right)
		: _pem_file(boost::move(right._pem_file)),
			_pem_pwd(boost::move(right._pem_pwd)),
			_ca_file(boost::move(right._ca_file)),
			_ca_dir(boost::move(right._ca_dir)),
			_crl_file(boost::move(right._crl_file)),
			_internal_tls_opts(boost::move(right._internal_tls_opts))
	{
		this_type::pro_init();

		base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
		base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
		base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
		base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
		base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();
		base_type::weak_cert_validation = right.weak_cert_validation;
		base_type::allow_invalid_hostname = right.allow_invalid_hostname;
		base_type::internal = reinterpret_cast<void*>(boost::addressof(_internal_tls_opts));
	}

	c_mongo_ssl_opt(const this_type& right);
	~c_mongo_ssl_opt(void);

public:
	this_type& operator=(const org_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_pem_file, boost::move(right._pem_file));
		copy_or_move_or_swap(_pem_pwd, boost::move(right._pem_pwd));
		copy_or_move_or_swap(_ca_file, boost::move(right._ca_file));
		copy_or_move_or_swap(_ca_dir, boost::move(right._ca_dir));
		copy_or_move_or_swap(_crl_file, boost::move(right._crl_file));
		copy_or_move_or_swap(_internal_tls_opts, boost::move(right._internal_tls_opts));

		base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
		base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
		base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
		base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
		base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();
		base_type::weak_cert_validation = right.weak_cert_validation;
		base_type::allow_invalid_hostname = right.allow_invalid_hostname;

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
	inline bool compare_eq(const org_type& right) const
	{
#ifdef YGGR_AT_WINDOWS
		return
			(static_cast<const base_type*>(this) == reinterpret_cast<const base_type*>(boost::addressof(right)))
			|| ( ((base_type::pem_file == right.pem_file)
					|| (right.pem_file && 0 == _pem_file.compare(right.pem_file)))
				&& ((base_type::pem_pwd == right.pem_pwd)
					|| (right.pem_pwd && 0 == _pem_pwd.compare(right.pem_pwd)))
				&& ((base_type::ca_file == right.ca_file)
					|| (right.ca_file && 0 == _ca_file.compare(right.ca_file)))
				&& ((base_type::ca_dir == right.ca_dir)
					|| (right.ca_dir && 0 == _ca_dir.compare(right.ca_dir)))
				&& ((base_type::crl_file == right.crl_file)
					|| (right.crl_file && 0 == _crl_file.compare(right.crl_file)))
				&& base_type::weak_cert_validation == right.weak_cert_validation
				&& base_type::allow_invalid_hostname == right.allow_invalid_hostname
				&& (right.internal
					&& _internal_tls_opts == *(reinterpret_cast<const internal_tls_opts_type*>(right.internal))));
#else
		return
			(static_cast<const base_type*>(this) == reinterpret_cast<const base_type*>(boost::addressof(right)))
			|| ( ((base_type::pem_file == right.pem_file)
					|| (right.pem_file && _pem_file.compare_eq(right.pem_file, YGGR_STR_UTF8_STRING_CHARSET_NAME())))
				&& ((base_type::pem_pwd == right.pem_pwd)
					|| (right.pem_pwd && _pem_pwd.compare_eq(right.pem_pwd, YGGR_STR_UTF8_STRING_CHARSET_NAME())))
				&& ((base_type::ca_file == right.ca_file)
					|| (right.ca_file && _ca_file.compare_eq(right.ca_file, YGGR_STR_UTF8_STRING_CHARSET_NAME())))
				&& ((base_type::ca_dir == right.ca_dir)
					|| (right.ca_dir && _ca_dir.compare_eq(right.ca_dir, YGGR_STR_UTF8_STRING_CHARSET_NAME())))
				&& ((base_type::crl_file == right.crl_file)
					|| (right.crl_file && _crl_file.compare_eq(right.crl_file, YGGR_STR_UTF8_STRING_CHARSET_NAME())))
				&& base_type::weak_cert_validation == right.weak_cert_validation
				&& base_type::allow_invalid_hostname == right.allow_invalid_hostname
				&& (right.internal
					&& _internal_tls_opts == *(reinterpret_cast<const internal_tls_opts_type*>(right.internal))));
#endif // YGGR_AT_WINDOWS
	}

	inline bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (_pem_file == right._pem_file
				&& _pem_pwd == right._pem_pwd
				&& _ca_file == right._ca_file
				&& _ca_dir == right._ca_dir
				&& _crl_file == right._crl_file
				&& base_type::weak_cert_validation == right.weak_cert_validation
				&& base_type::allow_invalid_hostname == right.allow_invalid_hostname
				&& _internal_tls_opts == right._internal_tls_opts);
	}

public:
	inline bool empty(void) const
	{
		static base_type tmp = {0};
		const base_type& base = *this;
		return (0 == memcmp(boost::addressof(tmp), boost::addressof(base), this_type::E_compare_size));
	}

	// string members

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

#	define YGGR_TMP_PP_STRING_MEMBER_GET_SAME( __type__, __val__, __foo__, _ ) \
	template<typename String, typename ...Args> inline \
	typename boost::enable_if<boost::is_same<String, __type__>, const String&>::type \
		__foo__(BOOST_FWD_REF(Args)... args) const { return __val__; }

#else

#	define YGGR_TMP_PP_STRING_MEMBER_GET_SAME( __type__, __val__, __foo__, __n__ ) \
		template< typename String \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if<boost::is_same<String, __type__>, const String&>::type \
			__foo__( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return __val__; }

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


#	define YGGR_TMP_PP_STRING_MEMBER_GET_DIFF( __type__, __val__, __foo__) \
		template<typename String> inline \
		typename boost::disable_if<boost::is_same<String, __type__>, String>::type \
			__foo__(const string& charset_name \
						= charset::charset_name_t<String>()) const { \
			typedef String now_string_type; \
			return charset::string_converter::s_conv<now_string_type>(__val__, charset_name); } \
		\
		template<typename String, typename Alloc> inline \
		typename \
			boost::enable_if< \
				boost::mpl::and_< \
					boost::mpl::not_< boost::is_same<String, __type__> >, \
					container_ex::is_allocator<Alloc> >, \
				String>::type \
			__foo__(const Alloc& alloc, \
						const string& charset_name \
							= charset::charset_name_t<String>()) const { \
			typedef String now_string_type; \
			return charset::string_converter::s_conv<now_string_type>(__val__, alloc, charset_name); }


	// var_pem_file
protected:
	inline const path_string_type& pro_get_pem_file(void) const
	{
		return _pem_file;
	}

	inline void pro_set_pem_file(const path_string_type& val)
	{
		_pem_file = val;
		base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
	}

public:
	inline path_string_member_modify_helper_type var_pem_file(void)
	{
		return
			path_string_member_modify_helper_type(
				*this, _pem_file,
				&this_type::pro_set_pem_file,
				&this_type::pro_get_pem_file );
	}

	inline const path_string_type& var_pem_file(void) const
	{
		return this_type::pro_get_pem_file();
	}

	//string_type var_pem_file(void) const
	//string_type var_pem_file(alloc) const
	//string_type var_pem_file(charset_name) const
	//string_type var_pem_file(allocator, charset_name) const

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_SAME(path_string_type, _pem_file, var_pem_file, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_PP_STRING_MEMBER_GET_SAME(path_string_type, _pem_file, var_pem_file, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_DIFF(path_string_type, _pem_file, var_pem_file)

	// var_pem_pwd
protected:
	inline const pwd_string_type& pro_get_pem_pwd(void) const
	{
		return _pem_pwd;
	}

	inline void pro_set_pem_pwd(const pwd_string_type& val)
	{
		_pem_pwd = val;
		base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
	}

public:
	inline pwd_string_member_modify_helper_type var_pem_pwd(void)
	{
		return
			pwd_string_member_modify_helper_type(
				*this, _pem_pwd,
				&this_type::pro_set_pem_pwd,
				&this_type::pro_get_pem_pwd);
	}

	inline const pwd_string_type& var_pem_pwd(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_get_pem_pwd();
	}

	//string_type var_pem_pwd(void) const
	//string_type var_pem_pwd(allocator) const
	//string_type var_pem_pwd(charset_name) const
	//string_type var_pem_pwd(allocator, charset_name) const

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_SAME(pwd_string_type, _pem_pwd, var_pem_pwd, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_PP_STRING_MEMBER_GET_SAME(pwd_string_type, _pem_pwd, var_pem_pwd, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_DIFF(pwd_string_type, _pem_pwd, var_pem_pwd)

	// var_ca_file
protected:
	inline const path_string_type& pro_get_ca_file(void) const
	{
		return _ca_file;
	}

	inline void pro_set_ca_file(const path_string_type& val)
	{
		_ca_file = val;
		base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
	}

public:
	inline path_string_member_modify_helper_type var_ca_file(void)
	{
		return
			path_string_member_modify_helper_type(
				*this, _ca_file,
				&this_type::pro_set_ca_file,
				&this_type::pro_get_ca_file);
	}

	inline const path_string_type& var_ca_file(void) const
	{
		return this_type::pro_get_ca_file();
	}

	//string_type var_ca_file(void) const
	//string_type var_ca_file(allocator) const
	//string_type var_ca_file(charset_name) const
	//string_type var_ca_file(allocator, charset_name) const

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_SAME(path_string_type, _ca_file, var_ca_file, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_PP_STRING_MEMBER_GET_SAME(path_string_type, _ca_file, var_ca_file, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_DIFF(path_string_type, _ca_file, var_ca_file)

	// var_ca_dir
protected:
	inline const path_string_type& pro_get_ca_dir(void) const
	{
		return _ca_dir;
	}

	inline void pro_set_ca_dir(const path_string_type& val)
	{
		_ca_dir = val;
		base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
	}

public:
	inline path_string_member_modify_helper_type var_ca_dir(void)
	{
		return
			path_string_member_modify_helper_type(
				*this, _ca_dir,
				&this_type::pro_set_ca_dir,
				&this_type::pro_get_ca_dir);
	}

	inline const path_string_type& var_ca_dir(void) const
	{
		return this_type::pro_get_ca_dir();
	}

	//string_type var_ca_dir(void) const
	//string_type var_ca_dir(allocator) const
	//string_type var_ca_dir(charset_name) const
	//string_type var_ca_dir(allocator, charset_name) const

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_SAME(path_string_type, _ca_dir, var_ca_dir, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_PP_STRING_MEMBER_GET_SAME(path_string_type, _ca_dir, var_ca_dir, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_DIFF(path_string_type, _ca_dir, var_ca_dir)

	// var_crl_file
protected:
	inline const path_string_type& pro_get_crl_file(void) const
	{
		return _crl_file;
	}

	inline void pro_set_crl_file(const path_string_type& val)
	{
		_crl_file = val;
		base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();
	}

public:
	inline path_string_member_modify_helper_type var_crl_file(void)
	{
		return
			path_string_member_modify_helper_type(
				*this, _pem_file,
				&this_type::pro_set_crl_file,
				&this_type::pro_get_crl_file);
	}

	inline const path_string_type& var_crl_file(void) const
	{
		return this_type::pro_get_crl_file();
	}

	//string_type var_crl_file(void) const
	//string_type var_crl_file(allocator) const
	//string_type var_crl_file(charset_name) const
	//string_type var_crl_file(allocator, charset_name) const

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_SAME(path_string_type, _crl_file, var_crl_file, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_PP_STRING_MEMBER_GET_SAME(path_string_type, _crl_file, var_crl_file, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_TMP_PP_STRING_MEMBER_GET_DIFF(path_string_type, _crl_file, var_crl_file)

#undef YGGR_TMP_PP_STRING_MEMBER_GET_SAME
#undef YGGR_TMP_PP_STRING_MEMBER_GET_DIFF

public:
	inline bool& var_weak_cert_validation(void)
	{
		return base_type::weak_cert_validation;
	}

	inline bool var_weak_cert_validation(void) const
	{
		return base_type::weak_cert_validation;
	}

	inline bool& var_allow_invalid_hostname(void)
	{
		return base_type::allow_invalid_hostname;
	}

	inline bool var_allow_invalid_hostname(void) const
	{
		return base_type::allow_invalid_hostname;
	}

	inline internal_tls_opts_type& var_internal_tls_opts(void)
	{
		return _internal_tls_opts;
	}

	inline const internal_tls_opts_type& var_internal_tls_opts(void) const
	{
		return _internal_tls_opts;
	}

	inline bool& var_tls_disable_certificate_revocation_check(void)
	{
		return _internal_tls_opts.tls_disable_certificate_revocation_check;
	}

	inline bool var_tls_disable_certificate_revocation_check(void) const
	{
		return _internal_tls_opts.tls_disable_certificate_revocation_check;
	}

	inline bool& var_tls_disable_ocsp_endpoint_check(void)
	{
		return _internal_tls_opts.tls_disable_ocsp_endpoint_check;
	}

	inline bool var_tls_disable_ocsp_endpoint_check(void) const
	{
		return _internal_tls_opts.tls_disable_ocsp_endpoint_check;
	}

public:
	inline static const this_type& s_get_default(void)
	{
		static const this_type def_ssl_opt(mongoc_ssl_opt_get_default());
		return def_ssl_opt;
	}

protected:
	void pro_init(void);

	template<typename String1, typename String2,
				typename String3, typename String4,
				typename String5 > inline
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
					charset::utf8_string_constructable<String5>
				>
			>,
			void
		>::type
		pro_init(const String1& pem_file_name, const String2& pem_passwd,
					const String3& ca_file_name, const String4& ca_dir_name,
					const String5& crl_file_name,
					bool bweak_cert_validation,
					bool ballow_invalid_hostname,
					const internal_tls_opts_type& internal_tls_opts)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;
		typedef typename native_t<String4>::type string4_type;
		typedef typename native_t<String5>::type string5_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string4_type, utf8_string4_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string5_type, utf8_string5_type);

		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));

		utf8_string1_type utf8_pem_file((charset::string_charset_helper_data(pem_file_name)));
		utf8_string2_type utf8_pem_pwd((charset::string_charset_helper_data(pem_passwd)));
		utf8_string3_type utf8_ca_file((charset::string_charset_helper_data(ca_file_name)));
		utf8_string4_type utf8_ca_dir((charset::string_charset_helper_data(ca_dir_name)));
		utf8_string5_type utf8_crl_file((charset::string_charset_helper_data(crl_file_name)));

		_pem_file = utf8_pem_file.template str<path_string_type>();
		_pem_pwd = utf8_pem_pwd.template str<pwd_string_type>();
		_ca_file = utf8_ca_file.template str<path_string_type>();
		_ca_dir = utf8_ca_dir.template str<path_string_type>();
		_crl_file = utf8_crl_file.template str<path_string_type>();
		_internal_tls_opts = internal_tls_opts;

		base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
		base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
		base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
		base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
		base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();
		base_type::weak_cert_validation = bweak_cert_validation;
		base_type::allow_invalid_hostname = ballow_invalid_hostname;
		base_type::internal = reinterpret_cast<void*>(boost::addressof(_internal_tls_opts));
	}

	this_type& pro_move(this_type& right);

	this_type& pro_copy(const base_type& right);
	this_type& pro_copy(const this_type& right);

	void pro_swap(this_type& right);

private:
	path_string_type _pem_file;
	pwd_string_type _pem_pwd;
	path_string_type _ca_file;
	path_string_type _ca_dir;
	path_string_type _crl_file;
	internal_tls_opts_type _internal_tls_opts;
};

// non-member function

// ==
inline bool operator==(const c_mongo_ssl_opt::org_type& l, const c_mongo_ssl_opt& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const c_mongo_ssl_opt& l, const c_mongo_ssl_opt::org_type& r)
{
	return l.compare_eq(r);
}

inline bool operator==(const c_mongo_ssl_opt& l, const c_mongo_ssl_opt& r)
{
	return l.compare_eq(r);
}

// !=
inline bool operator!=(const c_mongo_ssl_opt::org_type& l, const c_mongo_ssl_opt& r)
{
	return !r.compare_eq(l);
}

inline bool operator!=(const c_mongo_ssl_opt& l, const c_mongo_ssl_opt::org_type& r)
{
	return !l.compare_eq(r);
}
inline bool operator!=(const c_mongo_ssl_opt& l, const c_mongo_ssl_opt& r)
{
	return !l.compare_eq(r);
}

namespace detail
{

template<>
struct bson_operator_outter<mongoc_ssl_opt_t>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const mongoc_ssl_opt_t& val,
					bson_json_mode_def::mode_type mode) const
	{
		typedef bson_operator_outter<c_mongo_internal_tls_opts> internal_tls_outter_type;

		os << "{ "
			<< "\"pem_file\" : \"" << (val.pem_file? val.pem_file : "") << "\", "
			<< "\"pem_pwd\" : \"" << (val.pem_pwd? val.pem_pwd : "") << "\", "
			<< "\"ca_file\" : \"" << (val.ca_file? val.ca_file : "") << "\", "
			<< "\"ca_dir\" : \"" << (val.ca_dir? val.ca_dir : "") << "\", "
			<< "\"crl_file\" : \"" << (val.crl_file? val.crl_file : "") << "\", "
			<< "\"weak_cert_validation\" : " << (val.weak_cert_validation? "true" : "false") << ", "
			<< "\"allow_invalid_hostname\" : " << (val.allow_invalid_hostname? "true" : "false") << ", "
			<< "\"internal\" : ";

		internal_tls_outter_type inner_tls_outter;
		inner_tls_outter(
			os,
			(val.internal?
				*reinterpret_cast<const c_mongo_internal_tls_opts*>(val.internal)
				: c_mongo_internal_tls_opts()),
			mode);

		os << " }";
		return os;
	}
};

template<>
struct bson_operator_outter<c_mongo_ssl_opt>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_mongo_ssl_opt& val,
					bson_json_mode_def::mode_type mode) const
	{
		typedef bson_operator_outter<c_mongo_internal_tls_opts> internal_tls_outter_type;

		os << "{ "
			<< "\"pem_file\" : \"" << val.var_pem_file() << "\", "
			<< "\"pem_pwd\" : \"" << val.var_pem_pwd() << "\", "
			<< "\"ca_file\" : \"" << val.var_ca_file() << "\", "
			<< "\"ca_dir\" : \"" << val.var_ca_dir() << "\", "
			<< "\"crl_file\" : \"" << val.var_crl_file() << "\", "
			<< "\"weak_cert_validation\" : " << (val.var_weak_cert_validation()? "true" : "false") << ", "
			<< "\"allow_invalid_hostname\" : " << (val.var_allow_invalid_hostname()? "true" : "false") << ", "
			<< "\"internal\" : ";

		internal_tls_outter_type inner_tls_outter;
		inner_tls_outter(os, val.var_internal_tls_opts(), mode);

		os << " }";
		return os;
	}
};

} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_ssl_opt& val)
{
	typedef detail::bson_operator_outter<c_mongo_ssl_opt> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

} // namespace nsql_database_system
} // namespace yggr

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const mongoc_ssl_opt_t& val)
{
	typedef yggr::nsql_database_system::detail::bson_operator_outter<mongoc_ssl_opt_t> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_ssl_opt)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SSL_OPT_HPP__

