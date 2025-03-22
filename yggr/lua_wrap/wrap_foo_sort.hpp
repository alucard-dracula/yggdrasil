//wrap_foo_sort.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_FOO_SORT_HPP__
#define __YGGR_LUA_WRAP_WRAP_FOO_SORT_HPP__

#include <yggr/lua_wrap/detail/unwrap_t.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>
#include <algorithm>

namespace yggr
{
namespace lua_wrap
{

template<typename RandomIt> YGGR_CXX14_CONSTEXPR_OR_INLINE
void wrap_foo_sort_aesc(RandomIt s, RandomIt e)
{
	typedef RandomIt now_iter_type;
	typedef typename detail::unwrap_t<now_iter_type>::type iter_type;
	typedef typename iterator_ex::iterator_value_ex<iter_type>::type value_type;

	std::sort(s, e, std::less<value_type>());
}

template<typename RandomIt> YGGR_CXX14_CONSTEXPR_OR_INLINE
void wrap_foo_sort_desc(RandomIt s, RandomIt e)
{
	typedef RandomIt now_iter_type;
	typedef typename detail::unwrap_t<now_iter_type>::type iter_type;
	typedef typename iterator_ex::iterator_value_ex<iter_type>::type value_type;

	std::sort(s, e, std::greater<value_type>());
}

} // namespace lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_WRAP_FOO_SORT_HPP__
