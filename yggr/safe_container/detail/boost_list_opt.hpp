// boost_list_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_LIST_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_LIST_OPT_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/container/list.hpp>
#include <yggr/safe_container/detail/boost_container_opt_path.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename T, typename Allocator>
class boost_list_opt;

template<typename T>
struct cast_to_list_opt
{
	typedef T type;
};

template<typename T, typename A>
struct cast_to_list_opt< boost::container::list< T, A > >
{
	typedef 
		boost_list_opt
		<
			typename boost::container::list< T, A >::value_type,
			typename boost::container::list< T, A >::allocator_type
		> type;
};

#if !(BOOST_VERSION < 107000)

template<typename T, typename A>
struct cast_to_list_opt< yggr::container::detail::list< T, A > >
	: public cast_to_list_opt<typename yggr::container::detail::list< T, A >::base_type >
{
};

#endif // !(BOOST_VERSION < 107000)

} // namespace detail
} // namespace safe_container
} // namespace yggr

#include YGGR_PP_BOOST_CONTAINER_OPT_LATEST_VERSION_PATH(boost_list_opt)

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_LIST_OPT_HPP__
