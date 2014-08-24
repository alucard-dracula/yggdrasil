//foo_params.hpp

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

#ifndef __YGGR_PPEX_FOO_PARAMS_HELPER_HPP__
#define __YGGR_PPEX_FOO_PARAMS_HELPER_HPP__

#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/open.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/typedef.hpp>

#define YGGR_PP_FOO_DEFAULT_PARAMS_LEN YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN

#define YGGR_PP_FOO_GET_ARG_NAME YGGR_PP_OPEN

#define YGGR_PP_FOO_TYPES_DEF( __count__ ) YGGR_PP_TEMPLATE_PARAMS_TYPES( __count__, typename T )
#define YGGR_PP_FOO_TYPES_GROUP( __count__ ) YGGR_PP_TEMPLATE_PARAMS_TYPES( __count__, T )

// YGGR_PP_FOO_ARG_NAME  // foo args name def

// foo params simple version
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
#	define YGGR_PP_FOO_CREF_PARAMS( __z__, __n__, _ ) \
                BOOST_PP_CAT( T, __n__ )&& BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )
#else
#	define YGGR_PP_FOO_CREF_PARAMS( __z__, __n__, _ ) \
				const BOOST_PP_CAT( T, __n__ )& BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

#define YGGR_PP_FOO_REAL_CREF_PARAMS( __z__, __n__, _ ) \
				const BOOST_PP_CAT( T, __n__ )& BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )

#define YGGR_PP_FOO_REF_PARAMS( __z__, __n__, _ ) \
			BOOST_PP_CAT( T, __n__ ) & BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )

#define YGGR_PP_FOO_VAR_PARAMS( __z__, __n__, _ ) \
			BOOST_PP_CAT( T, __n__ ) BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )

// YGGR_PP_FOO_ARG_TYPE(__n__)
// foo params custom version
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
#	define YGGR_PP_FOO_CUSTOM_CREF_PARAMS( __z__, __n__, _ ) \
				YGGR_PP_FOO_ARG_TYPE(__n__)&& BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )
#else
#	define YGGR_PP_FOO_CUSTOM_CREF_PARAMS( __z__, __n__, _ ) \
				const YGGR_PP_FOO_ARG_TYPE(__n__)& BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

#define YGGR_PP_FOO_CUSTOM_REAL_CREF_PARAMS( __z__, __n__, _ ) \
				const YGGR_PP_FOO_ARG_TYPE(__n__)& BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )

#define YGGR_PP_FOO_CUSTOM_REF_PARAMS( __z__, __n__, _ ) \
			YGGR_PP_FOO_ARG_TYPE(__n__)& BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )

#define YGGR_PP_FOO_CUSTOM_VAR_PARAMS( __z__, __n__, _ ) \
			YGGR_PP_FOO_ARG_TYPE(__n__) BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )

//---------------------------------------------------------------------------------------------------------------------

#define YGGR_PP_FOO_PARAMS_DEF( __count__, __maroc__ ) \
			BOOST_PP_ENUM( __count__, __maroc__, YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ) )

#define YGGR_PP_FOO_ARG( __n__ ) BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )

#define YGGR_PP_FOO_ARGS_OP( __z__, __n__, __op__ ) \
            YGGR_PP_SYMBOL_IF( __n__, __op__ ) BOOST_PP_CAT( YGGR_PP_FOO_GET_ARG_NAME( YGGR_PP_FOO_ARG_NAME ), __n__ )

#define YGGR_PP_FOO_PARAMS_OP( __count__, __op__ ) BOOST_PP_REPEAT( __count__, YGGR_PP_FOO_ARGS_OP, __op__ )

#define YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( __count__, __foo_custom__, __op__ ) BOOST_PP_REPEAT( __count__, __foo_custom__, __op__ )

// define YGGR_PP_FOO_CUSTOM_OP() now_use_op
#define YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __count__, __op__ ) BOOST_PP_REPEAT( __count__, YGGR_PP_FOO_CUSTOM_OP(), __op__ )


// use this format instance it
/*
*	[ret_type] foo( base_params ) {};
*
*	#define BOOST_PP_LOCAL_MACRO( __n__ ) \
*
*	template< base_tyname, ... , YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
*	[ret_type] foo( base_params, ..., YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { ..... }
*
*	[#define YGGR_PP_FOO_ARG_NAME(  ) arg_name]
*	#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
*	#include BOOST_PP_LOCAL_ITERATE(  )
*	[#undef YGGR_PP_FOO_ARG_NAME]
*/




namespace yggr
{
namespace ppex
{

} // namespace ppex
} //namesapce yggr


#endif // __YGGR_PPEX_FOO_PARAMS_HELPER_HPP__

