//c_mongo_find_and_modify_opts.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_FIND_AND_MODIFY_OPTS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_FIND_AND_MODIFY_OPTS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongoc_find_and_modify_opts_native_ex.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>


namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_find_and_modify_opts_t {
//   bson_t *sort;
//   bson_t *update;
//   bson_t *fields;
//   mongoc_find_and_modify_flags_t flags;
//   bool bypass_document_validation;
//   uint32_t max_time_ms;
//   bson_t extra;
//};

class c_mongo_find_and_modify_opts
	: protected yggr_mongoc_find_and_modify_opts
{

public:
	typedef yggr_mongoc_find_and_modify_opts base_type;
	typedef mongoc_find_and_modify_opts_t org_type;

	typedef c_bson bson_type;

public:
	// bypass document
	YGGR_STATIC_CONSTANT(
		u32, 
		E_MONGOC_BYPASS_DOCUMENT_VALIDATION_FALSE 
			= mongoc_find_and_modify_opts_native_ex::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_FALSE);
	
	YGGR_STATIC_CONSTANT(
		u32, 
		E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE 
			= mongoc_find_and_modify_opts_native_ex::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE);
	
	YGGR_STATIC_CONSTANT(
		u32, 
		E_MONGOC_BYPASS_DOCUMENT_VALIDATION_DEFAULT 
			= mongoc_find_and_modify_opts_native_ex::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_DEFAULT);

public:
	// flag
	YGGR_STATIC_CONSTANT(
		u32, 
		E_MONGOC_FIND_AND_MODIFY_NONE 
			= mongoc_find_and_modify_opts_native_ex::E_MONGOC_FIND_AND_MODIFY_NONE);
	
	YGGR_STATIC_CONSTANT(
		u32, 
		E_MONGOC_FIND_AND_MODIFY_REMOVE 
			= mongoc_find_and_modify_opts_native_ex::E_MONGOC_FIND_AND_MODIFY_REMOVE);
	
	YGGR_STATIC_CONSTANT(
		u32, 
		E_MONGOC_FIND_AND_MODIFY_UPSERT 
			= mongoc_find_and_modify_opts_native_ex::E_MONGOC_FIND_AND_MODIFY_UPSERT);

	YGGR_STATIC_CONSTANT(
		u32, 
		E_MONGOC_FIND_AND_MODIFY_RETURN_NEW 
			= mongoc_find_and_modify_opts_native_ex::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW);

private:
	typedef c_mongo_find_and_modify_opts this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_find_and_modify_opts(void);

	c_mongo_find_and_modify_opts(const org_type* ptr);
	c_mongo_find_and_modify_opts(const org_type& right);

	c_mongo_find_and_modify_opts(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::prv_init();
		this_type::pro_move(right_ref);
	}

	c_mongo_find_and_modify_opts(const this_type& right);
	~c_mongo_find_and_modify_opts(void);

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
	// var_sort
	inline bson_type& var_sort(void)
	{
		return _sort;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, Bson&>::type
		var_sort(void)
	{
		typedef Bson now_bson_type;

		return reinterpret_cast<now_bson_type&>(_sort);
	}

	inline const bson_type& var_sort(void) const
	{
		return _sort;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_sort(void) const
	{
		typedef Bson now_bson_type;

		return reinterpret_cast<const now_bson_type&>(_sort);
	}

	// var_update
	inline bson_type& var_update(void)
	{
		return _update;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, Bson&>::type
		var_update(void)
	{
		typedef Bson now_bson_type;

		return reinterpret_cast<now_bson_type&>(_update);
	}

	inline const bson_type& var_update(void) const
	{
		return _update;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_update(void) const
	{
		typedef Bson now_bson_type;

		return reinterpret_cast<const now_bson_type&>(_update);
	}

	// var_fields
	inline bson_type& var_fields(void)
	{
		return _fields;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, Bson&>::type
		var_fields(void)
	{
		typedef Bson now_bson_type;

		return reinterpret_cast<now_bson_type&>(_fields);
	}

	inline const bson_type& var_fields(void) const
	{
		return _fields;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_fields(void) const
	{
		typedef Bson now_bson_type;

		return reinterpret_cast<const now_bson_type&>(_fields);
	}

public:
	inline u32& var_flags(void)
	{
		return base_type::flags;
	}

	inline u32 var_flags(void) const
	{
		return base_type::flags;
	}

	inline u32& var_bypass_document_validation(void)
	{
		return base_type::bypass_document_validation;
	}

	inline u32 var_bypass_document_validation(void) const
	{
		return base_type::bypass_document_validation;
	}

	inline uint32_t& var_max_time_ms(void)
	{
		return base_type::max_time_ms;
	}

	inline uint32_t var_max_time_ms(void) const
	{
		return base_type::max_time_ms;
	}

public:
	// hedge
	inline bson_type& var_extra(void)
	{
		return reinterpret_cast<bson_type&>(base_type::extra);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, Bson&>::type
		var_extra(void)
	{
		typedef Bson now_bson_type;

		return reinterpret_cast<now_bson_type&>(base_type::extra);
	}

	inline const bson_type& var_extra(void) const
	{
		return reinterpret_cast<const bson_type&>(base_type::extra);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_extra(void) const
	{
		typedef Bson now_bson_type;

		return reinterpret_cast<const now_bson_type&>(base_type::extra);
	}

private:
	void prv_init(void);

	this_type& prv_copy(const org_type* ptr);
	this_type& prv_copy(const this_type& right);

	void prv_swap(this_type& right);
	this_type& pro_move(this_type& right);

private:
	bson_type _sort;
	bson_type _update;
	bson_type _fields;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_find_and_modify_opts)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_FIND_AND_MODIFY_OPTS_HPP__
