//c_mongo_write_concern.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_HPP__

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>
#include <yggr/container/empty.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/mongoc_write_concern_native_ex.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_enum.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_write_concern_t 
//{
//   int8_t fsync_; /* deprecated */
//   int8_t journal;
//   int32_t w;
//   int64_t wtimeout;
//   char *wtag;
//   bool frozen;
//   bson_t compiled;
//   bool is_default;
//};

//#define MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED 0
//#define MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED -1 /* deprecated */
//#define MONGOC_WRITE_CONCERN_W_DEFAULT -2
//#define MONGOC_WRITE_CONCERN_W_MAJORITY -3
//#define MONGOC_WRITE_CONCERN_W_TAG -4

class c_mongo_write_concern 
	: protected yggr_mongoc_write_concern
{

public:
	YGGR_STATIC_CONSTANT(
		s8, 
		E_MONGOC_WRITE_CONCERN_FSYNC_DEFAULT = 
			mongoc_write_concern_native_ex::E_MONGOC_WRITE_CONCERN_FSYNC_DEFAULT);

	YGGR_STATIC_CONSTANT(
		s8, 
		E_MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT = 
			mongoc_write_concern_native_ex::E_MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT);

public:
	YGGR_STATIC_CONSTANT(
		s32, 
		E_MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED = 
			mongoc_write_concern_native_ex::E_MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED);
	YGGR_STATIC_CONSTANT(
		s32, 
		E_MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED = 
			mongoc_write_concern_native_ex::E_MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED); // deprecated
	YGGR_STATIC_CONSTANT(
		s32, 
		E_MONGOC_WRITE_CONCERN_W_DEFAULT = 
			mongoc_write_concern_native_ex::E_MONGOC_WRITE_CONCERN_W_DEFAULT);
	YGGR_STATIC_CONSTANT(
		s32, 
		E_MONGOC_WRITE_CONCERN_W_MAJORITY = 
			mongoc_write_concern_native_ex::E_MONGOC_WRITE_CONCERN_W_MAJORITY);
	YGGR_STATIC_CONSTANT(
		s32, 
		E_MONGOC_WRITE_CONCERN_W_TAG = 
			mongoc_write_concern_native_ex::E_MONGOC_WRITE_CONCERN_W_TAG);

public:
	typedef yggr_mongoc_write_concern base_type;
	typedef mongoc_write_concern_t org_type;

	typedef c_bson bson_type;

	typedef utf8_string inner_string_type;
	typedef utf8_string_view inner_string_view_type;

private:
	typedef c_mongo_write_concern this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			s8,
			bool,
			bool
		> s8_member_var_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			s32,
			s32,
			s32
		> s32_member_var_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			s64,
			s64, 
			s64
		> s64_member_var_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			s64,
			s32,
			bool
		> wtimeout32_member_var_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			char*,
			inner_string_view_type,
			inner_string_view_type
		> member_tag_modify_helper_type;

public:
	c_mongo_write_concern(void);
	explicit c_mongo_write_concern(s32 w_count);

	template<typename String> 
	explicit c_mongo_write_concern(const String& wtag,
									typename
										boost::enable_if
										<
											boost::mpl::and_
											<
												// gcc and cpp version < cpp11  YGGR_STATIC_CONSTANT is enum
												boost::mpl::not_< boost::is_enum<String> >,
												charset::utf8_string_constructable<String>
											>,
											mplex::sfinae_type
										>::type sfinae = 0)
	{
		mongoc_write_concern_native_ex::s_mongoc_write_concern_init(this);
		this_type::pro_set_var_wtag_tpl(wtag);
	}

	explicit c_mongo_write_concern(const org_type* ptr);
	c_mongo_write_concern(const org_type& right);

	c_mongo_write_concern(BOOST_RV_REF(this_type) right)
	{
		mongoc_write_concern_native_ex::s_mongoc_write_concern_init(this);
		this_type& right_ref = right;
		mongoc_write_concern_native_ex::s_mongoc_write_concern_move(this, boost::addressof(right_ref));
	}

	c_mongo_write_concern(const this_type& right);
	~c_mongo_write_concern(void);

public:
	this_type& operator=(const org_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		mongoc_write_concern_native_ex::s_mongoc_write_concern_move(this, boost::addressof(right_ref));
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

public:
	inline bool validate(void) const
	{
		return mongoc_write_concern_native_ex::s_mongoc_write_concern_is_valid(this);
	}

protected:
	inline bool pro_get_var_fsync(void) const
	{
		return base_type::fsync_ == static_cast<s8>(1);
	}

	inline void pro_set_var_fsync(bool nsync)
	{
		mongoc_write_concern_native_ex::s_mongoc_write_concern_set_fsync(this, nsync);
	}

public:
	inline s8_member_var_modify_helper_type var_fsync(void)
	{
		return 
			s8_member_var_modify_helper_type(
				*this, base_type::fsync_, 
				&this_type::pro_set_var_fsync,
				&this_type::pro_get_var_fsync);
	}

	inline bool var_fsync(void) const
	{
		return this_type::pro_get_var_fsync();
	}

protected:
	inline bool pro_get_var_journal(void) const
	{
		return base_type::journal == static_cast<s8>(1);
	}

	inline void pro_set_var_journal(bool njournal)
	{
		mongoc_write_concern_native_ex::s_mongoc_write_concern_set_journal(this, njournal);
	}

public:
	inline s8_member_var_modify_helper_type var_journal(void)
	{
		return 
			s8_member_var_modify_helper_type(
				*this, base_type::journal,
				&this_type::pro_set_var_journal,
				&this_type::pro_get_var_journal);
	}

	inline bool var_journal(void) const
	{
		return this_type::pro_get_var_journal();
	}

protected:
	inline s32 pro_get_var_w(void) const
	{
		return base_type::w;
	}

	inline void pro_set_var_w(s32 nw)
	{
		mongoc_write_concern_native_ex::s_mongoc_write_concern_set_w(this, nw);
	}

public:
	inline s32_member_var_modify_helper_type var_w(void)
	{
		return 
			s32_member_var_modify_helper_type(
				*this, base_type::w, 
				&this_type::pro_set_var_w,
				&this_type::pro_get_var_w);
	}

	inline s32 var_w(void) const
	{
		return this_type::pro_get_var_w();
	}

protected:
	inline s64 pro_get_var_wtimeout(void) const
	{
		return mongoc_write_concern_native_ex::s_mongoc_write_concern_get_wtimeout_int64(this);
	}

	inline void pro_set_var_wtimeout(s64 nwtimeout)
	{
		mongoc_write_concern_native_ex::mongoc_write_concern_set_wtimeout_int64(this, nwtimeout);
	}

public:
	inline s64_member_var_modify_helper_type var_wtimeout(void)
	{
		return 
			s64_member_var_modify_helper_type(
				*this, base_type::wtimeout,
				&this_type::pro_set_var_wtimeout,
				&this_type::pro_get_var_wtimeout);
	}

	inline s64 var_wtimeout(void) const
	{
		return this_type::pro_get_var_wtimeout();
	}

protected:
	inline bool pro_get_var_wmajority(void) const
	{
		return base_type::w == MONGOC_WRITE_CONCERN_W_MAJORITY;
	}

	inline void pro_set_var_wmajority(s32 nwtimeout)
	{
		mongoc_write_concern_native_ex::s_mongoc_write_concern_set_wmajority(this, nwtimeout);
	}

public:
	inline wtimeout32_member_var_modify_helper_type var_wmajority(void)
	{
		return 
			wtimeout32_member_var_modify_helper_type(
				*this, base_type::wtimeout,
				&this_type::pro_set_var_wmajority,
				&this_type::pro_get_var_wmajority);
	}

	inline bool var_wmajority(void) const
	{
		return this_type::pro_get_var_wmajority();
	}

protected:
	inline inner_string_view_type pro_get_var_wtag(void) const
	{
		return inner_string_view_type(base_type::wtag);
	}

	inline void pro_set_var_wtag(const inner_string_view_type& str_wtag)
	{
		mongoc_write_concern_native_ex::s_mongoc_write_concern_set_wtag(
			this, str_wtag.data(), str_wtag.size());
	}

	template<typename String> inline
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			void
		>::type
		pro_set_var_wtag_tpl(const String& str_wtag)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type utf8_str_wtag((charset::string_charset_helper_data(str_wtag)));

		mongoc_write_concern_native_ex::s_mongoc_write_concern_set_wtag(
			this, utf8_str_wtag.data(), utf8_str_wtag.size());
	}

public:
	inline member_tag_modify_helper_type var_wtag(void)
	{
		return 
			member_tag_modify_helper_type(
				*this, 
				base_type::wtag,
				&this_type::pro_set_var_wtag,
				&this_type::pro_get_var_wtag);
	}

	inline inner_string_view_type var_wtag(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return this_type::pro_get_var_wtag();
	}

	//template<typename Alloc> inline
	//utf8_string<char, traits, Alloc> 
	//	var_wtag(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//String var_wtag(const string& ret_charset_name = charset::charset_name_t<String>()) const;

	//template<typename String, typename Alloc> inline
	//typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
	//	var_wtag(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const;

	YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET(var_wtag)
	
public:
	inline const bson_type& freeze_compiled(void)
	{
		return 
			*(reinterpret_cast<const bson_type*>(
				mongoc_write_concern_native_ex::s_mongoc_write_concern_get_bson(this)));
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			const Bson&
		>::type
		freeze_compiled(void)
	{
		typedef Bson now_bson_type;

		return 
			*(reinterpret_cast<const now_bson_type*>(
				mongoc_write_concern_native_ex::s_mongoc_write_concern_get_bson(this)));
	}

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

		return *(reinterpret_cast<const now_bson_type*>(boost::addressof(base_type::compiled)));
	}

public:
	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_type::org_type, Bson>,
			bool
		>::type
		append_to(Bson& cmd) const
	{
		return
			mongoc_write_concern_native_ex::s_mongoc_write_concern_append(
				const_cast<this_type*>(this), static_cast<bson_type::org_type*>(boost::addressof(cmd)));
	}

public:
	inline bool freeze(void)
	{
		return mongoc_write_concern_native_ex::s_mongoc_write_concern_freeze(this);
	}

	inline bool unfreeze(void)
	{
		return mongoc_write_concern_native_ex::s_mongoc_write_concern_unfreeze(this);
	}

	inline bool is_frozen(void) const
	{
		return base_type::frozen;
	}

public:
	inline bool is_default(void) const
	{
		return base_type::is_default_v;
	}

	inline bool journal_is_set(void) const
	{
		 return base_type::journal != this_type::E_MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT;
	}

	inline bool is_acknowledged(void) const
	{
		return 
			(((base_type::w != this_type::E_MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED) 
				&& (base_type::w != this_type::E_MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED)) 
			|| (base_type::fsync_ == static_cast<s8>(1))
			|| (base_type::journal == static_cast<s8>(1) ) );
	}
};

// non-member function
// io

namespace detail
{

template<>
struct bson_operator_outter<c_mongo_write_concern>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_mongo_write_concern& val, 
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"fsync_\" : " << val.var_fsync() 
			<< ", \"journal\" : " << (val.var_journal()? "true" : "false");

		switch(val.var_w())
		{
		case c_mongo_write_concern::E_MONGOC_WRITE_CONCERN_W_DEFAULT:
			// nothing out
			break;
		case c_mongo_write_concern::E_MONGOC_WRITE_CONCERN_W_MAJORITY:
			os << ", \"w\" : \"majority\"";
			break;
		case c_mongo_write_concern::E_MONGOC_WRITE_CONCERN_W_TAG:
			os << ", \"w\" : \"" << val.var_wtag() << "\"";
			break;
		default:
			os << ", \"w\" : " << val.var_w();
		}

		os << ", \"wtimeout\" : " << val.var_wtimeout()
			<< ", \"frozen\" : " << (val.is_frozen()? "true" : "false") 
			<< ", \"compiled\" : " << val.var_compiled()
			<< ", \"is_default\" : " << val.is_default()
			<< " }";
		return os;
	}
};

} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_write_concern& val)
{
	typedef detail::bson_operator_outter<c_mongo_write_concern> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_mongo_write_concern) val)
//{
//	const c_mongo_write_concern& right_cref = val;
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

inline void swap(c_mongo_write_concern& l, c_mongo_write_concern& r)
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

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_HPP__
