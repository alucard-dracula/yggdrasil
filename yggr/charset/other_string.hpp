// other_string.hpp

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

#ifndef __YGGR_CHARSET_OTHER_STRING_HPP__
#define __YGGR_CHARSET_OTHER_STRING_HPP__

#include <yggr/charset/assert_char.hpp>
#include <yggr/charset/other_char.hpp>
#include <yggr/container_ex/allocator_t.hpp>

namespace yggr
{
namespace charset
{

// other_char_traits
template<typename T, typename C1, typename C2>
struct other_char_traits;

template<template<typename _Char> class Char_Traits,
			typename Char, typename C1, typename C2>
struct other_char_traits< Char_Traits<Char>, C1, C2>
{
	YGGR_ASSERT_CHAR_TYPE(Char);
	YGGR_ASSERT_CHAR_TYPE(C1);
	YGGR_ASSERT_CHAR_TYPE(C2);

	typedef Char char_type;
	typedef typename other_char<Char, C1, C2>::type other_char_type;

	typedef Char_Traits<other_char_type> type;
};

//other_char_allocator
template<typename A, typename C1, typename C2>
struct other_char_allocator;

template<template<typename _Char> class Char_Allcator,
			typename Char, typename C1, typename C2>
struct other_char_allocator< Char_Allcator<Char>, C1, C2 >
{
private:
	YGGR_ASSERT_CHAR_TYPE(Char);
	YGGR_ASSERT_CHAR_TYPE(C1);
	YGGR_ASSERT_CHAR_TYPE(C2);

	typedef Char char_type;
	typedef typename other_char<Char, C1, C2>::type other_char_type;

public:
	typedef Char_Allcator<other_char_type> type;
};

//other_char_allocator
template<typename A, typename C1, typename C2>
struct other_char_tpl_arg_allocator
{
	typedef A type;
};

template<typename C1, typename C2>
struct other_char_tpl_arg_allocator<void, C1, C2>
{
	typedef void type;
};

template<template<typename _Char> class Char_Allcator,
			typename Char, typename C1, typename C2>
struct other_char_tpl_arg_allocator< Char_Allcator<Char>, C1, C2 >
{
private:
	YGGR_ASSERT_CHAR_TYPE(Char);
	YGGR_ASSERT_CHAR_TYPE(C1);
	YGGR_ASSERT_CHAR_TYPE(C2);

	typedef Char char_type;
	typedef typename other_char<Char, C1, C2>::type other_char_type;

public:
	typedef Char_Allcator<other_char_type> type;
};

// other_char_object // compatible only one template params
template<typename OBj, typename C1, typename C2>
struct other_char_object;

template<template<typename _Char> class Char_Object,
			typename Char, typename C1, typename C2>
struct other_char_object< Char_Object<Char>, C1, C2 >
{
private:
	YGGR_ASSERT_CHAR_TYPE(Char);
	YGGR_ASSERT_CHAR_TYPE(C1);
	YGGR_ASSERT_CHAR_TYPE(C2);

	typedef Char char_type;
	typedef typename other_char<Char, C1, C2>::type other_char_type;

public:
	typedef Char_Object<other_char_type> type;
};

template<typename Str, typename C1, typename C2>
struct other_string;

template<template<typename _Char, typename _Traits, typename _Alloc> class Basic_String,
			typename Char, typename Traits, typename Alloc, typename C1, typename C2>
struct other_string< Basic_String<Char, Traits, Alloc>, C1, C2 >
{
	YGGR_ASSERT_CHAR_TYPE(Char);
	YGGR_ASSERT_CHAR_TYPE(C1);
	YGGR_ASSERT_CHAR_TYPE(C2);

	typedef Char value_type;
	typedef Traits traits_type;
	typedef Alloc tpl_arg_allocator_type;

	typedef Basic_String<Char, Traits, Alloc> string_type;
	typedef value_type char_type;
	typedef typename container_ex::allocator_t<string_type>::type allocator_type;

	typedef typename other_char<value_type, C1, C2>::type other_value_type;
	typedef typename other_char_traits<traits_type, C1, C2>::type other_traits_type;
	typedef typename other_char_tpl_arg_allocator<tpl_arg_allocator_type, C1, C2>::type other_tpl_arg_allocator_type;
	typedef typename other_char_allocator<allocator_type, C1, C2>::type other_allocator_type;

	typedef Basic_String<other_value_type, other_traits_type, other_tpl_arg_allocator_type> other_string_type;
	typedef other_value_type other_char_type;

	typedef other_string_type type;
};

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_OTHER_STRING_HPP__
