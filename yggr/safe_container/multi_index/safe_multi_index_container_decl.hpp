//safe_multi_index_container_decl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_MULTI_INDEX_CONTAINER_DECL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_MULTI_INDEX_CONTAINER_DECL_HPP__

#include <yggr/mplex/typename_expand.hpp>

#include <yggr/container/multi_index_ordered_index.hpp>
#include <yggr/container/multi_index_container.hpp>
#include <yggr/safe_container/multi_index/inheritance_list_maker.hpp>

namespace yggr
{
namespace safe_container
{

template<	typename Val,
			typename IndexSpecifierList
				= boost::multi_index::indexed_by<
					boost::multi_index::ordered_unique<
						boost::multi_index::identity< Val > > >,
			typename Mutex = void,
			typename Alloc = 
				typename
					mplex::typename_expand_get
					<
						boost::multi_index::multi_index_container<Val, IndexSpecifierList>,
						2
					>::type,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc>
			class Multi_Index_Container = boost::multi_index::multi_index_container
		>
class safe_multi_index_container;

} // namespace safe_container
} // namespace yggr

#endif //__YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_MULTI_INDEX_CONTAINER_DECL_HPP__
