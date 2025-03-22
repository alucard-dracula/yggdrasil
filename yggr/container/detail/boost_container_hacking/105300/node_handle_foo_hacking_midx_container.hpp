// node_handle_foo_hacking_midx_container.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/multi_index_container.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <yggr/container/detail/node_handle_traits.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_base_type.hpp>
#include <yggr/container/detail/cast_midx_base_type.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/functions.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Value, typename IndexSpecifierList, typename Allocator>
class node_handle_foo_hacking_midx_container 
	: private
		::boost::base_from_member
		<
			typename 
				boost::detail::allocator::rebind_to
				<
					Allocator,
					typename 
						boost::multi_index::detail::multi_index_node_type
						<
							Value,
							IndexSpecifierList,
							Allocator
						>::type
				>::type
		>,
		BOOST_MULTI_INDEX_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS
			boost::multi_index::detail::header_holder
			<
				typename 
					boost::multi_index::detail::prevent_eti
					<
						Allocator,
						typename 
							boost::detail::allocator::rebind_to
							<
								Allocator,
								typename 
									boost::multi_index::detail::multi_index_node_type
									<
										Value,
										IndexSpecifierList,
										Allocator
									>::type
							>::type
					>::type::pointer,
				node_handle_foo_hacking_midx_container
				<
					Value,
					IndexSpecifierList,
					Allocator
				>
			>,
		public
			//boost::multi_index::detail::multi_index_base_type
			/*
			 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*
			 *! this place make a super_mate					!*
			 *! the super_mete is a nth_layer					!*
			 *! the nth_layer is an indexed_wrap not an index	!*
			 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*
			 */
			multi_index_base_hacking_type
			<
				Value,
				IndexSpecifierList,
				Allocator
			>::type
{

public:
	typedef
		boost::multi_index::multi_index_container
		<
			Value,
			typename indexed_by_native_cast<IndexSpecifierList>::type,
			Allocator
		>																org_type;

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
/* The "ISO C++ Template Parser" option in CW8.3 has a problem with the
	* lifetime of const references bound to temporaries --precisely what
	* scopeguards are.
	*/

#pragma parse_mfunc_templ off
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

protected:

#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)

	template <typename,typename,typename>
	friend class boost::multi_index::detail::index_base;

	template <typename,typename>
	friend struct boost::multi_index::detail::header_holder;

	template <typename,typename>
	friend struct boost::multi_index::detail::converter;

	template <typename,typename,typename>
	friend class node_handle_foo_hacking_midx_index_base;

#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

protected:
	typedef typename
		multi_index_base_hacking_type
		<
			Value,
			IndexSpecifierList,
			Allocator
		>::type															super;

	typedef typename
		boost::detail::allocator::rebind_to
		<
			Allocator,
			typename super::index_node_type
		>::type					                                        node_allocator;

	typedef ::boost::base_from_member<node_allocator>                   bfm_allocator;

	typedef
		boost::multi_index::detail::header_holder
		<
			typename boost::multi_index::detail::prevent_eti
			<
				Allocator,
				node_allocator
			>::type::pointer,

			node_handle_foo_hacking_midx_container
			<
				Value,
				IndexSpecifierList,
				Allocator
			>
		>																bfm_header;

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
	/* see definition of index_type_list below */
	typedef typename super::index_type_list								super_index_type_list;
#endif // BOOST_WORKAROUND(BOOST_MSVC,<1300)

public:
	/* All types are inherited from super, a few are explicitly
	* brought forward here to save us some typename's.
	*/

	typedef typename super::ctor_args_list								ctor_args_list;
	typedef IndexSpecifierList											index_specifier_type_list;

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
	/* MSVC++ 6.0 chokes on moderately long index lists (around 6 indices
	* or more), with errors ranging from corrupt exes to duplicate
	* comdats. The following type hiding hack alleviates this condition;
	* best results combined with type hiding of the indexed_by construct
	* itself, as explained in the "Compiler specifics" section of
	* the documentation.
	*/

	typedef
		struct _index_type_list
				: super_index_type_list
		{
			typedef index_type_list type;
			typedef typename super_index_type_list::back back;
			typedef boost::mpl::v_iter<type,0> begin;
			typedef
				boost::mpl::v_iter
				<
					type,
					boost::mpl::size<super_index_type_list>::value
				> end;
		}																index_type_list;
#else
	typedef typename super::index_type_list								index_type_list;
#endif


	BOOST_STATIC_ASSERT((boost::multi_index::detail
							::no_duplicate_tags_in_index_list<index_type_list>::value));

	///* global project() needs to see this publicly */

	typedef typename org_type::iterator_type_list						iterator_type_list;
	typedef typename org_type::const_iterator_type_list					const_iterator_type_list;
	typedef typename org_type::value_type								value_type;
	typedef typename org_type::allocator_type							allocator_type;
	typedef typename org_type::iterator									iterator;
	typedef typename org_type::const_iterator							const_iterator;

	typedef typename super::index_node_type								index_node_type;
	typedef typename super::final_node_type								final_node_type;
	typedef typename super::final_node_handle_type						final_node_handle_type;

private:
	typedef node_handle_foo_hacking_midx_container this_type;

	///* construct/copy/destroy */
private:
	node_handle_foo_hacking_midx_container(void);
	node_handle_foo_hacking_midx_container(BOOST_RV_REF(this_type));
	node_handle_foo_hacking_midx_container(const this_type&);
	~node_handle_foo_hacking_midx_container(void);
	const this_type& operator=( const this_type& );
	const this_type& operator=( BOOST_RV_REF(this_type) );

private:
	inline org_type& org_cast(void)
	{
		return reinterpret_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return reinterpret_cast<const org_type&>(*this);
	}

public:
	inline allocator_type get_allocator(void) const YGGR_NOEXCEPT
	{
		return allocator_type(bfm_allocator::member);
	}

public:
// hash_opt_support
#if !defined(BOOST_NO_MEMBER_TEMPLATES)
	template<int N>
	struct nth_index
		: public boost::mpl::at_c<index_type_list, N>
	{
		BOOST_STATIC_ASSERT(( N >= 0 && N < boost::mpl::size<index_type_list>::type::value ));
	};

	template<int N> inline
	typename nth_index<N>::type&
		get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
	{
		BOOST_STATIC_ASSERT(( N >=0 && N < boost::mpl::size<index_type_list>::type::value ));
		return *this;
	}

	template<int N> inline
	const typename nth_index<N>::type&
		get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int, N)) const
	{
		BOOST_STATIC_ASSERT(( N >= 0 && N < boost::mpl::size<index_type_list>::type::value ));
		return *this;
	}

	template<int N> inline
	typename org_type::template nth_index<N>::type&
		org_get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
	{
		return org_cast().get<N>();
	}

	template<int N> inline
	const typename org_type::template nth_index<N>::type&
		org_get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int, N)) const
	{
		return org_cast().get<N>();
	}
#endif // BOOST_NO_MEMBER_TEMPLATES

	/* retrieval of indices by tag */

// hash_opt_support
#if !defined(BOOST_NO_MEMBER_TEMPLATES)
	template<typename Tag>
	struct index
	{
		typedef typename
			boost::mpl::find_if
			<
				index_type_list,
				boost::multi_index::detail::has_tag<Tag>
			>::type iter;

		YGGR_STATIC_CONSTANT(
			bool, index_found =
					!(boost::is_same< iter,
										typename boost::mpl::end<index_type_list>::type >::value));
		BOOST_STATIC_ASSERT(index_found);

		typedef typename boost::mpl::deref<iter>::type type;
	};

	template<typename Tag> inline
	typename index<Tag>::type&
		get(BOOST_EXPLICIT_TEMPLATE_TYPE(Tag))
	{
		return *this;
	}

	template<typename Tag> inline
	const typename index<Tag>::type&
		get(BOOST_EXPLICIT_TEMPLATE_TYPE(Tag))const
	{
		return *this;
	}

	template<typename Tag> inline
	typename org_type::template index<Tag>::type&
		org_get(BOOST_EXPLICIT_TEMPLATE_TYPE(Tag))
	{
		return org_cast().template get<Tag>();
	}

	template<typename Tag> inline
	const typename org_type::template index<Tag>::type&
		org_get(BOOST_EXPLICIT_TEMPLATE_TYPE(Tag))const
	{
		return org_cast().template get<Tag>();
	}
#endif // BOOST_NO_MEMBER_TEMPLATES

	/* projection of iterators by number */

// hash_opt_support
#if !defined(BOOST_NO_MEMBER_TEMPLATES)
	template<int N>
	struct nth_index_iterator
	{
		typedef typename nth_index<N>::type::iterator type;
	};

	template<int N>
	struct nth_index_const_iterator
	{
		typedef typename nth_index<N>::type::const_iterator type;
	};

	template<int N,typename IteratorType> inline
	typename nth_index_iterator<N>::type
		project(IteratorType it
					BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int, N))
	{
		typedef typename nth_index<N>::type index;

#if !defined(__SUNPRO_CC)||!(__SUNPRO_CC<0x580) /* fails in Sun C++ 5.7 */
		BOOST_STATIC_ASSERT(
			(boost::mpl::contains<iterator_type_list,IteratorType>::value));
#endif
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(it);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(
				it, static_cast<typename IteratorType::container_type&>(org_cast()));
		}

		return index::make_iterator(static_cast<index_node_type*>(it.get_node()));
	}

	template<int N,typename IteratorType> inline
	typename nth_index_const_iterator<N>::type
		project(IteratorType it
					BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int, N)) const
	{
		typedef typename nth_index<N>::type index;

#if !defined(__SUNPRO_CC)||!(__SUNPRO_CC<0x580) /* fails in Sun C++ 5.7 */
		BOOST_STATIC_ASSERT((
			boost::mpl::contains<iterator_type_list,IteratorType>::value||
			boost::mpl::contains<const_iterator_type_list,IteratorType>::value));
#endif
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(it);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(
				it, static_cast<const typename IteratorType::container_type&>(org_cast()));
		}
		return index::make_iterator(static_cast<index_node_type*>(it.get_node()));
	}

	template<int N,typename IteratorType> inline
	typename org_type::template nth_index_iterator<N>::type
		org_project(IteratorType it
					BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int, N))
	{
		return org_cast().template project<N>(it);
	}

	template<int N,typename IteratorType> inline
	typename org_type::template nth_index_const_iterator<N>::type
		org_project(IteratorType it
						BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int, N))const
	{
		return org_cast().template project<N>(it);
	}

#endif // BOOST_NO_MEMBER_TEMPLATES

	/* projection of iterators by tag */

// hash_opt_support
#if !defined(BOOST_NO_MEMBER_TEMPLATES)
	template<typename Tag>
	struct index_iterator
	{
		typedef typename index<Tag>::type::iterator type;
	};

	template<typename Tag>
	struct index_const_iterator
	{
		typedef typename index<Tag>::type::const_iterator type;
	};

	template<typename Tag,typename IteratorType> inline
	typename index_iterator<Tag>::type
		project(IteratorType it
					BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))
	{
		typedef typename index<Tag>::type index;

#if !defined(__SUNPRO_CC)||!(__SUNPRO_CC<0x580) /* fails in Sun C++ 5.7 */
		BOOST_STATIC_ASSERT(
			(boost::mpl::contains<iterator_type_list, IteratorType>::value));
#endif
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(it);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(
				it, static_cast<typename IteratorType::container_type&>(org_cast()));
		}
		return index::make_iterator(static_cast<index_node_type*>(it.get_node()));
	}

	template<typename Tag,typename IteratorType> inline
	typename index_const_iterator<Tag>::type
		project(IteratorType it
					BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))const
	{
		typedef typename index<Tag>::type index;

#if !defined(__SUNPRO_CC)||!(__SUNPRO_CC<0x580) /* fails in Sun C++ 5.7 */
		BOOST_STATIC_ASSERT((
			boost::mpl::contains<iterator_type_list, IteratorType>::value||
			boost::mpl::contains<const_iterator_type_list, IteratorType>::value));
#endif
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(it);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(
				it, static_cast<const typename IteratorType::container_type&>(org_cast()));
		}
		return index::make_iterator(static_cast<index_node_type*>(it.get_node()));
	}

	template<typename Tag,typename IteratorType> inline
	typename org_type::template index_iterator<Tag>::type
		org_project(IteratorType it
						BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))
	{
		return org_cast().template project<Tag>(it);
	}

	template<typename Tag,typename IteratorType> inline
	typename org_type::template index_const_iterator<Tag>::type
		org_project(IteratorType it
					BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))const
	{
		return org_cast().template project<Tag>(it);
	}

#endif // BOOST_NO_MEMBER_TEMPLATES

BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:

	inline index_node_type* header(void) const
	{
		return ::boost::addressof(*bfm_header::member);
	}

	inline index_node_type* allocate_node(void)
	{
		return ::boost::addressof(*bfm_allocator::member.allocate(1));
	}

	inline void deallocate_node(index_node_type* x)
	{
		typedef typename node_allocator::pointer node_pointer;
		bfm_allocator::member.deallocate(static_cast<node_pointer>(x),1);
	}

	inline final_node_handle_type extract_(index_node_type* x)
	{
		--node_count;
		super::extract_(x);
		return final_node_handle_type(x, get_allocator());
	}

	inline std::pair<final_node_type*, bool> 
		insert_nh_(final_node_handle_type& nh)
	{
		if(!nh)
		{
			return std::pair<final_node_type*, bool>(header(), false);
		}
		else
		{
			final_node_type* x = nh.node;
			final_node_type* res = super::insert_(x->value(), x, emplaced_tag());
			if(res == x)
			{
				nh.release_node();
				++node_count;
				return std::pair<final_node_type*, bool>(res, true);
			}
			else 
			{
				return std::pair<final_node_type*, bool>(res, false);
			}
		}
	}

	inline std::pair<final_node_type*, bool> 
		insert_nh_(final_node_handle_type& nh, index_node_type* position)
	{
		if(!nh)
		{
			return std::pair<final_node_type*, bool>(header(), false);
		}
		else
		{
			final_node_type* x = nh.node;
			final_node_type* res = super::insert_(x->value(), position, x, emplaced_tag());
			if(res == x)
			{
				nh.release_node();
				++node_count;
				return std::pair<final_node_type*, bool>(res, true);
			}
			else
			{
				return std::pair<final_node_type*, bool>(res, false);
			}
		}
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
	/* invariant stuff */

	inline bool invariant_(void) const
	{
		return super::invariant_();
	}

	inline void check_invariant_(void) const
	{
		BOOST_MULTI_INDEX_INVARIANT_ASSERT(invariant_());
	}
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

protected:
	std::size_t node_count;

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
};

template<typename V, typename I, typename A>
struct node_handle_traits_impl_custom< boost::multi_index::multi_index_container<V, I, A> >
	: public 
		node_handle_traits
		<
			typename
				boost::multi_index::detail::multi_index_base_type
				<
					V, I, A
				>::type
		>
{
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__
