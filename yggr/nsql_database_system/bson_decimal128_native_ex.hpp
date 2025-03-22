// bson_decimal128_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_DECIMAL128_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_DECIMAL128_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>
#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/string.hpp>

#include <ostream>

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

class bson_decimal128_native_ex
{
public:
	typedef bson_decimal128_t org_type;

public:
	YGGR_STATIC_CONSTANT(std::size_t, E_BSON_DECIMAL128_STRING = BSON_DECIMAL128_STRING); //43
	typedef boost::mpl::string<'Infi', 'nity'> E_BSON_DECIMAL128_INF; // "Infinity"
	typedef boost::mpl::string<'NaN'> E_BSON_DECIMAL128_NAN; // "NaN"

private:
	typedef bson_decimal128_native_ex this_type;

public:
	static char* s_bson_decimal128_to_string(const bson_decimal128_t* dec, char* out);
	static bool s_bson_decimal128_from_string(const char* str, bson_decimal128_t* out);
	static bool s_bson_decimal128_from_string(const char* str, std::size_t len, bson_decimal128_t* out);

public:
	//s_bson_decimal128_compare_eq
	template<typename T> inline
	static typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
		s_bson_decimal128_compare_eq(const bson_decimal128_t* pdec, T val)
	{
		assert(pdec);
		return pdec && pdec->high == 0 && pdec->low == val;
	}

	static bool s_bson_decimal128_compare_eq(const bson_decimal128_t* pdec, u64 nhigh, u64 nlow);
	static bool s_bson_decimal128_compare_eq(const bson_decimal128_t* l, const bson_decimal128_t* r);

	//s_bson_decimal128_compare
	template<typename T> inline
	static typename boost::enable_if<boost::is_arithmetic<T>, s32>::type
		s_bson_decimal128_compare(const bson_decimal128_t* pdec, T val)
	{
		assert(pdec);
		return
			pdec->high == 0?
				pdec->low == val?
					0
					: pdec->low < val? -1 :1
				: 1;
	}

	static s32 s_bson_decimal128_compare(const bson_decimal128_t* pdec, u64 nhigh, u64 nlow);
	static s32 s_bson_decimal128_compare(const bson_decimal128_t* l, const bson_decimal128_t* r);
};

} // namespace nsql_database_system
} // namespace yggr

// compare
// operator ==

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator==(const bson_decimal128_t& l, T r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare_eq(
			boost::addressof(l), r);
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator==(T l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare_eq(
			boost::addressof(r), l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const bson_decimal128_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_decimal128(
			boost::addressof(r), boost::addressof(l));
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_decimal128(
			boost::addressof(l), boost::addressof(r));
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator==(const bson_decimal128_t& l, const BsonDeciml128& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare_eq(
			boost::addressof(l), boost::addressof(r));
}


// operator !=
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator!=(const bson_decimal128_t& l, T r)
{
	return
		!yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare_eq(
			boost::addressof(l), r);
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator!=(T l, const bson_decimal128_t& r)
{
	return
		!yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare_eq(
			boost::addressof(r), l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const bson_decimal128_t& l, const BsonValue& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_decimal128(
			boost::addressof(r), boost::addressof(l));
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const bson_decimal128_t& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_decimal128(
			boost::addressof(l), boost::addressof(r));
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator!=(const bson_decimal128_t& l, const BsonDeciml128& r)
{
	return
		!yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare_eq(
			boost::addressof(l), boost::addressof(r));
}

// operator <
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator<(const bson_decimal128_t& l, T r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(l), r) < 0;
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator<(T l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(r), l) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const bson_decimal128_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_decimal128(
			boost::addressof(r), boost::addressof(l)) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const BsonValue& l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_decimal128(
			boost::addressof(l), boost::addressof(r)) < 0;
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator<(const bson_decimal128_t& l, const BsonDeciml128& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(l), boost::addressof(r)) < 0;
}

// operator <=

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator<=(const bson_decimal128_t& l, T r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(l), r) <= 0;
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator<=(T l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(r), l) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const bson_decimal128_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_decimal128(
			boost::addressof(r), boost::addressof(l)) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const BsonValue& l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_decimal128(
			boost::addressof(l), boost::addressof(r)) <= 0;
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator<=(const bson_decimal128_t& l, const BsonDeciml128& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(l), boost::addressof(r)) <= 0;
}

// operator >
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator>(const bson_decimal128_t& l, T r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(l), r) > 0;
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator>(T l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(r), l) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const bson_decimal128_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_decimal128(
			boost::addressof(r), boost::addressof(l)) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const BsonValue& l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_decimal128(
			boost::addressof(l), boost::addressof(r)) > 0;
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator>(const bson_decimal128_t& l, const BsonDeciml128& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(l), boost::addressof(r)) > 0;
}

// operator >=
template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator>=(const bson_decimal128_t& l, T r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(l), r) >= 0;
}

template<typename T> inline
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
	operator>=(T l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(r), l) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const bson_decimal128_t& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_decimal128(
			boost::addressof(r), boost::addressof(l)) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const BsonValue& l, const bson_decimal128_t& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_decimal128(
			boost::addressof(l), boost::addressof(r)) >= 0;
}

template<typename BsonDeciml128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
	operator>=(const bson_decimal128_t& l, const BsonDeciml128& r)
{
	return
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_compare(
			boost::addressof(l), boost::addressof(r)) >= 0;
}

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<>
struct bson_operator_outter<bson_decimal128_t>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const bson_decimal128_t& right,
					bson_json_mode_def::mode_type) const
	{
		char str_dec128[yggr::nsql_database_system::bson_decimal128_native_ex::E_BSON_DECIMAL128_STRING] = {0};
		yggr::nsql_database_system::bson_decimal128_native_ex::s_bson_decimal128_to_string(boost::addressof(right), str_dec128);

		os << "{ \"$numberDecimal\" : \"" << str_dec128 << "\" }";
		return os;
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

// operator<<
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const bson_decimal128_t& right)
{
	typedef yggr::nsql_database_system::detail::bson_operator_outter<bson_decimal128_t> outter_type;

	outter_type outter;
	return outter(os, right, 0);
}

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{


void swap(bson_decimal128_t& l, bson_decimal128_t& r);

template<typename BsonDecimal128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, void>::type
	swap(bson_decimal128_t& l, BsonDecimal128& r)
{
	if(boost::addressof(l) == boost::addressof(r))
	{
		return;
	}

	yggr::swap(l.low, r.low);
	yggr::swap(l.high, r.high);
}

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename BsonDecimal128> inline
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, void>::type
	swap(bson_decimal128_t& l, BOOST_RV_REF(BsonDecimal128) r)
{
	BsonDecimal128& right_ref = r;
	if(boost::addressof(l) == boost::addressof(right_ref))
	{
		return;
	}

	yggr::swap(l.low, right_ref.low);
	yggr::swap(l.high, right_ref.high);
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_DECIMAL128_NATIVE_EX_HPP__
