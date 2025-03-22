//const_strings.hpp

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

#ifndef __YGGR_CHARSET_CONST_STRINGS_HPP__
#define __YGGR_CHARSET_CONST_STRINGS_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/preprocessor/cat.hpp>
#include <cstddef> //char16_t, char32_t

// --------------local def detail-------------------
#if (defined(YGGR_NO_CWCHAR) && defined(YGGR_NO_CHAR8_T) && defined(YGGR_NO_CHAR16_T) && defined(YGGR_NO_CHAR32_T))
#	define YGGR_PP_CONST_CHAR_LOCAL_CHAR_FOO_IMPL( __char_val__ ) \
		YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC const char value( void* ) { return __char_val__; }

#	define YGGR_PP_CONST_STRING_LOCAL_CHAR_FOO_IMPL( __string__ ) \
		YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC const char* value( void* ) { \
			YGGR_CXX14_CONSTEXPR_OR_STATIC const char* str = __string__; return str; }
#else
#	define YGGR_PP_CONST_CHAR_LOCAL_CHAR_FOO_IMPL( __char_val__ ) \
		YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC const char value( char* ) { return __char_val__; }

#	define YGGR_PP_CONST_STRING_LOCAL_CHAR_FOO_IMPL( __string__ ) \
		YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC const char* value( char* ) { \
			YGGR_CXX14_CONSTEXPR_OR_STATIC const char* str = __string__; return str; }
#endif // (defined(YGGR_NO_CWCHAR) && defined(YGGR_NO_CHAR8_T) && defined(YGGR_NO_CHAR16_T) && defined(YGGR_NO_CHAR32_T))


#ifndef YGGR_NO_CWCHAR
#	define YGGR_PP_CONST_CHAR_LOCAL_WCHAR_FOO_IMPL( __char_val__ ) \
		YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC const wchar_t value( wchar_t* ) { return BOOST_PP_CAT( L, __char_val__ ); }

#	define YGGR_PP_CONST_STRING_LOCAL_WCHAR_FOO_IMPL( __string__ ) \
		YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC const wchar_t* value( wchar_t* ) { \
			YGGR_CXX14_CONSTEXPR_OR_STATIC const wchar_t* str = BOOST_PP_CAT( L, __string__ ); return str; }
#else
#	define YGGR_PP_CONST_CHAR_LOCAL_WCHAR_FOO_IMPL( __char_val__ )

#	define YGGR_PP_CONST_STRING_LOCAL_WCHAR_FOO_IMPL( __string__ )
#endif // BOOST_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
#	define YGGR_PP_CONST_CHAR_LOCAL_CHAR8_FOO_IMPL( __char_val__ ) \
		YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC const ::yggr::c8 value( const ::yggr::c8* ) { return BOOST_PP_CAT( u8, __char_val__ ); }

#	define YGGR_PP_CONST_STRING_LOCAL_CHAR8_FOO_IMPL( __string__ ) \
		YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC const ::yggr::c8* value( const ::yggr::c8* ) { \
			YGGR_CXX14_CONSTEXPR_OR_STATIC const ::yggr::c8* str = BOOST_PP_CAT( u8, __string__ ); return str; }
#else
#	define YGGR_PP_CONST_CHAR_LOCAL_CHAR8_FOO_IMPL( __char_val__ )

#	define YGGR_PP_CONST_STRING_LOCAL_CHAR8_FOO_IMPL( __string__ )
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
#	define YGGR_PP_CONST_CHAR_LOCAL_CHAR16_FOO_IMPL( __char_val__ ) \
		YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC const ::yggr::c16 value( const ::yggr::c16* ) { return BOOST_PP_CAT( u, __char_val__ ); }

#	define YGGR_PP_CONST_STRING_LOCAL_CHAR16_FOO_IMPL( __string__ ) \
		YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC const ::yggr::c16* value( const ::yggr::c16* ) { \
			YGGR_CXX14_CONSTEXPR_OR_STATIC const ::yggr::c16* str = BOOST_PP_CAT( u, __string__ ); return str; }
#else
#	define YGGR_PP_CONST_CHAR_LOCAL_CHAR16_FOO_IMPL( __char_val__ )

#	define YGGR_PP_CONST_STRING_LOCAL_CHAR16_FOO_IMPL( __string__ )
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
#	define YGGR_PP_CONST_CHAR_LOCAL_CHAR32_FOO_IMPL( __char_val__ ) \
		YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC const ::yggr::c32 value( const ::yggr::c32* ) { return BOOST_PP_CAT( U, __char_val__ ); }

#	define YGGR_PP_CONST_STRING_LOCAL_CHAR32_FOO_IMPL( __string__ ) \
		YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC const ::yggr::c32* value( const ::yggr::c32* ) { \
			YGGR_CXX14_CONSTEXPR_OR_STATIC const ::yggr::c32* str = BOOST_PP_CAT( U, __string__ ); return str; }
#else
#	define YGGR_PP_CONST_CHAR_LOCAL_CHAR32_FOO_IMPL( __char_val__ )

#	define YGGR_PP_CONST_STRING_LOCAL_CHAR32_FOO_IMPL( __string__ )
#endif // YGGR_NO_CHAR32_T

// --------------local def-------------------
#define YGGR_PP_CONST_CHAR_LOCAL_DEF( __name__, __char_val__ ) \
	struct __name__ { \
		YGGR_PP_CONST_CHAR_LOCAL_CHAR_FOO_IMPL( __char_val__ ) \
		YGGR_PP_CONST_CHAR_LOCAL_WCHAR_FOO_IMPL( __char_val__ ) \
		YGGR_PP_CONST_CHAR_LOCAL_CHAR16_FOO_IMPL( __char_val__ ) \
		YGGR_PP_CONST_CHAR_LOCAL_CHAR32_FOO_IMPL( __char_val__ ) };

#define YGGR_PP_CONST_STRING_LOCAL_DEF( __name__, __string__ ) \
	struct __name__ { \
		YGGR_PP_CONST_STRING_LOCAL_CHAR_FOO_IMPL( __string__ ) \
		YGGR_PP_CONST_STRING_LOCAL_WCHAR_FOO_IMPL( __string__ ) \
		YGGR_PP_CONST_STRING_LOCAL_CHAR16_FOO_IMPL( __string__ ) \
		YGGR_PP_CONST_STRING_LOCAL_CHAR32_FOO_IMPL( __string__ ) };

//--------------local get---------------------
#define YGGR_PP_CONST_CHAR_LOCAL_GET( __name__, __char__ ) \
	 __name__::value( static_cast< __char__* >( 0 ) )

#define YGGR_PP_CONST_STRING_LOCAL_GET( __name__, __char__ ) \
	 __name__::value( static_cast< __char__* >( 0 ) )

// --------------global def-------------------
#define YGGR_PP_CONST_CHAR_GLOBAL_DEF( __name__, __char_val__ ) \
	namespace yggr{ namespace charset{ namespace const_chars{ \
	YGGR_PP_CONST_CHAR_LOCAL_DEF( __name__, __char_val__ ) }}}

#define YGGR_PP_CONST_STRING_GLOBAL_DEF( __name__, __string__ ) \
	namespace yggr{ namespace charset{ namespace const_strings{ \
	YGGR_PP_CONST_STRING_LOCAL_DEF( __name__, __string__ ) }}}

// --------------global get-------------------
#define YGGR_PP_CONST_CHAR_GLOBAL_GET( __name__, __char__ ) \
	yggr::charset::const_chars::__name__::value( static_cast< __char__* >( 0 ) )

#define YGGR_PP_CONST_STRING_GLOBAL_GET( __name__, __char__ ) \
	yggr::charset::const_strings::__name__::value( static_cast< __char__* >( 0 ) )

namespace yggr
{
namespace charset
{
namespace const_strings
{
} // namespace const_string
} // namespace charset
} // namespace yggr


#endif // __YGGR_CHARSET_CONST_STRINGS_HPP__
