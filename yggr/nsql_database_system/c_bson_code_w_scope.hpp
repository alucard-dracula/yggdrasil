//c_bson_code_w_scope.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_CODE_W_SCOPE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_CODE_W_SCOPE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>

#include <yggr/nsql_database_system/c_bson_string_item.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/json_escape_string.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <cassert>
#include <algorithm>

namespace yggr
{
namespace nsql_database_system
{

template<typename Scope>
class c_bson_code_w_scope;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<typename Scope>
struct bson_typeid< c_bson_code_w_scope<Scope> >
{
	YGGR_ASSERT_BSON_TYPE(Scope);

	typedef c_bson_code_w_scope<Scope> type;
	typedef c_bson_code_w_scope<Scope> wrap_type;

	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type,
		value = bson_typeid_def::E_BSON_TYPE_CODEWSCOPE);

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const
	{
		return static_cast<u8>(value);
	}

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const
	{
		return static_cast<u8>(value);
	}
};

} // namespace nsql_database_system
} // namesapce yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<typename Scope,
		bool is_bson_t
			= boost::is_same<bson_t, Scope>::value>
struct c_bson_code_w_scope_bson_op_helper;

template<typename Scope>
struct c_bson_code_w_scope_bson_op_helper<Scope, true>
{
	typedef Scope scope_type;

	inline void init(scope_type& scope) const { ::bson_init(&scope); }
	inline void destroy(scope_type& scope) const { ::bson_destroy(&scope); }
};

template<typename Scope>
struct c_bson_code_w_scope_bson_op_helper<Scope, false>
{
	typedef Scope scope_type;

	inline void init(scope_type& scope) const {}
	inline void destroy(scope_type& scope) const {}
};


} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<typename Scope>
class c_bson_code_w_scope
	: public c_bson_string_item
{
	YGGR_ASSERT_BSON_TYPE(Scope);

public:
	typedef Scope scope_type;
	typedef c_bson_string_item base_type;
	typedef base_type::string_type string_type;

private:
	typedef c_bson_code_w_scope this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef detail::c_bson_code_w_scope_bson_op_helper<scope_type> bson_op_helper_type;

public:
	c_bson_code_w_scope(void)
	{
		bson_op_helper_type h;
		h.init(_scope);
	}

	c_bson_code_w_scope(const bson_value_t& val)
	{
		this_type::pro_assign_bson_value_t(val);
	}

	template<typename String, typename Compatible_Scope>
	c_bson_code_w_scope(const String& code, const Compatible_Scope& scope,
						typename
							boost::enable_if
							<
								boost::mpl::and_
								<
									charset::is_convertible_utf8_string< String >,
									boost::is_base_of<bson_t, Compatible_Scope>
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: base_type(code)
	{
		//YGGR_ASSERT_BSON_TYPE(Compatible_Scope);
		if(bson_native_ex::s_bson_is_nil(&scope))
		{
			bson_op_helper_type h;
			h.init(_scope);
		}
		else
		{
			bson_native_ex::s_bson_copy(&_scope, &scope);
		}
	}

	template<typename Compatible_Scope>
	c_bson_code_w_scope(BOOST_RV_REF(string_type) code, const Compatible_Scope& scope,
							typename
								boost::enable_if
								<
									boost::is_base_of<bson_t, Compatible_Scope>,
									mplex::sfinae_type
								>::type sfinae = 0)
		: base_type(boost::move(code))
	{
		if(bson_native_ex::s_bson_is_nil(&scope))
		{
			bson_op_helper_type h;
			h.init(_scope);
		}
		else
		{
			bson_native_ex::s_bson_copy(&_scope, &scope);
		}
	}

	template<typename Compatible_Scope>
	c_bson_code_w_scope(const c_bson_code_w_scope<Compatible_Scope>& right)
		: base_type(right)
	{
		bson_native_ex::s_bson_copy(&_scope, &(right.scope()));
	}

	c_bson_code_w_scope(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
		bson_op_helper_type h;
		h.init(_scope);

		this_type& right_ref = right;
		bson_native_ex::s_bson_swap(&_scope, &right_ref._scope);
	}

	c_bson_code_w_scope(const this_type& right)
		: base_type(right)
	{
		bson_native_ex::s_bson_copy(&_scope, &right._scope);
	}

	~c_bson_code_w_scope(void)
	{
		bson_op_helper_type h;
		h.destroy(_scope);
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	template<typename Compatible_Scope> inline
	this_type& operator=(const c_bson_code_w_scope<Compatible_Scope>& right)
	{
		base_type& l = *this;
		const base_type& r = right;
		l = r;
		bson_native_ex::s_bson_copy(&_scope, &(right.scope()));
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		this_type& right_ref = right;
		base_type& base = *this;

		copy_or_move_or_swap(base, boost::move(static_cast<base_type&>(right)));
		bson_native_ex::s_bson_swap(&_scope, &right_ref._scope);

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::operator=(static_cast<const base_type&>(right));
		bson_native_ex::s_bson_copy(&_scope, &right._scope);

		return *this;
	}

public:
	template<typename Compatible_Scope> inline
	void swap(BOOST_RV_REF_BEG
					c_bson_code_w_scope<Compatible_Scope>
				BOOST_RV_REF_END right)
	{
		c_bson_code_w_scope<Compatible_Scope>& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Compatible_Scope> inline
	void swap(c_bson_code_w_scope<Compatible_Scope>& right)
	{
		base_type::swap(right);
		bson_native_ex::s_bson_swap(&_scope, &(right.scope()));
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		base_type::swap(right);
		bson_native_ex::s_bson_swap(
			boost::addressof(_scope), boost::addressof(right._scope));
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		return
			bson_value_native_ex::s_value_cmp_eq_code_w_scope(
				boost::addressof(val),
				this_type::code(),
				boost::addressof(this_type::scope()));
	}

	template<typename Scope2> inline
	bool compare_eq(const c_bson_code_w_scope<Scope2>& right) const
	{
		return
			base_type::compare_eq(right)
			&& ::bson_equal(&_scope, &(right.scope()));
	}

	inline bool compare_eq(const this_type& right) const
	{
		return
			base_type::compare_eq(right)
			&& ::bson_equal(&_scope, &right._scope);
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_CODEWSCOPE;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_CODEWSCOPE;
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

public:
	inline scope_type& scope(void)
	{
		return _scope;
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			Bson&
		>::type
		scope(void)
	{
		return reinterpret_cast<Bson&>(_scope);
	}

	inline const scope_type& scope(void) const
	{
		return _scope;
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>,
			const Bson&
		>::type
		scope(void) const
	{
		return reinterpret_cast<const Bson&>(_scope);
	}

protected:
	inline void pro_assign_bson_value_t(const bson_value_t& val)
	{
		bson_op_helper_type h;
		h.init(_scope);

		bool bsuccess = bson_value_native_ex::s_value_to_code_w_scope(base_type::_str, &_scope, &val);
		assert(bsuccess);
	}

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
			bson_append_code_with_scope(
				boost::addressof(bs), str_name.data(), str_name.size(),
				base_type::_str.data(), &_scope);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		return
			bson_native_ex::s_bson_save_value_of_code_w_scope(
				&bs,
				base_type::_str.data(),
				base_type::_str.size(), &_scope);
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
		assert(iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name));

		if(!iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		u32 code_len = 0;
		u32 scope_len = 0;
		const u8* pscope = 0;
		const char* pcode = bson_iter_codewscope(&iter, &code_len, &scope_len, &pscope);

		if(pcode)
		{
			base_type::_str.assign(pcode, pcode + code_len, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}

		if(pscope)
		{
			bson_destroy(&_scope);
			bson_native_ex::s_bson_init_from_data(&_scope, pscope, scope_len);
		}
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		u32 code_len = 0;
		u32 scope_len = 0;
		const u8* pscope = 0;
		const char* pcode = bson_iter_codewscope(&iter, &code_len, &scope_len, &pscope);

		if(pcode)
		{
			base_type::_str.assign(pcode, pcode + code_len, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		}

		if(pscope)
		{
			bson_destroy(&_scope);
			bson_native_ex::s_bson_init_from_data(&_scope, pscope, scope_len);
		}
	}

private:
	scope_type _scope;
};

// non member foo
template<typename Scope1, typename Scope2> inline
bool operator==(const c_bson_code_w_scope<Scope1>& l, const c_bson_code_w_scope<Scope2>& r)
{
	return l.compare_eq(r);
}

template<typename Scope1, typename Scope2> inline
bool operator!=(const c_bson_code_w_scope<Scope1>& l, const c_bson_code_w_scope<Scope2>& r)
{
	return !l.compare_eq(r);
}

template<typename Scope> inline
bool operator==(const c_bson_code_w_scope<Scope>& l, const c_bson_code_w_scope<Scope>& r)
{
	return l.compare_eq(r);
}

template<typename Scope> inline
bool operator!=(const c_bson_code_w_scope<Scope>& l, const c_bson_code_w_scope<Scope>& r)
{
	return !l.compare_eq(r);
}

// ==
template<typename BsonValue, typename Scope> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_code_w_scope<Scope>& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue, typename Scope> inline
bool operator==(const c_bson_code_w_scope<Scope>& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

//template<typename Scope> inline
//bool operator==(const bson_value_t& l, const c_bson_code_w_scope<Scope>& r)
//{
//	typedef c_bson_code_w_scope<Scope> code_w_scope_type;
//	return
//		bson_value_native_ex::s_value_cmp_eq_code_w_scope(
//			&l, r.template code<typename code_w_scope_type::string_type>(), &(r.scope()));
//}
//
//template<typename Scope> inline
//bool operator==(const c_bson_code_w_scope<Scope>& l, const bson_value_t& r)
//{
//	typedef c_bson_code_w_scope<Scope> code_w_scope_type;
//	return
//		bson_value_native_ex::s_value_cmp_eq_code_w_scope(
//			&r, l.template code<typename code_w_scope_type::string_type>(), &(l.scope()));
//}

// !=
template<typename BsonValue, typename Scope> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_code_w_scope<Scope>& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue, typename Scope> inline
bool operator!=(const c_bson_code_w_scope<Scope>& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

//template<typename Scope> inline
//bool operator!=(const bson_value_t& l, const c_bson_code_w_scope<Scope>& r)
//{
//	typedef c_bson_code_w_scope<Scope> code_w_scope_type;
//	return
//		!bson_value_native_ex::s_value_cmp_eq_code_w_scope(
//			&l, r.template code<typename code_w_scope_type::string_type>(), &(r.scope()));
//}
//
//template<typename Scope> inline
//bool operator!=(const c_bson_code_w_scope<Scope>& l, const bson_value_t& r)
//{
//	typedef c_bson_code_w_scope<Scope> code_w_scope_type;
//	return
//		!bson_value_native_ex::s_value_cmp_eq_code_w_scope(
//			&r, l.template code<typename code_w_scope_type::string_type>(), &(l.scope()));
//}

namespace detail
{

template<typename Scope>
struct bson_operator_outter< c_bson_code_w_scope<Scope> >
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_code_w_scope<Scope>& val,
					bson_json_mode_def::mode_type) const
	{
		typedef c_bson_code_w_scope<Scope> now_code_wscope_type;
		typedef typename now_code_wscope_type::string_type now_string_type;

		const now_string_type& str_code = val.code();

		json_escape_string out_str(str_code.data(), str_code.size());

		os << "{ \"$code\" : " << out_str << ", \"$scope\" : " << val.scope() << " }";
		return os;
	}
};


} // namespace detail

template<typename Char, typename Traits, typename Scope> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_code_w_scope<Scope>& val)
{
	typedef c_bson_code_w_scope<Scope> value_type;
	typedef detail::bson_operator_outter<value_type> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

// need test, if ok, remove it
//template<typename Char, typename Traits, typename Scope>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os,
//				BOOST_RV_REF_BEG c_bson_code_w_scope<Scope> BOOST_RV_REF_END val)
//{
//	os << "\"" << val.template code<yggr::utf8_string>() << "\"";
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

template<typename Scope1, typename Scope2> inline
void swap(c_bson_code_w_scope<Scope1>& l, c_bson_code_w_scope<Scope2>& r)
{
	l.swap(r);
}

template<typename Scope> inline
void swap(c_bson_code_w_scope<Scope>& l, c_bson_code_w_scope<Scope>& r)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_CODE_W_SCOPE_HPP__
