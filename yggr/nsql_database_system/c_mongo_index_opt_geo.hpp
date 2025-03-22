//c_mongo_index_opt_geo.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_OPT_GEO_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_OPT_GEO_HPP__

#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/nsql_database_system/mongoc_index_opt_geo_native_ex.hpp>
#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{

//typedef struct
//{
//   uint8_t twod_sphere_version;
//   uint8_t twod_bits_precision;
//   double  twod_location_min;
//   double  twod_location_max;
//   double  haystack_bucket_size;
//   uint8_t *padding[32];
//} mongoc_index_opt_geo_t;


class c_mongo_index_opt_geo
	: public mongoc_index_opt_geo_t
{
public:
	typedef mongoc_index_opt_geo_t base_type;
	typedef base_type org_type;

private:
	typedef c_mongo_index_opt_geo this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_index_opt_geo(void);
	c_mongo_index_opt_geo(const base_type* ptr);

	c_mongo_index_opt_geo(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		mongoc_index_opt_geo_native_ex::s_mongoc_index_opt_geo_copy(this, &right_ref);
	}

	c_mongo_index_opt_geo(const base_type& right);

	c_mongo_index_opt_geo(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		mongoc_index_opt_geo_native_ex::s_mongoc_index_opt_geo_copy(this, &right_ref);
	}

	c_mongo_index_opt_geo(const this_type& right);
	~c_mongo_index_opt_geo(void);

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type& right_ref = right;
		mongoc_index_opt_geo_native_ex::s_mongoc_index_opt_geo_copy(
            this, boost::addressof(right_ref));
		return *this;
	}

	this_type& operator=(const base_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		this_type& right_ref = right;
		mongoc_index_opt_geo_native_ex::s_mongoc_index_opt_geo_copy(
            this, boost::addressof(right_ref));
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void)
	{
		return static_cast<org_type*>(this);
	}

	inline const org_type* org_pointer(void) const
	{
		return static_cast<const org_type*>(this);
	}

//public:
//	inline operator org_type&(void)
//	{
//		return *(this_type::operator org_type*());
//	}
//
//	inline operator const org_type&(void) const
//	{
//		return *(this_type::operator const org_type*());
//	}
//
//	inline operator org_type*(void)
//	{
//		return this;
//	}
//
//	inline operator const org_type*(void) const
//	{
//		return this;
//	}

public:
	inline u8& var_twod_sphere_version(void)
	{
		return base_type::twod_sphere_version;
	}

	inline u8 var_twod_sphere_version(void) const
	{
		return base_type::twod_sphere_version;
	}

	inline u8& var_twod_bits_precision(void)
	{
		return base_type::twod_bits_precision;
	}

	inline u8 var_twod_bits_precision(void) const
	{
		return base_type::twod_bits_precision;
	}

	inline f64& var_twod_location_min(void)
	{
		return base_type::twod_location_min;
	}

	inline f64 var_twod_location_min(void) const
	{
		return base_type::twod_location_min;
	}

	inline f64& var_twod_location_max(void)
	{
		return base_type::twod_location_max;
	}

	inline f64 var_twod_location_max(void) const
	{
		return base_type::twod_location_max;
	}

	inline f64& var_haystack_bucket_size(void)
	{
		return base_type::haystack_bucket_size;
	}

	inline f64 var_haystack_bucket_size(void) const
	{
		return base_type::haystack_bucket_size;
	}

};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_mongo_index_opt_geo::base_type& l, c_mongo_index_opt_geo& r)
{
	r.swap(l);
}

inline void swap(c_mongo_index_opt_geo& l, c_mongo_index_opt_geo::base_type& r)
{
	l.swap(r);
}

inline void swap(c_mongo_index_opt_geo& l, c_mongo_index_opt_geo& r)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_OPT_GEO_HPP__
