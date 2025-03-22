// node_handle.hpp 

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_HPP__

#include <yggr/base/yggrdef.h>

#if (BOOST_VERSION < 106200)

#include <yggr/type_traits/aligned_storage.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/move/detail/to_raw_pointer.hpp>

#include <yggr/container/detail/workaround.hpp>
#include <yggr/container/detail/placement_new.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_empty.hpp>

#include <boost/container/allocator_traits.hpp>
//#include <boost/type_traits/aligned_storage.hpp>


namespace yggr
{
namespace container
{
namespace detail
{

template<typename Value, typename KeyMapped>
struct node_handle_keymapped_traits
{
	typedef typename KeyMapped::key_type      key_type;
	typedef typename KeyMapped::mapped_type   mapped_type;
};

template<typename Value>
struct node_handle_keymapped_traits<Value, void>
{
	typedef Value key_type;
	typedef Value mapped_type;
};

class node_handle_friend
{
public:
	template<class NH> YGGR_CONTAINER_FORCEINLINE
	static void destroy_alloc(NH &nh) YGGR_NOEXCEPT
	{ 
		nh.destroy_alloc();  
	}

	template<class NH> YGGR_CONTAINER_FORCEINLINE
	static typename NH::node_pointer& get_node_pointer(NH &nh) YGGR_NOEXCEPT
	{
		return nh.get_node_pointer();  
	}
};


///@endcond

//! A node_handle is an object that accepts ownership of a single element from an associative container.
//! It may be used to transfer that ownership to another container with compatible nodes. Containers
//! with compatible nodes have the same node handle type. Elements may be transferred in either direction
//! between container types in the same row:.
//!
//! Container types with compatible nodes
//!
//! map<K, T, C1, A> <-> map<K, T, C2, A>
//!
//! map<K, T, C1, A> <-> multimap<K, T, C2, A>
//! 
//! set<K, C1, A> <-> set<K, C2, A>
//! 
//! set<K, C1, A> <-> multiset<K, C2, A>
//! 
//! If a node handle is not empty, then it contains an allocator that is equal to the allocator of the container
//! when the element was extracted. If a node handle is empty, it contains no allocator.

//node_handle_inner_alloc_cmper
template<typename Alloc, bool is_empty>
struct node_handle_inner_alloc_cmper_impl;

template<typename Alloc>
struct node_handle_inner_alloc_cmper_impl<Alloc, true>
{
public:
	YGGR_CONTAINER_FORCEINLINE bool operator()(const Alloc&, const Alloc&) const
	{
		return true;
	}
};

template<typename Alloc>
struct node_handle_inner_alloc_cmper_impl<Alloc, false>
{
public:
	YGGR_CONTAINER_FORCEINLINE bool operator()(const Alloc& a, const Alloc& b) const
	{
		return a == b;
	}
};

template<typename Alloc>
struct node_handle_inner_alloc_cmper
	: public node_handle_inner_alloc_cmper_impl<Alloc, boost::is_empty<Alloc>::value>
{
};

//node_handle_inner_alloc_move_setter
template<typename Alloc,
			bool is_propagate_on_container_move_assignment
				= boost::container::allocator_traits<Alloc>::propagate_on_container_move_assignment::value>
struct node_handle_inner_alloc_move_setter_impl;

template<typename Alloc>
struct node_handle_inner_alloc_move_setter_impl<Alloc, true>
{
public:
	template<typename T1, typename T2> YGGR_CONTAINER_FORCEINLINE 
	void operator()(T1& l, BOOST_RV_REF(T2) r) const
	{
		l = boost::move(r);
	}
};

template<typename Alloc>
struct node_handle_inner_alloc_move_setter_impl<Alloc, false>
{
public:
	template<typename T1, typename T2> YGGR_CONTAINER_FORCEINLINE 
	void operator()(T1&, BOOST_RV_REF(T2)) const
	{
	}
};

template<typename Alloc>
struct node_handle_inner_alloc_move_setter
	: public node_handle_inner_alloc_move_setter_impl<Alloc>
{
};

//node_handle_inner_alloc_swapper
template<typename Alloc,
			bool is_propagate_on_container_swap
				= boost::container::allocator_traits<Alloc>::propagate_on_container_swap::value>
struct node_handle_inner_alloc_swapper_impl;

template<typename Alloc>
struct node_handle_inner_alloc_swapper_impl<Alloc, true>
{
public:
	template<typename T1, typename T2> YGGR_CONTAINER_FORCEINLINE 
	void operator()(T1& l, T2& r) const
	{
		::boost::adl_move_swap(l, r);
	}
};

template<typename Alloc>
struct node_handle_inner_alloc_swapper_impl<Alloc, false>
{
public:
	template<typename T1, typename T2> YGGR_CONTAINER_FORCEINLINE 
	void operator()(T1&, T2&) const
	{
	}
};

template<typename Alloc>
struct node_handle_inner_alloc_swapper
	: public node_handle_inner_alloc_swapper_impl<Alloc>
{
};

// node_handle_bool_conversion
struct node_handle_bool_conversion 
{
	int for_bool; 
	int for_arg(void);
}; 

template <typename NodeAllocator, typename KeyMapped = void>
class node_handle
{
	typedef NodeAllocator											nallocator_type;
	typedef boost::container::allocator_traits<NodeAllocator>		nator_traits;
	typedef typename nator_traits::value_type						priv_node_t;
	typedef typename priv_node_t::value_type						priv_value_t;
	typedef node_handle_keymapped_traits<priv_value_t, KeyMapped>	keymapped_t;

public:
	typedef priv_value_t											value_type;
	typedef typename keymapped_t::key_type							key_type;
	typedef typename keymapped_t::mapped_type						mapped_type;
	typedef typename 
		nator_traits::
		template portable_rebind_alloc<value_type>::type			allocator_type;

	typedef priv_node_t												container_node_type;

	friend class node_handle_friend;

	///@cond
private:
	BOOST_MOVABLE_BUT_NOT_COPYABLE(node_handle)

	typedef typename nator_traits::pointer							node_pointer;
	typedef 
		::boost::aligned_storage
		< 
			sizeof(nallocator_type), 
			::boost::alignment_of<nallocator_type>::value
		>															nalloc_storage_t;

private:
	typedef mplex::sfinae_type sfinae_type;
	
private:
	typedef node_handle this_type;

private:
	inline void move_construct_alloc(nallocator_type& al)
	{  
		::new(_nalloc_storage.address(), boost_container_new_t()) nallocator_type(::boost::move(al));   
	}

	inline void destroy_deallocate_node(void)
	{
		nator_traits::destroy(
			this_type::node_alloc(), 
			::yggr::move::detail::to_raw_pointer(_ptr));
		nator_traits::deallocate(this_type::node_alloc(), _ptr, 1u);
	}

	template<class OtherNodeHandle> inline
	void move_construct_end(OtherNodeHandle &nh)
	{
		if(_ptr)
		{
			::new (_nalloc_storage.address(), boost_container_new_t()) nallocator_type(::boost::move(nh.node_alloc()));
			node_handle_friend::destroy_alloc(nh);
			node_handle_friend::get_node_pointer(nh) = node_pointer();
		}
		BOOST_ASSERT(nh.empty());
	}

	inline void destroy_alloc(void) YGGR_NOEXCEPT
	{  
		static_cast<nallocator_type*>(_nalloc_storage.address())->~nallocator_type();  
	}

	inline node_pointer& get_node_pointer(void) YGGR_NOEXCEPT
	{  
		return _ptr;  
	}

public:
	YGGR_CXX14_CONSTEXPR node_handle(void) YGGR_NOEXCEPT
		: _ptr()
	{ 
	}

	node_handle(node_pointer p, const nallocator_type &al) YGGR_NOEXCEPT
		: _ptr(p)
	{
		if(_ptr)
		{
			::new (_nalloc_storage.address(), boost_container_new_t()) nallocator_type(al);
		}
	}


	template<class KeyMapped2>
	node_handle( BOOST_RV_REF_BEG node_handle<NodeAllocator, KeyMapped2> BOOST_RV_REF_END nh, 
					typename 
						::boost::enable_if_c
						< (static_cast<unsigned>(boost::is_same<KeyMapped, void>::value) 
							+ static_cast<unsigned>(boost::is_same<KeyMapped2, void>::value)) == 1u,
							sfinae_type
						>::type sfinae = 0) YGGR_NOEXCEPT
		:  _ptr(nh.get())
	{  
		this_type::move_construct_end(nh);  
	}

	node_handle (BOOST_RV_REF(node_handle) nh) BOOST_NOEXCEPT
		: _ptr(nh._ptr)
	{
		this_type::move_construct_end(nh);
	}

	~node_handle(void) BOOST_NOEXCEPT
	{
		if(!this_type::empty())
		{
			this_type::destroy_deallocate_node();
			this_type::destroy_alloc();
		}
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) nh) YGGR_NOEXCEPT
	{
		BOOST_ASSERT(
			this_type::empty() 
			|| nator_traits::propagate_on_container_move_assignment::value 
			|| node_handle_inner_alloc_cmper<nallocator_type>()(this_type::node_alloc(), nh.node_alloc()) );

		bool const was_this_non_null = !this_type::empty();
		bool const was_nh_non_null   = !nh.empty();

		if(was_nh_non_null)
		{
			if(was_this_non_null)
			{
				this_type::destroy_deallocate_node();
				node_handle_inner_alloc_move_setter<NodeAllocator> setter;
				setter(this_type::node_alloc(), ::boost::move(nh.node_alloc()));
			}
			else
			{
				this_type::move_construct_alloc(nh.node_alloc());
			}

			_ptr = nh._ptr;
			nh._ptr = node_pointer();
			nh.destroy_alloc();
		}
		else if(was_this_non_null)
		{
			this_type::destroy_deallocate_node();
			this_type::destroy_alloc();
			_ptr = node_pointer();
		}

		return *this;
	}

public:
	inline value_type& value(void) const YGGR_NOEXCEPT
	{
		BOOST_STATIC_ASSERT((boost::is_same<KeyMapped, void>::value));
		BOOST_ASSERT(!this_type::empty());
		return _ptr->get_data();
	}

	inline key_type& key(void) const YGGR_NOEXCEPT
	{
		BOOST_STATIC_ASSERT((!boost::is_same<KeyMapped, void>::value));
		BOOST_ASSERT(!this_type::empty());
		return const_cast<key_type&>(KeyMapped().key_of_value(_ptr->get_data()));
	}

	inline mapped_type& mapped(void) const YGGR_NOEXCEPT
	{
		BOOST_STATIC_ASSERT((!boost::is_same<KeyMapped, void>::value));
		BOOST_ASSERT(!this_type::empty());
		return KeyMapped().mapped_of_value(_ptr->get_data());
	}

	inline allocator_type get_allocator(void) const
	{
		BOOST_ASSERT(!this_type::empty());
		return this_type::node_alloc();
	}

#	if defined(BOOST_CONTAINER_DOXYGEN_INVOKED)
	YGGR_CONTAINER_FORCEINLINE explicit operator bool(void) const YGGR_NOEXCEPT
#	else
private: 
	typedef int node_handle_bool_conversion::* explicit_bool_arg;

public: 
	YGGR_CONTAINER_FORCEINLINE operator explicit_bool_arg (void) const YGGR_NOEXCEPT
#	endif // BOOST_CONTAINER_DOXYGEN_INVOKED
	{  
		return _ptr? &node_handle_bool_conversion::for_bool : explicit_bool_arg(0); 
	}

	inline bool empty(void) const YGGR_NOEXCEPT
	{
		return !_ptr;
	}

	inline void swap(this_type& nh) 
		YGGR_NOEXCEPT_IF(
			nator_traits::propagate_on_container_swap::value 
			|| nator_traits::is_always_equal::value)
	{
		BOOST_ASSERT(
			this_type::empty() 
			|| nh.empty() 
			|| nator_traits::propagate_on_container_swap::value
			|| nator_traits::equal(node_alloc(), nh.node_alloc()));

		bool const was_this_non_null = !this_type::empty();
		bool const was_nh_non_null   = !nh.empty();

		if(was_nh_non_null)
		{
			if(was_this_non_null)
			{
				node_handle_inner_alloc_swapper<NodeAllocator> swapper;
				swapper(this_type::node_alloc(), nh.node_alloc());
			}
			else
			{
				this_type::move_construct_alloc(nh.node_alloc());
				nh.destroy_alloc();
			}
		}
		else if(was_this_non_null)
		{
			nh.move_construct_alloc(this_type::node_alloc());
			this_type::destroy_alloc();
		}

		::boost::adl_move_swap(_ptr, nh._ptr);
	}

	inline node_pointer release(void) YGGR_NOEXCEPT
	{
		node_pointer p(_ptr);
		_ptr = node_pointer();
		if(p)
		{
			this_type::destroy_alloc();
		}
		return p;
	}

	inline node_pointer get(void) const YGGR_NOEXCEPT
	{
		return _ptr;
	}

	inline nallocator_type& node_alloc(void) YGGR_NOEXCEPT
	{
		BOOST_ASSERT(!this_type::empty());
		return *static_cast<nallocator_type*>(_nalloc_storage.address());
	}

	inline const nallocator_type& node_alloc(void) const YGGR_NOEXCEPT
	{
		BOOST_ASSERT(!this_type::empty());
		return *static_cast<const nallocator_type*>(_nalloc_storage.address());
	}

private:
	node_pointer      _ptr;
	nalloc_storage_t  _nalloc_storage;
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
template <typename NA, typename KM> inline
void swap(node_handle<NA, KM>& l, node_handle<NA, KM>& r) YGGR_NOEXCEPT_IF(BOOST_NOEXCEPT(x.swap(y)))
{
	l.swap(r);
}

} // swap_support

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
	
#endif // (BOOST_VERSION < 106200)

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_HPP__
