// boost_fusion_pair_midx_node_handle_patch.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_FUSION_PAIR_MIDX_NODE_HANDLE_PATCH_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_FUSION_PAIR_MIDX_NODE_HANDLE_PATCH_HPP__

#include <yggr/base/yggrdef.h>

#if !defined(_MSC_VER) && defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

#include <yggr/move/move.hpp>

#include <boost/ref.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/multi_index/detail/node_handle.hpp>

#if !defined(BOOST_FUSION_GPU_ENABLED)
#	define YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED
#else
#	define YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED BOOST_FUSION_GPU_ENABLED
#endif // BOOST_FUSION_GPU_ENABLED

#if !defined(BOOST_CONSTEXPR)
#	define YGGR_TMP_PP_BOOST_CONSTEXPR
#else
#	define YGGR_TMP_PP_BOOST_CONSTEXPR BOOST_CONSTEXPR
#endif // BOOST_CONSTEXPR

#if !defined(BOOST_CXX14_CONSTEXPR)
#	define YGGR_TMP_PP_BOOST_CXX14_CONSTEXPR
#else
#	define YGGR_TMP_PP_BOOST_CXX14_CONSTEXPR BOOST_CXX14_CONSTEXPR
#endif // BOOST_CXX14_CONSTEXPR

namespace boost
{
namespace fusion
{

template <typename First, typename Iterator,typename NodeHandl >
struct pair<First, boost::multi_index::detail::insert_return_type<Iterator, NodeHandl > >
{
public:
    typedef First first_type;
	typedef boost::multi_index::detail::insert_return_type<Iterator, NodeHandl > second_type;

private:
	typedef pair this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
    YGGR_TMP_PP_BOOST_CONSTEXPR YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED
    pair(void)
        : second()
	{
	}

	YGGR_TMP_PP_BOOST_CONSTEXPR YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED
    pair(BOOST_RV_REF(second_type) val)
        : second( boost::move(val) )
	{
	}

    YGGR_TMP_PP_BOOST_CXX14_CONSTEXPR YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED
    pair( BOOST_RV_REF(this_type) rhs)
        : second(boost::move(rhs.second))
	{
	}

	YGGR_TMP_PP_BOOST_CXX14_CONSTEXPR YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED
    pair( const this_type& rhs)
        : second(boost::move(const_cast<second_type&>(rhs.second)))
	{
	}

public:
	YGGR_TMP_PP_BOOST_CXX14_CONSTEXPR YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED
    this_type& operator=(BOOST_RV_REF(this_type) rhs)
    {
        second = boost::move(rhs.second);
        return *this;
    }

	YGGR_TMP_PP_BOOST_CXX14_CONSTEXPR YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED
    this_type& operator=(const this_type& rhs)
    {
        second = boost::move(const_cast<second_type&>(rhs.second));
        return *this;
    }

public:
    second_type second;
};

} // namespace fusion
} // namespace boost


#undef YGGR_TMP_PP_BOOST_FUSION_GPU_ENABLED
#undef YGGR_TMP_PP_BOOST_CONSTEXPR
#undef YGGR_TMP_PP_BOOST_CXX14_CONSTEXPR

#endif // #if !defined(_MSC_VER) && defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

#endif // __YGGR_CONTAINER_DETAIL_BOOST_FUSION_PAIR_MIDX_NODE_HANDLE_PATCH_HPP__
