//c_mongo_server_api.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SERVER_API_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SERVER_API_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>
#include <yggr/type_traits/number_t.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/detail/incomplete t_move_patch_macro.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_server_api_t {
//   mongoc_server_api_version_t version;
//   mongoc_optional_t strict;
//   mongoc_optional_t deprecation_errors;
//};

struct yggr_mongoc_server_api_t 
{
	mongoc_server_api_version_t version;
	mongoc_optional_t strict;
	mongoc_optional_t deprecation_errors;
};

} // namespace nsql_database_system
} // namespace yggr

// gcc needed fix invalid use of incomplete type 'struct _mongoc_apm_callbacks_t'
YGGR_PP_NSQL_DBS_INCOMPLETE_T_MOVE_HELPER(
	mongoc_server_api_t, ::yggr::nsql_database_system::yggr_mongoc_server_api_t )

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_server_api_t {
//   mongoc_server_api_version_t version;
//   mongoc_optional_t strict;
//   mongoc_optional_t deprecation_errors;
//};

//struct yggr_mongoc_server_api_t 
//{
//	mongoc_server_api_version_t version;
//	mongoc_optional_t strict;
//	mongoc_optional_t deprecation_errors;
//};

class c_mongo_server_api
	: protected yggr_mongoc_server_api_t
{
protected:
	typedef yggr_mongoc_server_api_t base_type;

public:
	typedef mongoc_server_api_t org_type;

public:
	typedef YGGR_PP_UINTEGER_T_TYPE(sizeof(mongoc_server_api_version_t)) version_type;
	typedef mongoc_server_api_version_t org_version_type;

	typedef mongoc_optional_t org_mongoc_optional_type;

public:
	YGGR_STATIC_CONSTANT(version_type, E_MONGOC_SERVER_API_V1 = static_cast<version_type>(MONGOC_SERVER_API_V1));

private:
	typedef c_mongo_server_api this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			bool,
			bool,
			bool
		> bool_member_modify_helper_type;
	
public:
	c_mongo_server_api(void);
	c_mongo_server_api(version_type ver, bool bstrict = false, bool berrors = false);

	c_mongo_server_api(const org_type* right);
	c_mongo_server_api(const org_type& right);

	c_mongo_server_api(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_cref = right;
		this_type::pro_copy(right_cref);
	}

	c_mongo_server_api(const this_type& right);
	~c_mongo_server_api(void);

public:
	this_type& operator=(const org_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_cref = right;
		this_type::pro_copy(right_cref);
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	void swap(org_type& right);

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
	inline version_type& var_version(void)
	{
		return reinterpret_cast<version_type&>(base_type::version);
	}

	inline version_type var_version(void) const
	{
		return static_cast<version_type>(base_type::version);
	}

	// strict
	inline org_mongoc_optional_type& var_strict(void)
	{
		return base_type::strict;
	}

	inline const org_mongoc_optional_type& var_strict(void) const
	{
		return base_type::strict;
	}

protected:
	inline bool pro_get_strict(void) const
	{
		return base_type::strict.value;
	}

	inline void pro_set_strict(bool bval)
	{
		base_type::strict.value = bval;
		base_type::strict.is_set = true;
	}

public:
	inline bool_member_modify_helper_type strict_value(void)
	{
		return 
			bool_member_modify_helper_type(
				*this, 
				base_type::strict.value,
				&this_type::pro_set_strict,
				&this_type::pro_get_strict);
	}

	inline bool strict_value(void) const
	{
		return base_type::strict.value;
	}

	inline bool strict_is_set(void) const
	{
		return base_type::strict.is_set;
	}

public:
	// deprecation_errors
	inline org_mongoc_optional_type& var_deprecation_errors(void)
	{
		return base_type::deprecation_errors;
	}

	inline const org_mongoc_optional_type& var_deprecation_errors(void) const
	{
		return base_type::deprecation_errors;
	}

protected:
	inline bool pro_get_deprecation_errors(void) const
	{
		return base_type::deprecation_errors.value;
	}

	inline void pro_set_deprecation_errors(bool bval)
	{
		base_type::deprecation_errors.value = bval;
		base_type::deprecation_errors.is_set = true;
	}

public:
	inline bool_member_modify_helper_type deprecation_errors_value(void)
	{
		return 
			bool_member_modify_helper_type(
				*this, 
				base_type::deprecation_errors.value,
				&this_type::pro_set_deprecation_errors,
				&this_type::pro_get_deprecation_errors);
	}

	inline bool deprecation_errors_value(void) const
	{
		return base_type::deprecation_errors.value;
	}

	inline bool deprecation_errors_is_set(void) const
	{
		return base_type::deprecation_errors.is_set;
	}

private:
	void prv_init(void);
	void pro_copy(const base_type& right);
	void pro_copy(const this_type& right);
	void pro_swap(base_type& right);
	void pro_swap(this_type& right);
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_mongo_server_api::org_type& l, c_mongo_server_api& r) 
{ 
	r.swap(l); 
}

inline void swap(c_mongo_server_api& l, c_mongo_server_api::org_type& r) 
{ 
	l.swap(r); 
}
	
inline void swap(c_mongo_server_api& l, c_mongo_server_api& r) 
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SERVER_API_HPP__
