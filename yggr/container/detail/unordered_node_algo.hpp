// unordered_node_algo.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_UNORDERED_NODE_ALGO_HPP__
#define __YGGR_CONTAINER_DETAIL_UNORDERED_NODE_ALGO_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace container
{
namespace detail
{


template<typename N> 
struct node_algo
{
private:
	typedef node_algo this_type;

public:
    typedef typename N::node_pointer node_pointer;
    typedef typename N::link_pointer link_pointer;
	typedef typename N::link_pointer bucket_pointer;

public:

    inline static node_pointer next_node(link_pointer n) YGGR_NOEXCEPT_OR_NOTHROW
    {
        return static_cast<node_pointer>(n->next_);
    }

   inline static link_pointer next_for_erase(link_pointer prev) YGGR_NOEXCEPT_OR_NOTHROW
    {
        return prev->next_;
    }

    inline static node_pointer extract_first_node(link_pointer prev) YGGR_NOEXCEPT_OR_NOTHROW
    {
        node_pointer n = this_type::next_node(prev);
        prev->next_ = n->next_;
        return n;
    }

    inline static link_pointer split_groups(node_pointer, node_pointer) YGGR_NOEXCEPT_OR_NOTHROW
    {
        return link_pointer();
    }
};

template <typename N> 
struct grouped_node_algo
{
private:
	typedef grouped_node_algo this_type;

public:
    typedef typename N::node_pointer node_pointer;
    typedef typename N::link_pointer link_pointer;
	typedef typename N::link_pointer bucket_pointer;

public:
    inline static node_pointer next_node(link_pointer n) YGGR_NOEXCEPT_OR_NOTHROW
    {
        return static_cast<node_pointer>(n->next_);
    }

    inline static link_pointer next_for_erase(link_pointer prev) YGGR_NOEXCEPT_OR_NOTHROW
    {
        return static_cast<node_pointer>(prev->next_)->group_prev_;
    }

    // Adds node 'n' to the group containing 'pos'.
    // If 'pos' is the first node in group, add to the end of the group,
    // otherwise add before 'pos'. Other versions will probably behave
    // differently.
	inline static void add_to_node_group(node_pointer n, node_pointer pos)
    {
		n->next_ = pos->group_prev_->next_;
		n->group_prev_ = pos->group_prev_;
		pos->group_prev_->next_ = n;
		pos->group_prev_ = n;
    }

    inline static node_pointer extract_first_node(link_pointer prev) YGGR_NOEXCEPT_OR_NOTHROW
    {
        node_pointer n = this_type::next_node(prev);
        if(n->group_prev_ != n) 
		{
            node_pointer next = this_type::next_node(n);
            next->group_prev_ = n->group_prev_;
            n->group_prev_ = n;
        }
        prev->next_ = n->next_;
        return n;
    }

    // Split the groups containing 'i' and 'j' so that they can
    // be safely erased/extracted.
   inline static link_pointer split_groups(node_pointer i, node_pointer j) YGGR_NOEXCEPT_OR_NOTHROW
    {
        node_pointer prev = i->group_prev_;
        if(prev->next_ != i)
            prev = node_pointer();

        if(j) 
		{
            node_pointer first = j;
            while (first != i && first->group_prev_->next_ == first) 
			{
                first = first->group_prev_;
            }

            boost::swap(first->group_prev_, j->group_prev_);
            if(first == i)
                return prev;
        }

        if(prev) 
		{
            node_pointer first = prev;
            while (first->group_prev_->next_ == first) 
			{
                first = first->group_prev_;
            }
            boost::swap(first->group_prev_, i->group_prev_);
        }

        return prev;
    }
};


} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_UNORDERED_NODE_ALGO_HPP__
