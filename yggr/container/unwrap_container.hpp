//unwrap_container.hpp

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

#ifndef __YGGR_CONTAINER_UNWRAP_CONTAINER_HPP__
#define __YGGR_CONTAINER_UNWRAP_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/stack.hpp>
#include <yggr/container/queue.hpp>

namespace yggr
{
namespace container
{

namespace detail
{

template<typename T>
struct stl_unwrap_container_helper;

// std::stack
template<typename T, typename C>
struct stl_unwrap_container_helper< std::stack<T, C> >
	: public std::stack<T, C>
{
public:
	typedef std::stack<T, C> base_type;
	typedef C type;

private:
	typedef stl_unwrap_container_helper this_type;

public:
	inline type& container(void)
	{
		return base_type::c;
	}

	inline const type& container(void) const
	{
		return base_type::c;
	}
};

// std::queue
template<typename T, typename C>
struct stl_unwrap_container_helper< std::queue<T, C> >
	: public std::queue<T, C>
{
public:
	typedef std::queue<T, C> base_type;
	typedef C type;

private:
	typedef stl_unwrap_container_helper this_type;

public:
	inline type& container(void)
	{
		return base_type::c;
	}

	inline const type& container(void) const
	{
		return base_type::c;
	}
};

// std::priority_queue
template<typename T, typename C, typename P>
struct stl_unwrap_container_helper< std::priority_queue<T, C, P> >
	: public std::priority_queue<T, C, P>
{
public:
	typedef std::priority_queue<T, C, P> base_type;
	typedef C type;

private:
	typedef stl_unwrap_container_helper this_type;

public:
	inline type& container(void)
	{
		return base_type::c;
	}

	inline const type& container(void) const
	{
		return base_type::c;
	}
};

} // namespace detail

template<typename Container>
struct unwrap_container
{
	typedef Container type;

	inline type& operator()(type& src) const
	{
		return src;
	}

	inline const type& operator()(const type& src) const
	{
		return src;
	}
};

// stack
template<typename T, typename C>
struct unwrap_container< std::stack<T, C> >
{
public:
	typedef C type;

private:
	typedef std::stack<T, C> wrap_type;
	typedef detail::stl_unwrap_container_helper<wrap_type> unwrap_helper_type;

public:
	inline type& operator()(wrap_type& src) const
	{
		unwrap_helper_type& unwrap_helper = reinterpret_cast<unwrap_helper_type&>(src);
		return unwrap_helper.container();
	}

	inline const type& operator()(const wrap_type& src) const
	{
		const unwrap_helper_type& unwrap_helper = reinterpret_cast<const unwrap_helper_type&>(src);
		return unwrap_helper.container();
	}
};

template<typename T, typename C>
struct unwrap_container< yggr::container::stack<T, C> >
{
public:
	typedef C type;

private:
	typedef ::yggr::container::stack<T, C> wrap_type;

public:
	inline type& operator()(wrap_type& src) const
	{
		return src.container();
	}

	inline const type& operator()(const wrap_type& src) const
	{
		return src.container();
	}
};

// queue
template<typename T, typename C>
struct unwrap_container< std::queue<T, C> >
{
public:
	typedef C type;

private:
	typedef std::queue<T, C> wrap_type;
	typedef detail::stl_unwrap_container_helper<wrap_type> unwrap_helper_type;

public:
	inline type& operator()(wrap_type& src) const
	{
		unwrap_helper_type& unwrap_helper = reinterpret_cast<unwrap_helper_type&>(src);
		return unwrap_helper.container();
	}

	inline const type& operator()(const wrap_type& src) const
	{
		const unwrap_helper_type& unwrap_helper = reinterpret_cast<const unwrap_helper_type&>(src);
		return unwrap_helper.container();
	}
};

template<typename T, typename C>
struct unwrap_container< yggr::container::queue<T, C> >
{
public:
	typedef C type;

private:
	typedef ::yggr::container::queue<T, C> wrap_type;

public:
	inline type& operator()(wrap_type& src) const
	{
		return src.container();
	}

	inline const type& operator()(const wrap_type& src) const
	{
		return src.container();
	}
};

// priority_queue
template<typename T, typename C, typename P>
struct unwrap_container< std::priority_queue<T, C, P> >
{
public:
	typedef C type;

private:
	typedef std::priority_queue<T, C, P> wrap_type;
	typedef detail::stl_unwrap_container_helper<wrap_type> unwrap_helper_type;

public:
	inline type& operator()(wrap_type& src) const
	{
		unwrap_helper_type& unwrap_helper = reinterpret_cast<unwrap_helper_type&>(src);
		return unwrap_helper.container();
	}

	inline const type& operator()(const wrap_type& src) const
	{
		const unwrap_helper_type& unwrap_helper = reinterpret_cast<const unwrap_helper_type&>(src);
		return unwrap_helper.container();
	}
};

template<typename T, typename C, typename P>
struct unwrap_container< yggr::container::priority_queue<T, C, P> >
{
public:
	typedef C type;

private:
	typedef ::yggr::container::priority_queue<T, C, P> wrap_type;

public:
	inline type& operator()(wrap_type& src) const
	{
		return src.container();
	}

	inline const type& operator()(const wrap_type& src) const
	{
		return src.container();
	}
};


} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_UNWRAP_CONTAINER_HPP__
