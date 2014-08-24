//reference_to_value_t.hpp

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

#ifndef __YGGR_MPLEX_REFERENCE_TO_VALUE_T_HPP__
#define __YGGR_MPLEX_REFERENCE_TO_VALUE_T_HPP__

#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/ref.hpp>

namespace yggr
{
namespace mplex
{

template<typename T, bool is_reference>
struct reference_to_value_t_imp;

template<typename T>
struct reference_to_value_t_imp<T, false>
{
	typedef typename boost::remove_const< typename boost::unwrap_reference<T>::type >::type type;
};

template<typename T>
struct reference_to_value_t_imp<T, true>
{
	typedef typename boost::remove_const< typename boost::remove_reference<T>::type >::type type;
};

template<typename T>
struct reference_to_value_t
{
	typedef typename reference_to_value_t_imp<T, boost::is_reference<T>::value>::type type;
};

} // namespace mplex
} // namespace yggr

#endif //__YGGR_MPLEX_REFERENCE_TO_VALUE_T_HPP__