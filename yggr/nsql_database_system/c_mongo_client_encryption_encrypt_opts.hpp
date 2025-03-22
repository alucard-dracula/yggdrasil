// c_mongo_client_encryption_encrypt_opts.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_ENCRYPT_OPTS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_ENCRYPT_OPTS_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/move/move.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/mongoc_client_encryption_encrypt_opts_native_ex.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_encrypt_range_opts.hpp>
#include <yggr/nsql_database_system/detail/setmark_value.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_client_encryption_encrypt_opts_t {
//   bson_value_t keyid;
//   char *algorithm;
//   char *keyaltname;
//   struct {
//      int64_t value;
//      bool set;
//   } contention_factor;
//   char *query_type;
//   mongoc_client_encryption_encrypt_range_opts_t *range_opts;
//};

class c_mongo_client_encryption_encrypt_opts
	: protected yggr_mongoc_client_encryption_encrypt_opts 
{
public:
	typedef yggr_mongoc_client_encryption_encrypt_opts base_type;
	typedef mongoc_client_encryption_encrypt_opts_t org_type;

	typedef c_bson_value bson_value_type;
	typedef bson_value_type::org_type org_bson_value_type;

	typedef charset::utf8_string inner_string_type;
	typedef charset::utf8_string_view inner_string_view_type;

	typedef c_mongo_client_encryption_encrypt_range_opts range_opts_type;
	typedef range_opts_type::org_type org_range_opts_type;

public:
	typedef detail::setmark_value<s64> contention_factor_setmark_value_type;
	
private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(contention_factor_setmark_value_type) == sizeof(base_type::contention_factor_type))>));

protected:
	typedef mongoc_client_encryption_encrypt_opts_native_ex native_ex_type;

private:
	typedef c_mongo_client_encryption_encrypt_opts this_type;
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
			range_opts_type,
			range_opts_type,
			const range_opts_type&
		> range_opts_member_modify_helper_type;

public:
	c_mongo_client_encryption_encrypt_opts(void);

	template<typename KeyID, typename StrAlgo>
	c_mongo_client_encryption_encrypt_opts(const KeyID& key_id, const StrAlgo& str_algo)
	{
		this_type::pro_init();

		this_type::var_keyid() = key_id;
		this_type::var_algorithm() = str_algo;
	}

	template<typename KeyID, typename StrAlgo, typename StrKeyAltName, typename StrQueryMode>
	c_mongo_client_encryption_encrypt_opts(const KeyID& key_id, 
											const StrAlgo& str_algo,
											const StrKeyAltName& str_key_alt_name,
											s64 contention_factor, 
											const StrQueryMode& str_query_mode)
	{
		this_type::pro_init();

		this_type::var_keyid() = key_id;
		this_type::var_algorithm() = str_algo;
		this_type::var_keyaltname() = str_key_alt_name;
		this_type::var_contention_factor() = contention_factor;
		this_type::var_query_mode() = str_query_mode;
	}

	template<typename KeyID, typename StrAlgo, typename StrKeyAltName, 
				typename StrQueryMode, typename RangeOpts>
	c_mongo_client_encryption_encrypt_opts(const KeyID& key_id, 
											const StrAlgo& str_algo,
											const StrKeyAltName& str_key_alt_name,
											s64 contention_factor, 
											const StrQueryMode& str_query_mode,
											const RangeOpts& arg_range_opts)
	{
		this_type::pro_init();

		this_type::var_keyid() = key_id;
		this_type::var_algorithm() = str_algo;
		this_type::var_keyaltname() = str_key_alt_name;
		this_type::var_contention_factor() = contention_factor;
		this_type::var_query_mode() = str_query_mode;
		this_type::var_range_opts() = arg_range_opts;
	}

	c_mongo_client_encryption_encrypt_opts(const org_type* ptr);
	c_mongo_client_encryption_encrypt_opts(const org_type& right);

	c_mongo_client_encryption_encrypt_opts(BOOST_RV_REF(this_type) right)
	{
		this_type::pro_init();

		this_type& right_ref = right;
		this_type::pro_move(right_ref);
	}

	c_mongo_client_encryption_encrypt_opts(const this_type& right);
	~c_mongo_client_encryption_encrypt_opts(void);

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
		this_type::pro_swap(right_ref);
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
	// keyid
	inline bson_value_type& var_keyid(void)
	{
		return reinterpret_cast<bson_value_type&>(base_type::keyid);
	}

	inline const bson_value_type& var_keyid(void) const
	{
		return reinterpret_cast<const bson_value_type&>(base_type::keyid);
	}

	// var_algorithm
protected:
	inline const inner_string_type& pro_get_var_algorithm(void) const
	{
		return _algorithm;
	}

	inline void pro_set_var_algorithm(const inner_string_type& str_algo)
	{
		_algorithm = str_algo;
		base_type::algorithm = _algorithm.empty()? 0 : _algorithm.data();
	}

public:
	inline inner_string_member_modify_helper_type var_algorithm(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _algorithm,
				&this_type::pro_set_var_algorithm,
				&this_type::pro_get_var_algorithm);
	}

	// inline const inner_string_type& var_algorithm(void) const;
	
	// template<typename ...Args> inline 
	// const inner_string_type& var_algorithm(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String var_algorithm(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_algorithm(), var_algorithm, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_algorithm(), var_algorithm, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_var_algorithm(), var_algorithm)

	// var_keyaltname
protected:
	inline const inner_string_type& pro_get_var_keyaltname(void) const
	{
		return _keyaltname;
	}

	inline void pro_set_var_keyaltname(const inner_string_type& str_keyaltname)
	{
		_keyaltname = str_keyaltname;
		base_type::keyaltname = _keyaltname.empty()? 0 : _keyaltname.data();
	}

public:
	inline inner_string_member_modify_helper_type var_keyaltname(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _keyaltname,
				&this_type::pro_set_var_keyaltname,
				&this_type::pro_get_var_keyaltname);
	}

	// inline const inner_string_type& var_keyaltname(void) const;
	
	// template<typename ...Args> inline 
	// const inner_string_type& var_keyaltname(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String var_keyaltname(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_keyaltname(), var_keyaltname, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_keyaltname(), var_keyaltname, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_var_keyaltname(), var_keyaltname)

public:
	// contention_factor
	inline contention_factor_setmark_value_type& var_contention_factor(void)
	{
		return reinterpret_cast<contention_factor_setmark_value_type&>(base_type::contention_factor);
	}

	inline const contention_factor_setmark_value_type& var_contention_factor(void) const
	{
		return reinterpret_cast<const contention_factor_setmark_value_type&>(base_type::contention_factor);
	}

	inline s64 var_contention_factor_value(void) const
	{
		return base_type::contention_factor.value;
	}

	inline bool& var_contention_factor_set(void)
	{
		return (base_type::contention_factor).set;
	}

	inline bool var_contention_factor_set(void) const
	{
		return (base_type::contention_factor).set;
	}

	inline bool contention_factor_is_seted(void) const
	{
		return (base_type::contention_factor).set;
	}

	// var_query_mode
protected:
	inline const inner_string_type& pro_get_var_query_mode(void) const
	{
		return _query_mode;
	}

	inline void pro_set_var_query_mode(const inner_string_type& str_query_mode)
	{
		_query_mode = str_query_mode;
		base_type::query_type = _query_mode.empty()? 0 : _query_mode.data();
	}

public:
	inline inner_string_member_modify_helper_type var_query_mode(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _query_mode,
				&this_type::pro_set_var_query_mode,
				&this_type::pro_get_var_query_mode);
	}

	// inline const inner_string_type& var_query_mode(void) const;
	
	// template<typename ...Args> inline 
	// const inner_string_type& var_query_mode(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String var_query_mode(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_query_mode(), var_query_mode, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_var_query_mode(), var_query_mode, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_var_query_mode(), var_query_mode)

	// var_range_opts
protected:
	inline const range_opts_type& pro_get_var_range_opts(void) const
	{
		return _range_opts;
	}

	inline void pro_set_var_range_opts(const org_range_opts_type& arg_range_opts)
	{
		if(_algorithm == mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_range_preview())
		{
			_range_opts = arg_range_opts;
			base_type::range_opts = _range_opts.org_pointer();
		}
		else
		{
			_range_opts.clear();
			base_type::range_opts = 0;
		}
	}

public:
	inline range_opts_member_modify_helper_type var_range_opts(void)
	{
		return 
			range_opts_member_modify_helper_type(
				*this, _range_opts,
				&this_type::pro_set_var_range_opts,
				&this_type::pro_get_var_range_opts);
	}

	inline const range_opts_type& var_range_opts(void) const
	{
		return _range_opts;
	}

protected:
	void pro_init(void);
	void pro_rebuild_base(void);
	void pro_clear(void);
	this_type& pro_copy(const org_type* ptr);
	this_type& pro_copy(const this_type& right);
	void pro_swap(this_type& right);
	this_type& pro_move(this_type& right);
	
public:
	inline static const inner_string_type& s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_random(void)
	{
		return mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_random();
	}

	inline static const inner_string_type& s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic(void)
	{
		return mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic();
	}

public:
	inline static const inner_string_type& s_encrypt_algorithm_indexed(void)
	{
		return mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_indexed();
	}

	inline static const inner_string_type& s_encrypt_algorithm_unindexed(void)
	{
		return mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_unindexed();
	}

	inline static const inner_string_type& s_encrypt_algorithm_range_preview(void)
	{
		return mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_range_preview();
	}

public:
	inline static const inner_string_type& s_encrypt_query_mode_equality(void)
	{
		return mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_query_mode_equality();
	}

	inline static const inner_string_type& s_encrypt_query_mode_range_preview(void)
	{
		return mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_query_mode_range_preview();
	}

private:
	// bson_value_t keyid; // using base_type native
	inner_string_type _algorithm;
	inner_string_type _keyaltname;
	// struct contention_factor; // using base_type native
	inner_string_type _query_mode;
	range_opts_type _range_opts;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_client_encryption_encrypt_opts)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_ENCRYPT_OPTS_HPP__
