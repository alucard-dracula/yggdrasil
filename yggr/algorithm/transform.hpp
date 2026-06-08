// transform.hpp

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

#ifndef __YGGR_ALGORITHM_TRANSFROM_HPP__
#define __YGGR_ALGORITHM_TRANSFROM_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace algorithm
{

template<typename InputIt, typename OutputIt, typename UnaryOp>
OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOp unary_op)
{
    for(; !(first1 == last1); ++d_first, ++first1)
    {
        *d_first = unary_op(*first1);
    }
 
    return d_first;
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename BinaryOp>
OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                   OutputIt d_first, BinaryOp binary_op)
{
    for(; !(first1 == last1); ++d_first, ++first1, ++first2)
    {
        *d_first = binary_op(*first1, *first2);
    }
 
    return d_first;
}

} // namespace algorithm
} // namesapce yggr

#endif // __YGGR_ALGORITHM_TRANSFROM_HPP__