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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UGTABLE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UGTABLE_OPT_HPP__


#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UGTABLE_OPT_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UGTABLE_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/safe_container/detail/boost_utable_opt_basic.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Types>
struct boost_ugtable_opt
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

	//opt_type
	typedef typename base_type::opt_iterator_type opt_iterator_type;
	typedef typename base_type::opt_c_iterator_type opt_c_iterator_type;

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
	using base_type::org_cast;

public:
	using base_type::create_node_1;
	using base_type::create_node_2;

public:
	// Accessors

	inline std::size_t count(std::size_t key_hash, const_key_type& k) const
	{
		if(0 == org_type::size_)
		{
			return 0;
		}
		c_iterator iter = base_type::find_node(key_hash, k);
		return iter == org_type::end()? 0 : this_type::pro_group_count(iter);
	}

	inline std::pair<iterator, iterator>
		equal_range(std::size_t key_hash, const_key_type& k) const
	{
		iterator n = base_type::find_node(key_hash, k);
		return std::make_pair(n, org_type::next_group(k, n));
	}

	// Equality

	inline iterator emplace_hint_impl(c_iterator hint, std::size_t key_hash, node_pointer n)
	{
		opt_c_iterator_type& opt_hint = opt_c_iterator_type::opt_cast(hint);

		node_tmp a(n, org_type::node_alloc());
		const_key_type& k = org_type::get_key(a.node_);
		bucket_iterator itb = opt_hint.itb;
		node_pointer p = opt_hint.p;

		bool const needs_rehash = (org_type::size_ + 1 > org_type::max_load_);
		bool const usable_hint = (p && org_type::key_eq()(k, org_type::get_key(p)));

		if(!usable_hint) 
		{
			itb = org_type::buckets_.at(org_type::buckets_.position(key_hash));
			p = this_type::find_node_pointer, impl(k, key_hash, org_type::key_eq(), itb);
		} 

		if(needs_rehash) 
		{
			org_type::reserve(org_type::size_ + 1);
			itb = base_type::pro_get_bucket_iterator(key_hash);
		}

		a.release();
		this_type::pro_insert_node_hint(itb, n, p);
		return opt_iterator_type::make_org(n, itb);
	}

	inline void insert_impl_no_rehash(std::size_t key_hash, node_pointer n)
	{
		assert(n);
		BOOST_ASSERT((org_type::size_ + 1 <= org_type::max_load_));

		node_tmp a(n, org_type::node_alloc());
		const_key_type& k = org_type::get_key(a.node_);
		bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
		node_pointer hint = this_type::find_node_pointer_impl(itb, k, org_type::key_eq());

		a.release();
		this_type::pro_insert_node_hint(itb, n, hint);
	}

	inline iterator insert(std::size_t key_hash, node_pointer n)
	{
		assert(n);

		node_tmp a(n, org_type::node_alloc());
		const_key_type& k = org_type::get_key(a.node_);
		bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
		node_pointer hint = this_type::find_node_pointer_impl(itb, k, org_type::key_eq());

		if(org_type::size_ + 1 > org_type::max_load_) 
		{
			org_type::reserve(org_type::size_ + 1);
			itb = base_type::pro_get_bucket_iterator(key_hash);
        }

		 node_pointer p = a.release();
		 this_type::pro_insert_node_hint(itb, p, hint);

		 return opt_iterator_type::make_org(p, itb);
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

		std::size_t distance = static_cast<std::size_t>(std::distance(i, j));

		if(distance == 1)
		{
			assert(*i);
			this_type::insert(*ki, *i);
		}
		else
		{
			// Only require basic exception safety here
			org_type::reserve_for_insert(org_type::size_ + distance);
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
		assert((std::distance(ki, kj) == std::distance(i, j)));

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

		for (; i != j; ++i) 
		{
			this_type::insert(key_hash, *i);
		}
	}

	////////////////////////////////////////////////////////////////////////
	// Erase

	template<typename Destroy_Holder> inline
	std::size_t erase_key(Destroy_Holder& holder, std::size_t key_hash, const_key_type& k)
	{
		std::size_t deleted_count = 0;

		bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
		node_pointer* pp = org_type::find_prev(k, itb);
		if(pp) 
		{
			while (*pp && org_type::key_eq()(org_type::get_key(*pp), k)) 
			{
				node_pointer p = *pp;
				*pp = (*pp)->next;

				base_type::delete_node(holder, p);
				++deleted_count;
			}

			if(!itb->next) 
			{
				org_type::buckets_.unlink_bucket(itb);
			}
		}
		return deleted_count;
	}

	template<typename Destroy_Holder> inline
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

		base_type::delete_node(holder, opt_pos.p);

		return opt_iterator_type::make_org(opt_next.p, opt_next.itb);
	}

	using base_type::erase_range;
	using base_type::clear;
	using base_type::delete_node;
	using base_type::delete_nodes;
	using base_type::delete_buckets;

public:
	template<typename NodeType> inline
	iterator move_insert_node_type(std::size_t key_hash, NodeType& np)
    {
		iterator result;

		if(np) 
		{
			org_type::reserve_for_insert(org_type::size_ + 1);

			node_pointer& npp = boost_unordered_node_handler_opt_cast(np).ptr_;
			const_key_type& k = org_type::get_key(npp);

			bucket_iterator itb = base_type::pro_get_bucket_iterator(key_hash);
			node_pointer hint = this_type::find_node_pointer_impl(itb, k, org_type::key_eq());

			this_type::pro_insert_node_hint(itb, npp, hint);
			result = opt_iterator_type::make_org(npp, itb);
			npp = node_pointer();
		}

		return result;
    }

public:

	template<typename NodeType> inline
    iterator move_insert_node_type_with_hint(std::size_t key_hash, c_iterator hint, NodeType& np)
    {
		iterator result;
		if(np) 
		{
			node_pointer& npp = boost_unordered_node_handler_opt_cast(np).ptr_;
			opt_c_iterator_type& opt_hint = opt_c_iterator_type::opt_cast(hint);

			bucket_iterator itb = opt_hint.itb;
			node_pointer pos = opt_hint.p;
			const_key_type& k = org_type::get_key(npp);
			if(org_type::size_ + 1 > org_type::max_load_) 
			{
				org_type::reserve(org_type::size_ + 1);
				itb = base_type::pro_get_bucket_iterator(key_hash);
			}

			if(!(opt_hint.p && org_type::key_eq()(k, org_type::get_key(opt_hint.p))))
			{
				itb = base_type::pro_get_bucket_iterator(key_hash);
				pos = this_type::find_node_pointer_impl(itb, k, org_type::key_eq());
			}

			this_type::pro_insert_node_hint(itb, npp, pos);
			result = opt_iterator_type::make_org(npp, itb);
			npp = node_pointer();
		}

		return result;
    }

	// find_node

	using base_type::find_node;
	using base_type::find_node_impl;

	// find_node_pointer

	using base_type::find_node_pointer;
	using base_type::find_node_pointer_impl;

protected:
	std::size_t pro_group_count(bucket_iterator itb, const_key_type& k) const
	{

		assert(org_type::size_);

		if(itb == org_type::buckets_.end())
		{
			return 0;
		}
		else
		{
			std::size_t c = 0;
			bool found = false;

			for(node_pointer pos = itb->next; pos; pos = pos->next)
			{
				if((org_type::key_eq())(k, org_type::get_key(pos))) 
				{
					++c;
					found = true;
				}
				else if(found) 
				{
					break;
				}
			}

			return c;
		}
	}

	inline std::size_t pro_group_count(c_iterator pos) const
	{
		opt_c_iterator_type& opt_pos = opt_c_iterator_type::opt_cast(pos);
		return this_type::pro_group_count(opt_pos.itb, org_type::get_key(opt_pos.p));
	}

protected:

	using base_type::pro_get_bucket_iterator;

	inline void pro_insert_node_hint(bucket_iterator itb, node_pointer pnode, node_pointer hint)
	{
		assert(pnode);
		org_type::buckets_.insert_node_hint(itb, pnode, hint);
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
struct cast_to_ugtable_opt< boost::unordered::detail::table<Types> >
{
	typedef boost_ugtable_opt<Types> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UGTABLE_OPT_HPP__
