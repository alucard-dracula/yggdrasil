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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UTABLE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UTABLE_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UTABLE_OPT_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UTABLE_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/safe_container/detail/boost_utable_opt_basic.hpp>
#include <stdexcept>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template <typename Types>
struct boost_utable_opt
	: public boost_utable_opt_basic<Types>
{
public:
	typedef boost_utable_opt_basic<Types> base_type;

public:
	typedef typename base_type::org_type org_type;
	typedef org_type table;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::node_pointer node_pointer;
	typedef typename base_type::node_allocator_type node_allocator;

	typedef typename base_type::bucket_type bucket_type;
	typedef typename base_type::bucket_iterator bucket_iterator;

	typedef typename base_type::hasher hasher;
	typedef typename base_type::key_equal key_equal;
	typedef typename base_type::const_key_type const_key_type;
	typedef typename base_type::node_constructor node_constructor;
	typedef typename base_type::node_tmp node_tmp;
	typedef typename base_type::extractor extractor;
	typedef typename base_type::iterator iterator;
	typedef typename base_type::c_iterator c_iterator;

	typedef std::pair<iterator, bool> emplace_return;

	//opt_type
	typedef typename base_type::opt_iterator_type opt_iterator_type;
	typedef typename base_type::opt_c_iterator_type opt_c_iterator_type;

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
	using base_type::org_cast;

public:
	using base_type::create_node_1;
	using base_type::create_node_2;

public:
	// count
	inline std::size_t count(std::size_t key_hash, const_key_type& k) const
	{
		return ((0 == org_type::size_) || (base_type::find_node(key_hash, k) == org_type::end()))? 0 : 1;
	}

	// at
	inline value_type& at(std::size_t key_hash, const_key_type& k) const
	{
		if(org_type::size_)
		{
			node_pointer n = base_type::find_node_pointer(key_hash, k);
			if(n)
			{
				return n->value();
			}
		}

		boost::throw_exception(
			std::out_of_range("Unable to find key in unordered_map."));
	}

	inline std::pair<iterator, iterator>
		equal_range(std::size_t key_hash, const_key_type& k) const
	{
		iterator i = base_type::find_node(key_hash, k);
		return std::make_pair(i, i == org_type::end()? i : advance_copy(i, 1));
	}

	// boost 164 is deprecated, change using try_emplace, but safe_container not needit
	template<typename Key> inline
	value_type& subscript(std::size_t key_hash, BOOST_FWD_REF(Key) k)
	{
		typedef typename value_type::second_type mapped_type;

		//bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash);
		bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
		iterator iter_pos = this_type::find_node_impl(itb, k, org_type::key_eq());

		if(iter_pos != org_type::end())
		{
			return (opt_iterator_type::opt_cast(iter_pos).p)->value();
		}
		else
		{
			node_allocator alloc = org_type::node_alloc();
			node_tmp tmp(
              boost::unordered::detail::func::construct_node_pair(alloc, boost::forward<Key>(k)),
              alloc);

			if(org_type::size_ + 1 > org_type::max_load_) 
			{
				org_type::reserve(org_type::size_ + 1);
				//itb = this_type::pro_get_bucket_iterator(key_hash);
				itb = base_type::pro_get_bucket_iterator(key_hash);
			}

			node_pointer p = tmp.release();
			this_type::pro_insert_node(itb, p);

			return p->value();
		}
	}

	template<typename Destroy_Holder> inline
	emplace_return insert(Destroy_Holder& holder, std::size_t key_hash, node_pointer pnode)
	{
		assert(pnode);

		const_key_type& k = org_type::get_key(pnode);
		//bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash);
		bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
		iterator iter_pos = this_type::find_node_impl(itb, k, org_type::key_eq());

		if(iter_pos != org_type::end())
		{
			holder(pnode);
			return emplace_return(iter_pos, false);
		}
		else
		{
			node_tmp b(pnode, org_type::node_alloc());

			if(org_type::size_ + 1 > org_type::max_load_) 
			{
				org_type::reserve(org_type::size_ + 1);
				//itb = this_type::pro_get_bucket_iterator(key_hash);
				itb = base_type::pro_get_bucket_iterator(key_hash);
			}

			node_pointer p = b.release();
			this_type::pro_insert_node(itb, p);

			return emplace_return(opt_iterator_type::make_org(p, itb), true);
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

		node_allocator alloc = org_type::node_alloc();

		for(;(ki != kj) && (i != j); ++ki, ++i)
		{
			node_pointer pi = *i;
			if(!pi)
			{
				continue;
			}

			value_type const& value = pi->value();
			const_key_type& key = extractor::extract(value);
			std::size_t const h = *ki;

			//bucket_iterator itb = this_type::pro_get_bucket_iterator(h);
			bucket_iterator itb = base_type::pro_get_bucket_iterator(h);
			node_pointer it = this_type::find_node_pointer_impl(itb, key, org_type::key_eq());
			if(it)
			{
				holder(pi);
				continue;
			}

			if(org_type::size_ + 1 > org_type::max_load_) 
			{
				org_type::reserve(org_type::size_ + 1);
				itb = base_type::pro_get_bucket_iterator(h);
			}

			node_tmp tmp(pi, alloc);
			this_type::pro_insert_node(itb, tmp.release());
		}
	}

	template<typename Destroy_Holder> inline
	std::size_t erase_key(Destroy_Holder& holder, std::size_t key_hash, const_key_type& k)
	{
		if(!org_type::size_)
		{
			return 0;
		}

		//bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash); // why using base_type in cpp20?
		bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
		node_pointer* pp = org_type::find_prev(k, itb);
		if(!pp)
		{
			return 0;
		}

		node_pointer p = *pp;
		org_type::buckets_.extract_node_after(itb, pp);
		//this_type::delete_node(holder, p);
		base_type::delete_node(holder, p);
		return 1;
	}

	template<typename Destroy_Holder>
	iterator erase(Destroy_Holder& holder, c_iterator pos)
	{
		c_iterator next = pos;
		++next;

		opt_c_iterator_type& opt_pos = opt_c_iterator_type::opt_cast(pos);
		opt_c_iterator_type& opt_next = opt_c_iterator_type::opt_cast(next);
          
		bucket_iterator itb = opt_pos.itb;
		node_pointer* pp = boost::addressof(itb->next);

		while (*pp != opt_pos.p) 
		{
			pp = boost::addressof((*pp)->next);
		}

		org_type::buckets_.extract_node_after(itb, pp);
		//this_type::delete_node(holder, opt_pos.p);
		base_type::delete_node(holder, opt_pos.p);

		return opt_iterator_type::make_org(opt_next.p, opt_next.itb);
	}

	using base_type::erase_range;
	using base_type::clear;
	using base_type::delete_node;
	using base_type::delete_nodes;
	using base_type::delete_buckets;

	template<typename NodeType, typename InsertReturnType> inline
	void move_insert_node_type(std::size_t key_hash, NodeType& np, InsertReturnType& result)
	{
		if(!np) 
		{
			result.position = org_type::end();
			result.inserted = false;
			return;
		}

		node_pointer& npp = boost_unordered_node_handler_opt_cast(np).ptr_;
		assert(key_hash == org_type::hash(org_type::get_key(npp)));
		const_key_type& k = org_type::get_key(npp);
		//bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash);
		bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
		node_pointer p = this_type::find_node_pointer_impl(itb, k, org_type::key_eq());

		if(p) 
		{
			result.node = boost::move(np);
			result.position = opt_iterator_type::make_org(p, itb);
			result.inserted = false;
			return;
		}

		p = npp;

		if(org_type::size_ + 1 > org_type::max_load_) 
		{
			org_type::reserve(org_type::size_ + 1);
			//itb = this_type::pro_get_bucket_iterator(key_hash);
			itb = base_type::pro_get_bucket_iterator(key_hash);
		}

		this_type::pro_insert_node(itb, p);
		npp = node_pointer();

		result.position = opt_iterator_type::make_org(p, itb);
		result.inserted = true;
	}

	template<typename NodeType> inline
	iterator move_insert_node_type_with_hint(std::size_t key_hash, c_iterator hint, NodeType& np)
	{
		if(!np) 
		{
			return org_type::end();
		}

		opt_c_iterator_type& opt_hint = opt_c_iterator_type::opt_cast(hint);
		node_pointer& npp = boost_unordered_node_handler_opt_cast(np).ptr_;
		const_key_type& k = org_type::get_key(npp);

		if(opt_hint.p && org_type::key_eq()(k, org_type::get_key(opt_hint.p))) 
		{
			return opt_iterator_type::make_org(opt_hint.p, opt_hint.itb);
		}

		//bucket_iterator itb = this_type::pro_get_bucket_iterator(key_hash);
		bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
		node_pointer p = this_type::find_node_pointer_impl(itb, k, org_type::key_eq());
		if(p) 
		{
			return opt_iterator_type::make_org(p, itb);
		}

		p = npp;

		if(org_type::size_ + 1 > org_type::max_load_) 
		{
			org_type::reserve(org_type::size_ + 1);
			//itb = this_type::pro_get_bucket_iterator(key_hash);
			itb = base_type::pro_get_bucket_iterator(key_hash);
		}

		this_type::pro_insert_node(itb, p);
		npp = node_pointer();
		return opt_iterator_type::make_org(p, itb);
	}

	// find_node
	using base_type::find_node;
	using base_type::find_node_impl;

	// find_node_pointer
	using base_type::find_node_pointer;
	using base_type::find_node_pointer_impl;

protected:
	using base_type::pro_get_bucket_iterator;

	inline void pro_insert_node(bucket_iterator itb, node_pointer pnode)
	{
		assert(pnode);
		org_type::buckets_.insert_node(itb, pnode);
		++org_type::size_;
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
struct cast_to_utable_opt< boost::unordered::detail::table<Types> >
{
	typedef boost_utable_opt<Types> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UTABLE_OPT_HPP__
