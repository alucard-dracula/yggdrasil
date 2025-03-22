// boost_tree_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105800_BOOST_TREE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105800_BOOST_TREE_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_TREE_OPT_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_TREE_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/safe_container/detail/allocator_destroy_holder.hpp>
#include <yggr/safe_container/detail/boost_container_create_node.hpp>

#include <boost/container/detail/tree.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Key, typename Value, typename KeyOfValue,
			typename KeyCompare, typename A, typename Options>
class boost_tree_opt;


} // namespace detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename K, typename V, typename KFV,
			typename C, typename A, typename O>
struct cast_to_tree_opt<boost::container::container_detail::tree<K, V, KFV, C, A, O> >
{
	typedef boost_tree_opt<K, V, KFV, C, A, O> type;
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

template <typename Key, typename Value, typename KeyOfValue,
			typename KeyCompare, typename A,
			typename Options = container::tree_assoc_defaults>
class boost_tree_opt
	: protected 
		boost::container::container_detail::node_alloc_holder
		< 
			A, 
			typename 
				boost::container::container_detail::intrusive_tree_type
				< 
					A,
					boost::container::container_detail::tree_value_compare<Key, Value, KeyCompare, KeyOfValue>, 
					Options::tree_type, 
					Options::optimize_size
				>::type
		>
{

	typedef 
		boost::container::container_detail::tree
		<
			Key, Value, KeyOfValue, KeyCompare, A, Options
		>															org_type;

	typedef 
		boost::container::container_detail::tree_value_compare
		<
			Key, Value, KeyCompare, KeyOfValue
		>															ValComp;
	typedef typename 
		boost::container::container_detail::intrusive_tree_type
		< 
			A, ValComp, 
			Options::tree_type, Options::optimize_size
		>::type														Icont;
	typedef 
		boost::container::container_detail
		::node_alloc_holder
		<
			A, 
			Icont
		>															AllocHolder;
	typedef typename AllocHolder::NodePtr							NodePtr;

	typedef boost_tree_opt											ThisType;
	typedef typename AllocHolder::NodeAlloc							NodeAlloc;
	typedef typename AllocHolder::ValAlloc							ValAlloc;
	typedef typename AllocHolder::Node								Node;
	typedef typename Icont::iterator								iiterator;
	typedef typename Icont::const_iterator							iconst_iterator;
	typedef 
		boost::container::container_detail::allocator_destroyer
		<
			NodeAlloc
		>															Destroyer;
	typedef typename AllocHolder::alloc_version						alloc_version;

	typedef 
		boost::container::container_detail::intrusive_tree_proxy
		<
			Options::tree_type
		>															intrusive_tree_proxy_t;


	// opt_support
	typedef typename AllocHolder::AllocVersionTraits alloc_version_traits_type;
	typedef typename AllocHolder::Deallocator deallocator_type;

public:

	typedef typename org_type::key_type								key_type;
	typedef typename org_type::value_type							value_type;
	typedef typename org_type::allocator_type						allocator_type;
	typedef typename org_type::key_compare							key_compare;
	typedef typename org_type::value_compare						value_compare;
	typedef typename org_type::pointer								pointer;
	typedef typename org_type::const_pointer						const_pointer;
	typedef typename org_type::reference							reference;
	typedef typename org_type::const_reference						const_reference;
	typedef typename org_type::size_type							size_type;
	typedef typename org_type::difference_type						difference_type;
	typedef typename org_type::tree_difference_type					tree_difference_type;
	typedef typename org_type::tree_pointer							tree_pointer;
	typedef typename org_type::tree_const_pointer					tree_const_pointer;
	typedef typename org_type::tree_reference						tree_reference;
	typedef typename org_type::tree_const_reference					tree_const_reference;
	typedef typename org_type::stored_allocator_type				stored_allocator_type;

private:

	typedef 
		boost::container::container_detail
			::key_node_compare<value_compare, Node>					KeyNodeCompare;

public:
	typedef typename org_type::iterator								iterator;
	typedef typename org_type::const_iterator						const_iterator;
	typedef typename org_type::reverse_iterator						reverse_iterator;
	typedef typename org_type::const_reverse_iterator				const_reverse_iterator;

public:
	// opt_support
	typedef NodePtr node_ptr_type;
	typedef allocator_destroy_holder<NodeAlloc, 1> destroy_holder_type;
	typedef allocator_destroy_holder<NodeAlloc, 64> multi_destroy_holder_type;

private:
	boost_tree_opt(void);
	boost_tree_opt(BOOST_RV_REF(boost_tree_opt));
	boost_tree_opt(const boost_tree_opt&);
	~boost_tree_opt(void);
	const boost_tree_opt& operator=( const boost_tree_opt& );
	const boost_tree_opt& operator=( BOOST_RV_REF(boost_tree_opt) );

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

#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

	template<typename VAlloc, typename ...Args> inline
	static node_ptr_type create_node(const VAlloc& valloc, BOOST_FWD_REF(Args)... args)
	{
		return 
			__create_node<node_ptr_type, alloc_version_traits_type, deallocator_type>(
				valloc, boost::forward<Args>(args)...);
	}

#else

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

#endif // BOOST_CONTAINER_PERFECT_FORWARDING

public:
	typedef typename Icont::insert_commit_data insert_commit_data;

public:
	// insert/erase
	// 1.55 start
	inline iterator insert_unique_commit(node_ptr_type pnode, insert_commit_data &data)
	{
		typedef boost::container::container_detail::scoped_destroy_deallocator<NodeAlloc> destroy_deallocator_type;
		assert(pnode);
		destroy_deallocator_type destroy_deallocator(pnode, this->node_alloc());
		iterator ret(this->icont().insert_unique_commit(*pnode, data));
		destroy_deallocator.release();
		return ret;
	}

	template<typename Destroy_Holder> inline
	std::pair<iterator, bool>
		insert_unique(Destroy_Holder holder, const key_type& key, node_ptr_type pnode)
	{
		insert_commit_data data;
		std::pair<iterator, bool> ret =
			org_cast().insert_unique_check(key, data);
		if(!ret.second)
		{
			holder(pnode);
			return ret;
		}
		return std::pair<iterator, bool>(
				this->insert_unique_commit(pnode, data), true);
	}

public:
	template<typename Destroy_Holder> inline
	iterator insert_unique(Destroy_Holder holder, const_iterator hint,
							const key_type& key, node_ptr_type pnode)
	{
		insert_commit_data data;
		std::pair<iterator,bool> ret =
			org_cast().insert_unique_check(hint, key, data);
		if(!ret.second)
		{
			holder(pnode);
			return ret.first;
		}
		return this->insert_unique_commit(pnode, data);
	}

	// boost::move_detail::identity not support operator() const, so using "KFV& key_of_value"
	template <typename Destroy_Holder, typename KFV, typename InputIterator>
	void insert_unique(Destroy_Holder holder, /*const*/ KFV& key_of_value,
						InputIterator first, InputIterator last)
	{
		for( ; first != last; ++first)
		{
			assert(*first);
			this->insert_unique(holder, (key_of_value)((*first)->get_data()), *first);
		}
	}

	// multi support 1.55 start
	inline iterator insert_equal(node_ptr_type pnode)
	{
		typedef boost::container::container_detail::scoped_destroy_deallocator<NodeAlloc> destroy_deallocator_type;
		assert(pnode);
		destroy_deallocator_type destroy_deallocator(pnode, this->node_alloc());
		iterator ret(this->icont().insert_equal(this->icont().end(), *pnode));
		destroy_deallocator.release();
		return ret;
	}

	inline iterator insert_equal(const_iterator hint, node_ptr_type pnode)
	{
		typedef boost::container::container_detail::scoped_destroy_deallocator<NodeAlloc> destroy_deallocator_type;
		assert(pnode);
		destroy_deallocator_type destroy_deallocator(pnode, this->node_alloc());
		iterator ret(this->icont().insert_equal(hint.get(), *pnode));
		destroy_deallocator.release();
		return ret;
	}

	template <class InputIterator>
	void insert_equal(InputIterator first, InputIterator last)
	{
		for( ; first != last; ++first)
		{
			this->insert_equal(*first);
		}
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder, const_iterator position)
	{
		return iterator(this->icont().erase_and_dispose(position.get(), holder));
	}

	template<typename Destroy_Holder> inline
	size_type erase(Destroy_Holder holder, const key_type& k)
	{
		return this->erase_key(holder, k, KeyNodeCompare(org_cast().value_comp()), alloc_version());
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder, const_iterator first, const_iterator last)
	{
		return iterator( this->erase_range(
							holder,
							first.get(),
							last.get(),
							alloc_version()));
	}

	template<typename Destroy_Holder> inline
	void clear(Destroy_Holder holder)
	{
		this->clear_of_holder(holder, alloc_version());
	}

private:
	template<typename Destroy_Holder, typename Comparator> inline
	size_type erase_key(Destroy_Holder holder, const key_type& k, const Comparator &comp, alloc_version)
	{
		return this->icont().erase_and_dispose(k, comp, holder);
	}

	template<typename Destroy_Holder, typename Icont_Iter> inline
	Icont_Iter erase_range(Destroy_Holder holder,
								const Icont_Iter &first, const Icont_Iter &last, alloc_version)
	{
		return this->icont().erase_and_dispose(first, last, holder);
	}

	template<typename Destroy_Holder> inline
	void clear_of_holder(Destroy_Holder holder, alloc_version)
	{
		this->icont().clear_and_dispose(holder);
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr


#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105800_BOOST_TREE_OPT_HPP__
