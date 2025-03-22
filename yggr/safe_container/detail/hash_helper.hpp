//hash_helper.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_HASH_HELPER_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_HASH_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/utility/get_element.hpp>
#include <yggr/container/clear.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/functions.hpp>
#include <boost/ref.hpp>

#include <cassert>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Policy>
struct policy_result
{
	typedef typename Policy::result_type type;
};

template<typename T, template<typename _T> class policy >
struct policy_result< policy<T> >
{
	typedef T type;
};

template<typename R>
struct default_policy
{
	template<typename Hasher, typename Val> inline
	static R apply_hash(const Hasher& hasher, const Val& val)
	{
		return hasher(val);
	}
};

template<typename Policy, typename Hasher, typename Key> inline
typename policy_result<Policy>::type
	hash_helper(const Hasher& hasher, const Key& k)
{
	typedef Policy policy_type;

	return policy_type::apply_hash(hasher, k);
}

template<typename Policy, typename Hasher, typename Sequence, typename InputIt>
std::pair<typename boost::range_iterator<Sequence>::type,
			typename boost::range_iterator<Sequence>::type >
	hash_range_helper(Sequence& seq, const Hasher& hasher, InputIt first, InputIt last)
{
	typedef Policy policy_type;
	typedef typename policy_result<policy_type>::type key_hash_type;
	typedef Sequence seq_type;
	typedef InputIt input_iter_type;
	typedef typename boost::range_iterator<seq_type>::type iter_type;
	typedef typename boost::range_value<seq_type>::type chk_key_hash_type;

	BOOST_MPL_ASSERT((boost::is_same<key_hash_type, chk_key_hash_type>));

	if(!boost::empty(seq))
	{
		container::clear(seq);
	}

	for(input_iter_type i = first; i != last; ++i)
	{
		std::inserter(seq, boost::end(seq))
			= policy_type::apply_hash(hasher, get_first_element(*i));
	}

	assert(boost::size(seq) == std::distance(first, last));
	return std::make_pair(boost::begin(seq), boost::end(seq));
}

template<typename Policy, typename Hasher, typename Sequence, typename InputIt, typename KeyGetter>
std::pair<typename boost::range_iterator<Sequence>::type,
			typename boost::range_iterator<Sequence>::type >
	hash_range_helper(Sequence& seq, const Hasher& hasher, InputIt first, InputIt last, const KeyGetter& key_getter)
{
	typedef Policy policy_type;
	typedef typename policy_result<policy_type>::type key_hash_type;
	typedef Sequence seq_type;
	typedef InputIt input_iter_type;
	typedef typename boost::range_iterator<seq_type>::type iter_type;
	typedef typename boost::range_value<seq_type>::type chk_key_hash_type;

	BOOST_MPL_ASSERT((boost::is_same<key_hash_type, chk_key_hash_type>));

	if(!boost::empty(seq))
	{
		container::clear(seq);
	}

	for(input_iter_type i = first; i != last; ++i)
	{
		std::inserter(seq, boost::end(seq))
			= policy_type::apply_hash(hasher, key_getter(*i));
	}

	assert(boost::size(seq) == std::distance(first, last));
	return std::make_pair(boost::begin(seq), boost::end(seq));
}

// !!! in gcc cpp version < cpp11, gcc not support std::pair<reference, reference> !!!
#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

	template<typename Policy, typename Hasher, typename Key> inline
	std::pair<typename policy_result<Policy>::type, const Key* >
		hash_index_helper(const Hasher& hasher, const Key& k)
	{
		typedef Policy policy_type;
		typedef Key key_type;
		typedef typename policy_result<Policy>::type ret_first_type;
		typedef const key_type* ret_second_type;

		typedef std::pair<ret_first_type, ret_second_type > ret_type;

		return ret_type(policy_type::apply_hash(hasher, k), boost::addressof(k));
	}

#else

	template<typename Policy, typename Hasher, typename Key> inline
	std::pair<typename policy_result<Policy>::type, const Key&>
		hash_index_helper(const Hasher& hasher, const Key& k)
	{
		typedef Policy policy_type;
		typedef Key key_type;
		typedef
			std::pair
			<
				typename policy_result<Policy>::type,
				const key_type&
			> ret_type;

		return ret_type(policy_type::apply_hash(hasher, k), k);
	}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)


} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_HASH_HELPER_HPP__
