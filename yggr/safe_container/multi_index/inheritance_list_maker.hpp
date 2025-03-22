//inheritance_list_maker.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INHERITANCE_LIST_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INHERITANCE_LIST_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/tuple/tuple.hpp>

#include <yggr/safe_container/multi_index/index_tag.hpp>
#include <yggr/safe_container/multi_index/safe_index_maker.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size.hpp>

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

template<int I, int N, typename IN_Midx, typename IN_Mutex>
struct inheritance_list_maker_cons_maker
{
	typedef IN_Midx in_midx_type;
	typedef typename in_midx_type::index_type_list in_index_type_list_type;
	typedef typename thread::mutex_def_helper<IN_Mutex>::mutex_type in_mutex_type;
	typedef 
		::yggr::tuples::cons
		<
			typename
				safe_index_maker
				<
					I,
					IN_Midx,
					IN_Mutex,
					typename 
						boost::mpl::at_c<in_index_type_list_type, I>::type
				>::type,
			typename
				inheritance_list_maker_cons_maker<I + 1, N, IN_Midx, IN_Mutex>::type
		> type;

	inline type operator()(in_mutex_type& mutex, in_midx_type& midx) const
	{
		typedef typename type::head_type now_data_type;
		typedef inheritance_list_maker_cons_maker<I + 1, N, IN_Midx, IN_Mutex> next_maker_type;
		next_maker_type next_maker;
		return type(now_data_type(mutex, midx), next_maker(mutex, midx));
	}
};

template<int N, typename IN_Midx, typename IN_Mutex>
struct inheritance_list_maker_cons_maker<N, N, IN_Midx, IN_Mutex>
{
	typedef IN_Midx in_midx_type;
	typedef typename thread::mutex_def_helper<IN_Mutex>::mutex_type in_mutex_type;
	typedef ::yggr::tuples::null_type type;

	inline type operator()(in_mutex_type&, in_midx_type&) const
	{
		return type();
	}
};

template<typename Midx, typename Mutex>
struct inheritance_list_maker
{
private:
	typedef Midx midx_container_type;
	typedef typename midx_container_type::index_type_list index_type_list_type;
	typedef typename thread::mutex_def_helper<Mutex>::mutex_type mutex_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(!!(boost::mpl::size<index_type_list_type>::value))>));

private:
	typedef
		inheritance_list_maker_cons_maker
		<
			1, 
			boost::mpl::size<index_type_list_type>::value,
			Midx,
			Mutex
		> maker_type;

public:
	typedef typename maker_type::type type;

public:
	inline type operator()(mutex_type& mutex, midx_container_type& midx) const
	{
		maker_type maker; 
		return maker(mutex, midx);
	}
};

// make_inheritance_list

template<typename Mutex, typename Midx> inline
typename inheritance_list_maker<Midx, Mutex>::type
	make_inheritance_list(typename thread::mutex_def_helper<Mutex>::mutex_type& mutex,
							Midx& midx)
{
	typedef inheritance_list_maker<Midx, Mutex> maker_type;

	maker_type maker;
	return maker(mutex, midx);
}

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INHERITANCE_LIST_HPP__
