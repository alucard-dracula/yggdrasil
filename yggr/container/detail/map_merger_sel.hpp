//map_merger_sel.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_MAP_MERGER_SEL_HPP__
#define __YGGR_CONTAINER_DETAIL_MAP_MERGER_SEL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/type_traits/is_same_tpl.hpp>

#include <yggr/mplex/null_t_to.hpp>
#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/typename_replaceor.hpp>

#include <yggr/container/detail/map_merge_impl.hpp>
#include <yggr/container/detail/has_merge.hpp>
#include <yggr/container/detail/is_s_or_m_container.hpp>
#include <yggr/container/detail/get_base_or_this.hpp>
#include <yggr/container/detail/get_tree_options.hpp>

#include <yggr/container/is_isomeric_same_map.hpp>
#include <yggr/container/is_isomeric_same_options_map.hpp>

#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container_ex/map_regularisation.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace container
{
namespace detail
{
	

template<typename T>
typename 
	container_ex::map_regularisation
	<
		typename get_base_or_this_t<T>::type
	>::type&
	map_get_regular_base_or_this(T& t)
{
	typedef typename get_base_or_this_t<T>::type base_or_this_type;
	typedef typename container_ex::map_regularisation<base_or_this_type>::type regular_base_or_this_type;

	base_or_this_type& base_or_this_ref = get_base_or_this(t);
	return reinterpret_cast<regular_base_or_this_type&>(base_or_this_ref);
}

template<typename T>
const
typename 
	container_ex::map_regularisation
	<
		typename get_base_or_this_t<T>::type
	>::type&
	map_get_regular_base_or_this(const T& t)
{
	typedef typename get_base_or_this_t<T>::type base_or_this_type;
	typedef typename container_ex::map_regularisation<base_or_this_type>::type regular_base_or_this_type;

	const base_or_this_type& base_or_this_cref = get_base_or_this(t);
	return reinterpret_cast<const regular_base_or_this_type&>(base_or_this_cref);
}

struct map_merger_eei_handler
{
public:
	template<typename L, typename R> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_single_container<L>,
				is_single_container<R>
			>,
			void
		>::type
		operator()(L& l, R& r) const
	{
		map_merge_map_impl_eei(map_get_regular_base_or_this(l), map_get_regular_base_or_this(r));
	}

	template<typename L, typename R> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_single_container<L>,
				is_multi_container<R>
			>,
			void
		>::type
		operator()(L& l, R& r) const
	{
		map_merge_multimap_impl_eei(map_get_regular_base_or_this(l), map_get_regular_base_or_this(r));
	}

	template<typename L, typename R> inline
	typename
		boost::enable_if
		<
			is_multi_container<L>,
			void
		>::type
		operator()(L& l, R& r) const
	{
		multimap_merge_impl_eei(map_get_regular_base_or_this(l), map_get_regular_base_or_this(r));
	}
};

struct map_merger_eei
{
	typedef map_merger_eei_handler type;
};

struct map_merger_eti_handler
{
public:
	template<typename L, typename R> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_single_container<L>,
				is_single_container<R>
			>,
			void
		>::type
		operator()(L& l, R& r) const
	{
		map_merge_map_impl_eti(map_get_regular_base_or_this(l), map_get_regular_base_or_this(r));
	}

	template<typename L, typename R> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_single_container<L>,
				is_multi_container<R>
			>,
			void
		>::type
		operator()(L& l, R& r) const
	{
		map_merge_multimap_impl_eti(map_get_regular_base_or_this(l), map_get_regular_base_or_this(r));
	}

	template<typename L, typename R> inline
	typename
		boost::enable_if
		<
			is_multi_container<L>,
			void
		>::type
		operator()(L& l, R& r) const
	{
		multimap_merge_impl_eti(map_get_regular_base_or_this(l), map_get_regular_base_or_this(r));
	}
};

struct map_merger_eti
{
	typedef map_merger_eti_handler type;
};

struct map_merger_inner_handler
{
public:
	template<typename L, typename R> inline
	void operator()(L& l, R& r) const
	{
		map_get_regular_base_or_this(l).merge(map_get_regular_base_or_this(r));
	}
};

struct map_merger_inner
{
	typedef map_merger_inner_handler type;
};

template<typename L, typename R>
struct is_map_different_comp_only_base
	: public
		boost::mpl::and_
		<
			boost::is_same<typename L::value_type, typename R::value_type>,
			boost::is_same<typename L::allocator_type, typename R::allocator_type>,
			boost::is_same
			<
				typename 
					get_tree_options
					<
						typename
							mplex::null_t_to_void
							<
								typename mplex::typename_expand_get_safe<L, 3>::type
							>::type
					>::type,
				typename 
					get_tree_options
					<
						typename
							mplex::null_t_to_void
							<
								typename mplex::typename_expand_get_safe<R, 3>::type
							>::type
					>::type
			>
		>::type
{
};

template<typename L, typename R>
struct is_map_different_comp_only
	: public
		is_map_different_comp_only_base
		<
			typename get_base_or_this_t<L>::type,
			typename get_base_or_this_t<R>::type
		>::type
		
{
};

// rule

template<typename L, typename R, bool b_has_merge = has_any_merge<L>::value>
struct map_merger_sel_rule_has_merge_detail
	: public map_merger_eei
{
};

template<typename L, typename R>
struct map_merger_sel_rule_has_merge_detail<L, R, true>
	: public map_merger_inner
{
};

template<typename L, typename R>
struct map_merger_sel_rule_has_merge
	: public map_merger_sel_rule_has_merge_detail<L, R>
{
};

// map_merger_sel_rule_is_different_comp_only
template<typename L, typename R, bool b_is_different_comp_only = is_map_different_comp_only<L, R>::value>
struct map_merger_sel_rule_is_different_comp_only_detail
	: public map_merger_eei
{
};

template<typename L, typename R>
struct map_merger_sel_rule_is_different_comp_only_detail<L, R, true>
	: public map_merger_sel_rule_has_merge<L, R>
{
};

template<typename L, typename R>
struct map_merger_sel_rule_is_different_comp_only
	: public map_merger_sel_rule_is_different_comp_only_detail<L, R>
{
};

// map_merger_sel_rule_is_same_namespace
template<typename L, typename R, bool b_is_same_namespace = is_same_tpl<L, R>::value>
struct map_merger_sel_rule_is_same_namespace_detail
	: public map_merger_eei
{
};

template<typename L, typename R>
struct map_merger_sel_rule_is_same_namespace_detail<L, R, true>
	: public map_merger_sel_rule_is_different_comp_only<L, R>
{
};

template<typename L, typename R>
struct map_merger_sel_rule_is_same_namespace
	: public map_merger_sel_rule_is_same_namespace_detail<L, R>
{
};

template<typename L, typename R>
struct map_merger_sel_rule_regularisation
	: public 
		map_merger_sel_rule_is_same_namespace
		<
			typename
				get_base_or_this_t
				<
					typename ::yggr::container_ex::map_regularisation<L>::type
				>::type,
			typename
				get_base_or_this_t
				<
					typename ::yggr::container_ex::map_regularisation<R>::type
				>::type
		>
{
};

// map_merger_sel_rule_default

/*
	map_merger_sel_rule_default 
	-> map_merger_sel_rule_regularisation
	-> map_merger_sel_rule_is_same_namespace
	-> map_merger_sel_rule_is_same_tpl
	-> map_merger_sel_rule_is_different_comp_only
	-> map_merger_sel_rule_has_merge
*/
template<typename L, typename R>	
struct map_merger_sel_rule_default
	: map_merger_sel_rule_regularisation<L, R>
{
};

// extern map_merger_sel_rule or sub_rule
//map_merger_sel_rule
template<typename L, typename R>	
struct map_merger_sel_rule
	: public map_merger_sel_rule_default<L, R>
{
};

// map_merger_sel
template<typename L, typename R>	
struct map_merger_sel
	: public map_merger_sel_rule<L, R>::type
{
};


} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_MAP_MERGER_SEL_HPP__
