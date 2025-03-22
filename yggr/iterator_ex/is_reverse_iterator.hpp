//is_reverse_iterator.hpp

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

#ifndef __YGGR_ITERATOR_EX_IS_REVERSE_ITERATOR_HPP__
#define __YGGR_ITERATOR_EX_IS_REVERSE_ITERATOR_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/compatibility/boost_iterator.hpp>
#include <yggr/type_traits/native_t.hpp>
#include <yggr/mplex/typename_count.hpp>

#include <yggr/iterator_ex/has_iterator_category.hpp>

#include <boost/iterator/reverse_iterator.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>

#include <iterator>

namespace yggr
{
namespace iterator_ex
{
namespace detail
{

template<typename OrgIter>
struct __has_iterator_type_t
{
private:
	typedef OrgIter org_iter_type;

	typedef u8 true_type;
	typedef u32 false_type;

	template<typename T>
	static true_type chk_foo(typename ::yggr::native_t<typename T::iterator_type>::type*); // If T's parent defines the mappend_type then it is true

	template<typename T>
	static false_type chk_foo(...);

public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<org_iter_type>(0)))> type;
};

template<typename OrgIter>
struct _has_iterator_type_t
	: public __has_iterator_type_t<OrgIter>::type
{
};

// don't check .base() function,
// because std::reverse_iterator's base() function is written in the subclass

template<typename T>
struct _is_reverse_iterator_t
	: public
		boost::mpl::bool_
		<
			boost::mpl::and_
			<
				boost::mpl::bool_<(mplex::typename_count<T>::value == 1)>,
				has_iterator_category<T>,
				_has_iterator_type_t<T>
			>::type::value
		>
{
};

template<typename OrgIter>
struct _is_reverse_iterator_t< std::reverse_iterator<OrgIter> >
	: public boost::mpl::true_
{
};

// boost reverse_iterator not exists iterator_type
template<typename OrgIter>
struct _is_reverse_iterator_t< boost::reverse_iterator<OrgIter> >
	: public boost::mpl::true_
{
};

#if defined(__clang__) && (__clang__) \
		&& defined(_LIBCPP_BEGIN_NAMESPACE_STD) && defined(_LIBCPP_ABI_NAMESPACE)

template<typename OrgIter>
struct _is_reverse_iterator_t< std::_LIBCPP_ABI_NAMESPACE::__wrap_iter<OrgIter> >
	: public boost::mpl::false_
{
};

#endif // ANDROID

// custom reverse_iterator needs to be imported (e.g. YGGR_IS_REVERSE_ITERATOR_IMPORT(iter_type, true))
template<typename T>
struct is_reverse_iterator_t_impl
	: public boost::mpl::false_
{
};

template<typename T, bool b = is_reverse_iterator_t_impl<T>::value >
struct is_reverse_iterator_t_rst;

template<typename T>
struct is_reverse_iterator_t_rst<T, false>
	: detail::_is_reverse_iterator_t<T>
{
};

template<typename T>
struct is_reverse_iterator_t_rst<T, true>
	: boost::mpl::true_
{
};


} // namespace detail

template<typename T>
struct is_reverse_iterator
	: public detail::is_reverse_iterator_t_rst<T>
{
};

} // namespace iterator_ex

using iterator_ex::is_reverse_iterator;

} // namespace yggr


template<typename T>
struct is_reverse_iterator_t_impl
	: public boost::mpl::false_
{
};

// custom reverse_iterator needs to be imported

#define YGGR_IS_REVERSE_ITERATOR_IMPORT(__type__, __value__) \
	namespace yggr{ namespace iterator_ex{ namespace detail{ \
	template<> \
	struct is_reverse_iterator_t_impl< __type__ > \
		: public boost::mpl::bool_< __value__ > {}; }}}

#endif // __YGGR_ITERATOR_EX_IS_REVERSE_ITERATOR_HPP__
