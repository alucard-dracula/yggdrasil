// node_handle_foo_hacking_midx_hashed_index.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105500_NODE_HANDLE_FOO_HACKING_MIDX_HASHED_INDEX_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105500_NODE_HANDLE_FOO_HACKING_MIDX_HASHED_INDEX_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_HASHED_INDEX_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/multi_index_hashed_index.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_HASHED_INDEX_HPP__

#include <yggr/mplex/limits.hpp>

#include <yggr/container/detail/node_handle_traits.hpp>
#include <yggr/container/detail/boost_midx_check_helper.hpp>
#include <yggr/container/detail/cast_midx_base_type.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_hashed_index_decl.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>

#if !defined(YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT)
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)

#	define YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT \
		::boost::multi_index::detail::scope_guard BOOST_JOIN(check_invariant_, __LINE__) = \
			::boost::multi_index::detail::make_obj_guard( \
				*this, &node_handle_foo_hacking_midx_hashed_index::check_invariant_); \
		BOOST_JOIN(check_invariant_, __LINE__).touch();

#else

#	define YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#endif //YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT


namespace yggr
{
namespace container
{
namespace detail
{

template
<
	typename KeyFromValue, typename Hash, typename Pred,
	typename SuperMeta, typename TagList, typename Category
>
class node_handle_foo_hacking_midx_hashed_index :
	BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS SuperMeta::type

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
	,public
		boost::multi_index::detail::safe_ctr_proxy_impl
		<
			boost::multi_index::detail::hashed_index_iterator
			<
				boost::multi_index::detail::hashed_index_node<typename SuperMeta::type::index_node_type>,
				boost::multi_index::detail::bucket_array<typename SuperMeta::type::final_allocator_type>
			>,
			node_handle_foo_hacking_midx_hashed_index
			<
				KeyFromValue, Hash, Pred,
				SuperMeta, TagList, Category
			>
		>
#else
	,public
		boost::multi_index::safe_mode::safe_container
		<
			node_handle_foo_hacking_midx_hashed_index
			<
				KeyFromValue, Hash, Pred,
				SuperMeta, TagList, Category
			>
		>
#endif
#endif

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
		boost::multi_index::detail::hashed_index
		<
			KeyFromValue, Hash, Pred,
			typename nth_layer_native_cast<SuperMeta>::type,
			TagList, Category
		>														org_type;

public:
	typedef typename SuperMeta::type							super;

public:
	typedef
		boost::multi_index::detail::hashed_index_node
		<
			typename super::index_node_type
		>														index_node_type;

protected:
	typedef typename index_node_type::impl_type						node_impl_type;
	typedef typename node_impl_type::pointer					node_impl_pointer;
	typedef
		boost::multi_index::detail::bucket_array
		<
			typename super::final_allocator_type
		>														bucket_array_type;

public:
	/* types */

	typedef typename org_type::key_type							key_type;
	typedef typename org_type::value_type						value_type;
	typedef typename org_type::key_from_value					key_from_value;
	typedef typename org_type::hasher							hasher;
	typedef typename org_type::key_equal						key_equal;
	typedef typename org_type::ctor_args						ctor_args;
	typedef typename org_type::allocator_type					allocator_type;
	typedef typename org_type::pointer							pointer;
	typedef typename org_type::const_pointer					const_pointer;
	typedef typename org_type::reference						reference;
	typedef typename org_type::const_reference					const_reference;
	typedef typename org_type::size_type						size_type;
	typedef typename org_type::difference_type					difference_type;

	typedef typename org_type::iterator							iterator;
	typedef typename org_type::const_iterator					const_iterator;
	typedef typename org_type::local_iterator					local_iterator;
	typedef typename org_type::const_local_iterator				const_local_iterator;
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
		>												        ctor_args_list;

		typedef typename
			boost::mpl::push_front
			<
				typename super::index_type_list,
				node_handle_foo_hacking_midx_hashed_index
			>::type												index_type_list;

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
		>::type								                    const_iterator_type_list;

	typedef typename super::copy_map_type						copy_map_type;

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)

	typedef typename super::index_saver_type					index_saver_type;
	typedef typename super::index_loader_type					index_loader_type;

#endif // BOOST_MULTI_INDEX_DISABLE_SERIALIZATION

protected:
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#	if BOOST_WORKAROUND(BOOST_MSVC,<1300)

	typedef
		boost::multi_index::detail::safe_ctr_proxy_impl
		<
			boost::multi_index::detail::hashed_index_iterator
			<
				index_node_type,
				bucket_array_type
			>,
			boost::multi_index::hashed_index
		>							                             safe_super;
#else
	typedef
		boost::multi_index::safe_mode::safe_container
		<
			node_handle_foo_hacking_midx_hashed_index
		>							                             safe_super;

#	endif //BOOST_WORKAROUND
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	typedef typename
		boost::call_traits<value_type>::param_type				value_param_type;
	typedef typename
		boost::call_traits<key_type>::param_type                key_param_type;

private:
	typedef node_handle_foo_hacking_midx_hashed_index this_type;

private:
	node_handle_foo_hacking_midx_hashed_index(void);
	node_handle_foo_hacking_midx_hashed_index(BOOST_RV_REF(this_type));
	node_handle_foo_hacking_midx_hashed_index(const this_type&);
	~node_handle_foo_hacking_midx_hashed_index(void);
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
	inline allocator_type get_allocator(void) const
	{
		return this->final().get_allocator();
	}

	insert_return_type insert(BOOST_RV_REF(node_type) nh)
	{
		if(nh)
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(*this, nh);
		}
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

		std::pair<final_node_type*, bool> p = this->final_insert_nh_(nh);
		return insert_return_type(p.second, make_iterator(p.first), boost::move(nh));
	}

	iterator insert(const_iterator position, BOOST_RV_REF(node_type) nh)
	{
		BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, *this);

		if(nh) 
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(*this, nh);
		}
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

		std::pair<final_node_type*, bool> p = 
			this->final_insert_nh_(
				nh, static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	inline node_type extract(const_iterator position)
	{
		BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
		BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		return 
			this->final_extract_(
				static_cast<final_node_type*>(position.get_node()));
	}

	inline node_type extract(key_param_type x)
	{
		iterator position = org_cast().find(x);

		return 
			position == org_cast().end()?
				node_type()
				: this_type::extract(position);
	}

	inline size_type bucket(std::size_t key_hash) const
	{
		return buckets.position(key_hash);
	}

BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:

// hash_opt_support
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)

	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node, ::boost::addressof(buckets), ::boost::addressof(org_cast()));
	}

	inline const_iterator make_iterator(index_node_type* node) const
	{
		return 
			const_iterator(
				node,
				::boost::addressof(const_cast<bucket_array_type&>(buckets)),
				const_cast<org_type*>(::boost::addressof(org_cast())));
	}

#else
	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node, ::boost::addressof(buckets));
	}

	inline const_iterator make_iterator(index_node_type* node)const
	{
		return const_iterator(node, boost::addressof(const_cast<bucket_array_type&>(buckets)));
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
	index_node_type* insert_(value_param_type v, index_node_type* x, Tag tag)
	{
		reserve(org_cast().size() + 1);

		std::size_t buc = find_bucket(v);
		node_impl_pointer pos = buckets.at(buc);
		if(!link_point(v, pos, Category()))
		{
			return index_node_type::from_impl(pos);
		}

		index_node_type* res = static_cast<index_node_type*>(super::insert_(v, x, tag));
		if(res == x)
		{
			link(x, pos);
			if(first_bucket > buc)
			{
				first_bucket = buc;
			}
		}
		return res;
	}

	template<typename Tag> inline
	index_node_type* insert_(value_param_type v, index_node_type* position, index_node_type* x, Tag tag)
	{
		reserve(org_cast().size() + 1);

		std::size_t buc = find_bucket(v);
		node_impl_pointer pos = buckets.at(buc);
		if(!link_point(v, pos, Category()))
		{
			return index_node_type::from_impl(pos);
		}

		index_node_type* res = static_cast<index_node_type*>(super::insert_(v, position, x, tag));
		if(res == x)
		{
			link(x, pos);
			if(first_bucket > buc)
			{
				first_bucket = buc;
			}
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
			if(org.size() != 0 || org.begin() != org.end())
			{
				return false;
			}
		}
		else
		{
			size_type s0 = 0;
			for(const_iterator it = org.begin(), it_end = org.end(); it != it_end; ++it, ++s0){}

			if(s0 != org.size()) 
			{
				return false;
			}

			size_type s1 = 0;
			for(size_type buc = 0; buc < org.bucket_count(); ++buc)
			{
				size_type ss1 = 0;
				for(const_local_iterator it = org.begin(buc), it_end = org.end(buc);
						it != it_end; ++it, ++ss1)
				{
					if(org.bucket(key(*it)) != buc)
					{
						return false;
					}
				}
				if(ss1 != org.bucket_size(buc)) 
				{ 
					return false; 
				}
				s1 += ss1;
			}

			if(s1 != org.size())
			{
				return false;
			}
		}

		if(first_bucket != buckets.first_nonempty(0))
		{
			return false;
		}

		return super::invariant_();
	}

	/* This forwarding function eases things for the boost::mem_fn construct
	* in YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT. Actually,
	* final_check_invariant is already an inherited member function of index.
	*/
	inline void check_invariant_(void) const
	{
		this->final_check_invariant_();
	}

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

private:
	inline index_node_type* header(void) const 
	{
		return this->final_header(); 
	}

	inline std::size_t find_bucket(value_param_type v) const
	{
		return bucket(hash_(key(v)));
	}

	bool link_point(value_param_type v, node_impl_pointer& pos,
						boost::multi_index::detail::hashed_unique_tag )
	{
		node_impl_pointer x = pos->next();
		while(x != pos)
		{
			if(eq_(key(v), key(index_node_type::from_impl(x)->value())))
			{
				pos = x;
				return false;
			}
			x = x->next();
		}
		return true;
	}

	bool link_point(value_param_type v, node_impl_pointer& pos,
						boost::multi_index::detail::hashed_non_unique_tag)
	{
		node_impl_pointer prev = pos;
		node_impl_pointer x = pos->next();
		while(x != pos)
		{
			if(eq_(key(v), key(index_node_type::from_impl(x)->value())))
			{
				pos = prev;
				return true;
			}
			prev = x;
			x = x->next();
		}
		return true;
	}

	inline static void link(index_node_type* x, node_impl_pointer pos)
	{
		node_impl_type::link(x->impl(), pos);
	};

	inline static void link(node_impl_pointer x, node_impl_pointer pos)
	{
		node_impl_type::link(x, pos);
	};

	inline static void unlink(index_node_type* x)
	{
		node_impl_type::unlink(x->impl());
	};

	inline static node_impl_pointer prev(index_node_type* x)
	{
		return node_impl_type::prev(x->impl());
	}

	inline static void unlink_next(node_impl_pointer x)
	{
		node_impl_type::unlink_next(x);
	}

	inline void calculate_max_load(void)
	{
		float fml = static_cast<float>(mlf * org_cast().bucket_count());
		max_load = ::yggr::mplex::numeric_limits<size_type>::max_type::value;
		if(max_load > fml)
		{
			max_load = static_cast<size_type>(fml);
		}
	}

	// hash opt support
	inline void reserve(size_type n)
	{
		if(n > max_load)
		{
			size_type bc = ::yggr::mplex::numeric_limits<size_type>::max_type::value;
			float fbc = static_cast<float>(1 + static_cast<double>(n) / mlf);
			if(bc > fbc)
			{
				bc = static_cast<size_type>(fbc);
			}
			unchecked_rehash(bc);
		}
	}

	// hash opt support
	void unchecked_rehash(size_type n)
	{
		bucket_array_type buckets1(get_allocator(), header()->impl(), n);
		boost::multi_index::detail::auto_space
		<
			std::size_t, allocator_type
		> hashes(get_allocator(), org_cast().size());

		std::size_t i = 0;
		node_impl_pointer x = buckets.begin();
		node_impl_pointer x_end = buckets.end();
		for(; x != x_end; ++x)
		{
			node_impl_pointer y = x->next();
			while(y != x)
			{
				hashes.data()[i++] = hash_(key(index_node_type::from_impl(y)->value()));
				y = y->next();
			}
		}

		i = 0;
		x = buckets.begin();
		for(; x != x_end; ++x)
		{
			node_impl_pointer y = x->next();
			while(y != x)
			{
				node_impl_pointer z = y->next();
				std::size_t buc1 = buckets1.position(hashes.data()[i++]);
				node_impl_pointer x1 = buckets1.at(buc1);
				link(y, x1);
				y = z;
			}
		}

		buckets.swap(buckets1);
		calculate_max_load();
		first_bucket=buckets.first_nonempty(0);
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	inline void detach_iterators(index_node_type* x)
	{
		iterator it=make_iterator(x);
		{
			using namespace  boost::multi_index;
			safe_mode::detach_equivalent_iterators(it);
		}
	}
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

protected:
	key_from_value               key;
	hasher                       hash_;
	key_equal                    eq_;
	bucket_array_type            buckets;
	float                        mlf;
	size_type                    max_load;
	std::size_t                  first_bucket;

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif
};

template<typename K, typename H, typename P, typename S, typename T, typename C>
struct node_handle_traits_impl_custom< boost::multi_index::detail::hashed_index<K, H, P, S, T, C> >
{
private:
	typedef boost::multi_index::detail::hashed_index<K, H, P, S, T, C> native_type;
	typedef typename index_hacking_cast<native_type>::type hacking_type;

public:
	typedef typename hacking_type::node_type node_type;
	typedef typename hacking_type::insert_return_type insert_return_type;
	typedef typename hacking_type::iterator hint_insert_return_type;
};


template<typename Arg1, typename Arg2,
			typename Arg3, typename Arg4>
struct node_handle_foo_hacking_midx_hashed_unique
{
	typedef boost::multi_index::hashed_unique<Arg1, Arg2, Arg3, Arg4> org_type;

	typedef typename org_type::index_args index_args;
	typedef typename org_type::tag_list_type tag_list_type;
	typedef typename org_type::key_from_value_type key_from_value_type;
	typedef typename org_type::hash_type hash_type;
	typedef typename org_type::pred_type pred_type;

	template<typename Super>
	struct node_class
	{
		typedef boost::multi_index::detail::hashed_index_node<Super> type;
	};

	template<typename SuperMeta>
	struct index_class
	{
		typedef
			node_handle_foo_hacking_midx_hashed_index
			<
				key_from_value_type,
				hash_type,
				pred_type,
				SuperMeta,
				tag_list_type,
				boost::multi_index::detail::hashed_unique_tag
			> type;
	};
};

template<typename Arg1, typename Arg2,
			typename Arg3, typename Arg4>
struct node_handle_foo_hacking_midx_hashed_non_unique
{
	typedef boost::multi_index::hashed_non_unique< Arg1, Arg2, Arg3, Arg3 > org_type;

	typedef typename org_type::index_args index_args;
	typedef typename org_type::tag_list_type tag_list_type;
	typedef typename org_type::key_from_value_type key_from_value_type;
	typedef typename org_type::hash_type hash_type;
	typedef typename org_type::pred_type pred_type;

	template<typename Super>
	struct node_class
	{
		typedef boost::multi_index::detail::hashed_index_node<Super> type;
	};

	template<typename SuperMeta>
	struct index_class
	{
		typedef
			node_handle_foo_hacking_midx_hashed_index
			<
				key_from_value_type,
				hash_type,
				pred_type,
				SuperMeta,
				tag_list_type,
				boost::multi_index::detail::hashed_non_unique_tag
			> type;
	};
};

} // namespace detail
} // namespace container
} // namespace yggr

#if defined(YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT)
#	undef YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT
#endif // YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105500_NODE_HANDLE_FOO_HACKING_MIDX_HASHED_INDEX_HPP__
