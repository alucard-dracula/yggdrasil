// tags.hpp

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

#ifndef __YGGR_MPLEX_TAGS_HPP__
#define __YGGR_MPLEX_TAGS_HPP__

#include <yggr/mplex/null_t.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/preprocessor/cat.hpp>

#define YGGR_PP_TAGS_DEF( __name__ ) \
	struct BOOST_PP_CAT( tag_, __name__ ){};

namespace yggr
{
namespace mplex
{

typedef null_type tag_null;

YGGR_PP_TAGS_DEF(positive)
YGGR_PP_TAGS_DEF(negative)

YGGR_PP_TAGS_DEF(plus)
YGGR_PP_TAGS_DEF(minus)
YGGR_PP_TAGS_DEF(mul)
YGGR_PP_TAGS_DEF(div)
YGGR_PP_TAGS_DEF(mod)

YGGR_PP_TAGS_DEF(inc)
YGGR_PP_TAGS_DEF(dec)

YGGR_PP_TAGS_DEF(equal_to)
YGGR_PP_TAGS_DEF(not_equal_to)
YGGR_PP_TAGS_DEF(less)
YGGR_PP_TAGS_DEF(less_equal)
YGGR_PP_TAGS_DEF(greater)
YGGR_PP_TAGS_DEF(greater_equal)

YGGR_PP_TAGS_DEF(logic_and)
YGGR_PP_TAGS_DEF(logic_or)
YGGR_PP_TAGS_DEF(logic_not)

YGGR_PP_TAGS_DEF(bitwise_and)
YGGR_PP_TAGS_DEF(bitwise_or)
YGGR_PP_TAGS_DEF(bitwise_not)
YGGR_PP_TAGS_DEF(bitwise_xor)
YGGR_PP_TAGS_DEF(bitwise_left_shift)
YGGR_PP_TAGS_DEF(bitwise_right_shift)

YGGR_PP_TAGS_DEF(set)
YGGR_PP_TAGS_DEF(plus_set)
YGGR_PP_TAGS_DEF(minus_set)
YGGR_PP_TAGS_DEF(mul_set)
YGGR_PP_TAGS_DEF(div_set)
YGGR_PP_TAGS_DEF(mod_set)
YGGR_PP_TAGS_DEF(bitwise_and_set)
YGGR_PP_TAGS_DEF(bitwise_or_set)
YGGR_PP_TAGS_DEF(bitwise_xor_set)
YGGR_PP_TAGS_DEF(bitwise_left_shift_set)
YGGR_PP_TAGS_DEF(bitwise_right_shift_set)

YGGR_PP_TAGS_DEF(parentheses)
YGGR_PP_TAGS_DEF(parentheses_l)
YGGR_PP_TAGS_DEF(parentheses_r)

YGGR_PP_TAGS_DEF(brackets)
YGGR_PP_TAGS_DEF(brackets_l)
YGGR_PP_TAGS_DEF(brackets_r)

YGGR_PP_TAGS_DEF(braces)
YGGR_PP_TAGS_DEF(braces_l)
YGGR_PP_TAGS_DEF(braces_r)

YGGR_PP_TAGS_DEF(angle_brackets)
YGGR_PP_TAGS_DEF(angle_brackets_l)
YGGR_PP_TAGS_DEF(angle_brackets_r)

YGGR_PP_TAGS_DEF(rvalue_ref)
YGGR_PP_TAGS_DEF(ref)
YGGR_PP_TAGS_DEF(ind)

YGGR_PP_TAGS_DEF(member_ref)
YGGR_PP_TAGS_DEF(member_der)

YGGR_PP_TAGS_DEF(pointer_to_member_ref)
YGGR_PP_TAGS_DEF(pointer_to_member_der)

YGGR_PP_TAGS_DEF(empty)

YGGR_PP_TAGS_DEF(comma)

YGGR_PP_TAGS_DEF(semicolon)

YGGR_PP_TAGS_DEF(question_mark)

YGGR_PP_TAGS_DEF(colon)

YGGR_PP_TAGS_DEF(area)

YGGR_PP_TAGS_DEF(slash)

YGGR_PP_TAGS_DEF(ellipsis)

YGGR_PP_TAGS_DEF(number_zero)
YGGR_PP_TAGS_DEF(number_one)
YGGR_PP_TAGS_DEF(number_two)
YGGR_PP_TAGS_DEF(number_three)
YGGR_PP_TAGS_DEF(number_four)
YGGR_PP_TAGS_DEF(number_five)
YGGR_PP_TAGS_DEF(number_six)
YGGR_PP_TAGS_DEF(number_seven)
YGGR_PP_TAGS_DEF(number_eight)
YGGR_PP_TAGS_DEF(number_nine)

YGGR_PP_TAGS_DEF(number_null)
YGGR_PP_TAGS_DEF(number_eins)
YGGR_PP_TAGS_DEF(number_zwei)
YGGR_PP_TAGS_DEF(number_drei)
YGGR_PP_TAGS_DEF(number_vier)
YGGR_PP_TAGS_DEF(number_funf)
YGGR_PP_TAGS_DEF(number_sechs)
YGGR_PP_TAGS_DEF(number_sieben)
YGGR_PP_TAGS_DEF(number_acht)
YGGR_PP_TAGS_DEF(number_neun)
YGGR_PP_TAGS_DEF(bool_false)
YGGR_PP_TAGS_DEF(bool_true)

} // namespace mplex

typedef mplex::tag_null tag_non_handler;

#if defined(__BORLANDC__) || (defined(__GNUC__) && (__GNUC__ < 4)) 

inline tag_non_handler __non_handler(){ return tag_non_handler(); }
#	define non_handler __non_handler()

#elif defined(BOOST_MSVC) \
        || (defined(__DECCXX_VER) && __DECCXX_VER <= 60590031) \
        || defined(__MWERKS__) \
        || (defined(__GNUC__) /*&& (__GNUC__ == 4 && __GNUC_MINOR__ < 2)*/)

static tag_non_handler non_handler;

#else

tag_non_handler non_handler;

#endif // __BORLANDC__

namespace detail
{

struct __fix_non_handler_defined_but_not_used_t
{
	inline static tag_non_handler __fix_non_handler_defined_but_not_used(void) { return ::yggr::non_handler; }
};

} //namespace detail

} // namespace yggr

#endif // __YGGR_MPLEX_TAGS_HPP__
