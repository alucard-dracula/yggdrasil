//umap_merger_sel.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_UMAP_MERGER_SEL_HPP__
#define __YGGR_CONTAINER_DETAIL_UMAP_MERGER_SEL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/type_traits/is_same_tpl.hpp>

#include <yggr/mplex/null_t_to.hpp>
#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/typename_replaceor.hpp>

#include <yggr/container/detail/umap_merge_impl.hpp>
#include <yggr/container/detail/has_merge.hpp>
#include <yggr/container/detail/is_s_or_m_container.hpp>
#include <yggr/container/detail/get_base_or_this.hpp>

#include <yggr/container_ex/allocator_t.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

struct umap_merger_eei_handler
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
		umap_merge_umap_impl_eei(get_base_or_this(l), get_base_or_this(r));
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
		umap_merge_umultimap_impl_eei(get_base_or_this(l), get_base_or_this(r));
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
		umultimap_merge_impl_eei(get_base_or_this(l), get_base_or_this(r));
	}
};

struct umap_merger_eei
{
	typedef umap_merger_eei_handler type;
};

struct umap_merger_eti_handler
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
		umap_merge_umap_impl_eti(get_base_or_this(l), get_base_or_this(r));
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
		umap_merge_umultimap_impl_eti(get_base_or_this(l), get_base_or_this(r));
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
		umultimap_merge_impl_eti(get_base_or_this(l), get_base_or_this(r));
	}
};

struct umap_merger_eti
{
	typedef umap_merger_eti_handler type;
};

struct umap_merger_inner_handler
{
public:
	template<typename L, typename R> inline
	void operator()(L& l, R& r) const
	{
		get_base_or_this(l).merge(get_base_or_this(r));
	}
};

struct umap_merger_inner
{
	typedef umap_merger_inner_handler type;
};


// rule

template<typename L, typename R>
struct is_umap_different_hash_and_equal_only_base
	: public
		boost::mpl::and_
		<
			boost::is_same<typename L::value_type, typename R::value_type>,
			boost::is_same<typename L::allocator_type, typename R::allocator_type>
		>::type
{
};

template<typename L, typename R>
struct is_umap_different_hash_and_equal_only
	: public
		is_umap_different_hash_and_equal_only_base
		<
			typename get_base_or_this_t<L>::type,
			typename get_base_or_this_t<R>::type
		>::type
{
};


// umap_merger_sel_rule_has_merge
template<typename L, typename R, bool b_has_merge = has_any_merge<L>::value>
struct umap_merger_sel_rule_has_merge_detail
	: public umap_merger_eei
{
};

template<typename L, typename R>
struct umap_merger_sel_rule_has_merge_detail<L, R, true>
	: public umap_merger_inner
{
};

template<typename L, typename R>
struct umap_merger_sel_rule_has_merge
	: public umap_merger_sel_rule_has_merge_detail<L, R>
{
};

// umap_merger_sel_rule_is_different_hash_and_equal_only
template<typename L, typename R, bool b_is_different_hc_only = is_umap_different_hash_and_equal_only<L, R>::value>
struct umap_merger_sel_rule_is_different_hash_and_equal_only_detail
	: public umap_merger_eei
{
};

template<typename L, typename R>
struct umap_merger_sel_rule_is_different_hash_and_equal_only_detail<L, R, true>
	: public umap_merger_sel_rule_has_merge<L, R>
{
};

template<typename L, typename R>
struct umap_merger_sel_rule_is_different_hash_and_equal_only
	: public umap_merger_sel_rule_is_different_hash_and_equal_only_detail<L, R>
{
};

// umap_merger_sel_rule_is_same_namespace
template<typename L, typename R, bool b_is_same_namespace = is_same_tpl<L, R>::value>
struct umap_merger_sel_rule_is_same_namespace_detail
	: public umap_merger_eei
{
};

template<typename L, typename R>
struct umap_merger_sel_rule_is_same_namespace_detail<L, R, true>
	: public umap_merger_sel_rule_is_different_hash_and_equal_only<L, R>
{
};

template<typename L, typename R>
struct umap_merger_sel_rule_is_same_namespace
	: public umap_merger_sel_rule_is_same_namespace_detail<L, R>
{
};

template<typename L, typename R>
struct umap_merger_sel_rule_regularisation
	: public 
		umap_merger_sel_rule_is_same_namespace
		<
			typename get_base_or_this_t<L>::type,
			typename get_base_or_this_t<R>::type
		>
{
};

// umap_merger_sel_rule_default

/*
	umap_merger_sel_rule_default 
	-> umap_merger_sel_rule_regularisation
	-> umap_merger_sel_rule_is_same_namespace
	-> set_merger_sel_rule_is_same_tpl
	-> umap_merger_sel_rule_is_different_hash_and_equal_only
	-> umap_merger_sel_rule_has_merge
*/
template<typename L, typename R>	
struct umap_merger_sel_rule_default
	: public umap_merger_sel_rule_regularisation<L, R>
{
};

// extern umap_merger_sel_rule or sub_rule
//umap_merger_sel_rule
template<typename L, typename R>	
struct umap_merger_sel_rule
	: public umap_merger_sel_rule_default<L, R>
{
};

// umap_merger_sel
template<typename L, typename R>	
struct umap_merger_sel
	: public umap_merger_sel_rule<L, R>::type
{
};


} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_UMAP_MERGER_SEL_HPP__
