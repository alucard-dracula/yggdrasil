// boost_utable_opt_basic.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UTABLE_BASIC_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UTABLE_BASIC_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UTABLE_OPT_BASIC_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UTABLE_OPT_BASIC_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/safe_container/detail/boost_unordered_create_node.hpp>
#include <yggr/safe_container/detail/boost_uiterator_opt.hpp>
#include <yggr/safe_container/detail/boost_unordered_node_handle_opt.hpp>

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

template <typename Types>
struct boost_utable_opt_basic
	: boost::unordered::detail::table<Types>
{
public:
	typedef boost::unordered::detail::table<Types> org_type;
	typedef org_type table;

	typedef typename table::value_type value_type;
	typedef typename table::node_pointer node_pointer;
	typedef typename table::node_allocator_type node_allocator;

	typedef typename table::bucket_type bucket_type;
	typedef typename table::bucket_iterator bucket_iterator;

	typedef typename table::hasher hasher;
	typedef typename table::key_equal key_equal;
	typedef typename table::const_key_type const_key_type;
	typedef typename table::node_constructor node_constructor;
	typedef typename table::node_tmp node_tmp;
	typedef typename table::extractor extractor;
	typedef typename table::iterator iterator;
	typedef typename table::c_iterator c_iterator;

	//opt_type
	typedef typename cast_to_u_iterator_opt<iterator>::type opt_iterator_type;
	typedef typename cast_to_u_c_iterator_opt<c_iterator>::type opt_c_iterator_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(opt_iterator_type) == sizeof(iterator))>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(opt_c_iterator_type) == sizeof(c_iterator))>));

private:
	typedef boost_utable_opt_basic this_type;

private:
	boost_utable_opt_basic(void);
	boost_utable_opt_basic(BOOST_RV_REF(boost_utable_opt_basic));
	boost_utable_opt_basic(const boost_utable_opt_basic&);
	~boost_utable_opt_basic(void);
	const boost_utable_opt_basic& operator=( const boost_utable_opt_basic& );
	const boost_utable_opt_basic& operator=( BOOST_RV_REF(boost_utable_opt_basic) );

protected:
	inline org_type& org_cast(void)
	{
		return static_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return static_cast<const org_type&>(*this);
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
	template<typename Destroy_Holder>
	iterator erase_range(Destroy_Holder& holder, c_iterator first, c_iterator last)
	{
		opt_c_iterator_type& opt_first = opt_c_iterator_type::opt_cast(first);
		opt_c_iterator_type& opt_last = opt_c_iterator_type::opt_cast(last);

		if(first == last)
		{
			return opt_iterator_type::make_org(opt_last.p, opt_last.itb);
		}

		// though `first` stores of a copy of a pointer to a node, we wish to
		// mutate the pointers stored internally by the singly-linked list in
		// each bucket group so we have to retrieve it manually by iterating
		//
		bucket_iterator itb = opt_first.itb;
		node_pointer* pp = boost::addressof(itb->next);
		while (*pp != opt_first.p) 
		{
			pp = boost::addressof((*pp)->next);
		}

		while (*pp != opt_last.p) 
		{
			node_pointer p = *pp;
			*pp = (*pp)->next;

			this_type::delete_node(holder, p);

			bool const at_end = !(*pp);
			bool const is_empty_bucket = !(itb->next);

			if(at_end) 
			{
				if(is_empty_bucket) 
				{
					org_type::buckets_.unlink_bucket(itb++);
				} 
				else 
				{
					++itb;
				}
				pp = boost::addressof(itb->next);
			}
		}

		return opt_iterator_type::make_org(opt_last.p, opt_last.itb);
	}

	template<typename Destroy_Holder> inline
	void clear(Destroy_Holder& holder)
	{
		if(!org_type::size_)
		{
			return;
		}

		this_type::delete_buckets(holder);

		BOOST_ASSERT(!org_type::size_);
	}

	template<typename Destroy_Holder> inline
	void delete_node(Destroy_Holder& holder, node_pointer p)
	{
		if(p)
		{
			holder(p);
		}
		--(org_type::size_);
	}

	template<typename Destroy_Holder>
	void delete_nodes(Destroy_Holder& holder)
	{
		iterator pos = org_type::begin(), last = org_type::end();
		opt_iterator_type& opt_pos = opt_iterator_type::opt_cast(pos);

		for (; pos != last;)
		{
			node_pointer p = opt_pos.p;
			bucket_iterator itb = opt_pos.itb;
			++pos;
			org_type::buckets_.extract_node(itb, p);
			this_type::delete_node(holder, p);
		}

		assert(org_type::size_ == 0);
	}

	template<typename Destroy_Holder> inline
	void delete_buckets(Destroy_Holder& holder)
	{
		this_type::delete_nodes(holder);
		org_type::buckets_.clear();
	}

	// find_node
	template <typename Key> inline
	iterator find_node(std::size_t key_hash, const Key& k) const
    {
		assert(key_hash == org_type::hash(k));
		bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash);
		return this_type::find_node_impl(itb, k, org_type::key_eq());
    }

	template <typename Key, typename Pred> inline
    iterator find_node_impl(std::size_t key_hash, const Key& k, const Pred& pred) const
    {
		assert(key_hash == org_type::hash(k));
		bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash);
		return this_type::find_node_impl(itb, k, pred);
    }

	template <typename Key, typename Pred> inline
    iterator find_node_impl(bucket_iterator itb, const Key& k, const Pred& pred) const
    {
		node_pointer p = this_type::find_node_pointer_impl(itb, k, pred);
		return p? opt_iterator_type::make_org(p, itb) : org_type::end();
    }

	// find_node_pointer
	template <typename Key> inline
	node_pointer find_node_pointer(std::size_t key_hash, const Key& k) const
	{
		assert(key_hash == org_type::hash(k));
		bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash);
		return this_type::find_node_pointer_impl(itb, k, org_type::key_eq());
    }

	template <typename Key, typename Pred> inline
    node_pointer find_node_pointer_impl(std::size_t key_hash, const Key& k, const Pred& pred) const
    {
		assert(key_hash == org_type::hash(k));
		bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash);
		return this_type::find_node_pointer_impl(itb, k, pred);
    }

	template <typename Key, typename Pred>
    node_pointer find_node_pointer_impl(bucket_iterator itb, const Key& k, const Pred& pred) const
    {
		if(itb == org_type::buckets_.end())
		{
			return node_pointer();
		}
		else
		{
			node_pointer p = itb->next;
			for(; p; p = p->next) 
			{
				if(pred(k, extractor::extract(p->value()))) 
				{
					break;
				}
			}

			return p;
		}
    }

protected:
	inline bucket_iterator pro_get_bucket_iterator(std::size_t key_hash) const
	{
		return org_type::buckets_.at(org_type::buckets_.position(key_hash));
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UTABLE_BASIC_OPT_HPP__
