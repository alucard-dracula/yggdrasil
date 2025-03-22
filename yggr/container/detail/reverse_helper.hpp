//resize_helper.hpp

/****************************************************************************
Copyright (c) 2010-2022 yggdrasil

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

#ifndef __YGGR_CONTAINER_DETAIL_RESIZE_HELPER_HPP__
#define __YGGR_CONTAINER_DETAIL_RESIZE_HELPER_HPP__

#include <cassert>

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/native_t.hpp>


namespace yggr
{
namespace container
{
namespace detail
{

template<typename C>
struct resize_helper
{
	typedef typename native_t<C>::type type;
	typedef type& param_type;

	template<bool has_resize, typename Nil = void>
	struct inner_helper;

	template<typename Nil>
	struct inner_helper<true, Nil>
	{
		inline void operator()(param_type param, size_type size) const
		{
			param.resize(size);
		}

		template<typename T>
		inline void operator()(param_type param, size_type size, const T& t) const
		{
			param.resize(size, t);
		}
	};

	template<typename Nil>
	struct inner_helper<false, Nil>
	{
		inline void operator()(param_type param, size_type size) const
		{
			return;
		}

		template<typename T>
		inline void operator()(param_type param, size_type size, const T& t) const
		{
			return;
		}
	};

	typedef inner_helper<has_any_resize<type>::value> inner_helper_type;

	inline void operator()(param_type param, size_type size) const
	{
		inner_helper_type h;
		h(param, size);
	}

	template<typename T> inline 
	void operator()(param_type param, size_type size, const T& t) const
	{
		inner_helper_type h;
		h(param, size, t);
	}
};

template<typename T, std::size_t N>
struct resize_helper<T[N]>
{
	typedef T type[N];
	typedef type& param_type;

	inline void operator()(param_type param, size_type size) const
	{
		assert((size == sizeof(type) / sizeof(T)));
	}

	inline void operator()(param_type param, size_type size, const T&) const
	{
		assert((size == sizeof(type) / sizeof(T)));
	}
};

template<typename T, std::size_t N,
			template<typename _T, std::size_t _N> class Array>
struct resize_helper< Array<T, N> >
{
	typedef Array<T, N> type;
	typedef type& param_type;

	inline void operator()(param_type param, size_type size) const
	{
		assert((size == N));
	}

	inline void operator()(param_type param, size_type size, const T&) const
	{
		assert((size == N));
	}
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_RESIZE_HELPER_HPP__