// c_mongo_client_encryption_rewrap_many_datakey_result.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_REWRAP_MANY_DATAKEY_RESULT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_REWRAP_MANY_DATAKEY_RESULT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/move/move.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongoc_client_encryption_rewrap_many_datakey_result_native_ex.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_client_encryption_rewrap_many_datakey_result_t {
//   bson_t bulk_write_result;
//};

class c_mongo_client_encryption_rewrap_many_datakey_result
	: protected yggr_mongoc_client_encryption_rewrap_many_datakey_result_t 
{
public:
	typedef yggr_mongoc_client_encryption_rewrap_many_datakey_result_t base_type;
	typedef mongoc_client_encryption_rewrap_many_datakey_result_t org_type;

	typedef c_bson bson_type;

private:
	typedef c_mongo_client_encryption_rewrap_many_datakey_result this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_client_encryption_rewrap_many_datakey_result(void);
	c_mongo_client_encryption_rewrap_many_datakey_result(const bson_t& bs_data);

	c_mongo_client_encryption_rewrap_many_datakey_result(const org_type* ptr);
	c_mongo_client_encryption_rewrap_many_datakey_result(const org_type& right);

	c_mongo_client_encryption_rewrap_many_datakey_result(BOOST_RV_REF(this_type) right)
	{
		this_type::pro_init();
		::bson_init(boost::addressof(base_type::bulk_write_result));

		this_type& right_ref = right;
		this_type::pro_move(right_ref);
	}

	c_mongo_client_encryption_rewrap_many_datakey_result(const this_type& right);
	~c_mongo_client_encryption_rewrap_many_datakey_result(void);

public:
	this_type& operator=(const bson_t& right);
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

	inline operator bson_type&(void)
	{
		return reinterpret_cast<bson_type&>(base_type::bulk_write_result);
	}

	inline operator const bson_type&(void) const
	{
		return reinterpret_cast<const bson_type&>(base_type::bulk_write_result);
	}

public:
	inline bson_type& result(void)
	{
		return reinterpret_cast<bson_type&>(base_type::bulk_write_result);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, bson_type>, Bson&>::type
		result(void)
	{
		typedef Bson ret_type;
		return reinterpret_cast<ret_type&>(base_type::bulk_write_result);
	}

	inline const bson_type& result(void) const
	{
		return reinterpret_cast<const bson_type&>(base_type::bulk_write_result);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, bson_type>, const Bson&>::type
		result(void) const
	{
		typedef Bson ret_type;
		return reinterpret_cast<const ret_type&>(base_type::bulk_write_result);
	}

	inline void clear(void)
	{
		this_type::pro_clear();
	}

	inline bool empty(void) const
	{
		return 
			(!base_type::bulk_write_result.len)
			|| (bson_empty(boost::addressof(base_type::bulk_write_result)));
	}

protected:
	void pro_init(void);
	void pro_clear(void);
	this_type& pro_copy(const org_type* ptr);
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
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_client_encryption_rewrap_many_datakey_result)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_REWRAP_MANY_DATAKEY_RESULT_HPP__
