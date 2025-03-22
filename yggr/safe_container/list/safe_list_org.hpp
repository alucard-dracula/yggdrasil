//safe_list_org.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_LIST_SAFE_LIST_ORG_HPP__
#define __YGGR_SAFE_CONTAINER_LIST_SAFE_LIST_ORG_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__
#	error "please include<yggr/safe_container/safe_list.hpp>"
#endif // __YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <yggr/tuple/tuple.hpp>
#include <yggr/move/move.hpp>

#include <yggr/ppex/typedef.hpp>

#include <yggr/container/list.hpp>
#include <yggr/container/iterator_cast.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>
#include <yggr/safe_container/detail/comparer_def.hpp>

#include <yggr/safe_container/detail/safe_element_reference.hpp>
#include <yggr/safe_container/detail/front_back_at_impl.hpp>
#include <yggr/safe_container/detail/begin_rbegin_value_impl.hpp>
#include <yggr/safe_container/detail/assign_impl.hpp>
#include <yggr/safe_container/detail/operator_set_impl.hpp>
#include <yggr/safe_container/detail/safe_gernel_impl.hpp>
#include <yggr/safe_container/detail/swap_impl.hpp>
#include <yggr/safe_container/detail/push_pop_impl.hpp>
#include <yggr/safe_container/detail/safe_other_impl.hpp>
#include <yggr/safe_container/detail/erase_impl.hpp>
#include <yggr/safe_container/detail/insert_impl.hpp>
#include <yggr/safe_container/detail/emplace_impl.hpp>
#include <yggr/safe_container/detail/using_handler_impl.hpp>
#include <yggr/safe_container/detail/for_each_impl.hpp>
#include <yggr/safe_container/detail/range_size_def.hpp>
#include <yggr/safe_container/safe_container_error.hpp>
#include <yggr/safe_container/list/safe_list_member_impl.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/range/functions.hpp>
#include <boost/function.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <iterator>
#include <algorithm>
#include <utility>

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! All safe containers can not guarantee the creation and destruction of multi-threaded safe,		!!!
!!! in different threads to create and destroy the safe container, please use swap to deal with,	!!!
!!! and																								!!!
!!!  it is forbidden to pass any "iterator reference pointer" inside the safe_container				!!!
!!!	 through the callback function to the outside of the safe_container and save or use it!			!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

namespace yggr
{
namespace safe_container
{


template<typename Val, 
			typename Mutex = void,
			typename Ax = typename mplex::typename_expand_get<container::list<Val>, 1>::type,
			template<typename _Val, typename _Ax> class List = container::list
		>
class safe_list
{
public:
	typedef Ax tpl_arg_allocator_type;

public:
	typedef List<Val, Ax> base_type;

public:
	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	typedef safe_container_error::error_type error_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:
	typedef safe_list this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	//safe_list(void);
	//explicit safe_list(const allocator_type& alloc);
	//explicit safe_list(size_type count);

	//safe_list(size_type count, const value_type& val);
	//safe_list(size_type count, const value_type& val, const allocator_type& alloc);

	//template<class InputIterator>
	//safe_list(InputIterator first, InputIterator last);
	//template<class InputIterator>
	//safe_list(InputIterator first, InputIterator last, const allocator_type& alloc);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//safe_list(std::initializer_list<value_type> l);
	//safe_list(std::initializer_list<value_type> l, const allocator_type& alloc);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//safe_list(BOOST_RV_REF(base_type) right);
	//safe_list(const base_type& right);

	//safe_list(BOOST_RV_REF(base_type) right, const allocator_type& alloc);
	//safe_list(const base_type& right, const allocator_type& alloc);

	//safe_list(BOOST_RV_REF(this_type) right);
	//safe_list(const this_type& right);

	//safe_list(BOOST_RV_REF(this_type) right, const allocator_type& alloc);
	//safe_list(const this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_LIST_CONSTRUCTOR_IMPL()

	//safe_list(BOOST_RV_REF(isomeric_same_base_type) right);
	//safe_list(const isomeric_same_base_type& right);

	//safe_list(BOOST_RV_REF(isomeric_same_base_type) right, const allocator_type& alloc);
	//safe_list(const isomeric_same_base_type& right, const allocator_type& alloc);

	//safe_list(BOOST_RV_REF(isomeric_same_this_type) right);
	//safe_list(const isomeric_same_this_type& right);

	//safe_list(BOOST_RV_REF(isomeric_same_this_type) right, const allocator_type& alloc);
	//safe_list(const isomeric_same_this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_LIST_ISOMERIC_SAME_CONSTRUCTOR_IMPL(List)

	//~safe_list(void);
	YGGR_PP_SAFE_LIST_DESTRUCTOR_IMPL()

public:
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//this_type& operator=(std::initializer_list<value_type> il);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_SEQ_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_base_type) right);
	//this_type& operator=(const isomeric_same_base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_SET_IMPL(
		3, List, safe_list, container::is_isomeric_same_list)

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_base_type) right);
	//void swap(isomeric_same_base_type& right);

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP_IMPL(
		3, List, safe_list, container::is_isomeric_same_list)

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

	//operator isomeric_same_base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_BASE_IMPL(
		2, List, container::is_isomeric_same_list)

public:
	// capacity:
	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;
	//void resize(size_type size);

	YGGR_PP_SAFE_LIST_CAPACITY_IMPL()

	// element access:
	// front
	//detail::safe_element_reference<bridge_front> front(void);
	//value_type front(void) const;

	// back
	//detail::safe_element_reference<bridge_back> back(void);
	//value_type back(void) const;

	YGGR_PP_SAFE_LIST_ELEMENT_ACCESS_IMPL()

	// non-try-catch get and set
	//// front
	//bool get_front(value_type& val) const;
	//bool set_front(BOOST_RV_REF(value_type) val);
	//bool set_front(const value_type& val);

	YGGR_PP_SAFE_SEQ_FRONT_IMPL()

	//// back()
	//bool get_back(value_type& val) const;
	//bool set_back(BOOST_RV_REF(value_type) val);
	//bool set_back(const value_type& val);

	YGGR_PP_SAFE_SEQ_BACK_IMPL()

	// modifiers:
	////assign
	//template<typename InputIterator>
	//void assign(InputIterator first, InputIterator last);

	//void assign(size_type size, const value_type& val);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//void assign(std::initializer_list<value_type> list);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_SEQ_ASSIGN_IMPL()

	// push_front
	//bool push_front(BOOST_RV_REF(value_type) val);
	//bool push_front(const value_type& val);

	//template<typename Handler> 
	//typename Handler::result_type 
	//	push_front(BOOST_RV_REF(value_type) val, const Handler& handler);
	
	//template<typename Handler> 
	//typename Handler::result_type 
	//	push_front(const value_type& val, const Handler& handler);

	YGGR_PP_SAFE_SEQ_PUSH_FRONT_IMPL()

	//bool pop_front(void);
	//bool pop_front(value_type& val);

	//template<typename Handler>
	//typename Handler>::result_type
	//	pop_front(const Handler& handler);
	
	//template<typename Handler>
	//typename Handler>::result_type
	//	pop_front(value_type& val, const Handler& handler);

	YGGR_PP_SAFE_SEQ_POP_FRONT_IMPL()

	//bool push_back(BOOST_RV_REF(value_type) val);
	//bool push_back(const value_type& val);

	//template<typename Handler> 
	//typename Handler::result_type
	//	push_back(BOOST_RV_REF(value_type) val, const Handler& handler);
	
	//template<typename Handler> 
	//typename Handler::result_type
	//	push_back(const value_type& val, const Handler& handler);

	YGGR_PP_SAFE_SEQ_PUSH_BACK_IMPL()

	//bool pop_back(void);
	//bool pop_back(value_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	pop_back(const Handler& handler);
	
	//template<typename Handler>
	//typename Handler::result_type
	//	pop_back(value_type& val, const Handler& handler);

	YGGR_PP_SAFE_SEQ_POP_BACK_IMPL()

	// insert
	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, 
	//			BOOST_RV_REF(value_type) val);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, 
	//			const value_type& val);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler, 
	//			BOOST_RV_REF(value_type) val,
	//			const Handler& handler);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler, 
	//			const value_type& val,
	//			const Handler& handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				size_type n,
	//				const value_type& val);

	//template<typename IterHandler, typename InputIter>
	//bool insert(const IterHandler& iter_handler,
	//				InputIter s, InputIter e );

	//template<typename IterHandler, typename InputIter>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//				InputIter s, InputIter e,
	//				const Handler& handler);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, 
	//				std::initializer_list<value_type> list);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//				std::initializer_list<value_type> list,
	//				const Handler& handler );

//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_SEQ_INSERT_FUNCTION_IMPL()

	//// erase
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

	YGGR_PP_SAFE_SEQ_ERASE_FUNCTION_IMPL()

	// clear
	//void clear(void);

	YGGR_PP_SAFE_LIST_CLEAR_IMPL()

	//// emplace
	//template <typename IterHandler, typename ...Args>
	//bool emplace(const IterHandler& iter_handler, non_handler
	//				Args&&... args);

	//template <typename IterHandler, typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace(const IterHandler& iter_handler,
	//				const Handler& handler,
	//				Args&&... args);

	//template<typename ...Args>
	//bool emplace_front(Args&&... args);

	//template<typename ...Args>
	//bool emplace_back(Args&&... args);

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
	
	YGGR_PP_SAFE_SEQ_EMPLACE_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_SEQ_EMPLACE_FRONT_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_SEQ_EMPLACE_BACK_FUNCTION_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_SEQ_EMPLACE_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_SEQ_EMPLACE_FRONT_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_SEQ_EMPLACE_BACK_FUNCTION_IMPL( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// operations:

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

	YGGR_PP_SAFE_LIST_SPLICE_IMPL()

	//void splice(BOOST_RV_REF(isomeric_same_base_type) right);
	//void splice(isomeric_same_base_type& right);
	
	//void splice(BOOST_RV_REF(isomeric_same_base_type) right, iterator i);
	//void splice(isomeric_same_base_type& right, iterator i); 
	
	//void splice(BOOST_RV_REF(isomeric_same_base_type) right, iterator first, iterator last);
	//void splice(isomeric_same_base_type& right, iterator first, iterator last);
	
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, BOOST_RV_REF(isomeric_same_base_type) right);
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, isomeric_same_base_type& right);
	
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, 
	//				BOOST_RV_REF(isomeric_same_base_type) right, iterator i);
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, isomeric_same_base_type& right, 
	//				iterator i);
	
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, 
	//				BOOST_RV_REF(isomeric_same_base_type) right, iterator first, iterator last);
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, 
	//				isomeric_same_base_type& right, iterator first, iterator last);
	//	
	//void splice(BOOST_RV_REF(isomeric_same_this_type) right);
	//void splice(isomeric_same_this_type& right);

	//template<typename SrcEIterHandler> 
	//void splice(BOOST_RV_REF(isomeric_same_this_type) right, const SrcEIterHandler& src_eins_iter_handler);
	//template<typename SrcEIterHandler> 
	//void splice(isomeric_same_this_type& right, const SrcEIterHandler& src_eins_iter_handler);
	
	//template<typename SrcEIterHandler, typename SrcZIterHandler> 
	//void splice(BOOST_RV_REF(isomeric_same_this_type) right, 
	//			const SrcEIterHandler& src_eins_iter_handler,
	//			const SrcZIterHandler& src_zwei_iter_handler );	
	//template<typename SrcEIterHandler, typename SrcZIterHandler> 
	//void splice(isomeric_same_this_type& right, 
	//			const SrcEIterHandler& src_eins_iter_handler,
	//			const SrcZIterHandler& src_zwei_iter_handler );

	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, BOOST_RV_REF(isomeric_same_this_type) right);
	//template<typename IterHandler> 
	//void splice(const IterHandler& iter_handler, isomeric_same_this_type& right);
		
	//template<typename DstIterHandler, typename SrcEIterHandler> 
	//void splice(const DstIterHandler& dst_iter_handler, 
	//				BOOST_RV_REF(isomeric_same_this_type) right, const SrcEIterHandler& src_eins_iter_handler);
	//template<typename DstIterHandler, typename SrcEIterHandler> 
	//void splice(const DstIterHandler& dst_iter_handler, 
	//				isomeric_same_this_type& right, const SrcEIterHandler& src_eins_iter_handler);
		
	//template<typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> 
	//void splice(const DstIterHandler& dst_iter_handler, 
	//				BOOST_RV_REF(isomeric_same_this_type) right, 
	//				const SrcEIterHandler& src_eins_iter_handler, 
	//				const SrcZIterHandler& src_zwei_iter_handler);
	//template<typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> 
	//void splice(const DstIterHandler& dst_iter_handler, 
	//				isomeric_same_this_type& right, 
	//				const SrcEIterHandler& src_eins_iter_handler, 
	//				const SrcZIterHandler& src_zwei_iter_handler);

	YGGR_PP_SAFE_LIST_ISOMERIC_SAME_SPLICE_IMPL(List)

	// remove
	size_type remove(const value_type& val)
	{
		write_lock_type lk(_mutex);
		size_type old_size = _base.size();
		_base.remove(val);
		return old_size - _base.size();
	}

	template<typename Predicate>
	size_type remove_if(Predicate pred)
	{
		write_lock_type lk(_mutex);
		size_type old_size = _base.size();
		_base.remove_if(pred);
		return old_size - _base.size();
	}

	// unique
	size_type unique(void)
	{
		write_lock_type lk(_mutex);
		size_type old_size = _base.size();
		_base.unique();
		return old_size - _base.size();
	}

	template<typename Pred>
	size_type unique(Pred pred)
	{
		write_lock_type lk(_mutex);
		size_type old_size = _base.size();
		_base.unique(pred);
		return old_size - _base.size();
	}

	// warning::
	// boost list supported unordered merge but don't use it,
	// if you want to use unordred merge, please use splite.
	// why using sort before merge in safe_list, because the safe_list in multi_thread,
	// that is, the sorting in a thread is invalid, because the order may be disrupted by another thread

	//void merge(BOOST_RV_REF(base_type) right, 
	//				bool need_sort_self = true, 
	//				bool need_sort_other = true);
	//void merge(base_type& right, 
	//				bool need_sort_self = true, 
	//				bool need_sort_other = true);

	//template<typename Pred> 
	//void merge(BOOST_RV_REF(base_type) right, Pred comp, 
	//				bool need_sort_self = true, bool need_sort_other = true);
	//template<typename Pred> 
	//void merge(base_type& right, Pred comp, 
	//				bool need_sort_self = true, bool need_sort_other = true);

	//void merge(BOOST_RV_REF(this_type) right, 
	//				bool need_sort_self = true, 
	//				bool need_sort_other = true);
	//void merge(this_type& right, 
	//				bool need_sort_self = true, 
	//				bool need_sort_other = true);

	//template<typename Pred> 
	//void merge(BOOST_RV_REF(this_type) right, Pred comp, 
	//				bool need_sort_self = true, 
	//				bool need_sort_other = true);
	//template<typename Pred> 
	//void merge(this_type& right, Pred comp, 
	//				bool need_sort_self = true, 
	//				bool need_sort_other = true);

	YGGR_PP_SAFE_LIST_MERGE_IMPL()

	//void merge(BOOST_RV_REF(isomeric_same_base_type) right, 
	//				bool need_sort_self = true, 
	//				bool need_sort_other = true);
	//void merge(isomeric_same_base_type& right, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);

	//template<typename Pred> 
	//void merge(BOOST_RV_REF(isomeric_same_base_type) right, Pred comp, 
	//			bool need_sort_self = true, bool need_sort_other = true);
	//template<typename Pred> 
	//void merge(isomeric_same_base_type& right, Pred comp, 
	//			bool need_sort_self = true, bool need_sort_other = true);

	//void merge(BOOST_RV_REF(isomeric_same_this_type) right, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);
	//void merge(isomeric_same_this_type& right, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);

	//template<typename Pred> 
	//void merge(BOOST_RV_REF(isomeric_same_this_type) right, Pred comp, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);
	//template<typename Pred> 
	//void merge(isomeric_same_this_type& right, Pred comp, 
	//			bool need_sort_self = true, 
	//			bool need_sort_other = true);
	
	YGGR_PP_SAFE_LIST_ISOMERIC_SAME_MERGE_IMPL(List)

	//void sort(void);

	//template<typename Cmp>
	//void sort(Cmp cmp);

	YGGR_PP_SAFE_LIST_SORT_IMPL()

	//void reverse(void);

	YGGR_PP_SAFE_LIST_REVERSE_IMPL()

	//// allocator:
	//allocator_type get_allocator(void) const;

	YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL()

	// safe other
	//bool is_exists(const value_type& val) const;

	YGGR_PP_SAFE_LIST_SAFE_OTHER_IMPL()

	//base_type& copy_to_base(base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//isomeric_same_base_type& copy_to_base(isomeric_same_base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COPY_TO_BASE_IMPL(
		2, List, container::is_isomeric_same_list)

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	// isomeric_same_base_type load(void) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_LOAD_IMPL(container::is_isomeric_same_list)

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	//void store(BOOST_RV_REF(isomeric_same_base_type) base);
	//void store(const isomeric_same_base_type& base);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_STONE_IMPL(
		2, List, container::is_isomeric_same_list)

	//value_type begin_value(void) const;
	//bool get_begin_value(value_type& val) const;
	//bool set_begin_value(BOOST_RV_REF(value_type) val);
	//bool set_begin_value(const value_type& val);

	YGGR_PP_SAFE_SEQ_BEGIN_VALUE_FUCTION_IMPL()

	//value_type rbegin_value(void) const;
	//bool get_rbegin_value(value_type& val) const;
	//bool set_rbegin_value(BOOST_RV_REF(value_type) val);
	//bool set_rbegin_value(const value_type& val);

	YGGR_PP_SAFE_SEQ_RBEGIN_VALUE_FUCTION_IMPL()

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
	mutable mutex_type _mutex;
	base_type _base;
};

YGGR_PP_SAFE_CONTAINER_COMPARER_FULL(3, safe_list)

YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(3, safe_list, container::is_isomeric_same_list)

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT(3, safe_list)

} // namespace safe_container
} // namespace yggr

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(3, ::yggr::safe_container::safe_list)

//--------------------------------------------------support swap-------------------------------------

namespace yggr
{
namespace safe_container
{
namespace swap_support
{
	YGGR_PP_SAFE_CONTAINER_SWAP(3, safe_list)

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP(
		3, safe_list, container::is_isomeric_same_list)

} // namespace swap_support

using swap_support::swap;

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace boost

#endif //__YGGR_SAFE_CONTAINER_LIST_SAFE_LIST_ORG_HPP__
