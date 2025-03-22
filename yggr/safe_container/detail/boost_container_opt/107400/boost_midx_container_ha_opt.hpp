// boost_midx_container_ha_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_CONTAINER_HA_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_CONTAINER_HA_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_CONTAINER_OPT_HPP__
#	error "don't include boost_midx_container_ha_opt.hpp, please include boost_midx_container_opt.hpp"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_CONTAINER_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/container/resize.hpp>
#include <yggr/container/clear.hpp>
#include <yggr/container/size.hpp>
#include <yggr/compatibility/stl_exception.hpp>

#include <yggr/safe_container/detail/boost_midx_base_type_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_keyinfo_helper.hpp>
#include <yggr/safe_container/detail/allocator_destroy_holder_midx.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/functions.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1500))
#	pragma warning(push)
#	pragma warning(disable:4522) /* spurious warning on multiple operator=()'s */
#endif // BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1500))

template<typename Value, typename IndexSpecifierList, typename Allocator>
class boost_midx_container_opt
	: private
		::boost::base_from_member
		<
			typename boost::multi_index::detail::rebind_alloc_for
			<
				Allocator,
				typename boost::multi_index::detail::multi_index_node_type
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
			typename boost::multi_index::detail::allocator_traits
			<
				typename boost::multi_index::detail::rebind_alloc_for
				<
					Allocator,
					typename boost::multi_index::detail::multi_index_node_type
					<
						Value,
						IndexSpecifierList,
						Allocator
					>::type
				>::type
			>::pointer,
			boost_midx_container_opt
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
		multi_index_base_opt_type
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
			typename indexed_by_org_cast<IndexSpecifierList>::type,
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
	friend class boost_midx_index_base_opt;

#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

protected:

	typedef typename
		multi_index_base_opt_type
		<
			Value,
			IndexSpecifierList,
			Allocator
		>::type															super;


	typedef typename
		boost::multi_index::detail::rebind_alloc_for
		<
			Allocator,
			typename super::index_node_type
		>::type					                                        node_allocator;

	typedef
		boost::multi_index::detail::allocator_traits<node_allocator>	node_allocator_traits;
	typedef node_allocator_traits										node_alloc_traits;

	typedef ::boost::base_from_member<node_allocator>                   bfm_allocator;

	typedef
		boost::multi_index::detail::header_holder
		<
			//typename node_allocator::pointer,
			typename node_allocator_traits::pointer,

			//org_type
			boost_midx_container_opt
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
#endif // BOOST_WORKAROUND(BOOST_MSVC,<1300)

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

protected:
	typedef typename super::inner_final_node_handle_type				inner_final_node_handle_type;

private:
	BOOST_MPL_ASSERT((boost::is_same<index_node_type, final_node_type>));

	// hash_opt_support_insert
protected:
	typedef
		make_boost_midx_keyinfo_meta
		<
			value_type,
			index_specifier_type_list
		>																make_boost_midx_keyinfo_meta_type;

public:
	typedef index_node_type* node_ptr_type;
	typedef midx_allocator_destroy_holder<node_allocator, 1> destroy_holder_type;
	typedef midx_allocator_destroy_holder<node_allocator, 64> multi_destroy_holder_type;

public:
	typedef typename make_boost_midx_keyinfo_meta_type::makers_type keyinfo_makers_type;
	typedef typename make_boost_midx_keyinfo_meta_type::params_type keyinfo_params_type;

private:
	typedef boost_midx_container_opt this_type;

	///* construct/copy/destroy */
private:
	boost_midx_container_opt(void);
	boost_midx_container_opt(BOOST_RV_REF(boost_midx_container_opt));
	boost_midx_container_opt(const boost_midx_container_opt&);
	~boost_midx_container_opt(void);
	const boost_midx_container_opt& operator=( const boost_midx_container_opt& );
	const boost_midx_container_opt& operator=( BOOST_RV_REF(boost_midx_container_opt) );

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
	{
		BOOST_STATIC_ASSERT(( N >= 0 && N < boost::mpl::size<index_type_list>::type::value ));
		typedef typename boost::mpl::at_c<index_type_list, N>::type type;
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
		get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int, N))const
	{
		BOOST_STATIC_ASSERT(( N >= 0 && N < boost::mpl::size<index_type_list>::type::value ));
		return *this;
	}

	template<int N> inline
	typename org_type::template nth_index<N>::type&
		org_get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
	{
		return org_cast().template get<N>();
	}

	template<int N> inline
	const typename org_type::template nth_index<N>::type&
		org_get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int, N)) const
	{
		return org_cast().template get<N>();
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

	template<typename Tag>
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
		project(IteratorType it)
	{
		typedef typename nth_index<N>::type index_type;

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

		return index_type::make_iterator(static_cast<index_node_type*>(it.get_node()));
	}

	template<int N,typename IteratorType> inline
	typename nth_index_const_iterator<N>::type
		project(IteratorType it) const
	{
		typedef typename nth_index<N>::type index_type;

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
		return index_type::make_iterator(static_cast<index_node_type*>(it.get_node()));
	}

	template<int N,typename IteratorType> inline
	typename org_type::template nth_index_iterator<N>::type
		org_project(IteratorType it)
	{
		return org_cast().template project<N>(it);
	}

	template<int N,typename IteratorType> inline
	typename org_type::template nth_index_const_iterator<N>::type
		org_project(IteratorType it) const
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
		project(IteratorType it)
	{
		typedef typename index<Tag>::type index_type;

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
		return index_type::make_iterator(static_cast<index_node_type*>(it.get_node()));
	}

	template<typename Tag,typename IteratorType> inline
	typename index_const_iterator<Tag>::type
		project(IteratorType it) const
	{
		typedef typename index<Tag>::type index_type;

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
		return index_type::make_iterator(static_cast<index_node_type*>(it.get_node()));
	}

	template<typename Tag,typename IteratorType> inline
	typename org_type::template index_iterator<Tag>::type
		org_project(IteratorType it)
	{
		return org_cast().template project<Tag>(it);
	}

	template<typename Tag,typename IteratorType> inline
	typename org_type::template index_const_iterator<Tag>::type
		org_project(IteratorType it) const
	{
		return org_cast().template project<Tag>(it);
	}

#endif // BOOST_NO_MEMBER_TEMPLATES

BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:

	inline index_node_type* header() const
	{
		return ::boost::addressof(*bfm_header::member);
	}

	inline index_node_type* allocate_node()
	{
		return boost::addressof(*bfm_allocator::member.allocate(1));
	}

	inline void deallocate_node(index_node_type* x)
	{
		typedef typename node_allocator::pointer node_pointer;
		bfm_allocator::member.deallocate(static_cast<node_pointer>(x),1);
	}

	// a_opt_support
	template<typename Destroy_Holder> inline
	void deallocate_node(Destroy_Holder& holder, index_node_type* x)
	{
		holder(x);
	}

	void construct_value(index_node_type* x, const Value& v)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
	}

	void construct_value(index_node_type* x, BOOST_RV_REF(Value) v)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
	}

	inline void destroy_value(index_node_type* x)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
	}

	template<typename Destroy_Holder> inline
	std::pair<index_node_type*, bool> insert_(Destroy_Holder& holder, index_node_type* x)
	{
		index_node_type* res = super::insert_(x);
		if(res == x)
		{
			++node_count;
			return std::pair<index_node_type*, bool>(res, true);
		}
		else
		{
			holder(x);
			return std::pair<index_node_type*, bool>(res, false);
		}
	}

	// hash_opt_support_insert
	template<typename T, typename Meta_Map, typename Variant> inline
	std::pair<index_node_type*,bool>
		insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					const Value& v, Variant variant)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<index_node_type*, bool>
		insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					const Value& v)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<index_node_type*, bool>
		insert_rv_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						const Value& v)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map, typename V> inline
	std::pair<index_node_type*, bool>
		insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						V& t)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<index_node_type*, bool>
		insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						const value_type& x)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<index_node_type*, bool>
		insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						value_type& x)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<index_node_type*,bool>
		insert_(Destroy_Holder& holder,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					index_node_type* x)
	{
		index_node_type* res = super::insert_(keyinfo_params, x);
		if(res == x)
		{
			++node_count;
			return std::pair<index_node_type*, bool>(res, true);
		}
		else
		{
			holder(x);
			return std::pair<index_node_type*, bool>(res, false);
		}
	}

	// hash_opt_support_insert
	template<typename T, typename Meta_Map, typename Variant> inline
	std::pair<index_node_type*, bool>
		insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					const Value& v, index_node_type* position, Variant variant)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<index_node_type*, bool>
		insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					const Value& v, index_node_type* position)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<index_node_type*, bool>
		insert_rv_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						const Value& v, index_node_type* position)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map, typename V> inline
	std::pair<index_node_type*, bool>
		insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						V& t, index_node_type* position)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<index_node_type*, bool>
		insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						const value_type& x, index_node_type* position)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<index_node_type*, bool>
		insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						value_type& x, index_node_type* position)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::pair<index_node_type*, bool>(0, false);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<index_node_type*,bool>
		insert_(Destroy_Holder& holder,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					index_node_type* x, index_node_type* position)
	{
		index_node_type* res = super::insert_(keyinfo_params, position, x);
		if(res==x)
		{
			++node_count;
			return std::pair<index_node_type*, bool>(res, true);
		}
		else
		{
			holder(x);
			return std::pair<index_node_type*, bool>(res, false);
		}
	}

	inline void erase_(index_node_type* x)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
	}

	template<typename Destroy_Holder> inline
	void erase_(Destroy_Holder& holder, index_node_type* x)
	{
		--node_count;
		super::erase_(holder, x);
		deallocate_node(holder, x);
	}

	template<typename T, typename Meta_Map> inline
	bool replace_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					const Value& k, index_node_type* x)
	{
		return super::replace_(keyinfo_params, k, x, boost::multi_index::detail::lvalue_tag());
	}

	template<typename T, typename Meta_Map> inline
	bool replace_rv_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					const Value& k, index_node_type* x)
	{
		return super::replace_(keyinfo_params, k, x, boost::multi_index::detail::rvalue_tag());
	}

	// hash_opt_support_modify
	template<typename T, typename Meta_Map, typename Modifier> inline
	bool modify_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier& mod, index_node_type* x)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return false;
	}

	template<typename Destroy_Holder, typename Modifier> inline
	bool modify_(Destroy_Holder& holder, Modifier& mod, index_node_type* x)
	{
		mod(const_cast<value_type&>(x->value()));

		BOOST_TRY
		{
			if(!super::modify_(holder, x))
			{
				deallocate_node(holder, x);
				--node_count;
				return false;
			}
			else
			{
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			deallocate_node(holder, x);
			--node_count;
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier> inline
	bool modify_(Destroy_Holder& holder,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier& mod, index_node_type* x)
	{
		mod(const_cast<value_type&>(x->value()));

		BOOST_TRY
		{
			if(!super::modify_(holder, keyinfo_params, x))
			{
				deallocate_node(holder, x);
				--node_count;
				return false;
			}
			else
			{
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			deallocate_node(holder, x);
			--node_count;
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename T, typename Meta_Map,
				typename Modifier,typename Rollback> inline
	bool modify_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier& mod, Rollback& back_, index_node_type* x)
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return false;
	}

	template<typename Destroy_Holder, typename Modifier, typename Rollback> inline
	bool modify_(Destroy_Holder& holder, Modifier& mod, Rollback& back_, index_node_type* x)
	{
		mod(const_cast<value_type&>(x->value()));

		bool b;
		BOOST_TRY
		{
			b = super::modify_rollback_(x);
		}
		BOOST_CATCH(...)
		{
			BOOST_TRY
			{
				back_(const_cast<value_type&>(x->value()));
				BOOST_RETHROW;
			}
			BOOST_CATCH(...)
			{
				this->erase_(holder, x);
				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}
		BOOST_CATCH_END

		BOOST_TRY
		{
			if(!b)
			{
				back_(const_cast<value_type&>(x->value()));
				return false;
			}
			else
			{
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			this->erase_(holder, x);
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map,
				typename Modifier,typename Rollback> inline
	bool modify_(Destroy_Holder& holder,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier& mod, Rollback& back_, index_node_type* x)
	{
		mod(const_cast<value_type&>(x->value()));

		bool b = false;
		BOOST_TRY
		{
			b = super::modify_rollback_(keyinfo_params, x);
		}
		BOOST_CATCH(...)
		{
			BOOST_TRY
			{
				back_(const_cast<value_type&>(x->value()));
				BOOST_RETHROW;
			}
			BOOST_CATCH(...)
			{
				this->erase_(holder, x);
				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}
		BOOST_CATCH_END

		BOOST_TRY
		{
			if(!b)
			{
				back_(const_cast<value_type&>(x->value()));
				return false;
			}
			else
			{
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			this->erase_(holder, x);
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	// node extract and insert
	inline inner_final_node_handle_type extract_(index_node_type* x)
	{
		--node_count;
		super::extract_(x);
		return inner_final_node_handle_type(x, get_allocator());
	}

	template<typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool>
		insert_nh_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					inner_final_node_handle_type& nh)
	{
		if(!nh)
		{
			return std::pair<final_node_type*, bool>(header(), false);
		}
		else
		{
			final_node_type* x = nh.node;
			final_node_type* res = super::insert_(keyinfo_params, x);
			if(res == x)
			{
				++node_count;
				return std::pair<final_node_type*, bool>(res, true);
			}
			else
			{
				return std::pair<final_node_type*, bool>(res, false);
			}
		}
	}

	template<typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool>
		insert_nh_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					inner_final_node_handle_type& nh,
					index_node_type* position)
	{
		if(!nh)
		{
			return std::pair<final_node_type*, bool>(header(), false);
		}
		else
		{
			final_node_type* x = nh.node;
			final_node_type* res = super::insert_(keyinfo_params, position, x);
			if(res == x)
			{
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


public:
	inline static keyinfo_params_type s_gen_opt_keyinfo_params(const org_type& org, const value_type& val)
	{
		return keyinfo_params_type(val, keyinfo_makers_type(org));
	}

	template<typename Sequence, typename InputIt>
	static std::pair<typename boost::range_iterator<Sequence>::type,
						typename boost::range_iterator<Sequence>::type >
		s_gen_opt_keyinfo_params_range(Sequence& seq, const org_type& org,
										InputIt first, InputIt last)
	{
		typedef Sequence seq_type;
		typedef InputIt input_iter_type;
		typedef typename boost::range_iterator<seq_type>::type iter_type;
		typedef typename boost::range_value<seq_type>::type chk_type;

		BOOST_MPL_ASSERT((boost::is_same<keyinfo_params_type, chk_type>));

		if(!boost::empty(seq))
		{
			container::clear(seq);
		}

		for(input_iter_type i = first; i != last; ++i)
		{
			std::inserter(seq, boost::end(seq))
				= keyinfo_params_type(*i, keyinfo_makers_type(org));
		}

		assert(boost::size(seq) == std::distance(first, last));
		return std::make_pair(boost::begin(seq), boost::end(seq));
	}

public:
	inline static node_ptr_type create_node(const allocator_type& valloc, const value_type& val)
	{
		node_allocator alloc(valloc);
		node_ptr_type ptr = node_ptr_type();
		try
		{
			ptr = boost::addressof(*(alloc.allocate(1)));
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			return node_ptr_type();
		}

		node_allocator_traits::construct(alloc, boost::addressof(ptr->value()), val);
		return ptr;
	}

	template<typename Sequence>
	static std::pair<typename boost::range_iterator<Sequence>::type,
						typename boost::range_iterator<Sequence>::type >
		create_node_n(Sequence& seq, const allocator_type& valloc, std::size_t n, const value_type& val)
	{

		typedef Sequence seq_type;
		typedef typename boost::range_iterator<seq_type>::type iter_type;
		typedef typename boost::range_value<seq_type>::type now_node_ptr_type;

		BOOST_MPL_ASSERT((boost::is_same<now_node_ptr_type, node_ptr_type>));

		if(!n)
		{
			return std::make_pair(boost::end(seq), boost::end(seq));
		}

		if(!boost::empty(seq))
		{
			container::clear(seq);
		}

		try
		{
			container::resize(seq, n, now_node_ptr_type());
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			return std::make_pair(boost::end(seq), boost::end(seq));
		}

		assert(boost::size(seq) == n);

		{
			iter_type iptr = boost::begin(seq);
			for(std::size_t i = 0; i != n; ++i, ++iptr)
			{
				*iptr = this_type::create_node(valloc, val);
				if(!(*iptr))
				{
					multi_destroy_holder_type holder(container::size(seq), valloc);
					holder(boost::begin(seq), boost::end(seq));
					container::clear(seq);
					break;
				}
			}
		}

		return std::make_pair(boost::begin(seq), boost::end(seq));
	}

	template<typename Sequence, typename InputIt>
	static std::pair<typename boost::range_iterator<Sequence>::type,
						typename boost::range_iterator<Sequence>::type >
		create_node_range(Sequence& seq, const allocator_type& valloc, InputIt s, InputIt e)
	{

		typedef Sequence seq_type;
		typedef InputIt input_iter_type;
		typedef typename boost::range_iterator<seq_type>::type iter_type;
		typedef typename boost::range_value<seq_type>::type now_node_ptr_type;

		BOOST_MPL_ASSERT((boost::is_same<now_node_ptr_type, node_ptr_type>));

		if(!boost::empty(seq))
		{
			container::clear(seq);
		}

		try
		{
			container::resize(seq, std::distance(s, e), now_node_ptr_type());
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			return std::make_pair(boost::end(seq), boost::end(seq));
		}

		assert(boost::size(seq) == std::distance(s, e));

		{
			iter_type iptr = boost::begin(seq);
			for(input_iter_type i = s; i != e; ++i, ++iptr)
			{
				*iptr = this_type::create_node(valloc, *i);
				if(!(*iptr))
				{
					multi_destroy_holder_type holder(container::size(seq), valloc);
					holder(boost::begin(seq), boost::end(seq));
					container::clear(seq);
					break;
				}
			}
		}

		return std::make_pair(boost::begin(seq), boost::end(seq));
	}

protected:
	std::size_t node_count;

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
};

#if BOOST_WORKAROUND(BOOST_MSVC,BOOST_TESTED_AT(1500))
#	pragma warning(pop) /* C4522 */
#endif // BOOST_WORKAROUND(BOOST_MSVC,BOOST_TESTED_AT(1500))

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_CONTAINER_HA_OPT_HPP__
