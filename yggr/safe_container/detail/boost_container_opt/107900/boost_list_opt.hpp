// boost_list_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107900_BOOST_LIST_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107900_BOOST_LIST_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_LIST_OPT_HPP__
#	error "this file is inner file, can't include it, please include <yggr/safe_container/safe_list.hpp>"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_LIST_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/safe_container/detail/allocator_destroy_holder.hpp>
#include <yggr/safe_container/detail/boost_container_create_node.hpp>

#include <boost/container/list.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template <typename T, typename Allocator>
class boost_list_opt
	: protected
		boost::container::dtl::node_alloc_holder
		<
			typename boost::container::real_allocator<T, Allocator>::type,
			typename 
				boost::container::dtl::intrusive_list_type
				<
					typename boost::container::real_allocator<T, Allocator>::type
				>::type
		>
{
public:
	typedef boost::container::list<T, Allocator> org_type;


private:
	typedef typename
		boost::container::dtl::intrusive_list_type
		<
			typename boost::container::real_allocator<T, Allocator>::type
		>::type																				Icont;
	typedef 
		boost::container::dtl::node_alloc_holder
		<
			typename boost::container::real_allocator<T, Allocator>::type, 
			Icont
		>																					AllocHolder;
	typedef typename AllocHolder::NodePtr													NodePtr;
	typedef typename AllocHolder::NodeAlloc													NodeAlloc;
	typedef typename AllocHolder::ValAlloc													ValAlloc;
	typedef typename AllocHolder::Node														Node;
	typedef boost::container::dtl::allocator_node_destroyer<NodeAlloc>						Destroyer;
	typedef typename AllocHolder::alloc_version												alloc_version;
	typedef boost::container::allocator_traits<Allocator>									allocator_traits_type;
	typedef boost::container::equal_to_value<typename allocator_traits_type::value_type>	equal_to_value_type;
	typedef 
		boost::container::dtl::iterator_from_iiterator
		<
			typename Icont::iterator, false
		>																					iterator_impl;
	typedef 
		boost::container::dtl::iterator_from_iiterator
		<
			typename Icont::iterator, true
		>																					const_iterator_impl;

	typedef typename AllocHolder::AllocVersionTraits alloc_version_traits_type;
	typedef typename AllocHolder::Deallocator deallocator_type;

	typedef boost_list_opt this_type;

public:
	// opt_support
	typedef NodePtr node_ptr_type;
	typedef allocator_destroy_holder<NodeAlloc, 1> destroy_holder_type;
	typedef allocator_destroy_holder<NodeAlloc, 64> multi_destroy_holder_type;

public:
	typedef typename org_type::value_type value_type;
	typedef typename org_type::pointer pointer;
	typedef typename org_type::const_pointer const_pointer;
	typedef typename org_type::reference reference;
	typedef typename org_type::const_reference const_reference;
	typedef typename org_type::size_type size_type;
	typedef typename org_type::difference_type difference_type;
	typedef typename org_type::allocator_type allocator_type;
	typedef typename org_type::stored_allocator_type stored_allocator_type;
	typedef typename org_type::iterator iterator;
	typedef typename org_type::const_iterator const_iterator;
	typedef typename org_type::reverse_iterator reverse_iterator;
	typedef typename org_type::const_reverse_iterator const_reverse_iterator;

private:
	boost_list_opt(void);
	boost_list_opt(BOOST_RV_REF(boost_list_opt));
	boost_list_opt(const boost_list_opt&);
	~boost_list_opt(void);
	const boost_list_opt& operator=( const boost_list_opt& );
	const boost_list_opt& operator=( BOOST_RV_REF(boost_list_opt) );

private:
	inline org_type& org_cast(void)
	{
		return reinterpret_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return reinterpret_cast<const org_type&>(*this);
	}

public:

	// support_opt
	inline iterator insert(const_iterator p, node_ptr_type pnode)
	{
		return pnode? iterator(this->icont().insert(p.get(), *pnode)) : org_cast().end();
	}

	inline void push_back(node_ptr_type pnode)
	{
		this->insert(org_cast().cend(), pnode);
	}

	inline void push_front(node_ptr_type pnode)
	{
		this->insert(org_cast().cbegin(), pnode);
	}

	template<typename Destroy_Holder> inline
	void pop_front(Destroy_Holder holder)
	{
		this->erase(holder, org_cast().cbegin());
	}

	template<typename Destroy_Holder> inline
	void pop_back(Destroy_Holder holder)
	{
		const_iterator tmp = org_cast().cend();
		this->erase(holder, --tmp);
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder, const_iterator p)
	{
		return iterator(this->icont().erase_and_dispose(p.get(), holder));
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder,
					const_iterator first, const_iterator last)
	{
		return iterator(this->erase_range(holder, first.get(), last.get(), alloc_version()));
	}

private:
	template<typename Destroy_Holder, typename ICont_Iterator> inline
	ICont_Iterator erase_range(Destroy_Holder holder,
								const ICont_Iterator &first, const ICont_Iterator &last, alloc_version)
	{
		return this->icont().erase_and_dispose(first, last, holder);
	}

public:
	template<typename Destroy_Holder> inline
	void remove(Destroy_Holder holder, const value_type& value)
	{
		this->remove_if(holder, equal_to_value_type(value));
	}

	template <typename Destroy_Holder, typename Pred> inline
	void remove_if(Destroy_Holder holder, Pred pred)
	{
		typedef boost::container::value_to_node_compare<Node, Pred> value_to_node_compare_type;
		this->icont().remove_and_dispose_if(value_to_node_compare_type(pred), holder);
	}

	template<typename Destroy_Holder> inline
	void unique(Destroy_Holder holder)
	{
		typedef boost::container::value_equal<value_type> value_equal_t;
		this->unique(holder, value_equal_t());  
	}

	template<typename Destroy_Holder, typename Pred> inline
	void unique(Destroy_Holder holder, Pred pred)
	{
		typedef boost::container::value_to_node_compare<Node, Pred> Predicate;
		this->icont().unique_and_dispose(Predicate(pred), holder);
	}

// opt_support
public:

#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

	template<typename VAlloc, typename ...Args> inline
	static node_ptr_type create_node(const VAlloc& valloc, BOOST_FWD_REF(Args)... args)
	{
		return 
			__create_node<node_ptr_type, alloc_version_traits_type, deallocator_type>(
				valloc, boost::forward<Args>(args)...);
	}

#else //#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename VAlloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		static node_ptr_type create_node(const VAlloc& valloc \
											YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
											YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )){ \
			return \
				__create_node<node_ptr_type, alloc_version_traits_type, deallocator_type>( \
					valloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME( ) arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif   //#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

};

} // namespace detail
} // naemspace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107900_BOOST_LIST_OPT_HPP__
