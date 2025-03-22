// boost_midx_ordered_index_impl_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_ORDERED_INDEX_IMPL_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_ORDERED_INDEX_IMPL_OPT_HPP__

#if !(defined(__YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_ORDERED_INDEX_OPT_HPP__) \
		|| defined(__YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_RANKED_INDEX_OPT_HPP__))
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_ORDERED_INDEX_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>

#include <yggr/container/detail/boost_midx_check_helper.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>

#include <yggr/safe_container/detail/cast_midx_base_type.hpp>

#include <yggr/safe_container/detail/boost_midx_index_base_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_ordered_index_opt_decl.hpp>

#include <yggr/mplex/static_assert.h>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

#include <cassert>
#include <iterator>


#if !defined(YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT)
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)

#	define YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT \
		::boost::multi_index::detail::scope_guard BOOST_JOIN(check_invariant_, __LINE__) = \
			::boost::multi_index::detail::make_obj_guard( \
				*this, &boost_midx_ordered_index_impl_opt::check_invariant_); \
		BOOST_JOIN(check_invariant_, __LINE__).touch();

#else

#	define YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#endif //YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT

namespace yggr
{
namespace safe_container
{
namespace detail
{

/* boost_midx_ordered_index_impl_opt adds a layer of ordered indexing to a given Super */

/* Most of the implementation of unique and non-unique indices is
	* shared. We tell from one another on instantiation time by using
	* these tags.
	*/

template
<
	typename KeyFromValue, typename Compare,
	typename SuperMeta, typename TagList, 
	typename Category, typename AugmentPolicy
>
class boost_midx_ordered_index_opt;

// !!! gcc not support direct BOOST_MPL_ASSERT((false)) and static_assert(false), so not impl it !!!
#if defined(_MSC_VER)

template<typename T, typename AP>
struct make_augmented_interface_opt
{
	/*
	*	if a compilation error occurs here, 
	*	it means that the template specialization of an AugmentPolicy is missing
	*/
	BOOST_MPL_ASSERT((boost::mpl::false_));
};

#else

template<typename T, typename AP>
struct make_augmented_interface_opt;

#endif // _MSC_VER

template
<
	typename KeyFromValue, typename Compare,
	typename SuperMeta, typename TagList, 
	typename Category, typename AugmentPolicy
>
class boost_midx_ordered_index_impl_opt :
	BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS SuperMeta::type

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	,public 
		boost::multi_index::safe_mode::safe_container
		<
			boost_midx_ordered_index_impl_opt
			<
				KeyFromValue, Compare, 
				SuperMeta, TagList, 
				Category, AugmentPolicy
			> 
		>
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

{ 
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
/* The "ISO C++ Template Parser" option in CW8.3 has a problem with the
	* lifetime of const references bound to temporaries --precisely what
	* scopeguards are.
	*/

#pragma parse_mfunc_templ off
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

public:
	typedef
		boost::multi_index::detail::ordered_index_impl
		<
			KeyFromValue, Compare,
			typename nth_layer_org_cast<SuperMeta>::type,
			TagList, Category, AugmentPolicy
		>														org_type;

public:

	typedef typename SuperMeta::type							super;

public:
	typedef 
		boost::multi_index::detail::ordered_index_node
		<
			AugmentPolicy,
			typename super::index_node_type
		>														index_node_type;

protected:
	typedef typename index_node_type::impl_type				    node_impl_type;
	typedef typename node_impl_type::pointer					node_impl_pointer;

public:
	/* types */

	typedef typename org_type::key_type							key_type;
	typedef typename org_type::value_type						value_type;
	typedef typename org_type::key_from_value					key_from_value;
	typedef typename org_type::key_compare						key_compare;
	typedef typename org_type::value_compare					value_compare;
	typedef typename org_type::ctor_args						ctor_args;
	typedef typename org_type::allocator_type					allocator_type;
	typedef typename org_type::reference						reference;
	typedef typename org_type::const_reference					const_reference;

	typedef typename org_type::iterator							iterator;
	typedef typename org_type::const_iterator					const_iterator;
	typedef typename org_type::size_type						size_type;      
	typedef typename org_type::difference_type					difference_type;
	typedef typename org_type::pointer							pointer;
	typedef typename org_type::const_pointer					const_pointer;
	typedef typename org_type::reverse_iterator					reverse_iterator;
	typedef typename org_type::const_reverse_iterator			const_reverse_iterator;
	typedef typename org_type::tag_list							tag_list;

	typedef Category											category_type;
	typedef index_node_type*									node_ptr_type;

	typedef typename org_type::node_type						node_type;
	typedef typename org_type::insert_return_type				insert_return_type;

protected:
	typedef typename super::final_node_type						final_node_type;
	typedef 
		boost::tuples::cons
		<
			ctor_args,
			typename super::ctor_args_list
		>														ctor_args_list;
	typedef typename 
		boost::mpl::push_front
		<
			typename super::index_type_list,
			boost_midx_ordered_index_opt
			<
				KeyFromValue, Compare,
				SuperMeta, TagList, 
				Category, AugmentPolicy
			>
		>::type													index_type_list;
	typedef typename 
		boost::mpl::push_front
		<
			typename super::iterator_type_list,
			iterator
		>::type													iterator_type_list;
	typedef typename 
		boost::mpl::push_front
		<
			typename super::const_iterator_type_list,
			const_iterator
		>::type													const_iterator_type_list;
	typedef typename super::copy_map_type						copy_map_type;

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
	typedef typename super::index_saver_type					index_saver_type;
	typedef typename super::index_loader_type					index_loader_type;
#endif // BOOST_MULTI_INDEX_DISABLE_SERIALIZATION

protected:
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	typedef 
		boost::multi_index::safe_mode::safe_container
		<
			boost_midx_ordered_index_impl_opt
		>														safe_super;
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	typedef typename 
		boost::call_traits<value_type>::param_type				value_param_type;
	typedef typename 
		boost::call_traits<key_type>::param_type				key_param_type;

private:
	typedef boost_midx_ordered_index_impl_opt this_type;

private:
	boost_midx_ordered_index_impl_opt(void);
	boost_midx_ordered_index_impl_opt(BOOST_RV_REF(boost_midx_ordered_index_impl_opt));
	boost_midx_ordered_index_impl_opt(const boost_midx_ordered_index_impl_opt&);
	~boost_midx_ordered_index_impl_opt(void);
	const boost_midx_ordered_index_impl_opt& operator=( const boost_midx_ordered_index_impl_opt& );
	const boost_midx_ordered_index_impl_opt& operator=( BOOST_RV_REF(boost_midx_ordered_index_impl_opt) );

protected:
	inline org_type& org_cast(void)
	{
		return reinterpret_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return reinterpret_cast<const org_type&>(*this);
	}

public:
	// hash_opt_support final().get_allocator() needed
	inline allocator_type get_allocator(void) const
	{
		return this->final().get_allocator();
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator,bool> 
		insert(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
				BOOST_RV_REF(value_type) x)
	{
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_rv_(keyinfo_params, x);
		return std::pair<iterator ,bool>(make_iterator(p.first), p.second);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator,bool> 
		insert(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
				const value_type& x)
	{
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_(keyinfo_params, x);
		return std::pair<iterator ,bool>(make_iterator(p.first), p.second);
	}

	// a_opt_support
	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<iterator,bool> 
		insert(Destroy_Holder& holder, 
				const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
				final_node_type* x)
	{
		assert(x);
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_(holder, keyinfo_params, x);
		return std::pair<iterator ,bool>(make_iterator(p.first), p.second);
	}

	template<typename T, typename Meta_Map> inline
	iterator insert(iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						BOOST_RV_REF(value_type) x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p 
			= this->final_insert_rv_(keyinfo_params, x,
										static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	template<typename T, typename Meta_Map> inline
	iterator insert(iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						const value_type& x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p 
			= this->final_insert_(keyinfo_params, x,
									static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	iterator insert(Destroy_Holder& holder, 
						iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						final_node_type* x)
	{
		assert(x);
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p 
			= this->final_insert_(holder, keyinfo_params, x,
									static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	template<typename KeyParams_Iter, typename InputIterator>
	void insert(const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					InputIterator first, InputIterator last)
	{
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		assert(( std::distance(iter_pair.first, iter_pair.second)
					== std::distance(first, last) ));

		index_node_type* hint = header(); /* end */
		for(KeyParams_Iter ki = iter_pair.first, ksize = iter_pair.second;
				first != last && ki != ksize; ++first, ++ki)
		{
			hint = this->final_insert_ref_(*ki, *first, static_cast<final_node_type*>(hint)).first;
			index_node_type::increment(hint);
		}
	}

	template<typename Destroy_Holder, typename KeyParams_Iter, typename InputIterator>
	void insert(Destroy_Holder& holder,
					const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					InputIterator first, InputIterator last)
	{
		typedef typename boost::iterator_value<InputIterator>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<chk_type, final_node_type*>));

		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		assert(( std::distance(iter_pair.first, iter_pair.second)
					== std::distance(first, last) ));

		iterator hint = org_cast().end();
		for(KeyParams_Iter ki = iter_pair.first, ksize = iter_pair.second;
				first != last && ki != ksize; ++first, ++ki)
		{
			assert(*first);
			hint = insert(holder, hint, *ki, *first);
		}
	}

	template<typename Destroy_Holder, typename KeyParams_Iter, typename InputIterator> inline
	void insert(Destroy_Holder& holder,
					const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					const std::pair<InputIterator, InputIterator>& iter_pair_node)
	{
		return this_type::insert(holder, iter_pair, iter_pair_node.first, iter_pair_node.second);
	}

	// node insert
	template<typename T, typename Meta_Map> inline
	insert_return_type insert(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
								BOOST_RV_REF(node_type) nh)
	{
		if(nh)
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(*this, nh);
		}
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = 
			this->final_insert_nh_(
				keyinfo_params, 
				container::detail::cast_to_yggr_node_handle_midx(nh));
		return insert_return_type(make_iterator(p.first), p.second, boost::move(nh));
	}

	template<typename T, typename Meta_Map> inline
	iterator insert(const_iterator position, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					BOOST_RV_REF(node_type) nh)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
		}
		
		if(nh) 
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(*this, nh);
		}
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT
		std::pair<final_node_type*, bool> p = 
			this->final_insert_nh_(
				keyinfo_params, 
				container::detail::cast_to_yggr_node_handle_midx(nh),
				static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	// node extract
	inline node_type extract(const_iterator position)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}
		return 
			this->final_extract_(
				static_cast<final_node_type*>(position.get_node()));
	}

	inline node_type extract(key_param_type x)
	{
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		std::pair<iterator, iterator> p = org_cast().equal_range(x);

		return 
			(p.first == p.second)?
				node_type()
				: this_type::extract(p.first);
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder& holder, iterator position)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		this->final_erase_(holder, static_cast<final_node_type*>(position++.get_node()));
		return position;
	}

	template<typename Destroy_Holder>
	size_type erase(Destroy_Holder& holder, key_param_type x)
	{
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		std::pair<iterator, iterator> p = org_cast().equal_range(x);
		size_type s = 0;
		while(p.first != p.second)
		{
			p.first = this_type::erase(holder, p.first);
			++s;
		}
		return s;
	}

	template<typename Destroy_Holder>
	iterator erase(Destroy_Holder& holder, iterator first, iterator last)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(first, org_cast());
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(last, org_cast());
			BOOST_MULTI_INDEX_CHECK_VALID_RANGE(first, last);
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		while(first != last)
		{
			first = this_type::erase(holder, first);
		}
		return first;
	}

	template<typename T, typename Meta_Map> inline
	bool replace(iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					BOOST_RV_REF(value_type) x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		return this->final_replace_rv_(keyinfo_params, x, 
										static_cast<final_node_type*>(position.get_node()));
	}

	template<typename T, typename Meta_Map> inline
	bool replace(iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					const value_type& x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		return this->final_replace_(keyinfo_params, x, 
										static_cast<final_node_type*>(position.get_node()));
	}

	template<typename T, typename Meta_Map, typename Modifier> inline
	bool modify(iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	/* MSVC++ 6.0 optimizer on safe mode code chokes if this
		* this is not added. Left it for all compilers as it does no
		* harm.
		*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(keyinfo_params, mod, 
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename Modifier> inline
	bool modify(Destroy_Holder& holder, iterator position, Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	/* MSVC++ 6.0 optimizer on safe mode code chokes if this
		* this is not added. Left it for all compilers as it does no
		* harm.
		*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(holder, mod, 
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier> inline
	bool modify(Destroy_Holder& holder, iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	/* MSVC++ 6.0 optimizer on safe mode code chokes if this
		* this is not added. Left it for all compilers as it does no
		* harm.
		*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(holder, keyinfo_params, mod, 
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename T, typename Meta_Map, typename Modifier, typename Rollback> inline
	bool modify(iterator position, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(keyinfo_params, mod, back_, 
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename Modifier, typename Rollback> inline
	bool modify(Destroy_Holder& holder, iterator position, Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(holder, mod, back_, 
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier, typename Rollback> inline
	bool modify(Destroy_Holder& holder, iterator position, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(holder, keyinfo_params, mod, back_, 
									static_cast<final_node_type*>(position.get_node()));
	}
  
	template<typename T, typename Meta_Map, typename Modifier> inline
	bool modify_key(iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		return modify(position, keyinfo_params,
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key));
	}

	template<typename Destroy_Holder, typename Modifier> inline
	bool modify_key(Destroy_Holder& holder, iterator position, Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		return modify(holder, position,
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key));
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier> inline
	bool modify_key(Destroy_Holder& holder, iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}

		return modify(holder, position, keyinfo_params,
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key));
	}

	template<typename T, typename Meta_Map, typename Modifier, typename Rollback> inline
	bool modify_key(iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}
		return modify( position, keyinfo_params, 
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key),
						boost::multi_index::detail::modify_key_adaptor
						<
							Rollback, value_type, KeyFromValue
						>(back_, key) );
	}

	template<typename Destroy_Holder, typename Modifier, typename Rollback> inline
	bool modify_key(Destroy_Holder& holder, iterator position, Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}
		return modify( holder, position, 
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key),
						boost::multi_index::detail::modify_key_adaptor
						<
							Rollback, value_type, KeyFromValue
						>(back_, key) );
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier, typename Rollback> inline
	bool modify_key(Destroy_Holder& holder, iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		}
		return modify( holder, position, keyinfo_params, 
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key),
						boost::multi_index::detail::modify_key_adaptor
						<
							Rollback, value_type, KeyFromValue
						>(back_, key) );
	}

BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)

	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node, &org_cast());
	}

	inline const_iterator make_iterator(index_node_type* node) const
	{
		return const_iterator(node,const_cast<boost_midx_ordered_index_impl_opt*>(&org_cast()));
	}

#else

	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node);
	}
	
	inline const_iterator make_iterator(index_node_type* node) const
	{
		return const_iterator(node);
	}

#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	inline void extract_(index_node_type* x)
	{
		node_impl_type::rebalance_for_extract(
			x->impl(),header()->parent(), header()->left(),header()->right());
		super::extract_(x);

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename T, typename Meta_Map, typename Variant> inline
	final_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						value_param_type v, final_node_type*& x, Variant variant)
	{
		link_info inf;
		if(!link_point(key(v), inf, Category()))
		{
			return static_cast<final_node_type*>(index_node_type::from_impl(inf.pos));
		}

		final_node_type* res = super::insert_(keyinfo_params, v, x, variant);
		if(res == x)
		{
			node_impl_type::link(static_cast<index_node_type*>(x)->impl(), 
									inf.side, inf.pos, header()->impl());
		}

		return res;
	}

	// ordered_a_opt_support
	template<typename T, typename Meta_Map> inline
	final_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
								final_node_type* x)
	{
		link_info inf;
		if(!link_point(key(x->value()), inf, Category()))
		{
			return static_cast<final_node_type*>(index_node_type::from_impl(inf.pos));
		}

		final_node_type* res = super::insert_(keyinfo_params, x);
		if(res == x)
		{
			node_impl_type::link(static_cast<index_node_type*>(x)->impl(), 
									inf.side, inf.pos, header()->impl());
		}

		return res;
	}

	template<typename T, typename Meta_Map, typename Variant> inline
	final_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
								value_param_type v, index_node_type* position, final_node_type*& x, 
								Variant variant)
	{
		link_info inf;
		if(!hinted_link_point(key(v), position, inf, Category()))
		{
			return static_cast<final_node_type*>(index_node_type::from_impl(inf.pos));
		}

		final_node_type* res=super::insert_(keyinfo_params, v, position, x, variant);
		if(res == x)
		{
			node_impl_type::link(static_cast<index_node_type*>(x)->impl(),
									inf.side, inf.pos, header()->impl());
		}

		return res;
	}

	template<typename T, typename Meta_Map> inline
	final_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
								index_node_type* position, final_node_type* x)
	{

		link_info inf;
		if(!hinted_link_point(key(x->value()), position, inf, Category()))
		{
			return static_cast<final_node_type*>(index_node_type::from_impl(inf.pos));
		}

		final_node_type* res=super::insert_(keyinfo_params, position, x);
		if(res == x)
		{
			node_impl_type::link(static_cast<index_node_type*>(x)->impl(),
									inf.side, inf.pos, header()->impl());
		}

		return res;
	}

	inline void erase_(index_node_type* x)
	{
		node_impl_type::rebalance_for_extract(
			x->impl(),header()->parent(), header()->left(),header()->right());
		super::erase_(x);

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename Destroy_Holder> inline
	void erase_(Destroy_Holder& holder, index_node_type* x)
	{
		node_impl_type::rebalance_for_extract(
			x->impl(), header()->parent(), header()->left(), header()->right());
		super::erase_(holder, x);

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename T, typename Meta_Map, typename Variant> inline
	bool replace_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					value_param_type v, index_node_type* x, Variant variant)
	{
		if(in_place(v,x,Category()))
		{
			return super::replace_(keyinfo_params, v, x, variant);
		}

		index_node_type* next = x;
		index_node_type::increment(next);

		node_impl_type::rebalance_for_extract(
			x->impl(), header()->parent(), header()->left(), header()->right());

		BOOST_TRY
		{
			link_info inf;
			if(link_point(key(v), inf, Category())
				&& super::replace_(keyinfo_params, v, x, variant))
			{
				node_impl_type::link(x->impl(), inf.side, inf.pos, header()->impl());
				return true;
			}
			node_impl_type::restore(x->impl(), next->impl(), header()->impl());
			return false;
		}
		BOOST_CATCH(...)
		{
			node_impl_type::restore(x->impl(), next->impl(), header()->impl());
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename T, typename Meta_Map> inline
	bool modify_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					index_node_type* x)
	{
		bool b = false;
		BOOST_TRY
		{
			b = in_place(x->value(), x, Category());
		}
		BOOST_CATCH(...)
		{
			erase_(x);
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
		if(!b)
		{
			node_impl_type::rebalance_for_extract(
				x->impl(), header()->parent(), header()->left(), header()->right());
			BOOST_TRY
			{
				link_info inf;
				if(!link_point(key(x->value()),inf,Category()))
				{
					super::erase_(x);

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
					detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
					return false;
				}
				node_impl_type::link(x->impl(),inf.side,inf.pos,header()->impl());
			}
			BOOST_CATCH(...)
			{
				super::erase_(x);

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}

		BOOST_TRY
		{
			if(!super::modify_(keyinfo_params, x))
			{
				node_impl_type::rebalance_for_extract(
					x->impl(), header()->parent(), header()->left(), header()->right());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				return false;
			}
			else 
			{ 
				return true; 
			}
		}
		BOOST_CATCH(...)
		{
			node_impl_type::rebalance_for_extract(
				x->impl(), header()->parent(), header()->left(), header()->right());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename Destroy_Holder> inline
	bool modify_(Destroy_Holder& holder, index_node_type* x)
	{
		bool b = false;
		BOOST_TRY
		{
			b = in_place(x->value(), x, Category());
		}
		BOOST_CATCH(...)
		{
			erase_(holder, x);
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
		if(!b)
		{
			node_impl_type::rebalance_for_extract(
				x->impl(), header()->parent(), header()->left(), header()->right());
			BOOST_TRY
			{
				link_info inf;
				if(!link_point(key(x->value()),inf,Category()))
				{
					super::erase_(holder, x);

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
					detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
					return false;
				}
				node_impl_type::link(x->impl(),inf.side,inf.pos,header()->impl());
			}
			BOOST_CATCH(...)
			{
				super::erase_(holder, x);

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}

		BOOST_TRY
		{
			if(!super::modify_(holder, x))
			{
				node_impl_type::rebalance_for_extract(
					x->impl(), header()->parent(), header()->left(), header()->right());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				return false;
			}
			else 
			{ 
				return true; 
			}
		}
		BOOST_CATCH(...)
		{
			node_impl_type::rebalance_for_extract(
				x->impl(), header()->parent(), header()->left(), header()->right());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	bool modify_(Destroy_Holder& holder,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					index_node_type* x)
	{
		bool b = false;
		BOOST_TRY
		{
			b = in_place(x->value(), x, Category());
		}
		BOOST_CATCH(...)
		{
			erase_(holder, x);
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
		if(!b)
		{
			node_impl_type::rebalance_for_extract(
				x->impl(), header()->parent(), header()->left(), header()->right());
			BOOST_TRY
			{
				link_info inf;
				if(!link_point(key(x->value()),inf,Category()))
				{
					super::erase_(holder, x);

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
					detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
					return false;
				}
				node_impl_type::link(x->impl(),inf.side,inf.pos,header()->impl());
			}
			BOOST_CATCH(...)
			{
				super::erase_(holder, x);

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}

		BOOST_TRY
		{
			if(!super::modify_(holder, keyinfo_params, x))
			{
				node_impl_type::rebalance_for_extract(
					x->impl(), header()->parent(), header()->left(), header()->right());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				return false;
			}
			else 
			{ 
				return true; 
			}
		}
		BOOST_CATCH(...)
		{
			node_impl_type::rebalance_for_extract(
				x->impl(), header()->parent(), header()->left(), header()->right());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	inline bool modify_rollback_(index_node_type* x)
	{
		if(in_place(x->value(), x, Category()))
		{
			return super::modify_rollback_(x);
		}

		index_node_type* next = x;
		index_node_type::increment(next);

		node_impl_type::rebalance_for_extract(
			x->impl(), header()->parent(), header()->left(), header()->right());

		BOOST_TRY
		{
			link_info inf;
			if(link_point(key(x->value()), inf, Category())
				&& super::modify_rollback_(x))
			{
				node_impl_type::link(x->impl(), inf.side, inf.pos, header()->impl());
				return true;
			}
			node_impl_type::restore(x->impl(), next->impl(), header()->impl());
			return false;
		}
		BOOST_CATCH(...)
		{
			node_impl_type::restore(x->impl(), next->impl(), header()->impl());
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename T, typename Meta_Map> inline
	bool modify_rollback_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
							index_node_type* x)
	{
		if(in_place(x->value(), x, Category()))
		{
			return super::modify_rollback_(keyinfo_params, x);
		}

		index_node_type* next = x;
		index_node_type::increment(next);

		node_impl_type::rebalance_for_extract(
			x->impl(), header()->parent(), header()->left(), header()->right());

		BOOST_TRY
		{
			link_info inf;
			if(link_point(key(x->value()), inf, Category())
				&& super::modify_rollback_(keyinfo_params, x))
			{
				node_impl_type::link(x->impl(), inf.side, inf.pos, header()->impl());
				return true;
			}
			node_impl_type::restore(x->impl(), next->impl(), header()->impl());
			return false;
		}
		BOOST_CATCH(...)
		{
			node_impl_type::restore(x->impl(), next->impl(), header()->impl());
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
	/* invariant stuff */

	bool invariant_(void) const
	{
		const org_type& org = org_cast();

		if(org.size() == 0 || org.begin() == org.end())
		{
			if(org.size() != 0 || org.begin() != org.end() ||
				header()->left() != header()->impl() ||
				header()->right() != header()->impl()) 
			{
				return false;
			}
		}
		else
		{
			if(static_cast<size_type>(std::distance(org.begin(), org.end())) != org.size()) 
			{
				return false;
			}

			std::size_t len
				= node_impl_type::black_count(leftmost()->impl(), root()->impl());
			for(const_iterator it = org.begin(), it_end = org.end(); it != it_end; ++it)
			{
				index_node_type* x=it.get_node();
				index_node_type* left_x=index_node_type::from_impl(x->left());
				index_node_type* right_x=index_node_type::from_impl(x->right());

				if(x->color() == boost::multi_index::detail::red)
				{
					if((left_x&&left_x->color() == boost::multi_index::detail::red) ||
						(right_x&&right_x->color() == boost::multi_index::detail::red))
					{
						return false;
					}
				}
				if(left_x && comp_(key(x->value()), key(left_x->value())))
				{
					return false;
				}
				
				if(right_x && comp_(key(right_x->value()), key(x->value())))
				{
					return false;
				}

				if(!left_x && !right_x &&
					node_impl_type::black_count(x->impl(), root()->impl()) != len)
				{
					return false;
				}

				if(!AugmentPolicy::invariant(x->impl()))
				{
					return false;
				}
			}
    
			if(leftmost()->impl()!=node_impl_type::minimum(root()->impl()))
			{
				return false;
			}
			
			if(rightmost()->impl()!=node_impl_type::maximum(root()->impl()))
			{
				return false;
			}
		}

		return super::invariant_();
	}

  
	/* This forwarding function eases things for the boost::mem_fn construct
	* in YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT. Actually,
	* final_check_invariant is already an inherited member function of
	* boost_midx_ordered_index_impl_opt.
	*/
	inline void check_invariant_(void) const 
	{ 
		this->final_check_invariant_(); 
	}
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

protected:
	inline index_node_type* header(void) const { return this->final_header(); }
	inline index_node_type* root(void) const { return index_node_type::from_impl(header()->parent()); }
	inline index_node_type* leftmost(void) const { return index_node_type::from_impl(header()->left()); }
	inline index_node_type* rightmost(void) const { return index_node_type::from_impl(header()->right()); }

private:
	struct link_info
	{
		link_info(void)
			:side(boost::multi_index::detail::to_left)
		{
		}

		boost::multi_index::detail::ordered_index_side side;
		node_impl_pointer pos;
	};

	bool link_point(key_param_type k,link_info& inf, boost::multi_index::detail::ordered_unique_tag)
	{
		index_node_type* y = header();
		index_node_type* x = root();
		bool c = true;
		while(x)
		{
			y = x;
			c = comp_(k, key(x->value()));
			x = index_node_type::from_impl(c? x->left() : x->right());
		}

		index_node_type* yy = y;
		if(c)
		{
			if(yy == leftmost())
			{
				inf.side = boost::multi_index::detail::to_left;
				inf.pos = y->impl();
				return true;
			}
			else 
			{
				index_node_type::decrement(yy);
			}
		}

		if(comp_(key(yy->value()), k))
		{
			inf.side = c? boost::multi_index::detail::to_left : boost::multi_index::detail::to_right;
			inf.pos = y->impl();
			return true;
		}
		else
		{
			inf.pos = yy->impl();
			return false;
		}
	}

	bool link_point(key_param_type k,link_info& inf,boost::multi_index::detail::ordered_non_unique_tag)
	{
		index_node_type* y = header();
		index_node_type* x = root();
		bool c = true;
		while(x)
		{
			y = x;
			c = comp_(k, key(x->value()));
			x = index_node_type::from_impl(c? x->left() : x->right());
		}
		inf.side = c? boost::multi_index::detail::to_left : boost::multi_index::detail::to_right;
		inf.pos = y->impl();
		return true;
	}

	bool lower_link_point(key_param_type k,link_info& inf,boost::multi_index::detail::ordered_non_unique_tag)
	{
		index_node_type* y = header();
		index_node_type* x = root();
		bool c = false;
		while(x)
		{
			y = x;
			c = comp_(key(x->value()), k);
			x = index_node_type::from_impl(c? x->right() : x->left());
		}
		inf.side = c? boost::multi_index::detail::to_right : boost::multi_index::detail::to_left;
		inf.pos = y->impl();
		return true;
	}

	inline bool hinted_link_point(key_param_type k, index_node_type* position, 
									link_info& inf, boost::multi_index::detail::ordered_unique_tag)
	{
		if(position->impl() == header()->left())
		{ 
			if(org_cast().size() > 0 && comp_(k, key(position->value())))
			{
				inf.side = boost::multi_index::detail::to_left;
				inf.pos = position->impl();
				return true;
			}
			else
			{
				return link_point(k, inf, boost::multi_index::detail::ordered_unique_tag());
			}
		} 
		else 
		{
			if(position==header())
			{ 
				if(comp_(key(rightmost()->value()),k))
				{
					inf.side = boost::multi_index::detail::to_right;
					inf.pos = rightmost()->impl();
					return true;
				}
				else 
				{
					return link_point(k, inf, boost::multi_index::detail::ordered_unique_tag());
				}
			} 
			else
			{
				index_node_type* before = position;
				index_node_type::decrement(before);
				if(comp_(key(before->value()), k)
					&& comp_(k, key(position->value())))
				{
					if(before->right() == node_impl_pointer(0))
					{
						inf.side = boost::multi_index::detail::to_right;
						inf.pos = before->impl();
						return true;
					}
					else
					{
						inf.side = boost::multi_index::detail::to_left;
						inf.pos = position->impl();
						return true;
					}
				} 
				else 
				{ 
					return link_point(k, inf, boost::multi_index::detail::ordered_unique_tag()); 
				}
			}
		}
	}

	inline bool hinted_link_point(key_param_type k, index_node_type* position, 
									link_info& inf, boost::multi_index::detail::ordered_non_unique_tag)
	{
		if(position->impl() == header()->left())
		{ 
			if(org_cast().size() > 0 && !comp_(key(position->value()), k))
			{
				inf.side = boost::multi_index::detail::to_left;
				inf.pos = position->impl();
				return true;
			}
			else 
			{
				return lower_link_point(k, inf, boost::multi_index::detail::ordered_non_unique_tag());
			}
		} 
		else
		{ 
			if(position == header())
			{
				if(!comp_(k,key(rightmost()->value())))
				{
					inf.side = boost::multi_index::detail::to_right;
					inf.pos = rightmost()->impl();
					return true;
				}
				else 
				{
					return link_point(k,inf,boost::multi_index::detail::ordered_non_unique_tag());
				}
			} 
			else
			{
				index_node_type* before = position;
				index_node_type::decrement(before);
				if(!comp_(k, key(before->value())))
				{
					if(!comp_(key(position->value()), k))
					{
						if(before->right() == node_impl_pointer(0))
						{
							inf.side = boost::multi_index::detail::to_right;
							inf.pos = before->impl();
							return true;
						}
						else
						{
							inf.side = boost::multi_index::detail::to_left;
							inf.pos = position->impl();
							return true;
						}
					}
					else 
					{ 
						return lower_link_point(k,inf,boost::multi_index::detail::ordered_non_unique_tag()); 
					}
				} 
				else 
				{
					return link_point(k,inf,boost::multi_index::detail::ordered_non_unique_tag());
				}
			}
		}
	}

	inline bool in_place(value_param_type v,index_node_type* x,boost::multi_index::detail::ordered_unique_tag)
	{
		index_node_type* y = 0;
		if(x != leftmost())
		{
			y = x;
			index_node_type::decrement(y);
			if(!comp_(key(y->value()), key(v)))
			{
				return false;
			}
		}

		y = x;
		index_node_type::increment(y);
		return y == header() || comp_(key(v), key(y->value()));
	}

	inline bool in_place(value_param_type v,index_node_type* x,boost::multi_index::detail::ordered_non_unique_tag)
	{
		index_node_type* y = 0;
		if(x != leftmost())
		{
			y = x;
			index_node_type::decrement(y);
			if(comp_(key(v), key(y->value())))
			{
				return false;
			}
		}

		y = x;
		index_node_type::increment(y);
		return y == header() || !comp_(key(y->value()), key(v));
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	inline void detach_iterators(index_node_type* x)
	{
		iterator it = make_iterator(x);
		boost::multi_index::safe_mode::detach_equivalent_iterators(it);
	}
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

protected:

	key_from_value key;
	key_compare    comp_;

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
};

template
<
	typename KeyFromValue, typename Compare,
	typename SuperMeta, typename TagList, 
	typename Category, typename AugmentPolicy
>
class boost_midx_ordered_index_opt
	: public 
		make_augmented_interface_opt
		<
			boost_midx_ordered_index_impl_opt
			<
				KeyFromValue, Compare,
				SuperMeta, TagList, 
				Category, AugmentPolicy
			>,
			AugmentPolicy
		>::type
{
	typedef typename 
		make_augmented_interface_opt
		<
			boost_midx_ordered_index_impl_opt
			<
				KeyFromValue, Compare,
				SuperMeta, TagList, 
				Category, AugmentPolicy
			>,
			AugmentPolicy
		>::type                                     super;

	typedef 
		boost::multi_index::detail::ordered_index
		<
			KeyFromValue, Compare,
			SuperMeta, TagList,
			Category, AugmentPolicy
		>											org_type;

	typedef typename 
		AugmentPolicy::template augmented_interface
		<
			org_type
		>::type                                     org_super;

	
public:
	typedef typename super::ctor_args_list          ctor_args_list;
	typedef typename super::allocator_type          allocator_type;
	typedef typename super::iterator                iterator;

	/* construct/copy/destroy
	* Default and copy ctors are in the protected section as indices are
	* not supposed to be created on their own. No range ctor either.
	*/

private:
	typedef boost_midx_ordered_index_opt this_type;

private:
	boost_midx_ordered_index_opt(void);
	boost_midx_ordered_index_opt(BOOST_RV_REF(boost_midx_ordered_index_opt));
	boost_midx_ordered_index_opt(const boost_midx_ordered_index_opt&);
	~boost_midx_ordered_index_opt(void);
	const boost_midx_ordered_index_opt& operator=( const boost_midx_ordered_index_opt& );
	const boost_midx_ordered_index_opt& operator=( BOOST_RV_REF(boost_midx_ordered_index_opt) );

};

} // namespace yggr
} // namespace safe_container
} // namespace detail

#if defined(YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT)
#	undef YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT
#endif // YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_ORDERED_INDEX_IMPL_OPT_HPP__
