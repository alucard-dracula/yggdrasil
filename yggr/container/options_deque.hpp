//options_deque.hpp

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

#ifndef __YGGR_CONTAINER_OPTIONS_DEQUE_HPP__
#define __YGGR_CONTAINER_OPTIONS_DEQUE_HPP__

#include <yggr/move/move.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/container/deque.hpp>
#include <yggr/container/options.hpp>
#include <yggr/container/get_deque_options.hpp>
#include <yggr/container/is_isomeric_same_options_deque.hpp>

#include <yggr/container/detail/swap_def.hpp>
#include <yggr/container/detail/comparer_def.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#if BOOST_VERSION < 107100
#	include <boost/container/allocator_traits.hpp>
#endif // BOOST_VERSION < 107100

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace container
{

template<typename T, typename A, typename O>
class options_deque;

} // namespace container
} // namespace yggr

//----------------options_deque----------------------
namespace yggr
{
namespace container
{

template<typename T, 
			typename A = 
				typename mplex::typename_expand_get<boost::container::deque<T>, 1>::type,
			typename O =
#if (BOOST_VERSION < 107100)
				void
#else
				typename mplex::typename_expand_get<boost::container::vector<T>, 2>::type
#endif // (BOOST_VERSION < 107100)
		>
class options_deque
#if BOOST_VERSION < 107100
	: public boost::container::deque<T, A>
#else
	: public boost::container::deque<T, A, O>
#endif // BOOST_VERSION < 107100
{

public:
	typedef A tpl_arg_allocator_type;
	typedef O tpl_arg_options_type;

public:

#if BOOST_VERSION < 107100
	typedef boost::container::deque<T, A> base_type;
#else
	typedef boost::container::deque<T, A, O> base_type;
#endif // BOOST_VERSION < 105600

public:

#if BOOST_VERSION < 107100
	typedef deque_null_opt options_type;
#else
	typedef typename boost::container::get_deque_opt<O>::type options_type;
#endif // BOOST_VERSION < 107100

public:
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef	typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;
	typedef 
		boost::container::allocator_traits
		<
			allocator_type
		>												allocator_traits_type;

private:
	typedef options_deque this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

#if BOOST_VERSION < 107100
	 inline static size_type get_block_size(void)
     {
		 return 0; 
	 }
#else
	using base_type::get_block_size;
#endif // BOOST_VERSION < 107100

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(void)
		: base_type()
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit 
	options_deque(const allocator_type& a)
		: base_type(a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit 
	options_deque(size_type n)
		: base_type(n)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(size_type n, const allocator_type& a)
		: base_type(n, value_type(), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(size_type n, const value_type& value)
		: base_type(n, value, allocator_type())
	{ 
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(size_type n, const value_type& value, const allocator_type& a)
		: base_type(n, value, a)
	{
	}

	template <typename InIt> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(InIt first, InIt last)
		: base_type(first, last)
	{
	}

	template <typename InIt> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(InIt first, InIt last, const allocator_type& a)
		: base_type(first, last, a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

#	if !(BOOST_VERSION < 105700)
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(std::initializer_list<value_type> il, const allocator_type& a = allocator_type())
		: base_type(il, a)
	{
	}

#	endif // #if !(BOOST_VERSION < 105700)

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF(base_type) x)
		: base_type(boost::move(static_cast<base_type&>(x)))
	{ 
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const base_type& x)
		: base_type(x)
	{
	}

#if (BOOST_VERSION < 107100)

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF_BEG 
						boost::container::deque<value_type, A2> 
					BOOST_RV_REF_END right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_deque
							<
								base_type,
								boost::container::deque<value_type, A2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::deque<value_type, A2>&>(right))))
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const boost::container::deque<value_type, A2>& right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_deque
							<
								base_type,
								boost::container::deque<value_type, A2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0 )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

#else

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF_BEG 
						boost::container::deque<value_type, A2, O2> 
					BOOST_RV_REF_END right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_deque
							<
								base_type,
								boost::container::deque<value_type, A2, O2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::deque<value_type, A2, O2>&>(right))))
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const boost::container::deque<value_type, A2, O2>& right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_deque
							<
								base_type,
								boost::container::deque<value_type, A2, O2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0  )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}
#endif // (BOOST_VERSION < 107100)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF(this_type) x)
		: base_type(boost::move(static_cast<base_type&>(x)))
	{ 
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const this_type& x)
		:  base_type(static_cast<const base_type&>(x))
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF_BEG 
						options_deque<value_type, A2, O2> 
					BOOST_RV_REF_END right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_deque
							<
								base_type, 
								typename options_deque<value_type, A2, O2>::base_type 
							>,
							mplex::sfinae_type
						>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename options_deque<value_type, A2, O2>::base_type&>(right))))
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const options_deque<value_type, A2, O2>& right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_deque
							<
								base_type, 
								typename options_deque<value_type, A2, O2>::base_type 
							>,
							mplex::sfinae_type
						>::type sfinae = 0  )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF(base_type) x, const allocator_type& a)
		: base_type(boost::move(x), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const base_type& x, const allocator_type& a)
		:  base_type(x, a)
	{
	}

#if (BOOST_VERSION < 107100)
	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF_BEG 
						boost::container::deque<value_type, A2> 
					BOOST_RV_REF_END right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_deque
							<
								base_type, 
								boost::container::deque<value_type, A2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::deque<value_type, A2>&>(right))), a)
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const boost::container::deque<value_type, A2>& right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_deque
							<
								base_type, 
								boost::container::deque<value_type, A2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

#else

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF_BEG 
						boost::container::deque<value_type, A2, O2> 
					BOOST_RV_REF_END right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_deque
							<
								base_type, 
								boost::container::deque<value_type, A2, O2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::deque<value_type, A2, O2>&>(right))), a)
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const boost::container::deque<value_type, A2, O2>& right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_deque
							<
								base_type, 
								boost::container::deque<value_type, A2, O2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

#endif // (BOOST_VERSION < 107100)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF(this_type) x, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(x)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const this_type& x, const allocator_type& a)
		:  base_type(static_cast<const base_type&>(x), a)
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(BOOST_RV_REF_BEG 
						options_deque<value_type, A2, O2> 
					BOOST_RV_REF_END right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_deque
							<
								base_type, 
								typename options_deque<value_type, A2, O2>::base_type 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast< options_deque<value_type, A2, O2>& >(right))), a)
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_deque(const options_deque<value_type, A2, O2>& right,
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_deque
							<
								base_type, 
								typename options_deque<value_type, A2, O2>::base_type 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	~options_deque(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) x)
	{
		base_type::operator=(boost::move(x));
		return *this;
	}

	//inline this_type& operator=(BOOST_COPY_ASSIGN_REF(base_type) x)
	//{
	//	base_type::operator=(x);
	//	return *this;
	//}

	inline this_type& operator=(const base_type& x)
	{
		base_type::operator=(x);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) x)
	{
		base_type::operator=(boost::move(static_cast<base_type&>(x)));
		return *this;
	}

	//inline this_type& operator=(BOOST_COPY_ASSIGN_REF(this_type) x)
	//{
	//	base_type::operator=(x);
	//	return *this;
	//}

	inline this_type& operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
	inline this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

#if (BOOST_VERSION < 107100)

	template<typename A2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_deque
			<
				base_type, 
				boost::container::deque<value_type, A2> 
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG 
						boost::container::deque<value_type, A2> 
					BOOST_RV_REF_END right)
	{
		typedef boost::container::deque<value_type, A2> right_type;

		base_type::operator=(
			boost::move(reinterpret_cast<base_type&>(static_cast<right_type&>(right))));
		return *this;
	}

	template<typename A2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_deque
			<
				base_type, 
				boost::container::deque<value_type, A2> 
			>,
			this_type&
		>::type
		operator=(const boost::container::deque<value_type, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#else

	template<typename A2, typename O2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_deque
			<
				base_type, 
				boost::container::deque<value_type, A2, O2> 
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG 
						boost::container::deque<value_type, A2, O2> 
					BOOST_RV_REF_END right)
	{
		typedef boost::container::deque<value_type, A2, O2> right_type;

		base_type::operator=(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<right_type&>(right))));
		return *this;
	}

	template<typename A2, typename O2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_deque
			<
				base_type, 
				boost::container::deque<value_type, A2, O2> 
			>,
			this_type&
		>::type
		operator=(const boost::container::deque<value_type, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#endif // (BOOST_VERSION < 107100)

	template<typename A2, typename O2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_deque
			<
				base_type, 
				typename options_deque<value_type, A2, O2>::base_type 
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG 
						options_deque<value_type, A2, O2> 
					BOOST_RV_REF_END right)
	{
		typedef options_deque<value_type, A2, O2> right_type;
		typedef typename right_type::base_type right_base_type;

		base_type::operator=(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<right_base_type&>(right))));
		return *this;
	}

	template<typename A2, typename O2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_deque
			<
				base_type, 
				typename options_deque<value_type, A2, O2>::base_type 
			>,
			this_type&
		>::type
		operator=(const options_deque<value_type, A2, O2>& right)
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

#if (BOOST_VERSION < 107100)

	template<typename A2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_deque
			<
				base_type, 
				boost::container::deque<value_type, A2> 
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG 
					boost::container::deque<value_type, A2> 
				BOOST_RV_REF_END right)
	{
		typedef boost::container::deque<value_type, A2> right_type;
		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename A2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_deque
			<
				base_type, 
				boost::container::deque<value_type, A2> 
			>,
			void
		>::type
		swap(boost::container::deque<value_type, A2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

#else

	template<typename A2, typename O2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_deque
			<
				base_type, 
				boost::container::deque<value_type, A2, O2> 
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG 
					boost::container::deque<value_type, A2, O2> 
				BOOST_RV_REF_END right)
	{
		typedef boost::container::deque<value_type, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename A2, typename O2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_deque
			<
				base_type, 
				boost::container::deque<value_type, A2, O2> 
			>,
			void
		>::type
		swap(boost::container::deque<value_type, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

#endif // (BOOST_VERSION < 107100)

	template<typename A2, typename O2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_deque
			<
				base_type, 
				typename options_deque<value_type, A2, O2>::base_type 
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG 
					options_deque<value_type, A2, O2> 
				BOOST_RV_REF_END right)
	{
		typedef options_deque<value_type, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename A2, typename O2> inline
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_deque
			<
				base_type, 
				typename options_deque<value_type, A2, O2>::base_type 
			>,
			void
		>::type
		swap(options_deque<value_type, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

public:
	using base_type::assign;

	using base_type::get_allocator;
	using base_type::get_stored_allocator;

	using base_type::begin;
	using base_type::end;

	using base_type::rbegin;
	using base_type::rend;

	using base_type::cbegin;
	using base_type::cend;

	using base_type::crbegin;
	using base_type::crend;

	using base_type::empty;
	
	using base_type::size;
	using base_type::max_size;
	
	using base_type::resize;

	using base_type::shrink_to_fit;

	using base_type::front;
	using base_type::back;

	using base_type::operator[];

#if BOOST_VERSION < 105800

	inline iterator nth(size_type n)
	{
		BOOST_ASSERT(n <= base_type::size());
		return 
			n < base_type::size()?
				iterator_ex::advance_copy(base_type::begin(), n)
				: base_type::end();
	}

	inline const_iterator nth(size_type n) const
	{
		BOOST_ASSERT(n <= base_type::size());
		return 
			n < base_type::size()?
				iterator_ex::advance_copy(base_type::begin(), n)
				: base_type::end();
	}

	inline size_type index_of(iterator iter)
	{
		BOOST_ASSERT(iter <= base_type::end());
		return iterator_ex::distance(base_type::begin(), iter);
	}

	size_type index_of(const_iterator iter) const
	{
		BOOST_ASSERT(iter <= base_type::end());
		return iterator_ex::distance(base_type::begin(), iter);
	}

#else
	using base_type::nth;
	using base_type::index_of;

#endif // BOOST_VERSION < 105800

	using base_type::at;

	using base_type::emplace_front;
	using base_type::emplace_back;
	using base_type::emplace;

	using base_type::push_front;
	using base_type::push_back;

	using base_type::insert;

	using base_type::pop_front;
	using base_type::pop_back;

	using base_type::erase;

	using base_type::clear;

};

//#if (BOOST_VERSION < 107000)
//	YGGR_PP_CONTAINER_COMPARER_FULL(3, options_deque, inline)
//#endif // (BOOST_VERSION < 107000)

#if (BOOST_VERSION < 107100)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
		2, 3,
		::boost::container::deque, 
		options_deque, 
		is_isomeric_same_deque,
		inline)

#else

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(
		3, 
		::boost::container::deque, 
		options_deque, 
		is_isomeric_same_options_deque,
		inline)

#endif // (BOOST_VERSION < 107100)


YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, options_deque)


// shrink_to_fit
template<typename T, typename A, typename O> inline
void shrink_to_fit(options_deque<T, A, O>& c)
{
	c.shrink_to_fit();
}

//emplace
#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< options_deque<T, A, O> >,
		typename options_deque<T, A, O>::iterator
	>::type
	emplace(options_deque<T, A, O>& c, 
			typename options_deque<T, A, O>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< options_deque<T, A, O> >,
		typename options_deque<T, A, O>::iterator
	>::type
	emplace(options_deque<T, A, O>& c, 
			typename options_deque<T, A, O>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_front< options_deque<T, A, O> >,
		typename options_deque<T, A, O>::reference
	>::type
	emplace_front(options_deque<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_front(boost::forward<Args>(args)...);
	return c.front();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_front< options_deque<T, A, O> >,
		typename options_deque<T, A, O>::reference
	>::type
	emplace_front(options_deque<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_front(T(boost::forward<Args>(args)...));
	return c.front();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< options_deque<T, A, O> >,
		typename options_deque<T, A, O>::reference
	>::type
	emplace_back(options_deque<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< options_deque<T, A, O> >,
		typename options_deque<T, A, O>::reference
	>::type
	emplace_back(options_deque<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#else

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#	define YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE(__n__) \
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< options_deque<T, A, O> >, \
									typename options_deque<T, A, O>::iterator>::type \
			emplace(options_deque<T, A, O>& c, \
						typename options_deque<T, A, O>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_front< options_deque<T, A, O> >, \
			typename options_deque<T, A, O>::reference>::type \
			emplace_front(options_deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_front(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.front(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_back< options_deque<T, A, O> >, \
			typename options_deque<T, A, O>::reference >::type \
			emplace_back(options_deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.back(); }

#else

#	define YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE(__n__) \
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< options_deque<T, A, O> >, \
									typename options_deque<T, A, O>::iterator>::type \
			emplace(options_deque<T, A, O>& c, \
						typename options_deque<T, A, O>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_front< options_deque<T, A, O> >, \
			typename options_deque<T, A, O>::reference>::type \
			emplace_front(options_deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_back< options_deque<T, A, O> >, \
			typename options_deque<T, A, O>::reference >::type \
			emplace_back(options_deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.back(); }

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#	define YGGR_TMP_PP_EMPLACE_DEF_NOT_HAS_EMPLACE(__n__) \
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< options_deque<T, A, O> >, \
									typename options_deque<T, A, O>::iterator>::type \
			emplace(options_deque<T, A, O>& c, \
						typename options_deque<T, A, O>::iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_front< options_deque<T, A, O> >, \
			typename options_deque<T, A, O>::reference>::type \
			emplace_front(options_deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_back< options_deque<T, A, O> >, \
			typename options_deque<T, A, O>::reference >::type \
			emplace_back(options_deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.back(); }

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE(__n__) \
		YGGR_TMP_PP_EMPLACE_DEF_NOT_HAS_EMPLACE(__n__)

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF
#	undef YGGR_TMP_PP_EMPLACE_DEF_NOT_HAS_EMPLACE
#	undef YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// erase
template<typename T, typename A, typename O, typename Val> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename options_deque<T, A, O>::iterator>,
				boost::is_same<Val, typename options_deque<T, A, O>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename options_deque<T, A, O>::reverse_iterator>,
				boost::is_same<Val, typename options_deque<T, A, O>::const_reverse_iterator>
			>
		>,
		typename options_deque<T, A, O>::size_type
	>::type
	erase(options_deque<T, A, O>& c, const Val& value)
{
	typedef options_deque<T, A, O> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove(c.begin(), c.end(), value);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename O, typename Pred> inline
typename options_deque<T, A, O>::size_type 
	erase_if(options_deque<T, A, O>& c, Pred pred)
{
	typedef options_deque<T, A, O> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}


} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace swap_support
{
	YGGR_PP_CONTAINER_SWAP(3, ::yggr::container::options_deque, inline)

#if (BOOST_VERSION < 107100)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		2, 3,
		::boost::container::deque, 
		::yggr::container::options_deque, 
		::yggr::container::is_isomeric_same_deque,
		inline)

#else

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP(
		3, 
		::boost::container::deque, 
		::yggr::container::options_deque, 
		::yggr::container::is_isomeric_same_options_deque,
		inline)


#endif // (BOOST_VERSION < 107100)

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

using container::options_deque;

} // namespace yggr

#endif // __YGGR_CONTAINER_OPTIONS_DEQUE_HPP__
