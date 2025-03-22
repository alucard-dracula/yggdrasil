// has_iterator_category.hpp

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

#ifndef __YGGR_ITERATOR_EX_HAS_ITERATOR_CATEGORY_HPP__
#define __YGGR_ITERATOR_EX_HAS_ITERATOR_CATEGORY_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/native_t.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{
namespace iterator_ex
{
namespace detail
{

template<typename Iter>
struct _has_iterator_category_t 
{
private:
	typedef Iter iter_type;

	typedef u8 true_type;
	typedef u32 false_type;
		
	template<typename T>
	static true_type chk_foo(typename ::yggr::native_t<typename T::iterator_category>::type*); // If T's parent defines the mappend_type then it is true
		
	template<typename T>
	static false_type chk_foo(...);
		
public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<iter_type>(0)))> type;
};

} // namespace detail

template<typename Iter>
struct has_iterator_category 
	: public detail::_has_iterator_category_t<Iter>::type
{
};

} // namespace iterator_ex

using iterator_ex::has_iterator_category;

} // namespace yggr

#endif // __YGGR_ITERATOR_EX_HAS_ITERATOR_CATEGORY_HPP__
