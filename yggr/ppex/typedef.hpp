//typedef.hpp

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

#ifndef __YGGR_PPEX_TYPEDEF_HPP__
#define __YGGR_PPEX_TYPEDEF_HPP__

#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/expand.hpp>
#include <boost/preprocessor/control/expr_if.hpp>

#ifndef YGGR_PP_TEMPLATE_PARAMS_LEN_CFG
#	define YGGR_PP_TEMPLATE_PARAMS_LEN_CFG 10
#endif // YGGR_PP_TEMPLATE_PARAMS_LEN_CFG

#define YGGR_PP_TEMPLATE_PARAMS_LEN() YGGR_PP_TEMPLATE_PARAMS_LEN_CFG

#define YGGR_PP_TYPE( __class_name__, __x__ ) BOOST_PP_CAT( __class_name__, __x__ )

#define YGGR_PP_TEMPLATE_PARAMS_TYPES( __count__, __class_name__ ) \
			BOOST_PP_ENUM_PARAMS( __count__, __class_name__ )


// define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) now_real_make( __n__ )

#define YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER( __z__, __n__, _ ) \
			YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ )

//#define YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __count__, __macro__ ) \
//			BOOST_PP_ENUM( __count__, __macro__, YGGR_PP_SYMBOL_EMPTY())

#define YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __count__, __macro__ ) \
			BOOST_PP_ENUM( __count__, __macro__, _)

#define YGGR_PP_SAME_TYPE( __z__, __n__, __class_name__ ) __class_name__

#define YGGR_PP_TEMLATE_SAME_PARAMS(__count__, __class_name__) \
			BOOST_PP_ENUM( __count__, YGGR_PP_SAME_TYPE, __class_name__ )


// define YGGR_PP_TEMPLATE_DEFAULT_TYPE() now_default_type
#define YGGR_PP_TEMPLATE_DEFAULT_TYPE_OP( __z__, __n__, __class_name__ ) \
        YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
		BOOST_PP_CAT(__class_name__, __n__) = YGGR_PP_TEMPLATE_DEFAULT_TYPE()

#define YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( __count__, __class_name__ ) \
			BOOST_PP_REPEAT( __count__, YGGR_PP_TEMPLATE_DEFAULT_TYPE_OP, __class_name__ )


#define YGGR_PP_TEMPLATE_TYPE( __class_name__, __params__ ) \
			__class_name__< __params__ >

#define YGGR_PP_TEMPLATE_TYPE2( __class_name__, __params__, __other__ ) \
			__class_name__< __params__ YGGR_PP_SYMBOL( YGGR_PP_SYMBOL_COMMA ) __other__ >

#define YGGR_PP_CLASS_TYPE( __class_name__ ) __class_name__

#define YGGR_PP_REPEAT_CUSTOM_OP_NOW( __count__, __foo_custom__, __op__ ) BOOST_PP_REPEAT( __count__, __foo_custom__, __op__ )

// define YGGR_PP_REPEAT_CUSTOM_OP() now_use_op
#define YGGR_PP_REPEAT_CUSTOM_OP_LATER( __count__, __op__ ) BOOST_PP_REPEAT( __count__, YGGR_PP_REPEAT_CUSTOM_OP(), __op__ )

#define YGGR_PP_EXPR_IF BOOST_PP_EXPR_IF

namespace yggr
{
namespace ppex
{
} // namepace ppex
} // namespace yggr

#endif // __YGGR_PPEX_TYPEDEF_HPP__
