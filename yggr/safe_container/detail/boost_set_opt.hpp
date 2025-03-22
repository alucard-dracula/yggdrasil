// boost_set_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_SET_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_SET_OPT_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/container/set.hpp>
#include <yggr/safe_container/detail/boost_container_opt_path.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

#if BOOST_VERSION < 105600

template<typename Key, typename Compare, typename Allocator>
class boost_set_opt;

template <typename Key, typename Compare, typename Allocator>
class boost_multiset_opt;

#else

template<typename Key, typename Compare, typename Allocator, typename Options>
class boost_set_opt;

template <typename Key, typename Compare, typename Allocator, typename Options>
class boost_multiset_opt;

#endif // BOOST_VERSION < 105600


template<typename T>
struct cast_to_set_opt
{
	typedef T type;
};

template<typename T>
struct cast_to_multiset_opt
{
	typedef T type;
};


#if BOOST_VERSION < 105600

template<typename K, typename C, typename A>
struct cast_to_set_opt< boost::container::set<K, C, A> >
{
	typedef boost_set_opt<K, C, A> type;
};

template<typename K, typename C, typename A>
struct cast_to_multiset_opt< boost::container::multiset<K, C, A> >
{
	typedef boost_multiset_opt<K, C, A> type;
};

#else

template<typename K, typename C, typename A, typename O>
struct cast_to_set_opt< boost::container::set<K, C, A, O> >
{
	typedef boost_set_opt<K, C, A, O> type;
};

template<typename K, typename C, typename A, typename O>
struct cast_to_multiset_opt< boost::container::multiset<K, C, A, O> >
{
	typedef boost_multiset_opt<K, C, A, O> type;
};

template<typename K, typename C, typename A>
struct cast_to_set_opt< yggr::container::detail::set<K, C, A> >
	: public cast_to_set_opt<typename yggr::container::detail::set<K, C, A>::base_type>
{
};

template<typename K, typename C, typename A>
struct cast_to_multiset_opt< yggr::container::detail::multiset<K, C, A> >
	: public cast_to_multiset_opt<typename yggr::container::detail::multiset<K, C, A>::base_type>
{
};

#endif // BOOST_VERSION < 105600


} // namespace detail
} // namespace safe_container
} // namespace yggr

#include YGGR_PP_BOOST_CONTAINER_OPT_LATEST_VERSION_PATH(boost_set_opt)

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif //__YGGR_SAFE_CONTAINER_DETAIL_BOOST_SET_OPT_HPP__ 

