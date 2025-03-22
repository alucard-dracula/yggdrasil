// multi_index_config.hpp

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

#ifndef __YGGR_CONTAINER_MULTI_INDEX_CONFIG_HPP__
#define __YGGR_CONTAINER_MULTI_INDEX_CONFIG_HPP__

#include <yggr/base/yggrdef.h>

#if BOOST_VERSION >= 105500

#	if !defined(YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE)
#		define YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE
#	endif // YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE

#	if !defined(YGGR_BOOST_MULTI_INDEX_HAS_SHRINK_TO_FIT)
#		define YGGR_BOOST_MULTI_INDEX_HAS_SHRINK_TO_FIT
#	endif // YGGR_BOOST_MULTI_INDEX_HAS_SHRINK_TO_FIT

#endif // BOOST_VERSION >= 105500

#if BOOST_VERSION >= 105600

#	if !defined(YGGR_BOOST_MULTI_INDEX_HASHED_INDEX_HAS_COMPARER)
#		define YGGR_BOOST_MULTI_INDEX_HASHED_INDEX_HAS_COMPARER
#	endif // YGGR_BOOST_MULTI_INDEX_HASHED_INDEX_HAS_COMPARER

#endif // // BOOST_VERSION >= 105500

#if BOOST_VERSION >= 105900

#	if !defined(YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX)
#		define YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
#	endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

#	if !defined(YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX)
#		define YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX
#	endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

#endif // BOOST_VERSION >= 105900

namespace yggr
{
namespace container
{
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_MULTI_INDEX_CONFIG_HPP__
