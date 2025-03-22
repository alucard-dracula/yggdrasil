//operator_tags.hpp

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

#ifndef __YGGR_MPLEX_OPERATOR_TAGS_HPP__
#define __YGGR_MPLEX_OPERATOR_TAGS_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <boost/preprocessor/cat.hpp>

#include <boost/type_traits/is_base_of.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>

#define YGGR_PP_MAKE_OPERATOR_TAG(__name__, __is_after__, __is_mutable__, __param_size__) \
	namespace yggr { namespace mplex { \
		struct BOOST_PP_CAT(op_tag_, __name__) \
			: public boost::mpl::vector< \
						boost::mpl::bool_< (__is_after__) >, \
						boost::mpl::bool_< (__is_mutable__) >, \
						boost::mpl::int_< (__param_size__) > > { \
				typedef \
					boost::mpl::vector< \
					boost::mpl::bool_< (__is_after__) >, \
					boost::mpl::bool_< (__is_mutable__) >, \
					boost::mpl::int_< (__param_size__) > > base_type; \
			}; \
	\
	inline BOOST_PP_CAT(op_tag_, __name__) BOOST_PP_CAT(make_op_tag_, __name__)(void) { \
		return BOOST_PP_CAT(op_tag_, __name__)(); } } }

namespace yggr
{
namespace mplex
{

namespace detail
{

template<typename T>
struct is_operator_tag_bool_t
	: public boost::mpl::false_
{
};

template<bool B>
struct is_operator_tag_bool_t< boost::mpl::bool_<B> >
	: public boost::mpl::true_
{
};

template<typename T>
struct is_operator_tag_int_t
	: public boost::mpl::false_
{
};

template<int N>
struct is_operator_tag_int_t< boost::mpl::int_<N> >
	: public boost::mpl::true_
{
};

template<typename T>
struct is_operator_tag_vector_t
	: public boost::mpl::false_
{
};

template<typename T1, typename T2, typename T3>
struct is_operator_tag_vector_t< boost::mpl::vector<T1, T2, T3> >
	: public
		boost::mpl::and_
		<
			typename is_operator_tag_bool_t<T1>::type,
			typename is_operator_tag_bool_t<T2>::type,
			typename is_operator_tag_int_t<T3>::type
		>::type
{
};

} // namespace detail

template<typename T>
struct is_operator_tag
	: public detail::is_operator_tag_vector_t<typename T::base_type>::type	
{
};

template<typename Tag>
struct operator_is_after
	: public boost::mpl::at_c<typename Tag::base_type, 0>::type
{
	BOOST_MPL_ASSERT((is_operator_tag<Tag>));
};

template<typename Tag>
struct operator_is_mutable
	: public boost::mpl::at_c<typename Tag::base_type, 1>::type
{
	BOOST_MPL_ASSERT((is_operator_tag<Tag>));
};

template<typename Tag>
struct operator_parms_size
	: public boost::mpl::at_c<typename Tag::base_type, 2>::type
{
	BOOST_MPL_ASSERT((is_operator_tag<Tag>));
};

} // namespace mplex
} // namespace yggr

// example: struct op_tag_positive
// #define YGGR_PP_MAKE_OPERATOR_TAG(__name__, __is_after__, __is_mutable__, __param_size__)

YGGR_PP_MAKE_OPERATOR_TAG(positive, 0, 0, 1) // +a
YGGR_PP_MAKE_OPERATOR_TAG(negate, 0, 0, 1) // -a

YGGR_PP_MAKE_OPERATOR_TAG(head_increment, 0, 1, 1) // ++a
YGGR_PP_MAKE_OPERATOR_TAG(tail_increment, 1, 1, 1) // a++

YGGR_PP_MAKE_OPERATOR_TAG(head_decrement, 0, 1, 1) // --a
YGGR_PP_MAKE_OPERATOR_TAG(tail_decrement, 1, 1, 1) // a--

YGGR_PP_MAKE_OPERATOR_TAG(set, 0, 1, 2) // =

YGGR_PP_MAKE_OPERATOR_TAG(plus, 0, 0, 2) // +
YGGR_PP_MAKE_OPERATOR_TAG(plus_set, 0, 1, 2) // +=

YGGR_PP_MAKE_OPERATOR_TAG(minus, 0, 0, 2) // -
YGGR_PP_MAKE_OPERATOR_TAG(minus_set, 0, 1, 2) // -=

YGGR_PP_MAKE_OPERATOR_TAG(multiplies, 0, 0, 2) // *
YGGR_PP_MAKE_OPERATOR_TAG(multiplies_set, 0, 1, 2) // *=

YGGR_PP_MAKE_OPERATOR_TAG(divides, 0, 0, 2) // /
YGGR_PP_MAKE_OPERATOR_TAG(divides_set, 0, 1, 2) // /=

YGGR_PP_MAKE_OPERATOR_TAG(modulus, 0, 0, 2) // %
YGGR_PP_MAKE_OPERATOR_TAG(modulus_set, 0, 1, 2) // %=

YGGR_PP_MAKE_OPERATOR_TAG(logic_and, 0, 0, 2) // &&
YGGR_PP_MAKE_OPERATOR_TAG(logic_or, 0, 0, 2) // ||
YGGR_PP_MAKE_OPERATOR_TAG(logic_not, 0, 0, 1) // !

YGGR_PP_MAKE_OPERATOR_TAG(bitwise_and, 0, 0, 2) // &
YGGR_PP_MAKE_OPERATOR_TAG(bitwise_and_set, 0, 1, 2) // &=

YGGR_PP_MAKE_OPERATOR_TAG(bitwise_or, 0, 0, 2) // |
YGGR_PP_MAKE_OPERATOR_TAG(bitwise_or_set, 0, 1, 2) // |=

YGGR_PP_MAKE_OPERATOR_TAG(bitwise_xor, 0, 0, 2) // ^
YGGR_PP_MAKE_OPERATOR_TAG(bitwise_xor_set, 0, 1, 2) // ^=

YGGR_PP_MAKE_OPERATOR_TAG(bitwise_left_shift, 0, 0, 2) // <<
YGGR_PP_MAKE_OPERATOR_TAG(bitwise_left_shift_set, 0, 1, 2) // <<=

YGGR_PP_MAKE_OPERATOR_TAG(bitwise_right_shift, 0, 0, 2) // >>
YGGR_PP_MAKE_OPERATOR_TAG(bitwise_right_shift_set, 0, 1, 2) // >>=

YGGR_PP_MAKE_OPERATOR_TAG(bitwise_not, 0, 0, 1) // ~

YGGR_PP_MAKE_OPERATOR_TAG(equal_to, 0, 0, 2) // ==
YGGR_PP_MAKE_OPERATOR_TAG(not_equal_to, 0, 0, 2) // !=
YGGR_PP_MAKE_OPERATOR_TAG(less, 0, 0, 2) // <
YGGR_PP_MAKE_OPERATOR_TAG(less_equal, 0, 0, 2) // <=
YGGR_PP_MAKE_OPERATOR_TAG(greater, 0, 0, 2) // >
YGGR_PP_MAKE_OPERATOR_TAG(greater_equal, 0, 0, 2) // >=

#undef YGGR_PP_MAKE_OPERATOR_TAG

#endif // __YGGR_MPLEX_OPERATOR_TAGS_HPP__
