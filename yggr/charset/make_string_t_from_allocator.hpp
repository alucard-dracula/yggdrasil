// make_string_t_from_allocator.hpp

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

#ifndef __YGGR_CHARSET_MAKE_STRING_T_FROM_ALLOCATOR_HPP__
#define __YGGR_CHARSET_MAKE_STRING_T_FROM_ALLOCATOR_HPP__

#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/container_ex/allocator_traits.hpp>
#include <yggr/charset/string.hpp>

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename Alloc, bool is_alloc = container_ex::is_allocator<Alloc>::value >
struct _make_string_t_from_allocator;

template<typename Alloc>
struct _make_string_t_from_allocator<Alloc, true>
{
	typedef 
		::yggr::charset::basic_string
		< 
			typename container_ex::allocator_traits<Alloc>::value_type, 
			typename 
				charset::basic_string
				< 
					typename container_ex::allocator_traits<Alloc>::value_type 
				>::traits_type, 
			Alloc 
		> type;
};

template<typename Alloc>
struct _make_string_t_from_allocator<Alloc, false>
{
	typedef ::yggr::charset::string type;
};

} // namespace detail

template<typename Alloc>
struct make_string_t_from_allocator
	: public detail::_make_string_t_from_allocator<Alloc>
{
};

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_MAKE_STRING_T_FROM_ALLOCATOR_HPP__
