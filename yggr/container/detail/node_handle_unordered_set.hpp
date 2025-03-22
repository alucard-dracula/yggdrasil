// node_handle_unordered_set.hpp 

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_UNORDERED_SET_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_UNORDERED_SET_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/unordered/unordered_set.hpp>

#if (BOOST_VERSION < 106400)

#include <yggr/move/move.hpp>
#include <yggr/support/operator_t.hpp>

#include <yggr/container/detail/node_tmp.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename N, typename T, typename A> 
class node_handle_uset
{
	BOOST_MOVABLE_BUT_NOT_COPYABLE(node_handle_uset) // not using this_type cpp20 has issue

	typedef typename 
		::boost::unordered::detail::rebind_wrap
		<
			A, 
			T
		>::type value_allocator;
	
	typedef 
		::boost::unordered::detail::allocator_traits
		<
			value_allocator
		> value_allocator_traits;
	
	typedef N node;
	
	typedef typename 
		::boost::unordered::detail::rebind_wrap
		<
			A, 
			node
		>::type node_allocator;
	
	typedef 
		::boost::unordered::detail::allocator_traits
		<
			node_allocator
		> node_allocator_traits;
	
	typedef typename node_allocator_traits::pointer node_pointer;

	// appended
	typedef typename N::link_pointer link_pointer;

public:
	typedef T value_type;
	typedef A allocator_type;

private:
	typedef node_handle_uset this_type;

public:
	YGGR_CONSTEXPR 
	node_handle_uset(void) YGGR_NOEXCEPT 
		: ptr_(), has_alloc_(false)
	{
	}

	/*BOOST_CONSTEXPR */ 
	node_handle_uset(node_pointer ptr, allocator_type const& a)
		: ptr_(ptr), has_alloc_(false)
	{
		if(ptr_) 
		{
			new (static_cast<void*>(&alloc_)) value_allocator(a);
			has_alloc_ = true;
		}
	}

	node_handle_uset(BOOST_RV_REF(this_type) n) YGGR_NOEXCEPT
		: ptr_(n.ptr_), has_alloc_(false)
	{
		if(n.has_alloc_) 
		{
			new (static_cast<void*>(&alloc_)) value_allocator(boost::move(n.alloc_.value()));
			has_alloc_ = true;
			n.ptr_ = node_pointer();
			n.alloc_.value_ptr()->~value_allocator();
			n.has_alloc_ = false;
		}
	}

	~node_handle_uset(void)
	{
		if(has_alloc_)
		{
			if(ptr_)
			{
				node_allocator node_alloc(alloc_.value());
				node_tmp<node_allocator> tmp(ptr_, node_alloc);
			}

			alloc_.value_ptr()->~value_allocator();
		}
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) n)
	{
		BOOST_ASSERT(
			(!has_alloc_) 
			|| value_allocator_traits::propagate_on_container_move_assignment::value 
			|| (n.has_alloc_ && alloc_.value() == n.alloc_.value()));

		if(ptr_)
		{
			node_allocator node_alloc(alloc_.value());
			node_tmp<node_allocator> tmp(
				ptr_, node_alloc);
			ptr_ = node_pointer();
		}

		if(has_alloc_)
		{
			alloc_.value_ptr()->~value_allocator();
			has_alloc_ = false;
		}

		if(!has_alloc_ && n.has_alloc_) 
		{
			this_type::move_allocator(n);
		}

		ptr_ = n.ptr_;
		n.ptr_ = node_pointer();

		return *this;
	}

public:
	inline value_type& value(void) const 
	{
		return ptr_->value(); 
	}

	inline allocator_type get_allocator(void) const 
	{ 
		return *alloc_; 
	}

	inline bool operator!(void) const YGGR_NOEXCEPT 
	{
		return !ptr_;
	}

	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const YGGR_NOEXCEPT
    {
        return !(this_type::operator!());
    }

	inline bool empty(void) const YGGR_NOEXCEPT 
	{
		return !ptr_;
	}

	inline void swap(this_type& n) 
		YGGR_NOEXCEPT_IF(
			value_allocator_traits::propagate_on_container_swap::value
			|| value_allocator_traits::is_always_equal::value )
	{
		if(!has_alloc_) 
		{
			if(n.has_alloc_) 
			{
				move_allocator(n);
			}
		}
		else if(!n.has_alloc_) 
		{
			n.move_allocator(*this);
		} 
		else 
		{
			this_type::swap_impl(
				n, 
				boost::unordered::detail::integral_constant
				<
					bool,
					value_allocator_traits::propagate_on_container_swap::value
				>());
		}

		boost::swap(ptr_, n.ptr_);
	}

private:
	inline void move_allocator(this_type& n)
	{
		new (static_cast<void*>(&alloc_)) value_allocator(boost::move(n.alloc_.value()));
		n.alloc_.value_ptr()->~value_allocator();
		has_alloc_ = true;
		n.has_alloc_ = false;
	}

	YGGR_CONSTEXPR_OR_INLINE 
	void swap_impl(this_type&, boost::unordered::detail::false_type) YGGR_NOEXCEPT_OR_NOTHROW
	{
	}

	inline void swap_impl(this_type& n, boost::unordered::detail::true_type)
	{
		boost::swap(alloc_, n.alloc_);
	}

public:
	node_pointer ptr_;
	bool has_alloc_;
	boost::unordered::detail::value_base<value_allocator> alloc_;
};

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{
namespace swap_support
{

template <typename N, typename T, typename A> inline
void swap(node_handle_uset<N, T, A>& l, node_handle_uset<N, T, A>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace detail
} // namespace container
} // namespace yggr

namespace std
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace boost


#endif // (BOOST_VERSION < 106400)

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_UNORDERED_SET_HPP__
