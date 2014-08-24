//container_implementation_level_def.hpp

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

#ifndef __YGGR_SERIALIZATION_DETAIL_IMPLEMENTATION_LEVEL_DEF_HPP__
#define __YGGR_SERIALIZATION_DETAIL_IMPLEMENTATION_LEVEL_DEF_HPP__

#include <boost/serialization/collection_traits.hpp>
#include <yggr/ppex/typedef.hpp>

#define YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(__template_n__, __type__) \
namespace boost { namespace serialization { \
template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, typename T) > \
struct implementation_level< __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, T) > > { \
    typedef mpl::integral_c_tag tag; \
    typedef mpl::int_<object_serializable> type; \
	BOOST_STATIC_CONSTANT(int, value = object_serializable); }; } } 

#endif // __YGGR_SERIALIZATION_DETAIL_IMPLEMENTATION_LEVEL_DEF_HPP__