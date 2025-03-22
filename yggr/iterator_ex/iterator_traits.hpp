// iterator_traits.hpp

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

#ifndef __YGGR_ITERATOR_EX_ITERATOR_TRAITS_HPP__
#define __YGGR_ITERATOR_EX_ITERATOR_TRAITS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/iterator_ex/detail/iterator_traits_impl.hpp>

#if !(BOOST_VERSION < 105600)
#	include <yggr/iterator_ex/is_const_iterator.hpp>
#endif // !(BOOST_VERSION < 105600)

#include <boost/iterator/iterator_traits.hpp>

#if !(BOOST_VERSION < 105600)
#	include <boost/type_traits/add_reference.hpp>
#	include <boost/type_traits/is_base_of.hpp>
#endif // !(BOOST_VERSION < 105600)

namespace yggr
{
namespace iterator_ex
{

template<typename Iter>
struct iterator_value_ex
	: public detail::iterator_traits_impl<Iter, boost::iterator_value>
{
};

#if (BOOST_VERSION < 105600)

template<typename Iter>
struct iterator_reference_ex
	: public detail::iterator_traits_impl<Iter, boost::iterator_reference>
{
};

#else

template<typename Iter>
struct iterator_reference_ex
{
private:
	typedef Iter iter_type;
	typedef typename detail::iterator_traits_impl<iter_type, boost::iterator_reference>::type ref_type;
	typedef typename detail::iterator_traits_impl<iter_type, boost::iterator_value>::type val_type;
	typedef typename 
		boost::mpl::if_
		<
			boost::is_base_of<mplex::null_type, ref_type>,
			ref_type,
			typename
				boost::mpl::if_
				<
					is_const_iterator<iter_type>,
					typename boost::add_reference<const val_type>::type,
					typename boost::add_reference<val_type>::type
				>::type
		>::type ret_ref_type;

public:
	typedef ret_ref_type type;
};

#endif // (BOOST_VERSION < 105600)

template<typename Iter>
struct iterator_pointer_ex
	: public detail::iterator_traits_impl<Iter, boost::iterator_pointer>
{
};

template<typename Iter>
struct iterator_difference_ex
	: public detail::iterator_traits_impl<Iter, boost::iterator_difference>
{
};

template<typename Iter>
struct iterator_category_ex
	: public detail::iterator_traits_impl<Iter, boost::iterator_category>
{
};

} // namespace iterator_ex

using boost::iterator_value;
using boost::iterator_reference;
using boost::iterator_pointer;
using boost::iterator_difference;
using boost::iterator_category;

using iterator_ex::iterator_value_ex;
using iterator_ex::iterator_reference_ex;
using iterator_ex::iterator_pointer_ex;
using iterator_ex::iterator_difference_ex;
using iterator_ex::iterator_category_ex;

} // namespace yggr

#endif // __YGGR_ITERATOR_EX_ITERATOR_TRAITS_HPP__

