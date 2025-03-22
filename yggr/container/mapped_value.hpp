//mapped_value.hpp

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

#ifndef __YGGR_ITERATOR_MAPPED_VALUE_HPP__
#define __YGGR_ITERATOR_MAPPED_VALUE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/pair.hpp>
#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container_ex/container_mapped_value_t.hpp>

namespace yggr
{
namespace container
{

namespace detail
{

template<bool is_mappd_container, typename Mapped>
struct mapped_value_helper;

template<typename Mapped>
struct mapped_value_helper<true, Mapped>
{
	typedef Mapped mapped_type;


	template<typename T> inline
	mapped_type& operator()(T& t) const
	{
		return yggr::get<1>(t);
	}

	template<typename T> inline
	const mapped_type& operator()(const T& t) const
	{
		return yggr::get<1>(t);
	}
};

template<typename Mapped>
struct mapped_value_helper<false, Mapped>
{
	typedef Mapped mapped_type;

	template<typename T> inline
	T& operator()(T& t) const
	{
		BOOST_MPL_ASSERT((boost::is_same<mapped_type, T>));
		return t;
	}

	template<typename T> inline
	const T& operator()(const T& t) const
	{
		BOOST_MPL_ASSERT((boost::is_same<mapped_type, T>));
		return t;
	}
};

template<bool is_mappd_container, typename Mapped>
struct mapped_key_helper;

template<typename Mapped>
struct mapped_key_helper<true, Mapped>
{
	typedef Mapped mapped_type;

	template<typename T> inline
	mapped_type& operator()(T& t) const
	{
		return yggr::get<0>(t);
	}

	template<typename T> inline
	const mapped_type& operator()(const T& t) const
	{
		return yggr::get<0>(t);
	}
};

template<typename Mapped>
struct mapped_key_helper<false, Mapped>
{
	typedef Mapped mapped_type;

	template<typename T> inline
	T& operator()(T& t) const
	{
		BOOST_MPL_ASSERT((boost::is_same<mapped_type, T>));
		return t;
	}

	template<typename T> inline
	const T& operator()(const T& t) const
	{
		BOOST_MPL_ASSERT((boost::is_same<mapped_type, T>));
		return t;
	}
};

} // namespace detail

template<typename Container, typename T> inline
typename container_mapped_value_t<Container>::type& mapped_value(T& t)
{
	typedef Container cont_type;
	typedef has_mapped_t<cont_type> chk_type;
	typedef typename container_mapped_value_t<cont_type>::type ret_type;
	typedef detail::mapped_value_helper<chk_type::value, ret_type> h_type;

	h_type h;
	return h(t);
}

template<typename Container, typename T> inline 
const typename container_mapped_value_t<Container>::type& mapped_value(const T& t)
{
	typedef Container cont_type;
	typedef has_mapped_t<cont_type> chk_type;
	typedef typename container_mapped_value_t<cont_type>::type ret_type;
	typedef detail::mapped_value_helper<chk_type::value, ret_type> h_type;

	h_type h;
	return h(t);
}

template<typename Container, typename T> inline
typename container_mapped_key_t<Container>::type& mapped_key(T& t)
{
	typedef Container cont_type;
	typedef has_mapped_t<cont_type> chk_type;
	typedef typename container_mapped_key_t<cont_type>::type ret_type;
	typedef detail::mapped_key_helper<chk_type::value, ret_type> h_type;

	h_type h;
	return h(t);
}

template<typename Container, typename T> inline 
const typename container_mapped_key_t<Container>::type& mapped_key(const T& t)
{
	typedef Container cont_type;
	typedef has_mapped_t<cont_type> chk_type;
	typedef typename container_mapped_key_t<cont_type>::type ret_type;
	typedef detail::mapped_key_helper<chk_type::value, ret_type> h_type;

	h_type h;
	return h(t);
}

} // namespace container

using container::mapped_value;
using container::mapped_key;

} // namespace yggr

#endif // __YGGR_ITERATOR_MAPPED_VALUE_HPP__