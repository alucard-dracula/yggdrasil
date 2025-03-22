//boost_midx_check_helper.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_MIDX_CHECK_HELPER_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_MIDX_CHECK_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/ref.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Container0, typename Container1> inline 
bool boost_midx_check_equal_allocators(const Container0& cont0, const Container1& cont1)
{
	return cont0.get_allocator() == cont1.get_allocator();
}

} // namespace detail
} // namespace container
} // namespace yggr

#define YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(cont0, cont1) \
  BOOST_MULTI_INDEX_SAFE_MODE_ASSERT( \
    boost_midx_check_equal_allocators(cont0, cont1), \
    boost::multi_index::safe_mode::same_container + 1); // boost_174, boost::multi_index::safe_mode::unequal_allocators

#endif // __YGGR_CONTAINER_DETAIL_BOOST_MIDX_CHECK_HELPER_HPP__
