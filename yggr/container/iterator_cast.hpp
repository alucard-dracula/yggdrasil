//iterator_cast.hpp

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

#ifndef __YGGR_CONTAINER_ITERATOR_CAST_HPP__
#define __YGGR_CONTAINER_ITERATOR_CAST_HPP__

#include <yggr/base/yggrdef.h>
#include <iterator>

namespace yggr
{
namespace container
{

template<typename Container> inline
const typename Container::iterator&
	cast_to_iterator(Container&, const typename Container::iterator& src)
{
	return src;
}

template<typename Container> inline
typename Container::iterator&
	cast_to_iterator(Container&, typename Container::iterator& src)
{
	return src;
}

template<typename Container> inline
typename Container::iterator
	cast_to_iterator(Container& cont, const typename Container::const_iterator& src)
{
	typedef typename Container::iterator ret_type;
	ret_type ret = cont.begin();

	std::advance(ret, std::distance(ret, src));
}

} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_ITERATOR_CAST_HPP__
