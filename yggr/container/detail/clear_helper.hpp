//clear_helper.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_CLEAR_HELPER_HPP__
#define __YGGR_CONTAINER_DETAIL_CLEAR_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/swap.hpp>
#include <yggr/type_traits/native_t.hpp>

#include <yggr/ppex/has_member_check.hpp>

#include <boost/range/functions.hpp>

#include <cassert>
#include <algorithm>

namespace yggr
{
namespace container
{
namespace detail
{

YGGR_PP_HAS_MEMBER_FOO_DEF(clear, T, void, clear, 0, void)

template<typename T, bool has_clear = has_clear<T>::value>
struct clear_helper_inner_helper;

template<typename T>
struct clear_helper_inner_helper<T, true>
{
	typedef T type;
	typedef type& param_type;

	inline void operator()(param_type param) const
	{
		param.clear();
	}
};

template<typename T>
struct clear_helper_inner_helper<T, false>
{
	typedef T type;
	typedef type& param_type;

	inline void operator()(param_type param) const
	{
		type tmp;
		::yggr::swap(tmp, param);
	}
};

template<typename C>
struct clear_helper
{
	typedef typename native_t<C>::type type;
	typedef type& param_type;

	typedef clear_helper_inner_helper<type> inner_helper_type;

	inline void operator()(param_type param) const
	{
		inner_helper_type h;
		h(param);
	}
};

template<typename T, std::size_t N>
struct clear_helper<T[N]>
{
	typedef T type[N];
	typedef type& param_type;

	inline void operator()(param_type param) const
	{
		std::fill(boost::begin(param), boost::end(param), T());
	}
};

template<typename T, std::size_t N,
			template<typename _T, std::size_t _N> class Array>
struct clear_helper< Array<T, N> >
{
	typedef Array<T, N> type;
	typedef type& param_type;

	inline void operator()(param_type param) const
	{
		std::fill(boost::begin(param), boost::end(param), T());
	}
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_CLEAR_HELPER_HPP__
