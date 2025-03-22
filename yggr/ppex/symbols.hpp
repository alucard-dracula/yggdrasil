//symbols.hpp

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

#ifndef __YGGR_PPEX_SYMBOLS_HPP__
#define __YGGR_PPEX_SYMBOLS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/config.hpp>
#include <yggr/ppex/open.hpp>

#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>

#define YGGR_PP_SYMBOL YGGR_PP_OPEN

// math symbol
#define YGGR_PP_SYMBOL_POSITIVE YGGR_PP_SYMBOL_PLUS
#define YGGR_PP_SYMBOL_NEGATIVE YGGR_PP_SYMBOL_MINUS

#define YGGR_PP_SYMBOL_PLUS() +
#define YGGR_PP_SYMBOL_MINUS() -
#define YGGR_PP_SYMBOL_MUL() *
#define YGGR_PP_SYMBOL_DIV() /
#define YGGR_PP_SYMBOL_MOD() %

#define YGGR_PP_SYMBOL_INC() ++
#define YGGR_PP_SYMBOL_DEC() --

//compare
#define YGGR_PP_SYMBOL_EQUAL_TO() ==
#define YGGR_PP_SYMBOL_NOT_EQUAL_TO() !=
#define YGGR_PP_SYMBOL_LESS() <
#define YGGR_PP_SYMBOL_LESS_EQUAL() <=
#define YGGR_PP_SYMBOL_GREATER() >
#define YGGR_PP_SYMBOL_GREATER_EQUAL() >=

//logic
#define YGGR_PP_SYMBOL_LOGIC_AND() &&
#define YGGR_PP_SYMBOL_LOGIC_OR() ||
#define YGGR_PP_SYMBOL_LOGIC_NOT() !

//bitwise
#define YGGR_PP_SYMBOL_BITWISE_AND() &
#define YGGR_PP_SYMBOL_BITWISE_OR() |
#define YGGR_PP_SYMBOL_BITWISE_NOT() ~
#define YGGR_PP_SYMBOL_BITWISE_XOR() ^
#define YGGR_PP_SYMBOL_BITWISE_LEFT_SHIFT() <<
#define YGGR_PP_SYMBOL_BITWISE_RIGHT_SHIFT() >>

//set
#define YGGR_PP_SYMBOL_SET() =
#define YGGR_PP_SYMBOL_PLUS_SET() +=
#define YGGR_PP_SYMBOL_MINUS_EQUAL() -=
#define YGGR_PP_SYMBOL_MUL_SET() *=
#define YGGR_PP_SYMBOL_DIV_SET() /=
#define YGGR_PP_SYMBOL_MOD_SET() %=

#define YGGR_PP_SYMBOL_BITWISE_AND_SET() &=
#define YGGR_PP_SYMBOL_BITWISE_OR_SET() |=
#define YGGR_PP_SYMBOL_BITWISE_XOR_SET() ^=
#define YGGR_PP_SYMBOL_BITWISE_LEFT_SHIFT_SET() <<=
#define YGGR_PP_SYMBOL_BITWISE_RIGHT_SHIFT_SET() >>=

//parentheses, brackets, braces, angle brackets

#define YGGR_PP_SYMBOL_PARENTHESES() ( )
#define YGGR_PP_SYMBOL_PARENTHESES_L() (
#define YGGR_PP_SYMBOL_PARENTHESES_R() )

#define YGGR_PP_SYMBOL_BRACKETS() [ ]
#define YGGR_PP_SYMBOL_BRACKETS_L() [
#define YGGR_PP_SYMBOL_BRACKETS_R() ]

#define YGGR_PP_SYMBOL_BRACES() { }
#define YGGR_PP_SYMBOL_BRACES_L() {
#define YGGR_PP_SYMBOL_BRACES_R() }

#define YGGR_PP_SYMBOL_ANGLE_BRACKETS() < >
#define YGGR_PP_SYMBOL_ANGLE_BRACKETS_L() <
#define YGGR_PP_SYMBOL_ANGLE_BRACKETS_R() >

//pointer
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
#	define YGGR_SYMBOL_RVALUE_REF() &&
#else
#	define YGGR_SYMBOL_RVALUE_REF YGGR_PP_SYMBOL_REF
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

#define YGGR_PP_SYMBOL_REF() &
#define YGGR_PP_SYMBOL_IND() *

#define YGGR_PP_SYMBOL_MEMBER_REF() .
#define YGGR_PP_SYMOBL_MEMBER_DER() ->

#define YGGR_PP_SYMBOL_POINTER_TO_MEMBER_REF() .*
#define YGGR_PP_SYMOBL_POINTER_TO_MEMBER_DER() ->*

//other symbol
#define YGGR_PP_SYMBOL_EMPTY BOOST_PP_EMPTY
#define YGGR_PP_SYMBOL_COMMA BOOST_PP_COMMA

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define YGGR_PP_SYMBOL_EMPTY_N( __n__, ... )
#else
#	define YGGR_PP_SYMBOL_EMPTY_N( __n__, __args__ )
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

#define YGGR_PP_SYMBOL_SEMICOLON() ;

#define YGGR_PP_SYMBOL_QUESTION_MARK() ?
#define YGGR_PP_SYMBOL_COLON() :

#define YGGR_PP_SYMBOL_AREA() ::

#define YGGR_PP_SYMBOL_SLASH() /

#define YGGR_PP_SYMBOL_ELLIPSIS() ...

// number

#define YGGR_PP_SYMBOL_NUMBER_ZERO() 0
#define YGGR_PP_SYMBOL_NUMBER_ONE() 1
#define YGGR_PP_SYMBOL_NUMBER_TWO() 2
#define YGGR_PP_SYMBOL_NUMBER_THREE() 3
#define YGGR_PP_SYMBOL_NUMBER_FOUR() 4
#define YGGR_PP_SYMBOL_NUMBER_FIVE() 5
#define YGGR_PP_SYMBOL_NUMBER_SIX() 6
#define YGGR_PP_SYMBOL_NUMBER_SEVEN() 7
#define YGGR_PP_SYMBOL_NUMBER_EIGHT() 8
#define YGGR_PP_SYMBOL_NUMBER_NINE() 9

#define YGGR_PP_SYMBOL_NUMBER_NULL YGGR_PP_SYMBOL_NUMBER_ZERO
#define YGGR_PP_SYMBOL_NUMBER_EINS YGGR_PP_SYMBOL_NUMBER_ONE
#define YGGR_PP_SYMBOL_NUMBER_ZWEI YGGR_PP_SYMBOL_NUMBER_TWO
#define YGGR_PP_SYMBOL_NUMBER_DREI YGGR_PP_SYMBOL_NUMBER_THREE
#define YGGR_PP_SYMBOL_NUMBER_VIER YGGR_PP_SYMBOL_NUMBER_FOUR
#define YGGR_PP_SYMBOL_NUMBER_FUNF YGGR_PP_SYMBOL_NUMBER_FIVE
#define YGGR_PP_SYMBOL_NUMBER_SECHS YGGR_PP_SYMBOL_NUMBER_SIX
#define YGGR_PP_SYMBOL_NUMBER_SIEBEN YGGR_PP_SYMBOL_NUMBER_SEVEN
#define YGGR_PP_SYMBOL_NUMBER_ACHT YGGR_PP_SYMBOL_NUMBER_EIGHT
#define YGGR_PP_SYMBOL_NUMBER_NEUN YGGR_PP_SYMBOL_NUMBER_NINE

//bool value

#define YGGR_PP_SYMBOL_BOOL_FALSE() 0
#define YGGR_PP_SYMBOL_BOOL_TRUE() 1


namespace yggr
{
namespace ppex
{
} // namespace ppex
} //namespace yggr

#endif // __YGGR_PPEX_SYMBOLS_HPP__
