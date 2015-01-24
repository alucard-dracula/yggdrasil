//safe_multi_index_container.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_CONTAINER_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_CONTAINER_HPP__

#include <boost/multi_index/key_extractors.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
//#include <boost/mpl/if.hpp>
//#include <boost/type_traits/is_base_of.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/helper/mutex_def_helper.hpp>

#include <yggr/container/multi_index_container.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>
#include <yggr/safe_container/detail/multi_index_method.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>

namespace yggr
{
namespace safe_container
{

template<	typename Val,
			typename IndexSpecifierList = boost::multi_index::indexed_by<
												boost::multi_index::ordered_unique<
													boost::multi_index::identity< Val > > >,
			typename Mutex = boost::mutex,
			typename Alloc = std::allocator<Val>,
			template<typename _Val, typename _IndexSpecifierList, typename _Alloc>
				class Multi_Index_Container = boost::multi_index::multi_index_container
		>
class safe_multi_index_container
	: protected Multi_Index_Container<Val, IndexSpecifierList, Alloc>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:

	typedef Val val_type;
	typedef Alloc alloc_type;
	typedef IndexSpecifierList index_specifier_type_list;

public:
	typedef Multi_Index_Container<val_type, index_specifier_type_list, alloc_type> base_type;

	typedef typename base_type::ctor_args_list ctor_args_list;
	//typedef typename index_specifier_type_list index_specifier_type_list;

	typedef typename base_type::index_type_list index_type_list;

	typedef typename base_type::iterator_type_list       iterator_type_list;
	typedef typename base_type::const_iterator_type_list const_iterator_type_list;
	typedef typename base_type::value_type               value_type;
	typedef typename base_type::allocator_type		     allocator_type;
	typedef typename base_type::iterator                 iterator;
	typedef typename base_type::const_iterator           const_iterator;
	typedef typename base_type::node_type				 node_type;

private:
	typedef Mutex mutex_type;
	typedef helper::mutex_def_helper<mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

	typedef safe_multi_index_container this_type;

public:
	safe_multi_index_container(void)
	{
	}

	safe_multi_index_container(const alloc_type& alloc)
		: base_type(alloc)
	{
	}

	template<typename InputIterator>
	safe_multi_index_container(InputIterator first, InputIterator last)
		: base_type(first, last)
	{
	}

	template<typename InputIterator>
	safe_multi_index_container(InputIterator first, InputIterator last, const alloc_type& alloc)
		: base_type(first, last, ctor_args_list(), alloc)
	{
	}

	template<typename InputIterator>
	safe_multi_index_container(InputIterator first, InputIterator last, const ctor_args_list& args_list, const alloc_type& alloc)
		: base_type(first, last, args_list, alloc)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	safe_multi_index_container(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}
#else
	safe_multi_index_container(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	safe_multi_index_container(const base_type& right)
		: base_type(right)
	{
	}

	~safe_multi_index_container(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		write_lock_type lk(_mutex);
		base_type::operator=(right);
		return *this;
	}

	// capacity:
	bool empty(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::empty();
	}

	size_type size(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::size();
	}

	size_type max_size(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::max_size();
	}

	void clear(void)
	{
		write_lock_type lk(_mutex);
		base_type tmp(base_type::get_allocator());
		base_type::swap(tmp);
	}

	void swap(BOOST_RV_REF(base_type) right)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(base_type& right)
	{
		write_lock_type lk(_mutex);
		base_type::swap(right);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		write_lock_type lk(_mutex);
		base_type& base = *this;
		right.swap(base);
	}

	// allocator
	alloc_type get_allocator(void) const
	{
		read_lock_type lk(_mutex);
		return base_type::get_allocator();
	}

	// safe_other:

	void copy_to_base(base_type& out) const
	{
		read_lock_type lk(_mutex);
		const base_type& base = *this;
		out = base;
	}

	template<typename Key>
	bool is_exists(const Key& key) const
	{
		return is_exists<0>(key);
	}

	template<int N, typename Key>
	bool is_exists(const Key& key) const
	{
		read_lock_type lk(_mutex);
		return prv_is_exists(base_type::template get<N>(), key);
	}

	template<typename Tag, typename Key>
	bool is_exists(const Key& key) const
	{
		read_lock_type lk(_mutex);
		return prv_is_exists(base_type::template get<Tag>(), key);
	}

	template<typename Method_Handler>
	typename Method_Handler::result_type use_method(const Method_Handler& handler)
	{
		return use_method<0>(handler);
	}

	template<int N, typename Method_Handler>
	typename Method_Handler::result_type use_method(const Method_Handler& handler)
	{
		write_lock_type lk(_mutex);
		return handler(base_type::template get<N>());
	}

	template<int N, typename Method_Handler>
	typename Method_Handler::result_type use_method(const Method_Handler& handler) const
	{
		read_lock_type lk(_mutex);
		return handler(base_type::template get<N>());
	}

	template<typename Tag, typename Method_Handler>
	typename Method_Handler::result_type use_method(const Method_Handler& handler)
	{
		write_lock_type lk(_mutex);
		return handler(base_type::template get<Tag>());
	}

	template<typename Tag, typename Method_Handler>
	typename Method_Handler::result_type use_method(const Method_Handler& handler) const
	{
		read_lock_type lk(_mutex);
		return handler(base_type::template get<Tag>());
	}

	// use handler:
	template<typename Handler>
	typename Handler::result_type use_handler(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		base_type& base = *this;
		return handler(base);
	}

	template<typename Handler>
	typename Handler::result_type use_handler(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		const base_type& base = *this;
		return handler(base);
	}

	template<typename Handler, typename Return_Handler>
	typename Handler::result_type use_handler(const Handler& handler, const Return_Handler& ret)
	{
		write_lock_type lk(_mutex);
		base_type& base = *this;
		return handler(base, ret);
	}

	template<typename Handler, typename Return_Handler>
	typename Handler::result_type use_handler(const Handler& handler, const Return_Handler& ret) const
	{
		read_lock_type lk(_mutex);
		const base_type& base = *this;
		return handler(base, ret);
	}

public:
	//static foo

	inline static const typename base_type::template nth_index<0>::type* const this_index_t(void)
	{
		return this_type::this_index_t<0>();
	}

	template<int N> inline
	static const typename base_type::template nth_index<N>::type* const this_index_t(void)
	{
		typedef typename base_type::template nth_index<N> index_type;
		typedef typename index_type::type container_type;

		return static_cast<const container_type* const>(0);
	}

	template<typename Tag> inline
	static const typename base_type::template index<Tag>::type* const this_index_t(void)
	{
		typedef Tag tag_type;
		typedef typename base_type::template index<tag_type> index_type;
		typedef typename index_type::type container_type;

		return static_cast<const container_type* const>(0);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 6, typename T ) > inline
	static typename detail::hash_index_method< YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::hashed_index,
											YGGR_PP_TEMPLATE_PARAMS_TYPES( 6, T ) ) >
			::use_method
		make_method(const YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::hashed_index, YGGR_PP_TEMPLATE_PARAMS_TYPES( 6, T ) )* const)
	{
		typedef typename
			detail::hash_index_method< YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::hashed_index,
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 6, T ) ) >
				::use_method use_type;
		return use_type();
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 5, typename T ) > inline
	static typename detail::ordered_index_method< YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::ordered_index,
											YGGR_PP_TEMPLATE_PARAMS_TYPES( 5, T ) ) >
			::use_method
		make_method(const YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::ordered_index, YGGR_PP_TEMPLATE_PARAMS_TYPES( 5, T ) )* const)
	{
		typedef typename
			detail::ordered_index_method< YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::ordered_index,
											YGGR_PP_TEMPLATE_PARAMS_TYPES( 5, T ) ) >
				::use_method use_type;
		return use_type();
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T ) > inline
	static typename detail::sequenced_index_method< YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::sequenced_index,
													YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ) ) >
			::use_method
		make_method(const YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::sequenced_index, YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ) )* const)
	{
		typedef typename
			detail::sequenced_index_method< YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::sequenced_index,
											YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ) ) >
				::use_method use_type;
		return use_type();
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T ) > inline
	static typename detail::random_access_index_method< YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::random_access_index,
														YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ) ) >
			::use_method
		make_method(const YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::random_access_index, YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ) )* const)
	{
		typedef typename
			detail::random_access_index_method< YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::random_access_index,
												YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ) ) >
				::use_method use_type;
		return use_type();
	}

private:
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 6, typename T ), typename Key >
	bool prv_is_exists(const YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::hashed_index, YGGR_PP_TEMPLATE_PARAMS_TYPES( 6, T ) )& cont,
						const Key& key) const
	{
		return cont.find(key) != cont.end();
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 5, typename T ), typename Key >
	bool prv_is_exists(const YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::ordered_index, YGGR_PP_TEMPLATE_PARAMS_TYPES( 5, T ) )& cont,
						const Key& key) const
	{
		return cont.find(key) != cont.end();
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T ), typename Key >
	bool prv_is_exists(const YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::sequenced_index, YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ) )& cont,
						const Key& key) const
	{
		return std::find(cont.begin(), cont.end(), key) != cont.end();
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T ), typename Key >
	bool prv_is_exists(const YGGR_PP_TEMPLATE_TYPE(boost::multi_index::detail::random_access_index, YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ) )& cont,
						const Key& key) const
	{
		return std::find(cont.begin(), cont.end(), key) != cont.end();
	}

private:
	mutable mutex_type _mutex;
};

namespace multi_index_container_detail
{
	template<typename Safe_Mulit_Index_Container> inline
	static const typename Safe_Mulit_Index_Container
							::base_type
							::template nth_index<0>::type* const this_index_t(void)
	{
		typedef Safe_Mulit_Index_Container now_this_type;
		return now_this_type::this_index_t();
	}

	template<typename Safe_Mulit_Index_Container, int N > inline
	static const typename Safe_Mulit_Index_Container
							::base_type
							::template nth_index<N>::type* const this_index_t(void)
	{
		typedef Safe_Mulit_Index_Container now_this_type;
		return now_this_type::template this_index_t<N>();
	}

	template<typename Safe_Mulit_Index_Container, typename Tag> inline
	static const typename Safe_Mulit_Index_Container
							::base_type
							::template index<Tag>::type* const this_index_t(void)
	{
		typedef Tag tag_type;
		typedef Safe_Mulit_Index_Container now_this_type;
		return now_this_type::template this_index_t<tag_type>();
	}
} // namespace multi_index_container_detail

} // namespace safe_container
} // namesapce yggr

#define YGGR_PP_MIDX_MAKE_METHOD(__type__, __tag__) \
	__type__::make_method( yggr::safe_container::multi_index_container_detail::this_index_t< __type__, __tag__ >() )

#define YGGR_PP_MIDX_MAKE_METHOD_NONTAG(__type__) \
		__type__::make_method( yggr::safe_container::multi_index_container_detail::this_index_t<__type__>( ) )

namespace std
{
	YGGR_PP_SAFE_CONTAINER_SWAP(4, 3, yggr::safe_container::safe_multi_index_container)
} // namespace std

namespace boost
{
	YGGR_PP_SAFE_CONTAINER_SWAP(4, 3, yggr::safe_container::safe_multi_index_container)
} // namespace boost

#endif //__YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_CONTAINER_HPP__
