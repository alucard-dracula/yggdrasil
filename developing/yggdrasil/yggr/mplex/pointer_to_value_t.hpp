//pointer_to_value_t.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__
#define __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__

//#include <boost/intrusive/pointer_traits.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/if.hpp>

#define YGGR_PP_POINTER_UNWRAP_DEF( __type__ ) \
	namespace yggr { namespace mplex { \
	template<typename T> \
	struct unwrap_pointer< __type__ > { \
		typedef T type; }; } }

namespace yggr
{
namespace mplex
{

template<typename T>
struct unwrap_pointer
{
	typedef T type;
};

template<typename T, bool is_nude_pointer>
struct pointer_to_value_t_imp;

template<typename T>
struct pointer_to_value_t_imp<T, false>
{
	typedef typename boost::remove_const< typename unwrap_pointer< T >::type >::type type;
};

template<typename T>
struct pointer_to_value_t_imp<T, true>
{
	//typedef typename boost::remove_const< typename boost::intrusive::pointer_traits<T>::element_type >::type type;
	typedef typename boost::remove_const<typename boost::remove_pointer<T>::type >::type type;
};

template<typename T>
struct pointer_to_value_t
{
	typedef typename pointer_to_value_t_imp<T, boost::is_pointer<T>::value>::type type;
};

} // namespace mplex
} // namespace yggr

#include <yggr/mplex/detail/pointer_to_value_t_of_wrapper.hpp>

#endif // __YGGR_MPLEX_POINTER_TO_VALUE_T_HPP__