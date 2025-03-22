// boost_midx_ordered_index_opt_basic_decl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_ORDERED_INDEX_OPT_BASIC_DECL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_ORDERED_INDEX_OPT_BASIC_DECL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/container/multi_index_ordered_index.hpp>

#include <yggr/safe_container/detail/cast_midx_base_type.hpp>
#include <yggr/safe_container/detail/boost_midx_keyinfo_helper.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

#ifdef YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

template
<
	typename KeyFromValue, typename Compare,
	typename SuperMeta, typename TagList, typename Category,
	typename AugmentPolicy
>
class boost_midx_ordered_index_opt;

#else

template
<
	typename KeyFromValue, typename Compare,
	typename SuperMeta, typename TagList, typename Category
>
class boost_midx_ordered_index_opt;

#endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

template<typename T>
struct cast_to_midx_ordered_index_opt;

template<typename T>
struct cast_to_midx_ordered_index_org;

} // namespace detail
} // namespace safe_container
} // namespace yggr

// cast
namespace yggr
{
namespace safe_container
{
namespace detail
{

// cast to opt

#ifdef YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

template<typename K, typename P, 
			typename S, typename T, typename C, typename A>
struct cast_to_midx_ordered_index_opt
		< 
			boost::multi_index::detail::ordered_index<K, P, S, T, C, A> 
		>
{
	typedef 
		boost_midx_ordered_index_opt
		<
			K, P, 
			typename nth_layer_opt_cast<S>::type, 
			T, C, A
		> type;
};

#else

template<typename K, typename P, 
			typename S, typename T, typename C>
struct cast_to_midx_ordered_index_opt
		< 
			boost::multi_index::detail::ordered_index<K, P, S, T, C> 
		>
{
	typedef 
		boost_midx_ordered_index_opt
		<
			K, P, 
			typename nth_layer_opt_cast<S>::type, 
			T, C
		> type;
};

#endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

// cast to org

#ifdef YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

template<typename K, typename P, 
			typename S, typename T, typename C, typename A>
struct cast_to_midx_ordered_index_org
		<
			boost_midx_ordered_index_opt<K, P, S, T, C, A>
		>
{
	typedef 
		boost::multi_index::detail::ordered_index
		<
			K, P, 
			typename nth_layer_opt_cast<S>::type, 
			T, C, A
		> type;
};

#else

template<typename K, typename P, 
			typename S, typename T, typename C>
struct cast_to_midx_ordered_index_org
		<
			boost_midx_ordered_index_opt<K, P, S, T, C>
		>
{
	typedef 
		boost::multi_index::detail::ordered_index
		<
			K, P, 
			typename nth_layer_opt_cast<S>::type, 
			T, C
		> type;
};

#endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX


#ifdef YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

template<typename K, typename P, 
			typename S, typename T, typename C, typename A >
struct index_opt_cast<boost::multi_index::detail::ordered_index<K, P, S, T, C, A> >
{
private:
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> src_type;
public:
	typedef typename cast_to_midx_ordered_index_opt<src_type>::type type;
};

// index_org_cast
template<typename K, typename P, 
			typename S, typename T, typename C, typename A >
struct index_org_cast< boost_midx_ordered_index_opt<K, P, S, T, C, A> >
{
private:
	typedef boost_midx_ordered_index_opt<K, P, S, T, C, A> src_type;
public:
	typedef typename cast_to_midx_ordered_index_org<src_type>::type type;
};

#else

template<typename K, typename P, 
			typename S, typename T, typename C >
struct index_opt_cast<boost::multi_index::detail::ordered_index<K, P, S, T, C> >
{
private:
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> src_type;
public:
	typedef typename cast_to_midx_ordered_index_opt<src_type>::type type;
};

// index_org_cast
template<typename K, typename P, 
			typename S, typename T, typename C >
struct index_org_cast< boost_midx_ordered_index_opt<K, P, S, T, C> >
{
private:
	typedef boost_midx_ordered_index_opt<K, P, S, T, C> src_type;
public:
	typedef typename cast_to_midx_ordered_index_org<src_type>::type type;
};

#endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_ORDERED_INDEX_OPT_BASIC_DECL_HPP__
