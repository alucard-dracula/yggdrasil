// yggr serialization list.hpp

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

#ifndef __YGGR_SERIALIZATION_LIST_HPP__
#define __YGGR_SERIALIZATION_LIST_HPP__

#ifdef BOOST_SERIALIZATION_LIST_HPP
#error	"use this file can't include <boost/serialization/list.hpp> ."
#endif // BOOST_SERIALIZATION_LIST_HPP

#include <yggr/base/yggrdef.h>
#include <yggr/serialization/liner_container.hpp>
#include <yggr/container/list.hpp>

namespace boost
{
namespace serialization
{
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, std::list)
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, boost::container::list)

#if !(BOOST_VERSION < 107000)
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, yggr::container::detail::list)
#endif // !(BOOST_VERSION < 107000)

#if ! BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
	YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, std::list)
	YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, boost::container::list)

#	if !(BOOST_VERSION < 107000)
	YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, yggr::container::detail::list)
#	endif // !(BOOST_VERSION < 107000)

#endif // BOOST_WORKAROUND

} // namespace serialization
} // namespace boost

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, std::list, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, boost::container::list, object_serializable)

#if !(BOOST_VERSION < 107000)
	YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, yggr::container::detail::list, object_serializable)
#endif // !(BOOST_VERSION < 107000)

#endif // __YGGR_SERIALIZATION_LIST_HPP__

#include <yggr/serialization/list_bson_impl.hpp>

