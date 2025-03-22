// is_convertible.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_TYPE_TRAITS_IS_CONVERTIBLE_HPP__
#define __YGGR_TYPE_TRAITS_IS_CONVERTIBLE_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/add_lvalue_reference.hpp>

namespace yggr
{
namespace detail
{


#if defined(_MSC_VER) && (_MSC_VER >= 1400)

template <typename T, typename U>
struct is_convertible
	: public boost::mpl::bool_< __is_convertible_to(T, U) >
{
};

#else

template <typename From, typename To>
struct is_convertible_impl
{
private:
	typedef typename boost::add_lvalue_reference<From>::type t_reference;
	typedef u8 true_t;
	typedef u32 false_t;

	static false_t dispatch(...);
	static true_t dispatch(To);

	static t_reference trigger(void);

public:
	typedef boost::mpl::bool_< (sizeof(dispatch(trigger())) == sizeof(true_t)) > type;
};

template <typename T, typename U>
struct is_convertible
	: public is_convertible_impl<T, U>::type
{
};

#endif // (_MSC_VER) && (_MSC_VER >= 1400)

} // namespace detail

using detail::is_convertible;

} // namespace yggr

#endif // __YGGR_TYPE_TRAITS_IS_CONVERTIBLE_HPP__
