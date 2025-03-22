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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106700_BOOST_UITERATOR_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106700_BOOST_UITERATOR_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UITERATOR_OPT_HPP__
#	error "this file is inner file, can't include it."
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UITERATOR_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <boost/unordered/detail/implementation.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template <typename Node>
struct u_iterator_opt
{
public:
	typedef boost::unordered::iterator_detail::iterator<Node> org_type;

private:
	typedef u_iterator_opt this_type;

public:
    typedef typename Node::node_pointer node_pointer;
    node_pointer node_;

public:
    typedef typename Node::value_type value_type;

private:
	u_iterator_opt(void);
	u_iterator_opt(BOOST_RV_REF(u_iterator_opt));
	u_iterator_opt(const u_iterator_opt&);
	~u_iterator_opt(void);
	const u_iterator_opt& operator=( const u_iterator_opt& );
	const u_iterator_opt& operator=( BOOST_RV_REF(u_iterator_opt) );
};

template<typename Node>
struct u_c_iterator_opt
{

public:
	typedef boost::unordered::iterator_detail::c_iterator<Node> org_type;

private:
	typedef u_c_iterator_opt this_type;

public:
	typedef typename Node::node_pointer node_pointer;
	typedef boost::unordered::iterator_detail::iterator<Node> iterator;
	node_pointer node_;

public:

	typedef typename Node::value_type value_type;

private:
	u_c_iterator_opt(void);
	u_c_iterator_opt(BOOST_RV_REF(u_c_iterator_opt));
	u_c_iterator_opt(const u_c_iterator_opt&);
	~u_c_iterator_opt(void);
	const u_c_iterator_opt& operator=( const u_c_iterator_opt& );
	const u_c_iterator_opt& operator=( BOOST_RV_REF(u_c_iterator_opt) );
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
template<typename Node>
struct cast_to_u_iterator_opt
		<
			boost::unordered::iterator_detail::iterator<Node>
		>
{
	typedef u_iterator_opt<Node> type;
};

// cast_to_u_c_iterator_opt
template<typename Node>
struct cast_to_u_c_iterator_opt
		<
			boost::unordered::iterator_detail::c_iterator<Node>
		>
{
	typedef u_c_iterator_opt<Node> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106700_BOOST_UITERATOR_OPT_HPP__
