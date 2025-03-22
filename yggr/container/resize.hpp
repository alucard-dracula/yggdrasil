//resize.hpp

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

#ifndef __YGGR_CONTAINER_RESIZE_HPP__
#define __YGGR_CONTAINER_RESIZE_HPP__

#include <yggr/container/detail/resize_helper.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_const.hpp>

namespace yggr
{
namespace container
{

template<typename C> inline
typename boost::disable_if<boost::is_const<C>, void>::type
	resize(C& c, size_type size)
{
	typedef container::detail::resize_helper<C> helper_type;
	helper_type helper;
	helper(c, size);
}

template<typename C, typename T> inline
typename boost::disable_if<boost::is_const<C>, void>::type
	resize(C& c, size_type size, const T& t)
{
	typedef container::detail::resize_helper<C> helper_type;
	helper_type helper;
	helper(c, size, t);
}


} // namespace container

using container::resize;

} // namespace yggr

#endif // __YGGR_CONTAINER_RESIZE_HPP__
