// yggr serialization map.hpp

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

#ifndef __YGGR_SERIALIZATION_MAP_HPP__
#define __YGGR_SERIALIZATION_MAP_HPP__

#ifdef BOOST_SERIALIZATION_MAP_HPP
#error	"use this file can't include <boost/serialization/map.hpp> ."
#endif // BOOST_SERIALIZATION_MAP_HPP

#include <yggr/serialization/key_value_container.hpp>

#include <yggr/container/map.hpp>

namespace boost
{
namespace serialization
{
	YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, std::map)
	YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, std::multimap)

#if (BOOST_VERSION < 105600)

	YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, boost::container::map)
	YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, boost::container::multimap)

#else

	YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(5, boost::container::map)
	YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(5, boost::container::multimap)

	YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, yggr::container::detail::map)
	YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, yggr::container::detail::multimap)

#endif // BOOST_VERSION < 105600

} // namespace serialization
} // namespace boost

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, std::map, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, std::multimap, object_serializable)

#if (BOOST_VERSION < 105600)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, boost::container::map, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, boost::container::multimap, object_serializable)

#else

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(5, boost::container::map, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(5, boost::container::multimap, object_serializable)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, yggr::container::detail::map, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(4, yggr::container::detail::multimap, object_serializable)

#endif // BOOST_VERSION < 105600

#endif // __YGGR_SERIALIZATION_MAP_HPP__

#include <yggr/serialization/map_bson_impl.hpp>