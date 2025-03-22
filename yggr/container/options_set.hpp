//options_set.hpp

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

#ifndef __YGGR_CONTAINER_OPTIONS_SET_HPP__
#define __YGGR_CONTAINER_OPTIONS_SET_HPP__

#include <yggr/move/move.hpp>
#include <yggr/container/set.hpp>
#include <yggr/container/options.hpp>
#include <yggr/container/get_set_options.hpp>
#include <yggr/container/is_isomeric_same_options_set.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>

#if BOOST_VERSION < 105600
#	include <boost/container/allocator_traits.hpp>
#endif // BOOST_VERSION < 105600

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace container
{

template<typename T, typename C, typename A, typename O>
class options_set;

template<typename T, typename C, typename A, typename O>
class options_multiset;

} // namespace container
} // namespace yggr

//----------------options_set----------------------
namespace yggr
{
namespace container
{

template<typename T,
			typename C = typename mplex::typename_expand_get<boost::container::set<T>, 1>::type,
			typename A = typename mplex::typename_expand_get<boost::container::set<T>, 2>::type,
			typename O =
#if (BOOST_VERSION < 105600)
				void
#else
				typename mplex::typename_expand_get<boost::container::set<T>, 3>::type
#endif // (BOOST_VERSION < 106700)
			>
class options_set
#if BOOST_VERSION < 105600
	: public boost::container::set<T, C, A>
#else
	: public boost::container::set<T, C, A, O>
#endif // BOOST_VERSION < 105600
{

public:
	typedef O tpl_arg_options_type;
	typedef A tpl_arg_allocator_type;

public:

#if BOOST_VERSION < 105600
	typedef boost::container::set<T, C, A> base_type;
#else
	typedef boost::container::set<T, C, A, O> base_type;
#endif // BOOST_VERSION < 105600

//public:
//	typedef O options_type;

public:
	typedef typename base_type::key_type				key_type;
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::key_compare				key_compare;
	typedef typename base_type::value_compare			value_compare;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;

#if BOOST_VERSION < 105600
	typedef
		boost::container::allocator_traits
		<
			allocator_type
		>												allocator_traits_type;
#else
	typedef typename base_type::allocator_traits_type	allocator_traits_type;
#endif // #if BOOST_VERSION < 105600

	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;

	typedef typename detail::node_handle_traits<base_type>::node_type node_type;
	typedef typename detail::node_handle_traits<base_type>::insert_return_type insert_return_type;

public:
	typedef typename get_set_options<tpl_arg_options_type>::type options_type;

private:
	typedef options_set this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(void)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	options_set(const key_compare& comp,
				const allocator_type& a = allocator_type())
		: base_type(comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	options_set(const allocator_type& a)
		: base_type(key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& a = allocator_type())
		: base_type(first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(ordered_unique_range_t,
		InputIterator first, InputIterator last,
		const key_compare& comp = key_compare(),
		const allocator_type& a = allocator_type())
		: base_type(ordered_range, first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(InputIterator first, InputIterator last,
				const allocator_type& a)
		: base_type(first, last, key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(boost::container::ordered_range_t,
				InputIterator first, InputIterator last,
				const allocator_type& a)
		: base_type(boost::container::ordered_range, first, last, key_compare(), a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(std::initializer_list<value_type> il,
				const key_compare& comp = key_compare(),
				const allocator_type& a = allocator_type())
		: base_type(il.begin(), il.end(), comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(std::initializer_list<value_type> il, const allocator_type& a)
		: base_type(il.begin(), il.end(), key_compare(), a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const this_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const this_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	// isomeric_same

#if BOOST_VERSION < 105600
	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF_BEG
					boost::container::set<value_type, C2, A2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_set
						<
							boost::container::set<value_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::set<value_type, C2, A2>&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const boost::container::set<value_type, C2, A2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_set
						<
							boost::container::set<value_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF_BEG
					boost::container::set<value_type, C2, A2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_set
						<
							boost::container::set<value_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::set<value_type, C2, A2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const boost::container::set<value_type, C2, A2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_set
						<
							boost::container::set<value_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

#else

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF_BEG
					boost::container::set<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_set
						<
							boost::container::set<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::set<value_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const boost::container::set<value_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_set
						<
							boost::container::set<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF_BEG
					boost::container::set<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_set
						<
							boost::container::set<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::set<value_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const boost::container::set<value_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_set
						<
							boost::container::set<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

#endif // BOOST_VERSION < 105600

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF_BEG
					options_set<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_set
						<
							typename options_set<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<options_set<value_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const options_set<value_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_set
						<
							typename options_set<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(BOOST_RV_REF_BEG
					options_set<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_set
						<
							typename options_set<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<options_set<value_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_set(const options_set<value_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_set
						<
							typename options_set<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

	~options_set(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(right));
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(static_cast<base_type&>(right_ref)));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::operator=(right);
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	inline this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST


#if BOOST_VERSION < 105600

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_set
			<
				boost::container::set<value_type, C2, A2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::set<value_type, C2, A2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::set<value_type, C2, A2> right_type;

		right_type& right_ref = right;
		base_type::operator=(reinterpret_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_set
			<
				boost::container::set<value_type, C2, A2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::set<value_type, C2, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#else

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				boost::container::set<value_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::set<value_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::set<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(reinterpret_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				boost::container::set<value_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::set<value_type, C2, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#endif // BOOST_VERSION < 105600


	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				typename options_set<value_type, C2, A2, O2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						options_set<value_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef options_set<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				typename options_set<value_type, C2, A2, O2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const options_set<value_type, C2, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

public:
	using base_type::swap;

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
	}

#if BOOST_VERSION < 105600

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_set
			<
				boost::container::set<value_type, C2, A2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::set<value_type, C2, A2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::set<value_type, C2, A2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_set
			<
				boost::container::set<value_type, C2, A2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::set<value_type, C2, A2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
		return *this;
	}

#else

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				boost::container::set<value_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::set<value_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::set<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				boost::container::set<value_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::set<value_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

#endif // BOOST_VERSION < 105600

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				typename options_set<value_type, C2, A2, O2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					options_set<value_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef options_set<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				typename options_set<value_type, C2, A2, O2>::base_type,
				base_type
			>,
			void
		>::type
		swap(options_set<value_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

public:
	using base_type::get_allocator;
	using base_type::get_stored_allocator;

	using base_type::begin;
	using base_type::cbegin;

	using base_type::end;
	using base_type::cend;

	using base_type::rbegin;
	using base_type::crbegin;

	using base_type::rend;
	using base_type::crend;

	using base_type::empty;

	using base_type::size;
	using base_type::max_size;

	using base_type::emplace;
	using base_type::emplace_hint;

	// need compatibility node_insert, so using insert function wrapper
	inline std::pair<iterator, bool>
		insert(BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(boost::forward<value_type>(val));
	}

	inline std::pair<iterator, bool>
		insert(const value_type& val)
	{
		return base_type::insert(val);
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(hint, boost::forward<value_type>(val));
	}

	inline iterator insert(const_iterator hint, const value_type& val)
	{
		return base_type::insert(hint, val);
	}

	template<typename Iter> inline
	void insert(Iter s, Iter e)
	{
		return base_type::insert(s, e);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline void insert(std::initializer_list<value_type> il)
	{
		return base_type::insert(il);
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline insert_return_type insert(BOOST_RV_REF(node_type) nh)
	{
		return container::insert(static_cast<base_type&>(*this), boost::move(nh));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		return container::insert(static_cast<base_type&>(*this), hint, boost::move(nh));
	}

	using base_type::erase;

	template<typename Key> inline
	node_type extract(BOOST_RV_REF(Key) key)
	{
		return container::extract(static_cast<base_type&>(*this), boost::move(key));
	}

#if BOOST_VERSION < 106200
	inline node_type extract(const key_type& k)
	{
		return container::extract(static_cast<base_type&>(*this), k);
	}

	inline node_type extract(const_iterator position)
	{
		return container::extract(static_cast<base_type&>(*this), position);
	}
#else
	using base_type::extract;
#endif // BOOST_VERSION < 106200

	// merge
	template<template<typename _T, typename _C, typename _A> class SetOrMSet,
				typename C2, typename A2> inline
	void merge(BOOST_RV_REF_BEG SetOrMSet<value_type, C2, A2> BOOST_RV_REF_END right)
	{
		container::merge(static_cast<base_type&>(*this), boost::move(right));
	}

	template<template<typename _T, typename _C, typename _A> class SetOrMSet,
				typename C2, typename A2> inline
	void merge(SetOrMSet<value_type, C2, A2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
				typename C2, typename A2, typename O2> inline
	void merge(BOOST_RV_REF_BEG SetOrMSet<value_type, C2, A2, O2> BOOST_RV_REF_END right)
	{
		container::merge(static_cast<base_type&>(*this), boost::move(right));
	}

	template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
				typename C2, typename A2, typename O2> inline
	void merge(SetOrMSet<value_type, C2, A2, O2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	using base_type::clear;

	using base_type::key_comp;
	using base_type::value_comp;

	using base_type::find;

	template<typename Key> inline
	bool contains(const Key& key) const
	{
		return base_type::find(key) != base_type::end();
	}

	inline bool contains(const key_type& key) const
	{
		return base_type::find(key) != base_type::end();
	}

	using base_type::count;

	using base_type::lower_bound;
	using base_type::upper_bound;

	using base_type::equal_range;

#if BOOST_VERSION < 105600
	inline void rebalance(void) {}
#else
	using base_type::rebalance;
#endif // BOOST_VERSION < 105600
};


#if (BOOST_VERSION < 105600)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
		3, 4,
		::boost::container::set,
		options_set,
		is_isomeric_same_set,
		inline)

#else

	YGGR_PP_CONTAINER_COMPARER_FULL(4, options_set, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(
		4,
		::boost::container::set,
		options_set,
		is_isomeric_same_options_set,
		inline)

#endif // (BOOST_VERSION < 105600)

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, options_set)

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename C, typename A, typename O, typename ...Args> inline
std::pair<typename options_set<T, C, A, O>::iterator, bool>
	emplace(options_set<T, C, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename A, typename O, typename ...Args> inline
typename options_set<T, C, A, O>::iterator
	emplace_hint(options_set<T, C, A, O>& c,
					typename options_set<T, C, A, O>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename options_set<T, C, A, O>::iterator, bool> \
			emplace(options_set<T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename options_set<T, C, A, O>::iterator \
			emplace_hint(options_set<T, C, A, O>& c, \
							typename options_set<T, C, A, O>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// contains
template<typename T, typename C, typename A, typename O, typename Key> inline
bool contains(const options_set<T, C, A, O>& c, const Key& key)
{
	return c.contains(key);
}

// extract
template<typename T, typename C, typename A, typename O, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename options_set<T, C, A, O>::node_type
	>::type
	extract(options_set<T, C, A, O>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename T, typename C, typename A, typename O, typename KeyOrIter> inline
typename options_set<T, C, A, O>::node_type
	extract(options_set<T, C, A, O>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert_node_type
template<typename T, typename C, typename A, typename O> inline
typename options_set<T, C, A, O>::insert_return_type
	insert(options_set<T, C, A, O>& c,
			BOOST_RV_REF_BEG
				typename options_set<T, C, A, O>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename T, typename C, typename A, typename O> inline
typename options_set<T, C, A, O>::iterator
	insert(options_set<T, C, A, O>& c,
			typename options_set<T, C, A, O>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename options_set<T, C, A, O>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

//merge
template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename O,
			typename C2, typename A2> inline
void merge(options_set<T, C, A, O>& c,
			BOOST_RV_REF_BEG
				SetOrMSet<T, C2, A2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename O,
			typename C2, typename A2> inline
void merge(options_set<T, C, A, O>& c, SetOrMSet<T, C2, A2>& right)
{
	c.merge(right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O,
			typename C2, typename A2, typename O2> inline
void merge(options_set<T, C, A, O>& c,
			BOOST_RV_REF_BEG
				SetOrMSet<T, C2, A2, O2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O,
			typename C2, typename A2, typename O2> inline
void merge(options_set<T, C, A, O>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	c.merge(right);
}

template<typename T, typename C, typename A, typename O, typename Cmper> inline
typename options_set<T, C, A, O>::size_type
    erase_if(options_set<T, C, A, O>& c, Cmper cmper)
{
	typedef options_set<T, C, A, O> left_type;
	typedef typename left_type::base_type left_base_type;
	return detail::erase_if_impl(static_cast<left_base_type&>(c), cmper);
}

} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace swap_support
{
	YGGR_PP_CONTAINER_SWAP(4, ::yggr::container::options_set, inline)

#if (BOOST_VERSION < 105600)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		3, 4,
		::boost::container::set,
		::yggr::container::options_set,
		::yggr::container::is_isomeric_same_set,
		inline)

#else

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP(
		4,
		::boost::container::set,
		::yggr::container::options_set,
		::yggr::container::is_isomeric_same_options_set,
		inline)

#endif // (BOOST_VERSION < 105600)

} // namespace swap_support

using swap_support::swap;

} // namespace container
} // namespace yggr

namespace std
{
	using ::yggr::container::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::container::swap_support::swap;
} // namespace boost

//----------------options_multiset----------------------
namespace yggr
{
namespace container
{

template<typename T,
			typename C = typename mplex::typename_expand_get<boost::container::set<T>, 1>::type,
			typename A = typename mplex::typename_expand_get<boost::container::set<T>, 2>::type,
			typename O =
#if (BOOST_VERSION < 105600)
				void
#else
				typename mplex::typename_expand_get<boost::container::set<T>, 3>::type
#endif // (BOOST_VERSION < 106700)
			>
class options_multiset
#if BOOST_VERSION < 105600
	: public boost::container::multiset<T, C, A>
#else
	: public boost::container::multiset<T, C, A, O>
#endif // BOOST_VERSION < 105600
{

public:
	typedef A tpl_arg_allocator_type;
	typedef O tpl_arg_options_type;

public:

#if BOOST_VERSION < 105600
	typedef boost::container::multiset<T, C, A> base_type;
#else
	typedef boost::container::multiset<T, C, A, O> base_type;
#endif // BOOST_VERSION < 105600

public:
	typedef typename base_type::key_type				key_type;
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::key_compare				key_compare;
	typedef typename base_type::value_compare			value_compare;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;

#if BOOST_VERSION < 105600
	typedef
		boost::container::allocator_traits
		<
			allocator_type
		>												allocator_traits_type;
#else
	typedef typename base_type::allocator_traits_type	allocator_traits_type;
#endif // #if BOOST_VERSION < 105600

	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;

	typedef typename detail::node_handle_traits<base_type>::node_type node_type;

public:
	typedef typename get_multiset_options<tpl_arg_options_type>::type options_type;

private:
	typedef options_multiset this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(void)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	options_multiset(const key_compare& comp,
					const allocator_type& a = allocator_type())
		: base_type(comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	options_multiset(const allocator_type& a)
		: base_type(key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& a = allocator_type())
		: base_type(first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(ordered_range_t,
		InputIterator first, InputIterator last,
		const key_compare& comp = key_compare(),
		const allocator_type& a = allocator_type())
		: base_type(ordered_range, first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(InputIterator first, InputIterator last,
						const allocator_type& a)
		: base_type(first, last, key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(boost::container::ordered_range_t,
						InputIterator first, InputIterator last,
						const allocator_type& a)
		: base_type(boost::container::ordered_range, first, last, key_compare(), a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(std::initializer_list<value_type> il,
						const key_compare& comp = key_compare(),
						const allocator_type& a = allocator_type())
		: base_type(il.begin(), il.end(), comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(std::initializer_list<value_type> il, const allocator_type& a)
		: base_type(il.begin(), il.end(), key_compare(), a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const this_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const this_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	// isomeric_same

#if BOOST_VERSION < 105600
	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_multiset
						<
							boost::container::multiset<value_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multiset<value_type, C2, A2>&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const boost::container::multiset<value_type, C2, A2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_multiset
						<
							boost::container::multiset<value_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_multiset
						<
							boost::container::multiset<value_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multiset<value_type, C2, A2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const boost::container::multiset<value_type, C2, A2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_multiset
						<
							boost::container::multiset<value_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

#else

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							boost::container::multiset<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multiset<value_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const boost::container::multiset<value_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							boost::container::multiset<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							boost::container::multiset<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multiset<value_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const boost::container::multiset<value_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							boost::container::multiset<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

#endif // BOOST_VERSION < 105600

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF_BEG
					options_multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							typename options_multiset<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<options_multiset<value_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const options_multiset<value_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							typename options_multiset<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(BOOST_RV_REF_BEG
					options_multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							typename options_multiset<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<options_multiset<value_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multiset(const options_multiset<value_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							typename options_multiset<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

	~options_multiset(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(right));
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(static_cast<base_type&>(right_ref)));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::operator=(right);
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	inline this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

#if BOOST_VERSION < 105600

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_multiset
			<
				boost::container::multiset<value_type, C2, A2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::multiset<value_type, C2, A2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::multiset<value_type, C2, A2> right_type;

		right_type& right_ref = right;
		base_type::operator=(reinterpret_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_multiset
			<
				boost::container::multiset<value_type, C2, A2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::multiset<value_type, C2, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#else

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				boost::container::multiset<value_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::multiset<value_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::multiset<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(reinterpret_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				boost::container::multiset<value_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::multiset<value_type, C2, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#endif // BOOST_VERSION < 105600


	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				typename options_multiset<value_type, C2, A2, O2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						options_multiset<value_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef options_multiset<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				typename options_multiset<value_type, C2, A2, O2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const options_multiset<value_type, C2, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

public:
	using base_type::swap;

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
	}

#if BOOST_VERSION < 105600

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_multiset
			<
				boost::container::multiset<value_type, C2, A2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::multiset<value_type, C2, A2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_multiset
			<
				boost::container::multiset<value_type, C2, A2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::multiset<value_type, C2, A2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
		return *this;
	}

#else

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				boost::container::multiset<value_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::multiset<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				boost::container::multiset<value_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::multiset<value_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

#endif // BOOST_VERSION < 105600

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				typename options_multiset<value_type, C2, A2, O2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					options_multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef options_multiset<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				typename options_multiset<value_type, C2, A2, O2>::base_type,
				base_type
			>,
			void
		>::type
		swap(options_multiset<value_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

public:
	using base_type::get_allocator;
	using base_type::get_stored_allocator;

	using base_type::begin;
	using base_type::cbegin;

	using base_type::end;
	using base_type::cend;

	using base_type::rbegin;
	using base_type::crbegin;

	using base_type::rend;
	using base_type::crend;

	using base_type::empty;

	using base_type::size;
	using base_type::max_size;

	using base_type::emplace;
	using base_type::emplace_hint;

	//using base_type::insert;
	inline iterator insert(BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(boost::move(val));
	}

	inline iterator insert(const value_type& val)
	{
		return base_type::insert(val);
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(hint, boost::move(val));
	}

	inline iterator insert(const_iterator hint, const value_type& val)
	{
		return base_type::insert(hint, val);
	}

	template<typename Iter> inline
	void insert(Iter s, Iter e)
	{
		return base_type::insert(s, e);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline void basic_string_assign(std::initializer_list<value_type> il)
	{
		return base_type::insert(il);
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline iterator insert(BOOST_RV_REF(node_type) nh)
	{
		return container::insert(static_cast<base_type&>(*this), boost::move(nh));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		return container::insert(static_cast<base_type&>(*this), hint, boost::move(nh));
	}

	using base_type::erase;

	template<typename Key> inline
	node_type extract(BOOST_RV_REF(Key) key)
	{
		return container::extract(static_cast<base_type&>(*this), boost::move(key));
	}

#if BOOST_VERSION < 106200
	inline node_type extract(const key_type& k)
	{
		return container::extract(static_cast<base_type&>(*this), k);
	}

	inline node_type extract(const_iterator position)
	{
		return container::extract(static_cast<base_type&>(*this), position);
	}
#else
	using base_type::extract;
#endif // BOOST_VERSION < 106200

	// merge
	template<template<typename _T, typename _C, typename _A> class SetOrMSet,
				typename C2, typename A2> inline
	void merge(BOOST_RV_REF_BEG SetOrMSet<value_type, C2, A2> BOOST_RV_REF_END right)
	{
		container::merge(static_cast<base_type&>(*this), boost::move(right));
	}

	template<template<typename _T, typename _C, typename _A> class SetOrMSet,
				typename C2, typename A2> inline
	void merge(SetOrMSet<value_type, C2, A2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
				typename C2, typename A2, typename O2> inline
	void merge(BOOST_RV_REF_BEG SetOrMSet<value_type, C2, A2, O2> BOOST_RV_REF_END right)
	{
		container::merge(static_cast<base_type&>(*this), boost::move(right));
	}

	template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
				typename C2, typename A2, typename O2> inline
	void merge(SetOrMSet<value_type, C2, A2, O2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	using base_type::clear;

	using base_type::key_comp;
	using base_type::value_comp;

	using base_type::find;

	template<typename Key> inline
	bool contains(const Key& key) const
	{
		return base_type::find(key) != base_type::end();
	}

	inline bool contains(const key_type& key) const
	{
		return base_type::find(key) != base_type::end();
	}

	using base_type::count;

	using base_type::lower_bound;
	using base_type::upper_bound;

	using base_type::equal_range;

#if BOOST_VERSION < 105600
	inline void rebalance(void) {}
#else
	using base_type::rebalance;
#endif // BOOST_VERSION < 105600
};

#if (BOOST_VERSION < 105600)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
		3, 4,
		::boost::container::multiset,
		options_multiset,
		is_isomeric_same_multiset,
		inline)

#else

	YGGR_PP_CONTAINER_COMPARER_FULL(4, options_multiset, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(
		4,
		::boost::container::multiset,
		options_multiset,
		is_isomeric_same_options_multiset,
		inline)

#endif // (BOOST_VERSION < 105600)

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, options_multiset)

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename C, typename A, typename O, typename ...Args> inline
typename options_multiset<T, C, A, O>::iterator
	emplace(options_multiset<T, C, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename A, typename O, typename ...Args> inline
typename options_multiset<T, C, A, O>::iterator
	emplace_hint(options_multiset<T, C, A, O>& c,
					typename options_multiset<T, C, A, O>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename options_multiset<T, C, A, O>::iterator \
			emplace(options_multiset<T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename options_multiset<T, C, A, O>::iterator \
			emplace_hint(options_multiset<T, C, A, O>& c, \
							typename options_multiset<T, C, A, O>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// contains
template<typename T, typename C, typename A, typename O, typename K> inline
bool contains(const options_multiset<T, C, A, O>& c, const K& key)
{
	return c.contains(key);
}

// extract
template<typename T, typename C, typename A, typename O, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename options_multiset<T, C, A, O>::node_type
	>::type
	extract(options_multiset<T, C, A, O>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename T, typename C, typename A, typename O, typename KeyOrIter> inline
typename options_multiset<T, C, A, O>::node_type
	extract(options_multiset<T, C, A, O>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert_node_type
template<typename T, typename C, typename A, typename O> inline
typename options_multiset<T, C, A, O>::iterator
	insert(options_multiset<T, C, A, O>& c,
			BOOST_RV_REF_BEG typename options_multiset<T, C, A, O>::node_type BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename T, typename C, typename A, typename O> inline
typename options_multiset<T, C, A, O>::iterator
	insert(options_multiset<T, C, A, O>& c,
			typename options_multiset<T, C, A, O>::const_iterator hint,
			BOOST_RV_REF_BEG typename options_multiset<T, C, A, O>::node_type BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

//merge
template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename O,
			typename C2, typename A2> inline
void merge(options_multiset<T, C, A, O>& c,
			BOOST_RV_REF_BEG
				SetOrMSet<T, C2, A2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename O,
			typename C2, typename A2> inline
void merge(options_multiset<T, C, A, O>& c, SetOrMSet<T, C2, A2>& right)
{
	c.merge(right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O,
			typename C2, typename A2, typename O2> inline
void merge(options_multiset<T, C, A, O>& c,
			BOOST_RV_REF_BEG
				SetOrMSet<T, C2, A2, O2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O,
			typename C2, typename A2, typename O2> inline
void merge(options_multiset<T, C, A, O>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	c.merge(right);
}

// erase_if
template<typename T, typename C, typename A, typename O, typename Cmper> inline
typename options_multiset<T, C, A, O>::size_type
    erase_if(options_multiset<T, C, A, O>& c, Cmper cmper)
{
	typedef options_multiset<T, C, A, O> left_type;
	typedef typename left_type::base_type left_base_type;
	return detail::erase_if_impl(static_cast<left_base_type&>(c), cmper);
}

} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace swap_support
{
	YGGR_PP_CONTAINER_SWAP(4, ::yggr::container::options_multiset, inline)

#if (BOOST_VERSION < 105600)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		3, 4,
		::boost::container::multiset,
		::yggr::container::options_multiset,
		::yggr::container::is_isomeric_same_multiset,
		inline)

#else

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP(
		4,
		::boost::container::multiset,
		::yggr::container::options_multiset,
		::yggr::container::is_isomeric_same_options_multiset,
		inline)

#endif // (BOOST_VERSION < 105600)

} // namespace swap_support

using swap_support::swap;

} // namespace container
} // namespace yggr

namespace std
{
	using ::yggr::container::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::container::swap_support::swap;
} // namespace boost


namespace yggr
{

using container::options_set;
using container::options_multiset;

} // namespace yggr

#endif // __YGGR_CONTAINER_OPTIONS_SET_HPP__
