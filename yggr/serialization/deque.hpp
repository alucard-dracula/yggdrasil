// yggr serialization deque.hpp

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

#ifndef __YGGR_SERIALIZATION_DEQUE_HPP__
#define __YGGR_SERIALIZATION_DEQUE_HPP__

#ifdef BOOST_SERIALIZATION_DEQUE_HPP
#error	"use this file can't include <boost/serialization/deque.hpp> ."
#endif // BOOST_SERIALIZATION_DEQUE_HPP

#include <yggr/serialization/liner_container.hpp>

#include <yggr/container/deque.hpp>

namespace boost
{
namespace serialization
{
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, std::deque)
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, boost::container::deque)

#if ! BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
	YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, std::deque)
	YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, boost::container::deque)
#endif // BOOST_WORKAROUND

} // namespace serialization
} // namespace boost

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, std::deque, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, boost::container::deque, object_serializable)

#endif // __YGGR_SERIALIZATION_DEQUE_HPP__

#include <yggr/serialization/deque_bson_impl.hpp>

