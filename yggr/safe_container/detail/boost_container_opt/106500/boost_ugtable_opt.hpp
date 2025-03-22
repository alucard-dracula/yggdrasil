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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_UGTABLE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_UGTABLE_OPT_HPP__


#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UGTABLE_OPT_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UGTABLE_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/safe_container/detail/boost_unordered_create_node.hpp>
#include <yggr/safe_container/detail/boost_uiterator_opt.hpp>
#include <yggr/safe_container/detail/boost_unordered_node_handle_opt.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/unordered/detail/implementation.hpp>

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
	typedef boost::unordered::detail::table<Types> org_type;
	typedef org_type table;

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
	typedef typename table::const_key_type const_key_type;
	typedef typename table::node_constructor node_constructor;
	typedef typename table::node_tmp node_tmp;
	typedef typename table::extractor extractor;
	typedef typename table::iterator iterator;
	typedef typename table::c_iterator c_iterator;

	// opt_type
	typedef typename cast_to_u_iterator_opt<iterator>::type opt_iterator_type;
	typedef typename cast_to_u_c_iterator_opt<c_iterator>::type opt_c_iterator_type;

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
		return static_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return static_cast<const org_type&>(*this);
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
	template<typename VAlloc, YGGR_BOOST_UNORDERED_EMPLACE_TEMPLATE> inline
	static node_pointer create_node_1(const VAlloc& valloc, YGGR_BOOST_UNORDERED_EMPLACE_ARGS)
	{
		return 
			__create_u_node_1<node_pointer, node_allocator, node_constructor>(
				valloc, YGGR_BOOST_UNORDERED_EMPLACE_FORWARD);
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

	inline std::size_t count(std::size_t key_hash, const_key_type& k) const
	{
		const org_type& org = org_cast();
		if(0 == org.size_)
		{
			return 0;
		}
		node_pointer n = org.find_node(key_hash, k);
		return n? org.group_count(n) : 0;
	}

	inline std::pair<iterator, iterator>
		equal_range(std::size_t key_hash, const_key_type& k) const
	{
		const org_type& org = org_cast();
		node_pointer n = org.find_node(key_hash, k);
		return std::make_pair(iterator(n), iterator(n? org.next_group(n) : n));
	}

	// Equality

	inline iterator emplace_hint_impl(c_iterator hint, std::size_t key_hash, node_pointer n)
	{
		assert(n);

		org_type& org = org_cast();
		opt_c_iterator_type& opt_hint = opt_c_iterator_cast(hint);

		node_tmp a(n, org.node_alloc());
		const_key_type& k = org.get_key(a.node_);

		if(opt_hint.node_ && org.key_eq()(k, org.get_key(opt_hint.node_)))
		{
			org.reserve_for_insert(org.size_ + 1);
			return iterator(org.add_using_hint_equiv(a.release(), opt_hint.node_));
		}
		else
		{
			node_pointer position = org.find_node(key_hash, k);
			org.reserve_for_insert(org.size_ + 1);
			return iterator(org.add_node_equiv(a.release(), key_hash, position));
		}
	}

	inline void insert_impl_no_rehash(std::size_t key_hash, node_pointer pnode)
	{
		assert(pnode);
		org_type& org = org_cast();

		node_tmp a(pnode, org.node_alloc());
		org.add_node_equiv(a.release(), key_hash, org.find_node(key_hash, org.get_key(pnode)));
	}

	inline iterator insert(std::size_t key_hash, node_pointer pnode)
	{
		assert(pnode);

		org_type& org = org_cast();
		node_pointer position = org.find_node(key_hash, org.get_key(pnode));

		// reserve has basic exception safety if the hash function
		// throws, strong otherwise.
		node_tmp a(pnode, org.node_alloc());
		org.reserve_for_insert(org.size_ + 1);
		return iterator(org.add_node_equiv(a.release(), key_hash, position));
	}

	template<typename KI, typename I>
	void insert_range(KI ki, KI kj, I i, I j,
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
								void*
							>::type = 0)
	{
		typedef typename boost::iterator_value<I>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<node_pointer, chk_type>));

		if((ki == kj) || (i == j))
		{
			return;
		}

		std::size_t distance = static_cast<std::size_t>(std::distance(i, j));

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
	void insert_range(KI ki, KI kj, I i, I j,
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
								void*
							>::type = 0)
	{
		typedef typename boost::iterator_value<I>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<node_pointer, chk_type>));
		assert((std::distance(ki, kj) == std::distance(i, j)));

		for (; i != j; ++i, ++ki)
		{
			this_type::insert(*ki, *i);
		}
	}

	template<typename I>
	void insert_range(std::size_t key_hash, I i, I j,
						typename 
							boost::enable_if
							<
								boost::is_same
								<
									typename iterator_ex::iterator_value_ex<I>::type,
									node_pointer
								>,
								void*
							>::type = 0)
	{
		typedef typename boost::iterator_value<I>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<node_pointer, chk_type>));

		for (; i != j; ++i) 
		{
			this_type::insert(key_hash, *i);
		}
	}

	////////////////////////////////////////////////////////////////////////
	// Erase

	// start 163

	template<typename Destroy_Holder> inline
	std::size_t erase_key(Destroy_Holder& holder, std::size_t key_hash, const_key_type& k)
	{
		org_type& org = org_cast();

		if(!org.size_)
		{
			return 0;
		}

		std::size_t bucket_index = org.hash_to_bucket(key_hash);
		link_pointer prev = org.find_previous_node(k, bucket_index);

		if(!prev)
		{
			return 0;
		}

		node_pointer first_node = org_type::next_node(prev);
		link_pointer end = org_type::next_group(first_node);

		std::size_t delete_count = this_type::delete_nodes(holder, prev, end);
		org.fix_bucket(bucket_index, prev, static_cast<node_pointer>(prev->next_));
		return delete_count;
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder& holder, c_iterator r)
	{
		opt_c_iterator_type& r_opt = opt_c_iterator_cast(r);
		BOOST_ASSERT(r_opt.node_);
		node_pointer next = org_type::next_node(r_opt.node_);
		erase_nodes(holder, r_opt.node_, next);
		return iterator(next);
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
		org_type& org = org_cast();

		if(!org.size_)
		{
			return;
		}

		delete_nodes(holder, org.get_previous_start(), link_pointer());
		org.clear_buckets();

		BOOST_ASSERT(!org.size_);
	}

//private:
	// start 163
	template<typename Destroy_Holder>
	link_pointer erase_nodes(Destroy_Holder& holder, node_pointer i, node_pointer j)
	{
		org_type& org = org_cast();

		std::size_t bucket_index = org.node_bucket(i);

		// Split the groups containing 'i' and 'j'.
		// And get the pointer to the node before i while
		// we're at it.

		//link_pointer prev = org_type::split_groups(i, j);
		link_pointer prev = this->get_previous_start(bucket_index);
		while (prev->next_ != i) 
		{
			prev = org_type::next_node(prev);
		}

		// Delete the nodes.
		bool includes_first = false;
		do
		{
			includes_first = includes_first || static_cast<node_pointer>(prev->next_)->is_first_in_group();
			this_type::delete_node(holder, prev);
			bucket_index = org.fix_bucket(bucket_index, prev, static_cast<node_pointer>(prev->next_));
		} while(prev->next_ != j);

		if(j && includes_first) 
		{
            j->set_first_in_group();
        }

		return prev;
	}

	template<typename Destroy_Holder> inline
	void delete_node(Destroy_Holder& holder, link_pointer prev)
	{
		node_pointer n = static_cast<node_pointer>(prev->next_);
		prev->next_ = n->next_;
		if(prev->next_)
		{
			static_cast<node_pointer>(prev->next_)->set_first_in_group();
		}

		holder(n);
		--(org_cast().size_);
	}

	template<typename Destroy_Holder> inline
	void delete_node_no_count(Destroy_Holder& holder, link_pointer prev)
	{
		node_pointer n = static_cast<node_pointer>(prev->next_);
		prev->next_ = n->next_;
		if(prev->next_)
		{
			static_cast<node_pointer>(prev->next_)->set_first_in_group();
		}

		holder(n);
	}

	template<typename Destroy_Holder>
	std::size_t delete_nodes(Destroy_Holder& holder, link_pointer prev, link_pointer end)
	{
		BOOST_ASSERT(prev->next_ != end);

		std::size_t count = 0;

		do
		{
			this_type::delete_node_no_count(holder, prev);
			++count;
		} while(prev->next_ != end);

		(org_cast().size_) -= count;
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
            const_key_type& k = org.get_key(boost_unordered_node_handler_opt_cast(np).ptr_);
			assert(key_hash == org.hash(k));
			node_pointer pos = org.find_node(key_hash, k);
            org.reserve_for_insert(org.size_ + 1);
			result = iterator(org.add_node_equiv(boost_unordered_node_handler_opt_cast(np).ptr_, key_hash, pos));
            boost_unordered_node_handler_opt_cast(np).ptr_ = node_pointer();
        }

        return result;
    }

public:
	template<typename NodeType> inline
    iterator move_insert_node_type_with_hint(std::size_t key_hash, c_iterator hint, NodeType& np)
    {
		org_type& org = org_cast();

        iterator result;

        if(np) 
		{
			const_key_type& k = org.get_key(boost_unordered_node_handler_opt_cast(np).ptr_);
			opt_c_iterator_type& opt_hint = opt_c_iterator_cast(hint);

            if(opt_hint.node_ && org.key_eq()(k, org.get_key(opt_hint.node_))) 
			{
                org.reserve_for_insert(org.size_ + 1);
				result = 
					iterator(
						org.add_using_hint_equiv(
							boost_unordered_node_handler_opt_cast(np).ptr_, 
							opt_hint.node_));
            }
			else 
			{
				assert(org.hash(k) == key_hash);
				node_pointer pos = org.find_node(key_hash, k);
                org.reserve_for_insert(org.size_ + 1);

				result = 
					iterator(
						org.add_node_equiv(
							boost_unordered_node_handler_opt_cast(np).ptr_, key_hash, pos));
            }

            boost_unordered_node_handler_opt_cast(np).ptr_ = node_pointer();
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
struct cast_to_ugtable_opt< boost::unordered::detail::table<Types> >
{
	typedef boost_ugtable_opt<Types> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_UGTABLE_OPT_HPP__
