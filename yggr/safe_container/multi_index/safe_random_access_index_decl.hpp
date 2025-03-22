//safe_random_access_index_decl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_DECL_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_DECL_HPP__

#include <yggr/container/multi_index_random_access_index.hpp>
#include <yggr/safe_container/multi_index/inheritance_list_maker.hpp>

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

template
<
	int N,
	typename Multi_Index_Container,
	typename Mutex
>
class safe_random_access_index;

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

template<int N, 
			typename Multi_Index_Container,
			typename Mutex,
			typename S, typename T>
struct safe_index_maker
		<
			N, Multi_Index_Container, Mutex,
			boost::multi_index::detail::random_access_index
			<
				S, T
			>
		>
{
	typedef safe_random_access_index<N, Multi_Index_Container, Mutex> type;
};

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_DECL_HPP__
