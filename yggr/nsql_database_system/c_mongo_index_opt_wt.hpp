//c_mongo_index_opt_wt.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_OPT_WT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_OPT_WT_HPP__

#include <yggr/charset/utf8_string.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>
#include <yggr/nsql_database_system/mongoc_index_opt_wt_native_ex.hpp>

namespace yggr
{
namespace nsql_database_system
{

//typedef struct
//{
//   mongoc_index_opt_storage_t  base;
//   const char                 *config_str;
//   void                       *padding[8];
//} mongoc_index_opt_wt_t;

class c_mongo_index_opt_wt
	: public mongoc_index_opt_wt_t
{
public:
	typedef mongoc_index_opt_wt_t base_type;
	typedef mongoc_index_opt_wt_t org_type;

	typedef mongoc_index_opt_storage_t index_opt_storage_type;

public:
	typedef yggr::utf8_string inner_string_type;
	typedef inner_string_type string_type;

//public:
//	typedef ::yggr::string string_type;

private:
	typedef c_mongo_index_opt_wt this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type, 
			inner_string_type, 
			inner_string_type,
			const inner_string_type&
		> string_member_modify_helper_type;

public:
	c_mongo_index_opt_wt(void);
	c_mongo_index_opt_wt(const base_type* ptr);
	c_mongo_index_opt_wt(const base_type& right);

	c_mongo_index_opt_wt(BOOST_RV_REF(this_type) right)
	{
		this_type::prv_init();
		this_type& right_ref = right;
		this_type::pro_move(right_ref);
	}

	c_mongo_index_opt_wt(const this_type& right);
	~c_mongo_index_opt_wt(void);

public:
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
	inline index_opt_storage_type& var_index_opt_storage(void)
	{
		return base_type::base;
	}

	inline const index_opt_storage_type& var_index_opt_storage(void) const
	{
		return base_type::base;
	}

	inline string_member_modify_helper_type var_config_str(void)
	{
		return 
			string_member_modify_helper_type(
				*this, _config_str,
				&this_type::set_var_config_str,
				&this_type::get_var_config_str );
	}

	inline const inner_string_type& var_config_str(void) const
	{
		return this_type::get_var_config_str();
	}

private:
	inline const inner_string_type& get_var_config_str(void) const
	{
		return _config_str;
	}

	inline void set_var_config_str(const inner_string_type& cfg_str)
	{
		_config_str = cfg_str;
		base_type::config_str = _config_str.data();
	}

private:
	void prv_init(void);
	this_type& prv_copy(const base_type* ptr);
	this_type& prv_copy(const this_type& right);
	void prv_swap(this_type& right);
	this_type& pro_move(this_type& right);

private:
	inner_string_type _config_str;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_index_opt_wt)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_INDEX_OPT_WT_HPP__

