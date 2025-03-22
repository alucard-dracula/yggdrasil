// boost_midx_sequenced_index_opt_decl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_SEQUENCED_INDEX_OPT_DECL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_SEQUENCED_INDEX_OPT_DECL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/container/multi_index_sequenced_index.hpp>
#include <yggr/safe_container/detail/cast_midx_base_type.hpp>
#include <yggr/safe_container/detail/boost_midx_keyinfo_helper.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename SuperMeta, typename TagList>
class boost_midx_sequenced_index_opt;

template<typename TagList>
struct boost_midx_sequenced_opt;


template<typename T>
struct cast_to_midx_sequenced_index_opt;

template<typename T>
struct cast_to_midx_sequenced_index_wrap_opt;

template<typename T>
struct cast_to_midx_sequenced_index_org;

template<typename T>
struct cast_to_midx_sequenced_index_wrap_org;

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
template<typename S, typename T>
struct cast_to_midx_sequenced_index_opt
		< 
			boost::multi_index::detail::sequenced_index<S, T> 
		>
{
	typedef 
		boost_midx_sequenced_index_opt
		<
			typename nth_layer_opt_cast<S>::type, 
			T
		> type;
};

template<typename TL>
struct cast_to_midx_sequenced_index_wrap_opt
		<
			boost::multi_index::sequenced<TL>
		>
{
	typedef boost_midx_sequenced_opt<TL> type;
};

// cast to org
template<typename S, typename T>
struct cast_to_midx_sequenced_index_org
		<
			boost_midx_sequenced_index_opt<S, T>
		>
{
	typedef 
		boost::multi_index::detail::sequenced_index
		<
			typename nth_layer_org_cast<S>::type, 
			T
		> type;
};

template<typename TL>
struct cast_to_midx_sequenced_index_wrap_org
		<
			boost_midx_sequenced_opt<TL>
		>
{
	typedef boost::multi_index::sequenced<TL> type;
};

// index_opt_cast
template<typename S, typename T>
struct index_opt_cast< boost::multi_index::detail::sequenced_index<S, T> >
{
private:
	typedef boost::multi_index::detail::sequenced_index<S, T> src_type;
public:
	typedef typename cast_to_midx_sequenced_index_opt<src_type>::type type;
};

// index_org_cast
template<typename S, typename T>
struct index_org_cast< boost_midx_sequenced_index_opt<S, T> >
{
private:
	typedef boost_midx_sequenced_index_opt<S, T> src_type;
public:
	typedef typename cast_to_midx_sequenced_index_org<src_type>::type type;
};

// wrap_opt_cast
template<typename TL>
struct index_wrap_opt_cast< boost::multi_index::sequenced<TL> >
{
private:
	typedef boost::multi_index::sequenced<TL> src_type;
public:
	typedef typename cast_to_midx_sequenced_index_wrap_opt<src_type>::type type;
};

// wrap_org_cast
template<typename TL>
struct index_wrap_org_cast< boost_midx_sequenced_opt<TL> >
{
private:
	typedef boost_midx_sequenced_opt<TL> src_type;
public:
	typedef typename 
		cast_to_midx_sequenced_index_wrap_org<src_type>::type type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_SEQUENCED_INDEX_OPT_DECL_HPP__
