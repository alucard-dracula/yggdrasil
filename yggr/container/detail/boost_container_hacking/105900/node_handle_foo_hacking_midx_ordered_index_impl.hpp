// node_handle_foo_hacking_midx_ordered_index_impl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_IMPL_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_IMPL_HPP__

#if !(defined(__YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_HPP__) \
		|| defined(__YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_RANKED_INDEX_HPP__) )
#	error "this file is inner file, can't include it"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_HPP__

#include <yggr/container/detail/node_handle_traits.hpp>
#include <yggr/container/detail/boost_midx_check_helper.hpp>
#include <yggr/container/detail/cast_midx_base_type.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_ordered_index_decl.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>
#include <yggr/mplex/static_assert.hpp>

#if !defined(YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT)
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)

#	define YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT \
		::boost::multi_index::detail::scope_guard BOOST_JOIN(check_invariant_, __LINE__) = \
			::boost::multi_index::detail::make_obj_guard( \
				*this, &node_handle_foo_hacking_midx_ordered_index_impl::check_invariant_); \
		BOOST_JOIN(check_invariant_, __LINE__).touch();

#else

#	define YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#endif //YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT

namespace yggr
{
namespace container
{
namespace detail
{

/* node_handle_foo_hacking_midx_ordered_index_impl adds a layer of ordered indexing to a given Super */

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
class node_handle_foo_hacking_midx_ordered_index;

// !!! gcc not support direct BOOST_MPL_ASSERT((false)) and static_assert(false), so not impl it !!!

#if defined(_MSC_VER)

template<typename T, typename AP>
struct make_augmented_interface_hacking
{
	/*
	*	if a compilation error occurs here, 
	*	it means that the template specialization of an AugmentPolicy is missing
	*/
	BOOST_MPL_ASSERT((boost::mpl::false_));
};

#else

template<typename T, typename AP>
struct make_augmented_interface_hacking;

#endif // _MSC_VER

template
<
	typename KeyFromValue, typename Compare,
	typename SuperMeta, typename TagList, 
	typename Category, typename AugmentPolicy
>
class node_handle_foo_hacking_midx_ordered_index_impl :
	BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS SuperMeta::type

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	,public 
		boost::multi_index::safe_mode::safe_container
		<
			node_handle_foo_hacking_midx_ordered_index_impl
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
			typename nth_layer_native_cast<SuperMeta>::type,
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

	typedef typename super::final_node_handle_type				node_type;
	typedef 
		insert_return_type_base
		<
			iterator,
			node_type
		>														insert_return_type;

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
			node_handle_foo_hacking_midx_ordered_index
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
			node_handle_foo_hacking_midx_ordered_index_impl
		>														safe_super;
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	typedef typename 
		boost::call_traits<value_type>::param_type				value_param_type;
	typedef typename 
		boost::call_traits<key_type>::param_type				key_param_type;

private:
	typedef node_handle_foo_hacking_midx_ordered_index_impl this_type;

private:
	node_handle_foo_hacking_midx_ordered_index_impl(void);
	node_handle_foo_hacking_midx_ordered_index_impl(BOOST_RV_REF(this_type));
	node_handle_foo_hacking_midx_ordered_index_impl(const this_type&);
	~node_handle_foo_hacking_midx_ordered_index_impl(void);
	const this_type& operator=( const this_type& );
	const this_type& operator=( BOOST_RV_REF(this_type) );

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

	inline insert_return_type insert(BOOST_RV_REF(node_type) nh)
	{
		if(nh)
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(*this, nh);
		}
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_nh_(nh);
		return insert_return_type(p.second, make_iterator(p.first), boost::move(nh));
	}

	inline iterator insert(const_iterator position,BOOST_RV_REF(node_type) nh)
	{
		BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, *this);
		if(nh) 
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(*this, nh);
		}
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT
		std::pair<final_node_type*, bool> p = 
			this->final_insert_nh_(nh, static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	node_type extract(const_iterator position)
	{
		BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, *this);
		YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT;
		return 
			this->final_extract_(
				static_cast<final_node_type*>(position.get_node()));
	}

	node_type extract(key_param_type x)
	{
		iterator position = org_cast().lower_bound(x);

#if defined(_DEBUG)
		if(position == org_cast().end() || this_type::comp_(x, key(*position)))
		{
			return node_type();
		}
		else
		{
			return this_type::extract(position);
		}
#else
		return 
			(position == org_cast().end() || this_type::comp_(x, key(*position)))?
			node_type() : this_type::extract(position);
#endif // _DEBUG
	}

BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)

	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node, &org_cast());
	}

	inline const_iterator make_iterator(index_node_type* node) const
	{
		return const_iterator(node,const_cast<node_handle_foo_hacking_midx_ordered_index_impl*>(&org_cast()));
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
		node_impl_type::rebalance_for_erase(
			x->impl(),header()->parent(),header()->left(),header()->right());
		super::extract_(x);

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#	endif //BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename Tag> inline
	final_node_type* insert_(value_param_type v, final_node_type*& x, Tag tag)
	{
		link_info inf;
		if(!link_point(key(v), inf, Category()))
		{
			return static_cast<final_node_type*>(index_node_type::from_impl(inf.pos));
		}

		final_node_type* res = super::insert_(v, x, tag);
		if(res == x)
		{
			node_impl_type::link(
				static_cast<index_node_type*>(x)->impl(),
				inf.side,
				inf.pos,
				header()->impl());
		}
		return res;
	}

	template<typename Tag> inline
	final_node_type* insert_(value_param_type v, index_node_type* position, final_node_type*& x, Tag tag)
	{
		link_info inf;
		if(!hinted_link_point(key(v), position, inf, Category()))
		{
			return static_cast<final_node_type*>(index_node_type::from_impl(inf.pos));
		}

		final_node_type* res = super::insert_(v, position, x, tag);
		if(res == x)
		{
			node_impl_type::link(
				static_cast<index_node_type*>(x)->impl(),
				inf.side,
				inf.pos,
				header()->impl());
		}
		return res;
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
	* node_handle_foo_hacking_midx_ordered_index_impl.
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

// node_handle_foo_hacking_midx_ordered_index

template
<
	typename KeyFromValue, typename Compare,
	typename SuperMeta, typename TagList, 
	typename Category, typename AugmentPolicy
>
class node_handle_foo_hacking_midx_ordered_index
	: public 
		make_augmented_interface_hacking
		<
			node_handle_foo_hacking_midx_ordered_index_impl
			<
				KeyFromValue, Compare,
				SuperMeta, TagList, 
				Category, AugmentPolicy
			>,
			AugmentPolicy
		>::type
{
	typedef typename 
		make_augmented_interface_hacking
		<
			node_handle_foo_hacking_midx_ordered_index_impl
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
	typedef node_handle_foo_hacking_midx_ordered_index this_type;

private:
	node_handle_foo_hacking_midx_ordered_index(void);
	node_handle_foo_hacking_midx_ordered_index(BOOST_RV_REF(this_type));
	node_handle_foo_hacking_midx_ordered_index(const this_type&);
	~node_handle_foo_hacking_midx_ordered_index(void);
	const this_type& operator=( const this_type& );
	const this_type& operator=( BOOST_RV_REF(this_type) );

};

template<typename K, typename P, typename S, typename T, typename C, typename A>
struct node_handle_traits_impl_custom< boost::multi_index::detail::ordered_index<K, P, S, T, C, A> >
{
private:
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> native_type;
	typedef typename index_hacking_cast<native_type>::type hacking_type;

public:
	typedef typename hacking_type::node_type node_type;
	typedef typename hacking_type::insert_return_type insert_return_type;
	typedef typename hacking_type::iterator hint_insert_return_type;
};

} // namespace yggr
} // namespace container
} // namespace detail

#if defined(YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT)
#	undef YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT
#endif // YGGR_MULTI_INDEX_ORD_INDEX_CHECK_INVARIANT

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_IMPL_HPP__
