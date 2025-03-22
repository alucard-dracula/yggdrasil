//c_mongo_internal_tls_opts.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INTERNAL_TLS_OPTS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INTERNAL_TLS_OPTS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <ostream>

#if !defined(MONGOC_INSIDE)
#	define MONGOC_INSIDE
#	define YGGR_USING_MONGOC_INSIDE
#endif // MONGOC_INSIDE

#include <mongoc-ssl-private.h>

#if defined(YGGR_USING_MONGOC_INSIDE)
#	undef MONGOC_INSIDE
#	undef YGGR_USING_MONGOC_INSIDE
#endif // YGGR_USING_MONGOC_INSIDE

namespace yggr
{
namespace nsql_database_system
{

//typedef struct {
//   bool tls_disable_certificate_revocation_check;
//   bool tls_disable_ocsp_endpoint_check;
//} _mongoc_internal_tls_opts_t;

class c_mongo_internal_tls_opts
	: public _mongoc_internal_tls_opts_t
{
public:
	typedef _mongoc_internal_tls_opts_t base_type;
	typedef base_type org_type;

private:
	typedef c_mongo_internal_tls_opts this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_internal_tls_opts(void)
	{
		base_type::tls_disable_certificate_revocation_check = false;
		base_type::tls_disable_ocsp_endpoint_check = false;
	}

	c_mongo_internal_tls_opts(bool tdcrc, bool tdoec)
	{
		base_type::tls_disable_certificate_revocation_check = tdcrc;
		base_type::tls_disable_ocsp_endpoint_check = tdoec;
	}

	c_mongo_internal_tls_opts(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type::tls_disable_certificate_revocation_check = right_ref.tls_disable_certificate_revocation_check;
		base_type::tls_disable_ocsp_endpoint_check = right_ref.tls_disable_ocsp_endpoint_check;
	}

	c_mongo_internal_tls_opts(const base_type& right)
	{
		base_type::tls_disable_certificate_revocation_check = right.tls_disable_certificate_revocation_check;
		base_type::tls_disable_ocsp_endpoint_check = right.tls_disable_ocsp_endpoint_check;
	}

	c_mongo_internal_tls_opts(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		base_type::tls_disable_certificate_revocation_check = right_ref.tls_disable_certificate_revocation_check;
		base_type::tls_disable_ocsp_endpoint_check = right_ref.tls_disable_ocsp_endpoint_check;
	}

	c_mongo_internal_tls_opts(const this_type& right)
	{
		base_type::tls_disable_certificate_revocation_check = right.tls_disable_certificate_revocation_check;
		base_type::tls_disable_ocsp_endpoint_check = right.tls_disable_ocsp_endpoint_check;
	}

	~c_mongo_internal_tls_opts(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& base_ref = right;
		if(static_cast<base_type*>(this) == boost::addressof(base_ref))
		{
			return *this;
		}

		base_type::tls_disable_certificate_revocation_check = 
			boost::move(base_ref.tls_disable_certificate_revocation_check);
		base_type::tls_disable_ocsp_endpoint_check = 
			 boost::move(base_ref.tls_disable_ocsp_endpoint_check);

		return *this;
	}

	inline this_type& operator=(const base_type& right)
	{
		if(static_cast<base_type*>(this) == boost::addressof(right))
		{
			return *this;
		}

		base_type::tls_disable_certificate_revocation_check = right.tls_disable_certificate_revocation_check;
		base_type::tls_disable_ocsp_endpoint_check = right.tls_disable_ocsp_endpoint_check;

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::tls_disable_certificate_revocation_check = 
			boost::move(right.tls_disable_certificate_revocation_check);
		base_type::tls_disable_ocsp_endpoint_check = 
			boost::move(right.tls_disable_ocsp_endpoint_check);

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_type::tls_disable_certificate_revocation_check = right.tls_disable_certificate_revocation_check;
		base_type::tls_disable_ocsp_endpoint_check = right.tls_disable_ocsp_endpoint_check;

		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(base_type& right)
	{
		if(static_cast<base_type*>(this) == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(base_type::tls_disable_certificate_revocation_check, 
						right.tls_disable_certificate_revocation_check);
		::yggr::swap(base_type::tls_disable_ocsp_endpoint_check,
						right.base_type::tls_disable_ocsp_endpoint_check);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(base_type::tls_disable_certificate_revocation_check, 
						right.tls_disable_certificate_revocation_check);
		::yggr::swap(base_type::tls_disable_ocsp_endpoint_check,
						right.base_type::tls_disable_ocsp_endpoint_check);
	}

public:
	inline org_type* org_pointer(void)
	{
		return static_cast<org_type*>(this);
	}

	inline const org_type* org_pointer(void) const
	{
		return static_cast<const org_type*>(this);
	}

public:
	inline bool compare_eq(const base_type& right) const
	{
		return 
			(base_type::tls_disable_certificate_revocation_check
				== right.tls_disable_certificate_revocation_check)
			&& (base_type::tls_disable_ocsp_endpoint_check
				== right.tls_disable_ocsp_endpoint_check);
	}

	inline bool compare_eq(const this_type& right) const
	{
		return 
			(base_type::tls_disable_certificate_revocation_check
				== right.tls_disable_certificate_revocation_check)
			&& (base_type::tls_disable_ocsp_endpoint_check
				== right.tls_disable_ocsp_endpoint_check);
	}

public:
	inline bool& var_tls_disable_certificate_revocation_check(void)
	{
		return base_type::tls_disable_certificate_revocation_check;
	}

	inline bool var_tls_disable_certificate_revocation_check(void) const
	{
		return base_type::tls_disable_certificate_revocation_check;
	}

	inline bool& var_tls_disable_ocsp_endpoint_check(void)
	{
		return base_type::tls_disable_ocsp_endpoint_check;
	}

	inline bool var_tls_disable_ocsp_endpoint_check(void) const
	{
		return base_type::tls_disable_ocsp_endpoint_check;
	}
};

BOOST_MPL_ASSERT((
	boost::mpl::bool_<
		(sizeof(c_mongo_internal_tls_opts) == sizeof(c_mongo_internal_tls_opts::base_type))>));

// compare
inline bool operator==(const c_mongo_internal_tls_opts::base_type& l, const c_mongo_internal_tls_opts& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const c_mongo_internal_tls_opts& l, const c_mongo_internal_tls_opts::base_type& r)
{
	return l.compare_eq(r);
}

inline bool operator==(const c_mongo_internal_tls_opts& l, const c_mongo_internal_tls_opts& r)
{
	return l.compare_eq(r);
}

inline bool operator!=(const c_mongo_internal_tls_opts::base_type& l, const c_mongo_internal_tls_opts& r)
{
	return !r.compare_eq(l);
}

inline bool operator!=(const c_mongo_internal_tls_opts& l, const c_mongo_internal_tls_opts::base_type& r)
{
	return !l.compare_eq(r);
}

inline bool operator!=(const c_mongo_internal_tls_opts& l, const c_mongo_internal_tls_opts& r)
{
	return !l.compare_eq(r);
}

namespace detail
{

template<>
struct bson_operator_outter<c_mongo_internal_tls_opts>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_mongo_internal_tls_opts& val, 
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"tls_disable_certificate_revocation_check\" : " 
			<< (val.var_tls_disable_certificate_revocation_check()? "true" : "false")
			<< ", \"tls_disable_ocsp_endpoint_check\" : " 
			<< (val.var_tls_disable_ocsp_endpoint_check()? "true" : "false")
			<< " }";
		return os;
	}
};


} // namespace detail

// non-member foo
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_internal_tls_opts& val)
{
	typedef detail::bson_operator_outter<c_mongo_internal_tls_opts> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_mongo_internal_tls_opts& l, c_mongo_internal_tls_opts::base_type& r)
{ 
	l.swap(r); 
} 

inline void swap(c_mongo_internal_tls_opts::base_type& l, c_mongo_internal_tls_opts& r) 
{ 
	r.swap(l); 
} 

inline void swap(c_mongo_internal_tls_opts& l, c_mongo_internal_tls_opts& r) 
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INTERNAL_TLS_OPTS_HPP__
