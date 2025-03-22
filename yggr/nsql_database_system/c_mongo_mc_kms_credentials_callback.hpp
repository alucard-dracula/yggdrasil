// c_mongo_mc_kms_credentials_callback.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_MC_KMS_CREDENTIALS_CALLBACK_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_MC_KMS_CREDENTIALS_CALLBACK_HPP__

#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>

#include <yggr/nsql_database_system/mongoc_mc_kms_credentials_callback_native_ex.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct yggr_mc_kms_credentials_callback 
//{
//	mongoc_kms_credentials_provider_callback_fn fn;
//	void *userdata;
//};


class c_mc_kms_credentials_callback
	: protected yggr_mc_kms_credentials_callback
{
public:
	typedef yggr_mc_kms_credentials_callback base_type;

public:
	typedef mongoc_kms_credentials_provider_callback_fn callback_fn_type;
	typedef void* userdata_type;

private:
	typedef c_mc_kms_credentials_callback this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mc_kms_credentials_callback(void);
	c_mc_kms_credentials_callback(callback_fn_type pfoo, void* puserdata = 0);

	template<typename UserData>
	c_mc_kms_credentials_callback(callback_fn_type pfoo, UserData& arg_userdata)
	{
		base_type::fn = pfoo;
		base_type::userdata = reinterpret_cast<void*>(boost::addressof(arg_userdata));
	}

	c_mc_kms_credentials_callback(BOOST_RV_REF(this_type) right)
	{
		base_type& right_ref = right;
		base_type::fn = right_ref.fn;
		base_type::userdata = right.userdata;
	}

	c_mc_kms_credentials_callback(const this_type& right);
	~c_mc_kms_credentials_callback(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::fn = right_ref.fn;
		base_type::userdata = right_ref.userdata;
		
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
	inline callback_fn_type& var_fn(void)
	{
		return base_type::fn;
	}

	inline callback_fn_type var_fn(void) const
	{
		return base_type::fn;
	}

	inline void*& var_userdata(void)
	{
		return base_type::userdata;
	}

	inline void* var_userdata(void) const
	{
		return base_type::userdata;
	}

public:
	inline bool empty(void) const
	{
		return !(base_type::fn);
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
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mc_kms_credentials_callback)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_MC_KMS_CREDENTIALS_CALLBACK_HPP__
