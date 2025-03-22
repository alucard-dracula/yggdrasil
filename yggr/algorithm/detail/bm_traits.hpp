//bm_traits.hpp

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

#ifndef __YGGR_ALGORITHM_DETAIL_BM_TRAITS_HPP__
#define __YGGR_ALGORITHM_DETAIL_BM_TRAITS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/index_generator.hpp>

#include <yggr/algorithm/detail/corpus_traits.hpp>

#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/size_t.hpp>

namespace yggr
{
namespace algorithm
{
namespace detail
{

template<typename PatIter>
struct bm_traits;

} // namespace detail
} // namespace algorithm
} // namespace yggr


namespace yggr
{
namespace algorithm
{
namespace detail
{

template<typename PatIter>
struct bm_traits
{
	typedef PatIter iterator_type;
	typedef typename boost::iterator_difference<iterator_type>::type difference_type;
	typedef typename boost::iterator_value<iterator_type>::type value_type;
	typedef value_type key_type;
	typedef boost::mpl::size_t<0> array_size_type;
};

} // namespace detail
} // namespace algorithm
} // namespace yggr

#endif // __YGGR_ALGORITHM_DETAIL_BM_TRAITS_HPP__
