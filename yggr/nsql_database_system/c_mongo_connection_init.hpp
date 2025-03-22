//c_mongo_connection_init.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_INIT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_INIT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/type_traits/native_t.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>

#include <yggr/nsql_database_system/c_mongo_uri.hpp>
#include <yggr/nsql_database_system/c_mongo_ssl_opt.hpp>

namespace yggr
{
namespace nsql_database_system
{

struct c_mongo_connection_init
	: public c_mongo_uri
#ifdef MONGOC_ENABLE_SSL
	, public c_mongo_ssl_opt
#endif // MONGOC_ENABLE_SSL
{
public:
	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

	typedef c_mongo_uri uri_type;
	typedef uri_type::inner_string_type uri_string_type;
	typedef uri_type::inner_string_view_type uri_string_view_type;

#ifdef MONGOC_ENABLE_SSL
	typedef c_mongo_ssl_opt ssl_opt_type;
	typedef ssl_opt_type::path_string_type ssl_opt_path_string_type;
	typedef ssl_opt_type::pwd_string_type ssl_opt_pwd_string_type;
	typedef ssl_opt_type::internal_tls_opts_type internal_tls_opts_type;
#endif // MONGOC_ENABLE_SSL

private:
	typedef c_mongo_connection_init this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_mongo_connection_init(void);

	template<typename String>
	c_mongo_connection_init(const String& str_uri,
							typename 
								boost::enable_if
								<
									charset::utf8_string_constructable<String>,
									sfinae_type
								>::type sfinae = 0)
		: uri_type(str_uri)
	{
	}

	c_mongo_connection_init(BOOST_RV_REF(uri_type) uri)
		: uri_type(uri)
	{
	}

	c_mongo_connection_init(const uri_type& uri);
	
#ifdef MONGOC_ENABLE_SSL

	template<typename String1, 
				typename String2, typename String3,
				typename String4, typename String5,
				typename String6 >
	c_mongo_connection_init(const String1& str_uri,
								const String2& pem_file,
								const String3& pem_pwd,
								const String4& ca_file,
								const String5& ca_dir,
								const String6& crl_file,
								bool weak_cert_validation,
								bool ballow_invalid_hostname,
								const internal_tls_opts_type& internal_tls_opts,
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
												charset::utf8_string_constructable<String5>,
												charset::utf8_string_constructable<String6>
											>
										>,
										sfinae_type
									>::type sfinae = 0)
		: uri_type(str_uri),
			ssl_opt_type(
				pem_file, pem_pwd, 
				ca_file, ca_dir, crl_file,
				weak_cert_validation, ballow_invalid_hostname, internal_tls_opts)
	{
	}

	template<typename String1, 
				typename String2, typename String3,
				typename String4, typename String5,
				typename String6 >
	c_mongo_connection_init(const String1& str_uri,
								const String2& pem_file,
								const String3& pem_pwd,
								const String4& ca_file,
								const String5& ca_dir,
								const String6& crl_file,
								bool weak_cert_validation = false,
								bool ballow_invalid_hostname = false,
								bool tls_disable_certificate_revocation_check = false,
								bool tls_disable_ocsp_endpoint_check = false,
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
												charset::utf8_string_constructable<String5>,
												charset::utf8_string_constructable<String6>
											>
										>,
										sfinae_type
									>::type sfinae = 0)
		: uri_type(str_uri),
			ssl_opt_type(
				pem_file, pem_pwd, 
				ca_file, ca_dir, crl_file,
				weak_cert_validation, ballow_invalid_hostname,
				internal_tls_opts_type(
					tls_disable_certificate_revocation_check,
					tls_disable_ocsp_endpoint_check) )
	{
	}

	c_mongo_connection_init(BOOST_RV_REF(uri_type) uri, 
							BOOST_RV_REF(ssl_opt_type) ssl_opt)
		: uri_type(uri), ssl_opt_type(ssl_opt)
	{
	}

	c_mongo_connection_init(BOOST_RV_REF(uri_type) uri,
							const ssl_opt_type& ssl_opt)
		: uri_type(uri), ssl_opt_type(ssl_opt)
	{
	}

	c_mongo_connection_init(const uri_type& uri,
							BOOST_RV_REF(ssl_opt_type) ssl_opt)
		: uri_type(uri), ssl_opt_type(ssl_opt)
	{
	}

	c_mongo_connection_init(const uri_type& uri,
							const ssl_opt_type& ssl_opt);


#endif // MONGOC_ENABLE_SSL

	c_mongo_connection_init(BOOST_RV_REF(this_type) right)
		: uri_type(boost::move(static_cast<uri_type&>(right)))
#	if defined(MONGOC_ENABLE_SSL)
			,ssl_opt_type(boost::move(static_cast<ssl_opt_type&>(right)))
#	endif // MONGOC_ENABLE_SSL
	{
	}

	c_mongo_connection_init(const this_type& right);
	~c_mongo_connection_init(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		uri_type& base_uri = *this;
		copy_or_move_or_swap(base_uri, boost::move(static_cast<uri_type&>(right)));

#	if defined(MONGOC_ENABLE_SSL)
		ssl_opt_type& base_ssl_opt = *this;
		copy_or_move_or_swap(base_ssl_opt, boost::move(static_cast<ssl_opt_type&>(right)));
#	endif // MONGOC_ENABLE_SSL

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
	inline bool empty(void) const
	{
		return uri_type::empty();
	}

	inline bool is_using_tls(void) const
	{
#	ifdef MONGOC_ENABLE_SSL
		return uri_type::get_tls() && !ssl_opt_type::empty();
#	else
		return false;
#	endif // MONGOC_ENABLE_SSL
	}

	inline uri_type& uri(void)
	{
		return static_cast<uri_type&>(*this);
	}

	inline const uri_type& uri(void) const
	{
		return static_cast<const uri_type&>(*this);
	}

#ifdef MONGOC_ENABLE_SSL

	inline ssl_opt_type& ssl_opt(void)
	{
		return static_cast<ssl_opt_type&>(*this);
	}

	inline const ssl_opt_type& ssl_opt(void) const
	{
		return static_cast<const ssl_opt_type&>(*this);
	}

#endif // MONGOC_ENABLE_SSL

};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_connection_init)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CONNECTION_INIT_HPP__
