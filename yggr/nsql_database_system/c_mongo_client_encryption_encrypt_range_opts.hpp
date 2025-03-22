// c_mongo_client_encryption_encrypt_range_opts.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_ENCYPT_RANGE_OPTS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_ENCYPT_RANGE_OPTS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/move/move.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/mongoc_client_encryption_encrypt_range_opts_native_ex.hpp>
#include <yggr/nsql_database_system/detail/setmark_value.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_client_encryption_encrypt_range_opts_t {
//   struct {
//      bson_value_t value;
//      bool set;
//   } min;
//   struct {
//      bson_value_t value;
//      bool set;
//   } max;
//   int64_t sparsity;
//   struct {
//      int32_t value;
//      bool set;
//   } precision;
//};

namespace detail
{

struct bson_value_setter
{
	// copy
	template<typename T> inline
	bool operator()(setmark_tag_set, bson_value_t& val, BOOST_RV_REF(T) nval) const
	{
		typedef c_bson_value real_bson_value_type;

		reinterpret_cast<real_bson_value_type&>(val) = boost::move(nval);
		return true;
	}

	template<typename T> inline
	bool operator()(setmark_tag_set, bson_value_t& val, const T& nval) const
	{
		typedef c_bson_value real_bson_value_type;

		reinterpret_cast<real_bson_value_type&>(val) = nval;
		return true;
	}

	// swap
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		operator()(setmark_tag_swap, bson_value_t& val, const BsonValue& bs_val) const
	{
		typedef c_bson_value real_bson_value_type;

		reinterpret_cast<real_bson_value_type&>(val).swap(bs_val);
		return true;
	}
};

} // namespace detail

class c_mongo_client_encryption_encrypt_range_opts
	: protected yggr_mongoc_client_encryption_encrypt_range_opts 
{
public:
	typedef yggr_mongoc_client_encryption_encrypt_range_opts base_type;
	typedef mongoc_client_encryption_encrypt_range_opts_t org_type;

	typedef c_bson_value bson_value_type;
	typedef bson_value_type::org_type org_bson_value_type;

public:
	typedef detail::setmark_value<org_bson_value_type, detail::bson_value_setter> min_max_setmark_value_type;
	typedef detail::setmark_value<s32> precision_setmark_value_type;
	
private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(min_max_setmark_value_type) == sizeof(base_type::min_max_type))>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(precision_setmark_value_type) == sizeof(base_type::precision_type))>));

protected:
	typedef mongoc_client_encryption_encrypt_range_opts_native_ex native_ex_type;

private:
	typedef c_mongo_client_encryption_encrypt_range_opts this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_client_encryption_encrypt_range_opts(void);

	c_mongo_client_encryption_encrypt_range_opts(s64 arg_sparsity);

	template<typename MinVal, typename MaxVal>
	c_mongo_client_encryption_encrypt_range_opts(const MinVal& min_val, const MaxVal& max_val,
													s64 arg_sparsity = 0, s32 arg_precision = 0)
	{
		this_type::pro_init();

		this_type::var_min() = min_val;
		this_type::var_max() = max_val;
		base_type::sparsity = arg_sparsity;

		if(base_type::min.value.value_type != base_type::max.value.value_type)
		{
			base_type::min.set = base_type::max.set = false;
		}

		switch(base_type::min.value.value_type)
		{
		case bson_typeid_def::E_BSON_TYPE_DOUBLE:
		case bson_typeid_def::E_BSON_TYPE_DECIMAL128:
			this_type::var_precision() = arg_precision;
			break;
		default:
			break;
		}
	}

	c_mongo_client_encryption_encrypt_range_opts(const org_type* ptr);
	c_mongo_client_encryption_encrypt_range_opts(const org_type& right);

	c_mongo_client_encryption_encrypt_range_opts(BOOST_RV_REF(this_type) right)
	{
		this_type::pro_init();

		this_type& right_ref = right;
		this_type::pro_move(right_ref);
	}

	c_mongo_client_encryption_encrypt_range_opts(const this_type& right);
	~c_mongo_client_encryption_encrypt_range_opts(void);

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
	// min
	inline min_max_setmark_value_type& var_min(void)
	{
		return reinterpret_cast<min_max_setmark_value_type&>((base_type::min));
	}

	inline const min_max_setmark_value_type& var_min(void) const
	{
		return reinterpret_cast<const min_max_setmark_value_type&>((base_type::min));
	}

	inline const bson_value_type& var_min_value(void) const
	{
		return reinterpret_cast<const bson_value_type&>((base_type::min).value);
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, const BsonValue&>::type
		var_min_value(void) const
	{
		return reinterpret_cast<const bson_value_type&>((base_type::min).value);
	}

	inline bool& var_min_set(void)
	{
		return (base_type::min).set;
	}

	inline bool var_min_set(void) const
	{
		return (base_type::min).set;
	}

	inline bool min_is_seted(void) const
	{
		return (base_type::min).set;
	}

	// max
	inline min_max_setmark_value_type& var_max(void)
	{
		return reinterpret_cast<min_max_setmark_value_type&>((base_type::max));
	}

	inline const min_max_setmark_value_type& var_max(void) const
	{
		return reinterpret_cast<const min_max_setmark_value_type&>((base_type::max));
	}

	inline const bson_value_type& var_max_value(void) const
	{
		return reinterpret_cast<const bson_value_type&>((base_type::max).value);
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, const BsonValue&>::type
		var_max_value(void) const
	{
		return reinterpret_cast<const bson_value_type&>((base_type::max).value);
	}

	inline bool& var_max_set(void)
	{
		return (base_type::max).set;
	}

	inline bool var_max_set(void) const
	{
		return (base_type::max).set;
	}

	inline bool max_is_seted(void) const
	{
		return (base_type::max).set;
	}

	// sparsity
	inline s64& var_sparsity(void)
	{
		return base_type::sparsity;
	}

	inline s64 var_sparsity(void) const
	{
		return base_type::sparsity;
	}

	// precision
	inline precision_setmark_value_type& var_precision(void)
	{
		return reinterpret_cast<precision_setmark_value_type&>(base_type::precision);
	}

	inline const precision_setmark_value_type& var_precision(void) const
	{
		return reinterpret_cast<const precision_setmark_value_type&>(base_type::precision);
	}

	inline s32 var_precision_value(void) const
	{
		return base_type::precision.value;
	}

	inline bool& var_precision_set(void)
	{
		return (base_type::precision).set;
	}

	inline bool var_precision_set(void) const
	{
		return (base_type::precision).set;
	}

	inline bool precision_is_seted(void) const
	{
		return (base_type::precision).set;
	}

public:
	inline void clear(void)
	{
		this_type::pro_clear();
	}

protected:
	void pro_init(void);
	void pro_clear(void);
	this_type& pro_copy(const org_type* psrc);
	this_type& pro_copy(const this_type& right);
	void pro_swap(this_type& right);
	this_type& pro_move(this_type& right);
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_client_encryption_encrypt_range_opts)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_ENCYPT_RANGE_OPTS_HPP__
