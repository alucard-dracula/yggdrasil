//has_second_t.hpp

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

#ifndef __YGGR_TYPE_TRAITS_HAS_SECOND_T_HPP__
#define __YGGR_TYPE_TRAITS_HAS_SECOND_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/native_t.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{
namespace detail
{

template<typename Container>
struct _has_second_t 
{
private:
	typedef Container cont_type;

	typedef u8 true_type;
	typedef u32 false_type;
		
	template<typename T>
	static true_type chk_foo(typename ::yggr::native_t<typename T::second_type>::type*); // If T's parent defines the mappend_type then it is true
		
	template<typename T>
	static false_type chk_foo(...);
		
public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type;
};

} // namespace detail

template<typename Container>
struct has_second_t 
	: public detail::_has_second_t<Container>::type
{
};

} // namespace yggr


#endif // __YGGR_TYPE_TRAITS_HAS_MAPPED_T_HPP__