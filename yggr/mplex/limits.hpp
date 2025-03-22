//limits.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_MPLEX_LIMITS_HPP__
#define __YGGR_MPLEX_LIMITS_HPP__

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4307)
#endif // _MSC_VER

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/number_t.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/null_t.hpp>

#include <boost/cstdint.hpp>
#include <boost/limits.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/char.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/or.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace yggr
{
namespace mplex
{
namespace detail
{

// basic_numeric_limits
template<typename T>
struct basic_numeric_limits
	: public std::numeric_limits<T>
{
private:
	typedef basic_numeric_limits this_type;

public:
	typedef ::std::numeric_limits<T> base_type;
	typedef base_type base_limits_type;

	typedef T value_type;

	typedef boost::mpl::bool_<!!(base_type::is_specialized)> is_specialized_type;

	typedef boost::mpl::int_<static_cast<int>(base_type::digits)> digits_type;
	typedef boost::mpl::int_<static_cast<int>(base_type::digits10)> digits10_type;

	typedef boost::mpl::bool_<!!(base_type::is_signed)> is_signed_type;
	typedef boost::mpl::bool_<!!(base_type::is_integer)> is_integer_type;
	typedef boost::mpl::bool_<!!(base_type::is_exact)> is_exact_type;

	typedef boost::mpl::int_<static_cast<int>(base_type::radix)> radix_type;

	typedef boost::mpl::int_<static_cast<int>(base_type::min_exponent)> min_exponent_type;
	typedef boost::mpl::int_<static_cast<int>(base_type::min_exponent10)> min_exponent10_type;
	typedef boost::mpl::int_<static_cast<int>(base_type::max_exponent)> max_exponent_type;
	typedef boost::mpl::int_<static_cast<int>(base_type::max_exponent10)> max_exponent10_type;

	typedef boost::mpl::bool_<!!(base_type::has_infinity)> has_infinity_type;
	typedef boost::mpl::bool_<!!(base_type::has_quiet_NaN)> has_quiet_NaN_type;
	typedef boost::mpl::bool_<!!(base_type::has_signaling_NaN)> has_signaling_NaN_type;

	typedef boost::mpl::int_<static_cast<int>(base_type::has_denorm)> has_denorm_type;
	typedef boost::mpl::bool_<!!(base_type::has_denorm_loss)> has_denorm_loss_type;

	typedef boost::mpl::bool_<!!(base_type::is_iec559)> is_iec559_type;
	typedef boost::mpl::bool_<!!(base_type::is_bounded)> is_bounded_type;
	typedef boost::mpl::bool_<!!(base_type::is_modulo)> is_modulo_type;

	typedef boost::mpl::bool_<!!(base_type::traps)> traps_type;
	typedef boost::mpl::bool_<!!(base_type::tinyness_before)> tinyness_before_type;
	typedef boost::mpl::int_<static_cast<int>(base_type::round_style)> round_style_type;

	typedef null_type min_type;
	typedef null_type max_type;
	typedef null_type lowest_type;

	typedef null_type epsilon_type;
	typedef null_type round_error_type;

	typedef null_type infinity_type;
	typedef null_type quiet_NaN_type;
	typedef null_type signaling_NaN_type;

	typedef null_type denorm_min_type;

	//added in subclass
	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> min_type;
	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> max_type;
	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> lowest_type;

	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> epsilon_type;
	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> round_error_type;

	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> infinity_type;
	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> quiet_NaN_type;
	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> signaling_NaN_type;

	//typedef boost::mpl::integral_c<value_type, static_cast<value_type>(value_type())> denorm_min_type;
};

// integer_limits
template<typename T, T __min, T __max>
struct integer_limits
	: public basic_numeric_limits<T>
{
private:
	typedef integer_limits this_type;

public:
	typedef basic_numeric_limits<T> base_type;
	typedef typename base_type::base_limits_type base_limits_type;
	typedef typename base_type::value_type value_type;

	typedef value_type store_value_type;

	typedef boost::mpl::integral_c<store_value_type, __min> min_type;
	typedef boost::mpl::integral_c<store_value_type, __max> max_type;

	typedef boost::mpl::integral_c<store_value_type, __min> lowest_type;

	typedef boost::mpl::integral_c<store_value_type, static_cast<store_value_type>(store_value_type())> epsilon_type;
	typedef boost::mpl::integral_c<store_value_type, static_cast<store_value_type>(store_value_type())> round_error_type;

	typedef boost::mpl::vector_c<uint, 0, 0, 0, 0, 0> infinity_type;
	typedef boost::mpl::vector_c<uint, 0, 0, 0, 0, 0> quiet_NaN_type;
	typedef boost::mpl::vector_c<uint, 0, 0, 0, 0, 0> signaling_NaN_type;

	typedef boost::mpl::integral_c<store_value_type, static_cast<store_value_type>(store_value_type())> denorm_min_type;
};

// floating_limits

#if defined(YGGR_BIG_ENDIAN)

template<typename T, uint Word>
struct float_helper
{
	//sizeof(long double) == 16
	typedef T result_type;
	typedef boost::mpl::vector_c<uint, Word, 0, 0, 0, 0> type;
};

#else

template<typename T, uint Word>
struct float_helper
{
	// sizeof(long double) == 12, but only 10 bytes significant
	typedef T result_type;
	typedef boost::mpl::vector_c<
				uint, 0, 0, 0, Word,
				static_cast<uint>(16 - (sizeof(T) == 12? 10 : sizeof(T)))> type;
};

#endif // YGGR_BIG_ENDIAN


template<typename T,
			YGGR_PP_UINTEGER_T_TYPE_TPL(sizeof(T)) __min,
			YGGR_PP_UINTEGER_T_TYPE_TPL(sizeof(T)) __max,
			uint __infinity_word, uint __qNaN_word, uint __sNaN_word,
			YGGR_PP_UINTEGER_T_TYPE_TPL(sizeof(T)) __epsilon,
			YGGR_PP_UINTEGER_T_TYPE_TPL(sizeof(T)) __round_error>
struct floating_limits
	: public basic_numeric_limits<T>
{
private:
	typedef floating_limits this_type;

public:
	typedef basic_numeric_limits<T> base_type;
	typedef typename base_type::base_limits_type base_limits_type;
	typedef typename base_type::value_type value_type;

	typedef YGGR_PP_UINTEGER_T_TYPE_TPL(sizeof(T)) store_value_type;

	typedef boost::mpl::integral_c<store_value_type, __min> min_type;
	typedef boost::mpl::integral_c<store_value_type, __max> max_type;

	typedef boost::mpl::integral_c<store_value_type,
									(__max | (store_value_type(1) << (sizeof(store_value_type) * CHAR_BIT - 1)))> lowest_type;

	typedef boost::mpl::integral_c<store_value_type, __epsilon> epsilon_type;
	typedef boost::mpl::integral_c<store_value_type, __round_error> round_error_type;

	// !!! these data can not be compared	!!!
	typedef typename float_helper<value_type, __infinity_word>::type infinity_type;
	typedef typename float_helper<value_type, __qNaN_word>::type quiet_NaN_type;
	typedef typename float_helper<value_type, __sNaN_word>::type signaling_NaN_type;

	/*
	!!! the value is not necessarily equal to std::numeric_limits<T>::denorm_min()	!!!
	!!! but its same to FLT_MIN or DBL_MIN											!!!
	*/
	typedef boost::mpl::integral_c<store_value_type, __min> denorm_min_type;
};

} // namespace detail


#define YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(__type__, __min__, __max__) \
	template<> \
	struct numeric_limits< __type__ > \
		: public detail::integer_limits<__type__, (__min__), (__max__)> {};

#define YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_FLOATING_IMPL(__type__, __min__, __max__, \
														__infy__, __qNaN__, __sNaN__, \
														__ep__, __re__) \
	template<> \
	struct numeric_limits< __type__ > \
		: public detail::floating_limits<__type__, (__min__), (__max__), \
											(__infy__), (__qNaN__), (__sNaN__), \
											(__ep__), (__re__)> {};


template<typename T>
struct numeric_limits
	: public detail::basic_numeric_limits<T> {};


YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(bool, false, true);
YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(char, CHAR_MIN, CHAR_MAX);
YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(s8, SCHAR_MIN, SCHAR_MAX);
YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(u8, 0, UCHAR_MAX);


#if !defined(YGGR_NO_CWCHAR)

template<>
struct numeric_limits<wchar_t>
#	if !defined(WCHAR_MAX) || !defined(WCHAR_MIN)
#		if defined(_WIN32) || defined(__CYGWIN__)
		: public detail::integer_limits<wchar_t, 0, USHRT_MAX>
#		elif defined(__hppa)
		// wchar_t has "unsigned int" as the underlying type
		: public detail::integer_limits<wchar_t, 0, UINT_MAX>
#		else
		// assume that wchar_t has "int" as the underlying type
		: public detail::integer_limits<wchar_t, INT_MIN, INT_MAX>
#		endif // defined(_WIN32) || defined(__CYGWIN__)
#	else
	// we have WCHAR_MIN and WCHAR_MAX defined, so use it
	: public detail::integer_limits<wchar_t, WCHAR_MIN, WCHAR_MAX>
#	endif //  !defined(WCHAR_MAX) || !defined(WCHAR_MIN)
{};

#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)

YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(c8, static_cast<c8>(CHAR_MIN), static_cast<c8>(CHAR_MAX));

#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)

YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(c16, 0, USHRT_MAX);

#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)

YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(c32, 0, UINT_MAX);

#endif // YGGR_NO_CHAR32_T

YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(s16, SHRT_MIN, SHRT_MAX);
YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(u16, 0, USHRT_MAX);

YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(int, INT_MIN, INT_MAX);
YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(unsigned int, 0, UINT_MAX);

#if !defined(__GNUC__) || defined(_WIN32) || defined(_WIN64)
    YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(long, LONG_MIN, LONG_MAX);
    YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(unsigned long, 0, ULONG_MAX);
#endif // #if !defined(__GNUC__) || defined(_WIN32) || defined(_WIN64)


// android and gcc linux need defined, android clang defined __GNUC__
#if (defined(__GNUC__) || defined(YGGR_AT_ANDROID) || defined(__clang__)) \
	&& !(defined(_WIN32) || defined(_WIN64)) \
	&& ((defined( __i686__ ) && (__i686__ == 1)) \
			|| (defined(__i386__) && (__i386__ == 1)) \
			|| (defined(_ILP32) && (_ILP32 == 1)) \
		|| (defined(YGGR_AT_DARWIN)))

	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(
		long int, 
		static_cast<long int>(1) << (sizeof(long int) * CHAR_BIT - 1),
		~(static_cast<long int>(1) << (sizeof(long int) * CHAR_BIT - 1)));

	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(
		unsigned long int, 
		static_cast<unsigned long int>(0),
		~static_cast<unsigned long int>(0));
		
#endif // __GNUC__

// s64
#ifdef BOOST_HAS_MS_INT64
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(s64, INT64_C(0x8000000000000000), INT64_C(0x7FFFFFFFFFFFFFFF));
#elif defined(LLONG_MAX)
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(s64, LLONG_MIN, LLONG_MAX);
#elif defined(LONGLONG_MAX)
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(s64, LONGLONG_MIN, LONGLONG_MAX);
#else
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(s64, UINT64_C(1) << (sizeof(BOOST_LLT) * CHAR_BIT - 1,
														~(UINT64_C(1) << (sizeof(BOOST_LLT) * CHAR_BIT - 1)));
#endif // BOOST_HAS_MS_INT64

// u64
#ifdef BOOST_HAS_MS_INT64
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(u64, UINT64_C(0), UINT64_C(0xFFFFFFFFFFFFFFFF));
#elif defined(ULLONG_MAX) && defined(ULLONG_MIN)
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(u64, ULLONG_MIN, ULLONG_MAX);
#elif defined(ULONGLONG_MAX) && defined(ULONGLONG_MIN)
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(u64, ULONGLONG_MIN, ULONGLONG_MAX);
#else
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_INTEGER_IMPL(u64, UINT64_C(0), UINT64_C(~0));
#endif // BOOST_HAS_MS_INT64



#if defined(YGGR_BIG_ENDIAN)
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_FLOATING_IMPL(
		f32, static_cast<u32>(1) << 23,
				static_cast<u32>(0x7f7fffff),
				static_cast<uint>(0x7f80 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of +infinity
				static_cast<uint>(0x7f81 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of quiet NaN
				static_cast<uint>(0x7fc1 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of signaling NaN
				static_cast<u32>(0x34000000),
				static_cast<u32>(0x3f000000) );
#else
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_FLOATING_IMPL(
		f32, static_cast<u32>(1) << 23,
				static_cast<u32>(UINT32_C(0x7f7fffff)),
				static_cast<uint>(UINT32_C(0x7f800000)),	// Last word of +infinity
				static_cast<uint>(UINT32_C(0x7f810000)),	// Last word of quiet NaN
				static_cast<uint>(UINT32_C(0x7fc10000)),	// Last word of signaling NaN
				static_cast<u32>(UINT32_C(0x34000000)),
				static_cast<u32>(UINT32_C(0x3f000000)) );
#endif // YGGR_BIG_ENDIAN


#ifdef BOOST_HAS_MS_INT64
#	if defined(YGGR_BIG_ENDIAN)
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_FLOATING_IMPL(
		f64, static_cast<u64>(UINT64_C(0x0010000000000000)),
				static_cast<u64>(UINT64_C(0x7fefffffffffffff)),
				static_cast<uint>(0x7ff0 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of +infinity
				static_cast<uint>(0x7ff1 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of quiet NaN
				static_cast<uint>(0x7ff9 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of signaling NaN
				static_cast<u64>(UINT64_C(0x3cb0000000000000)),
				static_cast<u64>(UINT64_C(0x3fe0000000000000)) );
#	else
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_FLOATING_IMPL(
		f64, static_cast<u64>(UINT64_C(0x0010000000000000)),
				static_cast<u64>(UINT64_C(0x7fefffffffffffff)),
				static_cast<uint>(UINT32_C(0x7ff00000)),		// Last word of +infinity
				static_cast<uint>(UINT32_C(0x7ff10000)),		// Last word of quiet NaN
				static_cast<uint>(UINT32_C(0x7ff90000)),		// Last word of signaling NaN
				static_cast<u64>(UINT64_C(0x3cb0000000000000)),
				static_cast<u64>(UINT64_C(0x3fe0000000000000)) );
#	endif // YGGR_BIG_ENDIAN
#else
#	if defined(YGGR_BIG_ENDIAN)
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_FLOATING_IMPL(
		f64, static_cast<u64>(1) << 52,
				static_cast<u64>(UINT64_C(0x7fefffffffffffff)),
				static_cast<uint>(0x7ff0 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of +infinity
				static_cast<uint>(0x7ff1 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of quiet NaN
				static_cast<uint>(0x7ff9 << (sizeof(sint) * CHAR_BIT - 16)),	// Last word of signaling NaN
				static_cast<u64>(UINT64_C(0x3cb0000000000000)),
				static_cast<u64>(UINT64_C(0x3fe0000000000000)) );
#	else
	YGGR_PP_MPLEX_NUMERIC_LIMITS_OF_FLOATING_IMPL(
		f64, static_cast<u64>(1) << 52,
				static_cast<u64>(UINT64_C(0x7fefffffffffffff)),
				static_cast<uint>(UINT32_C(0x7ff00000)),		// Last word of +infinity
				static_cast<uint>(UINT32_C(0x7ff10000)),		// Last word of quiet NaN
				static_cast<uint>(UINT32_C(0x7ff90000)),		// Last word of signaling NaN
				static_cast<u64>(UINT64_C(0x3cb0000000000000)),
				static_cast<u64>(UINT64_C(0x3fe0000000000000)) );
#	endif // YGGR_BIG_ENDIAN
#endif // BOOST_HAS_MS_INT64


#if defined(YGGR_BIG_ENDIAN)

template<typename T, typename W> inline
typename
	boost::enable_if
	<
		boost::is_floating_point<T>,
		T
	>::type
	get_word(W)
{
	const uint _S_word[4] = {
			boost::mpl::at_c<W, 0>::type::value,
			boost::mpl::at_c<W, 1>::type::value,
			boost::mpl::at_c<W, 2>::type::value,
			boost::mpl::at_c<W, 3>::type::value };

	return *reinterpret_cast<const T*>(&_S_word);
}

#else

template<typename T, typename W> inline
typename
	boost::enable_if
	<
		boost::is_floating_point<T>,
		T
	>::type
	get_word(W)
{
	const uint _S_word[4] = {
			boost::mpl::at_c<W, 0>::type::value,
			boost::mpl::at_c<W, 1>::type::value,
			boost::mpl::at_c<W, 2>::type::value,
			boost::mpl::at_c<W, 3>::type::value };

	return *reinterpret_cast<const T*>(
				reinterpret_cast<const char *>(&_S_word) + boost::mpl::at_c<W, 4>::type::value);
}

#endif // YGGR_BIG_ENDIAN


template<typename T, typename W> inline
typename
	boost::disable_if
	<
		boost::is_floating_point<T>,
		T
	>::type
	get_word(W)
{
	BOOST_MPL_ASSERT((
		boost::mpl::or_
		<
			boost::is_same<W, null_type>,
			boost::is_same<typename W::type, boost::mpl::vector_c<uint, 0, 0, 0, 0, 0>::type >
		>));

	return T();
}

} // namespace mplex
} // namespace yggr

#ifdef _MSC_VER
#  pragma warning(pop)
#endif // _MSC_VER

#endif // __YGGR_MPLEX_LIMITS_HPP__
