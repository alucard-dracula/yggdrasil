// c_bson_decimal128.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DECIMAL128_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DECIMAL128_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/friend.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/bson_decimal128_native_ex.hpp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>

#include <cassert>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_decimal128;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_WRAP_TYPEID_IMPL(bson_decimal128_t, c_bson_decimal128, bson_typeid_def::E_BSON_TYPE_DECIMAL128)
YGGR_PP_BSON_TYPEID_IMPL(c_bson_decimal128, bson_typeid_def::E_BSON_TYPE_DECIMAL128)

/*
typedef struct {
#if BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
   uint64_t low;
   uint64_t high;
#elif BSON_BYTE_ORDER == BSON_BIG_ENDIAN
   uint64_t high;
   uint64_t low;
#endif
} bson_decimal128_t;
*/

namespace yggr
{
namespace nsql_database_system
{

class c_bson_decimal128
	: public bson_decimal128_t
{
public:
	typedef bson_decimal128_t base_type;
	typedef base_type org_type;

public:
	typedef charset::utf8_string inner_string_type;
	typedef charset::utf8_string_view inner_string_view_type;

private:
	typedef c_bson_decimal128 this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_bson_decimal128(void);

	c_bson_decimal128(const char* str_num);

#ifndef YGGR_NO_CWCHAR
	c_bson_decimal128(const wchar_t* str_num);
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
	c_bson_decimal128(const c8* str_num);
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
	c_bson_decimal128(const c16* str_num);
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
	c_bson_decimal128(const c32* str_num);
#endif // YGGR_NO_CHAR32_T

	template<typename Char>
	c_bson_decimal128(const Char* str_num,
						typename
							boost::enable_if
							<
								is_native_char< Char >,
								sfinae_type
							>::type sfinae = 0)
		: base_type(str_num)
	{
		base_type::low = base_type::high = 0;
		this_type::pro_init_from_string(str_num);
	}

	template<typename Char>
	c_bson_decimal128(const Char* str_num, std::size_t n)
	{
		base_type::low = base_type::high = 0;
		this_type::pro_init_from_string(str_num, n);
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename Char, typename Traits, typename Alloc>
	c_bson_decimal128(const BasicString<Char, Traits, Alloc>& str_num)
	{
		base_type::low = base_type::high = 0;
		this_type::pro_init_from_string(str_num);
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename Char, typename Traits, typename Alloc>
	c_bson_decimal128(const BasicString<Char, Traits, Alloc>& str_num, std::size_t n)
	{
		base_type::low = base_type::high = 0;
		this_type::pro_init_from_string(str_num, n);
	}

	template<template<typename _C, typename _T> class BasicStringView,
				typename Char, typename Traits>
	c_bson_decimal128(const BasicStringView<Char, Traits>& str_num)
	{
		base_type::low = base_type::high = 0;
		this_type::pro_init_from_string(str_num);
	}

	template<template<typename _C, typename _T> class BasicStringView,
				typename Char, typename Traits>
	c_bson_decimal128(const BasicStringView<Char, Traits>& str_num, std::size_t n)
	{
		base_type::low = base_type::high = 0;
		this_type::pro_init_from_string(str_num, n);
	}

	template<typename T1, typename T2>
	c_bson_decimal128(const charset::string_charset_helper<T1, T2>& str_num)
	{
		base_type::low = base_type::high = 0;
		this_type::pro_init_from_string(str_num);
	}

	template<typename T1, typename T2>
	c_bson_decimal128(const charset::string_charset_helper<T1, T2>& str_num, std::size_t n)
	{
		base_type::low = base_type::high = 0;
		this_type::pro_init_from_string(str_num, n);
	}

	c_bson_decimal128(u64 nlow);
	c_bson_decimal128(u64 nhigh, u64 nlow);
	c_bson_decimal128(const bson_value_t& val);

	c_bson_decimal128(BOOST_RV_REF(org_type) right)
	{
		org_type& right_ref = right;
		base_type::low = right_ref.low;
		base_type::high = right_ref.high;
	}

	c_bson_decimal128(const org_type& right);

	c_bson_decimal128(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		base_type::low = right_ref.low;
		base_type::high = right_ref.high;
	}

	c_bson_decimal128(const this_type& right);
	~c_bson_decimal128(void);

public:
	this_type& operator=(u64 nlow);

	template<typename String> inline
	typename boost::enable_if<charset::is_convertible_utf8_string< String >, this_type&>::type
		operator=(const String& str_num)
	{
		this_type::pro_init_from_string(str_num);
		return *this;
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(org_type) right)
	{
		org_type& right_ref = right;
		if(static_cast<base_type*>(this) == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::low = right_ref.low;
		base_type::high = right_ref.high;

		return *this;
	}

protected:
	this_type& pro_operator_set(const base_type& right);

public:
	template<typename BsonDecimal128> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonDecimal128>, this_type&>::type
		operator=(BOOST_RV_REF(BsonDecimal128) right)
	{
		BsonDecimal128& right_ref = right;
		return this_type::pro_operator_set(static_cast<base_type&>(right_ref));
	}

	template<typename BsonDecimal128> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonDecimal128>, this_type&>::type
		operator=(const BsonDecimal128& right)
	{
		return this_type::pro_operator_set(static_cast<const base_type&>(right));
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		return this_type::pro_operator_set(static_cast<base_type&>(right_ref));
	}

	this_type& operator=(const this_type& right);

protected:
	void pro_swap(base_type& right);

public:
	template<typename BsonDecimal128> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonDecimal128>, void>::type
		swap(BOOST_RV_REF(BsonDecimal128) right)
	{
		BsonDecimal128& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
	}

	template<typename BsonDecimal128> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonDecimal128>, void>::type
		swap(BsonDecimal128& right)
	{
		this_type::pro_swap(static_cast<base_type&>(right));
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
	}

	void swap(this_type& right);

protected:
	void pro_assign_bson_value_t(const bson_value_t& val);

public:
	inline org_type* org_pointer(void)
	{
		return static_cast<org_type*>(this);
	}

	inline const org_type* org_pointer(void) const
	{
		return static_cast<const org_type*>(this);
	}

public:
	inline u64& var_low(void)
	{
		return base_type::low;
	}

	inline u64 var_low(void) const
	{
		return base_type::low;
	}

	inline u64& var_high(void)
	{
		return base_type::high;
	}

	inline u64 var_high(void) const
	{
		return base_type::high;
	}

public:
	inner_string_type str(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	template<typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			inner_string_type
		>::type
		str(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		char out_str[bson_decimal128_native_ex::E_BSON_DECIMAL128_STRING] = {0};

		return
			bson_decimal128_native_ex::s_bson_decimal128_to_string(this, out_str)?
				inner_string_type(out_str, alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())
				: inner_string_type(alloc);
	}

	template<typename String> inline
	String str(const string& ret_charset_name = charset::charset_name_t<String>())
	{
		typedef String now_string_type;
		return this_type::str().str<now_string_type>(ret_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		str(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>())
	{
		typedef String now_string_type;
		return this_type::str().str<now_string_type>(ret_charset_name);
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_DECIMAL128;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_DECIMAL128;
	}

public:
	// compare_eq
	inline bool compare_eq(u64 nhigh, u64 nlow) const
	{
		return bson_decimal128_native_ex::s_bson_decimal128_compare_eq(this, nhigh, nlow);
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		return
			bson_value_native_ex::s_value_cmp_eq_decimal128(
				boost::addressof(val), this);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			bool
		>::type
		compare_eq(T val) const
	{
		return
			bson_decimal128_native_ex::s_bson_decimal128_compare_eq(
				static_cast<const base_type*>(this), val);
	}

	template<typename BsonDecimal128>  inline
	typename boost::enable_if<boost::is_base_of<org_type, BsonDecimal128>, bool>::type
		compare_eq(const BsonDecimal128& right) const
	{
		return this_type::compare_eq(right.high, right.low);
	}

	// compare
	inline s32 compare(u64 nhigh, u64 nlow) const
	{
		return bson_decimal128_native_ex::s_bson_decimal128_compare(this, nhigh, nlow);
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, s32>::type
		compare(const BsonValue& val) const
	{
		return
			-bson_value_native_ex::s_value_cmp_decimal128(
				boost::addressof(val), this);
	}

	template<typename T> inline
	typename
		boost::enable_if
		<
			boost::is_arithmetic<T>,
			s32
		>::type
		compare(T val) const
	{
		return bson_decimal128_native_ex::s_bson_decimal128_compare(static_cast<const base_type*>(this), val);
	}

	template<typename BsonDecimal128>  inline
	typename boost::enable_if<boost::is_base_of<org_type, BsonDecimal128>, s32>::type
		compare(const BsonDecimal128& right) const
	{
		return this_type::compare(right.high, right.low);
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

		if(bson_native_ex::s_bson_is_nil(boost::addressof(bs)))
		{
			bson_init(boost::addressof(bs));
		}

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return
			bson_append_decimal128(
				boost::addressof(bs),
				str_name.data(),
				str_name.size(),
				static_cast<const base_type*>(this) );
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		return
			bson_native_ex::s_bson_save_value_of_decimal128(
				boost::addressof(bs),
				static_cast<const base_type*>(this));
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

		bool bsuccess = bson_iter_decimal128(boost::addressof(iter), static_cast<base_type*>(this));
		assert(bsuccess);
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		bool bsuccess = bson_iter_decimal128(boost::addressof(iter), static_cast<base_type*>(this));
		assert(bsuccess);
	}

protected:
	// const char*
	template<typename Char> inline
	typename
		boost::enable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const Char* str_num)
	{
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				reinterpret_cast<const char*>(str_num), this);
	}

	template<typename Char> inline
	typename
		boost::enable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const Char* str_num, std::size_t n)
	{
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				reinterpret_cast<const char*>(str_num), n, this);
	}

	template<typename Char> inline
	typename
		boost::disable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const Char* str_num)
	{
		inner_string_type mid_str(str_num);
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				mid_str.c_str(), mid_str.size(), this);
	}

	template<typename Char> inline
	typename
		boost::disable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const Char* str_num, std::size_t n)
	{
		inner_string_type mid_str(str_num, n);
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				mid_str.c_str(), mid_str.size(), this);
	}

	// basic_string
	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const BasicString<Char, Traits, Alloc>& str_num)
	{
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				reinterpret_cast<const char*>(str_num.c_str()), str_num.size(), this);
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const BasicString<Char, Traits, Alloc>& str_num, std::size_t n)
	{
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				reinterpret_cast<const char*>(str_num.c_str()), (std::min)(str_num.size(), n), this);
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const BasicString<Char, Traits, Alloc>& str_num)
	{
		inner_string_type mid_str(str_num);
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				mid_str.c_str(), mid_str.size(), this);
	}

	template<template<typename _C, typename _T, typename _A> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const BasicString<Char, Traits, Alloc>& str_num, std::size_t n)
	{
		inner_string_type mid_str(str_num, 0, (std::min)(str_num.length(), n));
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				mid_str.c_str(), mid_str.size(), this);
	}

	// basic_string_view
	template<template<typename _C, typename _T> class BasicStringView,
				typename Char, typename Traits> inline
	typename
		boost::enable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const BasicStringView<Char, Traits>& str_num)
	{
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				reinterpret_cast<const char*>(str_num.data()), str_num.size(), this);
	}

	template<template<typename _C, typename _T> class BasicStringView,
				typename Char, typename Traits> inline
	typename
		boost::enable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const BasicStringView<Char, Traits>& str_num, std::size_t n)
	{
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				reinterpret_cast<const char*>(str_num.data()), (std::min)(str_num.size(), n), this);
	}

	template<template<typename _C, typename _T> class BasicStringView,
				typename Char, typename Traits> inline
	typename
		boost::disable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const BasicStringView<Char, Traits>& str_num)
	{
		inner_string_type mid_str(str_num);
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				mid_str.c_str(), mid_str.size(), this);
	}

	template<template<typename _C, typename _T> class BasicStringView,
				typename Char, typename Traits> inline
	typename
		boost::disable_if
		<
			boost::mpl::bool_<(sizeof(Char) == 1)>,
			bool
		>::type
		pro_init_from_string(const BasicStringView<Char, Traits>& str_num, std::size_t n)
	{
		inner_string_type mid_str(str_num, 0, (std::min)(str_num.length(), n));
		return
			bson_decimal128_native_ex::s_bson_decimal128_from_string(
				mid_str.c_str(), mid_str.size(), this);
	}

	template<typename T1, typename T2> inline
	bool pro_init_from_string(const charset::string_charset_helper<T1, T2>& str_num_helper)
	{
		if(str_num_helper.second == YGGR_STR_UTF8_STRING_CHARSET_NAME())
		{
			return this_type::pro_init_from_string(str_num_helper.first);
		}
		else
		{
			inner_string_type str_num(str_num_helper);
			return this_type::pro_init_from_string(str_num);
		}
	}

	template<typename T1, typename T2> inline
	bool pro_init_from_string(const charset::string_charset_helper<T1, T2>& str_num_helper, std::size_t n)
	{
		if(str_num_helper.second == YGGR_STR_UTF8_STRING_CHARSET_NAME())
		{
			return this_type::pro_init_from_string(str_num_helper.first, n);
		}
		else
		{
			inner_string_type str_num(str_num_helper, 0, n);
			return this_type::pro_init_from_string(str_num);
		}
	}
};

// non member operator foo

// operator ==
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator==(const c_bson_decimal128& l, T r)
{
	return l.compare_eq(r);
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator==(T l, const c_bson_decimal128& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_decimal128& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_decimal128& r)
{
	return r.compare_eq(l);
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator==(const c_bson_decimal128& l, const BsonDeciml128& r)
{
	return l.compare_eq(r);
}

// operator !=
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator!=(const c_bson_decimal128& l, T r)
{
	return !l.compare_eq(r);
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator!=(T l, const c_bson_decimal128& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_decimal128& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_decimal128& r)
{
	return !r.compare_eq(l);
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator!=(const c_bson_decimal128& l, const BsonDeciml128& r)
{
	return !l.compare_eq(r);
}

// operator <
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator<(const c_bson_decimal128& l, T r)
{
	return l.compare(r) < 0;
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator<(T l, const c_bson_decimal128& r)
{
	return r.compare(l) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const c_bson_decimal128& l, const BsonValue& r)
{
	return l.compare(r) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const BsonValue& l, const c_bson_decimal128& r)
{
	return r.compare(l) > 0;
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator<(const c_bson_decimal128& l, const BsonDeciml128& r)
{
	return l.compare(r) < 0;
}

// operator <=
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator<=(const c_bson_decimal128& l, T r)
{
	return l.compare(r) <= 0;
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator<=(T l, const c_bson_decimal128& r)
{
	return r.compare(l) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const c_bson_decimal128& l, const BsonValue& r)
{
	return l.compare(r) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const BsonValue& l, const c_bson_decimal128& r)
{
	return r.compare(l) >= 0;
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator<=(const c_bson_decimal128& l, const BsonDeciml128& r)
{
	return l.compare(r) <= 0;
}

// operator >
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator>(const c_bson_decimal128& l, T r)
{
	return l.compare(r) > 0;
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator>(T l, const c_bson_decimal128& r)
{
	return r.compare(l) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const c_bson_decimal128& l, const BsonValue& r)
{
	return l.compare(r) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const BsonValue& l, const c_bson_decimal128& r)
{
	return r.compare(l) < 0;
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator>(const c_bson_decimal128& l, const BsonDeciml128& r)
{
	return l.compare(r) > 0;
}

// operator >=
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator>=(const c_bson_decimal128& l, T r)
{
	return l.compare(r) >= 0;
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator>=(T l, const c_bson_decimal128& r)
{
	return r.compare(l) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const c_bson_decimal128& l, const BsonValue& r)
{
	return l.compare(r) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const BsonValue& l, const c_bson_decimal128& r)
{
	return r.compare(l) <= 0;
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator>=(const c_bson_decimal128& l, const BsonDeciml128& r)
{
	return l.compare(r) >= 0;
}

namespace detail
{

template<>
struct bson_operator_outter<c_bson_decimal128>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_decimal128& right,
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"$numberDecimal\" : \"" << right.str() << "\" }";
		return os;
	}
};


} // namespace detail

// operator<<

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_decimal128& right)
{
	typedef detail::bson_operator_outter<c_bson_decimal128> outter_type;

	outter_type outter;
	return outter(os, right, 0);
}

} // namespace nsql_database_system
} // namesapce yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_decimal128& l, bson_decimal128_t& r)
{
	l.swap(r);
}

#else

template<typename BsonDecimal128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, void>::type
	swap(c_bson_decimal128& l, BsonDecimal128& r)
{
	l.swap(r);
}

#endif //#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_decimal128& l, c_bson_decimal128& r)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DECIMAL128_HPP__
