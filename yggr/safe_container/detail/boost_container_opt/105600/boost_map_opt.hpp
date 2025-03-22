// boost_map_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_MAP_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_MAP_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MAP_OPT_HPP__
#	error "this file is inner file, can't include it, please include <yggr/safe_container/safe_map.hpp>"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MAP_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/safe_container/detail/boost_tree_opt.hpp>

#include <boost/container/map.hpp>

#include <cassert>

namespace yggr
{
namespace safe_container
{
namespace detail
{

//------------------map-----------------

template<typename Key, typename T, typename Compare, typename Allocator, typename MapOptions>
class boost_map_opt
	: public 
		boost::container::container_detail::tree
		< 
			Key, 
			std::pair<const Key, T>, 
			boost::container::container_detail::select1st< std::pair<const Key, T> >, 
			Compare, 
			Allocator, 
			MapOptions
		>
{
private:
	typedef std::pair<const Key, T>  value_type_impl;
	typedef 
		boost::container::container_detail::tree
		<
			Key, 
			value_type_impl, 
			boost::container::container_detail::select1st<value_type_impl>, 
			Compare, 
			Allocator, 
			MapOptions
		> base_t;

	typedef boost::container::container_detail::pair<Key, T> movable_value_type_impl;
	typedef 
		boost::container::container_detail::tree_value_compare
		< 
			Key, 
			value_type_impl, 
			Compare, 
			boost::container::container_detail::select1st<value_type_impl>
		>  value_compare_impl;

private:
	typedef boost::container::container_detail::select1st<value_type_impl> key_of_value_type;

private:
	typedef boost_map_opt this_type;

public:
	typedef base_t org_tree_type;
	typedef typename cast_to_tree_opt<base_t>::type opt_tree_type;
	typedef boost::container::map<Key, T, Compare, Allocator, MapOptions> org_type;

	//////////////////////////////////////////////
	//
	//                    types
	//
	//////////////////////////////////////////////

	typedef typename org_type::key_type						key_type;
	typedef typename org_type::allocator_traits_type		allocator_traits_type;
	typedef typename org_type::mapped_type					mapped_type;
	typedef typename org_type::value_type					value_type;
	typedef typename org_type::pointer						pointer;
	typedef typename org_type::const_pointer				const_pointer;
	typedef typename org_type::reference					reference;
	typedef typename org_type::const_reference				const_reference;
	typedef typename org_type::size_type					size_type;
	typedef typename org_type::difference_type				difference_type;
	typedef typename org_type::allocator_type				allocator_type;
	typedef typename org_type::stored_allocator_type		stored_allocator_type;
	typedef typename org_type::value_compare				value_compare;
	typedef typename org_type::key_compare					key_compare;
	typedef typename org_type::iterator						iterator;
	typedef typename org_type::const_iterator				const_iterator;
	typedef typename org_type::reverse_iterator				reverse_iterator;
	typedef typename org_type::const_reverse_iterator		const_reverse_iterator;
	//typedef typename org_type::nonconst_value_type			nonconst_value_type; // boost 176 discarded
	typedef typename org_type::movable_value_type			movable_value_type;

	typedef typename opt_tree_type::node_ptr_type node_ptr_type;
	typedef typename opt_tree_type::destroy_holder_type destroy_holder_type;
	typedef typename opt_tree_type::multi_destroy_holder_type multi_destroy_holder_type;

private:
	boost_map_opt(void);
	boost_map_opt(BOOST_RV_REF(boost_map_opt));
	boost_map_opt(const boost_map_opt&);
	~boost_map_opt(void);
	const boost_map_opt& operator=( const boost_map_opt& );
	const boost_map_opt& operator=( BOOST_RV_REF(boost_map_opt) );

private:
	inline org_type& org_cast(void)
	{
		return reinterpret_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return reinterpret_cast<const org_type&>(*this);
	}

	inline opt_tree_type& opt_tree_cast(void)
	{
		return reinterpret_cast<opt_tree_type&>(static_cast<org_tree_type&>(*this));
	}

	inline const opt_tree_type& opt_tree_cast(void) const
	{
		return reinterpret_cast<const opt_tree_type&>(static_cast<const org_tree_type&>(*this));
	}

public:

#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

	template<typename VAlloc, typename ...Args> inline
	static node_ptr_type create_node(const VAlloc& valloc, BOOST_FWD_REF(Args)... args)
	{
		return opt_tree_type::create_node(valloc, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename VAlloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		static node_ptr_type create_node(const VAlloc& valloc \
											YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
											YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ){ \
			return \
				opt_tree_type::create_node( \
					valloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME( ) arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // BOOST_CONTAINER_PERFECT_FORWARDING

private:
	inline static const key_type& key_of_value(const value_type& val)
	{
		return key_of_value_type()(val);
	}

public:
	// insert
	template<typename Destroy_Holder> inline
	std::pair<iterator, bool> 
		insert(Destroy_Holder holder, node_ptr_type pnode)
	{  
		assert(pnode);
		return opt_tree_cast().insert_unique(
				holder, this_type::key_of_value(pnode->get_data()), pnode);  
	}

	template<typename Destroy_Holder> inline
	iterator 
		insert(Destroy_Holder holder, 
				const_iterator p, node_ptr_type pnode)
	{
		assert(pnode);
		return opt_tree_cast().insert_unique(
				holder, p, this_type::key_of_value(pnode->get_data()), pnode); 
	}

	template<typename Destroy_Holder, typename InputIterator> inline
	void insert(Destroy_Holder holder, 
					InputIterator first, InputIterator last)
	{ 
		key_of_value_type key_of_value;
		opt_tree_cast().insert_unique(holder, key_of_value, first, last);  
	}

	// erase
	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder, const_iterator p)
	{  
		return opt_tree_cast().erase(holder, p); 
	}

	template<typename Destroy_Holder> inline
	size_type erase(Destroy_Holder holder, const key_type& x)
	{  
		return opt_tree_cast().erase(holder, x); 
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder,
					const_iterator first, const_iterator last)
	{
		return opt_tree_cast().erase(holder, first, last);
	}

	template<typename Destroy_Holder> inline
	void clear(Destroy_Holder holder)
	{
		opt_tree_cast().clear(holder); 
	}
};


// -------------multimap-----------------

template <typename Key, typename T, typename Compare, typename Allocator, typename MultiMapOptions>
class boost_multimap_opt
	: public 
		boost::container::container_detail::tree
		<
			Key, 
			std::pair<const Key, T>, 
			boost::container::container_detail::select1st< std::pair<const Key, T> >, 
			Compare, 
			Allocator, 
			MultiMapOptions
		>
{
private:
	typedef std::pair<const Key, T>  value_type_impl;
	typedef 
		boost::container::container_detail::tree
		<
			Key, 
			value_type_impl, 
			boost::container::container_detail::select1st<value_type_impl>, 
			Compare, Allocator, 
			MultiMapOptions
		> base_t;
	typedef boost::container::container_detail::pair<Key, T> movable_value_type_impl;
	typedef 
		boost::container::container_detail::tree_value_compare
		< 
			Key, 
			value_type_impl, 
			Compare, 
			boost::container::container_detail::select1st<value_type_impl>
		>  value_compare_impl;

public:
	typedef base_t org_tree_type;
	typedef typename cast_to_tree_opt<base_t>::type opt_tree_type;
	typedef boost::container::multimap<Key, T, Compare, Allocator, MultiMapOptions> org_type;

	//////////////////////////////////////////////
	//
	//                    types
	//
	//////////////////////////////////////////////

	typedef typename org_type::key_type								key_type;
	typedef typename org_type::mapped_type							mapped_type;
	typedef typename org_type::value_type							value_type;
	typedef typename org_type::pointer								pointer;
	typedef typename org_type::const_pointer						const_pointer;
	typedef typename org_type::reference							reference;
	typedef typename org_type::const_reference						const_reference;
	typedef typename org_type::size_type							size_type;
	typedef typename org_type::difference_type						difference_type;
	typedef typename org_type::allocator_type						allocator_type;
	typedef typename org_type::stored_allocator_type				stored_allocator_type;
	typedef typename org_type::value_compare						value_compare;
	typedef typename org_type::key_compare							key_compare;
	typedef typename org_type::iterator								iterator;
	typedef typename org_type::const_iterator						const_iterator;
	typedef typename org_type::reverse_iterator						reverse_iterator;
	typedef typename org_type::const_reverse_iterator				const_reverse_iterator;
	//typedef typename org_type::nonconst_value_type					nonconst_value_type; // boost 176 discarded
	typedef typename org_type::movable_value_type					movable_value_type;

	typedef typename opt_tree_type::node_ptr_type node_ptr_type;
	typedef typename opt_tree_type::destroy_holder_type destroy_holder_type;
	typedef typename opt_tree_type::multi_destroy_holder_type multi_destroy_holder_type;

	//////////////////////////////////////////////
	//
	//          construct/copy/destroy
	//
	//////////////////////////////////////////////

private:
	boost_multimap_opt(void);
	boost_multimap_opt(BOOST_RV_REF(boost_multimap_opt));
	boost_multimap_opt(const boost_multimap_opt&);
	~boost_multimap_opt(void);
	const boost_multimap_opt& operator=( const boost_multimap_opt& );
	const boost_multimap_opt& operator=( BOOST_RV_REF(boost_multimap_opt) );

private:
	inline org_type& org_cast(void)
	{
		return reinterpret_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return reinterpret_cast<const org_type&>(*this);
	}

	inline opt_tree_type& opt_tree_cast(void)
	{
		return reinterpret_cast<opt_tree_type&>(static_cast<org_tree_type&>(*this));
	}

	inline const opt_tree_type& opt_tree_cast(void) const
	{
		return reinterpret_cast<const opt_tree_type&>(static_cast<const org_tree_type&>(*this));
	}

public:

#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

	template<typename VAlloc, typename ...Args> inline
	static node_ptr_type create_node(const VAlloc& valloc, BOOST_FWD_REF(Args)... args)
	{
		return opt_tree_type::create_node(valloc, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename VAlloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		static node_ptr_type create_node(const VAlloc& valloc \
											YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
											YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ){ \
			return \
				opt_tree_type::create_node( \
					valloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME( ) arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // BOOST_CONTAINER_PERFECT_FORWARDING


public:
	inline iterator insert(node_ptr_type pnode)
	{  
		return opt_tree_cast().insert_equal(pnode); 
	}

	inline iterator insert(const_iterator p, node_ptr_type pnode)
	{  
		return opt_tree_cast().insert_equal(p, pnode);
	}	

	template< typename InputIterator > inline
	void insert(InputIterator first, InputIterator last)
	{ 
		opt_tree_cast().insert_equal(first, last);  
	}

	template< typename Destroy_Holder > inline
	iterator erase(Destroy_Holder holder, const_iterator p)
	{
		return opt_tree_cast().erase(holder, p);
	}

	template< typename Destroy_Holder > inline
	size_type erase(Destroy_Holder holder, const key_type& x)
	{
		return opt_tree_cast().erase(holder, x);
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder holder, const_iterator first, const_iterator last)
	{
		return opt_tree_cast().erase(holder, first, last); 
	}

	template<typename Destroy_Holder> inline
	void clear(Destroy_Holder holder)
	{
		opt_tree_cast().clear(holder); 
	}
};


} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif //__YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105600_BOOST_MAP_OPT_HPP__

