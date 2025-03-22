// placement_new.hpp

#ifndef __YGGR_CONTAINER_DETAIL_PLACEMENT_NEW_HPP__
#define __YGGR_CONTAINER_DETAIL_PLACEMENT_NEW_HPP__

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

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

namespace yggr
{
namespace container
{
namespace detail
{

struct boost_container_new_t
{
};

} // namespace detail
} // namespace container
} // namespace yggr

inline void* operator new(std::size_t, void *p, yggr::container::detail::boost_container_new_t)
{
	return p;  
}

inline void operator delete(void *, void *, yggr::container::detail::boost_container_new_t)
{
}

#endif // __YGGR_CONTAINER_DETAIL_PLACEMENT_NEW_HPP__
