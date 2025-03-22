// boost_midx_ordered_index_opt_decl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_ORDERED_INDEX_OPT_DECL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_ORDERED_INDEX_OPT_DECL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/safe_container/detail/boost_midx_ordered_index_opt_basic_decl.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Arg1,
			typename Arg2 = boost::mpl::na,
			typename Arg3 = boost::mpl::na>
struct boost_midx_ordered_unique_opt;

template<typename Arg1,
			typename Arg2 = boost::mpl::na,
			typename Arg3 = boost::mpl::na >
struct boost_midx_ordered_non_unique_opt;


template<typename T>
struct cast_to_midx_ordered_index_wrap_opt;

template<typename T>
struct cast_to_midx_ordered_index_wrap_org;

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

template<typename A1, typename A2, typename A3>
struct cast_to_midx_ordered_index_wrap_opt
		<
			boost::multi_index::ordered_unique<A1, A2, A3>
		>
{
	typedef boost_midx_ordered_unique_opt<A1, A2, A3> type;
};

template<typename A1, typename A2, typename A3>
struct cast_to_midx_ordered_index_wrap_opt
		<
			boost::multi_index::ordered_non_unique<A1, A2, A3>
		>
{
	typedef boost_midx_ordered_non_unique_opt<A1, A2, A3> type;
};

// cast to org

template<typename A1, typename A2, typename A3>
struct cast_to_midx_ordered_index_wrap_org
		<
			boost_midx_ordered_unique_opt<A1, A2, A3>
		>
{
	typedef boost::multi_index::ordered_unique<A1, A2, A3> type;
};

template<typename A1, typename A2, typename A3>
struct cast_to_midx_ordered_index_wrap_org
		<
			boost_midx_ordered_non_unique_opt<A1, A2, A3>
		>
{
	typedef boost::multi_index::ordered_non_unique<A1, A2, A3> type;
};

// wrap_opt_cast
template<typename A1, typename A2, typename A3>
struct index_wrap_opt_cast<boost::multi_index::ordered_non_unique<A1, A2, A3> >
{
private:
	typedef boost::multi_index::ordered_non_unique<A1, A2, A3> src_type;
public:
	typedef typename cast_to_midx_ordered_index_wrap_opt<src_type>::type type;
};

template<typename A1, typename A2, typename A3>
struct index_wrap_opt_cast<boost::multi_index::ordered_unique<A1, A2, A3> >
{
private:
	typedef boost::multi_index::ordered_unique<A1, A2, A3> src_type;
public:
	typedef typename cast_to_midx_ordered_index_wrap_opt<src_type>::type type;
};

// wrap_org_cast
template<typename A1, typename A2, typename A3>
struct index_wrap_org_cast< boost_midx_ordered_non_unique_opt<A1, A2, A3> >
{
private:
	typedef boost_midx_ordered_non_unique_opt<A1, A2, A3> src_type;
public:
	typedef typename cast_to_midx_ordered_index_wrap_org<src_type>::type type;
};

template<typename A1, typename A2, typename A3>
struct index_wrap_org_cast<boost_midx_ordered_unique_opt<A1, A2, A3> >
{
private:
	typedef boost_midx_ordered_unique_opt<A1, A2, A3> src_type;
public:
	typedef typename cast_to_midx_ordered_index_wrap_org<src_type>::type type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_ORDERED_INDEX_OPT_DECL_HPP__
