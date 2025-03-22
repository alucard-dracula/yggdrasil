// node_handle_midx.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_MIDX_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_MIDX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/aligned_storage.hpp>

#include <yggr/move/move.hpp>
#include <yggr/support/operator_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/container/allocator_traits.hpp>
#include <boost/multi_index_container_fwd.hpp>

//#include <boost/type_traits/aligned_storage.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/is_base_of.hpp>

#if !(BOOST_VERSION < 107400)
#	include <boost/multi_index/detail/node_handle.hpp>
#endif // !(BOOST_VERSION < 107400)

#include <algorithm>

namespace yggr
{
namespace container
{
namespace detail
{

//node_handle_midx_inner_alloc_move_setter
template<typename Alloc,
			bool is_propagate_on_container_move_assignment
				= boost::container::allocator_traits<Alloc>::propagate_on_container_move_assignment::value>
struct node_handle_midx_inner_alloc_move_setter_impl;

template<typename Alloc>
struct node_handle_midx_inner_alloc_move_setter_impl<Alloc, true>
{
public:
	template<typename T1, typename T2> inline 
	void operator()(T1& l, BOOST_RV_REF(T2) r) const
	{
		l = boost::move(r);
	}
};

template<typename Alloc>
struct node_handle_midx_inner_alloc_move_setter_impl<Alloc, false>
{
public:
	template<typename T1, typename T2> inline 
	void operator()(T1&, BOOST_RV_REF(T2)) const
	{
	}
};

template<typename Alloc>
struct node_handle_midx_inner_alloc_move_setter
	: public node_handle_midx_inner_alloc_move_setter_impl<Alloc>
{
};

//node_handle_midx_inner_alloc_swapper
template<typename Alloc,
			bool is_propagate_on_container_swap
				= boost::container::allocator_traits<Alloc>::propagate_on_container_swap::value>
struct node_handle_midx_inner_alloc_swapper_impl;

template<typename Alloc>
struct node_handle_midx_inner_alloc_swapper_impl<Alloc, true>
{
public:
	template<typename T1, typename T2> inline 
	void operator()(T1& l, T2& r) const
	{
		using std::swap;
		swap(l, r);
	}
};

template<typename Alloc>
struct node_handle_midx_inner_alloc_swapper_impl<Alloc, false>
{
public:
	template<typename T1, typename T2> inline 
	void operator()(T1&, T2&) const
	{
	}
};

template<typename Alloc>
struct node_handle_midx_inner_alloc_swapper
	: public node_handle_midx_inner_alloc_swapper_impl<Alloc>
{
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

template<typename Node, typename Allocator>
class node_handle_midx
{
public:
	typedef Node										index_node_type;
	typedef typename index_node_type::value_type		value_type;
	typedef Allocator									allocator_type;

private:
	typedef boost::container::allocator_traits<allocator_type> alloc_traits;

private:
	typedef node_handle_midx this_type;

public:
	node_handle_midx(void) YGGR_NOEXCEPT
		: node(0)
	{
	}

	node_handle_midx(BOOST_RV_REF(this_type) x) YGGR_NOEXCEPT
		: node(x.node)
	{
		if(!x.empty())
		{
			move_construct_allocator(boost::move(x));
			x.destroy_allocator();
			x.node = 0;
		}
	}

	~node_handle_midx(void)
	{
		if(!this_type::empty())
		{
			this_type::delete_node();
			this_type::destroy_allocator();
		}
	}


public:
	this_type& operator=(BOOST_RV_REF(this_type) x)
	{
		if(this == boost::addressof(x))
		{
			return *this;
		}

		if(!this_type::empty())
		{
			this_type::delete_node();
			if(!x.empty())
			{
				node_handle_midx_inner_alloc_move_setter<allocator_type> setter;
				setter(*this_type::allocator_ptr(), boost::move(*x.allocator_ptr()));
				x.destroy_allocator();
			}
			else
			{
				this_type::destroy_allocator();
			}
		}
		else if(!x.empty())
		{
			this_type::move_construct_allocator(boost::move(x));
			x.destroy_allocator();
		}

		node = x.node;
		x.node = 0;
		
		return *this;
	}

	void swap(this_type& x) BOOST_NOEXCEPT_IF(
								alloc_traits::propagate_on_container_swap::value||
								alloc_traits::is_always_equal::value)
	{
		if(!this_type::empty())
		{
			if(!x.empty())
			{
				node_handle_midx_inner_alloc_swapper<allocator_type> swapper;
				swapper(*this_type::allocator_ptr(), *x.allocator_ptr());
			}
			else
			{
				x.move_construct_allocator(boost::move(*this));
				this_type::destroy_allocator();
			}
		}
		else if(!x.empty())
		{
			this_type::move_construct_allocator(boost::move(x));
			x.destroy_allocator();
		}
		std::swap(node,x.node);
	}

public:
	inline value_type& value(void) const
	{
		return node->value();
	}

	inline allocator_type& get_allocator(void)
	{
		assert(allocator_ptr());
		return *allocator_ptr();
	}

	inline const allocator_type& get_allocator(void) const
	{
		assert(allocator_ptr());
		return *allocator_ptr();
	}

	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const YGGR_NOEXCEPT
	{
		return !!node;
	}

	YGGR_ATTRIBUTE_NODISCARD 
	inline bool empty(void) const YGGR_NOEXCEPT
	{
		return !node;
	}

private:
	BOOST_MOVABLE_BUT_NOT_COPYABLE(node_handle_midx) // not using this_type cpp20 has issue

private:
	template <typename V, typename I, typename A>
	friend class boost::multi_index::multi_index_container;

public:
	node_handle_midx(index_node_type* node_, const allocator_type& al)
		:node(node_)
	{
		::new (static_cast<void*>(allocator_ptr())) allocator_type(al);
	}

	inline void release_node(void)
	{
		if(!this_type::empty())
		{
			node = 0;
			destroy_allocator();
		}
	}

#include <yggr/container/detail/ignore_wstrict_aliasing.hpp>

	inline const allocator_type* allocator_ptr(void) const
	{
		return reinterpret_cast<const allocator_type*>(&space);
	}

	allocator_type* allocator_ptr(void)
	{
		return reinterpret_cast<allocator_type*>(&space);
	}

#include <yggr/container/detail/restore_wstrict_aliasing.hpp>

	inline void move_construct_allocator(BOOST_RV_REF(this_type) x)
	{
		::new (static_cast<void*>(allocator_ptr()))
			allocator_type(boost::move(*x.allocator_ptr()));
	}

	inline void destroy_allocator(void)
	{
		allocator_ptr()->~allocator_type();
	}

	inline void delete_node(void)
	{

#if (BOOST_VERSION < 107000)
		typedef typename
			boost::detail::allocator::rebind_to
			<
				allocator_type,
				index_node_type
			>::type node_allocator;

#else

		typedef typename
			boost::multi_index::detail::rebind_alloc_for
			<
				allocator_type,
				index_node_type
			>::type node_allocator;

#endif // BOOST_VERSION < 107000

		typedef boost::container::allocator_traits<node_allocator> node_alloc_traits;
		typedef typename node_alloc_traits::pointer node_pointer;

		alloc_traits::destroy(*allocator_ptr(), boost::addressof(node->value()));
		node_allocator nal(*allocator_ptr());
		node_alloc_traits::deallocate(nal, static_cast<node_pointer>(node), 1);
	}

public:
	index_node_type* node;
	typename 
		boost::aligned_storage
		<
			sizeof(allocator_type),
			boost::alignment_of<allocator_type>::value
		>::type space;
};

// node_handle_midx_private_construct
template<typename N, typename A> inline
void node_handle_midx_release_node(node_handle_midx<N, A>& node)
{
	node.release_node();
}

template<typename N, typename A,
			template<typename _N, typename _A> class NodeHandle> inline
void node_handle_midx_release_node(NodeHandle<N, A>& node)
{
	typedef NodeHandle<N, A> now_type;
	typedef node_handle_midx<N, A> fake_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(now_type) == sizeof(fake_type))>));

	reinterpret_cast<fake_type&>(node).release_node();
}

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T>
struct cast_to_yggr_node_handle_midx_t
{
	typedef T type;
};

template<typename T>
struct cast_to_boost_node_handle_midx_t
{
	typedef T type;
};

// cast_to_yggr_node_handle_midx
template<typename N, typename A> inline
node_handle_midx<N, A>&
	cast_to_yggr_node_handle_midx(node_handle_midx<N, A>& node)
{
	return node;
}

#if !(BOOST_VERSION < 107400)

template<typename N, typename A> inline
node_handle_midx<N, A>&
	cast_to_yggr_node_handle_midx(boost::multi_index::detail::node_handle<N, A>& node)
{
	typedef node_handle_midx<N, A> ret_type;
	typedef boost::multi_index::detail::node_handle<N, A> arg_type;

	return reinterpret_cast<ret_type&>(node);
}

#endif // !(BOOST_VERSION < 107400)


// cast_to_boost_node_handle_midx
#if (BOOST_VERSION < 107400)

template<typename N, typename A> inline
node_handle_midx<N, A>&
	cast_to_boost_node_handle_midx(node_handle_midx<N, A>& node)
{
	return node;
}

#else

template<typename N, typename A> inline
boost::multi_index::detail::node_handle<N, A>&
	cast_to_boost_node_handle_midx(node_handle_midx<N, A>& node)
{
	typedef boost::multi_index::detail::node_handle<N, A> ret_type;
	typedef node_handle_midx<N, A> args_type;
	
	return reinterpret_cast<ret_type&>(node);
}

#endif // !(BOOST_VERSION < 107400)

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

template<typename N,typename A> inline
void swap(node_handle_midx<N, A>& l, node_handle_midx<N, A>& r) YGGR_NOEXCEPT_IF(noexcept(l.swap(r)))
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
	using yggr::container::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using yggr::container::detail::swap_support::swap;
} // namespace boost

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_MIDX_HPP__
