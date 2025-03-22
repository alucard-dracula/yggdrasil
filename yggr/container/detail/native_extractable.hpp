//native_extractable.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NATIVE_EXTRACTABLE_HPP__
#define __YGGR_CONTAINER_DETAIL_NATIVE_EXTRACTABLE_HPP__

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/symbols.hpp>
#include <yggr/typeof/typeof.hpp>
#include <yggr/container/detail/has_extract.hpp>
#include <yggr/container/detail/has_node_type.hpp>
#include <yggr/container/detail/has_insert_return_type.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T>
struct native_extractable
	: public
		boost::mpl::and_
		<
			has_any_extract<T>,
			has_member_t_node_type<T>
		>::type
{
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_NATIVE_EXTRACTABLE_HPP__
