// yggr serialization vector.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_SERIALIZATION_VECTOR_HPP__
#define __YGGR_SERIALIZATION_VECTOR_HPP__

#include <vector>
#include <boost/container/vector.hpp>
#include <yggr/serialization/liner_container.hpp>

//#include <boost/container/vector.hpp>
#include <yggr/serialization/detail/type_traits.hpp>

namespace yggr
{
namespace serialization
{
namespace detail
{
template<typename Val, typename Alloc>
struct is_vector< std::vector<Val, Alloc> >
{
	typedef boost::mpl::bool_<true> type;
};

template<typename Val, typename Alloc>
struct is_vector< boost::container::vector<Val, Alloc> >
{
	typedef boost::mpl::bool_<true> type;
};

} // namespace detail
} // namespace serialization
} // namespace yggr

//#include <yggr/serialization/vector_bson_impl.hpp>

namespace boost
{
namespace serialization
{
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(std::vector)
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(boost::container::vector)

#if ! BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
	YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(std::vector)
	YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(boost::container::vector)
#endif // BOOST_WORKAROUND

} // namespace serialization
} // namespace boost

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, std::vector)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, boost::container::vector)

#endif // __YGGR_SERIALIZATION_VECTOR_HPP__

#include <yggr/serialization/vector_bson_impl.hpp>

