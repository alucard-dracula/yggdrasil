// node_handle_foo_hacking_midx_sequenced_index.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105600_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105600_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/multi_index_sequenced_index.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__

#include <yggr/container/detail/node_handle_traits.hpp>
#include <yggr/container/detail/boost_midx_check_helper.hpp>
#include <yggr/container/detail/cast_midx_base_type.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_sequenced_index_decl.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>

#if !defined(YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT)
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)

#	define YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT \
		::boost::multi_index::detail::scope_guard BOOST_JOIN(check_invariant_, __LINE__) = \
			::boost::multi_index::detail::make_obj_guard(*this, &node_handle_foo_hacking_midx_sequenced_index::check_invariant_); \
	BOOST_JOIN(check_invariant_, __LINE__).touch();

#else

#	define YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#endif //YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT

namespace yggr
{
namespace container
{
namespace detail
{

template<typename SuperMeta,typename TagList>
class node_handle_foo_hacking_midx_sequenced_index :
	BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS SuperMeta::type

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	, public 
		boost::multi_index::safe_mode::safe_container
		<
			node_handle_foo_hacking_midx_sequenced_index<SuperMeta, TagList> 
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
		boost::multi_index::detail::sequenced_index
		<
			typename nth_layer_native_cast<SuperMeta>::type,
			TagList
		>														org_type;

public:

	typedef typename SuperMeta::type							super;

public:
	typedef 
		boost::multi_index::detail::sequenced_index_node
		<
			typename super::index_node_type
		>														index_node_type;

protected:
	typedef typename index_node_type::impl_type					node_impl_type;
 
public:
	/* types */

	typedef typename org_type::value_type						value_type;
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
			node_handle_foo_hacking_midx_sequenced_index
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

private:
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	typedef 
		boost::multi_index::safe_mode::safe_container
		<
			node_handle_foo_hacking_midx_sequenced_index
		>														safe_super;
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	typedef typename 
		boost::call_traits<value_type>::param_type				value_param_type;

private:
	typedef node_handle_foo_hacking_midx_sequenced_index this_type;

private:
	node_handle_foo_hacking_midx_sequenced_index(void);
	node_handle_foo_hacking_midx_sequenced_index(BOOST_RV_REF(this_type));
	node_handle_foo_hacking_midx_sequenced_index(const this_type&);
	~node_handle_foo_hacking_midx_sequenced_index(void);
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
	inline node_type extract(const_iterator position)
	{
		BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
		YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
		return 
			this->final_extract_(
				static_cast<final_node_type*>(position.get_node()));
	}

	inline insert_return_type insert(const_iterator position, BOOST_RV_REF(node_type) nh)
	{
		BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
		if(nh)
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(*this, nh);
		}

		YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_nh_(nh);
		if(p.second && position.get_node() != header())
		{
			this_type::relink(position.get_node(), p.first);
		}

		return insert_return_type(p.second, make_iterator(p.first), boost::move(nh));
	}

BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)

	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node, &org_cast());
	}

	inline const_iterator make_iterator(index_node_type* node) const
	{
		return const_iterator(node, const_cast<node_handle_foo_hacking_midx_sequenced_index*>(&org_cast()));
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
		unlink(x);
		super::extract_(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename Tag> inline
	final_node_type* insert_(value_param_type v, final_node_type*& x, Tag tag)
	{
		final_node_type* res = super::insert_(v, x, tag);
		if(res == x)
		{
			link(static_cast<index_node_type*>(x));
		}
		return res;
	}

	template<typename Tag> inline
	final_node_type* insert_(value_param_type v, index_node_type* position, final_node_type*& x, Tag tag)
	{
		final_node_type* res = super::insert_(v, position, x, tag);
		if(res == x)
		{
			link(static_cast<index_node_type*>(x));
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
				header()->next() != header()->impl() ||
				header()->prior() != header()->impl()) 
			{
				return false;
			}
		}
		else
		{
			size_type s = 0;
			for(const_iterator it = org.begin(), it_end = org.end();
					it != it_end; ++it, ++s)
			{
				if(it.get_node()->next()->prior() != it.get_node()->impl()) 
				{
					return false;
				}
				
				if(it.get_node()->prior()->next() != it.get_node()->impl())
				{
					return false;
				}
			}
			if(s != org.size())
			{
				return false;
			}
		}

		return super::invariant_();
	}

	/* This forwarding function eases things for the boost::mem_fn construct
	* in YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT. Actually,
	* final_check_invariant is already an inherited member function of index.
	*/
	inline void check_invariant_(void) const
	{
		this->final_check_invariant_();
	}
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

private:
	inline index_node_type* header() const { return this->final_header(); }

	inline void link(index_node_type* x)
	{
		node_impl_type::link(x->impl(),header()->impl());
	};

	inline static void unlink(index_node_type* x)
	{
		node_impl_type::unlink(x->impl());
	}

	inline static void relink(index_node_type* position,index_node_type* x)
	{
		node_impl_type::relink(position->impl(), x->impl());
	}

	inline static void relink(index_node_type* position,index_node_type* first,index_node_type* last)
	{
		node_impl_type::relink(
			position->impl(), first->impl(), last->impl());
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	inline void detach_iterators(index_node_type* x)
	{
		iterator it = make_iterator(x);
		boost::multi_index::safe_mode::detach_equivalent_iterators(it);
	}
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

 
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
};

template<typename S, typename T>
struct node_handle_traits_impl_custom< boost::multi_index::detail::sequenced_index<S, T> >
{
private:
	typedef boost::multi_index::detail::sequenced_index<S, T> native_type;
	typedef typename index_hacking_cast<native_type>::type hacking_type;

public:
	typedef typename hacking_type::node_type node_type;
	typedef typename hacking_type::insert_return_type insert_return_type;
	typedef typename hacking_type::insert_return_type hint_insert_return_type;
};

template<typename TagList>
struct node_handle_foo_hacking_midx_sequenced
{
	BOOST_STATIC_ASSERT(boost::multi_index::detail::is_tag<TagList>::value);

	typedef typename boost::multi_index::sequenced<TagList> org_type;

	template<typename Super>
	struct node_class
	{
		typedef 
			boost::multi_index::detail::sequenced_index_node
			<
				Super
			> type;
	};

	template<typename SuperMeta>
	struct index_class
	{
		typedef 
			node_handle_foo_hacking_midx_sequenced_index
			<
				SuperMeta, 
				typename TagList::type
			> type;
	};
};

} // namespace detail
} // namespace container
} // namespace yggr

#if defined(YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT)
#	undef YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT
#endif // YGGR_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105600_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__
