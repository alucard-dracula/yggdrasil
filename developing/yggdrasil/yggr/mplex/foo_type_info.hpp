// foo_type_info.hpp

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

#ifndef __YGGR_MPLEX_FOO_TYPE_INFO_HPP__
#define __YGGR_MPLEX_FOO_TYPE_INFO_HPP__

#include <yggr/ppex/typedef.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/mplex/args.hpp>

#define YGGR_PP_FOO_TYPE_INFO(__n__) \
template< typename R YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA ) YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename A ) > \
struct BOOST_PP_CAT( foo_type_info_, __n__ ) { \
	typedef R result_type; \
	typedef R ( * foo_type )( YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, A ) ); \
	typedef boost::mpl::vector< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, A ) > params_type; \
	template<size_type N> struct arg { \
		typedef typename get_arg<params_type, N>::type type; }; };

#define YGGR_PP_MEMBER_FOO_TYPE_INFO(__n__) \
template< typename R, typename Class YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA ) YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename A ), bool is_const > \
struct BOOST_PP_CAT( member_foo_type_info_, __n__ ) { \
	typedef R result_type; \
	typedef Class class_type; \
	typedef typename boost::mpl::if_c< \
			is_const, \
			R ( Class::* )( YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, A ) ) const, \
			R ( Class::* )( YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, A ) ) >::type foo_type; \
	typedef boost::mpl::vector< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, A ) > params_type; \
	template<size_type N> struct arg { \
		typedef typename get_arg<params_type, N>::type type; }; };

namespace yggr
{
namespace mplex
{

#define BOOST_PP_LOCAL_MACRO(__n__) \
	YGGR_PP_FOO_TYPE_INFO(__n__)
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )


#define BOOST_PP_LOCAL_MACRO(__n__) \
	YGGR_PP_MEMBER_FOO_TYPE_INFO(__n__)
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )

template<typename T>
struct foo_typdef_helper
{
	typedef T type;
};


} // namesapce mplex
} // namespace yggr

#undef YGGR_PP_FOO_TYPE_INFO
#undef YGGR_PP_MEMBER_FOO_TYPE_INFO

#endif // __YGGR_MPLEX_FOO_TYPE_INFO_HPP__