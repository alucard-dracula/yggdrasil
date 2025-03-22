//c_mongo_read_concern.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_READ_CONCERN_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_READ_CONCERN_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongoc_read_concern_native_ex.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>
#include <yggr/nsql_database_system/detail/incomplete t_move_patch_macro.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>

#include <ostream>

// gcc needed fix invalid use of incomplete type 'struct _mongoc_apm_callbacks_t'
YGGR_PP_NSQL_DBS_INCOMPLETE_T_MOVE_HELPER(
	mongoc_read_concern_t, ::yggr::nsql_database_system::yggr_mongoc_read_concern )

namespace yggr
{
namespace nsql_database_system
{

/*
struct _mongoc_read_concern_t
{
   char   *level;
   bool    frozen;
   bson_t  compiled;
};
*/

// don't implement rvref org_type
class c_mongo_read_concern
	: protected yggr_mongoc_read_concern
{

public:
	typedef yggr_mongoc_read_concern base_type;
	typedef mongoc_read_concern_t org_type;

	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

	typedef c_bson bson_type;

private:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef c_mongo_read_concern this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			char*,
			inner_string_view_type,
			inner_string_view_type
		> member_level_modify_helper_type;

public:
	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_local(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return mongoc_read_concern_native_ex::s_level_local();
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_majority(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return mongoc_read_concern_native_ex::s_level_majority();
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_available(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return mongoc_read_concern_native_ex::s_level_available();
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_linearizable(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return mongoc_read_concern_native_ex::s_level_linearizable();
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_snapshot(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return mongoc_read_concern_native_ex::s_level_snapshot();
	}

	YGGR_CONSTEXPR_OR_INLINE static bool level_check(const char* str_lv) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return mongoc_read_concern_native_ex::s_level_check(str_lv);
	}

	template<typename String> YGGR_CONSTEXPR_OR_INLINE
	static 
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			bool
		>::type 
		level_check(const String& str_lv) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return mongoc_read_concern_native_ex::s_level_check(str_lv);
	}

public:
	c_mongo_read_concern(void);

	template<typename String>
	c_mongo_read_concern(const String& str_lv,
							typename
								boost::enable_if
								<
									charset::utf8_string_constructable<String>,
									sfinae_type
								>::type tag = 0)
	{
		mongoc_read_concern_native_ex::s_mongoc_read_concern_init(this);
		this_type::pro_set_var_level_tpl(str_lv);
	}

	explicit c_mongo_read_concern(const org_type* ptr);

	c_mongo_read_concern(const org_type& right);

	c_mongo_read_concern(BOOST_RV_REF(this_type) right)
	{
		mongoc_read_concern_native_ex::s_mongoc_read_concern_init(this);
		this_type& right_ref = right;
		mongoc_read_concern_native_ex::s_mongoc_read_concern_move(this, boost::addressof(right_ref));
	}

	c_mongo_read_concern(const this_type& right);
	~c_mongo_read_concern(void);

public:
	this_type& operator=(const org_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		mongoc_read_concern_native_ex::s_mongoc_read_concern_move(this, boost::addressof(right_ref));
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	void swap(org_type& right);

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

	// level
protected:
	inline inner_string_view_type pro_get_var_level(void) const
	{
		return inner_string_view_type(base_type::level);
	}

	void pro_set_var_level(const inner_string_view_type& str_lv);

	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			void
		>::type
		pro_set_var_level_tpl(const String& str_lv)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type utf8_str_lv((charset::string_charset_helper_data(str_lv)));
		
		assert(level_check(utf8_str_lv));
		
		if(!level_check(utf8_str_lv))
		{
			return;
		}

		mongoc_read_concern_native_ex::s_mongoc_read_concern_set_level(
			this, utf8_str_lv.data(), utf8_str_lv.size());
	}

public:
	inline member_level_modify_helper_type var_level(void)
	{
		return
			member_level_modify_helper_type(
				*this, 
				base_type::level, 
				&this_type::pro_set_var_level,
				&this_type::pro_get_var_level);
	}

	inline inner_string_view_type var_level(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_get_var_level();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc> 
	//	var_level(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String var_level(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	var_level(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(var_level)

	// compiled
public:
	inline const bson_type& var_compiled(void) const
	{
		return *(reinterpret_cast<const bson_type*>(boost::addressof(base_type::compiled)));
	}

	template<typename Bson> inline
	typename 
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			const Bson&
		>::type
		var_compiled(void) const
	{
		typedef Bson now_bson_type;

		const now_bson_type* pbs = 
			reinterpret_cast<const now_bson_type*>(boost::addressof(base_type::compiled));
		return *pbs;
	}

public:
	template<typename Bson> inline
	typename 
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			bool
		>::type
		append_to(Bson& cmd) const
	{
		return
			mongoc_read_concern_native_ex::s_mongoc_read_concern_append(
				const_cast<this_type*>(this), 
				static_cast<bson_type::org_type*>(boost::addressof(cmd)));
	}

public:
	inline this_type unfreeze_copy(void) const
	{
		this_type tmp;
		mongoc_read_concern_native_ex::s_mongoc_read_concern_unfreeze_copy(
			boost::addressof(tmp), this);
		return tmp;
	}

public:
	inline bool freeze(void)
	{
		return mongoc_read_concern_native_ex::s_mongoc_read_concern_freeze(this);
	}

	inline bool unfreeze(void)
	{
		return mongoc_read_concern_native_ex::s_mongoc_read_concern_unfreeze(this);
	}

	inline bool is_frozen(void) const
	{
		return base_type::frozen;
	}

public:
	inline bool is_default(void) const
	{
		return !(base_type::level);
	}
};

namespace detail
{

template<>
struct bson_operator_outter<c_mongo_read_concern>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_mongo_read_concern& val, 
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"level\" : \"" << val.var_level() 
			<< "\", \"forzen\" : " << (val.is_frozen()? "true" : "false") 
			<< ", \"compiled\" : " << val.var_compiled() 
			<< " }";
		return os;
	}
};

} // namespace detail

// non-member function
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_read_concern& val)
{
	typedef detail::bson_operator_outter<c_mongo_read_concern> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_mongo_read_concern) val)
//{
//	const c_mongo_read_concern& right_cref = val;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	
inline void swap(c_mongo_read_concern& l, c_mongo_read_concern& r) 
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_READ_CONCERN_HPP__
