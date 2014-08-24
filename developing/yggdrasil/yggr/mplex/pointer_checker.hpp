//pointer_check.hpp

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

#ifndef __YGGR_MPLEX_POINTER_CHECK_HPP__
#define __YGGR_MPLEX_POINTER_CHECK_HPP__

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_pointer.hpp>

#define YGGR_PP_IS_POINTER_WRAPPER_DEF( __type__ ) \
	namespace yggr { namespace mplex { \
	template<typename T> \
	struct is_pointer_wrapper< __type__ > : public boost::mpl::true_ { }; } }

namespace yggr
{
namespace mplex
{

template<typename T>
struct is_pointer_wrapper
	: public boost::mpl::false_
{
};

template<typename T>
struct is_pointer
	: public boost::mpl::or_
						<
							yggr::mplex::is_pointer_wrapper<T>,
							boost::is_pointer<T>
						>::type
{
};

template<typename T>
struct is_nude_pointer
	: public boost::is_pointer<T>::type
{
};

} // namespace mplex
} // namespace yggr

#include <yggr/mplex/detail/pointer_checker_of_wrapper.hpp>

#endif //__YGGR_MPLEX_POINTER_CHECK_HPP__