// is_permutation.hpp

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

#ifndef __YGGR_ALGORITHM_IS_PERMUTATION_HPP__
#define __YGGR_ALGORITHM_IS_PERMUTATION_HPP__

/* From is_permutation code in
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2010/n3068.pdf
 */

#include <yggr/base/yggrdef.h>
#include <iterator>
#include <algorithm>

namespace yggr
{
namespace algorithm
{

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template <typename ForwardIterator1, typename ForwardIterator2>
bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
{
	typedef typename std::iterator_traits<ForwardIterator2>::difference_type distance_type;

	// Efficiently compare identical prefixes: O(N) if sequences have the same elements in the same order.
	for(;first1 != last1; ++first1, ++first2) 
	{
		if(!(*first1 == *first2))
		{
			break;
		}
	}
		
	if(first1 == last1)
	{
		return true;
	}

	// Establish last2 assuming equal ranges by iterating over the rest of the list.
	ForwardIterator2 last2 = first2;
	std::advance(last2, std::distance(first1, last1));
	for (ForwardIterator1 scan = first1; scan != last1; ++scan) 
	{
		if(scan != std::find(first1, scan, *scan))
		{
			continue; // We've seen this one before.
		}

		distance_type matches = std::count(first2, last2, *scan);
		if(0 == matches || std::count(scan, last1, *scan) != matches)
		{
			return false;
		}
	}

	return true;
}

#else

using std::is_permutation;

#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

} // namespace algorithm

using algorithm::is_permutation;

} // namespace yggr


#endif // __YGGR_ALGORITHM_IS_PERMUTATION_HPP__
