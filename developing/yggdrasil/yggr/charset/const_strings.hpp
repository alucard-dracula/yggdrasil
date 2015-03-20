//const_strings.hpp

#ifndef __YGGR_CHARSET_CONST_STRINGS_HPP__
#define __YGGR_CHARSET_CONST_STRINGS_HPP__

#include <boost/preprocessor/cat.hpp>

#define YGGR_PP_CONST_CHAR_LOCAL_DEF( __name__, __char_val__ ) \
	struct __name__ { \
		inline static char value( char* ) { return __char_val__; } \
		inline static wchar_t value( wchar_t* ) { return BOOST_PP_CAT( L, __char_val__ ); }};

#define YGGR_PP_CONST_STRING_LOCAL_DEF( __name__, __string__ ) \
	struct __name__ { \
		inline static const char* value( char* ) { return __string__; } \
		inline static const wchar_t* value( wchar_t* ) { return BOOST_PP_CAT( L, __string__ ); }};

#define YGGR_PP_CONST_CHAR_GLOBAL_DEF( __name__, __char_val__ ) \
	namespace yggr{ namespace charset{ namespace const_chars{ \
	YGGR_PP_CONST_CHAR_LOCAL_DEF( __name__, __char_val__ ) }}}

#define YGGR_PP_CONST_STRING_GLOBAL_DEF( __name__, __string__ ) \
	namespace yggr{ namespace charset{ namespace const_strings{ \
	YGGR_PP_CONST_STRING_LOCAL_DEF( __name__, __string__ ) }}}

#define YGGR_PP_CONST_CHAR_LOCAL_GET( __name__, __char__ ) \
	 __name__::value( static_cast< __char__* >( 0 ) )

#define YGGR_PP_CONST_STRING_LOCAL_GET( __name__, __char__ ) \
	 __name__::value( static_cast< __char__* >( 0 ) )

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
} // namespace charset_set
} // namespace yggr

namespace yggr
{
namespace charset
{
namespace const_chars
{
} // namespace const_chars
} // namespace charset_set
} // namespace yggr

#endif // __YGGR_CHARSET_CONST_STRINGS_HPP__
