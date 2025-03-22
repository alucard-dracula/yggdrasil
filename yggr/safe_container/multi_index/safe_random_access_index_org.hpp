//safe_random_access_index_org.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_ORG_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_ORG_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_MULTI_RANDEOM_ACCESS_INDEX_HPP__
#	error "please include<yggr/safe_container/safe_multi_index_random_access_index.hpp>"
#endif // __YGGR_SAFE_CONTAINER_SAFE_MULTI_RANDEOM_ACCESS_INDEX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/thread/mutex_def_helper.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/any_val/operator_wrap.hpp>

#include <yggr/container/detail/node_handle_midx.hpp>
#include <yggr/container/node_handle_traits.hpp>
#include <yggr/container/multi_index_random_access_index.hpp>

#include <yggr/safe_container/multi_index/safe_random_access_index_decl.hpp>
#include <yggr/safe_container/multi_index/indexes_comparer_def.hpp>

#include <yggr/safe_container/multi_index/indexes_constructor_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_genel_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_swap_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_linear_gernel_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_operator_set_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_push_pop_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_insert_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_extract_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_erase_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_emplace_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_assign_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_replace_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_modify_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_modify_key_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_find_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_count_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_equal_range_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_using_handler_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_safe_other_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_range_size_def.hpp>

#include <yggr/safe_container/multi_index/indexes_swap_def.hpp>

#include <yggr/safe_container/detail/for_each_impl.hpp>
#include <yggr/safe_container/detail/begin_rbegin_value_impl.hpp>
#include <yggr/safe_container/detail/safe_insert_return_type_base.hpp>

#include <yggr/safe_container/safe_container_error.hpp>

#include <yggr/safe_container/multi_index/safe_multi_index_container_decl.hpp>
#include <yggr/safe_container/multi_index/safe_random_access_index_gernel_impl.hpp>

#include <boost/multi_index_container.hpp>

#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/fusion/include/pair.hpp>

#include <cassert>

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

template
<
	int N,
	typename Multi_Index_Container,
	typename Mutex
>
class safe_random_access_index
{
public:
	typedef Multi_Index_Container final_type;

	typedef typename 
		boost::multi_index::nth_index
		<
			final_type,
			N
		>::type base_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:

	BOOST_MPL_ASSERT((boost::mpl::bool_< (N >= 0) >));

public:

	typedef typename base_type::value_type						value_type;
	typedef typename base_type::ctor_args						ctor_args;
	typedef typename base_type::allocator_type					allocator_type;
	typedef typename base_type::pointer							pointer;
	typedef typename base_type::const_pointer					const_pointer;
	typedef typename base_type::reference						reference;
	typedef typename base_type::const_reference					const_reference;
	typedef typename base_type::size_type						size_type;
	typedef typename base_type::difference_type					difference_type;

	typedef typename base_type::iterator						iterator;
	typedef typename base_type::const_iterator					const_iterator;
	typedef typename base_type::reverse_iterator				reverse_iterator;
	typedef typename base_type::const_reverse_iterator			const_reverse_iterator;
	typedef typename base_type::tag_list						tag_list;

	typedef boost::mpl::int_<N> nth_index_id_type;
	typedef boost::mpl::pair<nth_index_id_type, tag_list> index_id_type; 

	typedef safe_container_error::error_type error_type;

protected:
	typedef yggr::container::node_handle_traits<base_type> node_handle_traits_type;

public:
	typedef typename node_handle_traits_type::node_type				node_type;
	typedef typename node_handle_traits_type::insert_return_type	base_insert_return_type;
	typedef detail::safe_insert_return_type_base<node_type>			insert_return_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef safe_random_access_index this_type;

private:
	// disable default constructor
	safe_random_access_index(void);

public:
	//safe_random_access_index(mutex_type& mutex, base_type& base);
	//safe_random_access_index(const this_type& right);

	YGGR_PP_SAFE_MIDX_INDEXES_CONSTRUCTOR_IMPL(safe_random_access_index)

	//~safe_random_access_index(void);
	YGGR_PP_SAFE_MIDX_INDEXES_DESTRUCTOR_IMPL(safe_random_access_index)

public:
	//// operator=
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(final_type) right);
	//this_type& operator=(const final_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

	//this_type& operator=(BOOST_RV_REF(safe_multi_index_containe_this) right);
	//this_type& operator=(const safe_multi_index_containe_this& right);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//this_type& operator=(std::initializer_list<value_type> list);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_OPERATOR_SET_IMPL(safe_random_access_index)

public:
	//inline void swap(BOOST_RV_REF(final_type) right);
	//void swap(final_type& right);

	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	//void swap(BOOST_RV_REF(safe_multi_index_containe_this) right);
	//void swap(safe_multi_index_containe_this& right);

	YGGR_PP_SAFE_MIDX_INDEXES_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_SWAP_IMPL(safe_random_access_index)

public:
	//operator final_type(void) const;

	YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_FINAL_IMPL()

	//// iterators:
	//template<typename ValueHandler, typename Handler> 
	//typename Handler::result_type 
	//	iterator_to(const ValueHandler& val_handler, const Handler& handler);
	
	//template<typename ValueHandler, typename Handler> 
	//typename Handler::result_type 
	//	iterator_to(const ValueHandler& val_handler, const Handler& handler) const;

	YGGR_PP_SAFE_MIDX_INDEXES_ITERATOR_TO_IMPL()

	//// assign
	//void assign(size_type n, const value_type& val);

	//template<typename InputIter>
	//void assign(InputIter s, InputIter e);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//void assign(std::initializer_list<value_type> l);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	
	YGGR_PP_SAFE_MIDX_INDEXES_ASSIGN_IMPL()

	// size and capacity:

	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;

	//void resize(size_type n);
	//void resize(size_type n, const value_type& x);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_CAPACITY_IMPL()

	//size_type capacity(void) const;
	//bool reserve(size_type size);
	//void shrink_to_fit(void);

	YGGR_PP_SAFE_MIDX_RND_INDEXES_CAPACITY_IMPL();

	// access

	//value_type front(void) const;
	//bool get_front(value_type& val) const;

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_FRONT_IMPL()

	//value_type back(void) const;
	//bool get_back(value_type& val) const;

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_BACK_IMPL()

	//value_type operator[](size_type idx) const;

	//value_type at(size_type idx) const;
	//bool get_at(size_type idx, value_type& val) const;

	//bool get_value(size_type idx, value_type& val) const;

	YGGR_PP_SAFE_MIDX_RND_INDEXES_AT_IMPL()

	//modifiers:

	//bool push_front(BOOST_RV_REF(value_type) val);
	//bool push_front(const value_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	push_front(BOOST_RV_REF(value_type) val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	push_front(const value_type& val, const Handler& handler);

	YGGR_PP_SAFE_MIDX_INDEXES_PUSH_FRONT_IMPL()

	//bool pop_front(void);
	//bool pop_front(value_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	pop_front(const Handler& handler);
	
	//template<typename Handler>
	//typename Handler::result_type
	//	pop_front(value_type& val, const Handler& handler);

	YGGR_PP_SAFE_MIDX_INDEXES_POP_FRONT_IMPL()

	//bool push_back(BOOST_RV_REF(value_type) val);
	//bool push_back(const value_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	push_back(BOOST_RV_REF(value_type) val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	push_back(const value_type& val, const Handler& handler);

	YGGR_PP_SAFE_MIDX_INDEXES_PUSH_BACK_IMPL()

	//bool pop_back(void);
	//bool pop_back(value_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	pop_back(const Handler& handler);
	
	//template<typename Handler>
	//typename Handler::result_type
	//	pop_back(value_type& val, const Handler& handler);

	YGGR_PP_SAFE_MIDX_INDEXES_POP_BACK_IMPL()

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, const value_type& val);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result
	//	insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val, const Handler& handler);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result
	//	insert(const IterHandler& iter_handler, const value_type& val, const Handler& handler);

	//template<typename IterHandler> 
	//bool insert(const IterHandler& iter_handler, size_type size, const value_type& val);

	//template<typename IterHandler, typename InputIter> 
	//bool insert(const IterHandler& iter_handler, InputIter first, InputIter last);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, std::initializer_list<value_type> il);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//template<typename IterHandler>
	//insert_return_type insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh);

	//template<typename IterHandler, typename Handler>
	//typename ::yggr::func::foo_t_info<Handler>::result_type
	//	insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh, const Handler& handler);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_IMPL()

	//template<typename IterHandler>
	//node_type extract(const IterHandler& iter_handler);

	YGGR_PP_SAFE_MIDX_INDEXES_LINEAR_EXTRACT_FUNCTION_IMPL()

	//// erase(iterator) or  erase(std::pair<iterator, iterator>)
	//template<typename IterHandler>
	//bool erase(const IterHandler&);

	//// erase(iterator, iterator)
	//template<typename EHandler, typename ZHandler>
	//bool erase(const EHandler& eins_handler, const ZHandler& zwei_iter_handler);

	//// handler(erase(std::pair<iterator, iterator>))
	//template<typename EHandler, typename ZHandler>
	//typename Handler::result_type erase(const EHandler& iter_handler, const Handler& handler);

	//// handler(erase(iterator, iterator))
	//template<typename EIterHandler, typename ZIterHandler, typename Handler>
	//typename Handler::result_type erase(const EIterHandler&, const ZIterHandler&, const Handler&)
	
	YGGR_PP_SAFE_MIDX_INDEXES_ERASE_FUNCTION_IMPL()

	//void clear(void);

	YGGR_PP_SAFE_MIDX_INDEXES_CLEAR_IMPL()

	// replace
	//template<typename IterHandler>
	//bool replace(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val);

	//template<typename IterHandler>
	//bool replace(const IterHandler& iter_handler, const value_type& val);

	YGGR_PP_SAFE_MIDX_INDEXES_REPLACE_IMPL()

	// modify
	//template<typename IterHandler, typename Modifier> 
	//bool modify(const IterHandler& iter_handler, Modifier mod);

	//template<typename IterHandler, typename Modifier, typename Rollback>
	//bool modify(const IterHandler& iter_handler, Modifier mod, Rollback back);

	YGGR_PP_SAFE_MIDX_INDEXES_MODIFY_IMPL()


	//template <typename IterHandler, typename ...Args> 
	//bool emplace(const IterHandler& iter_handler, tag_non_handler, Args&&... args);
	//	
	//template <typename IterHandler, typename Handler, typename ...Args> 
	//typename Handler::result_type 
	//	emplace(const IterHandler& iter_handler, 
	//			const Handler& handler, Args&&... args);

	//template<typename ...Args> 
	//bool emplace_front(tag_non_handler, Args&&... args);
	//	
	//template<typename Handler, typename ...Args> 
	//typename Handler::result_type
	//	emplace_front(const Handler& handler, Args&&... args);

	//template<typename ...Args> 
	//bool emplace_back(tag_non_handler, Args&&... args);
	//	
	//template<typename Handler, typename ...Args> 
	//typename Handler::result_type
	//	emplace_back(const Handler& handler, Args&&... args);

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
	
	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_IMPL( _ )
	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_FRONT_IMPL( _ )
	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_BACK_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_IMPL( __n__ ) \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_FRONT_IMPL( __n__ ) \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_BACK_IMPL( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// list operations:

	//// splice
	//void splice(BOOST_RV_REF(base_type) right);
	//void splice(base_type& right);
	
	//void splice(BOOST_RV_REF(base_type) right, iterator i);
	//void splice(base_type& right, iterator i); 
	
	//void splice(BOOST_RV_REF(base_type) right, iterator first, iterator last);
	//void splice(base_type& right, iterator first, iterator last);
	
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, BOOST_RV_REF(base_type) right);
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, base_type& right);
	
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, 
	//				BOOST_RV_REF(base_type) right, iterator i);
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, base_type& right, 
	//				iterator i);
	
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, 
	//				BOOST_RV_REF(base_type) right, iterator first, iterator last);
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, 
	//				base_type& right, iterator first, iterator last);
	//	
	//void splice(BOOST_RV_REF(this_type) right);
	//void splice(this_type& right);

	//template<typename SrcEIterHandler> 
	//void splice(BOOST_RV_REF(this_type) right, const SrcEIterHandler& src_eins_iter_handler);
	//template<typename SrcEIterHandler> 
	//void splice(this_type& right, const SrcEIterHandler& src_eins_iter_handler);
	
	//template<typename SrcEIterHandler, typename SrcZIterHandler> 
	//void splice(BOOST_RV_REF(this_type) right, 
	//			const SrcEIterHandler& src_eins_iter_handler,
	//			const SrcZIterHandler& src_zwei_iter_handler );	
	//template<typename SrcEIterHandler, typename SrcZIterHandler> 
	//void splice(this_type& right, 
	//			const SrcEIterHandler& src_eins_iter_handler,
	//			const SrcZIterHandler& src_zwei_iter_handler );

	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, BOOST_RV_REF(this_type) right);
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, this_type& right);
		
	//template<typename DstIterHandler, typename SrcEIterHandler> 
	//void splice(const DstIterHandler& dst_iter_handler, 
	//				BOOST_RV_REF(this_type) right, const SrcEIterHandler& src_eins_iter_handler);
	//template<typename DstIterHandler, typename SrcEIterHandler> 
	//void splice(const DstIterHandler& dst_iter_handler, 
	//				this_type& right, const SrcEIterHandler& src_eins_iter_handler);
		
	//template<typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> 
	//void splice(const DstIterHandler& dst_iter_handler, 
	//				BOOST_RV_REF(this_type) right, 
	//				const SrcEIterHandler& src_eins_iter_handler, 
	//				const SrcZIterHandler& src_zwei_iter_handler);
	//template<typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> 
	//void splice(const DstIterHandler& dst_iter_handler, 
	//				this_type& right, 
	//				const SrcEIterHandler& src_eins_iter_handler, 
	//				const SrcZIterHandler& src_zwei_iter_handler);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_SPLICE_IMPL()

	//void remove(const value_type& val);

	//template<typename Predicate> 
	//void remove_if(Predicate pred);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_REMOVE_IMPL()

	//void unique(void);
	
	//template<typename Cmp>
	//void unique(Cmp cmp);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_UNIQUE_IMPL()

	//void merge(BOOST_RV_REF(base_type) right, 
	//				bool need_sort_self = true, 
	//				bool need_sort_other = true);
	//void merge(base_type& right, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);

	//template<typename Pred> 
	//void merge(BOOST_RV_REF(base_type) right, Pred comp, 
	//			bool need_sort_self = true, bool need_sort_other = true);
	//template<typename Pred> 
	//void merge(base_type& right, Pred comp, 
	//			bool need_sort_self = true, bool need_sort_other = true);

	//void merge(BOOST_RV_REF(this_type) right, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);
	//void merge(this_type& right, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);

	//template<typename Pred> 
	//void merge(BOOST_RV_REF(this_type) right, Pred comp, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);
	//template<typename Pred> 
	//void merge(this_type& right, Pred comp, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_MERGE_IMPL()

	//void sort(void);

	//template<typename Cmp> 
	//void sort(Cmp cmp);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_SORT_IMPL()

	//void reverse(void);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_REVERSE_IMPL()

	//template<typename PosIterHandler, typename EIterHandler>
	//void relocate(const PosIterHandler& pos_iter_handler, 
	//				const EIterHandler& eins_iter_handler);

	//template<typename PosIterHandler, typename EIterHandler, typename ZIterHandler>
	//void relocate(const PosIterHandler& pos_iter_handler, 
	//				const EIterHandler& eins_iter_handler,
	//				const ZIterHandler& zwei_iter_handler);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_RELOCATE_IMPL()

	//template<typename IterHandler>
	//void rearrange(const IterHandler& iter_handler);

	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_REARRANGE_IMPL()

	// allocator
	//allocator_type get_allocator(void) const;

	YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL()

	// safe_other

	////is_exists
	//bool is_exists(const value_type& val) const;

	//template<typename Pred>
	//bool is_exists(const Pred& pred) const;

	YGGR_PP_SAFE_MIDX_LINER_INDEXES_IS_EXISTS_IMPL()

	//base_type& copy_to_base(base_type& out) const;
	//final_type& copy_to_base(final_type& out) const;

	YGGR_PP_SAFE_MIDX_INDEXES_COPY_TO_BASE_IMPL()

	//final_type load(void) const;

	YGGR_PP_SAFE_MIDX_INDEXES_LOAD_IMPL()

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	//void store(BOOST_RV_REF(final_type) final);
	//void store(const final_type& final);

	YGGR_PP_SAFE_MIDX_INDEXES_STONE_IMPL()

	//value_type begin_value(void) const;
	//bool get_begin_value(value_type& val) const;

	YGGR_PP_SAFE_MIDX_SEQ_BEGIN_VALUE_FUNCTION_IMPL()

	//value_type rbegin_value(void) const;
	//bool get_rbegin_value(value_type& val) const;

	YGGR_PP_SAFE_MIDX_SEQ_RBEGIN_VALUE_FUNCTION_IMPL()

	// use handler:
	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler);

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HANDLER_IMPL()

	// using_hook_handler
	//template<typename Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler(const Handler& handler);
	//
	//template<typename Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler) const;
	//
	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler(const Handler& handler, 
	//						const Return_Handler& ret_handler);
	//
	//template<typename Handler, typename Return_Handler> inline 
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler,
	//						const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HOOK_HANDLER_IMPL()

	//template<typename Handler>
	//typename Handler::result_type using_handler_final(const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type using_handler_final(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler_final(const Handler& handler, const Return_Handler& ret_handler);

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler_final(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_MIDX_INDEXES_USING_HANDLER_FINAL_IMPL()

	// using_hook_handler_final
	//template<typename Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler_final(const Handler& handler);
	//
	//template<typename Handler> inline 
	//typename Handler::result_type 
	//	using_hook_handler_final(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler_final(const Handler& handler, const Return_Handler& ret_handler);
	//
	//template<typename Handler, typename Return_Handler> inline 
	//typename Handler::result_type 
	//	using_hook_handler_final(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_MIDX_INDEXES_USING_HOOK_HANDLER_FINAL_IMPL()

	// for_each_iterator
	//template<typename Handler>
	//void for_each_iterator(const Handler& handler);

	//template<typename Handler>
	//void for_each_iterator(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator(const Handler& handler, 
	//							const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator(const Handler& handler, 
	//							const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_IMPL()

	//template<typename Handler>
	//void for_each_iterator_reverse(const Handler& handler);

	//template<typename Handler>
	//void for_each_iterator_reverse(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator_reverse(const Handler& handler,
	//									const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator_reverse(const Handler& handler,
	//									const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_REVERSE_IMPL()

	// for_each
	//template<typename Handler>
	//void for_each(const Handler& handler);

	//template<typename Handler>
	//void for_each(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each(const Handler& handler, 
	//				const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each(const Handler& handler, 
	//				const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_IMPL()

	//template<typename Handler>
	//void for_each_reverse(const Handler& handler);

	//template<typename Handler>
	//void for_each_reverse(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_reverse(const Handler& handler,
	//							const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_reverse(const Handler& handler,
	//						const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_REVERSE_IMPL()


private:
	/*mutable*/ mutex_type& _mutex;
	final_type& _final;
	base_type& _base;
};

YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FULL(safe_random_access_index)

YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FULL(safe_random_access_index)

YGGR_PP_SAFE_MIDX_INDEXES_RANGE_SIZE_SUPPORT(safe_random_access_index)

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

YGGR_PP_SAFE_MIDX_INDEXES_RANGE_SIZE_CHECK_PASS_SUPPORT(yggr::safe_container::multi_index_detail::safe_random_access_index)


//--------------------------------------------------support swap-------------------------------------
namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
namespace swap_support
{
	YGGR_PP_SAFE_MIDX_INDEXES_SWAP(safe_random_access_index)

	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_SWAP(safe_random_access_index)

} // namespace swap_support

using swap_support::swap;

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	using ::yggr::safe_container::multi_index_detail::swap_support::swap;
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	using ::yggr::safe_container::multi_index_detail::swap_support::swap;
} // namespace boost

#endif // __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_ORG_HPP__
