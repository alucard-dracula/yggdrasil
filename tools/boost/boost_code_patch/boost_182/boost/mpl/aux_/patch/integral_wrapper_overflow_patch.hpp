
#if !defined __BOOST_MPL__AUX_PATCH_INTEGRAL_WRAPPER_OVERFLOW_PATCH_HPP__
#define __BOOST_MPL__AUX_PATCH_INTEGRAL_WRAPPER_OVERFLOW_PATCH_HPP__

#include <boost/cstdint.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>


namespace boost_mpl_patch
{

namespace detail
{

template<typename T, bool is_sig = boost::is_signed<T>::value>
struct min_max_signed_value;

template<typename T>
struct min_max_signed_value<T, true>
{
    typedef T value_type;
    typedef min_max_signed_value type;

    BOOST_STATIC_CONSTANT(
		value_type,
		max_value = (~static_cast<value_type>(0)) ^ (static_cast<value_type>(1) << ((sizeof(value_type) << 3) - 1)) );

    BOOST_STATIC_CONSTANT(value_type, min_value = ~max_value);
};

template<typename T, bool is_usig = boost::is_unsigned<T>::value>
struct min_max_unsigned_value;

template<typename T>
struct min_max_unsigned_value<T, true>
{
    typedef T value_type;
    typedef min_max_unsigned_value type;

	BOOST_STATIC_CONSTANT(value_type, min_value = 0);
	BOOST_STATIC_CONSTANT(value_type, max_value = ~min_value);
};

template<typename T, T N>
struct value_store
{
    typedef value_store type;
    BOOST_STATIC_CONSTANT(T, value = N);
};

template<typename T, T N,
			bool is_sig = boost::is_signed<T>::value,
			bool is_unsig = boost::is_unsigned<T>::value>
struct inner_value_next;

// signed
template<typename T, T N>
struct inner_value_next<T, N, true, false>
	: public
		value_store
		<
			T,
			(N < (min_max_signed_value<T>::max_value)? (N + 1) : (min_max_signed_value<T>::min_value) )
		>
{
};

// unsigned
template<typename T, T N>
struct inner_value_next<T, N, false, true>
	: public
		value_store
		<
			T,
			(N < (min_max_unsigned_value<T>::max_value)? N + 1 : (min_max_unsigned_value<T>::min_value) )
		>
{
};

template<typename T, T N,
			bool is_sig = boost::is_signed<T>::value,
			bool is_unsig = boost::is_unsigned<T>::value>
struct inner_value_prior;

// signed
template<typename T, T N>
struct inner_value_prior<T, N, true, false>
	: public
		value_store
		<
			T,
			((min_max_signed_value<T>::min_value) < N? (N - 1) : (min_max_signed_value<T>::max_value) )
		>
{
};

// unsigned
template<typename T, T N>
struct inner_value_prior<T, N, false, true>
	: public
		value_store
		<
			T,
			((min_max_unsigned_value<T>::min_value) < N? (N - 1) : (min_max_unsigned_value<T>::max_value) )
		>
{
};

} // namespace detail

template<typename T, T N>
struct value_next
	: public detail::inner_value_next<T, N>
{
};

template<typename T, T N>
struct value_prior
	: public detail::inner_value_prior<T, N>
{
};

#if defined(_MSC_VER)
#	if !defined(_MSVC_LANG) || _MSVC_LANG < 202000
#		if !defined(BOOST_NO_CHAR8_T)
#			define BOOST_NO_CHAR8_T
#		endif // BOOST_NO_CHAR8_T
#	endif // _MSC_VER
#else
#	if __cplusplus < 202000
#		if !defined(BOOST_NO_CHAR8_T)
#			define BOOST_NO_CHAR8_T
#		endif // BOOST_NO_CHAR8_T
#	endif // __cplusplus < 202000
#endif // _MSC_VER


#if !defined(BOOST_NO_CHAR8_T)

template<char8_t N>
struct value_next<char8_t, N>
	: public detail::inner_value_next<boost::uint8_t, N>
{
};

template<char8_t N>
struct value_prior<char8_t, N>
	: public detail::inner_value_prior<boost::uint8_t, N>
{
};

#endif // #if !defined(BOOST_NO_CHAR8_T)

#if !defined(BOOST_NO_CHAR16_T)

template<char16_t N>
struct value_next<char16_t, N>
	: public detail::inner_value_next<boost::uint16_t, N>
{
};

template<char16_t N>
struct value_prior<char16_t, N>
	: public detail::inner_value_prior<boost::uint16_t, N>
{
};

#endif // #if !defined(BOOST_NO_CHAR16_T)

#if !defined(BOOST_NO_CHAR32_T)

template<char32_t N>
struct value_next<char32_t, N>
	: public detail::inner_value_next<boost::uint32_t, N>
{
};

template<char32_t N>
struct value_prior<char32_t, N>
	: public detail::inner_value_prior<boost::uint32_t, N>
{
};

#endif // #if !defined(BOOST_NO_CHAR32_T)

} // namesapce boost_mpl_patch

#endif // __BOOST_MPL__AUX_PATCH_INTEGRAL_WRAPPER_OVERFLOW_PATCH_HPP__
