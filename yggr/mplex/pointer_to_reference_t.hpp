//pointer_to_reference_t.hpp

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

#ifndef __YGGR_MPLEX_POINTER_TO_REFERENCE_T_HPP__
#define __YGGR_MPLEX_POINTER_TO_REFERENCE_T_HPP__

#include <yggr/mplex/pointer_to_value_t.hpp>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_volatile.hpp>


namespace yggr
{
namespace mplex
{
namespace detail
{
template<typename T, bool is_native_ptr>
struct pointer_to_reference_t_imp;

template<typename T>
struct pointer_to_reference_t_imp<T, false>
	: public
		boost::add_reference
		<
			typename 
				boost::remove_volatile
				< 
					typename unwrap_pointer< T >::type 
				>::type
		>
{
};

template<typename T>
struct pointer_to_reference_t_imp<T, true>
	: public
		boost::add_reference
		< 
			typename 
				boost::remove_volatile
				<
					typename boost::remove_pointer<T>::type 
				>::type
		>
		
{
};

} // namespace detail

template<typename T>
struct pointer_to_reference_t
	: public detail::pointer_to_reference_t_imp<T, boost::is_pointer<T>::value>
{
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_POINTER_TO_REFERENCE_T_HPP__
