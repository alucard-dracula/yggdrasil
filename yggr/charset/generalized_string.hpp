//generalized_string.hpp

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

#include <yggr/base/yggrdef.h>

#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/container/empty.hpp>
#include <yggr/container/size.hpp>

#include <boost/utility/enable_if.hpp>

#ifndef __YGGR_CHARSET_GENERALIZED_STRING_HPP__
#define __YGGR_CHARSET_GENERALIZED_STRING_HPP__

namespace yggr
{
namespace charset
{

namespace detail
{

template<typename T, 
			bool is_string_view = is_string_view_t<T>::value,
			bool is_container = container_ex::is_container<T>::value, 
			bool is_pointer = boost::is_pointer<T>::value>
struct is_generalized_string_t_empty_helper;

template<typename T, bool is_container>
struct is_generalized_string_t_empty_helper<T, true, is_container, false>
{
public:
	typedef T string_type;

public:
	inline bool operator()(const string_type& str) const
	{
		return str.empty();
	}
};

template<typename T>
struct is_generalized_string_t_empty_helper<T, false, true, false>
{
public:
	typedef T string_type;

public:
	inline bool operator()(const string_type& str) const
	{
		return container::empty(str);
	}
};

template<typename T>
struct is_generalized_string_t_empty_helper<T, false, false, true>
{
public:
	typedef T string_type;

public:
	inline bool operator()(string_type str) const
	{
		return (!str) || (str[0] == 0);
	}
};

} // namespace detial

template<typename String> inline
typename 
	boost::enable_if
	<
		is_generalized_string_t<String>,
		bool
	>::type
	generalized_string_empty(const String& str)
{
	typedef String string_type;
	typedef detail::is_generalized_string_t_empty_helper<string_type> helper_type;

	helper_type helper;
	return helper(str);
}

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_GENERALIZED_STRING_HPP__
