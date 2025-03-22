//c_bson_dbpointer.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DBPOINTER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DBPOINTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/ppex/friend.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>

#include <yggr/nsql_database_system/c_bson_string_item.hpp>

#include <yggr/nsql_database_system/bson_oid_native_ex.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/json_escape_string.hpp>

#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <cassert>

namespace yggr
{
namespace nsql_database_system
{

template<typename BsonOid>
class c_bson_dbpointer;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<typename BsonOid>
struct bson_typeid< c_bson_dbpointer<BsonOid> >
{
	BOOST_MPL_ASSERT((boost::is_base_of<bson_oid_t, BsonOid>));

	typedef c_bson_dbpointer<BsonOid> type;
	typedef c_bson_dbpointer<BsonOid> wrap_type;

	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type, 
		value = bson_typeid_def::E_BSON_TYPE_DBPOINTER);

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

template<typename BsonOid>
class c_bson_dbpointer
	: public c_bson_string_item
{
public:
	typedef c_bson_string_item base_type;
	typedef base_type::string_type string_type;

	typedef BsonOid oid_type;

private:
	typedef c_bson_dbpointer this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

	BOOST_MPL_ASSERT((boost::is_base_of<bson_oid_t, oid_type>));

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_bson_dbpointer(void)
	{
		bson_oid_init(&_oid, 0);
	}

	c_bson_dbpointer(const bson_value_t& val)
	{
		memset(&_oid, 0, sizeof(oid_type));
		this_type::pro_assign_bson_value_t(val);
	}

	template<typename String, typename Compatible_Oid>
	c_bson_dbpointer(const String& str_col, const Compatible_Oid& oid,
						typename
							boost::enable_if
							<
								charset::is_convertible_utf8_string< String >,
								sfinae_type
							>::type sfinae = 0)
		: base_type(str_col)
	{
		BOOST_MPL_ASSERT((boost::mpl::bool_<(bson_oid_native_ex::E_bson_oid_byte_size == sizeof(Compatible_Oid))>));
		memcpy(boost::addressof(_oid), boost::addressof(oid), bson_oid_native_ex::E_bson_oid_byte_size);
	}

	template<typename Compatible_Oid>
	c_bson_dbpointer(const c_bson_dbpointer<Compatible_Oid>& right)
		: base_type(right)
	{
		BOOST_MPL_ASSERT((boost::mpl::bool_<(bson_oid_native_ex::E_bson_oid_byte_size == sizeof(Compatible_Oid))>));
		memcpy(boost::addressof(_oid), boost::addressof(right.oid()), bson_oid_native_ex::E_bson_oid_byte_size);
	}

	c_bson_dbpointer(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
		this_type& right_ref = right;
		memcpy(boost::addressof(_oid), boost::addressof(right_ref._oid), bson_oid_native_ex::E_bson_oid_byte_size);
	}

	c_bson_dbpointer(const this_type& right)
		: base_type(right)
	{
		memcpy(boost::addressof(_oid), boost::addressof(right._oid), bson_oid_native_ex::E_bson_oid_byte_size);
	}

	~c_bson_dbpointer(void)
	{
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	template<typename Compatible_Oid> inline
	this_type& operator=(const c_bson_dbpointer<Compatible_Oid>& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::bool_<(bson_oid_native_ex::E_bson_oid_byte_size == sizeof(Compatible_Oid))>));
		base_type::operator=(right);
		memcpy(boost::addressof(_oid), boost::addressof(right.oid()), bson_oid_native_ex::E_bson_oid_byte_size);
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
		memcpy(&_oid, &right_ref._oid, sizeof(oid_type));
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}
		base_type::operator=(right);
		memcpy(&_oid, &right._oid, sizeof(oid_type));
		return *this;
	}

public:
	template<typename Compatible_Oid> inline
	void swap(BOOST_RV_REF(c_bson_dbpointer<Compatible_Oid>) right)
	{
		c_bson_dbpointer<Compatible_Oid>& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Compatible_Oid> inline
	void swap(c_bson_dbpointer<Compatible_Oid>& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::bool_<(bson_oid_native_ex::E_bson_oid_byte_size == sizeof(Compatible_Oid))>));

		base_type::swap(right);
		oid_type tmp;
		memcpy(boost::addressof(tmp), boost::addressof(_oid), bson_oid_native_ex::E_bson_oid_byte_size);
		memcpy(boost::addressof(_oid), boost::addressof(right.oid()), bson_oid_native_ex::E_bson_oid_byte_size);
		memcpy(boost::addressof(right.oid()), boost::addressof(tmp), bson_oid_native_ex::E_bson_oid_byte_size);
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
		oid_type tmp;
		memcpy(&tmp, &_oid, sizeof(oid_type));
		memcpy(&_oid, &right._oid, sizeof(oid_type));
		memcpy(&right._oid, &tmp, sizeof(oid_type));
	}

protected:
	inline void pro_assign_bson_value_t(const bson_value_t& val)
	{
		bool bsuccess = bson_value_native_ex::s_value_to_dbpointer(base_type::_str, &_oid, &val);
		assert(bsuccess);
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_DBPOINTER;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_DBPOINTER;
	}

public:
	template<typename BsonValue> inline
	bool compare_eq(const BsonValue& val) const
	{
		return 
			bson_value_native_ex::s_value_cmp_eq_dbpointer(
				boost::addressof(val), base_type::_str, &_oid);
	}

	template<typename Bson_Oid2> inline
	bool compare_eq(const c_bson_dbpointer<Bson_Oid2>& right) const
	{
		typedef Bson_Oid2 right_oid_type;
		typedef c_bson_dbpointer<Bson_Oid2> t2_type;

		BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(oid_type) == sizeof(right_oid_type)>));

		return 
			base_type::compare_eq(right)
			&& 0 == memcmp(&_oid, &(right.oid()), sizeof(oid_type));
	}

	inline bool compare_eq(const this_type& right) const
	{
		return 
			this == boost::addressof(right)
			|| (base_type::compare_eq(right)
					&& 0 == memcmp(&_oid, &right._oid, sizeof(oid_type)));
	}

public:
	inline string_type& collection(void)
	{
		return base_type::_str;
	}

	// inline const string_type& collection(void) const;
	
	// template<typename ...Args> inline 
	// const string_type& collection(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String collection(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(string_type, base_type::_str, collection, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(string_type, base_type::_str, collection, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(string_type, base_type::_str, collection)

public:
	inline oid_type& oid(void)
	{
		return _oid;
	}

	template<typename Oid> inline 
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_oid_t, Oid>,
			Oid&
		>::type
		oid(void)
	{
		return reinterpret_cast<Oid&>(_oid);
	}


	inline const oid_type& oid(void) const
	{
		return _oid;
	}

	template<typename Oid> inline 
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_oid_t, Oid>,
			const Oid&
		>::type
		scope(void) const
	{
		return reinterpret_cast<const Oid&>(_oid);
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
		return bson_append_dbpointer(&bs, str_name.data(), str_name.size(),
										base_type::_str.data(), &_oid);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		const bson_oid_t& oid_cref = _oid;
		return 
			bson_native_ex::s_bson_save_value_of_dbpointer(
				&bs, base_type::_str.data(), base_type::_str.size(), &oid_cref);
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
		load(BsonIter& iter, const String& name)
	{
		assert(iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_typeid< typename boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		uint32_t col_len = 0;
		const char* pcol = 0;
		const bson_oid_t* poid = 0;
		bson_iter_dbpointer(&iter, &col_len, &pcol, &poid);

		if(!(col_len && pcol && poid))
		{
			return;
		}

		base_type::_str.assign(pcol, pcol + col_len, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		memcpy(&_oid, poid, sizeof(oid_type));
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		uint32_t col_len = 0;
		const char* pcol = 0;
		const bson_oid_t* poid = 0;
		bson_iter_dbpointer(&iter, &col_len, &pcol, &poid);

		if(!(col_len && pcol && poid))
		{
			return;
		}

		base_type::_str.assign(pcol, pcol + col_len, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		memcpy(&_oid, poid, sizeof(oid_type));
	}

private:
	oid_type _oid;
};

// non member operator foo

template<typename Bson_Oid1, typename Bson_Oid2> inline
bool operator==(const c_bson_dbpointer<Bson_Oid1>& l, const c_bson_dbpointer<Bson_Oid2>& r)
{
	return l.compare_eq(r);
}

template<typename Bson_Oid1, typename Bson_Oid2> inline 
bool operator!=(const c_bson_dbpointer<Bson_Oid1>& l, const c_bson_dbpointer<Bson_Oid2>& r)
{
	return !l.compare_eq(r);
}

template<typename BsonOid> inline
bool operator==(const c_bson_dbpointer<BsonOid>& l, const c_bson_dbpointer<BsonOid>& r)
{
	return l.compare_eq(r);
}

template<typename BsonOid> inline 
bool operator!=(const c_bson_dbpointer<BsonOid>& l, const c_bson_dbpointer<BsonOid>& r)
{
	return !l.compare_eq(r);
}

// ==

template<typename BsonValue, typename BsonOid> inline
bool operator==(const BsonValue& l, const c_bson_dbpointer<BsonOid>& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue, typename BsonOid> inline
bool operator==(const c_bson_dbpointer<BsonOid>& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

//template<typename BsonOid> inline
//bool operator==(const bson_value_t& l, const c_bson_dbpointer<BsonOid>& r)
//{
//	typedef c_bson_dbpointer<BsonOid> dbpointer_type;
//	return 
//		bson_value_native_ex::s_value_cmp_eq_dbpointer(
//			&l, r.template collection<typename dbpointer_type::string_type>(), &(r.oid()));
//}
//
//template<typename BsonOid> inline
//bool operator==(const c_bson_dbpointer<BsonOid>& l, const bson_value_t& r)
//{
//	typedef c_bson_dbpointer<BsonOid> dbpointer_type;
//	return 
//		bson_value_native_ex::s_value_cmp_eq_dbpointer(
//			&r, l.template collection<typename dbpointer_type::string_type>(), &(l.oid()));
//}

// !=
template<typename BsonValue, typename BsonOid> inline
bool operator!=(const BsonValue& l, const c_bson_dbpointer<BsonOid>& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue, typename BsonOid> inline
bool operator!=(const c_bson_dbpointer<BsonOid>& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}


//template<typename BsonOid> inline
//bool operator!=(const bson_value_t& l, const c_bson_dbpointer<BsonOid>& r)
//{
//	typedef c_bson_dbpointer<BsonOid> dbpointer_type;
//	return 
//		!bson_value_native_ex::s_value_cmp_eq_dbpointer(
//			&l, r.template collection<typename dbpointer_type::string_type>(), &(r.oid()));
//}
//
//template<typename BsonOid> inline
//bool operator!=(const c_bson_dbpointer<BsonOid>& l, const bson_value_t& r)
//{
//	typedef c_bson_dbpointer<BsonOid> dbpointer_type;
//	return 
//		!bson_value_native_ex::s_value_cmp_eq_dbpointer(
//			&r, l.template collection<typename dbpointer_type::string_type>(), &(l.oid()));
//}


namespace detail
{

template<typename BsonOid>
struct bson_operator_outter< c_bson_dbpointer<BsonOid> >
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_bson_dbpointer<BsonOid>& val, 
					bson_json_mode_def::mode_type mode) const
	{
		typedef c_bson_dbpointer<BsonOid> dbpointer_type;
		typedef typename dbpointer_type::string_type now_string_type;

		yggr::string oid_str(sizeof(BsonOid) << 1, 0);
		bson_oid_to_string(&(val.oid()), &(oid_str[0]));

		const now_string_type& str_collection = val.collection();
		json_escape_string collection_escaped(str_collection.data(), str_collection.size());

		switch(mode)
		{
		case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
		case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
			os << "{ \"$dbPointer\" : { \"$ref\" : " << collection_escaped
				<< ", \"$id\" : \"" << oid_str 
				<< "\" } }";
			break;

		default:
			os << "{ \"$ref\" : " << collection_escaped
				<< ", \"$id\" : \"" << oid_str 
				<< "\" }";
		}

		return os;
	}
};


} // namespace detail

template<typename Char, typename Traits, typename BsonOid> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_dbpointer<BsonOid>& val)
{
	typedef c_bson_dbpointer<BsonOid> value_type;
	typedef detail::bson_operator_outter<value_type> outter_type;

	outter_type outter;
	return outter(os, val, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits, typename BsonOid>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os,
//				BOOST_RV_REF_BEG c_bson_dbpointer<BsonOid> BOOST_RV_REF_END val)
//{
//	const c_bson_dbpointer<BsonOid>& right_cref = val;
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

template<typename Bson_Oid1, typename Bson_Oid2> inline 
void swap(c_bson_dbpointer<Bson_Oid1>& l, c_bson_dbpointer<Bson_Oid2>& r)
{ 
	l.swap(r);
}

template<typename BsonOid> inline 
void swap(c_bson_dbpointer<BsonOid>& l, c_bson_dbpointer<BsonOid>& r)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DBPOINTER_HPP__
