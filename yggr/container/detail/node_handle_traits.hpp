//node_handle_traits.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_TRAITS_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_TRAITS_HPP__

#include <yggr/mplex/null_t.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/container/detail/native_extractable.hpp>
#include <yggr/container/detail/native_node_insertable.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T>
struct node_handle_traits_chk
	: public
		boost::mpl::and_
		<
			has_member_t_node_type<T>,
			has_member_t_insert_return_type<T>
		>::type
{
};

template<typename T,
			bool has_extract = native_extractable<T>::value, 
			bool has_node_insert = native_node_insertable<T>::value>
struct node_handle_traits_impl_default
{
	typedef mplex::null_type node_type;
	typedef mplex::null_type insert_return_type;
	typedef mplex::null_type hint_insert_return_type;
};

template<typename T>
struct node_handle_traits_impl_default<T, true, true>
{
private:
	typedef typename T::node_type tmp_node_type;
	typedef typename T::const_iterator tmp_const_iterator;
	static T cont;
	static tmp_node_type tmp_node;
	static tmp_const_iterator tmp_citer;

	typedef YGGR_TYPEOF_TPL(cont.insert(boost::move(tmp_node))) rst_type;
	//typedef BOOST_TYPEOF_TPL(cont.insert(tmp_citer, boost::move(tmp_node))) rst_hint_type;

public:
	typedef typename T::node_type node_type;
	typedef rst_type insert_return_type;
	typedef typename T::iterator hint_insert_return_type;
};

template<typename T>
struct node_handle_traits_impl_custom
{
};

template<typename T>
struct node_handle_traits
	: public
		boost::mpl::if_
		<
			node_handle_traits_chk< node_handle_traits_impl_custom<T> >,
			node_handle_traits_impl_custom<T>,
			node_handle_traits_impl_default<T>
		>::type
{
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_TRAITS_HPP__
