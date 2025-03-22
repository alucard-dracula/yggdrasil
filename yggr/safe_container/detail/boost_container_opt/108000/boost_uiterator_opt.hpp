//boost_uiterator_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UITERATOR_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UITERATOR_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UITERATOR_OPT_HPP__
#	error "this file is inner file, can't include it."
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UITERATOR_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <boost/unordered/detail/implementation.hpp>
#include <boost/unordered/detail/fca.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template <typename Node, typename Bucket>
struct u_iterator_opt
{
public:
	typedef boost::unordered::detail::iterator_detail::iterator<Node, Bucket> org_type;

private:
	typedef u_iterator_opt this_type;

public:
    typedef typename Node::node_pointer node_pointer;
	typedef boost::unordered::detail::grouped_bucket_iterator<Bucket> bucket_iterator;

    node_pointer p;
	bucket_iterator itb;

public:
    typedef typename Node::value_type value_type;

private:
	u_iterator_opt(void);
	u_iterator_opt(BOOST_RV_REF(u_iterator_opt));
	u_iterator_opt(const u_iterator_opt&);
	~u_iterator_opt(void);
	const u_iterator_opt& operator=( const u_iterator_opt& );
	const u_iterator_opt& operator=( BOOST_RV_REF(u_iterator_opt) );

public:
	inline static this_type& opt_cast(org_type& org)
	{
		return reinterpret_cast<this_type&>(org);
	}

	inline static const this_type& opt_cast(const org_type& org)
	{
		return reinterpret_cast<const this_type&>(org);
	}

public:
	inline static org_type& make_org(org_type& org, node_pointer p, bucket_iterator itb)
	{
		this_type& opt = this_type::opt_cast(org);
		opt.p = p;
		opt.itb = itb;
		return org;
	}

	inline static org_type make_org(node_pointer p, bucket_iterator itb)
	{
		org_type org;
		this_type::make_org(org, p, itb);
		return org;
	}
};

template<typename Node, typename Bucket>
struct u_c_iterator_opt
{

public:
	typedef boost::unordered::detail::iterator_detail::c_iterator<Node, Bucket> org_type;

private:
	typedef u_c_iterator_opt this_type;

public:
	typedef boost::unordered::detail::iterator_detail::iterator<Node, Bucket> iterator;

	typedef typename Node::node_pointer node_pointer;
	typedef boost::unordered::detail::grouped_bucket_iterator<Bucket> bucket_iterator;

	node_pointer p;
	bucket_iterator itb;

public:

	typedef typename Node::value_type value_type;

private:
	u_c_iterator_opt(void);
	u_c_iterator_opt(BOOST_RV_REF(u_c_iterator_opt));
	u_c_iterator_opt(const u_c_iterator_opt&);
	~u_c_iterator_opt(void);
	const u_c_iterator_opt& operator=( const u_c_iterator_opt& );
	const u_c_iterator_opt& operator=( BOOST_RV_REF(u_c_iterator_opt) );

public:
	inline static this_type& opt_cast(org_type& org)
	{
		return reinterpret_cast<this_type&>(org);
	}

	inline static const this_type& opt_cast(const org_type& org)
	{
		return reinterpret_cast<const this_type&>(org);
	}

public:
	inline static org_type& make_org(org_type& org, node_pointer p, bucket_iterator itb)
	{
		this_type& opt = this_type::opt_cast(org);
		opt.p = p;
		opt.itb = itb;
		return org;
	}

	inline static org_type make_org(node_pointer p, bucket_iterator itb)
	{
		org_type org;
		this_type::make_org(org, p, itb);
		return org;
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

// cast_to_u_iterator_opt
template<typename Node, typename Bucket>
struct cast_to_u_iterator_opt
		<
			boost::unordered::detail::iterator_detail::iterator<Node, Bucket>
		>
{
	typedef u_iterator_opt<Node, Bucket> type;
};

// cast_to_u_c_iterator_opt
template<typename Node, typename Bucket>
struct cast_to_u_c_iterator_opt
		<
			boost::unordered::detail::iterator_detail::c_iterator<Node, Bucket>
		>
{
	typedef u_c_iterator_opt<Node, Bucket> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_108000_BOOST_UITERATOR_OPT_HPP__
