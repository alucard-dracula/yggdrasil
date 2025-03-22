//options_vector.hpp

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

#ifndef __YGGR_CONTAINER_OPTIONS_VECTOR_HPP__
#define __YGGR_CONTAINER_OPTIONS_VECTOR_HPP__

#include <yggr/move/move.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/options.hpp>
#include <yggr/container/get_vector_options.hpp>
#include <yggr/container/is_isomeric_same_options_vector.hpp>

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
class options_vector;

} // namespace container
} // namespace yggr

//----------------options_vector----------------------
namespace yggr
{
namespace container
{

template<typename T, 
			typename A = 
				typename mplex::typename_expand_get<boost::container::vector<T>, 1>::type,
			typename O =
#if (BOOST_VERSION < 106700)
				void
#else
				typename mplex::typename_expand_get<boost::container::vector<T>, 2>::type
#endif // (BOOST_VERSION < 106700)
		>
class options_vector
#if (BOOST_VERSION < 106700)
	: public boost::container::vector<T, A>
#else
	: public boost::container::vector<T, A, O>
#endif // (BOOST_VERSION < 106700)
{

public:
	typedef A tpl_arg_allocator_type;
	typedef O tpl_arg_options_type;

public:

#if (BOOST_VERSION < 106700)
	typedef boost::container::vector<T, A> base_type;
#else
	typedef boost::container::vector<T, A, O> base_type;
#endif // (BOOST_VERSION < 106700)
	
public:
	typedef typename 
		get_vector_options
		<
			tpl_arg_options_type, typename base_type::size_type
		>::type options_type;

public:
	typedef typename base_type::value_type value_type;            
	typedef typename base_type::pointer pointer;               
	typedef typename base_type::const_pointer const_pointer;         
	typedef typename base_type::reference reference;             
	typedef typename base_type::const_reference const_reference;       
	typedef typename base_type::size_type size_type;             
	typedef typename base_type::difference_type difference_type;       
	typedef typename base_type::allocator_type allocator_type;        
	typedef typename base_type::stored_allocator_type stored_allocator_type; 
	typedef typename base_type::iterator iterator;              
	typedef typename base_type::const_iterator const_iterator;        
	typedef typename base_type::reverse_iterator reverse_iterator;      
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;
	typedef boost::container::allocator_traits<allocator_type> allocator_traits_type;

private:
	typedef options_vector this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

#if (BOOST_VRESION < 106700)
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(void)
		: base_type()
	{
	}

#else

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(void) BOOST_NOEXCEPT_IF(boost::container::dtl::is_nothrow_default_constructible<allocator_type>::value)
		: base_type()
	{
	}

#endif // (BOOST_VRESION < 106700)
	
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit 
	options_vector(const allocator_type& a) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit 
	options_vector(size_type n)
		: base_type(n)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE 
	options_vector(size_type n, const allocator_type& a)
		: base_type(n, value_type(), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(size_type n, const value_type& v)
		: base_type(n, v, allocator_type())
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(size_type n, const value_type& v, const allocator_type& a)
		: base_type(n, v, a)
	{
	}

	template<typename InIt> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(InIt first, InIt last)
		: base_type(first, last)
	{
	}

	template <typename InIt> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(InIt first, InIt last, const allocator_type& a)
		: base_type(first, last, a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

#	if !(BOOST_VERSION < 105700)
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(std::initializer_list<value_type> il,
					const allocator_type& a = allocator_type())
		: base_type(il, a)
	{
	}
#	endif // #	if !(BOOST_VERSION < 105700)

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF(base_type) right) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const this_type& right)
		: base_type(static_cast<const base_type&>(right))
	{
	}

#if !defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#	if !(BOOST_VERSION < 106700)
	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF_BEG
						boost::container::vector<value_type, A2, tpl_arg_options_type> 
					BOOST_RV_REF_END right, 
					typename 
						boost::container::dtl::enable_if_c
						<
							boost::container::dtl::is_version<A2, 0>::value
							&& !is_isomeric_same_options_vector
								<
									base_type,
									boost::container::vector<value_type, A2, tpl_arg_options_type>
								>::value
						>::type * = 0 )
		: base_type(boost::move(right))
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF_BEG 
						options_vector<value_type, A2, tpl_arg_options_type> 
					BOOST_RV_REF_END right, 
					typename 
						boost::container::dtl::enable_if_c
						<
							boost::container::dtl::is_version<A2, 0>::value
							&& !is_isomeric_same_options_vector
								<
									base_type, 
									typename options_vector<value_type, A2, tpl_arg_options_type>::base_type 
								>::value
						>::type * = 0 )
		: base_type(
			boost::move(
				static_cast<typename options_vector<value_type, A2, tpl_arg_options_type>::base_type&>(right)))
	{
	}

#	endif // !(BOOST_VERSION < 106700)

#endif   //!defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#if (BOOST_VERSION < 106700)

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF_BEG 
						boost::container::vector<value_type, A2> 
					BOOST_RV_REF_END right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_vector
							<
								base_type,
								boost::container::vector<value_type, A2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::vector<value_type, A2>&>(right))))
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const boost::container::vector<value_type, A2>& right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_vector
							<
								base_type,
								boost::container::vector<value_type, A2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0 )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

#else

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF_BEG 
						boost::container::vector<value_type, A2, O2> 
					BOOST_RV_REF_END right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_vector
							<
								base_type,
								boost::container::vector<value_type, A2, O2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::vector<value_type, A2, O2>&>(right))))
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const boost::container::vector<value_type, A2, O2>& right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_vector
							<
								base_type,
								boost::container::vector<value_type, A2, O2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0  )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}
#endif // (BOOST_VERSION < 106700)

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF_BEG 
						options_vector<value_type, A2, O2> 
					BOOST_RV_REF_END right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_vector
							<
								base_type, 
								typename options_vector<value_type, A2, O2>::base_type 
							>,
							mplex::sfinae_type
						>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename options_vector<value_type, A2, O2>::base_type&>(right))))
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const options_vector<value_type, A2, O2>& right, 
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_vector
							<
								base_type, 
								typename options_vector<value_type, A2, O2>::base_type 
							>,
							mplex::sfinae_type
						>::type sfinae = 0  )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF(base_type) right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const base_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

#if (BOOST_VERSION < 106700)
	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF_BEG 
						boost::container::vector<value_type, A2> 
					BOOST_RV_REF_END right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_vector
							<
								base_type, 
								boost::container::vector<value_type, A2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::vector<value_type, A2>&>(right))), a)
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const boost::container::vector<value_type, A2>& right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_vector
							<
								base_type, 
								boost::container::vector<value_type, A2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

#else

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF_BEG 
						boost::container::vector<value_type, A2, O2> 
					BOOST_RV_REF_END right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_vector
							<
								base_type, 
								boost::container::vector<value_type, A2, O2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::vector<value_type, A2, O2>&>(right))), a)
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const boost::container::vector<value_type, A2, O2>& right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_vector
							<
								base_type, 
								boost::container::vector<value_type, A2, O2> 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

#endif // (BOOST_VERSION < 106700)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const this_type& right, const allocator_type& a)
		: base_type(static_cast<const base_type&>(right), a)
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(BOOST_RV_REF_BEG 
						options_vector<value_type, A2, O2> 
					BOOST_RV_REF_END right, 
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_vector
							<
								base_type, 
								typename options_vector<value_type, A2, O2>::base_type 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast< options_vector<value_type, A2, O2>& >(right))), a)
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	options_vector(const options_vector<value_type, A2, O2>& right,
					const allocator_type& a,
					typename 
						boost::enable_if
						<
							is_isomeric_same_options_vector
							<
								base_type, 
								typename options_vector<value_type, A2, O2>::base_type 
							>,
							mplex::sfinae_type
						>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	~options_vector(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
	}

public:
	//YGGR_CXX14_CONSTEXPR_OR_INLINE 
	//this_type& operator=(BOOST_COPY_ASSIGN_REF(base_type) right)
	//{
	//	base_type::operator=(right);
	//	return *this;
	//}

	//YGGR_CXX14_CONSTEXPR_OR_INLINE 
	//this_type& operator=(BOOST_COPY_ASSIGN_REF(this_type) right)
	//{
	//	base_type::operator=(static_cast<BOOST_COPY_ASSIGN_REF(base_type)>(right));
	//	return *this;
	//}

	YGGR_CXX14_CONSTEXPR_OR_INLINE 
	this_type& operator=(const base_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE 
	this_type& operator=(const this_type& right)
	{
		base_type::operator=(static_cast<const base_type&>(right));
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CXX14_CONSTEXPR_OR_INLINE 
	this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CXX14_CONSTEXPR_OR_INLINE 
	this_type& operator=(BOOST_RV_REF(base_type) right)
		BOOST_NOEXCEPT_IF(
			allocator_traits_type::propagate_on_container_move_assignment::value
			|| allocator_traits_type::is_always_equal::value)
	{
		base_type::operator=(boost::move(right));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE 
	this_type& operator=(BOOST_RV_REF(this_type) right)
		BOOST_NOEXCEPT_IF(
			allocator_traits_type::propagate_on_container_move_assignment::value
			|| allocator_traits_type::is_always_equal::value)
	{
		base_type::operator=(boost::move(static_cast<base_type&>(right)));
		return *this;
	}

#if !defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#if !(BOOST_VERSION < 106700)

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::container::dtl::enable_if_and
		< 
			this_type&, 
			boost::container::dtl::is_version<A2, 0>,
			boost::container::dtl::is_different<A2, allocator_type>
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::vector<value_type, A2, tpl_arg_options_type> 
					BOOST_RV_REF_END right)
	{
		typedef boost::container::vector<value_type, A2, tpl_arg_options_type> right_vt_type;

		base_type::operator=(boost::move(static_cast<right_vt_type&>(right)));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::container::dtl::enable_if_and
		< 
			this_type&, 
			boost::container::dtl::is_version<A2, 0>, 
			boost::container::dtl::is_different<A2, allocator_type>
		>::type
		operator=(const boost::container::vector<value_type, A2, tpl_arg_options_type>& right)
	{
		typedef boost::container::vector<value_type, A2, tpl_arg_options_type> right_vt_type;

		base_type::operator=(static_cast<const right_vt_type&>(right));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::container::dtl::enable_if_and
		< 
			this_type&, 
			boost::container::dtl::is_version<A2, 0>,
			boost::container::dtl::is_different<A2, allocator_type>
		>::type
		operator=(BOOST_RV_REF_BEG 
						options_vector<value_type, A2, tpl_arg_options_type> 
					BOOST_RV_REF_END right)
	{
		typedef options_vector<value_type, A2, tpl_arg_options_type> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;

		base_type::operator=(boost::move(static_cast<right_base_vt_type&>(right)));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::container::dtl::enable_if_and
		< 
			this_type&, 
			boost::container::dtl::is_version<A2, 0>, 
			boost::container::dtl::is_different<A2, allocator_type>
		>::type
		operator=(const options_vector<value_type, A2, tpl_arg_options_type>& right)
	{
		typedef options_vector<value_type, A2, tpl_arg_options_type> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;

		base_type::operator=(static_cast<const right_base_vt_type&>(right));
		return *this;
	}

#endif // (BOOST_VERSION < 106700)

#endif // BOOST_CONTAINER_DOXYGEN_INVOKED

#if (BOOST_VERSION < 106700)

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_vector
			<
				base_type, 
				boost::container::vector<value_type, A2> 
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG 
						boost::container::vector<value_type, A2> 
					BOOST_RV_REF_END right)
	{
		typedef boost::container::vector<value_type, A2> right_vt_type;

		base_type::operator=(
			boost::move(reinterpret_cast<base_type&>(static_cast<right_vt_type&>(right))));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_vector
			<
				base_type, 
				boost::container::vector<value_type, A2> 
			>,
			this_type&
		>::type
		operator=(const boost::container::vector<value_type, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#else

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_vector
			<
				base_type, 
				boost::container::vector<value_type, A2, O2> 
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG 
						boost::container::vector<value_type, A2, O2> 
					BOOST_RV_REF_END right)
	{
		typedef boost::container::vector<value_type, A2, O2> right_vt_type;

		base_type::operator=(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<right_vt_type&>(right))));
		return *this;
	}

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_vector
			<
				base_type, 
				boost::container::vector<value_type, A2, O2> 
			>,
			this_type&
		>::type
		operator=(const boost::container::vector<value_type, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

#endif // (BOOST_VERSION < 106700)

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_vector
			<
				base_type, 
				typename options_vector<value_type, A2, O2>::base_type 
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG 
						options_vector<value_type, A2, O2> 
					BOOST_RV_REF_END right)
	{
		typedef options_vector<value_type, A2, O2> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;

		base_type::operator=(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<right_base_vt_type&>(right))));
		return *this;
	}

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_vector
			<
				base_type, 
				typename options_vector<value_type, A2, O2>::base_type 
			>,
			this_type&
		>::type
		operator=(const options_vector<value_type, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

public:
	using base_type::swap;

#if (BOOST_VERSION < 106700)

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_vector
			<
				base_type, 
				boost::container::vector<value_type, A2> 
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG 
					boost::container::vector<value_type, A2> 
				BOOST_RV_REF_END right)
	{
		typedef boost::container::vector<value_type, A2> right_vt_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_vt_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_vector
			<
				base_type, 
				boost::container::vector<value_type, A2> 
			>,
			void
		>::type
		swap(boost::container::vector<value_type, A2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

#else

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_vector
			<
				base_type, 
				boost::container::vector<value_type, A2, O2> 
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG 
					boost::container::vector<value_type, A2, O2> 
				BOOST_RV_REF_END right)
	{
		typedef boost::container::vector<value_type, A2, O2> right_vt_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_vt_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_vector
			<
				base_type, 
				boost::container::vector<value_type, A2, O2> 
			>,
			void
		>::type
		swap(boost::container::vector<value_type, A2, O2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

#endif // (BOOST_VERSION < 106700)

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
	}

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_vector
			<
				base_type, 
				typename options_vector<value_type, A2, O2>::base_type 
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG 
					options_vector<value_type, A2, O2> 
				BOOST_RV_REF_END right)
	{
		typedef options_vector<value_type, A2, O2> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_base_vt_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename 
		boost::enable_if
		< 
			is_isomeric_same_options_vector
			<
				base_type, 
				typename options_vector<value_type, A2, O2>::base_type 
			>,
			void
		>::type
		swap(options_vector<value_type, A2, O2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

public:
	using base_type::assign;

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

	using base_type::resize;

	using base_type::capacity;
	using base_type::reserve;
	using base_type::shrink_to_fit;

	using base_type::front;
	using base_type::back;

	using base_type::operator[];
	using base_type::at;

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

	using base_type::data;

	using base_type::emplace;
	using base_type::emplace_back;

#if (BOOST_VERSION < 105800)

#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename ...Args> YGGR_CXX14_CONSTEXPR_OR_INLINE 
	bool stable_emplace_back(BOOST_FWD_REF(Args)...args)
	{
		size_type old_size = base_type::size();
		base_type::emplace_back(boost::forward<Args>(args)...);
		return base_type::size() == (old_size + 1);
	}

#	else

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool stable_emplace_back(void)
	{
		size_type old_size = base_type::size();
		base_type::emplace_back(value_type());
		return base_type::size() == (old_size + 1);
	}

#		define YGGR_TMP_PP_STABLE_EMPLACE_BACK_DEF(__n__) \
			template< YGGR_PP_FOO_TYPES_DEF(__n__) > YGGR_CXX14_CONSTEXPR_OR_INLINE \
			bool stable_emplace_back(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				size_type old_size = base_type::size(); \
				base_type::emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
				return base_type::size() == (old_size + 1); }

#		define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_STABLE_EMPLACE_BACK_DEF( __n__ )

#		define YGGR_PP_FOO_ARG_NAME() init_arg
#		define BOOST_PP_LOCAL_LIMITS (1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#		include BOOST_PP_LOCAL_ITERATE()

#		undef YGGR_PP_FOO_ARG_NAME
#		undef YGGR_TMP_PP_STABLE_EMPLACE_BACK_DEF

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#else

	using base_type::stable_emplace_back;

#endif // (BOOST_VERSION < 105800)

	using base_type::push_back;
	using base_type::pop_back;
	
	using base_type::insert;

	using base_type::erase;
	using base_type::clear;

};

//YGGR_PP_CONTAINER_COMPARER_FULL(3, options_vector, YGGR_CXX14_CONSTEXPR_OR_INLINE)


#if (BOOST_VERSION < 106700)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
		2, 3,
		::boost::container::vector, 
		options_vector, 
		is_isomeric_same_vector,
		YGGR_CXX14_CONSTEXPR_OR_INLINE)

#else

	YGGR_PP_CONTAINER_COMPARER_FULL(3, options_vector, YGGR_CXX14_CONSTEXPR_OR_INLINE)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(
		3, 
		::boost::container::vector, 
		options_vector, 
		is_isomeric_same_options_vector,
		YGGR_CXX14_CONSTEXPR_OR_INLINE)

#endif // (BOOST_VERSION < 106700)

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, options_vector)


// shrink_to_fit
template<typename T, typename A, typename O> inline
void shrink_to_fit(options_vector<T, A, O>& c)
{
	c.shrink_to_fit();
}

//emplace
#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< options_vector<T, A, O> >,
		typename options_vector<T, A, O>::iterator
	>::type
	emplace(options_vector<T, A, O>& c, 
			typename options_vector<T, A, O>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< options_vector<T, A, O> >,
		typename options_vector<T, A, O>::iterator
	>::type
	emplace(options_vector<T, A, O>& c, 
			typename options_vector<T, A, O>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_front< options_vector<T, A, O> >,
		typename options_vector<T, A, O>::reference
	>::type
	emplace_front(options_vector<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_front(boost::forward<Args>(args)...);
	return c.front();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_front< options_vector<T, A, O> >,
		typename options_vector<T, A, O>::reference
	>::type
	emplace_front(options_vector<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_front(T(boost::forward<Args>(args)...));
	return c.front();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< options_vector<T, A, O> >,
		typename options_vector<T, A, O>::reference
	>::type
	emplace_back(options_vector<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< options_vector<T, A, O> >,
		typename options_vector<T, A, O>::reference
	>::type
	emplace_back(options_vector<T, A, O>& c, BOOST_FWD_REF(Args)... args)
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
		typename boost::enable_if<detail::has_any_emplace< options_vector<T, A, O> >, \
									typename options_vector<T, A, O>::iterator>::type \
			emplace(options_vector<T, A, O>& c, \
						typename options_vector<T, A, O>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_front< options_vector<T, A, O> >, \
			typename options_vector<T, A, O>::reference>::type \
			emplace_front(options_vector<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_front(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.front(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_back< options_vector<T, A, O> >, \
			typename options_vector<T, A, O>::reference >::type \
			emplace_back(options_vector<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.back(); }

#else

#	define YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE(__n__) \
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< options_vector<T, A, O> >, \
									typename options_vector<T, A, O>::iterator>::type \
			emplace(options_vector<T, A, O>& c, \
						typename options_vector<T, A, O>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_front< options_vector<T, A, O> >, \
			typename options_vector<T, A, O>::reference>::type \
			emplace_front(options_vector<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_back< options_vector<T, A, O> >, \
			typename options_vector<T, A, O>::reference >::type \
			emplace_back(options_vector<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.back(); }

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#	define YGGR_TMP_PP_EMPLACE_DEF_NOT_HAS_EMPLACE(__n__) \
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< options_vector<T, A, O> >, \
									typename options_vector<T, A, O>::iterator>::type \
			emplace(options_vector<T, A, O>& c, \
						typename options_vector<T, A, O>::iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_front< options_vector<T, A, O> >, \
			typename options_vector<T, A, O>::reference>::type \
			emplace_front(options_vector<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_back< options_vector<T, A, O> >, \
			typename options_vector<T, A, O>::reference >::type \
			emplace_back(options_vector<T, A, O>& c \
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
template<typename T, typename A, typename O, typename Val> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename options_vector<T, A, O>::iterator>,
				boost::is_same<Val, typename options_vector<T, A, O>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename options_vector<T, A, O>::reverse_iterator>,
				boost::is_same<Val, typename options_vector<T, A, O>::const_reverse_iterator>
			>
		>,
		typename options_vector<T, A, O>::size_type
	>::type
	erase(options_vector<T, A, O>& c, const Val& value)
{
	typedef options_vector<T, A, O> vt_type;
	typedef typename vt_type::iterator vt_iter_type;
	typedef typename vt_type::size_type vt_size_type;

	vt_iter_type it = std::remove(c.begin(), c.end(), value);
	vt_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename O, typename Pred> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename options_vector<T, A, O>::size_type 
	erase_if(options_vector<T, A, O>& c, Pred pred)
{
	typedef options_vector<T, A, O> vt_type;
	typedef typename vt_type::iterator vt_iter_type;
	typedef typename vt_type::size_type vt_size_type;

	vt_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	vt_size_type ret = std::distance(it, c.end());
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
	YGGR_PP_CONTAINER_SWAP(3, ::yggr::container::options_vector, YGGR_CXX14_CONSTEXPR_OR_INLINE)

#if (BOOST_VERSION < 106700)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		2, 3,
		::boost::container::vector, 
		::yggr::container::options_vector, 
		::yggr::container::is_isomeric_same_vector,
		YGGR_CXX14_CONSTEXPR_OR_INLINE)

#else

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP(
		3, 
		::boost::container::vector, 
		::yggr::container::options_vector, 
		::yggr::container::is_isomeric_same_options_vector,
		YGGR_CXX14_CONSTEXPR_OR_INLINE)

#endif // (BOOST_VERSION < 106700)

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

using container::options_vector;

} // namespace yggr

#endif // __YGGR_CONTAINER_OPTIONS_VECTOR_HPP__
