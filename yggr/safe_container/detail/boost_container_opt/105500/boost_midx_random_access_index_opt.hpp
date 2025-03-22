// boost_midx_random_access_index_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105500_BOOST_MIDX_RANDOM_ACCESS_INDEX_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105500_BOOST_MIDX_RANDOM_ACCESS_INDEX_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_RANDOM_ACCESS_INDEX_OPT_HPP__
#	error "this file is inner file, can't include it, please include <yggr/safe_container/safe_multi_index_random_access_index.hpp>"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_RANDOM_ACCESS_INDEX_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/container/detail/boost_midx_check_helper.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>

#include <yggr/safe_container/detail/cast_midx_base_type.hpp>
#include <yggr/safe_container/detail/boost_midx_seq_index_ops_opt.hpp>

#include <yggr/safe_container/detail/boost_midx_index_base_opt.hpp>
#include <yggr/safe_container/detail/boost_midx_random_access_index_opt_decl.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

#include <cassert>
#include <iterator>


#if !defined(YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT)
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)

#	define YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT \
		::boost::multi_index::detail::scope_guard BOOST_JOIN(check_invariant_, __LINE__) = \
			::boost::multi_index::detail::make_obj_guard(*this, &boost_midx_random_access_index_opt::check_invariant_); \
	BOOST_JOIN(check_invariant_, __LINE__).touch();

#else

#	define YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#endif //YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename SuperMeta, typename TagList>
class boost_midx_random_access_index_opt :
	BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS SuperMeta::type

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
	 , public 
	 boost::multi_index::detail::safe_ctr_proxy_impl
	 <
		boost::multi_index::detail::rnd_node_iterator
		<
			boost::multi_index::detail::random_access_index_node
			<
				typename SuperMeta::type::index_node_type
			> 
		>,
		boost_midx_random_access_index_opt<SuperMeta, TagList> 
	 >
#else
	, public 
	boost::multi_index::safe_mode::safe_container
	<
		boost_midx_random_access_index_opt<SuperMeta, TagList> 
	>

#endif //BOOST_WORKAROUND(BOOST_MSVC,<1300)
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
		boost::multi_index::detail::random_access_index
		<
			typename nth_layer_org_cast<SuperMeta>::type,
			TagList
		>														org_type;


	typedef typename SuperMeta::type							super;

protected:
	typedef 
		boost::multi_index::detail::random_access_index_node
		<
			typename super::index_node_type
		>														index_node_type;

private:
	typedef typename index_node_type::impl_type					node_impl_type;
	typedef 
		boost::multi_index::detail::
			random_access_index_ptr_array
		<
			typename super::final_allocator_type
		>														ptr_array;
	typedef typename ptr_array::pointer							node_impl_ptr_pointer;

public:
	/* types */

	typedef typename org_type::value_type						value_type;
	typedef typename org_type::ctor_args						ctor_args;
	typedef typename org_type::allocator_type					allocator_type;
	typedef typename org_type::reference						reference;
	typedef typename org_type::const_reference					const_reference;

	typedef typename org_type::iterator							iterator;
	typedef typename org_type::const_iterator					const_iterator;

	typedef typename org_type::size_type						size_type;
	typedef typename org_type::difference_type					difference_type;
	typedef typename org_type::pointer							pointer;
	typedef typename org_type::const_pointer					const_pointer;
	typedef typename org_type::reverse_iterator					reverse_iterator;
	typedef typename org_type::const_reverse_iterator			const_reverse_iterator;
	typedef typename org_type::tag_list							tag_list;

	typedef index_node_type*									node_ptr_type;

	typedef typename super::final_node_handle_type				node_type;
	typedef 
		container::detail::insert_return_type_base
		<
			iterator,
			node_type
		>														insert_return_type;

protected:
	typedef typename super::final_node_type						final_node_type;
	typedef 
		boost::tuples::cons
		<
			ctor_args, 
			typename super::ctor_args_list
		>														ctor_args_list;
	typedef typename 
		boost::mpl::push_front
		<
			typename super::index_type_list,
			boost_midx_random_access_index_opt
		>::type													index_type_list;
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
		>::type													const_iterator_type_list;
	typedef typename super::copy_map_type						copy_map_type;

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
	typedef typename super::index_saver_type					index_saver_type;
	typedef typename super::index_loader_type					index_loader_type;
#endif // BOOST_MULTI_INDEX_DISABLE_SERIALIZATION

private:
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)

	typedef 
		boost::multi_index::detail::safe_ctr_proxy_impl
		<
			boost::multi_index::detail::rnd_node_iterator
			<
				index_node_type
			>,
			boost_midx_random_access_index_opt
		>														safe_super;

#else

	typedef 
		boost::multi_index::safe_mode::safe_container
		<
			boost_midx_random_access_index_opt
		>														safe_super;

#endif //BOOST_WORKAROUND(BOOST_MSVC,<1300)
#endif //BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	typedef typename 
		boost::call_traits
		<
			value_type
		>::param_type											value_param_type;


private:
	typedef boost_midx_random_access_index_opt this_type;

private:
	boost_midx_random_access_index_opt(void);
	boost_midx_random_access_index_opt(BOOST_RV_REF(boost_midx_random_access_index_opt));
	boost_midx_random_access_index_opt(const boost_midx_random_access_index_opt&);
	~boost_midx_random_access_index_opt(void);
	const boost_midx_random_access_index_opt& operator=( const boost_midx_random_access_index_opt& );
	const boost_midx_random_access_index_opt& operator=( BOOST_RV_REF(boost_midx_random_access_index_opt) );

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
	template<typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		push_front(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
						BOOST_RV_REF(value_type) x)
	{
		return this_type::insert(org_cast().begin(), keyinfo_params, boost::move(x));
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		push_front(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
						const value_type& x)
	{
		return this_type::insert(org_cast().begin(), keyinfo_params, x);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		push_front(Destroy_Holder& holder,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
						final_node_type* x)
	{
		return this_type::insert(holder, org_cast().begin(), keyinfo_params, x);
	}

	template<typename Destroy_Holder> inline
	void pop_front(Destroy_Holder& holder)
	{
		this_type::erase(holder, org_cast().begin());
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		push_back(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					BOOST_RV_REF(value_type) x)
	{
		return this_type::insert(org_cast().end(), keyinfo_params, boost::move(x));
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		push_back(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					const value_type& x)
	{
		return this_type::insert(org_cast().end(), keyinfo_params, x);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		push_back(Destroy_Holder& holder,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					final_node_type* x)
	{
		return this_type::insert(holder, org_cast().end(), keyinfo_params, x);
	}

	template<typename Destroy_Holder> inline
	void pop_back(Destroy_Holder& holder)
	{
		this_type::erase(holder, --(org_cast().end()));
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		insert(iterator position, 
				const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
				BOOST_RV_REF(value_type) x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p = this->final_insert_rv_(keyinfo_params, x);
		if(p.second && position.get_node() != header())
		{
			this_type::relocate(position.get_node(), p.first);
		}

		return std::pair<iterator, bool>(make_iterator(p.first), p.second);
	}

	template<typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		insert(iterator position, 
				const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
				const value_type& x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p = this->final_insert_(keyinfo_params, x);
		if(p.second && position.get_node() != header())
		{
			this_type::relocate(position.get_node(), p.first);
		}

		return std::pair<iterator, bool>(make_iterator(p.first), p.second);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map> inline
	std::pair<iterator, bool> 
		insert(Destroy_Holder& holder, iterator position, 
				const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
				final_node_type* x)
	{
		assert(x);
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

		std::pair<final_node_type*, bool> p = this->final_insert_(holder, keyinfo_params, x);
		if(p.second && position.get_node() != header())
		{
			this_type::relocate(position.get_node(), p.first);
		}

		return std::pair<iterator, bool>(make_iterator(p.first), p.second);
	}

	template<typename T, typename Meta_Map>
	void insert(iterator position, size_type n,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					value_param_type x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

		org_type& org = org_cast();
		size_type s = 0;
		BOOST_TRY
		{
			while(n--)
			{
				if(this_type::push_back(keyinfo_params, x).second)
				{
					++s;
				}
			}
		}
		BOOST_CATCH(...)
		{
			org.relocate(position, org.end() - s, org.end());
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
		org.relocate(position, org.end() - s, org.end());
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename InputIterator> inline
	void insert(Destroy_Holder& holder, iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					InputIterator first, InputIterator last)
	{
		this_type::insert_iter(holder, position, keyinfo_params, first, last);
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename InputIterator> inline
	void insert(Destroy_Holder& holder, iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					const std::pair<InputIterator, InputIterator>& iter_pair_node)
	{
		this_type::insert_iter(holder, position, keyinfo_params, iter_pair_node.first, iter_pair_node.second);
	}

	template<typename KeyParams_Iter, typename InputIterator> inline
	void insert(iterator position,
					const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					InputIterator first, InputIterator last)
	{
		assert(( std::distance(iter_pair.first, iter_pair.second)
					== std::distance(first, last) ));
		this_type::insert_iter(position, iter_pair, first, last,
								boost::mpl::not_< boost::is_integral<InputIterator> >());
	}

	template<typename Destroy_Holder, typename KeyParams_Iter, typename InputIterator> inline
	void insert(Destroy_Holder& holder, iterator position,
					const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					InputIterator first, InputIterator last)
	{
		assert(( std::distance(iter_pair.first, iter_pair.second)
					== std::distance(first, last) ));
		this_type::insert_iter(holder, position, iter_pair, first, last,
								boost::mpl::not_< boost::is_integral<InputIterator> >());
	}

	template<typename Destroy_Holder, typename KeyParams_Iter, typename InputIterator> inline
	void insert(Destroy_Holder& holder, iterator position,
					const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
					const std::pair<InputIterator, InputIterator>& iter_pair_node)
	{
		return this_type::insert(holder, position, iter_pair, iter_pair_node.first, iter_pair_node.second);
	}

	template<typename T, typename Meta_Map> inline
	insert_return_type insert(const_iterator position,
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
			YGGR_MULTI_INDEX_CHECK_EQUAL_ALLOCATORS(org_cast(), nh);
		}
		YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		std::pair<final_node_type*, bool> p = this->final_insert_nh_(keyinfo_params, nh); // native
		if(p.second && position.get_node() != header())
		{
			this_type::relocate(position.get_node(), p.first);
		}

		return insert_return_type(p.second, make_iterator(p.first), boost::move(nh));
	}

	inline node_type extract(const_iterator position)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}
		
		return 
			this->final_extract_(
				static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder> inline
	iterator erase(Destroy_Holder& holder, iterator position)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
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
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

		org_type& org = org_cast();
		difference_type n = last - first;
		org.relocate(org.end(), first, last);
		
		while(n--)
		{
			this_type::pop_back(holder);
		}
		return last;
	}

	template<typename T, typename Meta_Map> inline
	bool replace(iterator position,
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					BOOST_RV_REF(value_type) x)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
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
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
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
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(keyinfo_params, mod,
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename Modifier> inline
	bool modify(Destroy_Holder& holder, iterator position, Modifier mod)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(holder, mod,
									static_cast<final_node_type*>(position.get_node()));
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
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(holder, keyinfo_params, mod,
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename T, typename Meta_Map, typename Modifier, typename Rollback> inline
	bool modify(iterator position, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					Modifier mod, Rollback back)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(keyinfo_params, mod, back,
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename Modifier, typename Rollback> inline
	bool modify(Destroy_Holder& holder, iterator position, Modifier mod, Rollback back)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(holder, mod, back,
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename Modifier, typename Rollback> inline
	bool modify(Destroy_Holder& holder, iterator position, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					Modifier mod, Rollback back)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		/* MSVC++ 6.0 optimizer on safe mode code chokes if this
			* this is not added. Left it for all compilers as it does no
			* harm.
			*/

		position.detach();
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

		return this->final_modify_(holder, keyinfo_params, mod, back,
									static_cast<final_node_type*>(position.get_node()));
	}

	template<typename Destroy_Holder>
	void remove(Destroy_Holder& holder, value_param_type value)
	{
		YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;

		difference_type n 
			= org_cast().end() 
				- make_iterator(
					boost::multi_index::detail::random_access_index_remove<index_node_type>(		
						ptrs,
#					if YGGR_CPP_VERSION < YGGR_CPP_VER_17
						std::bind2nd(std::equal_to<value_type>(), value)
#					else
						boost::bind(std::equal_to<value_type>(), _1, boost::cref(value))
#					endif // YGGR_CPP_VERSION < YGGR_CPP_VER_17
						) );
		
		while(n--)
		{
			pop_back(holder);
		}
	}

	template<typename Destroy_Holder, typename Predicate>
	void remove_if(Destroy_Holder& holder, Predicate pred)
	{
		YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		difference_type n
			= org_cast().end() 
				- make_iterator(
					boost::multi_index::detail::random_access_index_remove<index_node_type>(ptrs, pred));
		
		while(n--)
		{
			pop_back(holder);
		}
	}
    
BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)

	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node, &org_cast());
	}
	
	inline const_iterator make_iterator(index_node_type* node) const
	{
		return const_iterator(node, const_cast<boost_midx_random_access_index_opt*>(&org_cast()));
	}

#else

	inline iterator make_iterator(index_node_type* node)
	{
		return iterator(node);
	}
	
	inline const_iterator make_iterator(index_node_type* node) const
	{
		return const_iterator(node);
	}

#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	inline void extract_(index_node_type* x)
	{
		ptrs.erase(x->impl());
		super::extract_(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename T, typename Meta_Map, typename Variant> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						value_param_type v, index_node_type* x, Variant variant)
	{
		ptrs.room_for_one();
		index_node_type* res = static_cast<index_node_type*>(super::insert_(keyinfo_params, v, x, variant));
		if(res == x)
		{
			ptrs.push_back(x->impl());
		}
		return res;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						index_node_type* x)
	{
		ptrs.room_for_one();
		index_node_type* res = static_cast<index_node_type*>(super::insert_(keyinfo_params, x));
		if(res == x)
		{
			ptrs.push_back(x->impl());
		}
		return res;
	}

	template<typename T, typename Meta_Map, typename Variant> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						value_param_type v, index_node_type* position, index_node_type* x, Variant variant)
	{
		ptrs.room_for_one();
		index_node_type* res = static_cast<index_node_type*>(super::insert_(keyinfo_params, v, position, x, variant));
		if(res == x)
		{
			ptrs.push_back(x->impl());
		}
		return res;
	}

	template<typename T, typename Meta_Map> inline
	index_node_type* insert_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
							index_node_type* position, index_node_type* x)
	{
		ptrs.room_for_one();
		index_node_type* res = static_cast<index_node_type*>(super::insert_(keyinfo_params, position, x));
		if(res == x)
		{
			ptrs.push_back(x->impl());
		}
		return res;
	}

	inline void erase_(index_node_type* x)
	{
		ptrs.erase(x->impl());
		super::erase_(x);

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename Destroy_Holder> inline
	void erase_(Destroy_Holder& holder, index_node_type* x)
	{
		ptrs.erase(x->impl());
		super::erase_(holder, x);

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
		detach_iterators(x);
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
	}

	template<typename T, typename Meta_Map, typename Variant> inline
	bool replace_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
					value_param_type v, index_node_type* x, Variant variant)
	{
		return super::replace_(keyinfo_params, v, x, variant);
	}

	template<typename T, typename Meta_Map> inline
	bool modify_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					index_node_type* x)
	{
		BOOST_TRY
		{
			if(!super::modify_(keyinfo_params, x))
			{
				ptrs.erase(x->impl());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				return false;
			}
			else 
			{ 
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			ptrs.erase(x->impl());

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename Destroy_Holder> inline
	bool modify_(Destroy_Holder& holder, index_node_type* x)
	{
		BOOST_TRY
		{
			if(!super::modify_(holder, x))
			{
				ptrs.erase(x->impl());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				return false;
			}
			else 
			{ 
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			ptrs.erase(x->impl());

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	template<typename Destory_Holder, typename T, typename Meta_Map> inline
	bool modify_(Destory_Holder& holder, 
					const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
					index_node_type* x)
	{
		BOOST_TRY
		{
			if(!super::modify_(holder, keyinfo_params, x))
			{
				ptrs.erase(x->impl());

#		if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
				detach_iterators(x);
#		endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

				return false;
			}
			else 
			{ 
				return true;
			}
		}
		BOOST_CATCH(...)
		{
			ptrs.erase(x->impl());

#	if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
			detach_iterators(x);
#	endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

			BOOST_RETHROW;
		}
		BOOST_CATCH_END
	}

	inline bool modify_rollback_(index_node_type* x)
	{
		return super::modify_rollback_(x);
	}

	template<typename T, typename Meta_Map> inline
	bool modify_rollback_(const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params, 
							index_node_type* x)
	{
		return super::modify_rollback_(keyinfo_params, x);
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
	/* invariant stuff */

	bool invariant_(void) const
	{
		const org_type& org = org_cast();

		if(org.size() > org.capacity())
		{
			return false;
		}

		if(org.size() == 0 || org.begin() == org.end())
		{
			if(org.size() != 0 || org.begin() != org.end())
			{
				return false;
			}
		}
		else
		{
			size_type s = 0;
			for(const_iterator it = org.begin(), it_end = org.end(); ; ++it, ++s)
			{
				if(*(it.get_node()->up()) != it.get_node()->impl()) 
				{
					return false;
				}
				
				if(it == it_end)
				{
					break;
				}
			}
			
			if( s != org.size())
			{
				return false;
			}
		}

		return super::invariant_();
	}

	/* This forwarding function eases things for the boost::mem_fn construct
	* in YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT. Actually,
	* final_check_invariant is already an inherited member function of index.
	*/
	
	inline void check_invariant_(void) const
	{
		this->final_check_invariant_();
	}

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

private:
	inline index_node_type* header() const{ return this->final_header(); }

	inline static void relocate(index_node_type* position, index_node_type* x)
	{
		node_impl_type::relocate(position->up(), x->up());
	}

	inline static void relocate(index_node_type* position, index_node_type* first, index_node_type* last)
	{
		node_impl_type::relocate(position->up(), first->up(), last->up());
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
	inline void detach_iterators(index_node_type* x)
	{
		iterator it = make_iterator(x);
		boost::multi_index::safe_mode::detach_equivalent_iterators(it);
	}
#endif // BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

	template<typename KeyParams_Iter, typename InputIterator>
	void insert_iter(iterator position, 
						const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
						InputIterator first, InputIterator last, boost::mpl::true_)
	{
		YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		org_type& org = org_cast();
		size_type s = 0;
		BOOST_TRY
		{
			for(KeyParams_Iter ki = iter_pair.first; first != last; ++first, ++ki)
			{
				if(this->final_insert_ref_(*ki, *first).second)
				{
					++s;
				}
			}
		}
		BOOST_CATCH(...)
		{
			org.relocate(position, org.end() - s, org.end());
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
		org.relocate(position, org.end() - s, org.end());
	}

	template<typename Destroy_Holder, typename KeyParams_Iter, typename InputIterator>
	void insert_iter(Destroy_Holder& holder, iterator position, 
						const std::pair<KeyParams_Iter, KeyParams_Iter>& iter_pair,
						InputIterator first, InputIterator last, boost::mpl::true_)
	{
		YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		org_type& org = org_cast();
		size_type s = 0;
		BOOST_TRY
		{
			
			for(KeyParams_Iter ki = iter_pair.first; first != last; ++first, ++ki)
			{
				if(push_back(holder, *ki, *first).second) 
				{
					++s;
				}
			}
		}
		BOOST_CATCH(...)
		{
			org.relocate(position, org.end() - s, org.end());
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
		org.relocate(position, org.end() - s, org.end());
	}

	template<typename T, typename Meta_Map>
	void insert_iter(iterator position, 
						size_type n,
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						value_param_type x,
						boost::mpl::false_)
	{
		{
			using namespace boost::multi_index;
			BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
			BOOST_MULTI_INDEX_CHECK_IS_OWNER(position, org_cast());
			YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		}

		org_type& org = org_cast();
		size_type s = 0;
		BOOST_TRY
		{
			while(n--)
			{
				if(push_back(keyinfo_params, x).second)
				{
					++s;
				}
			}
		}
		BOOST_CATCH(...)
		{
			org.relocate(position, org.end() - s, org.end());
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
		org.relocate(position, org.end() - s, org.end());
	}

	template<typename Destroy_Holder, typename T, typename Meta_Map, typename InputIterator>
	void insert_iter(Destroy_Holder& holder, iterator position, 
						const boost_midx_keyinfo_params<T, Meta_Map>& keyinfo_params,
						InputIterator first, InputIterator last)
	{
		YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT;
		org_type& org = org_cast();
		size_type s = 0;
		BOOST_TRY
		{
			
			for(; first != last; ++first)
			{
				if(push_back(holder, keyinfo_params, *first).second) 
				{
					++s;
				}
			}
		}
		BOOST_CATCH(...)
		{
			org.relocate(position, org.end() - s, org.end());
			BOOST_RETHROW;
		}
		BOOST_CATCH_END
		org.relocate(position, org.end() - s, org.end());
	}
 
	ptr_array ptrs;

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING) && \
		BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
};

template <typename TagList>
struct boost_midx_random_access_opt
{
	BOOST_STATIC_ASSERT(boost::multi_index::detail::is_tag<TagList>::value);

	typedef typename boost::multi_index::random_access<TagList> org_type;

	template<typename Super>
	struct node_class
	{
		typedef boost::multi_index::detail::random_access_index_node<Super> type;
	};

	template<typename SuperMeta>
	struct index_class
	{
		typedef 
			boost_midx_random_access_index_opt
			<
				SuperMeta, 
				typename TagList::type
			>  type;
	};
};

} // namespace detail
} // namespace safe_container
} // namespace yggr


#if defined(YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT)
#	undef YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT
#endif // YGGR_MULTI_INDEX_RND_INDEX_CHECK_INVARIANT

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105500_BOOST_MIDX_RANDOM_ACCESS_INDEX_OPT_HPP__
