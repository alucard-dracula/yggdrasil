//is_xxx_container.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_IS_XXX_CONTAINER_HPP__
#define __YGGR_CONTAINER_DETAIL_IS_XXX_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/typedef.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/cat.hpp>

#define YGGR_PP_IS_XXX_CONTAINER_DEF(__n__, __name__, __container__) \
	namespace yggr { namespace container { \
		template< typename Container > \
		struct BOOST_PP_CAT(is_, __name__) \
			: public boost::mpl::false_ {}; \
		\
		template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T)> \
		struct BOOST_PP_CAT(is_, __name__)< __container__<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T)> > \
			: public boost::mpl::true_ {}; } }

#endif // __YGGR_CONTAINER_DETAIL_IS_XXX_CONTAINER_HPP__