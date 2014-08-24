//iterator_to_value_t.hpp

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

#ifndef __YGGR_MPLEX_ITERATOR_TO_VALUE_Y_HPP__
#define __YGGR_MPLEX_ITERATOR_TO_VALUE_Y_HPP__

#include <yggr/mplex/pointer_to_value_t.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace yggr
{
namespace mplex
{
	template<typename Iterator, bool is_pointer>
	struct iterator_to_value_t_impl;

	template<typename Iterator>
	struct iterator_to_value_t_impl<Iterator, true>
	{
		typedef typename pointer_to_value_t<Iterator>::type type;
	};

	template<typename Iterator>
	struct iterator_to_value_t_impl<Iterator, false>
	{
		typedef typename boost::remove_const<typename Iterator::value_type>::type type;
	};

	template<typename Iterator>
	struct iterator_to_value_t
	{
		typedef typename iterator_to_value_t_impl<Iterator, boost::is_pointer<Iterator>::value>::type type;
		//typedef const type lc_type;
		//typedef type const rc_type;
		//typedef const type const lrc_type;
	};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_ITERATOR_TO_VALUE_Y_HPP__
