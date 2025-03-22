//insert_or_assign_impl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_INSERT_OR_ASSIGN_IMPL_HPP__
#define __YGGR_CONTAINER_DETAIL_INSERT_OR_ASSIGN_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/container/detail/has_insert_or_assign.hpp>

#include <boost/utility/enable_if.hpp>

#include <algorithm>


namespace yggr
{
namespace container
{
namespace detail
{

template<typename Map, typename M> inline
typename
	boost::enable_if
	<
		detail::has_any_insert_or_assign< Map >,
		std::pair< typename Map::iterator, bool>
	>::type
	insert_or_assign_impl(Map& c, BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(boost::move(key), boost::forward<M>(m));
}

template<typename Map, typename M> inline
typename
	boost::enable_if
	<
		detail::has_any_insert_or_assign< Map >,
		std::pair< typename Map::iterator, bool>
	>::type
	insert_or_assign_impl(Map& c, const typename Map::key_type& key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(key, boost::forward<M>(m));
}

template<typename Map, typename M> inline
typename
	boost::disable_if
	<
		detail::has_any_insert_or_assign< Map >,
		std::pair< typename Map::iterator, bool>
	>::type
	insert_or_assign_impl(Map& c, BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(M) m)
{
	typedef Map map_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::iterator iter_type;
	typedef std::pair<iter_type, bool> ret_type;

	ret_type ret;

	ret.first = c.find(static_cast<key_type&>(key));
	ret.second = ret.first == c.end();

	return
		ret.second?
			c.insert(value_type(boost::move(key), mapped_type(boost::forward<M>(m))))
			: (ret.first->second = boost::forward<M>(m), ret);
}

template<typename Map, typename M> inline
typename
	boost::disable_if
	<
		detail::has_any_insert_or_assign< Map >,
		std::pair< typename Map::iterator, bool>
	>::type
	insert_or_assign_impl(Map& c, const typename Map::key_type& key, BOOST_FWD_REF(M) m)
{
	typedef Map map_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::iterator iter_type;
	typedef std::pair<iter_type, bool> ret_type;

	ret_type ret;

	ret.first = c.find(key);
	ret.second = ret.first == c.end();

	return
		ret.second?
			c.insert(value_type(key, mapped_type(boost::forward<M>(m))))
			: (ret.first->second = boost::forward<M>(m), ret);
}

template<typename Map, typename M> inline
typename
	boost::enable_if
	<
		detail::has_any_insert_or_assign< Map >,
		typename Map::iterator
	>::type
	insert_or_assign_impl(Map& c, typename Map::const_iterator hint,
							BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(hint, boost::move(key), boost::forward<M>(m));
}

template<typename Map, typename M> inline
typename
	boost::enable_if
	<
		detail::has_any_insert_or_assign< Map >,
		typename Map::iterator
	>::type
	insert_or_assign_impl(Map& c, typename Map::const_iterator hint,
							const typename Map::key_type& key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(hint, key, boost::forward<M>(m));
}

template<typename Map, typename M> inline
typename Map::iterator
	non_has_insert_or_assign_impl(Map& c, typename Map::const_iterator hint,
									BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(M) m)
{
	typedef Map map_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::iterator iter_type;

	iter_type iter = c.find(static_cast<key_type&>(key));

	return
		iter == c.end()?
			c.insert(value_type(boost::move(key), mapped_type(boost::forward<M>(m)))).first
			: (iter->second = boost::forward<M>(m), iter);
}

template<typename Map, typename M> inline
typename Map::iterator
	non_has_insert_or_assign_impl(Map& c, typename Map::const_iterator hint,
									const typename Map::key_type& key, BOOST_FWD_REF(M) m)
{
	typedef Map map_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::iterator iter_type;

	iter_type iter = c.find(key);

	return
		iter == c.end()?
			c.insert(value_type(key, mapped_type(boost::forward<M>(m)))).first
			: (iter->second = boost::forward<M>(m), iter);
}

template<typename Map, typename M> inline
typename
	boost::disable_if
	<
		detail::has_any_insert_or_assign< Map >,
		typename Map::iterator
	>::type
	insert_or_assign_impl(Map& c, typename Map::const_iterator hint,
							BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(M) m)
{
	return non_has_insert_or_assign_impl(c, hint, boost::move(key), boost::forward<M>(m));
}

template<typename Map, typename M> inline
typename
	boost::disable_if
	<
		detail::has_any_insert_or_assign< Map >,
		typename Map::iterator
	>::type
	insert_or_assign_impl(Map& c, typename Map::const_iterator hint,
							const typename Map::key_type& key, BOOST_FWD_REF(M) m)
{
	return non_has_insert_or_assign_impl(c, hint, key, boost::forward<M>(m));
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_INSERT_OR_ASSIGN_IMPL_HPP__
