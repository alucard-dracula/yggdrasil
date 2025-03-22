//boost_uset_ha_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_BOOST_USET_HA_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_BOOST_USET_HA_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETIAL_BOOST_USET_HA_OPT_HPP__
#	error "this file is inner file, can't include it, please include <yggr/safe_container/safe_set.hpp>"
#endif // __YGGR_SAFE_CONTAINER_DETIAL_BOOST_USET_HA_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>

#include <yggr/container/detail/node_handle_unordered_set.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>

#include <yggr/safe_container/detail/hash_helper.hpp>
#include <yggr/safe_container/detail/boost_utable_opt.hpp>
#include <yggr/safe_container/detail/allocator_destroy_holder_unordered.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/unordered_set.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template< typename T, typename H, typename P, typename A>
class boost_uset_ha_opt
{
public:
	typedef boost::unordered_set<T, H, P, A> org_type;

	typedef typename org_type::key_type key_type;
	typedef typename org_type::value_type value_type;
	typedef typename org_type::hasher hasher;
	typedef typename org_type::key_equal key_equal;
	typedef typename org_type::allocator_type allocator_type;

protected:
	typedef boost::unordered::detail::set<A, T, H, P> types;
	typedef typename types::traits allocator_traits;
	typedef typename types::table table;
	typedef typename types::node node;

	typedef typename cast_to_utable_opt<table>::type opt_table_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(table) == sizeof(opt_table_type))>));

public:
	typedef typename org_type::pointer pointer;
	typedef typename org_type::const_pointer const_pointer;

	typedef typename org_type::reference reference;
	typedef typename org_type::const_reference const_reference;

	typedef typename org_type::size_type size_type;
	typedef typename org_type::difference_type difference_type;

	typedef typename org_type::const_local_iterator const_local_iterator;
	typedef typename org_type::local_iterator local_iterator;
	typedef typename org_type::const_iterator const_iterator;
	typedef typename org_type::iterator iterator;

public:
	typedef container::detail::node_handle_uset<node, value_type, allocator_type> node_type;
	typedef 
		container::detail::insert_return_type_base
		<
			iterator,
			node_type
		> insert_return_type;

public:
	typedef typename table::node_allocator node_allocator;
	typedef typename table::node_pointer node_pointer;

	typedef node_pointer node_ptr_type;
	typedef unordered_allocator_destroy_holder<node_allocator, 1> destroy_holder_type;
	typedef unordered_allocator_destroy_holder<node_allocator, 64> multi_destroy_holder_type;

protected:
	typedef typename table::policy policy;
	typedef typename table::extractor extractor;

private:
	typedef boost_uset_ha_opt this_type;

private:
	// constructors
	boost_uset_ha_opt(void);
	boost_uset_ha_opt(BOOST_RV_REF(boost_uset_ha_opt));
	boost_uset_ha_opt(const boost_uset_ha_opt&);
	~boost_uset_ha_opt(void);
	const boost_uset_ha_opt& operator=( const boost_uset_ha_opt& );
	const boost_uset_ha_opt& operator=( BOOST_RV_REF(boost_uset_ha_opt) );

protected:
	inline opt_table_type& opt_table_cast(void)
	{
		return reinterpret_cast<opt_table_type&>(table_);
	}

	inline const opt_table_type& opt_table_cast(void) const
	{
		return reinterpret_cast<const opt_table_type&>(table_);
	}

public:
	// insert
	template<typename Destroy_Holder> inline
	std::pair<iterator, bool> insert(Destroy_Holder& holder, std::size_t key_hash, node_pointer pnode)
	{
		return opt_table_cast().insert(holder, key_hash, pnode);
	}

	template<typename Destroy_Holder> inline
	iterator insert(Destroy_Holder& holder, const_iterator hint, std::size_t key_hash, node_pointer pnode)
	{
		return opt_table_cast().insert(holder, key_hash, pnode).first;
	}

	template<typename Destroy_Holder, typename Key_Hash_It, typename InputIt> inline
    void insert(Destroy_Holder& holder, Key_Hash_It ki, Key_Hash_It kj, InputIt first, InputIt last)
    {
        opt_table_cast().insert_range(holder, ki, kj, first, last);
    }

	//insert node
	inline insert_return_type insert(std::size_t key_hash, BOOST_RV_REF(node_type) np)
	{
		insert_return_type result;
		opt_table_cast().move_insert_node_type(key_hash, static_cast<node_type&>(np), result);
		return YGGR_MOVE_RET(insert_return_type, result);
	}

    inline iterator insert(std::size_t key_hash, const_iterator hint, BOOST_RV_REF(node_type) np)
    {
        return 
			opt_table_cast().move_insert_node_type_with_hint(
				key_hash, hint, static_cast<node_type&>(np));
    }

	// erase
	template<typename Destroy_Holder> inline
    size_type erase(Destroy_Holder& holder, 
						std::size_t key_hash, const key_type& k)
    {
        return opt_table_cast().erase_key(holder, key_hash, k);
    }

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder& holder, const_iterator position)
	{
		return opt_table_cast().erase(holder, position);
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder& holder, const_iterator first, const_iterator last)
	{
		return opt_table_cast().erase_range(holder, first, last);
	}

	// clear
	template<typename Destroy_Holder> inline
	void clear(Destroy_Holder& holder)
	{
		opt_table_cast().clear(holder);
	}

	// find
	inline iterator find(std::size_t key_hash, const key_type& k)
	{
		return iterator(table_.find_node(key_hash, k));
	}

	inline const_iterator find(std::size_t key_hash, const key_type& k) const
	{
		return const_iterator(table_.find_node(key_hash, k));
	}

	template <typename CompatibleKey, typename CompatiblePredicate> inline
	iterator find(std::size_t key_hash,
					CompatibleKey const& k,
					CompatiblePredicate const& eq)
	{
		return iterator(table_.find_node_impl(key_hash, k, eq));
	}

	template <typename CompatibleKey, typename CompatiblePredicate> inline
	const_iterator find( std::size_t key_hash,
							CompatibleKey const& k,
							CompatiblePredicate const& eq) const
	{
		return const_iterator(table_.find_node_impl(key_hash, k, eq));
	}

	// count
    inline size_type count(std::size_t key_hash, const key_type& k) const
    {
        return opt_table_cast().count(key_hash, k);
    }

	// equal_range
    inline std::pair<const_iterator, const_iterator>
        equal_range(std::size_t key_hash, const key_type& k) const
    {
        return opt_table_cast().equal_range(key_hash, k);
    }

	// bucket interface
	inline size_type bucket(std::size_t key_hash) const
	{
		return opt_table_cast().hash_to_bucket(key_hash);
	}

public:
	inline static const key_type& s_get_key(const value_type& val)
	{
		return extractor::extract(val);
	}

	template<typename CH, typename CK> inline 
	static std::size_t s_hash(const CH& h, const CK& k)
	{
		return hash_helper<policy>(h, k);
	}

	inline static std::size_t s_set_node_hash(node_pointer pnode, std::size_t key_hash)
	{
		return (pnode->hash_ = key_hash);
	}

	template<typename CH, typename CK> inline 
	static std::size_t s_set_node_hash(node_pointer pnode, const CH& h, const CK& k)
	{
		return this_type::s_set_node_hash(pnode, hash_helper<policy>(h, k));
	}

	template<typename Sequence, typename CH, typename InputIt> 
	static 
	typename 
		boost::enable_if
		<
			boost::is_same<node_pointer, typename iterator_ex::iterator_value_ex<InputIt>::type>,
			std::pair<typename boost::range_iterator<Sequence>::type, typename boost::range_iterator<Sequence>::type >
		>::type
		s_hash_range(Sequence& seq, const CH& h, InputIt first, InputIt last)
	{
		typedef InputIt iter_type;
		for(iter_type i = first, isize = last; i != isize; ++i)
		{
			assert(*i);
			std::inserter(seq, boost::end(seq)) = (*i)->hash_;
		}

		return std::make_pair(boost::begin(seq), boost::end(seq));
	}

	template<typename Sequence, typename CH, typename InputIt> inline 
	static 
	typename 
		boost::disable_if
		<
			boost::is_same<node_pointer, typename iterator_ex::iterator_value_ex<InputIt>::type>,
			std::pair<typename boost::range_iterator<Sequence>::type, typename boost::range_iterator<Sequence>::type >
		>::type
		s_hash_range(Sequence& seq, const CH& h, InputIt first, InputIt last)
	{
		return hash_range_helper<policy>(seq, h, first, last);
	}

private:
	struct key_getter
	{
		inline const key_type& operator()(node_pointer pnode) const
		{
			assert(pnode);
			return this_type::s_get_key(pnode->value());
		}
	};

public:
	template<typename Sequence, typename CH, typename InputIt> inline 
	static 
	typename 
		boost::enable_if
		<
			boost::is_same<node_pointer, typename iterator_ex::iterator_value_ex<InputIt>::type>,
			std::pair<typename boost::range_iterator<Sequence>::type, typename boost::range_iterator<Sequence>::type >
		>::type
		s_rehash_range(Sequence& seq, const CH& h, InputIt first, InputIt last)
	{
		return hash_range_helper<policy>(seq, h, first, last, key_getter());
	}

public:

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
	template<typename VAlloc, typename CH, typename... Args> inline
	static node_pointer create_node_1(std::size_t& key_hash, const VAlloc& valloc, const CH& h, 
										BOOST_FWD_REF(Args)... args)
	{
		node_pointer pnode = opt_table_type::create_node_1(valloc, boost::forward<Args>(args)...);
		if(!pnode)
		{
			return pnode;
		}
		key_hash = pnode->hash_ = this_type::s_hash(h, this_type::s_get_key(pnode->value()));
		return pnode;
	}

#else

#	if !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x5100))

	// 0 argument emplace requires special treatment in case
	// the container is instantiated with a value type that
	// doesn't have a default constructor.
	template<typename VAlloc, typename CH> inline
	static node_pointer create_node_1(std::size_t& key_hash, const VAlloc& valloc, const CH& h, 
										boost::unordered::detail::empty_emplace
											= boost::unordered::detail::empty_emplace(),
										value_type v = value_type())
	{
		return this_type::create_node_1(key_hash, valloc, h, boost::move(v));
	}

#	endif

	template<typename VAlloc, typename CH, typename A0> inline
	static node_pointer create_node_1(std::size_t& key_hash, const VAlloc& valloc, const CH& h, 
										BOOST_FWD_REF(A0) a0)
	{
		node_pointer pnode
			= opt_table_type::create_node_1( 
				valloc,
				boost::unordered::detail::create_emplace_args(
					boost::forward<A0>(a0)) );
		if(!pnode)
		{
			return pnode;
		}
		key_hash = pnode->hash_ = this_type::s_hash(h, this_type::s_get_key(pnode->value()));
		return pnode;
	}

	template<typename VAlloc, typename CH, typename A0, typename A1> inline
	static node_pointer create_node_1(std::size_t& key_hash,
										const VAlloc& valloc,
										const CH& h,
										BOOST_FWD_REF(A0) a0, 
										BOOST_FWD_REF(A1) a1)
	{
		node_pointer pnode 
			= opt_table_type::create_node_1(
				valloc,
				boost::unordered::detail::create_emplace_args(
					boost::forward<A0>(a0),
					boost::forward<A1>(a1)) );
		if(!pnode)
		{
			return pnode;
		}
		key_hash = pnode->hash_ = this_type::s_hash(h, this_type::s_get_key(pnode->value()));
		return pnode;
	}

	template<typename VAlloc, typename CH, typename A0, typename A1, typename A2> inline
	static node_pointer create_node_1(std::size_t& key_hash,
										const VAlloc& valloc,
										const CH& h,
										BOOST_FWD_REF(A0) a0,
										BOOST_FWD_REF(A1) a1,
										BOOST_FWD_REF(A2) a2)
	{
		node_pointer pnode
			= opt_table_type::create_node_1(
				valloc,
				boost::unordered::detail::create_emplace_args(
					boost::forward<A0>(a0),
					boost::forward<A1>(a1),
					boost::forward<A2>(a2)) );

		if(!pnode)
		{
			return pnode;
		}
		key_hash = pnode->hash_ = this_type::s_hash(h, this_type::s_get_key(pnode->value()));
		return pnode;
	}

#define BOOST_UNORDERED_EMPLACE(z, n, _) \
	template<typename VAlloc, typename CH, \
		BOOST_PP_ENUM_PARAMS_Z(z, n, typename A) > inline \
	static node_pointer create_node_1( std::size_t& key_hash, const VAlloc& valloc, const CH& h, \
			BOOST_PP_ENUM_##z(n, BOOST_UNORDERED_FWD_PARAM, a) ) { \
		node_pointer pnode \
			= opt_table_type::create_node_1( \
				valloc, \
				boost::unordered::detail::create_emplace_args( \
					BOOST_PP_ENUM_##z(n, BOOST_UNORDERED_CALL_FORWARD, \
					a) )); \
		if(!pnode) { return pnode; } \
		key_hash = pnode->hash_ = this_type::s_hash(h, this_type::s_get_key(pnode->value())); \
		return pnode; }

	BOOST_PP_REPEAT_FROM_TO(4, BOOST_UNORDERED_EMPLACE_LIMIT,
		BOOST_UNORDERED_EMPLACE, _)

#undef BOOST_UNORDERED_EMPLACE

#endif // BOOST_NO_CXX11_VARIADIC_TEMPLATES

	template<typename VAlloc, typename CH, typename A0> inline
	static node_pointer create_node_2( std::size_t& key_hash, const VAlloc& valloc, const CH& h, BOOST_FWD_REF(A0) a0 )
	{
		node_pointer pnode = opt_table_type::create_node_2(valloc, boost::forward<A0>(a0));

		if(pnode)
		{
			key_hash = pnode->hash_ = this_type::s_hash(h, this_type::s_get_key(pnode->value()));
		}

		return pnode;
	}

private:
	table table_;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106300_BOOST_USET_HA_OPT_HPP__
