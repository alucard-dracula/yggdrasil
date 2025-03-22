// boost_midx_seq_index_ops_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_SEQ_INDEX_OPS_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_SEQ_INDEX_OPS_OPT_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/safe_container/detail/cast_midx_index.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Destroy_Holder, typename SequencedIndex, typename Predicate>
void sequenced_index_remove_opt(Destroy_Holder& holder, SequencedIndex& x, Predicate pred)
{
	typedef typename index_org_cast<SequencedIndex>::type org_type;
	typedef typename org_type::iterator iterator;

	org_type& org = cast_to_index_org(x);

	iterator first = org.begin(), last = org.end();
	while(first != last)
	{
		if(pred(*first))
		{	
			x.erase(holder, first++);
		}
		else 
		{
			++first;
		}
	}
}

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_SEQ_INDEX_OPS_OPT_HPP__
