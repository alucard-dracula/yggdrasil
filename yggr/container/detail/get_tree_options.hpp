// get_tree_options.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_GET_TREE_OPTIONS_HPP__
#define __YGGR_CONTAINER_DETAIL_GET_TREE_OPTIONS_HPP__

#include <yggr/container/options.hpp>

#if !(BOOST_VERSION < 106700)
#   include <boost/container/detail/tree.hpp>
#endif //!(BOOST_VERSION < 106700)

namespace yggr
{
namespace container
{
namespace detail
{

template<typename TplArgOpt>
struct get_tree_options
{
#if (BOOST_VERSION < 105600)
	typedef tree_assoc_defaults type;
#elif (BOOST_VERSION < 106700)
	typedef TplArgOpt type;
#else
	typedef typename boost::container::dtl::get_tree_opt<TplArgOpt>::type type;
#endif // (BOOST_VERSION < 106700)
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_GET_TREE_OPTIONS_HPP__
