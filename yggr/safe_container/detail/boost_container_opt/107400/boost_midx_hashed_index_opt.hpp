// boost_midx_hashed_index_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_HASHED_INDEX_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_HASHED_INDEX_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_HASHED_INDEX_OPT_HPP__
#	error "this file is inner file, can't include it, please include <yggr/safe_container/safe_multi_index_hashed_index.hpp>"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_HASHED_INDEX_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <cassert>
#include <iterator>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/functions.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/limits.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/container/detail/boost_midx_check_helper.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>

#include <yggr/safe_container/detail/cast_midx_base_type.hpp>
#include <yggr/safe_container/detail/hash_helper.hpp>

#include <yggr/safe_container/detail/boost_midx_index_base_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_hashed_index_opt_decl.hpp>


#if !defined(YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT)
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)

#	define YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT \
		::boost::multi_index::detail::scope_guard BOOST_JOIN(check_invariant_, __LINE__) = \
			::boost::multi_index::detail::make_obj_guard( \
				*this, &boost_midx_hashed_index_opt::check_invariant_); \
		BOOST_JOIN(check_invariant_, __LINE__).touch();

#else

#	define YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#endif //YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT


namespace yggr
{
namespace safe_container
{
namespace detail
{

template
<
	typename KeyFromValue, typename Hash, typename Pred,
	typename SuperMeta, typename TagList, typename Category
>
class boost_midx_hashed_index_opt :
	BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS SuperMeta::type

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	,public
		boost::multi_index::safe_mode::safe_container
		<
			boost_midx_hashed_index_opt
			<
				KeyFromValue, Hash, Pred,
				SuperMeta, TagList, Category
			>
		>
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE


{
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
/* The "ISO C++ Template Parser" option in CW8.3 has a problem with the
	* lifetime of const references bound to temporaries --precisely what
	* scopeguards are.
	*/

#pragma parse_mfunc_templ off
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

public:
	typedef
		boost::multi_index::detail::hashed_index
		<
			KeyFromValue, Hash, Pred,
			typename nth_layer_org_cast<SuperMeta>::type,
			TagList, Category
		>														org_type;

public:
	typedef typename SuperMeta::type							super;

public:
	typedef
		boost::multi_index::detail::hashed_index_node
		<
			typename super::index_node_type
		>														index_node_type;

protected:
	typedef typename index_node_type::
		 template node_alg<Category>::type						node_alg;
	typedef typename index_node_type::impl_type					node_impl_type;
	typedef typename node_impl_type::pointer					node_impl_pointer;
	typedef typename node_impl_type::base_pointer				node_impl_base_pointer;
	typedef
		boost::multi_index::detail::bucket_array
		<
			typename super::final_allocator_type
		>														bucket_array_type;

public:
	/* types */

	typedef typename org_type::key_type							key_type;
	typedef typename org_type::value_type						value_type;
	typedef typename org_type::key_from_value					key_from_value;
	typedef typename org_type::hasher							hasher;
	typedef typename org_type::key_equal						key_equal;
	typedef typename org_type::ctor_args						ctor_args;
	typedef typename org_type::allocator_type					allocator_type;
	typedef typename org_type::pointer							pointer;
	typedef typename org_type::const_pointer					const_pointer;
	typedef typename org_type::reference						reference;
	typedef typename org_type::const_reference					const_reference;
	typedef typename org_type::size_type						size_type;
	typedef typename org_type::difference_type					difference_type;

	typedef typename org_type::iterator							iterator;
	typedef typename org_type::const_iterator					const_iterator;
	typedef typename org_type::local_iterator					local_iterator;
	typedef typename org_type::const_local_iterator				const_local_iterator;
	typedef typename org_type::tag_list							tag_list;

	typedef Category											category_type;
	typedef index_node_type*									node_ptr_type;

	typedef typename org_type::node_type						node_type;
	typedef typename org_type::insert_return_type				insert_return_type;
		
//protected:
//	typedef typename supper::inner_final_node_handle_type		inner_node_type;

protected:
	typedef typename super::final_node_type						final_node_type;
	typedef
		boost::tuples::cons
		<
			ctor_args,
			typename super::ctor_args_list
		>												        ctor_args_list;

		typedef typename
			boost::mpl::push_front
			<
				typename super::index_type_list,
				boost_midx_hashed_index_opt
			>::type												index_type_list;

	typedef typename
		boost::mpl::push_front
		<
			typename super::iterator_type_list,
			iterator
		>::type													iterator_type_list;

	typedef typename
		boost::mpl::push_front
		<
			typename super::const_iterator_type_list,
			const_iterator
		>::type								                    const_iterator_type_list;

	typedef typename super::copy_map_type						copy_map_type;

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)

	typedef typename super::index_saver_type					index_saver_type;
	typedef typename super::index_loader_type					index_loader_type;

#endif // BOOST_MULTI_INDEX_DISABLE_SERIALIZATION

protected:
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	typedef
		boost::multi_index::safe_mode::safe_container
		<
			boost_midx_hashed_index_opt
		>							                             safe_super;
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	typedef typename
		boost::call_traits<value_type>::param_type				value_param_type;
	typedef typename
		boost::call_traits<key_type>::param_type                key_param_type;

private:
	typedef boost_midx_hashed_index_opt this_type;

private:
	boost_midx_hashed_index_opt(void);
	boost_midx_hashed_index_opt(BOOST_RV_REF(boost_midx_hashed_index_opt));
	boost_midx_hashed_index_opt(const boost_midx_hashed_index_opt&);
	~boost_midx_hashed_index_opt(void);
	const boost_midx_hashed_index_opt& operator=( const boost_midx_hashed_index_opt& );
	const boost_midx_hashed_index_opt& operator=( BOOST_RV_REF(boost_midx_hashed_index_opt) );

protected:
	inline org_type& org_cast(void)
	{
		return reinterpret_cast<org_type&>(*this);
	}

	inline const org_type& org_cast(void) const
	{
		return reinterpret_cast<const org_type&>(*this);
	}

public:
	// hash_opt_support final().get_allocator() needed
	inline allocator_type get_allocator(void) const
	{
		return this->final().get_allocator();
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator, bool>
		insert(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
				BOOST_RV_REF(value_type) x)
	{
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_rv_(keyinfo_params, x);
		return std::pair<iterator, bool>(make_iterator(p.first), p.second);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator, bool>
		insert(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
				const value_type& x)
	{
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_(keyinfo_params, x);
		return std::pair<iterator, bool>(make_iterator(p.first), p.second);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<iterator, bool>
		insert(Destroy_Holder& holder, 
				const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
				final_node_type* x)
	{
		assert(x);
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_(holder, keyinfo_params, x);
		return std::pair<iterator, bool>(make_iterator(p.first), p.second);
	}

	// hash_opt_support_insert_range
	template<typename T, typename Meta_Map> inline
	iterator insert(iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						BOOST_RV_REF(value_type) x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p
			= this->final_insert_rv_(keyinfo_params, x,
									static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	template<typename T, typename Meta_Map> inline
	iterator insert(iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						const value_type& x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p
			= this->final_insert_(keyinfo_params, x,
									static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	iterator insert(Destroy_Holder& holder,
						iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						final_node_type* x)
	{
		assert(x);
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p
			= this->final_insert_(holder, 
									keyinfo_params, x,
									static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	template<typename KeyParams_Iter, typename InputIterator>
	void insert(const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					InputIterator first, InputIterator last)
	{
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		assert(( std::distance(iter_pair.first, iter_pair.second)
					== std::distance(first, last) ));

		for(KeyParams_Iter ki = iter_pair.first, ksize = iter_pair.second;
				first != last && ki != ksize; ++first, ++ki)
		{
			this->final_insert_ref_(*ki, *first);
		}
	}

	template<typename Destroy_Holder, typename KeyParams_Iter, typename InputIterator>
	void insert(Destroy_Holder& holder,
					const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					InputIterator first, InputIterator last)
	{
		typedef typename boost::iterator_value<InputIterator>::type chk_type;
		BOOST_MPL_ASSERT((boost::is_same<chk_type, final_node_type*>));

		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		assert(( std::distance(iter_pair.first, iter_pair.second)
					== std::distance(first, last) ));

		for(KeyParams_Iter ki = iter_pair.first, ksize = iter_pair.second;
				first != last && ki != ksize; ++first, ++ki)
		{
			assert(*first);
			this->final_insert_(holder, *ki, *first);
		}
	}

	template<typename Destroy_Holder, typename KeyParams_Iter, typename InputIterator> inline
	void insert(Destroy_Holder& holder,
					const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					const std::pair<InputIterator, InputIterator>& iter_pair_node)
	{
		return this_type::insert(holder, iter_pair, iter_pair_node.first, iter_pair_node.second);
	}

	// node insert
	template<typename T, typename Meta_Map> inline
	insert_return_type insert(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
								BOOST_RV_REF(node_type) nh)
	{
		if(nh)
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(org_cast(), nh);
		}
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

		std::pair<final_node_type*, bool> p =
			this->final_insert_nh_(
				keyinfo_params,
				container::detail::cast_to_yggr_node_handle_midx(nh));
		return insert_return_type(make_iterator(p.first), p.second, boost::move(nh));
	}

	template<typename T, typename Meta_Map> inline
	iterator insert(const_iterator position, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					BOOST_RV_REF(node_type) nh)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
		}

		if(nh) 
		{
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(*this, nh);
		}
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

		std::pair<final_node_type*, bool> p = 
			this->final_insert_nh_(
				keyinfo_params,
				container::detail::cast_to_yggr_node_handle_midx(nh), 
				static_cast<final_node_type*>(position.get_node()));
		return make_iterator(p.first);
	}

	// node extract
	inline node_type extract(const_iterator position)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		return 
			this->final_extract_(
				static_cast<final_node_type*>(position.get_node()));
	}

	inline node_type extract(std::size_t key_hash, key_param_type k)
	{
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

		std::size_t buc = buckets.position(key_hash);
		for(node_impl_pointer x = buckets.at(buc)->prior();
				x != node_impl_pointer(0);
				x = node_alg::next_to_inspect(x))
		{
			if(eq_(k, key(index_node_type::from_impl(x)->value())))
			{
				return this->final_extract_(static_cast<final_node_type*>(index_node_type::from_impl(x)));
			}
		}
		return node_type();
	}

	size_type erase(std::size_t key_hash, key_param_type k)
	{
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

		std::size_t buc = buckets.position(key_hash);

		for(node_impl_pointer x = buckets.at(buc)->prior();
				x != node_impl_pointer(0); 
				x = node_alg::next_to_inspect(x))
		{
			if(eq_(k, key(index_node_type::from_impl(x)->value())))
			{
				node_impl_pointer y = end_of_range(x);
				size_type s = 0;
				do
				{
					node_impl_pointer z = node_alg::after(x);
					this->final_erase_(static_cast<final_node_type*>(index_node_type::from_impl(x)));
					x = z;
					++s;
				} while(x != y);

				return s;
			}
		}

		return 0;
	}

	template<typename Destroy_Holder>
	size_type erase(Destroy_Holder& holder, std::size_t key_hash, key_param_type k)
	{
		YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

		std::size_t buc = buckets.position(key_hash);

		for(node_impl_pointer x = buckets.at(buc)->prior();
				x != node_impl_pointer(0); 
				x = node_alg::next_to_inspect(x))
		{
			if(eq_(k, key(index_node_type::from_impl(x)->value())))
			{
				node_impl_pointer y = end_of_range(x);
				size_type s = 0;
				do
				{
					node_impl_pointer z = node_alg::after(x);
					this->final_erase_(holder, static_cast<final_node_type*>(index_node_type::from_impl(x)));
					x = z;
					++s;
				} while(x != y);

				return s;
			}
		}

		return 0;
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder& holder, iterator position)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}
		this->final_erase_(holder, static_cast<final_node_type*>(position++.get_node()));
		return position;
	}

	template<typename Destroy_Holder>
	iterator erase(Destroy_Holder& holder, iterator first, iterator last)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(first, org_cast());
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(last, org_cast());
			BOOST_MULTI_INDEX_CHECK_VALID_RANGE(first, last);
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		while(first != last)
		{
			first = this_type::erase(holder, first);
		}
		return first;
	}
	
	// hash_opt_support_relace
	template<typename T, typename Meta_Map> inline
	bool replace(iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					BOOST_RV_REF(value_type) x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}
		return this->final_replace_rv_(keyinfo_params, x, 
										static_cast<final_node_type*>(position.get_node()));
	}

	template<typename T, typename Meta_Map> inline
	bool replace(iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					const value_type& x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}
		return this->final_replace_(keyinfo_params, x, 
										static_cast<final_node_type*>(position.get_node()));
	}

	template<typename T, typename Meta_Map, typename Modifier> inline
	bool modify(iterator position, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#endif

		return this->final_modify_( keyinfo_params,
			mod, static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename Modifier> inline
	bool modify(Destroy_Holder& holder, iterator position, Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#endif

		return this->final_modify_(holder,
			mod, static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier> inline
	bool modify(Destroy_Holder& holder, iterator position, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#endif

		return this->final_modify_(holder, keyinfo_params,
			mod, static_cast<final_node_type*>(position.get_node()));
	}

	template<typename T, typename Meta_Map,
				typename Modifier, typename Rollback> inline
	bool modify(iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#endif

		return this->final_modify_(keyinfo_params, mod, back_, 
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename Modifier,typename Rollback> inline
	bool modify(Destroy_Holder& holder, iterator position, Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#endif

		return this->final_modify_(holder,
			mod, back_, static_cast<final_node_type*>(position.get_node()));
	}


	template<typename Destroy_Holder, typename T, typename Meta_Map,
				typename Modifier, typename Rollback> inline
	bool modify(Destroy_Holder& holder, iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#endif

		return this->final_modify_(holder, keyinfo_params, mod, back_, 
									static_cast<final_node_type*>(position.get_node()));
	}

	// hash_opt_support_modify_key
	template<typename T, typename Meta_Map, typename Modifier> inline
	bool modify_key(iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		return modify( position, 
						keyinfo_params, 
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier,value_type,KeyFromValue
						>(mod,key) );
	}

	template<typename Destroy_Holder, typename Modifier> inline
	bool modify_key(Destroy_Holder& holder, iterator position, Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		return modify( holder, position,
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier,value_type,KeyFromValue
						>(mod, key) );
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier> inline
	bool modify_key(Destroy_Holder& holder,
						iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		return modify( holder, position, 
						keyinfo_params, 
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier,value_type,KeyFromValue
						>(mod,key) );
	}

	template<typename T, typename Meta_Map, typename Modifier,typename Rollback> inline
	bool modify_key(iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}
		
		return modify(
						position,
						keyinfo_params,
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key),
						boost::multi_index::detail::modify_key_adaptor
						<
							Rollback, value_type, KeyFromValue
						>(back_, key) );
	}

	template<typename Destroy_Holder, typename Modifier, typename Rollback> inline
	bool modify_key(Destroy_Holder& holder, iterator position, Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}

		return modify(
						holder,
						position,
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key),
						boost::multi_index::detail::modify_key_adaptor
						<
							Rollback, value_type, KeyFromValue
						>(back_, key) );
	}

	template<typename Destroy_Holder, 
				typename T, typename Meta_Map, typename Modifier,typename Rollback> inline
	bool modify_key(Destroy_Holder& holder, iterator position,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						Modifier mod, Rollback back_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,org_cast());
			YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
		}
		
		return modify(
						holder,
						position,
						keyinfo_params,
						boost::multi_index::detail::modify_key_adaptor
						<
							Modifier, value_type, KeyFromValue
						>(mod, key),
						boost::multi_index::detail::modify_key_adaptor
						<
							Rollback, value_type, KeyFromValue
						>(back_, key) );
	}

	template<typename CompatibleKey> inline
	iterator find(std::size_t key_hash, const CompatibleKey& k) const
	{
		return this_type::find(key_hash, k, eq_);
	}

	template< typename CompatibleKey,
				typename CompatiblePred > inline
	iterator find( std::size_t key_hash,
					const CompatibleKey& k,
					const CompatiblePred& eq) const
	{
		return this_type::find(key_hash, k, eq, 
								boost::multi_index::detail::promotes_1st_arg
								<
									CompatiblePred,
									CompatibleKey,
									key_type
								>());
	}

	template<typename CompatibleKey> inline
	size_type count(std::size_t key_hash, const CompatibleKey& k) const
	{
		return this_type::count(key_hash, k, eq_);
	}

	template<typename CompatibleKey, typename CompatiblePred> inline
	size_type count(std::size_t key_hash,
					const CompatibleKey& k,
					const CompatiblePred& eq) const
	{
		return this_type::count(key_hash, k, eq, 
									boost::multi_index::detail::promotes_1st_arg
									<
										CompatiblePred,
										CompatibleKey,
										key_type
									>());
	}

	// hash_opt_support_equal_range
	template<typename CompatibleKey> inline
	std::pair<iterator, iterator> 
		equal_range(std::size_t key_hash, const CompatibleKey& k) const
	{
		return equal_range(key_hash, k, eq_);
	}

	template<typename CompatibleKey, typename CompatiblePred> inline
	std::pair<iterator, iterator> 
		equal_range( std::size_t key_hash,
						const CompatibleKey& k,
						const CompatiblePred& eq) const
	{
		return this_type::equal_range(key_hash, k, eq,
										boost::multi_index::detail::promotes_1st_arg
										<
											CompatiblePred,
											CompatibleKey,
											key_type
										>());
	}

	inline size_type bucket(std::size_t key_hash) const
	{
		return buckets.position(key_hash);
	}

BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:

// hash_opt_support
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)

	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node, &org_cast());
	}

	inline const_iterator make_iterator(index_node_type* node) const
	{
		return const_iterator(node, const_cast<org_type*>(&org_cast()));
	}

#else
	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node);
	}

	inline const_iterator make_iterator(index_node_type* node)const
	{
		return const_iterator(node);
	}

#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	inline void extract_(index_node_type* x)
	{
		unlink(x);
		super::extract_(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename T, typename Meta_Map, typename Variant> inline
	final_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
								value_param_type v, final_node_type*& x, Variant variant)
	{
		reserve_for_insert(org_cast().size() + 1);

		std::size_t buc = find_bucket(keyinfo_params);
		link_info pos(buckets.at(buc));

		if(!link_point(v, pos))
		{
			return 
				static_cast<final_node_type*>(
					index_node_type::from_impl(node_impl_type::pointer_from(pos)));
		}

		final_node_type* res = super::insert_(keyinfo_params, v, x, variant);
		if(res == x)
		{
			link(static_cast<index_node_type*>(x), pos);
		}

		return res;
	}

	template<typename T, typename Meta_Map> inline
	final_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
								final_node_type* x)
	{
		reserve_for_insert(org_cast().size() + 1);

		std::size_t buc = find_bucket(keyinfo_params);
		link_info pos(buckets.at(buc));

		if(!link_point(x->value(), pos))
		{
			return 
				static_cast<final_node_type*>(
					index_node_type::from_impl(node_impl_type::pointer_from(pos)));
		}

		final_node_type* res = super::insert_(keyinfo_params, x);
		if(res == x)
		{
			link(static_cast<index_node_type*>(x), pos);
		}

		return res;
	}

	// hash_opt_support_insert_range

	template<typename T, typename Meta_Map, typename Variant> inline
	final_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
								value_param_type v, index_node_type* position, final_node_type*& x, Variant variant)
	{
		reserve_for_insert(org_cast().size() + 1);

		std::size_t buc = find_bucket(keyinfo_params);
		link_info pos(buckets.at(buc));
		if(!link_point(v, pos))
		{
			return 
				static_cast<final_node_type*>(
					index_node_type::from_impl(node_impl_type::pointer_from(pos)));
		}

		final_node_type* res = super::insert_(keyinfo_params, v, position, x, variant);
		if(res == x)
		{
			link(static_cast<index_node_type*>(x), pos);
		}

		return res;
	}

	template<typename T, typename Meta_Map> inline
	final_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							index_node_type* position, final_node_type* x)
	{
		reserve_for_insert(org_cast().size() + 1);

		std::size_t buc = find_bucket(keyinfo_params);
		link_info pos(buckets.at(buc));
		if(!link_point(x->value(), pos))
		{
			return 
				static_cast<final_node_type*>(
					index_node_type::from_impl(node_impl_type::pointer_from(pos)));
		}

		final_node_type* res = super::insert_(keyinfo_params, position, x);
		if(res == x)
		{
			link(static_cast<index_node_type*>(x), pos);
		}

		return res;
	}

	inline void erase_(index_node_type* x)
	{
		unlink(x);
		super::erase_(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename Destroy_Holder> inline
	void erase_(Destroy_Holder& holder, index_node_type* x)
	{
		unlink(x);
		super::erase_(holder, x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename T, typename Meta_Map, typename Variant> inline
	bool replace_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					value_param_type v, index_node_type* x, Variant variant)
	{
		if(eq_(key(v), key(x->value())))
		{
			return super::replace_(keyinfo_params, v, x, variant);
		}
	      
		unlink_undo undo;
		unlink(x, undo);

		BOOST_TRY
		{
			std::size_t buc = find_bucket(keyinfo_params);
			link_info pos(buckets.at(buc));
			if(link_point(v, pos)
				&& super::replace_(keyinfo_params, v, x, variant))
			{
				link(x,pos);
				return true;
			}

			undo();
			return false;
		}
		BOOST_CATCH(...)
		{
			undo();
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename T, typename Meta_Map> inline
	bool modify_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					index_node_type* x)
	{
		std::size_t buc = 0;
		bool b = false; 
		BOOST_TRY
		{
			buc = find_bucket(keyinfo_params);
			b = in_place(x->impl(), key(x->value()), buc);
		}
		BOOST_CATCH(...)
		{
			erase_(x);
			BOOST_RETHROW;
		}
		BOOST_CATCH_END

		if(!b)
		{
			unlink(x);
			BOOST_TRY
			{
				link_info pos(buckets.at(buc));
				if(!link_point(x->value(), pos))
				{
					super::erase_(x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
					detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
					return false;
				}
				link(x, pos);
			}
			BOOST_CATCH(...)
			{
				super::erase_(x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	
				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}
	
		BOOST_TRY
		{
			if(!super::modify_(keyinfo_params, x))
			{
				unlink(x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
				return false;
			}
			else 
			{
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			unlink(x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	
			BOOST_RETHROW;
		}

		BOOST_CATCH_END
	}

	template<typename Destroy_Holder> inline
	bool modify_(Destroy_Holder& holder, index_node_type* x)
	{
		std::size_t buc = 0;
		bool b = false; 
		BOOST_TRY
		{
			buc = find_bucket(x->value());
			b = in_place(x->impl(), key(x->value()), buc);
		}
		BOOST_CATCH(...)
		{
			erase_(holder, x);
			BOOST_RETHROW;
		}
		BOOST_CATCH_END

		if(!b)
		{
			unlink(x);
			BOOST_TRY
			{
				link_info pos(buckets.at(buc));
				if(!link_point(x->value(), pos))
				{
					super::erase_(holder, x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
					detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
					return false;
				}
				link(x, pos);
			}
			BOOST_CATCH(...)
			{
				super::erase_(holder, x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	
				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}
	
		BOOST_TRY
		{
			if(!super::modify_(holder, x))
			{
				unlink(x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
				return false;
			}
			else 
			{
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			unlink(x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	
			BOOST_RETHROW;
		}

		BOOST_CATCH_END
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	bool modify_(Destroy_Holder& holder, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					index_node_type* x)
	{
		std::size_t buc = 0;
		bool b = false; 
		BOOST_TRY
		{
			buc = find_bucket(keyinfo_params);
			b = in_place(x->impl(), key(x->value()), buc);
		}
		BOOST_CATCH(...)
		{
			erase_(holder, x);
			BOOST_RETHROW;
		}
		BOOST_CATCH_END

		if(!b)
		{
			unlink(x);
			BOOST_TRY
			{
				link_info pos(buckets.at(buc));
				if(!link_point(x->value(), pos))
				{
					super::erase_(holder, x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
					detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
					return false;
				}
				link(x, pos);
			}
			BOOST_CATCH(...)
			{
				super::erase_(holder, x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	
				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}
	
		BOOST_TRY
		{
			if(!super::modify_(holder, keyinfo_params, x))
			{
				unlink(x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
				return false;
			}
			else 
			{
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			unlink(x);
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	
			BOOST_RETHROW;
		}

		BOOST_CATCH_END
	}

	inline bool modify_rollback_(index_node_type* x)
	{
		std::size_t buc = find_bucket(x->value());
		if(in_place(x->impl(), key(x->value()), buc))
		{
			return super::modify_rollback_(x);
		}

		unlink_undo undo;
		unlink(x, undo);

		BOOST_TRY
		{
			link_info pos(buckets.at(buc));
			if(link_point(x->value(), pos)
				&& super::modify_rollback_(x))
			{
				link(x, pos);
				return true;
			}
			undo();
			return false;
		}
		BOOST_CATCH(...)
		{
			undo();
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename T, typename Meta_Map> inline
	bool modify_rollback_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							index_node_type* x)
	{
		std::size_t buc = find_bucket(keyinfo_params);
		if(in_place(x->impl(), key(x->value()), buc))
		{
			return super::modify_rollback_(x);
		}

		unlink_undo undo;
		unlink(x, undo);

		BOOST_TRY
		{
			link_info pos(buckets.at(buc));
			if(link_point(x->value(), pos)
				&& super::modify_rollback_(keyinfo_params, x))
			{
				link(x, pos);
				return true;
			}
			undo();
			return false;
		}
		BOOST_CATCH(...)
		{
			undo();
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
	/* invariant stuff */

	bool invariant_(void) const
	{
		const org_type& org = org_cast();

		if(org.size() == 0 || org.begin() == org.end())
		{
			if(org.size() != 0 || org.begin() != org.end())
			{
				return false;
			}
		}
		else
		{
			size_type s0 = 0;
			for(const_iterator it = org.begin(), it_end = org.end(); it != it_end; ++it, ++s0){}

			if(s0 != org.size()) 
			{
				return false;
			}

			size_type s1 = 0;
			for(size_type buc = 0; buc < org.bucket_count(); ++buc)
			{
				size_type ss1 = 0;
				for(const_local_iterator it = org.begin(buc), it_end = org.end(buc);
						it != it_end; ++it, ++ss1)
				{
					if(org.bucket(key(*it)) != buc)
					{
						return false;
					}
				}
				if(ss1 != org.bucket_size(buc)) 
				{ 
					return false; 
				}
				s1 += ss1;
			}

			if(s1 != org.size())
			{
				return false;
			}
		}

		return super::invariant_();
	}

	/* This forwarding function eases things for the boost::mem_fn construct
	* in YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT. Actually,
	* final_check_invariant is already an inherited member function of index.
	*/
	inline void check_invariant_(void) const
	{
		this->final_check_invariant_();
	}

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

private:
	inline index_node_type* header(void) const { return this->final_header(); }

	inline std::size_t find_bucket(value_param_type v) const
	{
		return bucket(hash_(key(v)));
	}

	// hash_opt_support_insert
	template<typename T, typename Meta_Map> inline
	std::size_t find_bucket(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params) const
	{
		typedef typename boost_midx_get_tag<tag_list>::type tag_type;
		return bucket(keyinfo_params. template get<tag_type>());
	}

	struct link_info_non_unique
	{
		link_info_non_unique(node_impl_base_pointer pos)
			: first(pos), last(node_impl_base_pointer(0))
		{
		}

		inline operator const node_impl_base_pointer&(void) const
		{
			return this->first;
		}

		node_impl_base_pointer first, last;
	};

	typedef typename 
		boost::mpl::if_
		<
			boost::is_same<Category, boost::multi_index::detail::hashed_unique_tag>,
			node_impl_base_pointer,
			link_info_non_unique
		>::type link_info;

	inline bool link_point(value_param_type v, link_info& pos)
	{
		return link_point(v, pos, Category());
	}

	bool link_point(value_param_type v, node_impl_base_pointer& pos,
						boost::multi_index::detail::hashed_unique_tag )
	{
		for(node_impl_pointer x = pos->prior();
				x != node_impl_pointer(0);
				x = node_alg::after_local(x))
		{
			if(eq_(key(v), key(index_node_type::from_impl(x)->value())))
			{
				pos = node_impl_type::base_pointer_from(x);
				return false;
			}
		}
		return true;
	}

	bool link_point(value_param_type v, link_info_non_unique& pos,
						boost::multi_index::detail::hashed_non_unique_tag)
	{
		for(node_impl_pointer x = pos.first->prior();
				x != node_impl_pointer(0);
				x = node_alg::next_to_inspect(x))
		{
			if(eq_(key(v), key(index_node_type::from_impl(x)->value())))
			{
				pos.first = node_impl_type::base_pointer_from(x);
				pos.last = node_impl_type::base_pointer_from(last_of_range(x));
				return true;
			}
		}
		return true;
	}

	inline node_impl_pointer last_of_range(node_impl_pointer x) const
	{
		return last_of_range(x,Category());
	}

	inline node_impl_pointer last_of_range(node_impl_pointer x, 
											boost::multi_index::detail::hashed_unique_tag) const
	{
		return x;
	}

	inline node_impl_pointer last_of_range(node_impl_pointer x, 
											boost::multi_index::detail::hashed_non_unique_tag) const
	{
		node_impl_base_pointer y = x->next();
		node_impl_pointer z = y->prior();
		if(z == x)								/* range of size 1 or 2 */
		{                     
			node_impl_pointer yy = node_impl_type::pointer_from(y);
			return eq_(key(index_node_type::from_impl(x)->value()),
						key(index_node_type::from_impl(yy)->value()))? yy : x;
		}
		else 
		{
			if(z->prior() == x)					/* last of bucket */
			{
				return x;
			}
			else                                /* group of size>2 */
			{
				return z;
			}
		}
	}

	inline node_impl_pointer end_of_range(node_impl_pointer x) const
	{
		return end_of_range(x, Category());
	}

	inline node_impl_pointer end_of_range(node_impl_pointer x, 
											boost::multi_index::detail::hashed_unique_tag) const
	{
		return node_alg::after(last_of_range(x));
	}

	inline node_impl_pointer end_of_range(node_impl_pointer x, 
											boost::multi_index::detail::hashed_non_unique_tag) const
	{
		node_impl_base_pointer y = x->next();
		node_impl_pointer z = y->prior();
		if(z == x)								/* range of size 1 or 2 */
		{                      
			node_impl_pointer yy = node_impl_type::pointer_from(y);
			if(!eq_(key(index_node_type::from_impl(x)->value()),
						key(index_node_type::from_impl(yy)->value())))
			{ 
				yy = x;
			}
			return yy->next()->prior() == yy?
					node_impl_type::pointer_from(yy->next())
					: yy->next()->prior();
		}
		else 
		{
			if(z->prior() == x)					/* last of bucket */
			{
				return z;
			}
			else								/* group of size > 2 */
			{
				return z->next()->prior() == z?
						node_impl_type::pointer_from(z->next())
						: z->next()->prior();
			}
		}
	}

	inline void link(index_node_type* x, const link_info& pos)
	{
		link(x,pos,Category());
	}

	inline void link(index_node_type* x, node_impl_base_pointer pos, 
				boost::multi_index::detail::hashed_unique_tag)
	{
		node_alg::link(x->impl(),pos,header()->impl());
	}

	inline void link(index_node_type* x, const link_info_non_unique& pos, 
				boost::multi_index::detail::hashed_non_unique_tag)
	{
		if(pos.last == node_impl_base_pointer(0))
		{
			node_alg::link(x->impl(), pos.first, header()->impl());
		}
		else
		{
			node_alg::link( x->impl(),
							node_impl_type::pointer_from(pos.first),
							node_impl_type::pointer_from(pos.last));
		}
	}

	inline void unlink(index_node_type* x)
	{
		node_alg::unlink(x->impl());
	}

	typedef typename node_alg::unlink_undo unlink_undo;

	inline void unlink(index_node_type* x, unlink_undo& undo)
	{
		node_alg::unlink(x->impl(),undo);
	}

	inline void calculate_max_load(void)
	{
		float fml = static_cast<float>(mlf * static_cast<float>(org_cast().bucket_count()));
		max_load = ::yggr::mplex::numeric_limits<size_type>::max_type::value;
		if(max_load > fml)
		{
			max_load = static_cast<size_type>(fml);
		}
	}

	// hash opt support

	inline void reserve_for_insert(size_type n)
	{
		if(n > max_load)
		{
			size_type bc = ::yggr::mplex::numeric_limits<size_type>::max_type::value;
			float fbc = static_cast<float>(1 + static_cast<double>(n) / mlf);
			if(bc > fbc)
			{
				bc = static_cast<size_type>(fbc);
			}
			unchecked_rehash(bc);
		}
	}

	// hash opt support
	inline void unchecked_rehash(size_type n)
	{
		unchecked_rehash(n, Category());
	}

	void unchecked_rehash(size_type n, 
							boost::multi_index::detail::hashed_unique_tag)
	{
		node_impl_type cpy_end_node;
		node_impl_pointer cpy_end = node_impl_pointer(&cpy_end_node);
		node_impl_pointer end_ = header()->impl();

		bucket_array_type buckets_cpy(get_allocator(), cpy_end, n);

		org_type& org = org_cast();

		if(org.size() != 0)
		{
			boost::multi_index::detail::auto_space
			<
				std::size_t, allocator_type
			> hashes(get_allocator(), org.size());

			boost::multi_index::detail::auto_space
			<
				node_impl_pointer, allocator_type
			> node_ptrs(get_allocator(), org.size());

			std::size_t i = 0, size_ = org.size();
			bool within_bucket = false;
			BOOST_TRY
			{
				for(;i != size_; ++i)
				{
					node_impl_pointer x = end_->prior();

					/* only this can possibly throw */
					std::size_t h = hash_(key(index_node_type::from_impl(x)->value()));

					hashes.data()[i] = h;
					node_ptrs.data()[i] = x;
					within_bucket = !node_alg::unlink_last(end_);
					node_alg::link(x, buckets_cpy.at(buckets_cpy.position(h)), cpy_end);
				}
			}
			BOOST_CATCH(...)
			{
				if(i != 0)
				{
					std::size_t prev_buc = buckets.position(hashes.data()[i-1]);
					if(!within_bucket)
					{
						prev_buc = ~prev_buc;
					}

					for(std::size_t j = i; j--;)
					{
						std::size_t buc = buckets.position(hashes.data()[j]);
						node_impl_pointer x = node_ptrs.data()[j];
						if(buc == prev_buc)
						{
							node_alg::append(x, end_);
						}
						else 
						{
							node_alg::link(x, buckets.at(buc), end_);
						}
						prev_buc = buc;
					}
				}
				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}

		end_->prior() = cpy_end->prior() != cpy_end? cpy_end->prior() : end_;
		end_->next() = cpy_end->next();
		end_->prior()->next()->prior() = end_->next()->prior()->prior() = end_;
		buckets.swap(buckets_cpy);
		calculate_max_load();
	}

	void unchecked_rehash(size_type n, 
							boost::multi_index::detail::hashed_non_unique_tag)
	{
		node_impl_type cpy_end_node;
		node_impl_pointer cpy_end = node_impl_pointer(&cpy_end_node);
		node_impl_pointer end_ = header()->impl();

		bucket_array_type buckets_cpy(get_allocator(), cpy_end, n);

		org_type& org = org_cast();

		if(org.size() != 0)
		{
			boost::multi_index::detail::auto_space
			<
				std::size_t, 
				allocator_type
			> hashes(get_allocator(), org.size());

			boost::multi_index::detail::auto_space
			<
				node_impl_pointer,
				allocator_type
			> node_ptrs(get_allocator(), org.size());

			std::size_t i = 0;
			bool within_bucket = false;
			BOOST_TRY
			{
				for(; ; ++i)
				{
					node_impl_pointer x = end_->prior();
					if(x == end_) 
					{
						break;
					}

					/* only this can possibly throw */
					std::size_t h = hash_(key(index_node_type::from_impl(x)->value()));

					hashes.data()[i] = h;
					node_ptrs.data()[i] = x;
					std::pair<node_impl_pointer, bool> p = node_alg::unlink_last_group(end_);
					node_alg::link_range(p.first, x, 
											buckets_cpy.at(buckets_cpy.position(h)), cpy_end);
					within_bucket = !(p.second);
				}
			}
			BOOST_CATCH(...)
			{
				if(i != 0)
				{
					std::size_t prev_buc = buckets.position(hashes.data()[i - 1]);
					if(!within_bucket)
					{
						prev_buc = ~prev_buc;
					}

					for(std::size_t j = i; j--; )
					{
						std::size_t buc = buckets.position(hashes.data()[j]);
						node_impl_pointer x = node_ptrs.data()[j];
						node_impl_pointer y = 
							x->prior()->next() != node_impl_type::base_pointer_from(x)
							&& x->prior()->next()->prior() != x?
								node_impl_type::pointer_from(x->prior()->next()) : x;
						node_alg::unlink_range(y, x);
						if(buc == prev_buc)
						{
							node_alg::append_range(y, x, end_);
						}
						else 
						{
							node_alg::link_range(y, x, buckets.at(buc), end_);
						}
						prev_buc = buc;
					}
				}
				BOOST_RETHROW;
			}
			BOOST_CATCH_END
		}

		end_->prior() = cpy_end->prior() != cpy_end? cpy_end->prior() : end_;
		end_->next() = cpy_end->next();
		end_->prior()->next()->prior() = end_->next()->prior()->prior() = end_;
		buckets.swap(buckets_cpy);
		calculate_max_load();
	}

	// hash_opt_support_modify

	inline bool in_place(node_impl_pointer x, key_param_type k, std::size_t buc) const
	{
		return in_place(x, k, buc, Category());
	}

	bool in_place(node_impl_pointer x, key_param_type k, std::size_t buc,
					boost::multi_index::detail::hashed_unique_tag) const
	{
		bool found = false;
		for(node_impl_pointer y = buckets.at(buc)->prior();
				y != node_impl_pointer(0);
				y = node_alg::after_local(y))
		{
			if(y == x) 
			{
				found = true;
			}
			else 
			{
				if(eq_(k, key(index_node_type::from_impl(y)->value())))
				{
					return false;
				}
			}
		}
		return found;
	}

	bool in_place(node_impl_pointer x, key_param_type k, std::size_t buc, 
					boost::multi_index::detail::hashed_non_unique_tag) const
	{
		bool found = false;
		int range_size = 0;
		for(node_impl_pointer y = buckets.at(buc)->prior();
				y != node_impl_pointer(0);)
		{
			if(node_alg::is_first_of_group(y)) /* group of 3 or more */
			{ 
				if(y == x)
				{
					/* in place <-> equal to some other member of the group */
					return eq_(k, 
								key(
									index_node_type::from_impl(
										node_impl_type::pointer_from(y->next()))->value()));
				}
				else
				{
					node_impl_pointer z = node_alg::after_local(y->next()->prior()); /* end of range */
					
					if(eq_(k, key(index_node_type::from_impl(y)->value())))
					{
						if(found)
						{
							return false; /* x lies outside */
						}

						do
						{
							if(y == x)
							{ 
								return true; 
							}
							y = node_alg::after_local(y);
						} while(y != z);
						return false; /* x not found */
					}
					else
					{
						if(range_size == 1 && !found)
						{
							return false;
						}
						
						if(range_size == 2)
						{
							return found;
						}
						range_size = 0;
						y = z; /* skip range (and potentially x, too, which is fine) */
					}
				}
			}
			else /* group of 1 or 2 */
			{ 
				if(y == x)
				{
					if(range_size == 1) 
					{ 
						return true; 
					}
					range_size = 1;
					found = true;
				}
				else 
				{
					if(eq_(k, key(index_node_type::from_impl(y)->value())))
					{
						if(range_size == 0 && found)
						{
							return false;
						}
						
						if(range_size == 1 && !found)
						{
							return false;
						}

						if(range_size == 2)
						{
							return false;
						}
						++range_size;
					}
					else
					{
						if(range_size == 1 && !found)
						{
							return false;
						}
						
						if(range_size == 2)
						{
							return found;
						}

						range_size = 0;
					}
				}
				y = node_alg::after_local(y);
			}
		}

		return found;
	}
	
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	inline void detach_iterators(index_node_type* x)
	{
		iterator it=make_iterator(x);
		{
			using namespace  boost::multi_index;
			safe_mode::detach_equivalent_iterators(it);
		}
	}
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	template<typename CompatiblePred> inline
	iterator find(std::size_t key_hash, const key_type& k,
					const CompatiblePred& eq, boost::mpl::true_) const
	{
		return find(key_hash, k, eq, boost::mpl::false_());
	}

	template<typename CompatibleKey, typename CompatiblePred>
	iterator find(std::size_t key_hash, const CompatibleKey& k,
					 const CompatiblePred& eq, boost::mpl::false_) const
	{
		std::size_t buc = buckets.position(key_hash);
		for(node_impl_pointer x = buckets.at(buc)->prior();
				x != node_impl_pointer(0);
				x = node_alg::next_to_inspect(x) )
		{
			if(eq(k, key(index_node_type::from_impl(x)->value())))
			{
				return make_iterator(index_node_type::from_impl(x));
			}
		}
		return org_cast().end();
	}

	template<typename CompatiblePred> inline
	size_type count(std::size_t key_hash, const key_type& k,
						const CompatiblePred& eq, boost::mpl::true_) const
	{
		return count(key_hash, k, eq, boost::mpl::false_());
	}

	template<typename CompatibleKey, typename CompatiblePred>
	size_type count(std::size_t key_hash, const CompatibleKey& k, 
						const CompatiblePred& eq, boost::mpl::false_) const
	{
		std::size_t buc = buckets.position(key_hash);
		for(node_impl_pointer x = buckets.at(buc)->prior();
				x != node_impl_pointer(0);
				x = node_alg::next_to_inspect(x))
		{
			if(eq(k, key(index_node_type::from_impl(x)->value())))
			{
				size_type res = 0;
				node_impl_pointer y = end_of_range(x);
				do
				{
					++res;
					x = node_alg::after(x);
				} while(x != y);

				return res;
			}
		}
		return 0;
	}

	template<typename CompatiblePred> inline
	std::pair<iterator,iterator> 
		equal_range(std::size_t key_hash, const key_type& k,
						const CompatiblePred& eq, boost::mpl::true_) const
	{
		return equal_range(key_hash, k, eq, boost::mpl::false_());
	}

	template<typename CompatibleKey, typename CompatiblePred>
	std::pair<iterator, iterator> 
		equal_range(std::size_t key_hash, const CompatibleKey& k, 
						const CompatiblePred& eq, boost::mpl::false_) const
	{
		std::size_t buc = buckets.position(key_hash);
		for(node_impl_pointer x = buckets.at(buc)->prior();
				x != node_impl_pointer(0);
				x = node_alg::next_to_inspect(x))
		{
			if(eq(k, key(index_node_type::from_impl(x)->value())))
			{
				return std::pair<iterator, iterator>(
						make_iterator(index_node_type::from_impl(x)),
						make_iterator(index_node_type::from_impl(end_of_range(x))));
			}
		}

		return std::pair<iterator, iterator>(org_cast().end(), org_cast().end());
	}

//------------------------------

// hash_opt_support
public:
	template<typename H, typename K> inline 
	static std::size_t s_hash(const H& h, const K& k)
	{
		return hash_helper<default_policy<std::size_t> >(h, k);
	}

	template<typename Sequence, typename H, typename InputIt> inline 
	static std::pair<typename boost::range_iterator<Sequence>::type,
					typename boost::range_iterator<Sequence>::type >
		s_hash_range(Sequence& seq, const H& h, InputIt first, InputIt last)
	{
		typedef std::pair<typename boost::range_iterator<Sequence>::type,
				typename boost::range_iterator<Sequence>::type > ret_type;

		return hash_range_helper<default_policy<std::size_t> >(seq, h, first, last);
	}

protected:
	key_from_value               key;
	hasher                       hash_;
	key_equal                    eq_;
	bucket_array_type            buckets;
	float                        mlf;
	size_type                    max_load;

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif
};


template<typename Arg1, typename Arg2,
			typename Arg3, typename Arg4>
struct boost_midx_hashed_unique_opt
{
	typedef boost::multi_index::hashed_unique<Arg1, Arg2, Arg3, Arg4> org_type;

	typedef typename org_type::index_args index_args;
	typedef typename org_type::tag_list_type tag_list_type;
	typedef typename org_type::key_from_value_type key_from_value_type;
	typedef typename org_type::hash_type hash_type;
	typedef typename org_type::pred_type pred_type;

	template<typename Super>
	struct node_class
	{
		typedef 
			boost::multi_index::detail::hashed_index_node
			<
				Super
			> type;
	};

	template<typename SuperMeta>
	struct index_class
	{
		typedef
			boost_midx_hashed_index_opt
			<
				key_from_value_type,
				hash_type,
				pred_type,
				SuperMeta,
				tag_list_type,
				boost::multi_index::detail::hashed_unique_tag
			> type;
	};
};

template<typename Arg1, typename Arg2,
			typename Arg3, typename Arg4>
struct boost_midx_hashed_non_unique_opt
{
	typedef boost::multi_index::hashed_non_unique< Arg1, Arg2, Arg3, Arg3 > org_type;

	typedef typename org_type::index_args index_args;
	typedef typename org_type::tag_list_type tag_list_type;
	typedef typename org_type::key_from_value_type key_from_value_type;
	typedef typename org_type::hash_type hash_type;
	typedef typename org_type::pred_type pred_type;

	template<typename Super>
	struct node_class
	{
		typedef 
			boost::multi_index::detail::hashed_index_node
			<
				Super
			> type;
	};

	template<typename SuperMeta>
	struct index_class
	{
		typedef
			boost_midx_hashed_index_opt
			<
				key_from_value_type,
				hash_type,
				pred_type,
				SuperMeta,
				tag_list_type,
				boost::multi_index::detail::hashed_non_unique_tag
			> type;
	};
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#if defined(YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT)
#	undef YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT
#endif // YGGR_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_107400_BOOST_MIDX_HASHED_INDEX_OPT_HPP__
