// node_handle_foo_hacking_table.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_106300_NODE_HANDLE_FOO_HACKING_TABLE_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_106300_NODE_HANDLE_FOO_HACKING_TABLE_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_TABLE_HPP__
#	error "this file is inner file, can't include it!"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_TABLE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/detail/unordered_node_algo.hpp>
#include <yggr/container/detail/has_emplace_return.hpp>

#include <boost/unordered_set.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename HackingTableImpl>
class node_handle_foo_hacking_table;

template<typename TableImpl>
class node_handle_foo_hacking_table_impl;

template<typename TableImpl>
class node_handle_foo_hacking_grouped_table_impl;


} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{

// node_algo_selector

template<typename TableImpl, bool is_single>
struct node_algo_selector_impl;

template<template<typename _Types> class TableImpl,
			typename Types>
struct node_algo_selector_impl<TableImpl<Types>, true> // single
{
	typedef node_algo<typename Types::node> type;
};

template<template<typename _Types> class TableImpl,
			typename Types>
struct node_algo_selector_impl<TableImpl<Types>, false> // multi
{
	typedef grouped_node_algo<typename Types::node> type;
};

template<typename TableImpl>
struct node_algo_selector
	: public 
		node_algo_selector_impl
		<
			TableImpl,
			!!has_member_t_emplace_return<TableImpl>::value
		>
{
};

// node_handle_foo_hacking_table
template<template<typename _OrgTableImpl> class HackingTableImpl,
			typename OrgTableImpl>
class node_handle_foo_hacking_table< HackingTableImpl<OrgTableImpl> >
	: public 
		boost::unordered::detail::functions
		<
            typename OrgTableImpl::hasher,
            typename OrgTableImpl::key_equal
		>
{
private:
	typedef node_handle_foo_hacking_table this_type;

protected:
	typedef  HackingTableImpl<OrgTableImpl> hacking_table_impl_type;

public:
	typedef OrgTableImpl org_table_impl_type;
	typedef typename org_table_impl_type::table org_table_type;
	typedef node_handle_foo_hacking_table hacking_table_type;

public:
    typedef typename org_table_type::node node;
    typedef typename org_table_type::key_type key_type;
    typedef typename org_table_type::link_pointer link_pointer;
    typedef typename org_table_type::c_iterator c_iterator;

	typedef typename org_table_type::allocator allocator;
	typedef typename org_table_type::node_allocator node_allocator;
	typedef typename org_table_type::bucket_allocator bucket_allocator;
	typedef typename org_table_type::node_pointer node_pointer;
	typedef typename org_table_type::bucket_pointer bucket_pointer;
	
	typedef typename 
		node_algo_selector
		< 
			org_table_impl_type 
		>::type node_algo_type;

protected:
	YGGR_CONSTEXPR_OR_INLINE org_table_type& org_table_cast(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<org_table_type&>(*this);
	}

	YGGR_CONSTEXPR_OR_INLINE org_table_type& org_table_cast(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<org_table_type&>(*this);
	}

public:

	link_pointer find_previous_node(
        const key_type& k, std::size_t key_hash, std::size_t bucket_index)
    {
        link_pointer prev = this_type::org_table_cast().get_previous_start(bucket_index);
        if(!prev)
		{
            return prev;
        }

        for (;;) 
		{
            if(!prev->next_) 
			{
                return link_pointer();
            }

            std::size_t node_hash = node_algo_type::next_node(prev)->hash_;
            if(this_type::org_table_cast().hash_to_bucket(node_hash) != bucket_index) 
			{
                return link_pointer();
            }

            if(node_hash == key_hash 
				&&this_type::org_table_cast().key_eq()(
                    k, 
					this_type::org_table_cast().get_key(node_algo_type::next_node(prev)->value())))
			{
                return prev;
            }

            prev = node_algo_type::next_for_erase(prev);
        }
    }

	inline node_pointer extract_by_key(const key_type& k)
    {
        if(!this->size_)
		{
            return node_pointer();
        }

        std::size_t key_hash = this_type::org_table_cast().hash(k);
        std::size_t bucket_index = this_type::org_table_cast().hash_to_bucket(key_hash);
        link_pointer prev = this_type::find_previous_node(k, key_hash, bucket_index);
        
		if(!prev) 
		{
            return node_pointer();
        }

        node_pointer n = node_algo_type::extract_first_node(prev);
        --(this_type::org_table_cast().size_);
        this_type::org_table_cast().fix_bucket(bucket_index, prev); 
        n->next_ = link_pointer();

        return n;
    }

public:

    ////////////////////////////////////////////////////////////////////////
    // Members

    boost::unordered::detail::compressed
	<
		bucket_allocator, 
		node_allocator
	> allocators_;
    std::size_t bucket_count_;
    std::size_t size_;
    float mlf_;
    std::size_t max_load_;
    bucket_pointer buckets_;
};

// hacking_iterator
template<typename Iter>
struct hacking_iterator;

template<template<typename _Node> class Iter,
		typename Node>
struct hacking_iterator<Iter<Node> >
	: public
		std::iterator
		<
            std::forward_iterator_tag,
            typename Node::value_type,
            std::ptrdiff_t,
            typename Node::value_type const*,
            typename Node::value_type const&
		>
{
public:
	typedef typename Node::node_pointer node_pointer;

public:
	node_pointer node_;
};


// hacking_iterator_cast
template<template<typename _Node> class Iter, typename Node> YGGR_CONSTEXPR_OR_INLINE
hacking_iterator<Iter<Node> >& 
	hacking_iterator_cast(Iter<Node>& iter) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef hacking_iterator<Iter<Node> > ret_type;
	return reinterpret_cast<ret_type&>(iter);
}

template<template<typename _Node> class Iter, typename Node> YGGR_CONSTEXPR_OR_INLINE
const hacking_iterator<Iter<Node> >& 
	hacking_iterator_cast(const Iter<Node>& iter) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef hacking_iterator<Iter<Node> > ret_type;
		
	return reinterpret_cast<const ret_type&>(iter);
}

// hacking_c_iterator
template<typename CIter>
struct hacking_c_iterator;

template<template<typename _Node> class CIter,
		typename Node>
struct hacking_c_iterator<CIter<Node> >
	: public hacking_iterator<CIter<Node> >
{
};

template<template<typename _Node> class CIter, typename Node> YGGR_CONSTEXPR_OR_INLINE
hacking_c_iterator<CIter<Node> >& 
	hacking_c_iterator_cast(CIter<Node>& citer) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef hacking_c_iterator<CIter<Node> > ret_type;
	return reinterpret_cast<ret_type&>(citer);
}

template<template<typename _Node> class CIter, typename Node> YGGR_CONSTEXPR_OR_INLINE
const hacking_c_iterator<CIter<Node> >& 
	hacking_c_iterator_cast(const CIter<Node>& citer) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef hacking_c_iterator<CIter<Node> > ret_type;
		
	return reinterpret_cast<const ret_type&>(citer);
}


template<template<typename _Types> class TableImpl,
			typename Types>
class node_handle_foo_hacking_table_impl< TableImpl<Types> >
	: public 
		node_handle_foo_hacking_table
		<
			node_handle_foo_hacking_table_impl< TableImpl<Types> > 
		>
{
private:
	typedef node_handle_foo_hacking_table_impl this_type;

protected:
	typedef 
		node_handle_foo_hacking_table
		<
			node_handle_foo_hacking_table_impl< TableImpl<Types> > 
		> base_type;

public:
	typedef TableImpl<Types> org_table_impl_type;
	typedef node_handle_foo_hacking_table_impl hacking_table_impl_type;

	typedef typename org_table_impl_type::table org_table_type;

	typedef typename org_table_impl_type::iterator iterator;
	typedef typename org_table_impl_type::c_iterator c_iterator;
	typedef typename org_table_impl_type::node_pointer node_pointer;

public:
	YGGR_CONSTEXPR_OR_INLINE 
	org_table_impl_type& org_table_impl_cast(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<org_table_impl_type&>(*this);
	}

	YGGR_CONSTEXPR_OR_INLINE 
	const org_table_impl_type& org_table_impl_cast(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<const org_table_impl_type&>(*this);
	}

public:
	node_pointer extract_by_iterator(c_iterator i)
	{
		org_table_impl_type& org_tab_impl = this_type::org_table_impl_cast();

		node_pointer n = hacking_c_iterator_cast(i).node_;
		BOOST_ASSERT(n);
		std::size_t key_hash = n->hash_;
		std::size_t bucket_index = org_tab_impl.hash_to_bucket(key_hash);
		link_pointer prev = org_tab_impl.get_previous_start(bucket_index);
		while (prev->next_ != n) 
		{
			prev = prev->next_;
		}
		prev->next_ = n->next_;
		--base_type::size_;
		org_tab_impl.fix_bucket(bucket_index, prev);
		n->next_ = link_pointer();
		return n;
	}

	template<typename NodeType, typename InsertReturnType> inline
	void move_insert_node_type(NodeType& np, InsertReturnType& result)
	{
		org_table_impl_type& org_tab_impl = this_type::org_table_impl_cast();

		if(np) 
		{
			const key_type& k = org_tab_impl.get_key(np.ptr_->value());
			std::size_t key_hash = org_tab_impl.hash(k);
			node_pointer pos = org_tab_impl.find_node(key_hash, k);

			if(pos)
			{
				result.node = boost::move(np);
				result.position = iterator(pos);
			} 
			else 
			{
				org_tab_impl.reserve_for_insert(base_type::size_ + 1);
				result.position = iterator(org_tab_impl.add_node(np.ptr_, key_hash));
				result.inserted = true;
				np.ptr_ = node_pointer();
			}
		}
	}

public:
	template<typename NodeType> inline
    iterator move_insert_node_type_with_hint(c_iterator hint, NodeType& np)
    {
		org_table_impl_type& org_tab_impl = this_type::org_table_impl_cast();

        if(!np)
		{
            return iterator();
        }
        const key_type& k = org_tab_impl.get_key(np.ptr_->value());
        if(hacking_c_iterator_cast(hint).node_ 
			&& org_tab_impl.key_eq()(k, org_tab_impl.get_key(*hint))) 
		{
            return iterator(hacking_c_iterator_cast(hint).node_);
        }

        std::size_t key_hash = org_tab_impl.hash(k);
        node_pointer pos = org_tab_impl.find_node(key_hash, k);
        
		if(!pos) 
		{
            org_tab_impl.reserve_for_insert(base_type::size_ + 1);
            pos = org_tab_impl.add_node(np.ptr_, key_hash);
            np.ptr_ = node_pointer();
        }

        return iterator(pos);
    }
};

// node_handle_foo_hacking_grouped_table_impl

template<template<typename _Types> class TableImpl,
			typename Types>
class node_handle_foo_hacking_grouped_table_impl< TableImpl<Types> >
	: public 
		node_handle_foo_hacking_table
		<
			node_handle_foo_hacking_grouped_table_impl< TableImpl<Types> > 
		>
{
private:
	typedef node_handle_foo_hacking_grouped_table_impl this_type;

protected:
	typedef 
		node_handle_foo_hacking_table
		<
			node_handle_foo_hacking_grouped_table_impl< TableImpl<Types> > 
		> base_type;

public:
	typedef TableImpl<Types> org_table_impl_type;
	typedef node_handle_foo_hacking_grouped_table_impl hacking_table_impl_type;

	typedef typename org_table_impl_type::table org_table_type;

	typedef typename org_table_impl_type::iterator iterator;
	typedef typename org_table_impl_type::c_iterator c_iterator;
	typedef typename org_table_impl_type::node_pointer node_pointer;

	typedef typename base_type::node_algo_type node_algo_type;

public:
	YGGR_CONSTEXPR_OR_INLINE 
	org_table_impl_type& org_table_impl_cast(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<org_table_impl_type&>(*this);
	}

	YGGR_CONSTEXPR_OR_INLINE 
	const org_table_impl_type& org_table_impl_cast(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<const org_table_impl_type&>(*this);
	}

public:

	node_pointer extract_by_iterator(c_iterator n)
	{
		org_table_impl_type& org_tab_impl = this_type::org_table_impl_cast();

        node_pointer i = hacking_c_iterator_cast(n).node_;
        BOOST_ASSERT(i);
        node_pointer j(node_algo_type::next_node(i));
        std::size_t bucket_index = org_tab_impl.hash_to_bucket(i->hash_);
        // Split the groups containing 'i' and 'j'.
        // And get the pointer to the node before i while
        // we're at it.
        link_pointer prev = node_algo_type::split_groups(i, j);

        // If we don't have a 'prev' it means that i is at the
        // beginning of a block, so search through the blocks in the
        // same bucket.
        if(!prev) 
		{
            prev = org_tab_impl.get_previous_start(bucket_index);
            while (prev->next_ != i) 
			{
                prev = node_algo_type::next_for_erase(prev);
            }
        }

        prev->next_ = i->next_;
        --base_type::size_;
        org_tab_impl.fix_bucket(bucket_index, prev);
        i->next_ = link_pointer();

        return i;
	}

	template<typename NodeType> inline
	iterator move_insert_node_type(NodeType& np)
    {
		org_table_impl_type& org_tab_impl = this_type::org_table_impl_cast();

        iterator result;

        if(np) 
		{
            const key_type& k = org_tab_impl.get_key(np.ptr_->value());
            std::size_t key_hash = org_tab_impl.hash(k);
            node_pointer pos = org_tab_impl.find_node(key_hash, k);
            org_tab_impl.reserve_for_insert(base_type::size_ + 1);
            result = iterator(org_tab_impl.add_node(np.ptr_, key_hash, pos));
            np.ptr_ = node_pointer();
        }

        return result;
    }

	template<typename NodeType> inline
    iterator move_insert_node_type_with_hint(c_iterator hint, NodeType& np)
    {
		org_table_impl_type& org_tab_impl = this_type::org_table_impl_cast();

        iterator result;

        if(np) 
		{
            const key_type& k = org_tab_impl.get_key(np.ptr_->value());

            if(hacking_c_iterator_cast(hint).node_ 
				&& org_tab_impl.key_eq()(k, org_tab_impl.get_key(*hint))) 
			{
                org_tab_impl.reserve_for_insert(base_type::size_ + 1);
                result = 
					iterator(
						org_tab_impl.add_using_hint(
							np.ptr_, hacking_c_iterator_cast(hint).node_));
            }
			else 
			{
                std::size_t key_hash = org_tab_impl.hash(k);
                node_pointer pos = org_tab_impl.find_node(key_hash, k);
                org_tab_impl.reserve_for_insert(base_type::size_ + 1);
                result = iterator(org_tab_impl.add_node(np.ptr_, key_hash, pos));
            }

            np.ptr_ = node_pointer();
        }

        return result;
    }
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_106300_NODE_HANDLE_FOO_HACKING_TABLE_HPP__
