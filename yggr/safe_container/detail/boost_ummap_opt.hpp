//boost_ummap_opt.hpp

/****************************************************************************
Copyright (c) 2010-2022 yggdrasil

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

#ifndef __YGGR_SAFE_CONTAINER_DETIAL_BOOST_UMMAP_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETIAL_BOOST_UMMAP_OPT_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/unordered_map.hpp>

#include <yggr/container/clear.hpp>
#include <yggr/struct_support/get_element.hpp>
#include <yggr/safe_container/detail/hash_helper.hpp>
#include <yggr/safe_container/detail/boost_ugtable_opt.hpp>
#include <yggr/safe_container/detail/cast_to_ucontainer_opt.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template< typename K, typename T,
			typename H = boost::hash<K>,
			typename P = std::equal_to<K>,
			typename A = std::allocator<std::pair<const K, T> > >
class boost_ummap_opt
{
public:

	typedef boost::unordered_multimap<K, T, H, P, A> org_type;

	typedef typename org_type::key_type key_type;
	typedef typename org_type::value_type value_type;
	typedef typename org_type::mapped_type mapped_type;
	typedef typename org_type::hasher hasher;
	typedef typename org_type::key_equal key_equal;
	typedef typename org_type::allocator_type allocator_type;

protected:

	typedef boost::unordered::detail::multimap<A, K, T, H, P> types;
	typedef typename types::traits allocator_traits;
	typedef typename types::table table;

	typedef typename cast_to_utable_opt<table>::type opt_table_type;

public:

	typedef typename allocator_traits::pointer pointer;
	typedef typename allocator_traits::const_pointer const_pointer;

	typedef typename org_type::reference reference;
	typedef typename org_type::const_reference const_reference;

	typedef typename org_type::size_type size_type;
	typedef typename org_type::difference_type difference_type;

	typedef typename table::cl_iterator const_local_iterator;
	typedef typename table::l_iterator local_iterator;
	typedef typename table::c_iterator const_iterator;
	typedef typename table::iterator iterator;

private:
	typedef typename org_type::const_local_iterator chk_const_local_iterator;
	typedef typename org_type::local_iterator chk_local_iterator;
	typedef typename org_type::const_iterator chk_const_iterator;
	typedef typename org_type::iterator chk_iterator;

	BOOST_MPL_ASSERT((boost::is_same<const_local_iterator, chk_const_local_iterator>));
	BOOST_MPL_ASSERT((boost::is_same<local_iterator, chk_local_iterator>));
	BOOST_MPL_ASSERT((boost::is_same<const_iterator, chk_const_iterator>));
	BOOST_MPL_ASSERT((boost::is_same<iterator, chk_iterator>));

protected:
	typedef typename table::policy policy;

private:
	typedef boost_ummap_opt this_type;

private:
	// constructors
	boost_ummap_opt(void) {};
	boost_ummap_opt(BOOST_RV_REF(boost_ummap_opt)){};
	boost_ummap_opt(const boost_ummap_opt&) {};
	~boost_ummap_opt(void) {};

public:
	// insert
	iterator insert(std::size_t key_hash, value_type const& x)
	{
		return this_type::emplace(key_hash, x);
	}

	iterator insert(std::size_t key_hash, BOOST_RV_REF(value_type) x)
	{
		return this_type::emplace(key_hash, boost::forward<value_type>(x));
	}

    template <typename Key_Hash_It, typename InputIt>
    void insert(Key_Hash_It kfirst, Key_Hash_It klast,
					InputIt first, InputIt last)
    {
        opt_table_cast()
			.insert_range(kfirst, klast,
							first, last);
    }

	template <typename Key_Hash_It, typename InputIt>
	void insert(const std::pair<Key_Hash_It, Key_Hash_It>& ki_pair,
					InputIt first, InputIt last)
	{
		opt_table_cast()
			.insert_range(ki_pair.first, ki_pair.second,
							first, last);
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
	template<typename Key_Hash_It>
    void insert(Key_Hash_It kfirst, Key_Hash_It klast,
					std::initializer_list<value_type> list)
    {
        opt_table_cast()
			.insert_range(kfirst, klast,
							list.begin(), list.end());
    }

	template<typename Key_Hash_It>
	void insert(const std::pair<Key_Hash_It, Key_Hash_It>& ki_pair,
					std::initializer_list<value_type> list)
	{
		opt_table_cast().insert_range(ki_pair.first, ki_pair.second,
										list.begin(), list.end());
	}

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	// erase
    size_type erase(std::size_t key_hash, const key_type& k)
    {
        return opt_table_cast().erase_key(key_hash, k);
    }

	// find

	iterator find(std::size_t key_hash, const key_type& k)
    {
		return table_.find_node(key_hash, k);
    }

	const_iterator find(std::size_t key_hash, const key_type& k) const
    {
		return table_.find_node(key_hash, k);
    }

	template <typename CompatibleKey, typename CompatiblePredicate>
	iterator find(std::size_t key_hash,
					CompatibleKey const& k,
					CompatiblePredicate const& eq)
	{
		return table_.find_node_impl(key_hash, k, eq);
	}

	template <typename CompatibleKey, class CompatiblePredicate>
	const_iterator find( std::size_t key_hash,
							CompatibleKey const& k,
							CompatiblePredicate const& eq) const
	{
		return table_.find_node_impl(key_hash, k, eq);
	}

	// count
	size_type count(std::size_t key_hash, const key_type& k) const
    {
        return opt_table_cast().count(key_hash, k);
    }

	// equal_range

    std::pair<iterator, iterator>
		equal_range(std::size_t key_hash, const key_type& k)
    {
        return opt_table_cast().equal_range(key_hash, k);
    }

    std::pair<const_iterator, const_iterator>
		equal_range(std::size_t key_hash, const key_type& k) const
    {
        return opt_table_cast().equal_range(key_hash, k);
    }

	// bucket
	size_type bucket(std::size_t key_hash) const
	{
		return opt_table_cast().hash_to_bucket(key_hash);
	}

protected:
	// emplace

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
	template <typename ...Args>
	iterator emplace(std::size_t key_hash, BOOST_FWD_REF(Args)... args)
	{
		return opt_table_cast().emplace(key_hash, boost::forward<Args>(args)...);
	}

#else

#	if !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x5100))

	// 0 argument emplace requires special treatment in case
	// the container is instantiated with a value type that
	// doesn't have a default constructor.

	iterator emplace(
			std::size_t key_hash,
			boost::unordered::detail::empty_emplace
				= boost::unordered::detail::empty_emplace(),
			value_type v = value_type())
	{
		return this_type::emplace(key_hash, boost::move(v));
	}

#	endif // !BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x5100))

	template <typename A0>
	iterator emplace(std::size_t key_hash, BOOST_FWD_REF(A0) a0)
	{
		return opt_table_cast().emplace(
			key_hash,
			boost::unordered::detail::create_emplace_args(
				boost::forward<A0>(a0))
		);
	}

#endif // BOOST_NO_CXX11_VARIADIC_TEMPLATES

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
	template<typename CH, typename CK>
	inline static
	std::size_t s_hash(const CH& h, const CK& k)
	{
		return hash_helper<policy>(h, k);
	}

	template<typename Sequence, typename CH, typename InputIt>
	inline static
	std::pair<typename boost::range_iterator<Sequence>::type,
				typename boost::range_iterator<Sequence>::type >
		s_hash_range(Sequence& seq, const CH& h, InputIt first, InputIt last)
	{
		return hash_range_helper<policy>(seq, h, first, last);
	}

protected:
	table table_;
};

} // namespace detail
} // namespace container
} // namespace yggr

YGGR_CAST_TO_UMAP_OPT_DEF(boost_ummap_opt, boost::unordered_multimap)

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETIAL_BOOST_UMMAP_OPT_HPP__
