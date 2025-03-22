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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_UTABLE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_UTABLE_OPT_HPP__

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
		return ((0 == org.size_) || (org.find_node(key_hash, k) == iterator()))? 0 : 1;
	}

	// at
	inline value_type& at(std::size_t key_hash, key_type const& k) const
	{
		const org_type& org = org_cast();

		if(org.size_)
		{
			iterator it = org.find_node(key_hash, k);
			if(it != iterator())
			{
				return *it;
			}
		}

		boost::throw_exception(
			std::out_of_range("Unable to find key in unordered_map."));
	}

	inline std::pair<iterator, iterator>
		equal_range(std::size_t key_hash, key_type const& k) const
	{
		iterator n = org_cast().find_node(key_hash, k);
		iterator n2 = n;
		if(opt_iterator_cast(n2).node_)
		{
			++n2;
		}

		return std::make_pair(n, n2);
	}

	inline value_type& subscript(std::size_t key_hash, key_type const& k)
	{
		typedef typename value_type::second_type mapped_type;

		org_type& org = org_cast();

		iterator pos = org.find_node(key_hash, k);

		if(pos != iterator())
		{
			return *pos;
		}

		// Create the node before rehashing in case it throws an
		// exception (need strong safety in such a case).
		node_constructor a(org.node_alloc());
		using namespace boost::unordered::detail;
		a.construct_with_value(BOOST_UNORDERED_EMPLACE_ARGS3(
			boost::unordered::piecewise_construct,
			boost::make_tuple(k),
			boost::make_tuple()));

		org.reserve_for_insert(org.size_ + 1);
		return *(org.add_node(a, key_hash));
	}

	// !!!don't support this operator, becauset this operator slow of first insert only!!!
	//template<typename Destroy_Holder>
	//value_type& subscript(Destroy_Holder& holder, node_pointer pnode)
	//{
	//	typedef typename value_type::second_type mapped_type;

	//	assert(pnode);
	//	org_type& org = org_cast();
	//	iterator pos = org.find_node(pnode->hash_, org.get_key(pnode->value()));
	//	if(pos != iterator())
	//	{
	//		holder(pnode);
	//		return *pos;
	//	}
	//	org.reserve_for_insert(org.size_ + 1);
	//	return *(this_type::add_node(pnode));
	//}

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#   if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
	inline emplace_return emplace(std::size_t,
									boost::unordered::detail::emplace_args1
									<
										boost::unordered::detail::please_ignore_this_overload
									> const&)
	{
		BOOST_ASSERT(false);
		return emplace_return(org_cast().begin(), false);
	}
#   else
	inline emplace_return emplace(std::size_t,
									boost::unordered::detail::please_ignore_this_overload const&)
	{
		BOOST_ASSERT(false);
		return emplace_return(org_cast().begin(), false);
	}
#   endif
#endif


	template <BOOST_UNORDERED_EMPLACE_TEMPLATE> inline
	emplace_return emplace(std::size_t key_hash, BOOST_UNORDERED_EMPLACE_ARGS)
	{
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
		return emplace_impl(
				key_hash,
				extractor::extract(BOOST_UNORDERED_EMPLACE_FORWARD),
				BOOST_UNORDERED_EMPLACE_FORWARD);
#else
		return emplace_impl(
				key_hash,
				extractor::extract(args.a0, args.a1),
				BOOST_UNORDERED_EMPLACE_FORWARD);
#endif
	}

#if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
	template <typename A0> inline
	emplace_return emplace(
			std::size_t key_hash,
			boost::unordered::detail::emplace_args1<A0> const& args)
	{
		return emplace_impl(key_hash, extractor::extract(args.a0), args);
	}
#endif // BOOST_NO_CXX11_VARIADIC_TEMPLATES

	template <BOOST_UNORDERED_EMPLACE_TEMPLATE> inline
	emplace_return emplace_impl(std::size_t key_hash, key_type const& k,
		BOOST_UNORDERED_EMPLACE_ARGS)
	{
		org_type& org = org_cast();
		iterator pos = org.find_node(key_hash, k);

		if(pos != iterator())
		{
			return emplace_return(pos, false);
		}

		// Create the node before rehashing in case it throws an
		// exception (need strong safety in such a case).
		node_constructor a(org.node_alloc());
		a.construct_with_value(BOOST_UNORDERED_EMPLACE_FORWARD);

		// reserve has basic exception safety if the hash function
		// throws, strong otherwise.
		org.reserve_for_insert(org.size_ + 1);
		return emplace_return(org.add_node(a, key_hash), true);
	}

	inline emplace_return emplace_impl_with_node(std::size_t key_hash, node_constructor& a)
	{
		org_type& org = org_cast();

		key_type const& k = org.get_key(a.value());
		iterator pos = org.find_node(key_hash, k);

		if(pos != iterator())
		{
			return emplace_return(pos, false);
		}

		// reserve has basic exception safety if the hash function
		// throws, strong otherwise.
		org.reserve_for_insert(org.size_ + 1);
		return emplace_return(org.add_node(a, key_hash), true);
	}

	template <BOOST_UNORDERED_EMPLACE_TEMPLATE> inline
	emplace_return emplace_impl(std::size_t key_hash,
								boost::unordered::detail::no_key,
								BOOST_UNORDERED_EMPLACE_ARGS)
	{
		// Don't have a key, so construct the node first in order
		// to be able to lookup the position.
		node_constructor a(org_cast().node_alloc());
		a.construct_with_value(BOOST_UNORDERED_EMPLACE_FORWARD);
		return emplace_impl_with_node(key_hash, a);
	}

	template<typename Destroy_Holder> inline
	emplace_return insert(Destroy_Holder& holder, std::size_t key_hash, node_pointer pnode)
	{
		assert(pnode);
		assert(pnode->hash_ == key_hash);
		org_type& org = org_cast();

		key_type const& k = org.get_key(pnode->value());
		iterator pos = org.find_node(key_hash, k);

		if(pos != iterator())
		{
			holder(pnode);
			return emplace_return(pos, false);
		}

		// reserve has basic exception safety if the hash function
		// throws, strong otherwise.
		org.reserve_for_insert(org.size_ + 1);
		return emplace_return(this_type::add_node(pnode), true);
	}

	template <typename Key_Hash_It, typename InputIt> inline
	void insert_range(Key_Hash_It ki, Key_Hash_It kj, InputIt i, InputIt j)
	{
		assert((std::distance(ki, kj) == std::distance(i, j)));
		if( (i != j) && (ki != kj) )
		{
			return 
				insert_range_impl(
					*ki,
					extractor::extract(*i),
					ki, kj, i, j);
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

	template <typename Key_Hash_It, typename InputIt>
	void insert_range_impl(std::size_t key_hash, key_type const& k,
							Key_Hash_It ki, Key_Hash_It kj,
							InputIt i, InputIt j)
	{
		node_constructor a(org_cast().node_alloc());

		insert_range_impl2(a, key_hash, k, ki, kj, i, j);

		while((++i != j) && (++ki != kj))
		{
			// Note: can't use get_key as '*i' might not be value_type - it
			// could be a pair with first_types as key_type without const or
			// a different second_type.
			//
			// TODO: Might be worth storing the value_type instead of the
			// key here. Could be more efficient if '*i' is expensive. Could
			// be less efficient if copying the full value_type is
			// expensive.
			insert_range_impl2(a,
								*ki,
								extractor::extract(*i),
								ki, kj, i, j);
		}
	}

	template <typename Key_Hash_It, typename InputIt> inline
	void insert_range_impl2(node_constructor& a,
							std::size_t key_hash,
							key_type const& k,
							Key_Hash_It ki, Key_Hash_It kj,
							InputIt i, InputIt j)
	{
		org_type& org = org_cast();
		// No side effects in this initial code
		iterator pos = org.find_node(key_hash, k);

		if(pos == iterator())
		{
			a.construct_with_value2(*i);
			if(org.size_ + 1 > org.max_load_)
			{
				org.reserve_for_insert(org.size_ +
					boost::unordered::detail::insert_size(i, j));
			}

			// Nothing after this point can throw.
			org.add_node(a, key_hash);
		}
	}

	template <typename Key_Hash_It, typename InputIt>
	void insert_range_impl(std::size_t key_hash,
							boost::unordered::detail::no_key,
							Key_Hash_It ki, Key_Hash_It kj,
							InputIt i, InputIt j)
	{
		node_constructor a(org_cast().node_alloc());

		do
		{
			key_hash = *ki;
			a.construct_with_value2(*i);
			emplace_impl_with_node(key_hash, a);
		} while((++i != j) && (++ki != kj));
	}

	// start 156, but just only variable names are different

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
			prev = prev->next_;
		}

		link_pointer end = static_cast<node_pointer>(prev->next_)->next_;

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
        iterator next(opt_r.node_);
        ++next;
        erase_nodes(holder, opt_r.node_, opt_iterator_cast(next).node_);
        return next;
    }

	template<typename Destroy_Holder> inline
    iterator erase_range(Destroy_Holder& holder,
							c_iterator r1, c_iterator r2)
    {
        if(r1 == r2)
		{
			return iterator(opt_c_iterator_cast(r2).node_);
		}
        erase_nodes(holder,
						opt_c_iterator_cast(r1).node_,
						opt_c_iterator_cast(r2).node_);
        return iterator(opt_c_iterator_cast(r2).node_);
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

	// start 156, but just only variable names are different
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

	inline iterator add_node(node_pointer n)
	{
		assert(n);

		org_type& org = org_cast();

		bucket_pointer b = org.get_bucket(org.hash_to_bucket(n->hash_));

		if(!b->next_)
		{
			link_pointer start_node = org.get_previous_start();

			if(start_node->next_)
			{
				org.get_bucket(org.hash_to_bucket(
					static_cast<node_pointer>(start_node->next_)->hash_) )->next_ = n;
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

		++(org.size_);
		return iterator(n);
	}

	inline iterator add_node(node_pointer n, std::size_t key_hash)
	{
		n->hash_ = key_hash;
		return this_type::add_node(n);
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
			iterator pos = org.find_node(key_hash, k);

			if(pos != iterator())
			{
				result.node = boost::move(np);
				result.position = pos;
			} 
			else 
			{
				org.reserve_for_insert(org.size_ + 1);
				result.position = this_type::add_node(np.ptr_, key_hash);
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
		iterator pos = org.find_node(key_hash, k);
        
		if(pos == iterator()) 
		{
			org.reserve_for_insert(org.size_ + 1);
			pos = this_type::add_node(np.ptr_, key_hash);
			np.ptr_ = node_pointer();
		}

		return pos;
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

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_UTABLE_OPT_HPP__
