// integer_sequence.hpp

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

#ifndef __YGGR_TYPE_TRAITS_DETAIL_INTEGER_SEQUENCE_HPP__
#define __YGGR_TYPE_TRAITS_DETAIL_INTEGER_SEQUENCE_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

#if defined(__cpp_lib_integer_sequence)
#	if !defined(YGGR_HAS_MAKE_INTEGER_SEQ)
#		define YGGR_HAS_MAKE_INTEGER_SEQ
#	endif // YGGR_HAS_MAKE_INTEGER_SEQ
#endif // __cpp_lib_integer_sequence

#if !defined(YGGR_HAS_MAKE_INTEGER_SEQ)
#	include <boost/mpl/if.hpp>
#	include <boost/type_traits/is_integral.hpp>
#endif //YGGR_HAS_MAKE_INTEGER_SEQ

#include <utility>
#include <cstddef>

#	if (!defined(YGGR_NO_CXX14_VARIADIC_TEMPLATES) \
			&& defined(YGGR_HAS_MAKE_INTEGER_SEQ) )

namespace yggr
{

using std::integer_sequence;
using std::make_integer_sequence;

using std::index_sequence;
using std::make_index_sequence;

template<typename... T> 
using index_sequence_for = make_integer_sequence<std::size_t, sizeof...(T)>;

} // namespace yggr

#	else

namespace yggr
{

// integer_sequence
template<typename T, T... I> 
struct integer_sequence
{
	static_assert(boost::is_integral<T>::value, "integer_sequence<T, I...> requires T to be an integral type.");

	using value_type = T;

	static constexpr size_t size() noexcept 
	{
		return sizeof...(I);
	}
};

#		if defined(YGGR_HAS_MAKE_INTEGER_SEQ)

template<typename T, T N> 
using make_integer_sequence = __make_integer_seq<integer_sequence, T, N>;

#		else

// detail::make_integer_sequence_impl
namespace detail
{

// iseq_if_c
template<bool C, typename T, typename E> 
using iseq_if_c = typename boost::mpl::if_c<C, T, E>::type;

// iseq_identity
template<typename T> 
struct iseq_identity
{
	using type = T;
};

template<typename S1, typename S2> 
struct append_integer_sequence;

template<typename T, T... I, T... J> 
struct append_integer_sequence<integer_sequence<T, I...>, 
								integer_sequence<T, J...> >
{
	using type = integer_sequence< T, I..., ( sizeof...(I) + J )... >;
};

template<typename T, T N> 
struct make_integer_sequence_impl;

template<typename T, T N> 
struct make_integer_sequence_impl_
{
private:

	static_assert( N >= 0, "make_integer_sequence<T, N>: N must not be negative" );

	static T const M = N / 2;
	static T const R = N % 2;

	using S1 = typename make_integer_sequence_impl<T, M>::type;
	using S2 = typename append_integer_sequence<S1, S1>::type;
	using S3 = typename make_integer_sequence_impl<T, R>::type;
	using S4 = typename append_integer_sequence<S2, S3>::type;

public:
	using type = S4;
};

template<typename T, T N> 
struct make_integer_sequence_impl
	: public 
		iseq_if_c
		<
			N == 0, 
			iseq_identity< integer_sequence<T> >, 
			iseq_if_c
			<
				N == 1, 
				iseq_identity< integer_sequence<T, 0> >, 
				make_integer_sequence_impl_<T, N> 
			>
		>
{
};

} // namespace detail

// make_integer_sequence
template<typename T, T N> 
using make_integer_sequence = typename detail::make_integer_sequence_impl<T, N>::type;

#		endif // defined(BOOST_MP11_HAS_MAKE_INTEGER_SEQ)

// index_sequence
template<std::size_t... I> 
using index_sequence = integer_sequence<std::size_t, I...>;

// make_index_sequence
template<std::size_t N> 
using make_index_sequence = make_integer_sequence<std::size_t, N>;

// index_sequence_for
template<typename... T> 
using index_sequence_for = make_integer_sequence<std::size_t, sizeof...(T)>;

} // namespace yggr

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#endif //YGGR_NO_CXX14_VARIADIC_TEMPLATES

#endif // __YGGR_TYPE_TRAITS_DETAIL_INTEGER_SEQUENCE_HPP__