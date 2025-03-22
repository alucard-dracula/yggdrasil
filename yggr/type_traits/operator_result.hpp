//operator_result.hpp

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

#ifndef __YGGR_TYPE_TRAITS_OPERATOR_RESUTL_HPP__
#define __YGGR_TYPE_TRAITS_OPERATOR_RESUTL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/type_traits/traits.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/open.hpp>

#include <boost/preprocessor/cat.hpp>

namespace yggr
{
namespace detail
{

// fix error c2784 in operator_result

template<typename T>
struct operator_result_t_getter
	: public traits<T>
{
public:
    operator T() const;
};

} // namespace detail
} // namespace yggr

#define YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(__op_name__, __op__) \
	namespace yggr { \
		template<typename L, typename R> \
		struct BOOST_PP_CAT(__op_name__, _operator_result) { \
			public: \
			typedef \
				YGGR_TYPEOF_TPL( \
					(declval< typename ::yggr::detail::operator_result_t_getter<L>::type >()) \
					YGGR_PP_OPEN( __op__ ) \
					(declval< typename ::yggr::detail::operator_result_t_getter<R>::type >()) ) type; }; }

#define YGGR_PP_TYPE_TRAITS_BEFORE_UNARY_OPERATOR_RESULT(__op_name__, __op__) \
	namespace yggr { \
		template<typename T> \
		struct BOOST_PP_CAT(__op_name__, _operator_result) { \
			public: \
			typedef \
				YGGR_TYPEOF_TPL( \
					YGGR_PP_OPEN( __op__ ) \
					declval< typename ::yggr::detail::operator_result_t_getter<T>::type >() ) type; }; }

#define YGGR_PP_TYPE_TRAITS_AFTER_UNARY_OPERATOR_RESULT(__op_name__, __op__) \
	namespace yggr { \
		template<typename T> \
		struct BOOST_PP_CAT(__op_name__, _operator_result) { \
			public: \
			typedef \
				YGGR_TYPEOF_TPL( \
					declval< typename ::yggr::detail::operator_result_t_getter<T>::type >() \
					YGGR_PP_OPEN( __op__ ) ) type; }; }

// math operator

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4804)
#	pragma warning (disable : 4805)
#endif //_MSC_VER

//positive_operator_result (+a)
YGGR_PP_TYPE_TRAITS_BEFORE_UNARY_OPERATOR_RESULT(positive, YGGR_PP_SYMBOL_POSITIVE)

//negate_operator_result (-a)
YGGR_PP_TYPE_TRAITS_BEFORE_UNARY_OPERATOR_RESULT(negate, YGGR_PP_SYMBOL_NEGATIVE)

//plus_operator_result (a + b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(plus, YGGR_PP_SYMBOL_PLUS)

//minus_operator_result (a - b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(minus, YGGR_PP_SYMBOL_MINUS)

//multiplies_operator_result (a * b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(multiplies, YGGR_PP_SYMBOL_MUL)

//divides_operator_result (a / b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(divides, YGGR_PP_SYMBOL_DIV)

//modulus_operator_result (a % b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(modulus, YGGR_PP_SYMBOL_DIV)

// bit operator

//bitwise_and_operator_result (a & b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(bitwise_and, YGGR_PP_SYMBOL_BITWISE_AND)

//bitwise_or_operator_result (a | b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(bitwise_or, YGGR_PP_SYMBOL_BITWISE_OR)

//bitwise_xor_operator_result (a ^ b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(bitwise_xor, YGGR_PP_SYMBOL_BITWISE_XOR)

//bitwise_left_shift_operator_result (a << b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(bitwise_left_shift, YGGR_PP_SYMBOL_BITWISE_LEFT_SHIFT)

//bitwise_right_shift_operator_result (a >> b)
YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT(bitwise_right_shift, YGGR_PP_SYMBOL_BITWISE_RIGHT_SHIFT)

//bitwise_not_operator_result (~a)
YGGR_PP_TYPE_TRAITS_BEFORE_UNARY_OPERATOR_RESULT(bitwise_not, YGGR_PP_SYMBOL_BITWISE_NOT)

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#undef YGGR_PP_TYPE_TRAITS_BINARY_OPERATOR_RESULT
#undef YGGR_PP_TYPE_TRAITS_BEFORE_UNARY_OPERATOR_RESULT
#undef YGGR_PP_TYPE_TRAITS_AFTER_UNARY_OPERATOR_RESULT

#endif // __YGGR_TYPE_TRAITS_OPERATOR_RESUTL_HPP__
