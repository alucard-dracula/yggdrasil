//multi_index_ranked_index.hpp

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

#ifndef __YGGR_CONTAINER_MULTI_INDEX_RANKED_INDEX_HPP__
#define __YGGR_CONTAINER_MULTI_INDEX_RANKED_INDEX_HPP__

#include <yggr/container/multi_index_config.hpp>

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/multi_index_ordered_index.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_ranked_index.hpp>

#include <boost/multi_index/ranked_index.hpp>

namespace yggr
{
namespace container
{

using boost::multi_index::ranked_unique;
using boost::multi_index::ranked_non_unique;

} // namespace container
} // namespace yggr

#else

#	error "if you want to using boost::multi_index::ranked_index the boost version must >= 1.59"

#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

#endif // __YGGR_CONTAINER_MULTI_INDEX_RANKED_INDEX_HPP__
