// yggr serialization unordered_set.hpp

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

#ifndef __YGGR_SERIALIZATION_UNORDERED_SET_HPP__
#define __YGGR_SERIALIZATION_UNORDERED_SET_HPP__

#include <yggr/serialization/unordered_unliner_container.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/serialization/detail/type_traits.hpp>

namespace yggr
{
namespace serialization
{
namespace detail
{

#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET

template<typename Key, typename HashFoo, typename Cmp, typename Alloc>
struct is_multi_container<std::unordered_multiset<Key, HashFoo, Cmp, Alloc> >
	: public boost::mpl::true_
{
};

#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

template<typename Key, typename HashFoo, typename Cmp, typename Alloc>
struct is_multi_container<boost::unordered_multiset<Key, HashFoo, Cmp, Alloc> >
	: public boost::mpl::true_
{
};

} // namspace detail
} // namespace serialization
} //namespace yggr

namespace boost
{
namespace serialization
{

#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
	YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(4, std::unordered_set)
	YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(4, std::unordered_multiset)
#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

	YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(4, boost::unordered_set)
	YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(4, boost::unordered_multiset)

} // namespace serialization
} // namespace boost

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, std::unordered_set, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, std::unordered_multiset, object_serializable)
#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, boost::unordered_set, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, boost::unordered_multiset, object_serializable)

#endif // __YGGR_SERIALIZATION_UNORDERED_SET_HPP__

#include <yggr/serialization/unordered_set_bson_impl.hpp>