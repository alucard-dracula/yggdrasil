// c_mongo_server_description.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SERVER_DESCRIPTION_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SERVER_DESCRIPTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongoc_server_description_native_ex.hpp>
#include <yggr/nsql_database_system/c_mongo_host_info.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

struct mongoc_server_description_t_deleter
{
	inline void operator()(mongoc_server_description_t* x) const
	{
		if(x) 
		{
			mongoc_server_description_destroy(x);
		}
	}
};

} // namespace detail
} // namespcae nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_server_description
{
public:
	typedef mongoc_server_description_t org_type;
	typedef ::yggr::shared_ptr<org_type> mongoc_server_description_t_ptr_type;

public:
	typedef mongoc_server_description_native_ex::typeid_type typeid_type;

	typedef c_bson bson_type;
	typedef bson_type::org_type org_bson_type;

	typedef c_mongo_host_info host_info_type;

public:
	typedef charset::utf8_string inner_string_type;
	typedef charset::utf8_string_view inner_string_view_type;

private:
	typedef mongoc_server_description_t_ptr_type desc_ptr_type;
	typedef detail::mongoc_server_description_t_deleter desc_deleter_type;

private:
	typedef c_mongo_server_description this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_server_description(void);
	c_mongo_server_description(mongoc_server_description_t* pdesc);

	c_mongo_server_description(BOOST_RV_REF(this_type) right)
		: _pdesc(boost::move(right._pdesc))
	{
	}

	c_mongo_server_description(const this_type& right);
	~c_mongo_server_description(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		_pdesc.swap(right._pdesc);

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
	inline org_type* org_pointer(void)
	{
		return _pdesc.get();
	}

	inline const org_type* org_pointer(void) const
	{
		return _pdesc.get();
	}

public:
	inline operator org_type*(void)
	{
		return this_type::org_pointer();
	}

	inline operator const org_type*(void) const
	{
		return this_type::org_pointer();
	}

public:
	inline bool empty(void) const
	{
		return !_pdesc;
	}

public:
	inline u32 id(void) const
	{
		return _pdesc? mongoc_server_description_id(this_type::org_pointer()) : 0;
	}

	inline s64 last_update_time(void) const // usec
	{
		return _pdesc? mongoc_server_description_last_update_time(this_type::org_pointer()) : 0;
	}

	inline s64 round_trip_time(void) const // msec
	{
		return _pdesc? mongoc_server_description_round_trip_time(this_type::org_pointer()) : 0;
	}

protected:
	inline const host_info_type& pro_host(void) const
	{
		static const host_info_type host_empty;

		const host_info_type* phost 
			= _pdesc? 
				const_cast<const host_info_type*>(
					reinterpret_cast<host_info_type*>(
						mongoc_server_description_host (this_type::org_pointer())))
				: boost::addressof(host_empty);
		
		return *phost;
	}

public:
	inline const host_info_type& host(void) const
	{
		return this_type::pro_host();
	}

protected:
	inline const bson_type& pro_hello_response(void) const
	{
		static const bson_type bs_empty;
		
		return
			_pdesc?
				(*(reinterpret_cast<const bson_type*>(
					mongoc_server_description_hello_response(this_type::org_pointer()))))
				: bs_empty;
	}
public:
	inline const bson_type& hello_response(void) const
	{
		return this_type::pro_hello_response();
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<org_bson_type, Bson>, const Bson&>::type
		hello_response(void) const 
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type&>(this_type::pro_hello_response());
	}

	// typeid(enum type)
public:
	inline typeid_type typeid_value(void) const
	{
		return mongoc_server_description_native_ex::s_typeid(this_type::org_pointer());
	}

protected:
	inline inner_string_view_type pro_typeid_name(void) const
	{
		return 
			_pdesc?
				inner_string_view_type(mongoc_server_description_type(this_type::org_pointer()))
				: inner_string_view_type();
	}

public:
	inline inner_string_view_type typeid_name(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_typeid_name();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc> 
	//	typeid_name(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String typeid_name(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	typeid_name(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(typeid_name)

private:
	desc_ptr_type _pdesc;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_mongo_server_description& l, 
					c_mongo_server_description& r) 
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_SERVER_DESCRIPTION_HPP__
