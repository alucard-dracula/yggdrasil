//boost_ugtable_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_UGTABLE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_UGTABLE_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UGTABLE_OPT_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UGTABLE_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/container/detail/unordered_node_algo.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_table.hpp>
#include <yggr/safe_container/detail/boost_unordered_create_node.hpp>
#include <yggr/safe_container/detail/boost_uiterator_opt.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/unordered/detail/equivalent.hpp>

#include <iterator>
#include <cassert>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Types>
struct boost_ugtable_opt 
	: boost::unordered::detail::table<Types>
{
	typedef boost::unordered::detail::grouped_table_impl<Types> org_type;
	typedef typename org_type::table table;
	
	typedef typename table::value_type value_type;
	typedef typename table::bucket bucket;
	typedef typename table::policy policy;
	typedef typename table::node_pointer node_pointer;
	typedef typename table::node_allocator node_allocator;
	typedef typename table::node_allocator_traits node_allocator_traits;
	typedef typename table::bucket_pointer bucket_pointer;
	typedef typename table::link_pointer link_pointer;
	typedef typename table::hasher hasher;
	typedef typename table::key_equal key_equal;
	typedef typename table::key_type key_type;
	typedef typename table::node_constructor node_constructor;
	typedef typename table::extractor extractor;
	typedef typename table::iterator iterator;
	typedef typename table::c_iterator c_iterator;

	// opt_type
	typedef typename cast_to_u_iterator_opt<iterator>::type opt_iterator_type;
	typedef typename cast_to_u_c_iterator_opt<c_iterator>::type opt_c_iterator_type;

private:
	typedef typename 
		container::detail::node_algo_selector
		< 
			org_type 
		>::type node_algo_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(opt_iterator_type) == sizeof(iterator))>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(opt_c_iterator_type) == sizeof(c_iterator))>));

private:
	typedef boost_ugtable_opt this_type;

private:
	// Constructors
	boost_ugtable_opt(void);
	boost_ugtable_opt(BOOST_RV_REF(boost_ugtable_opt));
	boost_ugtable_opt(const boost_ugtable_opt&);
	~boost_ugtable_opt(void);
	const boost_ugtable_opt& operator=( const boost_ugtable_opt& );
	const boost_ugtable_opt& operator=( BOOST_RV_REF(boost_ugtable_opt) );

protected:
	inline org_type& org_cast(void)
	{
		return reinterpret_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return reinterpret_cast<const org_type&>(*this);
	}

	inline static opt_c_iterator_type& opt_c_iterator_cast(c_iterator& src)
	{
		return reinterpret_cast<opt_c_iterator_type&>(src);
	}

	inline static const opt_c_iterator_type& opt_c_iterator_cast(const c_iterator& src)
	{
		return reinterpret_cast<const opt_c_iterator_type&>(src);
	}

	inline static opt_iterator_type& opt_iterator_cast(iterator& src)
	{
		return reinterpret_cast<opt_iterator_type&>(src);
	}

	inline static const opt_iterator_type& opt_iterator_cast(const iterator& src)
	{
		return reinterpret_cast<const opt_iterator_type&>(src);
	}

public:

	template<typename VAlloc, BOOST_UNORDERED_EMPLACE_TEMPLATE> inline
	static node_pointer create_node_1(const VAlloc& valloc, BOOST_UNORDERED_EMPLACE_ARGS)
	{
		return 
			__create_u_node_1<node_pointer, node_allocator, node_constructor>(
				valloc, BOOST_UNORDERED_EMPLACE_FORWARD);
	}

	template<typename VAlloc, typename A0> inline
	static node_pointer create_node_2(const VAlloc& valloc, BOOST_FWD_REF(A0) a0)
	{
		return 
			__create_u_node_2<node_pointer, node_allocator, node_constructor>(
				valloc, boost::forward<A0>(a0));
	}

public:
	// Accessors
	std::size_t count(std::size_t key_hash, key_type const& k) const
	{
		const org_type& org = org_cast();

		if(0 == org.size_)
		{
			return 0;
		}

		iterator n = org.find_node(key_hash, k);
		if(n == iterator()) 
		{
			return 0;
		}

		std::size_t x = 0;
		node_pointer it = opt_iterator_cast(n).node_;
		node_pointer it_end = it; 
		do 
		{
			it = it->group_prev_;
			++x;
		} while(it != it_end);

		return x;
	}

	inline std::pair<iterator, iterator>
		equal_range(std::size_t key_hash, key_type const& k) const
	{
		iterator n = org_cast().find_node(key_hash, k);
		const node_pointer& node_ptr = opt_iterator_cast(n).node_;
		return std::make_pair(
			n, node_ptr ? iterator(node_ptr->group_prev_->next_) : n);
	}

	// Equality

	inline iterator emplace_impl(std::size_t key_hash, node_constructor& a)
	{
		org_type& org = org_cast();
		key_type const& k = org.get_key(a.value());
		iterator position = org.find_node(key_hash, k);

		// reserve has basic exception safety if the hash function
		// throws, strong otherwise.
		org.reserve_for_insert(org.size_ + 1);
		return org.add_node(a, key_hash, position);
	}

	inline void emplace_impl_no_rehash(std::size_t key_hash, node_constructor& a)
	{
		org_type& org = org_cast();
		key_type const& k = org.get_key(a.value());
		org.add_node(a, key_hash, org.find_node(key_hash, k));
	}

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#   if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
	inline iterator emplace(std::size_t, 
							boost::unordered::detail::emplace_args1
							<
								boost::unordered::detail::please_ignore_this_overload
							> const&)
	{
		BOOST_ASSERT(false);
		return iterator();
	}
#   else
	inline iterator emplace(std::size_t, 
							boost::unordered::detail::please_ignore_this_overload const&)
	{
		BOOST_ASSERT(false);
		return iterator();
	}
#   endif
#endif

	template<BOOST_UNORDERED_EMPLACE_TEMPLATE> inline
	iterator emplace(std::size_t key_hash, BOOST_UNORDERED_EMPLACE_ARGS)
	{
		node_constructor a(org_cast().node_alloc());
		a.construct_with_value(BOOST_UNORDERED_EMPLACE_FORWARD);

		return iterator(this_type::emplace_impl(key_hash, a));
	}

	inline void insert_impl_no_rehash(std::size_t key_hash, node_pointer pnode)
	{
		assert(pnode);
		assert(pnode->hash_ == key_hash);
		org_type& org = org_cast();
		this_type::add_node(pnode, org.find_node(key_hash, org.get_key(pnode->value())));
	}

	inline iterator insert(std::size_t key_hash, node_pointer pnode)
	{
		assert(pnode);
		assert(key_hash == pnode->hash_);

		org_type& org = org_cast();
		iterator position = org.find_node(key_hash, org.get_key(pnode->value()));

		// reserve has basic exception safety if the hash function
		// throws, strong otherwise.
		org.reserve_for_insert(org.size_ + 1);
		return this_type::add_node(pnode, position);
	}

	template<typename KI, typename I>
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::unordered::detail::is_forward<I>,
				boost::mpl::not_
				<
					boost::is_same
					<
						typename iterator_ex::iterator_value_ex<I>::type,
						node_pointer
					>
				>
			>,
			void
		>::type
		insert_range(KI ki, KI kj, I i, I j)
	{
		if((ki == kj) || (i == j)) 
		{
			return;
		}

		std::size_t distance = boost::unordered::detail::distance(i, j);
		assert((std::distance(ki, kj) == distance));

		org_type& org = org_cast();

		if(distance == 1) 
		{
			node_constructor a(org.node_alloc());
			a.construct_with_value2(*i);
			this_type::emplace_impl(*ki, a);
		}
		else 
		{
			// Only require basic exception safety here
			org.reserve_for_insert(org.size_ + distance);

			node_constructor a(org.node_alloc());
			for (; (i != j); ++i, ++ki) 
			{
				assert((ki != kj));
				a.construct_with_value2(*i);
				this_type::emplace_impl_no_rehash(*ki, a);
			}
		}
	}

	template<typename KI, typename I>
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::unordered::detail::is_forward<I> >,
				boost::mpl::not_
				<
					boost::is_same
					<
						typename iterator_ex::iterator_value_ex<I>::type,
						node_pointer
					>
				>
			>,
			void
		>::type
		insert_range(KI ki, KI kj, I i, I j)
	{
		assert((std::distance(ki, kj) == std::distance(i, j)));

		if(!(std::distance(ki, kj) == std::distance(i, j)))
		{
			return;
		}

		node_constructor a(org_cast(). node_alloc());
		for (; i != j; ++i, ++ki) 
		{
			a.construct_with_value2(*i);
			this_type::emplace_impl(*ki, a);
		}
	}

	template<typename KI, typename I>
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::unordered::detail::is_forward<I>,
				boost::is_same
				<
					typename iterator_ex::iterator_value_ex<I>::type,
					node_pointer
				>
			>,
			void
		>::type
		insert_range(KI ki, KI kj, I i, I j)
	{
		typedef typename boost::iterator_value<I>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<node_pointer, chk_type>));

		if((ki == kj) || (i == j)) 
		{
			return;
		}

		std::size_t distance = boost::unordered::detail::distance(i, j);
		assert((std::distance(ki, kj) == distance));

		org_type& org = org_cast();

		if(distance == 1) 
		{
			assert(*i);
			this_type::insert(*ki, *i);
		}
		else 
		{
			// Only require basic exception safety here
			org.reserve_for_insert(org.size_ + distance);
			for ( ; (i != j); ++i, ++ki) 
			{
				this_type::insert_impl_no_rehash(*ki, *i);
			}
		}
	}

	template<typename KI, typename I>
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::unordered::detail::is_forward<I> >,
				boost::is_same
				<
					typename iterator_ex::iterator_value_ex<I>::type,
					node_pointer
				>
			>,
			void
		>::type
		insert_range(KI ki, KI kj, I i, I j)
	{
		typedef typename boost::iterator_value<I>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<node_pointer, chk_type>));
		assert((std::distance(ki, kj) == boost::unordered::detail::distance(i, j)));

		for (; i != j; ++i, ++ki) 
		{
			this_type::insert(*ki, *i);
		}
	}

	template<typename I>
	typename 
		boost::enable_if
		<
			boost::is_same
			<
				typename iterator_ex::iterator_value_ex<I>::type,
				node_pointer
			>,
			void
		>::type
		insert_range(std::size_t key_hash, I i, I j)
	{
		typedef typename boost::iterator_value<I>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<node_pointer, chk_type>));

#ifdef _DEBUG
		for (I chk_i = i; chk_i != j; ++chk_i) 
		{
			assert((key_hash == (*i)->hash_));
		}
#endif // _DEBUG

		for (; i != j; ++i) 
		{
			this_type::insert(key_hash, *i);
		}
	}

	// Erase

	template<typename Destroy_Holder>
	std::size_t erase_key(Destroy_Holder& holder, std::size_t key_hash, key_type const& k)
	{
		org_type& org = org_cast();

		if(!org.size_) 
		{
			return 0;
		}

		std::size_t bucket_index = org.hash_to_bucket(key_hash);
		link_pointer prev = org.get_previous_start(bucket_index);
		
		if(!prev) 
		{
			return 0;
		}

		for (;;)
		{
			if(!prev->next_)
			{
				return 0;
			}

			std::size_t node_hash =
				static_cast<node_pointer>(prev->next_)->hash_;
			if(org.hash_to_bucket(node_hash) != bucket_index)
			{
				return 0;
			}
			if(node_hash == key_hash &&
				org.key_eq()(k, org.get_key(
					static_cast<node_pointer>(prev->next_)->value())))
			{
				break;
			}
			prev = static_cast<node_pointer>(prev->next_)->group_prev_;
		}

		node_pointer first_node = static_cast<node_pointer>(prev->next_);
		link_pointer end = first_node->group_prev_->next_;

		std::size_t count = this_type::delete_nodes(holder, prev, end);
		org.fix_bucket(bucket_index, prev);
		return count;
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder& holder, c_iterator r)
	{
		opt_c_iterator_type& r_opt = opt_c_iterator_cast(r);
		BOOST_ASSERT(r_opt.node_);
		iterator next(r_opt.node_);
		++next;
		erase_nodes(holder, r_opt.node_, opt_c_iterator_cast(next).node_);
		return next;
	}

	template<typename Destroy_Holder> inline
	iterator erase_range(Destroy_Holder& holder, c_iterator r1, c_iterator r2)
	{
		opt_c_iterator_type& r1_opt = opt_c_iterator_cast(r1);
		opt_c_iterator_type& r2_opt = opt_c_iterator_cast(r2);

		if(r1 == r2) 
		{	
			return iterator(r2_opt.node_);
		}
		erase_nodes(holder, r1_opt.node_, r2_opt.node_);
		return iterator(r2_opt.node_);
	}

	template<typename Destroy_Holder> inline
	void clear(Destroy_Holder& holder)
	{
		if(!size_)
		{
			return;
		}

		org_type& org = org_cast();

		delete_nodes(holder, org.get_previous_start(), link_pointer());
		org.clear_buckets();

		BOOST_ASSERT(!org.size_);
	}

//private:
	inline iterator add_node(node_pointer n, iterator pos)
	{
		org_type& org = org_cast();
		opt_iterator_type& pos_opt = opt_iterator_cast(pos);
		if(pos_opt.node_) 
		{
			org.add_after_node(n, pos_opt.node_);
			if(n->next_) 
			{
				std::size_t next_bucket 
					= org.hash_to_bucket(static_cast<node_pointer>(n->next_)->hash_);
				if(next_bucket != org.hash_to_bucket(n->hash_)) 
				{
					org.get_bucket(next_bucket)->next_ = n;
				}
			}
		}
		else 
		{
			bucket_pointer b = org.get_bucket(org.hash_to_bucket(n->hash_));

			if(!b->next_)
			{
				link_pointer start_node =org.get_previous_start();
                    
				if(start_node->next_) 
				{
					org.get_bucket(
						org.hash_to_bucket(
							static_cast<node_pointer>(start_node->next_)->hash_))->next_ = n;
				}
    
				b->next_ = start_node;
				n->next_ = start_node->next_;
				start_node->next_ = n;
			}
			else
			{
				n->next_ = b->next_->next_;
				b->next_->next_ = n;
			}
		}
		++(org.size_);
		return iterator(n);
	}

	inline iterator add_node(node_pointer n, std::size_t key_hash, iterator pos)
	{
		n->hash_ = key_hash;
		return this_type::add_node(n, pos);
	}

	// start 156, but just only variable names are different
	template<typename Destroy_Holder>
	link_pointer erase_nodes(Destroy_Holder& holder, node_pointer i, node_pointer j)
	{
		org_type& org = org_cast();

		std::size_t bucket_index = org.hash_to_bucket(i->hash_);

		// Split the groups containing 'i' and 'j'.
		// And get the pointer to the node before i while
		// we're at it.
		link_pointer prev = org.split_groups(i, j);

		// If we don't have a 'prev' it means that i is at the
		// beginning of a block, so search through the blocks in the
		// same bucket.
		if(!prev) 
		{
			prev = org.get_previous_start(bucket_index);
			while(prev->next_ != i)
			{
				prev = static_cast<node_pointer>(prev->next_)->group_prev_;
			}
		}

		// Delete the nodes.
		do 
		{
			link_pointer group_end =
				static_cast<node_pointer>(prev->next_)->group_prev_->next_;
			this_type::delete_nodes(holder, prev, group_end);
			bucket_index = org.fix_bucket(bucket_index, prev);
		} while(prev->next_ != j);

		return prev;
	}

	template<typename Destroy_Holder> inline
	void delete_node(Destroy_Holder& holder, link_pointer prev)
	{
		node_pointer n = static_cast<node_pointer>(prev->next_);
		prev->next_ = n->next_;
		n->init(n);

		holder(n);
		--(org_cast().size_);
	}

	template<typename Destroy_Holder>
	std::size_t delete_nodes(Destroy_Holder& holder, link_pointer prev, link_pointer end)
	{
		BOOST_ASSERT(prev->next_ != end);

		std::size_t count = 0;

		do 
		{
			delete_node(holder, prev);
			++count;
		} while(prev->next_ != end);

		return count;
	}

public:
	template<typename NodeType> inline
	iterator move_insert_node_type(std::size_t key_hash, NodeType& np)
    {
		org_type& org = org_cast();

        iterator result;

        if(np) 
		{
            const key_type& k = org.get_key(np.ptr_->value());
			assert(key_hash == org.hash(k));
			iterator pos = org.find_node(key_hash, k);
            org.reserve_for_insert(org.size_ + 1);
			result = this_type::add_node(np.ptr_, key_hash, pos);
            np.ptr_ = node_pointer();
        }

        return result;
    }

protected:
	inline node_pointer add_using_hint(node_pointer n, node_pointer hint)
	{
		org_type& org = org_cast();

		n->hash_ = hint->hash_;
		org.add_after_node(n, hint);
		if(n->next_ != hint && n->next_)
		{
			std::size_t next_bucket = org.hash_to_bucket(node_algo_type::next_node(n)->hash_);
			if(next_bucket != org.hash_to_bucket(n->hash_)) 
			{
				org.get_bucket(next_bucket)->next_ = n;
			}
		}
		++org.size_;
		return n;
	}

public:
	template<typename NodeType> inline
    iterator move_insert_node_type_with_hint(std::size_t key_hash, c_iterator hint, NodeType& np)
    {
		org_type& org = org_cast();

        iterator result;

        if(np) 
		{
            const key_type& k = org.get_key(np.ptr_->value());

            if(opt_c_iterator_cast(hint).node_ 
				&& org.key_eq()(k, org.get_key(*hint))) 
			{
                org.reserve_for_insert(org.size_ + 1);
				result = 
					iterator(
						this_type::add_using_hint(
							np.ptr_, opt_c_iterator_cast(hint).node_));
            }
			else 
			{
				assert(org.hash(k) == key_hash);
				iterator pos = org.find_node(key_hash, k);
                org.reserve_for_insert(org.size_ + 1);
				result = this_type::add_node(np.ptr_, key_hash, pos);
            }

            np.ptr_ = node_pointer();
        }

        return result;
    }
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Types>
struct cast_to_ugtable_opt< boost::unordered::detail::grouped_table_impl<Types> >
{
	typedef boost_ugtable_opt<Types> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr


#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_UGTABLE_OPT_HPP__