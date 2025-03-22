// any_operator_mgr_reg_macro.hpp

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

#ifndef __YGGR_ANY_VAL_ANY_OPERATOR_MGR_REG_MACRO_HPP__
#define __YGGR_ANY_VAL_ANY_OPERATOR_MGR_REG_MACRO_HPP__

#include <yggr/base/yggrdef.h>

/*
in mingw to reduce the size of the lib, deleted the non-safe data type operation, 
because the mingw's ld.exe of 32-bit will appear out of memory.
*/

//#if defined(__MINGW32__)
//#	if !defined(YGGR_ANY_VAL_OP_INIT_NOT_SUPPORT_BASE_TYPE)
//#		define YGGR_ANY_VAL_OP_INIT_NOT_SUPPORT_BASE_TYPE
//#	endif //YGGR_ANY_VAL_OP_INIT_NOT_SUPPORT_BASE_TYPE
//#endif // __MINGW32__

//#if defined(__MINGW32__)
//#	if !defined(YGGR_ANY_VAL_OP_NOT_INIT_BASE_TYPE)
//#		define YGGR_ANY_VAL_OP_NOT_INIT_BASE_TYPE
//#	endif //YGGR_ANY_VAL_OP_NOT_INIT_BASE_TYPE
//#endif // __MINGW32__

//#if defined(__MINGW32__)
//#	if !defined(YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE)
//#		define YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE
//#	endif //YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE
//#endif // __MINGW32__

//register

#if defined(YGGR_ANY_VAL_OP_NOT_INIT_BASE_TYPE)
#	define YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR( ... ) (true)
#else
#	define YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR( __pmgr__, __foo_name__, __param__ ) \
				( __pmgr__->BOOST_PP_CAT( register_, __foo_name__ )< __param__ >() )
#endif // YGGR_ANY_VAL_OP_NOT_INIT_BASE_TYPE

#define YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR

#if defined(YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE)
#	define YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR_UNSAFE(...) (true)
#	define YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR_UNSAFE(...) (true)
#else
#	define YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR_UNSAFE YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR
#	define YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR_UNSAFE YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR
#endif // YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE

#if defined(YGGR_ANY_VAL_OP_NOT_INIT_BASE_TYPE)
#	define YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR( ... ) (true)
#else
#	define YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR( __pmgr__, __foo_name__, __left__, __right__ ) \
				( __pmgr__->BOOST_PP_CAT( register_, __foo_name__ )< __left__, __right__ >() )
#endif // YGGR_ANY_VAL_OP_NOT_INIT_BASE_TYPE

#define YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR

#if defined(YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE)
#	define YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(...) (true)
#	define YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR_UNSAFE(...) (true)
#else
#	define YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR
#	define YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR_UNSAFE YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR
#endif // YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE

//unregister
#define YGGR_PP_ANY_VAL_UNREGISTER_UNARY_OPERATOR( __pmgr__, __foo_name__, __param__ ) \
			( __pmgr__->BOOST_PP_CAT( unregister_, __foo_name__ )< __param__ >() )

#define YGGR_PP_ANY_VAL_UNREGISTER_MUTABLE_UNARY_OPERATOR YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR

#if defined(YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE)
#	define YGGR_PP_ANY_VAL_UNREGISTER_UNARY_OPERATOR_UNSAFE(...) (true)
#	define YGGR_PP_ANY_VAL_UNREGISTER_MUTABLE_UNARY_OPERATOR_UNSAFE(...) (true)
#else
#	define YGGR_PP_ANY_VAL_UNREGISTER_UNARY_OPERATOR_UNSAFE YGGR_PP_ANY_VAL_UNREGISTER_UNARY_OPERATOR
#	define YGGR_PP_ANY_VAL_UNREGISTER_MUTABLE_UNARY_OPERATOR_UNSAFE YGGR_PP_ANY_VAL_UNREGISTER_UNARY_OPERATOR
#endif // YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE

#define YGGR_PP_ANY_VAL_UNREGISTER_BINARY_OPERATOR( __pmgr__, __foo_name__, __left__, __right__ ) \
			( __pmgr__->BOOST_PP_CAT( unregister_, __foo_name__ )< __left__, __right__ >() )

#define YGGR_PP_ANY_VAL_UNREGISTER_MUTABLE_BINARY_OPERATOR YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR

#if defined(YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE)
#	define YGGR_PP_ANY_VAL_UNREGISTER_BINARY_OPERATOR_UNSAFE(...) (true)
#	define YGGR_PP_ANY_VAL_UNREGISTER_MUTABLE_BINARY_OPERATOR_UNSAFE(...) (true)
#else
#	define YGGR_PP_ANY_VAL_UNREGISTER_BINARY_OPERATOR_UNSAFE YGGR_PP_ANY_VAL_UNREGISTER_BINARY_OPERATOR
#	define YGGR_PP_ANY_VAL_UNREGISTER_MUTABLE_BINARY_OPERATOR_UNSAFE YGGR_PP_ANY_VAL_UNREGISTER_BINARY_OPERATOR
#endif // YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE

namespace yggr
{
namespace any_val
{
} // namespace any_val
} // namespace yggr

#endif // __YGGR_ANY_VAL_ANY_OPERATOR_MGR_REG_MACRO_HPP__
