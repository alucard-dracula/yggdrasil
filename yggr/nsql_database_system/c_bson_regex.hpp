//c_bson_regex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_REGEX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_REGEX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>

#include <yggr/nsql_database_system/c_bson_string_item.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/json_escape_string.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <cassert>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_regex;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_TYPEID_IMPL(c_bson_regex, bson_typeid_def::E_BSON_TYPE_REGEX)

namespace yggr
{
namespace nsql_database_system
{

class c_bson_regex
	: public c_bson_string_item
{
public:
	typedef c_bson_string_item base_type;
	typedef base_type::string_type string_type;
	typedef string_type inner_string_type;

private:
	typedef c_bson_regex this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			inner_string_type,
			inner_string_type,
			const inner_string_type&
		> inner_string_member_modify_helper_type;

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_bson_regex(void);

	c_bson_regex(const bson_value_t& val);

	template<typename RegexStr, typename OptsStr>
	c_bson_regex(const RegexStr& regex_str, const OptsStr& opts,
					typename
						boost::enable_if
						<
							boost::mpl::and_
							<
								charset::is_convertible_utf8_string< RegexStr >,
								charset::is_convertible_utf8_string< OptsStr >
							>,
							sfinae_type
						>::type sfinae = 0)
		: base_type(regex_str)
	{
		this_type::pro_set_options_tpl(opts);
	}

	template<typename OptsStr>
	c_bson_regex(BOOST_RV_REF(string_type) regex_str, const OptsStr& opts,
					typename
						boost::enable_if
						<
							charset::is_convertible_utf8_string< OptsStr >,
							sfinae_type
						>::type sfinae = 0)
		: base_type(boost::move(regex_str))
	{
		this_type::pro_set_options_tpl(opts);
	}

	template<typename RegexStr>
	c_bson_regex(const RegexStr& regex_str, BOOST_RV_REF(string_type) opts,
					typename
						boost::enable_if
						<
							charset::is_convertible_utf8_string< RegexStr >,
							sfinae_type
						>::type sfinae = 0)
		: base_type(regex_str)
	{
		this_type::pro_set_options(opts);
	}

	c_bson_regex(BOOST_RV_REF(string_type) regex_str, BOOST_RV_REF(string_type) opts)
		: base_type(boost::move(regex_str))
	{
		this_type::pro_set_options(opts);
	}

	c_bson_regex(const string_type& regex_str, const string_type& opts)
		: base_type(regex_str), _opts(opts)
	{
		this_type::pro_set_options(opts);
	}

	c_bson_regex(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right))),
			_opts(boost::move(right._opts))
	{
	}

	c_bson_regex(const this_type& right);
	~c_bson_regex(void);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(static_cast<base_type&>(right)));
		copy_or_move_or_swap(_opts, boost::move(right._opts));

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

protected:
	void pro_assign_bson_value_t(const bson_value_t& val);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		return 
			bson_value_native_ex::s_value_cmp_eq_regex(
				boost::addressof(val), 
				base_type::_str, this_type::_opts);
	}

	inline bool compare_eq(const this_type& right) const
	{
		return 
			this == boost::addressof(right)
			|| (base_type::compare_eq(right)
				&& _opts == right._opts);
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_REGEX;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_REGEX;
	}

public:
	inline string_type& regex(void)
	{
		return base_type::_str;
	}

	// inline const string_type& regex(void) const;
	
	// template<typename ...Args> inline 
	// const string_type& regex(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String regex(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(string_type, base_type::_str, regex, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(string_type, base_type::_str, regex, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(string_type, base_type::_str, regex)

protected:
	inline const inner_string_type& pro_get_options(void) const
	{
		return _opts;
	}

	
	template<typename String> inline
	typename 
		boost::enable_if
		< 
			charset::utf8_string_constructable< String >, 
			void 
		>::type
		pro_set_options_tpl(const String& str_opts)
	{
		typedef String opts_string_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(opts_string_type, conv_opts_string_type);

		conv_opts_string_type mid_str((charset::string_charset_helper_data(str_opts)));

		char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
		std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, mid_str.data(), mid_str.size());
		_opts.assign(options_buffer, options_buffer + opts_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	inline void pro_set_options(const inner_string_type& str_opts)
	{
		char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
		std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, str_opts.data(), str_opts.size());
		_opts.assign(options_buffer, options_buffer + opts_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

public:
	inline inner_string_member_modify_helper_type options(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _opts,
				&this_type::pro_set_options,
				&this_type::pro_get_options);
	}

	// inline const inner_string_type& options(void) const;
	
	// template<typename ...Args> inline 
	// const inner_string_type& options(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String options(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_options(), options, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, this_type::pro_get_options(), options, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, this_type::pro_get_options(), options)

private:
	YGGR_PP_FRIEND_CLASS( bson_inner_data_accesser<true> );

	//save and load def
	template<typename Bson, typename String > inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson>,
				charset::utf8_string_constructable< String >
			>,
			bool
		>::type
		save(Bson& bs, const String& name) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(bson_native_ex::s_bson_is_nil(&bs))
		{
			bson_init(&bs);
		}

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return
			bson_append_regex(
				&bs, 
				str_name.data(), str_name.size(), 
				base_type::_str.data(), _opts.data());
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		return 
			bson_native_ex::s_bson_save_value_of_regex(
				&bs, 
				base_type::_str.data(), base_type::_str.size(),
				_opts.data(), _opts.size());
	}

	//template<typename BsonIter, typename Char> inline
	//typename 
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_iter_t, BsonIter>,
	//			is_native_char<Char>
	//		>, 
	//		void
	//	>::type
	//	load(const BsonIter& iter, const Char* name)
	//{
	//	assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));
	//	if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
	//	{
	//		return;
	//	}

	//	const char* pops = 0;
	//	const char* pval = 0;
	//	pval = bson_iter_regex(&iter, &pops);
	//	if(pval)
	//	{
	//		base_type::str().assign(pval, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	}

	//	if(pops)
	//	{
	//		char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
	//		std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, pops);
	//		_opts.assign(options_buffer, options_buffer + opts_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	}
	//}

	//template<typename BsonIter, typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	//typename 
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_iter_t, BsonIter>,
	//			charset::is_string_t< Basic_String<Char, Traits, Alloc> >
	//		>, 
	//		void
	//	>::type
	//	load(const BsonIter& iter, const Basic_String<Char, Traits, Alloc>& name)
	//{
	//	assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));
	//	if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
	//	{
	//		return;
	//	}

	//	const char* pops = 0;
	//	const char* pval = 0;
	//	pval = bson_iter_regex(&iter, &pops);
	//	if(pval)
	//	{
	//		base_type::str().assign(pval, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	}

	//	if(pops)
	//	{
	//		char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
	//		std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, pops);
	//		_opts.assign(options_buffer, options_buffer + opts_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	}
	//}

	template<typename BsonIter, typename String > inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable< String >
			>, 
			void
		>::type
		load(const BsonIter& iter, const String& name)
	{
		assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		const char* pops = 0;
		const char* pval = 0;
		pval = bson_iter_regex(&iter, &pops);
		if(pval)
		{
			base_type::str().assign(pval, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}

		if(pops)
		{
			char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
			std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, pops);
			_opts.assign(options_buffer, options_buffer + opts_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		const char* pops = 0;
		base_type::str() = bson_iter_regex(&iter, &pops);
		if(pops)
		{
			char options_buffer[bson_regex_native_ex::E_BSON_REGEX_OPTIONS_STRING_BUFFER_SIZE] = {0};
			std::size_t opts_size = bson_regex_native_ex::s_bson_regex_options_sorted(options_buffer, pops);
			_opts.assign(options_buffer, options_buffer + opts_size, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}
	}

private:
	string_type _opts;
};

// non member operator foo
inline bool operator==(const c_bson_regex& l, const c_bson_regex& r)
{
	return l.compare_eq(r);
}

inline bool operator!=(const c_bson_regex& l, const c_bson_regex& r)
{
	return !l.compare_eq(r);
}

// == 
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_regex& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_regex& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

// !=
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_regex& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_regex& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

namespace detail
{

template<>
struct bson_operator_outter<c_bson_regex>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_bson_regex& val, 
					bson_json_mode_def::mode_type mode) const
	{
		typedef c_bson_regex::string_type now_string_type;

		const now_string_type& str_regex = val.regex();
		json_escape_string escaped_str_regex(str_regex.data(), str_regex.size());

		switch(mode)
		{
		case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
		case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
			os << "{ \"$regularExpression\" : { \"pattern\" : "
				<< escaped_str_regex
				<< ", \"options\" : \""
				<< val.options()
				<< "\" } }";
			break;
		default:
			os << "{ \"$regex\" : " << escaped_str_regex 
				<< ", \"$options\" : \"" << val.options() << "\" }";
		}

		return os;
	}
};

} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_regex& val)
{
	typedef detail::bson_operator_outter<c_bson_regex> outter_type;

	outter_type outter;
	return outter(os, val, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_bson_regex) val)
//{
//	const c_bson_regex& right_cref = val;
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
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_bson_regex)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_REGEX_HPP__
