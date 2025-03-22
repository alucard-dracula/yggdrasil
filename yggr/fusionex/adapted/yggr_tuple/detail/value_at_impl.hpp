// value_at_impl.hpp

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

#ifndef __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_DETAIL_VALUE_AT_IMPL_HPP__
#define __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_DETAIL_VALUE_AT_IMPL_HPP__

#include <yggr/tuple/tuple.hpp>

namespace boost 
{ 
namespace fusion
{

struct yggr_tuple_tag;

namespace extension
{

template<typename T>
struct value_at_impl;

template <>
struct value_at_impl<yggr_tuple_tag>
{
	template <typename Sequence, typename N>
	struct apply 
		: public yggr::tuples::element<N::value, Sequence> 
	{
	};
};

} // namespace extension

} // namespace fusion
} // namespace boost

#endif // __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_DETAIL_VALUE_AT_IMPL_HPP__
