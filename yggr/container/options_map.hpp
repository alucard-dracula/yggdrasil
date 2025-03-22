//options_map.hpp

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

#ifndef __YGGR_CONTAINER_OPTIONS_MAP_HPP__
#define __YGGR_CONTAINER_OPTIONS_MAP_HPP__

#include <yggr/container/map.hpp>
#include <yggr/container/options.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>

#if BOOST_VERSION < 105600
#	include <boost/container/allocator_traits.hpp>
#endif // BOOST_VERSION < 105600

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace container
{

template<typename K, typename T, typename C, typename A, typename O>
class options_map;

template<typename K, typename T, typename C, typename A, typename O>
class options_multimap;

} // namespace container
} // namespace yggr

//----------------options_map----------------------
namespace yggr
{
namespace container
{

template<typename K, typename T,
			typename C = typename mplex::typename_expand_get<boost::container::map<K, T>, 2>::type,
			typename A = typename mplex::typename_expand_get<boost::container::map<K, T>, 3>::type,
			typename O =
#if (BOOST_VERSION < 105600)
				void
#else
				typename mplex::typename_expand_get<boost::container::map<K, T>, 4>::type
#endif // (BOOST_VERSION < 106700)
			>
class options_map
#if BOOST_VERSION < 105600
	: public boost::container::map<K, T, C, A>
#else
	: public boost::container::map<K, T, C, A, O>
#endif // BOOST_VERSION < 105600
{

public:
	typedef O tpl_arg_options_type;
	typedef A tpl_arg_allocator_type;

public:

#if BOOST_VERSION < 105600
	typedef boost::container::map<K, T, C, A> base_type;
#else
	typedef boost::container::map<K, T, C, A, O> base_type;
#endif // BOOST_VERSION < 105600

public:
	typedef typename base_type::key_type				key_type;
	typedef typename base_type::mapped_type				mapped_type;
	typedef typename base_type::value_type				value_type;
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

	typedef typename base_type::value_compare			value_compare;
	typedef typename base_type::key_compare				key_compare;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;
	//typedef typename base_type::nonconst_value_type		nonconst_value_type; // boost 176 discarded
	typedef typename base_type::movable_value_type		movable_value_type;

	typedef typename detail::node_handle_traits<base_type>::node_type node_type;
	typedef typename detail::node_handle_traits<base_type>::insert_return_type insert_return_type;

public:
	typedef typename get_map_options<tpl_arg_options_type>::type options_type;

private:
	typedef options_map this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(void)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	options_map(const key_compare& comp,
				const allocator_type& a = allocator_type())
		: base_type(comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	options_map(const allocator_type& a)
		: base_type(key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& a = allocator_type())
		: base_type(first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(ordered_unique_range_t,
				InputIterator first, InputIterator last,
				const key_compare& comp = key_compare(),
				const allocator_type& a = allocator_type())
		: base_type(ordered_range, first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(InputIterator first, InputIterator last,
				const allocator_type& a)
		: base_type(first, last, key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(boost::container::ordered_range_t,
				InputIterator first, InputIterator last,
				const allocator_type& a)
		: base_type(boost::container::ordered_range, first, last, key_compare(), a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(std::initializer_list<value_type> il,
				const key_compare& comp = key_compare(),
				const allocator_type& a = allocator_type())
		: base_type(il.begin(), il.end(), comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(std::initializer_list<value_type> il, const allocator_type& a)
		: base_type(il.begin(), il.end(), key_compare(), a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const this_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const this_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	// isomeric_same
#if BOOST_VERSION < 105600
	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF_BEG
					boost::container::map<key_type, mapped_type, C2, A2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_map
						<
							boost::container::map<key_type, mapped_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::map<key_type, mapped_type, C2, A2>&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const boost::container::map<key_type, mapped_type, C2, A2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_map
						<
							boost::container::map<key_type, mapped_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF_BEG
					boost::container::map<key_type, mapped_type, C2, A2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_map
						<
							boost::container::map<key_type, mapped_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::map<key_type, mapped_type, C2, A2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const boost::container::map<key_type, mapped_type, C2, A2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_map
						<
							boost::container::map<key_type, mapped_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

#else

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF_BEG
					boost::container::map<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_map
						<
							boost::container::map<key_type, mapped_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::map<key_type, mapped_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const boost::container::map<key_type, mapped_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_map
						<
							boost::container::map<key_type, mapped_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF_BEG
					boost::container::map<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_map
						<
							boost::container::map<key_type, mapped_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::map<key_type, mapped_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const boost::container::map<key_type, mapped_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_map
						<
							boost::container::map<key_type, mapped_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

#endif // BOOST_VERSION < 105600

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF_BEG
					options_map<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_map
						<
							typename options_map<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<options_map<value_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const options_map<value_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_map
						<
							typename options_map<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(BOOST_RV_REF_BEG
					options_map<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_map
						<
							typename options_map<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<options_map<value_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_map(const options_map<value_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_map
						<
							typename options_map<value_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

	~options_map(void)
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
			is_isomeric_same_map
			<
				boost::container::map<key_type, mapped_type, C2, A2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::map<key_type, mapped_type, C2, A2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::map<key_type, mapped_type, C2, A2> right_type;

		right_type& right_ref = right;
		base_type::operator=(reinterpret_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_map
			<
				boost::container::map<key_type, mapped_type, C2, A2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::map<key_type, mapped_type, C2, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#else

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				boost::container::map<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::map<key_type, mapped_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::map<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(reinterpret_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				boost::container::map<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::map<key_type, mapped_type, C2, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#endif // BOOST_VERSION < 105600


	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				typename options_map<key_type, mapped_type, C2, A2, O2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						options_map<key_type, mapped_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef options_map<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				typename options_map<key_type, mapped_type, C2, A2, O2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const options_map<key_type, mapped_type, C2, A2, O2>& right)
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
			is_isomeric_same_map
			<
				boost::container::map<key_type, mapped_type, C2, A2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::map<key_type, mapped_type, C2, A2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::map<key_type, mapped_type, C2, A2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_map
			<
				boost::container::map<key_type, mapped_type, C2, A2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::map<key_type, mapped_type, C2, A2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
		return *this;
	}

#else

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				boost::container::map<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::map<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::map<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				boost::container::map<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::map<key_type, mapped_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

#endif // BOOST_VERSION < 105600

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				typename options_map<key_type, mapped_type, C2, A2, O2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					options_map<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef options_map<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				typename options_map<key_type, mapped_type, C2, A2, O2>::base_type,
				base_type
			>,
			void
		>::type
		swap(options_map<key_type, mapped_type, C2, A2, O2>& right)
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

	using base_type::operator[];
	using base_type::at;

	using base_type::emplace;
	using base_type::emplace_hint;

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

	template<typename ...Args> inline
	std::pair<iterator, bool>
		try_emplace(BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args)
	{
		return
			container::try_emplace(
				static_cast<base_type&>(*this),
				boost::move(key),
				boost::forward<Args>(args)...);
	}

	template<typename ...Args> inline
	std::pair<iterator, bool>
		try_emplace(const key_type& key, BOOST_FWD_REF(Args)... args)
	{
		return
			container::try_emplace(
				static_cast<base_type&>(*this),
				key,
				boost::forward<Args>(args)...);
	}

	template<typename ...Args> inline
	iterator try_emplace(const_iterator hint,
							BOOST_RV_REF(key_type) key,
							BOOST_FWD_REF(Args)... args)
	{
		return
			container::try_emplace(
				static_cast<base_type&>(*this),
				hint,
				boost::move(key),
				boost::forward<Args>(args)...);
	}

	template<typename ...Args> inline
	iterator try_emplace(const_iterator hint,
							const key_type& key,
							BOOST_FWD_REF(Args)... args)
	{
		return
			container::try_emplace(
				static_cast<base_type&>(*this),
				hint,
				key,
				boost::forward<Args>(args)...);

	}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		std::pair<iterator, bool> \
			try_emplace(BOOST_RV_REF(key_type) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				container::try_emplace( \
					static_cast<base_type&>(*this), boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		std::pair<iterator, bool> \
			try_emplace(const key_type& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				container::try_emplace( \
					static_cast<base_type&>(*this), key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		iterator try_emplace(const_iterator hint, \
								BOOST_RV_REF(key_type) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				container::try_emplace( \
					static_cast<base_type&>(*this), hint, boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		iterator try_emplace(const_iterator hint, \
								const key_type& key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				container::try_emplace( \
					static_cast<base_type&>(*this), hint, key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

	//using base_type::insert;
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

	template<typename P> inline
	std::pair<iterator, bool>
		insert(BOOST_RV_REF(P) p)
	{
		return base_type::insert(boost::forward<P>(p));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(hint, boost::forward<value_type>(val));
	}

	inline iterator insert(const_iterator hint, const value_type& val)
	{
		return base_type::insert(hint, val);
	}

	template<typename P> inline
	iterator insert(const_iterator hint, BOOST_RV_REF(P) p)
	{
		return base_type::insert(hint, boost::forward<P>(p));
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

	template<typename M> inline
	std::pair<iterator, bool>
		insert_or_assign(BOOST_RV_REF(key_type) key, BOOST_FWD_REF(M) m)
	{
		return
			container::insert_or_assign(
				static_cast<base_type&>(*this), boost::move(key), boost::forward<M>(m));
	}

	template<typename M> inline
	std::pair<iterator, bool>
		insert_or_assign(const key_type& key, BOOST_FWD_REF(M) m)
	{
		return
			container::insert_or_assign(
				static_cast<base_type&>(*this), key, boost::forward<M>(m));
	}

	template<typename M> inline
	iterator insert_or_assign(const_iterator hint, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(M) m)
	{
		return
			container::insert_or_assign(
				static_cast<base_type&>(*this), hint, boost::move(key), boost::forward<M>(m));
	}

	template<typename M> inline
	iterator insert_or_assign(const_iterator hint, const key_type& key, BOOST_FWD_REF(M) m)
	{
		return
			container::insert_or_assign(
				static_cast<base_type&>(*this), hint, key, boost::forward<M>(m));
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

	template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
				typename C2, typename A2> inline
	void merge(BOOST_RV_REF_BEG MapOrMMap<key_type, mapped_type, C2, A2> BOOST_RV_REF_END right)
	{
		container::merge(static_cast<base_type&>(*this), boost::move(right));
	}

	template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
				typename C2, typename A2> inline
	void merge(MapOrMMap<key_type, mapped_type, C2, A2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
				typename C2, typename A2, typename O2> inline
	void merge(BOOST_RV_REF_BEG MapOrMMap<key_type, mapped_type, C2, A2, O2> BOOST_RV_REF_END right)
	{
		container::merge(static_cast<base_type&>(*this), boost::move(right));
	}

	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
				typename C2, typename A2, typename O2> inline
	void merge(MapOrMMap<key_type, mapped_type, C2, A2, O2>& right)
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
		4, 5,
		::boost::container::map,
		options_map,
		is_isomeric_same_map,
		inline)

#else

	YGGR_PP_CONTAINER_COMPARER_FULL(5, options_map, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(
		5,
		::boost::container::map,
		options_map,
		is_isomeric_same_options_map,
		inline)

#endif // (BOOST_VERSION < 105600)


YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, options_map)


#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
std::pair<typename options_map<K, T, C, A, O>::iterator, bool>
	emplace(options_map<K, T, C, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

#	if !(BOOST_VERSION < 105600)

template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
typename options_map<K, T, C, A, O>::iterator
	emplace_hint(options_map<K, T, C, A, O>& c,
					typename options_map<K, T, C, A, O>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#	endif // !(BOOST_VERSION < 105600)

#else

/* !!! i don't know why emplace can be denduce and emplace_hint error C2666(2 overloads have similar conversions) is occurred !!!*/

#	if !(BOOST_VERSION < 105600)

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename options_map<K, T, C, A, O>::iterator, bool> \
			emplace(options_map<K, T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename options_map<K, T, C, A, O>::iterator \
			emplace_hint(options_map<K, T, C, A, O>& c, \
							typename options_map<K, T, C, A, O>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename options_map<K, T, C, A, O>::iterator, bool> \
			emplace(options_map<K, T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	endif // !(BOOST_VERSION < 105600)

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// contains
template<typename K, typename T, typename C, typename A, typename O, typename Key> inline
bool contains(const options_map<K, T, C, A, O>& c, const Key& key)
{
	return c.contains(key);
}

// extract
template<typename K, typename T, typename C, typename A, typename O, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename options_map<K, T, C, A, O>::node_type
	>::type
	extract(options_map<K, T, C, A, O>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename O, typename KeyOrIter> inline
typename options_map<K, T, C, A, O>::node_type
	extract(options_map<K, T, C, A, O>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert_node_type
template<typename K, typename T, typename C, typename A, typename O> inline
typename options_map<K, T, C, A, O>::insert_return_type
	insert(options_map<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG
				typename options_map<K, T, C, A, O>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename C, typename A, typename O> inline
typename options_map<K, T, C, A, O>::iterator
	insert(options_map<K, T, C, A, O>& c,
			typename options_map<K, T, C, A, O>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename options_map<K, T, C, A, O>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

//merge
template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O,
			typename C2, typename A2> inline
void merge(options_map<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG
				MapOrMMap<K, T, C2, A2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O,
			typename C2, typename A2> inline
void merge(options_map<K, T, C, A, O>& c, MapOrMMap<K, T, C2, A2>& right)
{
	c.merge(right);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O,
			typename C2, typename A2, typename O2> inline
void merge(options_map<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG
				MapOrMMap<K, T, C2, A2, O2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O,
			typename C2, typename A2, typename O2> inline
void merge(options_map<K, T, C, A, O>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	c.merge(right);
}


// try_emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
std::pair<typename options_map<K, T, C, A, O>::iterator, bool>
	try_emplace(options_map<K, T, C, A, O>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(boost::move(key), boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
std::pair<typename options_map<K, T, C, A, O>::iterator, bool>
	try_emplace(options_map<K, T, C, A, O>& c, const K& key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(key, boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
typename options_map<K, T, C, A, O>::iterator
	try_emplace(options_map<K, T, C, A, O>& c,
				typename options_map<K, T, C, A, O>::const_iterator hint,
				BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(hint, boost::move(key), boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
typename options_map<K, T, C, A, O>::iterator
	try_emplace(options_map<K, T, C, A, O>& c,
				typename options_map<K, T, C, A, O>::const_iterator hint,
				const K& key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(hint, key, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename options_map<K, T, C, A, O>::iterator, bool> \
			try_emplace(options_map<K, T, C, A, O>& c, BOOST_RV_REF(K) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename options_map<K, T, C, A, O>::iterator, bool> \
			try_emplace(options_map<K, T, C, A, O>& c, const K& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename options_map<K, T, C, A, O>::iterator \
			try_emplace(options_map<K, T, C, A, O>& c, \
						typename options_map<K, T, C, A, O>::const_iterator hint, \
						BOOST_RV_REF(K) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					hint, boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename options_map<K, T, C, A, O>::iterator \
			try_emplace(options_map<K, T, C, A, O>& c, \
						typename options_map<K, T, C, A, O>::const_iterator hint, \
						const K& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					hint, key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// insert_or_assign
template<typename K, typename T, typename C, typename A, typename O, typename M> inline
std::pair<typename options_map<K, T, C, A, O>::iterator, bool>
	insert_or_assign(options_map<K, T, C, A, O>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename O, typename M> inline
std::pair<typename options_map<K, T, C, A, O>::iterator, bool>
	insert_or_assign(options_map<K, T, C, A, O>& c, const K& key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(key, boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename O, typename M> inline
typename options_map<K, T, C, A, O>::iterator
	insert_or_assign(options_map<K, T, C, A, O>& c,
						typename options_map<K, T, C, A, O>::const_iterator hint,
						BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(hint, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename O, typename M> inline
typename options_map<K, T, C, A, O>::iterator
	insert_or_assign(options_map<K, T, C, A, O>& c,
						typename options_map<K, T, C, A, O>::const_iterator hint,
						const K& key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(hint, key, boost::forward<M>(m));
}

// erase_if
template<typename K, typename T, typename C, typename A, typename O, typename Cmper> inline
typename options_map<K, T, C, A, O>::size_type
    erase_if(options_map<K, T, C, A, O>& c, Cmper cmper)
{
	typedef options_map<K, T, C, A, O> left_type;
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
	YGGR_PP_CONTAINER_SWAP(5, ::yggr::container::options_map, inline)

#if (BOOST_VERSION < 105600)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		4, 5,
		::boost::container::map,
		::yggr::container::options_map,
		::yggr::container::is_isomeric_same_map,
		inline)

#else

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP(
		5,
		::boost::container::map,
		::yggr::container::options_map,
		::yggr::container::is_isomeric_same_options_map,
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

//----------------options_multimap----------------------
namespace yggr
{
namespace container
{

template<typename K, typename T,
			typename C = typename mplex::typename_expand_get<boost::container::multimap<K, T>, 2>::type,
			typename A = typename mplex::typename_expand_get<boost::container::multimap<K, T>, 3>::type,
			typename O =
#if (BOOST_VERSION < 105600)
				void
#else
				typename mplex::typename_expand_get<boost::container::multimap<K, T>, 4>::type
#endif // (BOOST_VERSION < 106700)
		>
class options_multimap
#if BOOST_VERSION < 105600
	: public boost::container::multimap<K, T, C, A>
#else
	: public boost::container::multimap<K, T, C, A, O>
#endif // BOOST_VERSION < 105600
{

public:
	typedef O tpl_arg_options_type;
	typedef A tpl_arg_allocator_type;

public:

#if BOOST_VERSION < 105600
	typedef boost::container::multimap<K, T, C, A> base_type;
#else
	typedef boost::container::multimap<K, T, C, A, O> base_type;
#endif // BOOST_VERSION < 105600

public:
	typedef typename base_type::key_type				key_type;
	typedef typename base_type::mapped_type				mapped_type;
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;
	typedef typename base_type::value_compare			value_compare;
	typedef typename base_type::key_compare				key_compare;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;
	//typedef typename base_type::nonconst_value_type		nonconst_value_type; // boost 176 discarded
	typedef typename base_type::movable_value_type		movable_value_type;

	typedef typename detail::node_handle_traits<base_type>::node_type node_type;

public:
	typedef typename get_multimap_options<tpl_arg_options_type>::type options_type;

private:
	typedef options_multimap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(void)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	options_multimap(const key_compare& comp,
					const allocator_type& a = allocator_type())
		: base_type(comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	options_multimap(const allocator_type& a)
		: base_type(key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& a = allocator_type())
		: base_type(first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(ordered_range_t,
		InputIterator first, InputIterator last,
		const key_compare& comp = key_compare(),
		const allocator_type& a = allocator_type())
		: base_type(ordered_range, first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(InputIterator first, InputIterator last,
						const allocator_type& a)
		: base_type(first, last, key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(boost::container::ordered_range_t,
						InputIterator first, InputIterator last,
						const allocator_type& a)
		: base_type(boost::container::ordered_range, first, last, key_compare(), a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(std::initializer_list<value_type> il,
						const key_compare& comp = key_compare(),
						const allocator_type& a = allocator_type())
		: base_type(il.begin(), il.end(), comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(std::initializer_list<value_type> il, const allocator_type& a)
		: base_type(il.begin(), il.end(), key_compare(), a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const this_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const this_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	// isomeric_same

#if BOOST_VERSION < 105600
	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF_BEG
					boost::container::multimap<key_type, mapped_type, C2, A2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_multimap
						<
							boost::container::multimap<key_type, mapped_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multimap<key_type, mapped_type, C2, A2>&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const boost::container::multimap<key_type, mapped_type, C2, A2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_multimap
						<
							boost::container::multimap<key_type, mapped_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF_BEG
						boost::container::multimap<key_type, mapped_type, C2, A2>
					BOOST_RV_REF_END right,
					const allocator_type& alloc,
					typename
						boost::enable_if
						<
							is_isomeric_same_multimap
							<
								boost::container::multimap<key_type, mapped_type, C2, A2>,
								base_type
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multimap<key_type, mapped_type, C2, A2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const boost::container::multimap<key_type, mapped_type, C2, A2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_multimap
						<
							boost::container::multimap<key_type, mapped_type, C2, A2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

#else

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF_BEG
					boost::container::multimap<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multimap
						<
							boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multimap<key_type, mapped_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const boost::container::multimap<key_type, mapped_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multimap
						<
							boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF_BEG
					boost::container::multimap<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multimap
						<
							boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multimap<key_type, mapped_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const boost::container::multimap<key_type, mapped_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multimap
						<
							boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

#endif // BOOST_VERSION < 105600

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF_BEG
					options_multimap<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multimap
						<
							typename options_multimap<key_type, mapped_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<options_multimap<key_type, mapped_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const options_multimap<key_type, mapped_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multimap
						<
							typename options_multimap<key_type, mapped_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(BOOST_RV_REF_BEG
					options_multimap<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multimap
						<
							typename options_multimap<key_type, mapped_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<options_multimap<key_type, mapped_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_multimap(const options_multimap<key_type, mapped_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multimap
						<
							typename options_multimap<key_type, mapped_type, C2, A2, O2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

	~options_multimap(void)
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
			is_isomeric_same_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::multimap<key_type, mapped_type, C2, A2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::multimap<key_type, mapped_type, C2, A2> right_type;

		right_type& right_ref = right;
		base_type::operator=(reinterpret_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::multimap<key_type, mapped_type, C2, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#else

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::multimap<key_type, mapped_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::multimap<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(reinterpret_cast<base_type&>(right_ref));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::multimap<key_type, mapped_type, C2, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#endif // BOOST_VERSION < 105600


	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				typename options_multimap<key_type, mapped_type, C2, A2, O2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						options_multimap<key_type, mapped_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef options_multimap<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				typename options_multimap<key_type, mapped_type, C2, A2, O2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const options_multimap<key_type, mapped_type, C2, A2, O2>& right)
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
			is_isomeric_same_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::multimap<key_type, mapped_type, C2, A2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::multimap<key_type, mapped_type, C2, A2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::multimap<key_type, mapped_type, C2, A2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
		return *this;
	}

#else

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::multimap<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::multimap<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::multimap<key_type, mapped_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

#endif // BOOST_VERSION < 105600

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				typename options_multimap<key_type, mapped_type, C2, A2, O2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					options_multimap<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef options_multimap<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				typename options_multimap<key_type, mapped_type, C2, A2, O2>::base_type,
				base_type
			>,
			void
		>::type
		swap(options_multimap<key_type, mapped_type, C2, A2, O2>& right)
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
		return base_type::insert(boost::forward<value_type>(val));
	}

	inline iterator insert(const value_type& val)
	{
		return base_type::insert(val);
	}

	template<typename P> inline
	iterator insert(BOOST_RV_REF(P) p)
	{
		return base_type::insert(boost::forward<P>(p));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(hint, boost::forward<value_type>(val));
	}

	inline iterator insert(const_iterator hint, const value_type& val)
	{
		return base_type::insert(hint, val);
	}

	template<typename P> inline
	iterator insert(const_iterator hint, BOOST_RV_REF(P) p)
	{
		return base_type::insert(hint, boost::forward<P>(p));
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

	inline iterator insert(BOOST_RV_REF(node_type) nh)
	{
		return container::insert(static_cast<base_type&>(*this), boost::move(nh));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		return container::insert(static_cast<base_type&>(*this), hint, boost::move(nh));
	}

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

	template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
				typename C2, typename A2> inline
	void merge(BOOST_RV_REF_BEG MapOrMMap<key_type, mapped_type, C2, A2> BOOST_RV_REF_END right)
	{
		container::merge(static_cast<base_type&>(*this), boost::move(right));
	}

	template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
				typename C2, typename A2> inline
	void merge(MapOrMMap<key_type, mapped_type, C2, A2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
				typename C2, typename A2, typename O2> inline
	void merge(BOOST_RV_REF_BEG MapOrMMap<key_type, mapped_type, C2, A2, O2> BOOST_RV_REF_END right)
	{
		container::merge(static_cast<base_type&>(*this), boost::move(right));
	}

	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
				typename C2, typename A2, typename O2> inline
	void merge(MapOrMMap<key_type, mapped_type, C2, A2, O2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	using base_type::erase;

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
		4, 5,
		::boost::container::multimap,
		options_multimap,
		is_isomeric_same_multimap,
		inline)

#else

	YGGR_PP_CONTAINER_COMPARER_FULL(5, options_multimap, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(
		5,
		::boost::container::multimap,
		options_multimap,
		is_isomeric_same_options_multimap,
		inline)

#endif // (BOOST_VERSION < 105600)

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, options_multimap)

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
typename options_multimap<K, T, C, A, O>::iterator
	emplace(options_multimap<K, T, C, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
typename options_multimap<K, T, C, A, O>::iterator
	emplace_hint(options_multimap<K, T, C, A, O>& c,
					typename options_multimap<K, T, C, A, O>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename options_multimap<K, T, C, A, O>::iterator \
			emplace(options_multimap<K, T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename options_multimap<K, T, C, A, O>::iterator \
			emplace_hint(options_multimap<K, T, C, A, O>& c, \
							typename options_multimap<K, T, C, A, O>::const_iterator pos \
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
template<typename K, typename T, typename C, typename A, typename O, typename Key> inline
bool contains(const options_multimap<K, T, C, A, O>& c, const Key& key)
{
	return c.contains(key);
}

// extract
template<typename K, typename T, typename C, typename A, typename O, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename options_multimap<K, T, C, A, O>::node_type
	>::type
	extract(options_multimap<K, T, C, A, O>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename O, typename KeyOrIter> inline
typename options_multimap<K, T, C, A, O>::node_type
	extract(options_multimap<K, T, C, A, O>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert_node_type
template<typename K, typename T, typename C, typename A, typename O> inline
typename options_multimap<K, T, C, A, O>::iterator
	insert(options_multimap<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG
				typename options_multimap<K, T, C, A, O>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename C, typename A, typename O> inline
typename options_multimap<K, T, C, A, O>::iterator
	insert(options_multimap<K, T, C, A, O>& c,
			typename options_multimap<K, T, C, A, O>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename options_multimap<K, T, C, A, O>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

//merge

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O,
			typename C2, typename A2> inline
void merge(options_multimap<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG
				MapOrMMap<K, T, C2, A2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O,
			typename C2, typename A2> inline
void merge(options_multimap<K, T, C, A, O>& c, MapOrMMap<K, T, C2, A2>& right)
{
	c.merge(right);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O,
			typename C2, typename A2, typename O2> inline
void merge(options_multimap<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG
				MapOrMMap<K, T, C2, A2, O2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O,
			typename C2, typename A2, typename O2> inline
void merge(options_multimap<K, T, C, A, O>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	c.merge(right);
}

// erase_if
template<typename K, typename T, typename C, typename A, typename O, typename Cmper> inline
typename options_multimap<K, T, C, A, O>::size_type
    erase_if(options_multimap<K, T, C, A, O>& c, Cmper cmper)
{
	typedef options_multimap<K, T, C, A, O> left_type;
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
	YGGR_PP_CONTAINER_SWAP(5, ::yggr::container::options_multimap, inline)

#if (BOOST_VERSION < 105600)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		4, 5,
		::boost::container::multimap,
		::yggr::container::options_multimap,
		::yggr::container::is_isomeric_same_multimap,
		inline)

#else

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP(
		5,
		::boost::container::multimap,
		::yggr::container::options_multimap,
		::yggr::container::is_isomeric_same_options_multimap,
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

using container::options_map;
using container::options_multimap;

} // namespace yggr

#endif // __YGGR_CONTAINER_OPTIONS_MAP_HPP__
