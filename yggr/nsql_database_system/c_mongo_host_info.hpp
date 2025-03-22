// c_mongo_host_info.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_HOST_INFO_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_HOST_INFO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/utility/member_var_modify_helper.hpp>
#include <yggr/move/move.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>
#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>

#include <boost/mpl/bool.hpp>
#include <ostream>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_host_list_t
//{
//   mongoc_host_list_t *next;
//   char                host [BSON_HOST_NAME_MAX + 1];
//   char                host_and_port [BSON_HOST_NAME_MAX + 7];
//   uint16_t            port;
//   int                 family;
//   void               *padding [4];
//};

struct yggr_mongoc_host_info
{
	yggr_mongoc_host_info*	next;
	char					host[YGGR_BSON_HOST_NAME_MAX + 1];
	char					host_and_port[YGGR_BSON_HOST_NAME_MAX + 7];
	uint16_t				port;
	int						family;
	void*					padding[4];
};

class c_mongo_host_info
	: protected yggr_mongoc_host_info
{
public:
	typedef yggr_mongoc_host_info base_type;
	typedef mongoc_host_list_t org_type;

	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

private:
	YGGR_STATIC_CONSTANT(u32, E_copy_start = (sizeof(yggr_mongoc_host_info*)));
	YGGR_STATIC_CONSTANT(u32, E_copy_end = (sizeof(yggr_mongoc_host_info) - (sizeof(void*) * 4)));
	YGGR_STATIC_CONSTANT(u32, E_copy_size = (E_copy_end - E_copy_start)); 

private:
	typedef c_mongo_host_info this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type, 
			mplex::null_type, 
			inner_string_type,
			inner_string_view_type
		> inner_string_member_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type, 
			u16, 
			u16,
			u16
		> u16_member_modify_helper_type;

public:
	c_mongo_host_info(void);

	template<typename StrHost>
	c_mongo_host_info(const StrHost& str_host, uint16_t nport, int nfamily,
						typename
							boost::enable_if
							<
								charset::utf8_string_constructable<StrHost>,
								sfinae_type
							>::type sfinae = 0)
	{
		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));

		this_type::pro_set_host_and_port(str_host, nport);
		base_type::family = nfamily;
	}

	c_mongo_host_info(BOOST_RV_REF(org_type) right)
	{
		base_type& base = *this;
		org_type& right_ref = right;
		
		memset(&base, 0, sizeof(base_type));
		memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right_ref), E_copy_size);
	}

	c_mongo_host_info(const org_type& right);
	c_mongo_host_info(const org_type* right);

	c_mongo_host_info(BOOST_RV_REF(base_type) right)
	{
		base_type& base = *this;
		base_type& right_ref = right;

		memset(&base, 0, sizeof(base_type));
		memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right_ref), E_copy_size);
	}

	c_mongo_host_info(const base_type& right);

	c_mongo_host_info(BOOST_RV_REF(this_type) right)
	{
		base_type& base = *this;
		this_type& right_ref = right;

		memset(&base, 0, sizeof(base_type));
		memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right_ref), E_copy_size);
	}

	c_mongo_host_info(const this_type& right);

	~c_mongo_host_info(void);

public:
	inline this_type& operator=(BOOST_RV_REF(org_type) right)
	{
		org_type& right_chk = right;
		if(reinterpret_cast<void*>(this) 
			== reinterpret_cast<void*>(&right_chk))
		{
			return *this;
		}

		base_type& base = *this;
		org_type& right_ref = right;

		memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right_ref), E_copy_size);
		return *this;
	}

	this_type& operator=(const org_type& right);

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type& base = *this;
		base_type& right_ref = right;

		memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right_ref), E_copy_size);
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

		base_type& base = *this;
		this_type& right_ref = right;

		memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right_ref), E_copy_size);
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(org_type) right)
	{
		org_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(org_type& right);

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

protected:
	inline inner_string_view_type pro_get_var_host(void) const
	{
		return inner_string_view_type(base_type::host);
	}

	inline void pro_set_var_host(const inner_string_type& val)
	{
		this_type::pro_set_host_and_port(val, base_type::port);
	}

public:
	inner_string_member_modify_helper_type var_host(void)
	{
		mplex::null_type null_obj;
		return 
			inner_string_member_modify_helper_type(
				*this, null_obj,
				&this_type::pro_set_var_host,
				&this_type::pro_get_var_host);
	}

	inline inner_string_view_type var_host(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_get_var_host();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc> 
	//	var_host(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String var_host(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	var_host(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(var_host)

	inline inner_string_view_type var_host_and_port(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return inner_string_view_type(base_type::host_and_port);
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc> 
	//	var_host_and_port(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String var_host_and_port(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	var_host_and_port(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(var_host_and_port)

protected:
	inline u16 pro_get_var_port(void) const
	{
		return base_type::port;
	}

	inline void pro_set_var_port(u16 nport)
	{
		this_type::pro_set_host_and_port(
			charset::make_string_charset_helper(
				base_type::host, YGGR_STR_UTF8_STRING_CHARSET_NAME()),
			nport);
	}

public:
	u16_member_modify_helper_type var_port(void)
	{
		return 
			u16_member_modify_helper_type(
				*this, base_type::port,
				&this_type::pro_set_var_port,
				&this_type::pro_get_var_port);
	}

	inline u16 var_port(void) const
	{
		return base_type::port;
	}

public:
	inline int& var_family(void)
	{
		return base_type::family;
	}
	
	inline int var_family(void) const
	{
		return base_type::family;
	}

public:
	inline this_type*& var_next(void)
	{
		base_type*& base_next_ref = base_type::next;
		return reinterpret_cast<this_type*&>(base_next_ref);
	}

	inline const this_type* var_next(void) const
	{
		return reinterpret_cast<this_type*>(base_type::next);
	}

	template<typename T> inline
	typename 
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_base_of<base_type, T>,
				boost::is_base_of<org_type, T>
			>,
			T*&
		>::type
		var_next(void)
	{
		typedef T*& ret_type;

		base_type*& base_next_ref = base_type::next;
		return reinterpret_cast<ret_type>(base_next_ref);
	}

	template<typename T> inline
	typename 
		boost::enable_if
		<
			boost::mpl::or_
			<
				boost::is_base_of<base_type, T>,
				boost::is_base_of<org_type, T>
			>,
			const T*
		>::type
		var_next(void) const
	{
		typedef const T* ret_type;

		return reinterpret_cast<ret_type>(base_type::next);
	}
	
protected:
	template<typename StrHost>
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<StrHost>,
			void
		>::type
		pro_set_host_and_port(const StrHost& str_host, uint16_t nport)
	{
		typedef typename native_t<StrHost>::type str_host_type;
		//YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(str_host_type, utf8_str_host_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(str_host_type, utf8_str_host_type);

		utf8_str_host_type utf8_str_host((charset::string_charset_helper_data(str_host)));
		assert(utf8_str_host.size() < YGGR_BSON_HOST_NAME_MAX + 1);

		// host
		if(utf8_str_host.size())
		{
			inner_string_type utf8_str_port;
			{
				std::stringstream ss;
				ss << nport;
				utf8_str_port = charset::make_string_charset_helper(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
				assert(utf8_str_port.size());
			}

			assert((utf8_str_host.size() + 1 + utf8_str_port.size()) < YGGR_BSON_HOST_NAME_MAX + 7);

			memcpy(base_type::host, utf8_str_host.data(), utf8_str_host.size());
			memcpy(base_type::host_and_port, utf8_str_host.data(), utf8_str_host.size());
			if(utf8_str_port.size())
			{
				base_type::host_and_port[utf8_str_host.size()] = ':';
				memcpy(base_type::host_and_port + utf8_str_host.size() + 1, utf8_str_port.data(), utf8_str_port.size());
			}
		}
		else
		{
			base_type::host_and_port[0] = 0;
		}
		
		base_type::port = nport;
	}

protected:
	inline static const void* pro_get_copy_begin(const void* pobj)
	{
		const base_type* ptr = reinterpret_cast<const base_type*>(pobj);
		return ptr->host;
	}

	inline static void* pro_get_copy_begin(void* pobj)
	{
		base_type* ptr = reinterpret_cast<base_type*>(pobj);
		return ptr->host;
	}
};

BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(yggr_mongoc_host_info) == sizeof(c_mongo_host_info))>));

// non-member function

namespace detail
{

template<>
struct bson_operator_outter<c_mongo_host_info>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_mongo_host_info& val, 
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"host\" : \"" << val.var_host_and_port() 
			<< "\", \"family\" : " << val.var_family()
			<< " }";

		return os;
	}
};


} // namespace detail

// io
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_host_info& val)
{
	typedef detail::bson_operator_outter<c_mongo_host_info> outter_type;

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

template<typename HostInfo> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_base_of<c_mongo_host_info::org_type, HostInfo>,
			boost::is_base_of<c_mongo_host_info::base_type, HostInfo>
		>,
		void
	>::type
	swap(c_mongo_host_info& l, HostInfo& r)
{
	l.swap(r);
}

template<typename HostInfo> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_base_of<c_mongo_host_info::org_type, HostInfo>,
			boost::is_base_of<c_mongo_host_info::base_type, HostInfo>
		>,
		void
	>::type
	swap(HostInfo& l, c_mongo_host_info& r)
{
	r.swap(l);
}

inline void swap(c_mongo_host_info& l, c_mongo_host_info& r) 
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_HOST_INFO_HPP__