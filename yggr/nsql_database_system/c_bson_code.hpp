//c_bson_code.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_CODE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_CODE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/mplex/static_assert.hpp>
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

class c_bson_code;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_TYPEID_IMPL(c_bson_code, bson_typeid_def::E_BSON_TYPE_CODE)

namespace yggr
{
namespace nsql_database_system
{

class c_bson_code
	: public c_bson_string_item
{
public:
	typedef c_bson_string_item base_type;
	typedef base_type::string_type string_type;

private:
	typedef c_bson_code this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_bson_code(void);
	c_bson_code(const bson_value_t& val);

	c_bson_code(const char* code_str);

#ifndef YGGR_NO_CWCHAR
	c_bson_code(const wchar_t* code_str);
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
	c_bson_code(const c8* code_str);
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
	c_bson_code(const c16* code_str);
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
	c_bson_code(const c32* code_str);
#endif // YGGR_NO_CHAR32_T

	template<typename Char>
	c_bson_code(const Char* code_str,
				typename
					boost::enable_if
					<
						is_native_char< Char >,
						sfinae_type
					>::type sfinae = 0)
		: base_type(code_str)
	{
	}

	template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class Basic_StringView>
	c_bson_code(const Basic_StringView<Char, Traits>& code_str)
		: base_type(code_str)
	{
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
	c_bson_code(const Basic_String<Char, Traits, Alloc>& code_str)
		: base_type(code_str)
	{
	}

	template<typename T1, typename T2>
	c_bson_code(const charset::string_charset_helper<T1, T2>& code_str)
		: base_type(code_str)
	{
	}

	c_bson_code(BOOST_RV_REF(string_type) code_str)
		: base_type(code_str)
	{
	}

	c_bson_code(const string_type& code_str)
		: base_type(code_str)
	{
	}

	c_bson_code(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	c_bson_code(const this_type& right);
	~c_bson_code(void);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	template<typename Char> inline
	typename boost::enable_if< is_native_char<Char>, this_type&>::type
		operator=(const Char* code_str)
	{
		base_type::operator=(code_str);
		return *this;
	}

	template<typename String> inline
	typename boost::enable_if<charset::is_convertible_utf8_string< String >, this_type&>::type
		operator=(const String& code_str)
	{
		base_type::operator=(code_str);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(string_type) right)
	{
		base_type::operator=(boost::move(right));
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
		return bson_value_native_ex::s_value_cmp_eq_code(&val, this_type::code());
	}

	inline bool compare_eq(const this_type& right) const
	{
		return base_type::compare_eq(right);
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_CODE;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_CODE;
	}

public:
	inline string_type& code(void)
	{
		return base_type::_str;
	}

	// inline const string_type& code(void) const;
	
	// template<typename ...Args> inline 
	// const string_type& code(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String code(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(string_type, base_type::_str, code, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(string_type, base_type::_str, code, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(string_type, base_type::_str, code)

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
		return bson_append_code(&bs, str_name.data(), str_name.size(), base_type::_str.data());
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		return bson_native_ex::s_bson_save_value_of_code(&bs, base_type::_str.data(), base_type::_str.size());
	}

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
		u32 len = 0;
		const char* pdata = bson_iter_code(&iter, &len);
		if(pdata)
		{
			base_type::_str.assign(pdata, pdata + len, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		u32 len = 0;
		const char* pdata = bson_iter_code(&iter, &len);
		if(pdata)
		{
			base_type::_str.assign(pdata, pdata + len, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}
	}
};

// non member operator foo
inline bool operator==(const c_bson_code& l, const c_bson_code& r)
{
	return l.compare_eq(r);
}

inline bool operator!=(const c_bson_code& l, const c_bson_code& r)
{
	return !l.compare_eq(r);
}

// code
// == 

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_code& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_code& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

// !=

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_code& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_code& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

namespace detail
{

template<>
struct bson_operator_outter<c_bson_code>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_bson_code& val, 
					bson_json_mode_def::mode_type) const
	{
		/*
		old_version:
			os << "\"" << val.value.v_code.code<< "\""; // old_version
		new_version:
			bson_string_append(state->str, "{ \"$code\" : \"");
			bson_string_append(state->str, escaped);
			bson_string_append(state->str, "\" }");
		*/

		typedef c_bson_code::string_type now_string_type;

		const now_string_type& str = val.code();
		json_escape_string out_str(str.c_str(), str.size());
		os << "{ \"$code\" : " << out_str << " }";
		return os;
	}
};


} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_code& val)
{
	typedef detail::bson_operator_outter<c_bson_code> outter_type;
	
	outter_type outter;
	return outter(os, val, 0);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>& 
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_bson_code) val)
//{
//	os << "\"" << val.code<yggr::utf8_string>() << "\"";
//	return os;
//}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_bson_code)

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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_CODE_HPP__
