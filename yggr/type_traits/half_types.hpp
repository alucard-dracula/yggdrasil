//half_types.hpp

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

#ifndef __YGGR_TYPE_TRAITS_HALF_TYPES_HPP__
#define __YGGR_TYPE_TRAITS_HALF_TYPES_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/bytes_to_types.hpp>

#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#define YGGR_PP_HALF_T_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct half_t< __src__ > { \
		typedef __dst__ type; }; }

namespace yggr
{
namespace detail
{

template<typename T, std::size_t Mark>
struct half_t_switch
	: public mplex::null_type
{
};

template<typename T>
struct half_t_switch<T, 0x00000001>
	: public bytes_to_float<(sizeof(T) >> 1)>
{
};

template<typename T>
struct half_t_switch<T, 0x00000002>
	: public bytes_to_signed<(sizeof(T) >> 1)>
{
};

template<typename T>
struct half_t_switch<T, 0x00000004>
	: public bytes_to_unsigned<(sizeof(T) >> 1)>
{
};


template<typename T>
struct half_t_detail
	: public 
		half_t_switch
		<
			T,
			(static_cast<std::size_t>(boost::is_floating_point<T>::value) << 0)
			+ (static_cast<std::size_t>(boost::is_signed<T>::value) << 1)
			+ (static_cast<std::size_t>(boost::is_unsigned<T>::value) << 2)
		>
{
};

} // namespace detail

template<typename T>
struct half_t
	: public detail::half_t_detail<T>
{
};

} // namespace yggr


#endif //__YGGR_TYPE_TRAITS_HALF_TYPES_HPP__