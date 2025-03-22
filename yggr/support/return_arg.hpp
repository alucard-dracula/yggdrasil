//return_arg.hpp

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

#ifndef __YGGR_SUPPORT_RETURN_ARG_HPP__
#define __YGGR_SUPPORT_RETURN_ARG_HPP__

#include <yggr/base/yggrdef.h>

#if defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/symbols.hpp>

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

namespace yggr
{
namespace support
{

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename ...Args> YGGR_CXX23_CONSTEXPR_OR_INLINE
void return_void(BOOST_FWD_REF(Args)... args) YGGR_NOEXCEPT_OR_NOTHROW
{
	return;
}

template<typename EArg, typename ...Args> YGGR_CXX23_CONSTEXPR_OR_INLINE
EArg& return_first_arg(EArg& first, BOOST_FWD_REF(Args)... args) YGGR_NOEXCEPT_OR_NOTHROW
{
	return first;
}

template<typename ...Args, typename LArg> YGGR_CXX23_CONSTEXPR_OR_INLINE
LArg& return_last_arg(BOOST_FWD_REF(Args)... args, LArg& last) YGGR_NOEXCEPT_OR_NOTHROW
{
	return last;
}

#else

// return_void
#	define YGGR_TMP_PP_FOO_RETURN_VOID( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) YGGR_CXX23_CONSTEXPR_OR_INLINE \
		void return_void( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) YGGR_NOEXCEPT_OR_NOTHROW { return; }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_FOO_RETURN_VOID( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_FOO_RETURN_VOID

// return_first_arg
#	define YGGR_TMP_PP_FOO_RETURN_FIRST_ARG( __n__ ) \
		template< \
			typename EArg \
			YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) >  YGGR_CXX23_CONSTEXPR_OR_INLINE \
		EArg& return_first_arg( EArg& first \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) YGGR_NOEXCEPT_OR_NOTHROW { \
			return first; }


#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_FOO_RETURN_FIRST_ARG( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_FOO_RETURN_FIRST_ARG

// return_last_arg
#	define YGGR_TMP_PP_FOO_RETURN_LAST_ARG( __n__ ) \
		template< \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
			YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
			typename LArg >  YGGR_CXX23_CONSTEXPR_OR_INLINE \
		LArg& return_last_arg( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								LArg& last) YGGR_NOEXCEPT_OR_NOTHROW { \
			return last; } 


#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_FOO_RETURN_LAST_ARG( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_FOO_RETURN_LAST_ARG

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace support

using support::return_void;
using support::return_first_arg;
using support::return_last_arg;

} // namespace yggr

#endif // __YGGR_SUPPORT_RETURN_ARG_HPP__
