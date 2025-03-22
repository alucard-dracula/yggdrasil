//indexes_range_size_def.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_RANGE_SIZE_DEF_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_RANGE_SIZE_DEF_HPP__

#include <yggr/safe_container/detail/range_size_def.hpp>

#include <boost/range/size.hpp>

#define YGGR_PP_SAFE_MIDX_INDEXES_RANGE_SIZE_SUPPORT( __type__ ) \
	template< int N, typename Multi_Index_Container, typename Mutex > inline \
	typename __type__< N, Multi_Index_Container, Mutex >::size_type \
		range_calculate_size(const __type__< N, Multi_Index_Container, Mutex >& c ) { return c.size(); } 

// boost::size() meta check support

#if (defined(BOOST_RANGE_ENABLE_CONCEPT_ASSERT)) && (BOOST_RANGE_ENABLE_CONCEPT_ASSERT == 1)

#	define YGGR_PP_SAFE_MIDX_INDEXES_RANGE_SIZE_CHECK_PASS_SUPPORT( __type__ ) \
		namespace boost { \
			template< int N, typename Multi_Index_Container, typename Mutex > \
			struct SinglePassRangeConcept< __type__< N, Multi_Index_Container, Mutex > > { \
				YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_DETAIL( \
					__type__< N YGGR_PP_SYMBOL_COMMA() Multi_Index_Container YGGR_PP_SYMBOL_COMMA() Mutex >) }; }

#else

#	define YGGR_PP_SAFE_MIDX_INDEXES_RANGE_SIZE_CHECK_PASS_SUPPORT( __type__ )

#endif // BOOST_RANGE_ENABLE_CONCEPT_ASSERT

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_RANGE_SIZE_DEF_HPP__
