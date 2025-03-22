//begin_impl.hpp

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

#ifndef __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_DETAIL_BEGIN_IMPL_HPP__
#define __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_DETAIL_BEGIN_IMPL_HPP__

#include <yggr/fusionex/adapted/yggr_tuple/yggr_tuple_iterator.hpp>

namespace boost 
{ 
namespace fusion 
{

struct yggr_tuple_tag;

namespace extension
{

template<typename T>
struct begin_impl;

template<>
struct begin_impl<yggr_tuple_tag>
{
	template <typename Sequence>
	struct apply 
	{
		typedef yggr_tuple_iterator<Sequence> type;
    
		inline static type call(Sequence& v)
		{
			return type(v);
		}
	};
};

} // namespace extension
} // namespace fusion
} // namespace boost

#endif // __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_DETAIL_BEGIN_IMPL_HPP__
