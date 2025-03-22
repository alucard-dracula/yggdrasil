// make_zero_value.hpp

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

#ifndef __YGGR_MATH_MAKE_ZERO_VALUE_HPP__
#define __YGGR_MATH_MAKE_ZERO_VALUE_HPP__

#include <yggr/base/yggrdef.h>

#ifdef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	include <yggr/ppex/foo_params.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

namespace yggr
{
namespace math
{

namespace detail
{

template<typename TVal>
struct make_zero_value_t
{
public:
	typedef TVal value_type;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	value_type make_zero_value(BOOST_FWD_REF(Args)... args)
	{
		value_type val{ boost::forward<Args>(args)...};
		return val - val;
	}

#else

	template<typename T> inline
	value_type make_zero_value(void)
	{
		value_type val;
		return val - val; // must using this style in msvc10
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		value_type make_zero_value( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			value_type val( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return val - val; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

};

} // namespace detail

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename T, typename ...Args> inline
T make_zero_value(BOOST_FWD_REF(Args)... args)
{
	typedef detail::make_zero_value_t<T> make_type;
	make_type make;
	return make(boost::forward<Args>(args)...);
}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< \
			typename T \
			YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) >  inline \
		T make_zero_value( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef detail::make_zero_value_t<T> make_type; \
			make_type make; \
			return make(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


} // namespace math

using math::make_zero_value;

} // namespace yggr

#endif // __YGGR_MATH_MAKE_ZERO_VALUE_HPP__

