//options.hpp

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

#ifndef __YGGR_CONTAINER_OPTIONS_HPP__
#define __YGGR_CONTAINER_OPTIONS_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 105600)
#	include <boost/container/options.hpp>
#endif // !(BOOST_VERSION < 105600)

namespace yggr
{
namespace container
{

#if BOOST_VERSION < 105600

enum tree_type_enum
{
	red_black_tree
};

template<tree_type_enum TreeType, bool OptimizeSize>
struct tree_opt;

typedef tree_opt<red_black_tree, true> tree_assoc_defaults;

#else

	using boost::container::tree_type_enum;
	using boost::container::tree_opt;
	using boost::container::tree_assoc_defaults;

#endif // BOOST_VERSION < 105600

using boost::container::ordered_unique_range_t;
using boost::container::ordered_range_t;
using boost::container::ordered_range;

} // namespace container
} // namespace yggr

// vector option

namespace yggr
{
namespace container
{

#if (BOOST_VERSION < 106700)

template<typename AllocTraits, typename StoredSizeType>
struct get_stored_size_type_with_alloctraits
{
   typedef StoredSizeType type;
};

template<typename AllocTraits>
struct get_stored_size_type_with_alloctraits<AllocTraits, void>
{
   typedef typename AllocTraits::size_type type;
};

template<class GrowthType, class StoredSizeType>
struct vector_opt
{
   typedef GrowthType      growth_factor_type;
   typedef StoredSizeType  stored_size_type;

   template<typename AllocTraits>
   struct get_stored_size_type
      : get_stored_size_type_with_alloctraits<AllocTraits, StoredSizeType>
   {
   };
};

typedef vector_opt<void, void> vector_null_opt;

#else

	using boost::container::vector_opt;
	using boost::container::vector_null_opt;
	using boost::container::vector_options;

#endif // (BOOST_VERSION < 106700)

} // namespace container
} // namespace yggr

// deque option
namespace yggr
{
namespace container
{

#if BOOST_VERSION < 107100
template<std::size_t BlockBytes, std::size_t BlockSize>
struct deque_opt
{
   static const std::size_t block_bytes = BlockBytes;
   static const std::size_t block_size  = BlockSize;
   BOOST_STATIC_ASSERT_MSG(!(block_bytes && block_size), "block_bytes and block_size can't be specified at the same time");
};

typedef deque_opt<0u, 0u> deque_null_opt;

#else

	using boost::container::deque_opt;
	using boost::container::deque_null_opt;
	using boost::container::deque_options;

#endif // BOOST_VERSION < 107100


} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_OPTIONS_HPP__
