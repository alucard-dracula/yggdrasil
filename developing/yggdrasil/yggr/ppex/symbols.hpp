//symbols.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#include <yggr/move/config.hpp>

#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <yggr/ppex/open.hpp>

#define YGGR_PP_SYMBOL YGGR_PP_OPEN

// math symbol
#define YGGR_PP_SYMBOL_NEGATIVE() -
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
#define YGGR_PP_SYMBOL_BIT_AND() &
#define YGGR_PP_SYMBOL_BIT_OR() |
#define YGGR_PP_SYMBOL_BIT_NOT() ~
#define YGGR_PP_SYMBOL_BIT_XOR() ^
#define YGGR_PP_SYMBOL_BIT_LEFT_SHIFT() <<
#define YGGR_PP_SYMBOL_BIT_RIGHT_SHIFT() >>

// equal
#define YGGR_PP_SYMBOL_EQUAL() =
#define YGGR_PP_SYMBOL_PLUS_EQUAL() +=
#define YGGR_PP_SYMBOL_MINUS_EQUAL() -=
#define YGGR_PP_SYMBOL_MUL_EQUAL() *=
#define YGGR_PP_SYMBOL_DIV_EQUAL() /=
#define YGGR_PP_SYMBOL_MOD_EQUAL() %=

#define YGGR_PP_SYMBOL_BIT_AND_EQUAL() &=
#define YGGR_PP_SYMBOL_BIT_OR_EQUAL() |=
#define YGGR_PP_SYMBOL_BIT_NOT_EQUAL() ~=
#define YGGR_PP_SYMBOL_BIT_XOR_EQUAL() ^=
#define YGGR_PP_SYMBOL_BIT_LEFT_SHIFT_EQUAL() <<=
#define YGGR_PP_SYMBOL_BIT_RIGHT_SHIFT_EQUAL() >>=

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
#	define YGGR_SYMBOL_RIGHT_REF() &&
#else
#	define YGGR_SYMBOL_RIGHT_REF YGGR_PP_SYMBOL_REF
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

#define YGGR_PP_SYMBOL_REF() &
#define YGGR_PP_SYMBOL_IND() *

#define YGGR_PP_SYMBOL_MEMBER_REF() .
#define YGGR_PP_SYMOBL_MEMBER_DER() ->

//other symbol
#define YGGR_PP_SYMBOL_EMPTY BOOST_PP_EMPTY
#define YGGR_PP_SYMBOL_COMMA BOOST_PP_COMMA

#define YGGR_PP_SYMBOL_SEMICOLON() ;

#define YGGR_PP_SYMBOL_QUESTION_MARK() ?
#define YGGR_PP_SYMBOL_COLON() :

#define YGGR_PP_SYMBOL_AREA() ::

namespace yggr
{
namespace ppex
{
} // namespace ppex
} //namespace yggr

#endif // __YGGR_PPEX_SYMBOLS_HPP__
