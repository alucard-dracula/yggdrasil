//boost_midx_index_base_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105500_BOOST_MIDX_INDEX_BASE_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105500_BOOST_MIDX_INDEX_BASE_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_INDEX_BASE_OPT_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_INDEX_BASE_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/container/detail/node_handle_midx.hpp>

#include <yggr/safe_container/detail/cast_midx_indexed_by.hpp>
#include <yggr/safe_container/detail/boost_midx_container_opt_decl.hpp>
#include <yggr/safe_container/detail/boost_midx_index_base_opt_decl.hpp>
#include <yggr/safe_container/detail/boost_midx_keyinfo_helper.hpp>

#include <boost/ref.hpp>
#include <boost/multi_index/detail/index_base.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Value, typename IndexSpecifierList, typename Allocator>
class boost_midx_index_base_opt
{
public:
	typedef
		boost::multi_index::detail::index_base
		<
			Value,
			typename indexed_by_org_cast<IndexSpecifierList>::type,
			Allocator
		>															org_type;

protected:
	typedef
		boost::multi_index::detail::index_node_base
		<
			Value,
			Allocator
		>															index_node_type;

	typedef typename
		boost::multi_index::detail::multi_index_node_type
		<
			Value,
			IndexSpecifierList,
			Allocator
		>::type														final_node_type;

	typedef
		boost_midx_container_opt
		<
			Value,
			IndexSpecifierList,
			Allocator
		>															final_type;

	typedef boost::tuples::null_type								ctor_args_list;
	typedef typename
		boost::detail::allocator::rebind_to
		<
			Allocator,
			typename Allocator::value_type
		>::type														final_allocator_type;

	typedef 
		container::detail::node_handle_midx
		<
			final_node_type,
			final_allocator_type
		>															final_node_handle_type;

	typedef boost::mpl::vector0<>									index_type_list;
	typedef boost::mpl::vector0<>									iterator_type_list;
	typedef boost::mpl::vector0<>									const_iterator_type_list;

	typedef
		boost::multi_index::detail::copy_map
		<
			final_node_type,
			final_allocator_type
		>															copy_map_type;

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
	typedef
		boost::multi_index::detail::index_saver
		<
			index_node_type,
			final_allocator_type
		>										                     index_saver_type;

	typedef
		boost::multi_index::detail::index_loader
		<
			index_node_type,
			final_node_type,
			final_allocator_type
		>										                    index_loader_type;
#endif // BOOST_MULTI_INDEX_DISABLE_SERIALIZATION

protected:
	typedef Value												value_type;

private:
	typedef boost_midx_index_base_opt this_type;

private:
	boost_midx_index_base_opt(void);
	boost_midx_index_base_opt(BOOST_RV_REF(boost_midx_index_base_opt));
	boost_midx_index_base_opt(const boost_midx_index_base_opt&);
	~boost_midx_index_base_opt(void);
	const boost_midx_index_base_opt& operator=( const boost_midx_index_base_opt& );
	const boost_midx_index_base_opt& operator=( BOOST_RV_REF(boost_midx_index_base_opt) );


protected:
	template<typename T, typename Meta_Map> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>&,
							const value_type& v, index_node_type* x, 
							boost::multi_index::detail::lvalue_tag)
	{
		boost::detail::allocator::construct(::boost::addressof(x->value()),v);
		return x;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>&,
							const value_type& v, index_node_type* x, 
							boost::multi_index::detail::rvalue_tag)
	{
		boost::detail::allocator::construct(
			::boost::addressof(x->value()),
			value_type(boost::move(const_cast<value_type&>(v))));
		return x;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* 
		insert_(const boost_midx_keyinfo_params<T, Meta_Map>&,
					const value_type&, index_node_type* x, 
					boost::multi_index::detail::emplaced_tag)
	{
		return x;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>&,
								index_node_type* x)
	{
		return x;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>&,
						const value_type& v, index_node_type*, index_node_type* x, 
						boost::multi_index::detail::lvalue_tag)
	{
		boost::detail::allocator::construct(::boost::addressof(x->value()),v);
		return x;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>&,
						const value_type& v, index_node_type*, index_node_type* x, 
						boost::multi_index::detail::rvalue_tag)
	{
		boost::detail::allocator::construct(
			::boost::addressof(x->value()),
			value_type(boost::move(const_cast<value_type&>(v))));
		return x;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* 
		insert_(const boost_midx_keyinfo_params<T, Meta_Map>&,
				const value_type&, index_node_type*, index_node_type* x, 
				boost::multi_index::detail::emplaced_tag)
	{
		return x;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>&,
								index_node_type*, index_node_type* x)
	{
		return x;
	}

	inline void extract_(index_node_type* x)
	{
	}

	inline void erase_(index_node_type* x)
	{
		boost::detail::allocator::destroy(boost::addressof(x->value())); // ~foo
	}

	template<typename Destroy_Holder> inline
	void erase_(Destroy_Holder& holder, index_node_type* x)
	{
	}

	// opt_support
	template<typename T, typename Meta_Map>
	bool replace_(const boost_midx_keyinfo_params<T, Meta_Map>&,
					const value_type& v, index_node_type* x,
					boost::multi_index::detail::lvalue_tag)
	{
		x->value() = v;
		return true;
	}

	template<typename T, typename Meta_Map> inline
	bool replace_(const boost_midx_keyinfo_params<T, Meta_Map>&,
					const value_type& v, index_node_type* x,
					boost::multi_index::detail::rvalue_tag)
	{
		x->value() = boost::move(const_cast<value_type&>(v));
		return true;
	}

	// opt_support
	template<typename Destroy_Holder> inline
	bool modify_(Destroy_Holder&, index_node_type*)
	{
		return true;
	}

	template<typename T, typename Meta_Map> inline
	bool modify_(const boost_midx_keyinfo_params<T, Meta_Map>&, 
							index_node_type*)
	{
		return true;
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	bool modify_(Destroy_Holder&,
							const boost_midx_keyinfo_params<T, Meta_Map>&, 
							index_node_type*)
	{
		return true;
	}

	inline bool modify_rollback_(index_node_type*)
	{
		return true;
	}

	template<typename T, typename Meta_Map> inline
	bool modify_rollback_(const boost_midx_keyinfo_params<T, Meta_Map>&, 
									index_node_type*)
	{
		return true;
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
	/* invariant stuff */

	inline bool invariant_(void) const {return true;}
#endif

	/* access to backbone memfuns of Final class */

	inline final_type& final(void)
	{
		return *static_cast<final_type*>(this);
	}

	inline const final_type& final(void) const
	{
		return *static_cast<const final_type*>(this);
	}

	inline final_node_type* final_header(void) const
	{
		return final().header();
	}

	// hash_opt_support_insert
	template<typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool>
		final_insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						const value_type& x)
	{
		return final().insert_(keyinfo_params, x);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool> 
		final_insert_rv_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							const value_type& x)
	{
		return final().insert_rv_(keyinfo_params, x);
	}

	template<typename T, typename Meta_Map, typename V> inline
	std::pair<final_node_type*, bool> 
		final_insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							V& t)
	{
		return final().insert_ref_(keyinfo_params, t);
	}

	template<typename T, typename Meta_Map, typename V> inline
	std::pair<final_node_type*, bool> 
		final_insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							const V& t)
	{
		return final().insert_ref_(keyinfo_params, t);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool>
		final_insert_(Destroy_Holder& holder, 
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						final_node_type* x)
	{
		return final().insert_(holder, keyinfo_params, x);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool>
		final_insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						const value_type& x, final_node_type* position)
	{
		return final().insert_(keyinfo_params, x, position);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool> 
		final_insert_rv_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							const value_type& x, final_node_type* position)
	{
		return final().insert_rv_(keyinfo_params, x, position);
	}

	template<typename T, typename Meta_Map, typename V> inline
	std::pair<final_node_type*, bool> 
		final_insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
							const V& t, final_node_type* position)
	{
		return final().insert_ref_(keyinfo_params, t, position);
	}

	template<typename T, typename Meta_Map, typename V> inline
	std::pair<final_node_type*, bool> 
		final_insert_ref_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
							V& t, final_node_type* position)
	{
		return final().insert_ref_(keyinfo_params, t, position);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool>
		final_insert_(Destroy_Holder& holder,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						final_node_type* x, final_node_type* position)
	{
		return final().insert_(holder, keyinfo_params, x, position);
	}


	inline void final_erase_(final_node_type* x)
	{
		final().erase_(x);
	}

	template<typename Destroy_Holder> inline
	void final_erase_(Destroy_Holder& holder, final_node_type* x)
	{
		final().erase_(holder, x);
	}

	// hash_opt_support_relace
	template<typename T, typename Meta_Map> inline
	bool final_replace_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							const value_type& k, final_node_type* x)
	{
		return final().replace_(keyinfo_params, k, x);
	}

	template<typename T, typename Meta_Map> inline
	bool final_replace_rv_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							const value_type& k, final_node_type* x)
	{
		return final().replace_rv_(keyinfo_params, k, x);
	}

	// hash_opt_support_modify
	template<typename T, typename Meta_Map, typename Modifier> inline
	bool final_modify_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier& mod, final_node_type* x)
	{
		return final().modify_(keyinfo_params, mod, x);
	}

	template<typename Destroy_Holder, typename Modifier> inline
	bool final_modify_(Destroy_Holder& holder,
							Modifier& mod, final_node_type* x)
	{
		return final().modify_(holder, mod, x);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier> inline
	bool final_modify_(Destroy_Holder& holder, const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier& mod, final_node_type* x)
	{
		return final().modify_(holder, keyinfo_params, mod, x);
	}

	// hash_opt_support_modify_rollback
	template<typename T, typename Meta_Map,
				typename Modifier,typename Rollback> inline
	bool final_modify_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier& mod, Rollback& back, final_node_type* x)
	{
		return final().modify_(keyinfo_params, mod, back, x);
	}

	template<typename Destroy_holder, typename Modifier,typename Rollback> inline
	bool final_modify_(Destroy_holder& holder,
							Modifier& mod, Rollback& back, final_node_type* x)
	{
		return final().modify_(holder, mod, back, x);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map,
				typename Modifier,typename Rollback> inline
	bool final_modify_(Destroy_Holder& holder, 
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier& mod, Rollback& back, final_node_type* x)
	{
		return final().modify_(holder, keyinfo_params, mod, back, x);
	}

	// final_extract_
	inline final_node_handle_type final_extract_(final_node_type* x)
	{
		return final().extract_(x);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool> 
		final_insert_nh_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							final_node_handle_type& nh)
	{
		return final().insert_nh_(keyinfo_params, nh);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<final_node_type*, bool> 
		final_insert_nh_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							final_node_handle_type& nh, 
							final_node_type* position)
	{
		return final().insert_nh_(keyinfo_params, nh, position);
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)

	inline void final_check_invariant_(void) const
	{ 
		final().check_invariant_(); 
	}

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105500_BOOST_MIDX_INDEX_BASE_OPT_HPP__
