// binary_buffer_assign_helper.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BINARY_BUFFER_ASSIGN_HELPER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BINARY_BUFFER_ASSIGN_HELPER_HPP__

#include <yggr/bytes/byte_size.hpp>

#include <yggr/container/resize.hpp>
#include <yggr/container/size.hpp>
#include <yggr/container/empty.hpp>
#include <yggr/container_ex/is_container.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>

#include <memory>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

// binary_buffer_assign_iterator_helper
template<typename Buf, typename InputIter,
			bool is_random_access = is_random_access_iterator<InputIter>::value>
struct binary_buffer_assign_iterator_helper;

template<typename Buf, typename InputIter>
struct binary_buffer_assign_iterator_helper<Buf, InputIter, true>
{
public:
	typedef Buf buf_type;
	typedef InputIter iter_type;

public:
	typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_value_type;

private:
	BOOST_MPL_ASSERT((is_random_access_container<buf_type>));

public:
	inline void operator()(buf_type& buf, iter_type s, iter_type e) const
	{
		u64 buf_size = bytes::byte_size(s, e);
		if(!buf_size)
		{
			container::clear(buf);
			return;
		}

		container::resize(buf, buf_size);
		memcpy(boost::addressof(buf[0]), boost::addressof(*s), buf_size);
	}
};

template<typename Buf, typename InputIter>
struct binary_buffer_assign_iterator_helper<Buf, InputIter, false>
{
public:
	typedef Buf buf_type;
	typedef InputIter iter_type;

public:
	typedef typename iterator_ex::iterator_value_ex<iter_type>::type iter_value_type;

private:
	BOOST_MPL_ASSERT((is_random_access_container<buf_type>));

public:
	template<typename InIter> inline
	void operator()(buf_type& buf, InIter s, InIter e) const
	{
		u64 buf_size = bytes::byte_size(s, e);
		if(!buf_size)
		{
			container::clear(buf);
			return;
		}

		container::resize(buf, buf_size);
		std::copy(s, e, reinterpret_cast<iter_value_type*>(boost::addressof(*boost::begin(buf))));
	}
};

// binary_buffer_assign_container_helper
template<typename Buf, typename Container,
			bool is_random_access = is_random_access_container<Container>::value>
struct binary_buffer_assign_container_helper;

template<typename Buf, typename Container>
struct binary_buffer_assign_container_helper<Buf, Container, true>
{
public:
	typedef Buf buf_type;
	typedef Container cont_type;

protected:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_value_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(buf_value_type) == 1>));

public:
	inline void operator()(buf_type& buf, const cont_type& cont) const
	{
		u64 buf_size = bytes::byte_size(cont);
		if(!buf_size)
		{
			container::clear(buf);
			return;
		}

		container::resize(buf, buf_size);
		memcpy(boost::addressof(buf[0]), boost::addressof(*boost::begin(cont)), buf_size);
	}
};

template<typename Buf, typename Container>
struct binary_buffer_assign_container_helper<Buf, Container, false>
{
public:
	typedef Buf buf_type;
	typedef Container cont_type;

protected:
	typedef typename range_ex::range_value_ex<buf_type>::type buf_value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_value_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(buf_value_type) == 1>));

public:
	inline void operator()(buf_type& buf, const cont_type& cont) const
	{
		if(container::empty(cont))
		{
			container::clear(buf);
			return;
		}

		u64 buf_size = bytes::byte_size(cont);
		container::resize(buf, buf_size);

		std::copy(
			boost::begin(cont),
			boost::end(cont),
			reinterpret_cast<cont_value_type*>(boost::addressof(*boost::begin(buf))));
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BINARY_BUFFER_ASSIGN_HELPER_HPP__
