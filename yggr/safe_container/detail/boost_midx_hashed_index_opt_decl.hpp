// boost_midx_hashed_index_opt_decl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_HASHED_INDEX_OPT_DECL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_HASHED_INDEX_OPT_DECL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/container/multi_index_hashed_index.hpp>
#include <yggr/safe_container/detail/cast_midx_base_type.hpp>
#include <yggr/safe_container/detail/boost_midx_keyinfo_helper.hpp>

#include <boost/multi_index/hashed_index.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template
<
	typename KeyFromValue, typename Hash, typename Pred,
	typename SuperMeta, typename TagList, typename Category
>
class boost_midx_hashed_index_opt;

template<typename Arg1,
			typename Arg2 = boost::mpl::na,
			typename Arg3 = boost::mpl::na,
			typename Arg4 = boost::mpl::na >
struct boost_midx_hashed_non_unique_opt;

template<typename Arg1,
			typename Arg2 = boost::mpl::na,
			typename Arg3 = boost::mpl::na,
			typename Arg4 = boost::mpl::na >
struct boost_midx_hashed_unique_opt;

// cast decl
template<typename T>
struct cast_to_midx_hashed_index_opt;

template<typename T>
struct cast_to_midx_hashed_index_wrap_opt;

template<typename T>
struct cast_to_midx_hashed_index_org;

template<typename T>
struct cast_to_midx_hashed_index_wrap_org;

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
template<typename K, typename H, typename P, 
			typename S, typename T, typename C>
struct cast_to_midx_hashed_index_opt
		< 
			boost::multi_index::detail::hashed_index<K, H, P, S, T, C> 
		>
{
	typedef 
		boost_midx_hashed_index_opt
		<
			K, H, P, 
			typename nth_layer_opt_cast<S>::type, 
			T, C
		> type;
};

template<typename A1, typename A2, typename A3, typename A4>
struct cast_to_midx_hashed_index_wrap_opt
		<
			boost::multi_index::hashed_unique<A1, A2, A3, A4>
		>
{
	typedef boost_midx_hashed_unique_opt<A1, A2, A3, A4> type;
};

template<typename A1, typename A2, typename A3, typename A4>
struct cast_to_midx_hashed_index_wrap_opt
		<
			boost::multi_index::hashed_non_unique<A1, A2, A3, A4>
		>
{
	typedef boost_midx_hashed_non_unique_opt<A1, A2, A3, A4> type;
};

// cast to org
template<typename K, typename H, typename P, 
			typename S, typename T, typename C>
struct cast_to_midx_hashed_index_org
		<
			boost_midx_hashed_index_opt<K, H, P, S, T, C>
		>
{
	typedef 
		boost::multi_index::detail::hashed_index
		<
			K, H, P, 
			typename nth_layer_org_cast<S>::type, 
			T, C
		> type;
};

template<typename A1, typename A2, typename A3, typename A4>
struct cast_to_midx_hashed_index_wrap_org
		<
			boost_midx_hashed_unique_opt<A1, A2, A3, A4>
		>
{
	typedef boost::multi_index::hashed_unique<A1, A2, A3, A4> type;
};

template<typename A1, typename A2, typename A3, typename A4>
struct cast_to_midx_hashed_index_wrap_org
		<
			boost_midx_hashed_non_unique_opt<A1, A2, A3, A4>
		>
{
	typedef boost::multi_index::hashed_non_unique<A1, A2, A3, A4> type;
};

// index_opt_cast
template<typename K, typename H, typename P, 
			typename S, typename T, typename C >
struct index_opt_cast< boost::multi_index::detail::hashed_index<K, H, P, S, T, C> >
{
private:
	typedef boost::multi_index::detail::hashed_index<K, H, P, S, T, C> src_type;
public:
	typedef typename cast_to_midx_hashed_index_opt<src_type>::type type;
};

// index_org_cast
template<typename K, typename H, typename P, 
			typename S, typename T, typename C >
struct index_org_cast< boost_midx_hashed_index_opt<K, H, P, S, T, C> >
{
private:
	typedef boost_midx_hashed_index_opt<K, H, P, S, T, C> src_type;
public:
	typedef typename cast_to_midx_hashed_index_org<src_type>::type type;
};

// wrap_opt_cast
template<typename T1, typename T2, typename T3, typename T4>
struct index_wrap_opt_cast<boost::multi_index::hashed_non_unique<T1, T2, T3, T4> >
{
private:
	typedef boost::multi_index::hashed_non_unique<T1, T2, T3, T4> src_type;
public:
	typedef typename cast_to_midx_hashed_index_wrap_opt<src_type>::type type;
};

template<typename T1, typename T2, typename T3, typename T4>
struct index_wrap_opt_cast<boost::multi_index::hashed_unique<T1, T2, T3, T4> >
{
private:
	typedef boost::multi_index::hashed_unique<T1, T2, T3, T4> src_type;
public:
	typedef typename cast_to_midx_hashed_index_wrap_opt<src_type>::type type;
};

// wrap_org_cast
template<typename T1, typename T2, typename T3, typename T4>
struct index_wrap_org_cast< boost_midx_hashed_non_unique_opt<T1, T2, T3, T4> >
{
private:
	typedef boost_midx_hashed_non_unique_opt<T1, T2, T3, T4> src_type;
public:
	typedef typename cast_to_midx_hashed_index_wrap_org<src_type>::type type;
};

template<typename T1, typename T2, typename T3, typename T4>
struct index_wrap_org_cast<boost_midx_hashed_unique_opt<T1, T2, T3, T4> >
{
private:
	typedef boost_midx_hashed_unique_opt<T1, T2, T3, T4> src_type;
public:
	typedef typename cast_to_midx_hashed_index_wrap_org<src_type>::type type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

YGGR_PP_CONTAINER_DETAIL_KEYHASH_META_PAIR_DEF(yggr::safe_container::detail::boost_midx_hashed_unique_opt);
YGGR_PP_CONTAINER_DETAIL_KEYHASH_META_PAIR_DEF(yggr::safe_container::detail::boost_midx_hashed_non_unique_opt);

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_HASHED_INDEX_OPT_DECL_HPP__
