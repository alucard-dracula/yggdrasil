// boost_utable_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_BOOST_UTABLE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_BOOST_UTABLE_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UTABLE_OPT_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UTABLE_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/safe_container/detail/boost_unordered_create_node.hpp>
#include <yggr/safe_container/detail/boost_uiterator_opt.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/unordered/detail/unique.hpp>

#include <iterator>
#include <cassert>
#include <stdexcept>


namespace yggr
{
namespace safe_container
{
namespace detail
{

template <typename Types>
struct boost_utable_opt
	: boost::unordered::detail::table<Types>
{
public:
	typedef boost::unordered::detail::table_impl<Types> org_type;
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
	typedef typename table::node_tmp node_tmp;
	typedef typename table::extractor extractor;
	typedef typename table::iterator iterator;
	typedef typename table::c_iterator c_iterator;

	typedef std::pair<iterator, bool> emplace_return;

	//opt_type
	typedef typename cast_to_u_iterator_opt<iterator>::type opt_iterator_type;
	typedef typename cast_to_u_c_iterator_opt<c_iterator>::type opt_c_iterator_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(opt_iterator_type) == sizeof(iterator))>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(opt_c_iterator_type) == sizeof(c_iterator))>));

private:
	typedef boost_utable_opt this_type;

private:
	boost_utable_opt(void);
	boost_utable_opt(BOOST_RV_REF(boost_utable_opt));
	boost_utable_opt(const boost_utable_opt&);
	~boost_utable_opt(void);
	const boost_utable_opt& operator=( const boost_utable_opt& );
	const boost_utable_opt& operator=( BOOST_RV_REF(boost_utable_opt) );

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
	// count
	inline std::size_t count(std::size_t key_hash, key_type const& k) const
	{
		const org_type& org = org_cast();
		return (org.size_ && org_cast().find_node(key_hash, k))? 1 : 0;
	}

	// at
	inline value_type& at(std::size_t key_hash, key_type const& k) const
	{
		const org_type& org = org_cast();

		if(org.size_)
		{
			node_pointer n = org.find_node(key_hash, k);
			if(n)
			{
				return n->value();
			}
		}

		boost::throw_exception(
			std::out_of_range("Unable to find key in unordered_map."));
	}

	inline std::pair<iterator, iterator>
		equal_range(std::size_t key_hash, key_type const& k) const
	{
		const org_type& org = org_cast();

		node_pointer n = org.find_node(key_hash, k);
		return std::make_pair(iterator(n), iterator(n ? org.next_node(n) : n));
	}

	inline value_type& subscript(std::size_t key_hash, key_type const& k)
	{
		typedef typename value_type::second_type mapped_type;
		org_type& org = org_cast();

		node_pointer pos = org.find_node(key_hash, k);

		if(pos)
		{
			return pos->value();
		}
		else
		{
			return (org.resize_and_add_node(
						boost::unordered::detail::func::construct_node_pair(org.node_alloc(), k),
						key_hash))->value();
		}
	}

	template<typename Destroy_Holder> inline
	emplace_return insert(Destroy_Holder& holder, std::size_t key_hash, node_pointer pnode)
	{
		assert(pnode);
		assert(pnode->hash_ == key_hash);
		org_type& org = org_cast();

		key_type const& k = org.get_key(pnode->value());
		node_pointer pos = org.find_node(key_hash, k);

		if(pos)
		{
			holder(pnode);
			return emplace_return(iterator(pos), false);
		}
		else
		{
			node_tmp b(pnode, org.node_alloc());
			return emplace_return(
				iterator(org.resize_and_add_node(b.release(), key_hash)),
				true);
		}
	}

	template<typename Destroy_Holder, typename Key_Hash_It, typename InputIt>
	void insert_range(Destroy_Holder& holder, 
						Key_Hash_It ki, Key_Hash_It kj,
						InputIt i, InputIt j)
	{
		typedef typename boost::iterator_value<InputIt>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<node_pointer, chk_type>));
		assert((std::distance(ki, kj) == std::distance(i, j)));

		for(;(ki != kj) && (i != j); ++ki, ++i)
		{
			this_type::insert(holder, *ki, *i);
		}
	}

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

			std::size_t node_hash = org.next_node(prev)->hash_;

			if(org.hash_to_bucket(node_hash) != bucket_index)
			{
				return 0;
			}

			if(node_hash == key_hash &&
					org.key_eq()(k, org.get_key(org.next_node(prev)->value())))
			{
				break;
			}
			prev = prev->next_;
		}

		link_pointer end = org.next_node(prev)->next_;

		std::size_t count = this_type::delete_nodes(holder, prev, end);
		org.fix_bucket(bucket_index, prev);
		return count;
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder& holder,
					c_iterator r)
    {
		opt_c_iterator_type& opt_r = opt_c_iterator_cast(r);
        BOOST_ASSERT(opt_r.node_);
        node_pointer next = org_cast().next_node(opt_r.node_);
        erase_nodes(holder, opt_r.node_, next);
        return iterator(next);
    }

	template<typename Destroy_Holder> inline
    iterator erase_range(Destroy_Holder& holder,
							c_iterator r1, c_iterator r2)
    {
        if(r1 == r2)
		{
			return iterator(opt_c_iterator_cast(r2).node_);
		}
		else
		{
			erase_nodes(holder,
							opt_c_iterator_cast(r1).node_,
							opt_c_iterator_cast(r2).node_);
			return iterator(opt_c_iterator_cast(r2).node_);
		}
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

	// start 163
	template<typename Destroy_Holder>
	void erase_nodes(Destroy_Holder& holder, node_pointer i, node_pointer j)
	{
		org_type& org = org_cast();

		std::size_t bucket_index = org.hash_to_bucket(i->hash_);

		// Find the node before i.
		link_pointer prev = org.get_previous_start(bucket_index);
		while(prev->next_ != i) prev = prev->next_;

		// Delete the nodes.
		do
		{
			this_type::delete_node(holder, prev);
			bucket_index = org.fix_bucket(bucket_index, prev);
		} while (prev->next_ != j);
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
		} while (prev->next_ != end);

		return count;
	}

	template<typename NodeType, typename InsertReturnType> inline
	void move_insert_node_type(std::size_t key_hash, NodeType& np, InsertReturnType& result)
	{
		org_type& org = org_cast();

		if(np) 
		{
			const key_type& k = org.get_key(np.ptr_->value());
			assert(org.hash(k) == key_hash);
			node_pointer pos = org.find_node(key_hash, k);

			if(pos)
			{
				result.node = boost::move(np);
				result.position = iterator(pos);
			} 
			else 
			{
				org.reserve_for_insert(org.size_ + 1);
				result.position = iterator(org.add_node(np.ptr_, key_hash));
				result.inserted = true;
				np.ptr_ = node_pointer();
			}
		}
	}

	template<typename NodeType> inline
	iterator move_insert_node_type_with_hint(std::size_t key_hash, c_iterator hint, NodeType& np)
	{
		org_type& org = org_cast();

		if(!np)
		{
			return iterator();
		}
		const key_type& k = org.get_key(np.ptr_->value());

		if(opt_c_iterator_cast(hint).node_ 
			&& org.key_eq()(k, org.get_key(*hint))) 
		{
			return iterator(opt_c_iterator_cast(hint).node_);
		}

		assert(key_hash == org.hash(k));
		node_pointer pos = org.find_node(key_hash, k);
        
		if(!pos) 
		{
			org.reserve_for_insert(org.size_ + 1);
			pos = org.add_node(np.ptr_, key_hash);
			np.ptr_ = node_pointer();
		}

		return iterator(pos);
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
struct cast_to_utable_opt< boost::unordered::detail::table_impl<Types> >
{
	typedef boost_utable_opt<Types> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_BOOST_UTABLE_OPT_HPP__
