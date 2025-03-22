//safe_multi_index_container.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_MULTI_INDEX_CONTAINER_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_MULTI_INDEX_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <yggr/move/move.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/safe_container/multi_index/safe_multi_index_container_decl.hpp>

#include <yggr/safe_container/detail/swap_def.hpp>
#include <yggr/safe_container/detail/comparer_def.hpp>

#include <yggr/safe_container/detail/operator_set_impl.hpp>
#include <yggr/safe_container/detail/using_handler_impl.hpp>
#include <yggr/safe_container/detail/swap_impl.hpp>
#include <yggr/safe_container/detail/safe_other_impl.hpp>
#include <yggr/safe_container/detail/safe_gernel_impl.hpp>
#include <yggr/safe_container/detail/range_size_def.hpp>

#include <yggr/safe_container/safe_container_error.hpp>

#include <boost/fusion/include/pair.hpp>
#include <boost/thread/mutex.hpp>

#include <iterator>
#include <algorithm>
#include <utility>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

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

template<	typename Val,
			typename IndexSpecifierList,
			typename Mutex,
			typename Alloc,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc> class Multi_Index_Container
		>
class safe_multi_index_container
	: public
		multi_index_detail::safe_index_maker
		<
			0,
			Multi_Index_Container<Val, IndexSpecifierList, Alloc>,
			Mutex,
			typename
				boost::multi_index::nth_index
				<
					Multi_Index_Container<Val, IndexSpecifierList, Alloc>,
					0
				>::type
		>::type,
		protected
			multi_index_detail::inheritance_list_maker
			<
				Multi_Index_Container<Val, IndexSpecifierList, Alloc>,
				Mutex
			>::type

{
public:
	typedef Multi_Index_Container<Val, IndexSpecifierList, Alloc>	base_type;

	typedef typename base_type::ctor_args_list						ctor_args_list;
	typedef typename base_type::index_specifier_type_list			index_specifier_type_list;
	typedef typename base_type::index_type_list						index_type_list;

	typedef typename base_type::iterator_type_list					iterator_type_list;
	typedef typename base_type::const_iterator_type_list			const_iterator_type_list;

	typedef typename base_type::allocator_type						allocator_type;

	typedef typename base_type::value_type							value_type;
	typedef typename base_type::iterator							iterator;
	typedef typename base_type::const_iterator						const_iterator;
	typedef typename base_type::node_type							node_type;

	typedef safe_container_error::error_type error_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

public:
	typedef typename
		multi_index_detail::safe_index_maker
		<
			0,
			base_type,
			Mutex,
			typename boost::multi_index::nth_index< base_type, 0 >::type
		>::type first_index_type;

protected:
	typedef typename
		multi_index_detail::inheritance_list_maker
		<
			base_type,
			Mutex
		>::type tail_indexes_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:
	typedef safe_multi_index_container this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// index
	template<int N>
	struct nth_index
		: public
			multi_index_detail::safe_index_maker
			<
				N,
				base_type,
				Mutex,
				typename boost::multi_index::nth_index< base_type, N >::type
			>
	{
	};

	template<typename Tag>
	struct index
		: public
			multi_index_detail::safe_index_maker
			<
				multi_index_detail::get_N
				<
					index_type_list,
					Tag
				>::type::value,
				base_type,
				Mutex,
				typename boost::multi_index::index< base_type, Tag >::type
			>
	{
	};

	// index iterator
	template<int N>
	struct nth_index_iterator
		: public boost::multi_index::nth_index_iterator<base_type, N>
	{
	};

	template<int N>
	struct nth_index_const_iterator
		: public boost::multi_index::nth_index_const_iterator<base_type, N>
	{
	};

	template<typename Tag>
	struct index_iterator
		: public boost::multi_index::index_iterator<base_type, Tag>
	{
	};

	template<typename Tag>
	struct index_const_iterator
		: public boost::multi_index::index_const_iterator<base_type, Tag>
	{
	};

public:
	safe_multi_index_container(void)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base))
	{
	}

	explicit safe_multi_index_container(const ctor_args_list& args_list)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(args_list, allocator_type())
	{
	}

	explicit safe_multi_index_container(const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(alloc)
	{
	}

	safe_multi_index_container(const ctor_args_list& args_list, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(args_list, alloc)
	{
	}

	template<typename InputIterator>
	safe_multi_index_container(InputIterator first, InputIterator last)
		: first_index_type(_mutex, _base),
            tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(first, last)
	{
	}

	template<typename InputIterator>
	safe_multi_index_container(InputIterator first, InputIterator last, const ctor_args_list& args_list)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(first, last, args_list)
	{
	}

	template<typename InputIterator>
	safe_multi_index_container(InputIterator first, InputIterator last, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(first, last, ctor_args_list(), alloc)
	{
	}

	template<typename InputIterator>
	safe_multi_index_container(InputIterator first, InputIterator last,
								const ctor_args_list& args_list, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(first, last, args_list, alloc)
	{
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	safe_multi_index_container(std::initializer_list<value_type> list)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(list.begin(), list.end())
	{
	}

	safe_multi_index_container(std::initializer_list<value_type> list, const ctor_args_list& ctor_args)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(list.begin(), list.end(), ctor_args)
	{
	}

	safe_multi_index_container(std::initializer_list<value_type> list, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
            _base(list.begin(), list.end(), ctor_args_list(), alloc)
	{
	}

	safe_multi_index_container(std::initializer_list<value_type> list,
								const ctor_args_list& args_list, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(list.begin(), list.end(), args_list, alloc)
	{
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	safe_multi_index_container(BOOST_RV_REF(base_type) right)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(boost::move(right))
	{
	}

	safe_multi_index_container(const base_type& right)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(right)
	{
	}

	safe_multi_index_container(BOOST_RV_REF(base_type) right, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(alloc)
	{
		this_type::store(boost::move(right));
	}

	safe_multi_index_container(const base_type& right, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(alloc)
	{
		this_type::store(right);
	}

	safe_multi_index_container(BOOST_RV_REF(this_type) right)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(right.get_allocator())
	{
		this_type& right_ref = right;
		right_ref.swap(_base);
	}

	safe_multi_index_container(const this_type& right)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(right.get_allocator())
	{
		right.copy_to_base(_base);
	}

	safe_multi_index_container(BOOST_RV_REF(this_type) right, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(alloc)
	{
		this_type& right_ref = right;
		right_ref.swap(_base);
	}

	safe_multi_index_container(const this_type& right, const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(alloc)
	{
		right.copy_to_base(_base);
	}

	// isomeric_same
	template<typename V2, typename I2, typename M2, typename A2>
	safe_multi_index_container(BOOST_RV_REF_BEG
									safe_multi_index_container<V2, I2, M2, A2>
								BOOST_RV_REF_END right)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(right.get_allocator())
	{
		typedef safe_multi_index_container<V2, I2, M2, A2> right_type;
		right_type& right_ref = right;
		right_ref.swap(_base);
	}

	template<typename V2, typename I2, typename M2, typename A2>
	safe_multi_index_container(const safe_multi_index_container<V2, I2, M2, A2>& right,
								typename
									boost::enable_if
									<
										boost::is_same
										<
											base_type,
											typename safe_multi_index_container<V2, I2, M2, A2>::base_type
										>,
										mplex::sfinae_type
									>::type sfinae = 0)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(right.get_allocator())
	{
		right.copy_to_base(_base);
	}

	template<typename V2, typename I2, typename M2, typename A2>
	safe_multi_index_container(BOOST_RV_REF_BEG
									safe_multi_index_container<V2, I2, M2, A2>
								BOOST_RV_REF_END right,
								const allocator_type& alloc,
								typename
									boost::enable_if
									<
										boost::is_same
										<
											base_type,
											typename safe_multi_index_container<V2, I2, M2, A2>::base_type
										>,
										mplex::sfinae_type
									>::type sfinae = 0)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(alloc)
	{
		typedef safe_multi_index_container<V2, I2, M2, A2> right_type;
		right_type& right_ref = right;
		right_ref.swap(_base);
	}

	template<typename V2, typename I2, typename M2, typename A2>
	safe_multi_index_container(const safe_multi_index_container<V2, I2, M2, A2>& right,
								const allocator_type& alloc,
								typename
									boost::enable_if
									<
										boost::is_same
										<
											base_type,
											typename safe_multi_index_container<V2, I2, M2, A2>::base_type
										>,
										mplex::sfinae_type
									>::type sfinae = 0)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(alloc)
	{
		right.copy_to_base(_base);
	}

	template<template<int _N, typename _Midx, typename _Mutex> class Safe_Index,
				int N, typename MIC, typename M2>
	safe_multi_index_container(BOOST_RV_REF_BEG Safe_Index<N, MIC, M2> BOOST_RV_REF_END right)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(right.get_allocator())
	{
		this_type::get<N>() = boost::move(right);
	}

	template<template<int _N, typename _Midx, typename _Mutex> class Safe_Index,
				int N, typename MIC, typename M2>
	safe_multi_index_container(const Safe_Index<N, MIC, M2>& right)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(right.get_allocator())
	{
		this_type::get<N>() = right;
	}

	template<template<int _N, typename _Midx, typename _Mutex> class Safe_Index,
				int N, typename MIC, typename M2>
	safe_multi_index_container(BOOST_RV_REF_BEG Safe_Index<N, MIC, M2> BOOST_RV_REF_END right,
								const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			_base(alloc)
	{
		this_type::get<N>() = boost::move(right);
	}

	template<template<int _N, typename _Midx, typename _Mutex> class Safe_Index,
				int N, typename MIC, typename M2>
	safe_multi_index_container(const Safe_Index<N, MIC, M2>& right,
								const allocator_type& alloc)
		: first_index_type(_mutex, _base),
			tail_indexes_type(
				multi_index_detail::make_inheritance_list<Mutex>(_mutex, _base)),
			 _base(alloc)
	{
		this_type::get<N>() = right;
	}

	~safe_multi_index_container(void)
	{
	}

public:
	// the operator= need return this_type&
	inline this_type& operator=(BOOST_RV_REF(typename first_index_type::base_type) right)
	{
		first_index_type::operator=(boost::move(right));
		return *this;
	}

	this_type& operator=(const typename first_index_type::base_type& right)
	{
		first_index_type::operator=(right);
		return *this;
	}

	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

	YGGR_PP_SAFE_CONTAINER_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_MIDX_ISOMERIC_SAME_OPERATOR_SET_IMPL(4, Multi_Index_Container, safe_multi_index_container)

	// safe_index operator=
	template<template<int _N, typename _Midx, typename _Mutex> class Safe_Index,
				int N, typename MIC, typename M2>
	this_type& operator=(BOOST_RV_REF_BEG Safe_Index< N, MIC, M2 > BOOST_RV_REF_END right)
	{
		this_type::get<N>() = boost::move(right);
		return *this;
	}

	template<template<int _N, typename _Midx, typename _Mutex> class Safe_Index,
				int N, typename MIC, typename M2>
	this_type& operator=(const Safe_Index<N, MIC, M2>& right)
	{
		this_type::get<N>() = right;
		return *this;
	}


#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type tmp(il.begin(), il.end(),
						ctor_args_list(),
						this_type::get_allocator());
		this_type::swap(tmp);
		return *this;
	}
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST


public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	using first_index_type::swap;


	template<template<int _N, typename _Midx, typename _Mutex> class Safe_Index,
				int N, typename MIC, typename M>
	typename boost::enable_if_c<(N > 0), void>::type
		swap(BOOST_RV_REF_BEG Safe_Index< N, MIC, M > BOOST_RV_REF_END right)
	{
		this_type::get<N>().swap(boost::move(right));
	}

	template<template<int _N, typename _Midx, typename _Mutex> class Safe_Index,
				int N, typename MIC, typename M>
	typename boost::enable_if_c<(N > 0), void>::type
		swap(Safe_Index<N, MIC, M>& right)
	{
		this_type::get<N>().swap(right);
	}

public:

	//operator base_type(void) const;
	//YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

	// get
	// get<N>
	template<int N> inline
	typename
		boost::enable_if_c
		<
			(N > 0),
			typename nth_index<N>::type&
		>::type
		get(void)
	{
		tail_indexes_type& indexes = *this;
		return ::yggr::get<N - 1>(indexes);
	}

	template<int N> inline
	typename
		boost::disable_if_c
		<
			(N > 0),
			typename nth_index<N>::type&
		>::type
		get(void)
	{
		first_index_type& index = *this;
		return index;
	}

	template<int N> inline
	typename
		boost::enable_if_c
		<
			(N > 0),
			const typename nth_index<N>::type&
		>::type
		get(void) const
	{
		const tail_indexes_type& indexes = *this;
		return ::yggr::get<N - 1>(indexes);
	}

	template<int N> inline
	typename
		boost::disable_if_c
		<
			(N > 0),
			const typename nth_index<N>::type&
		>::type
		get(void) const
	{
		const first_index_type& index = *this;
		return index;
	}

	// get<Tag>

	template<typename Tag> inline
	typename index<Tag>::type& get(void)
	{
		return
			this_type::get
			<
				multi_index_detail::get_N
				<
					index_type_list,
					Tag
				>::type::value
			>();
	}

	template<typename Tag> inline
	const typename index<Tag>::type& get(void) const
	{
		return
			this_type::get
			<
				multi_index_detail::get_N
				<
					index_type_list,
					Tag
				>::type::value
			>();
	}

	// project
	// project<N>
	template<int N, typename IterHandler, typename Handler>
	typename func::foo_t_info<Handler>::result_type
		project(const IterHandler& iter_handler, const Handler& handler)
	{
		typedef typename nth_index<N>::type now_cont_type;
		typedef typename now_cont_type::iterator now_iterator_type;
		typedef typename now_cont_type::index_id_type index_id_type;

		write_lock_type lk(_mutex);
		return handler(_base,
						boost::fusion::pair
						<
							index_id_type,
							now_iterator_type
						>(_base.template project<N>(iter_handler(_base))));
	}

	template<int N, typename IterHandler, typename Handler>
	typename func::foo_t_info<Handler>::result_type
		project(const IterHandler& iter_handler, const Handler& handler) const
	{
		typedef typename nth_index<N>::type now_cont_type;
		typedef typename now_cont_type::const_iterator now_iterator_type;
		typedef typename now_cont_type::index_id_type index_id_type;

		read_lock_type lk(_mutex);
		return handler(_base,
						boost::fusion::pair
						<
							index_id_type,
							now_iterator_type
						>(_base.template project<N>(iter_handler(_base))));
	}

	// project<Tag>
	template<typename Tag, typename IterHandler, typename Handler>
	typename func::foo_t_info<Handler>::result_type
		project(const IterHandler& iter_handler, const Handler& handler)
	{
		typedef typename index<Tag>::type now_cont_type;
		typedef typename now_cont_type::iterator now_iterator_type;
		typedef typename now_cont_type::index_id_type index_id_type;

		write_lock_type lk(_mutex);
		return handler(_base,
						boost::fusion::pair
						<
							index_id_type,
							now_iterator_type
						>(_base.template project<Tag>(iter_handler(_base))));
	}

	template<typename Tag, typename IterHandler, typename Handler>
	typename func::foo_t_info<Handler>::result_type
		project(const IterHandler& iter_handler, const Handler& handler) const
	{
		typedef typename index<Tag>::type now_cont_type;
		typedef typename now_cont_type::const_iterator now_iterator_type;
		typedef typename now_cont_type::index_id_type index_id_type;

		write_lock_type lk(_mutex);
		return handler(_base,
						boost::fusion::pair
						<
							index_id_type,
							now_iterator_type
						>(_base.template project<Tag>(iter_handler(_base))));
	}

	// allocator:
	//allocator_type get_allocator(void) const;

	YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL()

	// safe_other:
	//base_type& copy_to_base(base_type& out) const;

	//YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//base_type load(void) const;

	//YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	//YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	// use handler base:
	template<int N, typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler)
	{
		typedef typename this_type::nth_index<N>::type now_index_type;
		now_index_type now_index(this_type::get<N>());
		return now_index.using_handler(handler);
	}

	template<typename Tag, typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler)
	{
		typedef typename this_type::index<Tag>::type now_index_type;
		now_index_type now_index(this_type::get<Tag>());
		return now_index.using_handler(handler);
	}

	template<int N, typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler) const
	{
		return this_type::get<N>().using_handler(handler);
	}

	template<typename Tag, typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler) const
	{
		return this_type::get<Tag>().using_handler(handler);
	}

	template<int N, typename Handler, typename Return_Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler, const Return_Handler& ret_handler)
	{
		typedef typename this_type::nth_index<N>::type now_index_type;
		now_index_type now_index(this_type::get<N>());
		return now_index.using_handler(handler, ret_handler);
	}

	template<typename Tag, typename Handler, typename Return_Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler, const Return_Handler& ret_handler)
	{
		typedef typename this_type::index<Tag>::type now_index_type;
		now_index_type now_index(this_type::get<Tag>());
		return now_index.using_handler(handler, ret_handler);
	}

	template<int N, typename Handler, typename Return_Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler, const Return_Handler& ret_handler) const
	{
		return this_type::get<N>().using_handler(handler, ret_handler);
	}

	template<typename Tag, typename Handler, typename Return_Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler, const Return_Handler& ret_handler) const
	{
		return this_type::get<Tag>().using_handler(handler, ret_handler);
	}

	// use handler this:
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

private:
    mutable mutex_type _mutex;
    base_type _base;
};

YGGR_PP_SAFE_CONTAINER_COMPARER_FULL(4, safe_multi_index_container)

YGGR_PP_SAFE_MIDX_ISOMERIC_SAME_COMPARER_FULL(4, safe_multi_index_container)

#define YGGR_PP_SAFE_MIDX_FIRST_INDEX_COMPARER_FUNCTION( __op__, __symbol__ ) \
	template<typename V, typename I, typename M, typename A, \
				template <typename _V, typename _I, typename _A> class Midx > inline \
	bool __op__( const typename safe_multi_index_container<V, I, M, A, Midx>::first_index_type::base_type& l, \
					const safe_multi_index_container<V, I, M, A, Midx>& r ) { \
		return l YGGR_PP_SYMBOL(__symbol__) r.template get<0>(); } \
	\
	template<typename V, typename I, typename M, typename A, \
				template <typename _V, typename _I, typename _A> class Midx > inline \
	bool __op__( const safe_multi_index_container<V, I, M, A, Midx>& l, \
					const typename safe_multi_index_container<V, I, M, A, Midx>::first_index_type::base_type& r) { \
		return l.template get<0>() YGGR_PP_SYMBOL(__symbol__) r; }

YGGR_PP_SAFE_MIDX_FIRST_INDEX_COMPARER_FUNCTION(operator==, YGGR_PP_SYMBOL_EQUAL_TO)
YGGR_PP_SAFE_MIDX_FIRST_INDEX_COMPARER_FUNCTION(operator!=, YGGR_PP_SYMBOL_NOT_EQUAL_TO)
YGGR_PP_SAFE_MIDX_FIRST_INDEX_COMPARER_FUNCTION(operator<, YGGR_PP_SYMBOL_LESS)
YGGR_PP_SAFE_MIDX_FIRST_INDEX_COMPARER_FUNCTION(operator<=, YGGR_PP_SYMBOL_LESS_EQUAL)
YGGR_PP_SAFE_MIDX_FIRST_INDEX_COMPARER_FUNCTION(operator>, YGGR_PP_SYMBOL_GREATER)
YGGR_PP_SAFE_MIDX_FIRST_INDEX_COMPARER_FUNCTION(operator>=, YGGR_PP_SYMBOL_GREATER_EQUAL)

#undef YGGR_PP_SAFE_MIDX_FIRST_INDEX_COMPARER_FUNCTION

#define YGGR_PP_SAFE_MIDX_SAFE_INDEXES_COMPARER_FUNCTION( __op__, __symbol__ ) \
	template<int N, typename MIC, typename M1, \
				template<int _N, typename _MIC, typename _M> class Safe_Index, \
				typename V, typename I, typename M2, typename A, \
				template <typename _V, typename _I, typename _A> class Midx > inline \
	bool __op__( const Safe_Index<N, MIC, M1>& l, \
					const safe_multi_index_container<V, I, M2, A, Midx>& r ) { \
		return l YGGR_PP_SYMBOL(__symbol__) r.template get<N>(); } \
	\
	template<typename V, typename I, typename M2, typename A, \
				template <typename _V, typename _I, typename _A> class Midx, \
				int N, typename MIC, typename M1, \
				template<int _N, typename _MIC, typename _M> class Safe_Index > inline \
	bool __op__( const safe_multi_index_container<V, I, M2, A, Midx>& l, \
					const Safe_Index<N, MIC, M1>& r) { \
		return l.template get<N>() YGGR_PP_SYMBOL(__symbol__) r; }

YGGR_PP_SAFE_MIDX_SAFE_INDEXES_COMPARER_FUNCTION(operator==, YGGR_PP_SYMBOL_EQUAL_TO)
YGGR_PP_SAFE_MIDX_SAFE_INDEXES_COMPARER_FUNCTION(operator!=, YGGR_PP_SYMBOL_NOT_EQUAL_TO)
YGGR_PP_SAFE_MIDX_SAFE_INDEXES_COMPARER_FUNCTION(operator<, YGGR_PP_SYMBOL_LESS)
YGGR_PP_SAFE_MIDX_SAFE_INDEXES_COMPARER_FUNCTION(operator<=, YGGR_PP_SYMBOL_LESS_EQUAL)
YGGR_PP_SAFE_MIDX_SAFE_INDEXES_COMPARER_FUNCTION(operator>, YGGR_PP_SYMBOL_GREATER)
YGGR_PP_SAFE_MIDX_SAFE_INDEXES_COMPARER_FUNCTION(operator>=, YGGR_PP_SYMBOL_GREATER_EQUAL)

#undef YGGR_PP_SAFE_MIDX_SAFE_INDEXES_COMPARER_FUNCTION

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT(4, safe_multi_index_container)

} // namespace safe_container
} // namesapce yggr

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(4, yggr::safe_container::safe_multi_index_container)

namespace boost
{
namespace multi_index
{

// nth_index
template<typename Val, typename IndexSpecifierList, typename Mutex, typename Allocator,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc>
			class Multi_Index_Container,
			int N>
struct nth_index
		<
			yggr::safe_container::safe_multi_index_container
			<
				Val, IndexSpecifierList, Mutex, Allocator,
				Multi_Index_Container
			>,
			N
		>
{
private:
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			Val, IndexSpecifierList, Mutex, Allocator,
			Multi_Index_Container
		> cont_type;

public:
	typedef typename cont_type::template nth_index<N>::type type;
};

// index
template<typename Val, typename IndexSpecifierList, typename Mutex, typename Allocator,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc>
			class Multi_Index_Container,
			typename Tag>
struct index
		<
			yggr::safe_container::safe_multi_index_container
			<
				Val, IndexSpecifierList, Mutex, Allocator,
				Multi_Index_Container
			>,
			Tag
		>
{
private:
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			Val, IndexSpecifierList, Mutex, Allocator,
			Multi_Index_Container
		> cont_type;

public:
	typedef typename cont_type::template index<Tag>::type type;
};

// nth_index_iterator

template<typename Val, typename IndexSpecifierList, typename Mutex, typename Allocator,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc>
			class Multi_Index_Container,
			int N>
struct nth_index_iterator
		<
			yggr::safe_container::safe_multi_index_container
			<
				Val, IndexSpecifierList, Mutex, Allocator,
				Multi_Index_Container
			>,
			N
		>
{
private:
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			Val, IndexSpecifierList, Mutex, Allocator,
			Multi_Index_Container
		> cont_type;

public:
	typedef typename cont_type::template nth_index_iterator<N>::type type;
};

template<typename Val, typename IndexSpecifierList, typename Mutex, typename Allocator,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc>
			class Multi_Index_Container,
			int N>
struct nth_index_const_iterator
		<
			yggr::safe_container::safe_multi_index_container
			<
				Val, IndexSpecifierList, Mutex, Allocator,
				Multi_Index_Container
			>,
			N
		>
{
private:
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			Val, IndexSpecifierList, Mutex, Allocator,
			Multi_Index_Container
		> cont_type;

public:
	typedef typename cont_type::template nth_index_const_iterator<N>::type type;
};

// index_iterator
template<typename Val, typename IndexSpecifierList, typename Mutex, typename Allocator,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc>
			class Multi_Index_Container,
			typename Tag>
struct index_iterator
		<
			yggr::safe_container::safe_multi_index_container
			<
				Val, IndexSpecifierList, Mutex, Allocator,
				Multi_Index_Container
			>,
			Tag
		>
{
private:
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			Val, IndexSpecifierList, Mutex, Allocator,
			Multi_Index_Container
		> cont_type;

public:
	typedef typename cont_type::template index_iterator<Tag>::type type;
};

template<typename Val, typename IndexSpecifierList, typename Mutex, typename Allocator,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc>
			class Multi_Index_Container,
			typename Tag>
struct index_const_iterator
		<
			yggr::safe_container::safe_multi_index_container
			<
				Val, IndexSpecifierList, Mutex, Allocator,
				Multi_Index_Container
			>,
			Tag
		>
{
private:
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			Val, IndexSpecifierList, Mutex, Allocator,
			Multi_Index_Container
		> cont_type;

public:
	typedef typename cont_type::template index_const_iterator<Tag>::type type;
};

} // namespace multi_index
} // namespace boost


namespace yggr
{
namespace safe_container
{

using boost::multi_index::nth_index;
using boost::multi_index::index;

using boost::multi_index::nth_index_iterator;
using boost::multi_index::nth_index_const_iterator;

using boost::multi_index::index_iterator;
using boost::multi_index::index_const_iterator;

} // nmamespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace swap_support
{
	YGGR_PP_SAFE_CONTAINER_SWAP(4, safe_multi_index_container)


template<typename V, typename I, typename M1, typename A,
			template<typename _V, typename _I, typename _A> class Midx,
			int N, typename MIC, typename M2,
			template<int _N, typename _MIC, typename _M> class Safe_Index > inline
void swap(safe_multi_index_container<V, I, M1, A, Midx>& l, Safe_Index<N, MIC, M2>& r)
{
	l.swap(r);
}

template<int N, typename MIC, typename M1,
			template<int _N, typename _MIC, typename _M> class Safe_Index,
			typename V, typename I, typename M2, typename A,
			template<typename _V, typename _I, typename _A> class Midx> inline
void swap( Safe_Index<N, MIC, M1>& l,
			safe_multi_index_container<V, I, M2, A, Midx>& r)
{
	l.swap(r);
}

template<typename V, typename I, typename M, typename A,
			template<typename _V, typename _I, typename _A> class Midx> inline
void swap(safe_multi_index_container<V, I, M, A, Midx>& l,
			typename safe_multi_index_container<V, I, M, A, Midx>::first_index_type::base_type& r)
{
	l.swap(r);
}

template<typename V, typename I, typename M, typename A,
			template<typename _V, typename _I, typename _A> class Midx> inline
void swap( typename safe_multi_index_container<V, I, M, A, Midx>::first_index_type::base_type& l,
			safe_multi_index_container<V, I, M, A, Midx>& r)
{
	r.swap(l);
}

template<template<typename _V, typename _I, typename _A> class Midx,
			typename V, typename I, typename M1, typename A, typename M2> inline
void swap( safe_multi_index_container<V, I, M1, A, Midx>& l,
			safe_multi_index_container<V, I, M2, A, Midx>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace safe_container
} // namespace yggr

namespace std
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace boost


#endif //__YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_MULTI_INDEX_CONTAINER_HPP__
