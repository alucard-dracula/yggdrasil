//wrap_foo_of_container.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_FOO_OF_CONTAINER_HPP__
#define __YGGR_LUA_WRAP_WRAP_FOO_OF_CONTAINER_HPP__

#include <yggr/container/size.hpp>
#include <yggr/container/resize.hpp>
#include <yggr/container/empty.hpp>
#include <yggr/container/clear.hpp>

#include <yggr/container/begin.hpp>
#include <yggr/container/end.hpp>

#include <yggr/container/rbegin.hpp>
#include <yggr/container/rend.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/detail/unwrap_t.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{
namespace lua_wrap
{
// size
template<typename Container> inline
typename range_ex::range_size_ex<Container>::type
	wrap_foo_size(const Container& cont)
{
	return container::size(detail::unwrap_object(cont));
}

template<typename Container> inline
typename 
	wrap_base_t_maker
	<
		typename range_ex::range_size_ex<Container>::type
	>::type
	wrap_foo_wraped_size(const Container& cont)
{
	typedef typename 
		wrap_base_t_maker
		<
			typename range_ex::range_size_ex<Container>::type
		>::type ret_type;

	return ret_type(::yggr::lua_wrap::wrap_foo_size(cont));
}

template<typename Container> inline
typename upper_signed< typename range_ex::range_size_ex<Container>::type >::type
	wrap_foo_ssize(const Container& cont)
{
	return container::ssize(detail::unwrap_object(cont));
}

template<typename Container> inline
typename 
	wrap_base_t_maker
	<
		typename 
			upper_signed
			< 
				typename range_ex::range_size_ex<Container>::type 
			>::type
	>::type
	wrap_foo_wraped_ssize(const Container& cont)
{
	typedef typename 
		wrap_base_t_maker
		<
			typename 
				upper_signed
				< 
					typename range_ex::range_size_ex<Container>::type 
				>::type
		>::type ret_type;

	return ret_type(::yggr::lua_wrap::wrap_foo_ssize(cont));
}

// resize 
template<typename Container, typename Dis> inline
void wrap_foo_resize(Container& cont, 
						typename 
							boost::mpl::if_
							<
								boost::is_class<Dis>,
								const Dis&,
								Dis
							>::type n)
{
	container::resize(detail::unwrap_object(cont), detail::unwrap_object(n));
}

template<typename Container, typename Dis, typename T> inline
void wrap_foo_resize(Container& cont, 
						typename 
							boost::mpl::if_
							<
								boost::is_class<Dis>,
								const Dis&,
								Dis
							>::type n,
						const T& val)
{
	container::resize(detail::unwrap_object(cont), detail::unwrap_object(n), val);
}

// empty
template<typename Container> inline
bool wrap_foo_empty(const Container& cont)
{
	return container::empty(detail::unwrap_object(cont));
}

// clear
template<typename Container> inline
void wrap_foo_clear(Container& cont)
{
	container::clear(detail::unwrap_object(cont));
}

// begin end
template<typename Container> inline
typename Container::wrap_iterator_type
	wrap_foo_ref_begin(Container& cont)
{
	typedef typename Container::wrap_iterator_type ret_type;
	return ret_type(container::begin(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_const_iterator_type
	wrap_foo_cref_begin(const Container& cont)
{
	typedef typename Container::wrap_const_iterator_type ret_type;
	return ret_type(container::begin(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_const_iterator_type
	wrap_foo_cbegin(const Container& cont)
{
	typedef typename Container::wrap_const_iterator_type ret_type;
	return ret_type(container::cbegin(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_iterator_type
	wrap_foo_ref_end(Container& cont)
{
	typedef typename Container::wrap_iterator_type ret_type;
	return ret_type(container::end(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_const_iterator_type
	wrap_foo_cref_end(const Container& cont)
{
	typedef typename Container::wrap_const_iterator_type ret_type;
	return ret_type(container::end(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_const_iterator_type
	wrap_foo_cend(const Container& cont)
{
	typedef typename Container::wrap_const_iterator_type ret_type;
	return ret_type(container::cend(detail::unwrap_object(cont)));
}

// rbegin rend

template<typename Container> inline
typename Container::wrap_reverse_iterator_type
	wrap_foo_ref_rbegin(Container& cont)
{
	typedef typename Container::wrap_reverse_iterator_type ret_type;
	return ret_type(container::rbegin(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_const_reverse_iterator_type
	wrap_foo_cref_rbegin(const Container& cont)
{
	typedef typename Container::wrap_const_reverse_iterator_type ret_type;
	return ret_type(container::rbegin(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_const_reverse_iterator_type
	wrap_foo_crbegin(const Container& cont)
{
	typedef typename Container::wrap_const_reverse_iterator_type ret_type;
	return ret_type(container::crbegin(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_reverse_iterator_type
	wrap_foo_ref_rend(Container& cont)
{
	typedef typename Container::wrap_reverse_iterator_type ret_type;
	return ret_type(container::rend(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_const_reverse_iterator_type
	wrap_foo_cref_rend(const Container& cont)
{
	typedef typename Container::wrap_const_reverse_iterator_type ret_type;
	return ret_type(container::rend(detail::unwrap_object(cont)));
}

template<typename Container> inline
typename Container::wrap_const_reverse_iterator_type
	wrap_foo_crend(const Container& cont)
{
	typedef typename Container::wrap_const_reverse_iterator_type ret_type;
	return ret_type(container::crend(detail::unwrap_object(cont)));
}

} // namespace lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_WRAP_FOO_OF_CONTAINER_HPP__
