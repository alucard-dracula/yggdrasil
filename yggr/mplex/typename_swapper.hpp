//typename_swapper.hpp

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

#ifndef __YGGR_MPLEX_TYPENAME_SWAPPER_HPP__
#define __YGGR_MPLEX_TYPENAME_SWAPPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/traits.hpp>
#include <yggr/mplex/null_t.hpp>

namespace yggr
{
namespace mplex
{

namespace detail
{

template<typename T>
struct tpl_swapper
	: public null_type
{
};

template<typename L, typename R, 
			template<typename _L, typename _R> class Pair>
struct tpl_swapper< Pair<L, R> >
	: public traits< Pair<R, L> >
{
};
	
} // namespace detail

template<typename T>
struct typename_swapper
	: public detail::tpl_swapper<T>
{
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_TYPENAME_SWAPPER_HPP__
