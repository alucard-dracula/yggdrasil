//typedef.hpp

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

#ifndef __YGGR_PPEX_TYPEDEF_HPP__
#define __YGGR_PPEX_TYPEDEF_HPP__

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <yggr/ppex/symbols.hpp>

#define YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN 10

#define YGGR_PP_TYPE( __class_name__, __x__ ) BOOST_PP_CAT( __class_name__, __x__ )

#define YGGR_PP_TEMPLATE_PARAMS_TYPES( __count__, __class_name__ ) \
			BOOST_PP_ENUM_PARAMS( __count__, __class_name__ )


// YGGR_PP_TEMPLATE_CUSTOM_TYPE(__n__)

#define YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER( __z__, __n__, _ ) \
			YGGR_PP_TEMPLATE_CUSTOM_TYPE(__n__)

#define YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __count__, __maroc__ ) \
			BOOST_PP_ENUM( __count__, __maroc__, YGGR_PP_SYMBOL_EMPTY())

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

namespace yggr
{
namespace ppex
{
} // namepace ppex
} // namespace yggr

#endif // __YGGR_PPEX_TYPEDEF_HPP__
