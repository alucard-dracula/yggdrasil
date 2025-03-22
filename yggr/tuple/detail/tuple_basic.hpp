//tuple_basic.hpp

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

#ifndef _YGGR_TUPLE_DETAIL_TUPLE_BASIC_HPP__
#define _YGGR_TUPLE_DETAIL_TUPLE_BASIC_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/tuple/detail/tuple_config.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
#	include <yggr/type_traits/integer_sequence.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/selection/max.hpp>
#include <boost/preprocessor/selection/min.hpp>

#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace tuples
{

// -- null_type --------------------------------------------------------
using boost::tuples::null_type;

// a helper function to provide a const null_type type temporary
namespace detail
{

inline const null_type cnull_type() { return null_type(); }
using boost::tuples::detail::cnull;

// -- if construct ------------------------------------------------
using boost::tuples::detail::IF;

} // end detail

// - cons forward declaration -----------------------------------------------
template<typename HT, typename TT>
struct cons;


// - tuple forward declaration -----------------------------------------------
#define YGGR_TMP_PP_T_PARAMS_TITLE() T
#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::tuples::null_type

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE(
			YGGR_TUPLE_LIMIT_LENGTH(),
			typename YGGR_TMP_PP_T_PARAMS_TITLE() )
	>
class tuple;

#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
#undef YGGR_TMP_PP_T_PARAMS_TITLE

using boost::tuples::length;

namespace detail
{

// -- generate error template, referencing to non-existing members of this
// template is used to produce compilation errors intentionally

using boost::tuples::detail::generate_error;
using boost::tuples::detail::drop_front;

} // namespace detail


// -cons type accessors ----------------------------------------
// typename tuples::element<N,T>::type gets the type of the
// Nth element ot T, first element is at index 0
// -------------------------------------------------------

using boost::tuples::element;

// -get function templates -----------------------------------------------

using boost::tuples::access_traits;


// -- the cons template  --------------------------------------------------
namespace detail
{

//  These helper templates wrap void types and plain function types.
//  The reationale is to allow one to write tuple types with those types
//  as elements, even though it is not possible to instantiate such object.
//  E.g: typedef tuple<void> some_type; // ok
//  but: some_type x; // fails

using boost::tuples::detail::non_storeable_type;
using boost::tuples::detail::wrap_non_storeable_type;

} // namespace detail
} // namespace tuples
} // namespace yggr


namespace boost
{
namespace tuples
{

// access_traits
template<int N, typename HT, typename TT>
typename
	access_traits
	<
		typename element<N, yggr::tuples::cons<HT, TT> >::type
	>::non_const_type
	get(yggr::tuples::cons<HT, TT>& c BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int, N));

template<int N, typename HT, typename TT>
typename
	access_traits
	<
		typename element<N, yggr::tuples::cons<HT, TT> >::type
	>::const_type
	get(const yggr::tuples::cons<HT, TT>& c BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int, N));

} // namespace tuples
} // namespace boost

namespace yggr
{
namespace tuples
{

template<typename Tuple>
struct tuple_head_t
{
	typedef typename Tuple::stored_head_type type;
};

template<typename Tuple>
struct tuple_tail_t
{
	typedef typename Tuple::tail_type type;
};

// get_head and get_tail
// get_head
template<typename HT, typename TT> inline
typename
	access_traits
	<
		typename detail::wrap_non_storeable_type
		<
			typename ::yggr::tuples::cons<HT, TT>::head_type
		>::type
	>::non_const_type
	get_head(::yggr::tuples::cons<HT, TT>& val)
{
	return val.head;
}

template<typename HT, typename TT> inline
typename
	access_traits
	<
		typename detail::wrap_non_storeable_type
		<
			typename ::yggr::tuples::cons<HT, TT>::head_type
		>::type
	>::const_type
	get_head(const ::yggr::tuples::cons<HT, TT>& val)
{
	return val.head;
}

template<typename HT, typename TT> inline
typename
	access_traits
	<
		typename detail::wrap_non_storeable_type
		<
			typename ::boost::tuples::cons<HT, TT>::head_type
		>::type
	>::non_const_type
	get_head(::boost::tuples::cons<HT, TT>& val)
{
	return val.head;
}

template<typename HT, typename TT> inline
typename
	access_traits
	<
		typename detail::wrap_non_storeable_type
		<
			typename ::boost::tuples::cons<HT, TT>::head_type
		>::type
	>::const_type
	get_head(const ::boost::tuples::cons<HT, TT>& val)
{
	return val.head;
}

template<typename F, typename S> inline
typename access_traits<F>::non_const_type
	get_head(::std::pair<F, S>& val)
{
	return val.first;
}

template<typename F, typename S> inline
typename access_traits<F>::const_type
	get_head(const ::std::pair<F, S>& val)
{
	return val.first;
}

// get_tail

template<typename HT, typename TT> inline
typename
	access_traits
	<
		typename ::yggr::tuples::cons<HT, TT>::tail_type
	>::non_const_type
	get_tail(::yggr::tuples::cons<HT, TT>& val)
{
	return val.tail;
}

template<typename HT, typename TT> inline
typename
	access_traits
	<
		typename ::yggr::tuples::cons<HT, TT>::tail_type
	>::const_type
	get_tail(const ::yggr::tuples::cons<HT, TT>& val)
{
	return val.tail;
}

template<typename HT, typename TT> inline
typename
	access_traits
	<
		typename ::boost::tuples::cons<HT, TT>::tail_type
	>::non_const_type
	get_tail(::boost::tuples::cons<HT, TT>& val)
{
	return val.tail;
}

template<typename HT, typename TT> inline
typename
	access_traits
	<
		typename ::yggr::tuples::cons<HT, TT>::tail_type
	>::const_type
	get_tail(const ::boost::tuples::cons<HT, TT>& val)
{
	return val.tail;
}

template<typename F, typename S> inline
typename access_traits<S>::non_const_type
	get_tail(::std::pair<F, S>& val)
{
	return val.second;
}

template<typename F, typename S> inline
typename access_traits<S>::const_type
	get_tail(const ::std::pair<F, S>& val)
{
	return val.second;
}

// get_inherited

template<typename T> inline
T& get_inherited(T& val)
{
	return val;
}

template<typename T> inline
const T& get_inherited(const T& val)
{
	return val;
}

template<YGGR_PP_FOO_TYPES_DEF(BOOST_TUPLE_LIMIT_LENGTH())> inline
typename ::boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>::inherited&
	get_inherited(::boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>& val)
{
	typedef typename
		::boost::tuples::tuple
		<
			YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())
		>::inherited inherited_type;
	return static_cast<inherited_type&>(val);
}

template<YGGR_PP_FOO_TYPES_DEF(BOOST_TUPLE_LIMIT_LENGTH())> inline
const typename ::boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>::inherited&
	get_inherited(const ::boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>& val)
{
	typedef typename
		::boost::tuples::tuple
		<
			YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())
		>::inherited inherited_type;
	return static_cast<const inherited_type&>(val);
}

template<YGGR_PP_FOO_TYPES_DEF(YGGR_TUPLE_LIMIT_LENGTH())> inline
typename ::yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>::inherited&
	get_inherited(::yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>& val)
{
	typedef typename
		::yggr::tuples::tuple
		<
			YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())
		>::inherited inherited_type;
	return static_cast<inherited_type&>(val);
}

template<YGGR_PP_FOO_TYPES_DEF(YGGR_TUPLE_LIMIT_LENGTH())> inline
const typename ::yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>::inherited&
	get_inherited(const ::yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>& val)
{
	typedef typename
		::yggr::tuples::tuple
		<
			YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())
		>::inherited inherited_type;
	return static_cast<const inherited_type&>(val);
}

// cons_conv_helper
// boost to yggr

template<typename Cons>
struct cons_conv_helper;

template<typename HT, typename TT>
struct cons_conv_helper< boost::tuples::cons<HT, TT> >
{
private:
	typedef boost::tuples::cons<HT, TT> src_type;
	typedef typename src_type::head_type src_head_type;
	typedef typename src_type::tail_type src_tail_type;

public:
	typedef
		yggr::tuples::cons
		<
			src_head_type,
			typename cons_conv_helper<src_tail_type>::type
		> type;

	typedef type result_type;
	typedef type dst_type;

	inline result_type operator()(const src_type& src) const
	{
		typedef cons_conv_helper<src_tail_type> tail_helper_type;
		tail_helper_type tail_helper;
		return result_type(src.head, tail_helper(src.tail));
	}
};

template<typename HT>
struct cons_conv_helper< boost::tuples::cons<HT, null_type> >
{
private:
	typedef boost::tuples::cons<HT, null_type> src_type;
	typedef typename src_type::head_type src_head_type;
	typedef typename src_type::tail_type src_tail_type;

public:
	typedef
		yggr::tuples::cons
		<
			src_head_type,
			typename cons_conv_helper<src_tail_type>::type
		> type;

	typedef type result_type;
	typedef type dst_type;

	inline result_type operator()(const src_type& src) const
	{
		return result_type(src.head, null_type());
	}
};

// cons_conv_helper
template<typename HT, typename TT>
struct cons_conv_helper< yggr::tuples::cons<HT, TT> >
{
private:
	typedef yggr::tuples::cons<HT, TT> src_type;
	typedef typename src_type::head_type src_head_type;
	typedef typename src_type::tail_type src_tail_type;

public:
	typedef
		boost::tuples::cons
		<
			src_head_type,
			typename cons_conv_helper<src_tail_type>::type
		> type;

	typedef type result_type;
	typedef type dst_type;

	inline result_type operator()(const src_type& src) const
	{
		typedef cons_conv_helper<src_tail_type> tail_helper_type;
		tail_helper_type tail_helper;
		return result_type(src.head, tail_helper(src.tail));
	}

};

template<typename HT>
struct cons_conv_helper< yggr::tuples::cons<HT, null_type> >
{
private:
	typedef yggr::tuples::cons<HT, null_type> src_type;
	typedef typename src_type::head_type src_head_type;
	typedef typename src_type::tail_type src_tail_type;

public:
	typedef
		boost::tuples::cons
		<
			src_head_type,
			typename cons_conv_helper<src_tail_type>::type
		> type;

	typedef type result_type;
	typedef type dst_type;

	inline result_type operator()(const src_type& src) const
	{
		return result_type(src.head, null_type());
	}
};

template<>
struct cons_conv_helper< null_type >
{

public:
	typedef null_type type;
	typedef type result_type;
	typedef type dst_type;

	inline result_type operator()(const type& src) const
	{
		return result_type();
	}
};

// cons_conv only_params
template<typename HT, typename TT,
			template<typename _HT, typename _TT> class Cons_Src> inline
typename cons_conv_helper< Cons_Src<HT, TT> >::type
	cons_conv(const Cons_Src<HT, TT>& src)
{
	typedef Cons_Src<HT, TT> src_cons_type;
	typedef cons_conv_helper< src_cons_type > helper_type;

	helper_type h;
	return h(src);
}

inline const null_type& cons_conv(const null_type& src)
{
	return src;
}

// cons_conv two params
namespace detail
{

inline null_type& cons_conv_impl(null_type& dst, const null_type& src)
{
	return dst;
}

template<typename HT, typename HT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
Cons_Dst<HT, null_type>&
	cons_conv_impl(Cons_Dst<HT, null_type>& dst,
					BOOST_RV_REF_BEG Cons_Src<HT2, null_type> BOOST_RV_REF_END src)
{
	typedef Cons_Src<HT2, null_type> src_cons_type;

	typedef typename src_cons_type::stored_head_type src_head_type;

	get_head(dst) = boost::move(get_head(src));
	return dst;
}

template<typename HT, typename HT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
Cons_Dst<HT, null_type>&
	cons_conv_impl(Cons_Dst<HT, null_type>& dst, const Cons_Src<HT2, null_type>& src)
{
	get_head(dst) = get_head(src);
	return dst;
}

template<typename HT, typename TT, typename HT2, typename TT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
Cons_Dst<HT, TT>&
	cons_conv_impl(Cons_Dst<HT, TT>& dst,
					BOOST_RV_REF_BEG Cons_Src<HT2, TT2> BOOST_RV_REF_END src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT2, TT2> src_cons_type;

	typedef typename src_cons_type::stored_head_type src_head_type;
	typedef typename src_cons_type::tail_type src_tail_type;

	BOOST_STATIC_ASSERT((length<dst_cons_type>::value == length<src_cons_type>::value));

	get_head(dst) = boost::move(get_head(src));
	cons_conv_impl(get_tail(dst), boost::move(get_tail(src)));

	return dst;
}

template<typename HT, typename TT, typename HT2, typename TT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
Cons_Dst<HT, TT>&
	cons_conv_impl(Cons_Dst<HT, TT>& dst, const Cons_Src<HT2, TT2>& src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT2, TT2> src_cons_type;

	BOOST_STATIC_ASSERT((length<dst_cons_type>::value == length<src_cons_type>::value));
	get_head(dst) = get_head(src);
	cons_conv_impl(get_tail(dst), get_tail(src));
	return dst;
}

} // namespace detail

template<typename HT, typename TT, typename HT2, typename TT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
Cons_Dst<HT, TT>&
	cons_conv(Cons_Dst<HT, TT>& dst,
				BOOST_RV_REF_BEG Cons_Src<HT2, TT2> BOOST_RV_REF_END src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT, TT> src_cons_type;

	src_cons_type& src_ref = src;

	if(&dst == reinterpret_cast<dst_cons_type*>(&src_ref))
	{
		return dst;
	}

	return detail::cons_conv_impl(dst, src);
}

template<typename HT, typename TT, typename HT2, typename TT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
Cons_Dst<HT, TT>&
	cons_conv(Cons_Dst<HT, TT>& dst, const Cons_Src<HT2, TT2>& src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT, TT> src_cons_type;

	if(&dst == reinterpret_cast<const dst_cons_type*>(&src))
	{
		return dst;
	}

	return detail::cons_conv_impl(dst, src);
}

template<typename HT, typename TT, typename F, typename S,
			template<typename _HT, typename _TT> class Cons_Dst> inline
Cons_Dst<HT, TT>&
	cons_conv(Cons_Dst<HT, TT>& dst,
				BOOST_RV_REF_BEG ::std::pair<F, S> BOOST_RV_REF_END src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef ::std::pair<F, S> src_pair_type;

	BOOST_STATIC_ASSERT((length<dst_cons_type>::value == 2));

	src_pair_type& src_ref = src;
	if(&dst == reinterpret_cast<dst_cons_type*>(&src_ref))
	{
		return dst;
	}

	get_head(dst) = boost::move(get_head(src_ref));
	get_head(get_tail(dst)) = boost::move(get_tail(src_ref));

	return dst;
}

template<typename HT, typename TT, typename F, typename S,
			template<typename _HT, typename _TT> class Cons_Dst> inline
Cons_Dst<HT, TT>&
	cons_conv(Cons_Dst<HT, TT>& dst, const ::std::pair<F, S>& src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;

	BOOST_STATIC_ASSERT((length<dst_cons_type>::value == 2));

	if(&dst == reinterpret_cast<const dst_cons_type*>(&src))
	{
		return dst;
	}

	get_head(dst) = get_head(src);
	get_head(get_tail(dst)) = get_tail(src);
	return dst;
}

template<typename HT, typename F,
			template<typename _HT, typename _TT> class Cons_Dst> inline
Cons_Dst<HT, null_type>&
	cons_conv(Cons_Dst<HT, null_type>& dst,
				BOOST_RV_REF_BEG ::std::pair<F, null_type> BOOST_RV_REF_END src)
{
    typedef Cons_Dst<HT, null_type> dst_cons_type;
	typedef ::std::pair<F, null_type> src_pair_type;

	src_pair_type& src_ref = src;
	if(&dst == reinterpret_cast<dst_cons_type*>(&src_ref))
	{
		return dst;
	}

	get_head(dst) = boost::move(get_head(src_ref));

	return dst;
}

template<typename HT, typename F,
			template<typename _HT, typename _TT> class Cons_Dst> inline
Cons_Dst<HT, null_type>&
	cons_conv(Cons_Dst<HT, null_type>& dst, const ::std::pair<F, null_type>& src)
{
	typedef Cons_Dst<HT, null_type> dst_cons_type;

	if(&dst == reinterpret_cast<const dst_cons_type*>(&src))
	{
		return dst;
	}

	get_head(dst) = get_head(src);
	return dst;
}

template<typename F, typename S, typename HT, typename TT,
			template<typename _HT, typename _TT> class Cons_Src> inline
::std::pair<F, S>&
	cons_conv(::std::pair<F, S>& dst,
				BOOST_RV_REF_BEG Cons_Src<HT, TT> BOOST_RV_REF_END src)
{
	typedef Cons_Src<HT, TT> src_cons_type;
	typedef typename src_cons_type::stored_head_type src_head_type;
	typedef typename src_cons_type::tail_type::stored_head_type src_tail_head_type;


	BOOST_STATIC_ASSERT((length<src_cons_type>::value == 2));

	src_cons_type src_ref = src;

	if(&src_ref == reinterpret_cast<src_cons_type*>(&dst))
	{
		return dst;
	}

	get_head(dst) = boost::move(get_head(src_ref));
	get_tail(dst) = boost::move(get_head(get_tail(src_ref)));

	return dst;
}

template<typename F, typename S, typename HT, typename TT,
			template<typename _HT, typename _TT> class Cons_Src> inline
::std::pair<F, S>&
	cons_conv(::std::pair<F, S>& dst, const Cons_Src<HT, TT>& src)
{
	typedef Cons_Src<HT, TT> src_cons_type;

	BOOST_STATIC_ASSERT((length<src_cons_type>::value == 2));

	if(&src == reinterpret_cast<src_cons_type*>(&dst))
	{
		return dst;
	}

	get_head(dst) = get_head(src);
	get_tail(dst) = get_head(get_tail(src));
	return dst;
}

template<typename F, typename HT,
			template<typename _HT, typename _TT> class Cons_Src> inline
::std::pair<F, null_type>&
	cons_conv(::std::pair<F, null_type>& dst,
				BOOST_RV_REF_BEG Cons_Src<HT, null_type> BOOST_RV_REF_END src)
{
	typedef Cons_Src<HT, null_type> src_cons_type;
	typedef typename src_cons_type::stored_head_type src_head_type;

	src_cons_type src_ref = src;

	if(&src_ref == reinterpret_cast<src_cons_type*>(&dst))
	{
		return dst;
	}

	get_head(dst) = boost::move(get_head(src));
	return dst;
}

template<typename F, typename HT,
			template<typename _HT, typename _TT> class Cons_Src> inline
::std::pair<F, null_type>&
	cons_conv(::std::pair<F, null_type>& dst, const Cons_Src<HT, null_type>& src)
{
	typedef Cons_Src<HT, null_type> src_cons_type;

	if(&src == reinterpret_cast<src_cons_type*>(&dst))
	{
		return dst;
	}

	get_head(dst) = get_head(src);
	return dst;
}

inline null_type& cons_conv(null_type& dst, const null_type& src)
{
	return dst;
}

// cons_swap
namespace detail
{

inline void cons_swap_impl(null_type& dst, null_type& src)
{
	return;
}

template<typename HT, typename HT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
void cons_swap_impl(Cons_Dst<HT, null_type>& dst, Cons_Src<HT2, null_type>& src)
{
	yggr::swap(get_head(dst), get_head(src));
}

template<typename HT, typename TT, typename HT2, typename TT2,
		template<typename _HT, typename _TT> class Cons_Dst,
		template<typename _HT, typename _TT> class Cons_Src> inline
void cons_swap_impl(Cons_Dst<HT, TT>& dst, Cons_Src<HT2, TT2>& src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT, TT> src_cons_type;

	BOOST_STATIC_ASSERT((length<dst_cons_type>::value == length<src_cons_type>::value));
	yggr::swap(get_head(dst), get_head(src));
	cons_swap_impl(get_tail(dst), get_tail(src));
}

} // namespace detail

template<typename HT, typename TT, typename HT2, typename TT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
void cons_swap(Cons_Dst<HT, TT>& dst, Cons_Src<HT2, TT2>& src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT, TT> src_cons_type;

	if(&dst == reinterpret_cast<dst_cons_type*>(&src))
	{
		return;
	}

	detail::cons_swap_impl(src, dst);
}


inline void cons_swap(null_type& dst, null_type& src)
{
	return;
}

// struct cons
template<typename HT, typename TT>
struct cons
{
private:
	typedef cons this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef HT head_type;
	typedef TT tail_type;

	typedef typename
		detail::wrap_non_storeable_type<head_type>::type stored_head_type;

public:
	stored_head_type head;
	tail_type tail;

public:
	inline typename access_traits<stored_head_type>::non_const_type
		get_head(void)
	{
		return head;
	}

	inline typename access_traits<tail_type>::non_const_type
		get_tail(void)
	{
		return tail;
	}

	inline typename access_traits<stored_head_type>::const_type
		get_head(void) const
	{
		return head;
	}

	inline typename access_traits<tail_type>::const_type
		get_tail(void) const
	{
		return tail;
	}

public:
	cons(void)
		: head(), tail()
	{
	}

	// the argument for head is not strictly needed, but it prevents
	// array type elements. This is good, since array type elements
	// cannot be supported properly in any case (no assignment,
	// copy works only if the tails are exactly the same type, ...)

	cons(typename access_traits<stored_head_type>::parameter_type h,
			const tail_type& t)
		: head (h), tail(t)
	{
	}

#	define YGGR_PP_FOO_ARG_NAME() t
#	define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) BOOST_PP_CAT(YGGR_PP_OPEN(YGGR_PP_FOO_ARG_NAME), BOOST_PP_ADD(__n__, 1))

	template< YGGR_PP_FOO_TYPES_DEF( YGGR_TUPLE_LIMIT_LENGTH() ) >
	cons( YGGR_PP_FOO_PARAMS_DEF(YGGR_TUPLE_LIMIT_LENGTH(), YGGR_PP_FOO_REF_PARAMS) )
		:head(t0),
			tail( YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), 1),
														YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER),
					detail::cnull())
	{
	}

#	undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
#	undef YGGR_PP_FOO_ARG_NAME

#	define YGGR_PP_FOO_ARG_NAME() t

	template<YGGR_PP_FOO_TYPES_DEF( BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), 1) ) >
	cons( const null_type&,
			YGGR_PP_FOO_PARAMS_DEF(BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), 1),
									YGGR_PP_FOO_REF_PARAMS) )
		: head(),
			tail( YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), 1), YGGR_PP_OPEN(YGGR_PP_FOO_ARG_NAME)),
					detail::cnull())
	{
	}

#	undef YGGR_PP_FOO_ARG_NAME

	template<typename HT2, typename TT2>
	cons( BOOST_RV_REF_BEG ::boost::tuples::cons<HT2, TT2> BOOST_RV_REF_END u )
		: head(boost::move(u.head)),
			tail(cons_conv(u.tail))
	{
	}

	template<typename HT2, typename TT2>
	cons( const ::boost::tuples::cons<HT2, TT2>& u )
		: head(u.head),
			tail(cons_conv(u.tail))
	{
	}

	template<typename HT2, typename TT2>
	cons( BOOST_RV_REF_BEG cons<HT2, TT2> BOOST_RV_REF_END u )
		: head(boost::move(u.head)),
			tail(boost::move(u.tail))
	{
	}

	template<typename HT2, typename TT2>
	cons( const cons<HT2, TT2>& u )
		: head(u.head), tail(u.tail)
	{
	}

#if defined(_MSC_VER)
	cons( BOOST_RV_REF(this_type) u )
		: head(boost::move(u.head)), // !!! if head is Foo_obj&, gcc not suppord it !!!
			tail(boost::move(u.tail))
	{
	}
#else
    cons( BOOST_RV_REF(this_type) u )
		: head(boost::forward<head_type>(u.head)),
			tail(boost::forward<tail_type>(u.tail))
	{
	}
#endif // _MSC_VER

	cons(const this_type& u)
		: head(u.head),
			tail(u.tail)
	{
	}

public:
	inline operator typename cons_conv_helper<this_type>::type(void) const
	{
		return cons_conv(*this);
	}

public:
	template<typename HT2, typename TT2> inline
	this_type& operator=( BOOST_RV_REF_BEG cons<HT2, TT2> BOOST_RV_REF_END u )
	{
		typedef cons<HT2, TT2> right_type;
		right_type& right_chk = u;
		if(this == reinterpret_cast<this_type*>(&right_chk))
		{
			return *this;
		}

		head = boost::move(u.head);
		tail = boost::move(u.tail);

		return *this;
	}

	template<typename HT2, typename TT2>
	this_type& operator=( const cons<HT2, TT2>& u )
	{
		if(this == reinterpret_cast<const this_type*>(&u))
		{
			return *this;
		}

		head = u.head;
		tail = u.tail;
		return *this;
	}

	template<typename HT2, typename TT2> inline
	this_type& operator=( BOOST_RV_REF_BEG ::boost::tuples::cons<HT2, TT2> BOOST_RV_REF_END u )
	{
		cons_conv(*this, u);
		return *this;
	}

	template<typename HT2, typename TT2>
	this_type& operator=( const ::boost::tuples::cons<HT2, TT2>& u )
	{
		cons_conv(*this, u);
		return *this;
	}

	template<typename T1, typename T2> inline
	this_type& operator=( BOOST_RV_REF_BEG std::pair<T1, T2> BOOST_RV_REF_END u )
	{
		cons_conv(*this, u);
		return *this;
	}

	template<typename T1, typename T2>
	this_type& operator=( const std::pair<T1, T2>& u )
	{
		cons_conv(*this, u);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) u)
	{
		this_type& right_chk = u;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(head, boost::move(u.head));
		copy_or_move_or_swap(tail, boost::move(u.tail));

		return *this;
	}

	this_type& operator=(const this_type& u)
	{
		if(this == &u)
		{
			return *this;
		}

		head = u.head;
		tail = u.tail;
		return *this;
	}

public:
	template<typename HT2, typename TT2> inline
	void swap(BOOST_RV_REF_BEG ::boost::tuples::cons<HT2, TT2> BOOST_RV_REF_END u)
	{
		typedef ::boost::tuples::cons<HT2, TT2> right_type;
		right_type& right_ref = u;
		cons_swap(*this, right_ref);
	}

	template<typename HT2, typename TT2>
	void swap(::boost::tuples::cons<HT2, TT2>& u)
	{
		cons_swap(*this, u);
	}

	template<typename HT2, typename TT2>
	inline void swap(BOOST_RV_REF_BEG cons<HT2, TT2> BOOST_RV_REF_END u)
	{
		typedef cons<HT2, TT2> right_type;
		right_type& right_ref = u;
		cons_swap(*this, right_ref);
	}

	template<typename HT2, typename TT2>
	void swap(cons<HT2, TT2>& u)
	{
		cons_swap(*this, u);
	}

	inline void swap(BOOST_RV_REF(this_type) u)
	{
		this_type& right_ref = u;
		this_type::swap(right_ref);
	}

	void swap(this_type& u)
	{
		cons_swap(*this, u);
	}

public:
	// get member functions (non-const and const)
	template<int N> inline
	typename
		access_traits
		<
			typename element<N, cons<HT, TT> >::type
		>::non_const_type
		get(void)
	{
		return boost::tuples::get<N>(*this); // delegate to non-member get
	}

	template<int N> inline
	typename
		access_traits
		<
			typename element<N, cons<HT, TT> >::type
		>::const_type
		get(void) const
	{
		return boost::tuples::get<N>(*this); // delegate to non-member get
	}
};

template<typename HT>
struct cons<HT, null_type>
{
private:
	typedef cons this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef HT head_type;
	typedef null_type tail_type;
	typedef cons<HT, null_type> self_type;

	typedef typename
		detail::wrap_non_storeable_type<head_type>::type stored_head_type;

public:
	stored_head_type head;

public:
	inline typename access_traits<stored_head_type>::non_const_type
		get_head(void)
	{
		return head;
	}

	inline null_type get_tail(void)
	{
		return null_type();
	}

	inline typename access_traits<stored_head_type>::const_type
		get_head(void) const
	{
		return head;
	}

	inline const null_type get_tail(void) const
	{
		return null_type();
	}

public:
	cons(void)
		: head()
	{
	}

	cons(typename access_traits<stored_head_type>::parameter_type h,
			const null_type& = null_type())
		: head(h)
	{
	}

#	define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) const null_type&
	template<typename T>
	cons(T& t,
			YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), 1),
													YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER) )
		: head(t)
	{
	}
#	undef YGGR_PP_TEMPLATE_CUSTOM_TYPE

#	define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) const null_type&
	cons( YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(YGGR_TUPLE_LIMIT_LENGTH(),
													YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER) )
		: head()
	{
	}
#	undef YGGR_PP_TEMPLATE_CUSTOM_TYPE

	template<typename HT2>
	cons( BOOST_RV_REF_BEG boost::tuples::cons<HT2, null_type> BOOST_RV_REF_END u )
		: head(boost::move(u.head))
	{
	}

	template<typename HT2>
	cons( const boost::tuples::cons<HT2, null_type>& u )
		: head(u.head)
	{
	}

	template<typename HT2>
	cons( BOOST_RV_REF_BEG cons<HT2, null_type> BOOST_RV_REF_END u )
		: head(boost::move(u.head))
	{
	}

	template<typename HT2>
	cons( const cons<HT2, null_type>& u )
		: head(u.head)
	{
	}

#if defined(_MSC_VER)
	cons(BOOST_RV_REF(this_type) u)
		: head(boost::move(u.head))
	{
	}
#else
    cons(BOOST_RV_REF(this_type) u)
		: head(boost::forward<head_type>(u.head))
	{
	}
#endif // _MSC_VER

	cons(const this_type& u)
		: head(u.head)
	{
	}

public:
	inline operator typename cons_conv_helper<this_type>::type(void) const
	{
		return cons_conv(*this);
	}

public:
	template<typename HT2> inline
	this_type& operator=(BOOST_RV_REF_BEG cons<HT2, null_type> BOOST_RV_REF_END u )
	{
		typedef cons<HT2, null_type> right_type;
		right_type& right_chk = u;
		if(this == reinterpret_cast<this_type*>(&right_chk))
		{
			return *this;
		}

		head = boost::move(u.head);
		return *this;
	}

	template<typename HT2>
	this_type& operator=(const cons<HT2, null_type>& u )
	{
		if(this == reinterpret_cast<const this_type*>(&u))
		{
			return *this;
		}

		head = u.head;
		return *this;
	}

	template<typename HT2> inline
	this_type& operator=(BOOST_RV_REF_BEG ::boost::tuples::cons<HT2, null_type> BOOST_RV_REF_END u )
	{
		cons_conv(*this, u);
		return *this;
	}

	template<typename HT2>
	this_type& operator=(const ::boost::tuples::cons<HT2, null_type>& u )
	{
		cons_conv(*this, u);
		return *this;
	}

	template<typename T1> inline
	this_type& operator=( BOOST_RV_REF_BEG std::pair<T1, null_type> BOOST_RV_REF_END u )
	{
		cons_conv(*this, u);
		return *this;
	}

	template<typename T1>
	this_type& operator=(const std::pair<T1, null_type>& u)
	{
		cons_conv(*this, u);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) u)
	{
		this_type& right_chk = u;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(head, boost::move(u.head));

		return *this;
	}

	this_type& operator=(const this_type& u)
	{
		if(this == &u)
		{
			return *this;
		}

		head = u.head;
		return *this;
	}

public:
	template<typename HT2> inline
	void swap(BOOST_RV_REF_BEG ::boost::tuples::cons<HT2, null_type> BOOST_RV_REF_END u)
	{
		typedef ::boost::tuples::cons<HT2, null_type> right_type;
		right_type& right_ref = u;
		cons_swap(*this, right_ref);
	}

	template<typename HT2>
	void swap(::boost::tuples::cons<HT2, null_type>& u)
	{
		cons_swap(*this, u);
	}

	template<typename HT2> inline
	void swap(BOOST_RV_REF_BEG cons<HT2, null_type> BOOST_RV_REF_END u)
	{
		typedef cons<HT2, null_type> right_type;
		right_type& right_ref = u;
		cons_swap(*this, right_ref);
	}

	template<typename HT2>
	void swap(cons<HT2, null_type>& u)
	{
		cons_swap(*this, u);
	}

	inline void swap(BOOST_RV_REF(this_type) u)
	{
		this_type& right_ref = u;
		this_type::swap(u);
	}

	void swap(this_type& u)
	{
		cons_swap(*this, u);
	}

public:
	template<int N> inline
	typename
		access_traits
		<
			typename element<N, self_type>::type
		>::non_const_type
		get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int, N))
	{
		return boost::tuples::get<N>(*this);
		//return boost::tuples::get<N>(*this);
		//return yggr::tuples::detail::get_class<N>::get(*this);
	}

	template<int N> inline
	typename
		access_traits
		<
			typename element<N, self_type>::type
		>::const_type
		get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int, N)) const
	{
		return boost::tuples::get<N>(*this);
		//return boost::tuples::get<N>(*this);
		//return yggr::tuples::detail::get_class<N>::get(*this);
	}
};

// templates for finding out the length of the tuple -------------------
// using boost::tuples::length and extended in the file last

namespace detail
{

// Tuple to cons mapper --------------------------------------------------

#define YGGR_PARAMS_NAME() T
#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) BOOST_PP_CAT(YGGR_PARAMS_NAME(), BOOST_PP_ADD( __n__, 1 ))

template<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename YGGR_PARAMS_NAME() )>
struct map_tuple_to_cons
{
	typedef
		cons // the yggr::tuples::cons
		<
			T0,
			typename map_tuple_to_cons
			<
				YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(
					BOOST_PP_SUB( YGGR_TUPLE_LIMIT_LENGTH(), 1 ),
					YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER),
				null_type
			>::type
		> type;
};

#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
#undef YGGR_PARAMS_NAME

template<>
struct map_tuple_to_cons
#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) yggr::tuples::null_type
		<
			YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(YGGR_TUPLE_LIMIT_LENGTH(),
													YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER)
		>
#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
{
	typedef null_type type;
};

} // namespace detail

namespace detail
{

template<int size>
struct boost_tie_converter;

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<int size>
struct boost_tie_converter
{
private:
	typedef boost_tie_converter this_type;

private:
	template<typename Tie_Tuple, typename Tuple, std::size_t ...I> inline
	Tie_Tuple prv_conv_detail(Tuple& val, ::yggr::index_sequence<I...>) const
	{
		return boost::tuples::tie(boost::tuples::get<I>(val)...);
	}

public:
	template<typename Tie_Tuple, typename Tuple> inline
	Tie_Tuple conv(Tuple& val) const
	{
		typedef Tie_Tuple tie_tuple_type;

		return
			this_type::prv_conv_detail<tie_tuple_type>(
				val, yggr::make_index_sequence<static_cast<std::size_t>(size)>{});
	}
};

#else

#	define TIE_CONV_PARAM_NAME() val

#	define YGGR_TMP_PP_TUPLE_ARGS_REF_GET( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) boost::tuples::get< __n__ >(TIE_CONV_PARAM_NAME())

#	define YGGR_TIE_CONV( __n__ ) \
		template< > \
		struct boost_tie_converter< __n__ > { \
			template<typename Tie_Tuple, typename Tuple> inline \
			Tie_Tuple conv(Tuple& TIE_CONV_PARAM_NAME()) const { \
				return boost::tuples::tie \
					YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES_L) \
						YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
							__n__, YGGR_TMP_PP_TUPLE_ARGS_REF_GET, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES_R); } };

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TIE_CONV( __n__ )

#	define BOOST_PP_LOCAL_LIMITS ( 1, BOOST_TUPLE_LIMIT_LENGTH() )
#	include BOOST_PP_LOCAL_ITERATE(  )

#	undef YGGR_TMP_PP_TUPLE_ARGS_REF_GET
#	undef YGGR_TIE_CONV

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace detail

// -------------------------------------------------------------------
// -- tuple ------------------------------------------------------

#define YGGR_TMP_PP_T_PARAMS_TITLE() T

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(
			YGGR_TUPLE_LIMIT_LENGTH(),
			typename YGGR_TMP_PP_T_PARAMS_TITLE() )
	>
class tuple
	: public
		detail::map_tuple_to_cons
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(
				YGGR_TUPLE_LIMIT_LENGTH(),
				YGGR_TMP_PP_T_PARAMS_TITLE())
		>::type
{
public:
	typedef typename
		detail::map_tuple_to_cons
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(),
											YGGR_TMP_PP_T_PARAMS_TITLE())
		>::type inherited;

	typedef typename inherited::head_type head_type;
	typedef typename inherited::tail_type tail_type;

private:
	typedef tuple this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
// access_traits<T>::parameter_type takes non-reference types as const T&
	tuple(void)
	{
	}

// self def _YGGR_TMP_PP_TUPLE_SUB_PARAMS_MAKE(__n__, __val__)
#define YGGR_TMP_PP_TUPLE_PARAMS_DEF( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						_YGGR_TMP_PP_TUPLE_SUB_PARAMS_MAKE( __n__ )

#define YGGR_TUPLE_CONSTRUCT( __n__ ) \
	tuple( YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ) \
		: inherited(YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(__n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER) \
						YGGR_PP_SYMBOL_IF( BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), __n__), YGGR_PP_SYMBOL_COMMA) \
						BOOST_PP_IF(BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), __n__), \
										YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES, YGGR_PP_SYMBOL_EMPTY_N) \
											( BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), __n__), \
												YGGR_PP_TEMPLATE_PARAMS_CNULL_MAKER ) ) {}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	YGGR_TUPLE_CONSTRUCT( __n__ )

#define YGGR_TMP_PP_V_PARAMS_TITLE() t

#define _YGGR_TMP_PP_TUPLE_SUB_PARAMS_MAKE( __n__ ) \
	typename access_traits< BOOST_PP_CAT(YGGR_TMP_PP_T_PARAMS_TITLE(), __n__) >:: \
				parameter_type BOOST_PP_CAT(YGGR_TMP_PP_V_PARAMS_TITLE(), __n__)

#define YGGR_PP_FOO_CUSTOM_OP() YGGR_TMP_PP_TUPLE_PARAMS_DEF

#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) BOOST_PP_CAT(YGGR_TMP_PP_V_PARAMS_TITLE(), __n__)

#define YGGR_PP_TEMPLATE_PARAMS_CNULL_MAKER( __z__, __n__, _ ) \
			YGGR_PP_TEMPLATE_CNULL( __n__ )

#define YGGR_PP_TEMPLATE_CNULL( __n__ ) detail::cnull()

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_TUPLE_LIMIT_LENGTH() )
#include BOOST_PP_LOCAL_ITERATE(  )

#undef YGGR_PP_TEMPLATE_CNULL

#undef _YGGR_TMP_PP_TUPLE_SUB_PARAMS_MAKE
#undef YGGR_PP_TEMPLATE_PARAMS_CNULL_MAKER

#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
#undef YGGR_PP_FOO_CUSTOM_OP
#undef YGGR_TMP_PP_V_PARAMS_TITLE

#undef YGGR_TUPLE_CONSTRUCT
#undef YGGR_TMP_PP_TUPLE_PARAMS_DEF

	template<typename U1, typename U2>
	tuple(BOOST_RV_REF_BEG cons<U1, U2> BOOST_RV_REF_END p)
		: inherited(boost::move(p))
	{
	}

	template<typename U1, typename U2>
	tuple(const cons<U1, U2>& p)
		: inherited(p)
	{
	}

	template<typename U1, typename U2>
	tuple(BOOST_RV_REF_BEG boost::tuples::cons<U1, U2> BOOST_RV_REF_END p)
		: inherited(cons_conv(p))
	{
	}

	template<typename U1, typename U2>
	tuple(const boost::tuples::cons<U1, U2>& p)
		: inherited(cons_conv(p))
	{
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)>
	tuple(BOOST_RV_REF(::yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>) p)
		: inherited(get_inherited(p))
	{
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)>
	tuple(const ::yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>& p)
		: inherited(get_inherited(p))
	{
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)>
	tuple(BOOST_RV_REF(boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>) p)
		: inherited(cons_conv(get_inherited(p)))
	{
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)>
	tuple(const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>& p)
		: inherited(cons_conv(get_inherited(p)))
	{
	}

	tuple(BOOST_RV_REF(this_type) right)
		: inherited(boost::move(static_cast<inherited&>(right)))
	{
	}

	tuple(const this_type& right)
		: inherited(right)
	{
	}

	using inherited::operator typename cons_conv_helper<inherited>::type;

	inline operator
		boost::tuples::tuple
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(
				BOOST_PP_MIN(BOOST_TUPLE_LIMIT_LENGTH(), YGGR_TUPLE_LIMIT_LENGTH() ),
				YGGR_TMP_PP_T_PARAMS_TITLE() )
		>(void) const
	{
		typedef
			boost::tuples::tuple
			<
				YGGR_PP_TEMPLATE_PARAMS_TYPES(
					BOOST_PP_MIN(BOOST_TUPLE_LIMIT_LENGTH(), YGGR_TUPLE_LIMIT_LENGTH() ),
					YGGR_TMP_PP_T_PARAMS_TITLE() )
			> rst_type;

		const inherited& base = *this;
		return rst_type(cons_conv(base));
	}

public:
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)> inline
	operator
		boost::tuples::tuple
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), BT)
		>(void) const
	{
		typedef
			boost::tuples::tuple
			<
				YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), BT)
			> rst_type;

		BOOST_STATIC_ASSERT((boost::tuples::length<rst_type>::value));
		typedef detail::boost_tie_converter<boost::tuples::length<rst_type>::value> converter_type;

		this_type& ref = const_cast<this_type&>(*this);

		converter_type converter;
		return converter.template conv<rst_type>(ref);
	}

	template<typename U1, typename U2> inline
	this_type& operator=(BOOST_RV_REF_BEG std::pair<U1, U2> BOOST_RV_REF_END k)
	{
		inherited& base = *this;
		cons_conv(base, k);

		return *this;
	}

	template<typename U1, typename U2>
	this_type& operator=(const std::pair<U1, U2>& k)
	{
		inherited& base = *this;
		cons_conv(base, k);
		return *this;
	}

	template<typename U1, typename U2> inline
	this_type& operator=( BOOST_RV_REF_BEG ::boost::tuples::cons<U1, U2> BOOST_RV_REF_END k)
	{
		inherited& base = *this;
		cons_conv(base, k);
		return *this;
	}

	template<typename U1, typename U2>
	this_type& operator=(const ::boost::tuples::cons<U1, U2>& k)
	{
		inherited& base = *this;
		cons_conv(base, k);
		return *this;
	}

	template<typename U1, typename U2> inline
	this_type& operator=(BOOST_RV_REF_BEG cons<U1, U2> BOOST_RV_REF_END k)
	{
		inherited::operator=(boost::move(k));
		return *this;
	}

	template<typename U1, typename U2>
	this_type& operator=(const cons<U1, U2>& k)
	{
		inherited::operator=(k);
		return *this;
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)> inline
	this_type& operator=(BOOST_RV_REF(boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>) p)
	{
		typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)> other_type;
		typedef typename other_type::inherited other_inherited;

		other_type& other_ref = p;
		inherited& base = *this;
		other_inherited& obase = p;

		cons_conv(base, boost::move(obase));
		return *this;
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)>
	this_type& operator=(const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>& p)
	{
		typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)> other_type;
		typedef typename other_type::inherited other_inherited;

		inherited& base = *this;
		const other_inherited& obase = p;

		cons_conv(base, obase);
		return *this;
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)> inline
	this_type& operator=(BOOST_RV_REF(tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>) p)
	{
		typedef tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)> other_type;
		typedef typename other_type::inherited other_inherited;

		inherited& base = *this;
		other_inherited& obase = p;

		base = boost::move(obase);
		return *this;
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)>
	this_type& operator=(const tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>& p)
	{
		typedef tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)> other_type;
		typedef typename other_type::inherited other_inherited;

		inherited& base = *this;
		const other_inherited& obase = p;

		base = obase;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		inherited::operator=(boost::move(right_ref));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		inherited::operator=(right);
		return *this;
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)> inline
	void swap(BOOST_RV_REF(
				::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>) p)
	{
		typedef ::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)> other_type;
		typedef typename other_type::inherited other_inherited;

		inherited& base = *this;
		other_type& other = p;
		other_inherited& obase = other;
		cons_swap(base, obase);
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)>
	void swap(::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>& p)
	{
		typedef ::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)> other_type;
		typedef typename other_type::inherited other_inherited;

		inherited& base = *this;
		other_inherited& obase = p;
		cons_swap(base, obase);
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename YT)> inline
	void swap(BOOST_RV_REF(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), YT)>) p)
	{
		typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), YT)> other_type;
		typedef typename other_type::inherited other_inherited;

		inherited& base = *this;
		other_type& other = p;
		other_inherited& obase = other;
		cons_swap(base, obase);
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename YT)>
	void swap(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), YT)>& p)
	{
		typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), YT)> other_type;
		typedef typename other_type::inherited other_inherited;

		inherited& base = *this;
		other_inherited& obase = p;
		cons_swap(base, obase);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		inherited::swap(right);
	}
};

#undef YGGR_TMP_PP_T_PARAMS_TITLE

// The empty tuple
template<>
class tuple
#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) yggr::tuples::null_type
	<
		YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(
			YGGR_TUPLE_LIMIT_LENGTH(),
			YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER)
	>
#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
	: public null_type
{
public:
	typedef null_type inherited;

private:
	typedef tuple this_type;

public:

	tuple(void)
	{
	}

	template<typename T>
	tuple(const T&)
	{
	}

	tuple(const this_type&)
	{
	}

	~tuple(void)
	{
	}

public:
	inline operator boost::tuples::tuple<>(void) const
	{
		return boost::make_tuple();
	}

public:
	template<typename T> inline
	this_type& operator=(const T&)
	{
		return *this;
	}

public:
	template<typename T> inline
	void swap(T&)
	{
	}

	inline void swap(this_type&)
	{
	}
};

namespace detail
{

using boost::tuples::detail::swallow_assign;
using boost::tuples::detail::ignore_t;

} // namespace detail

// "ignore" allows tuple positions to be ignored when using "tie".
using boost::tuples::ignore;

// ---------------------------------------------------------------------------
// make_tuple_traits
using boost::tuples::make_tuple_traits;

namespace detail
{

// a helper traits to make the make_tuple functions shorter (Vesa Karvonen's
// suggestion)

#define YGGR_TMP_PP_T_PARAMS_TITLE() T
#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::tuples::null_type

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE(
			YGGR_TUPLE_LIMIT_LENGTH(), typename YGGR_TMP_PP_T_PARAMS_TITLE() )
	>

#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct make_tuple_mapper
{
#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) \
	typename make_tuple_traits< BOOST_PP_CAT(YGGR_TMP_PP_T_PARAMS_TITLE(), __n__) >::type

	typedef
		tuple
		<
			YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(
				YGGR_TUPLE_LIMIT_LENGTH(),
				YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER )
		> type;

#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
};

#undef YGGR_TMP_PP_T_PARAMS_TITLE

} // end detail

// -make_tuple function templates -----------------------------------
inline tuple<> make_tuple(void)
{
	return tuple<>();
}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename detail::make_tuple_mapper<YGGR_PP_FOO_TYPES_GROUP( __n__ )>::type \
		make_tuple(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_CREF_PARAMS)) { \
			typedef typename detail::make_tuple_mapper< YGGR_PP_FOO_TYPES_GROUP( __n__ ) >::type ret_type; \
			return ret_type(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_TUPLE_LIMIT_LENGTH() )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

// tie detail
namespace detail
{

// tie_traits
using boost::tuples::detail::tie_traits;

#define YGGR_TMP_PP_T_PARAMS_TITLE() T

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() void
	template
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE(
				YGGR_TUPLE_LIMIT_LENGTH(),
				typename YGGR_TMP_PP_T_PARAMS_TITLE() )
		>
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct tie_mapper
{
#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) \
	typename tie_traits< BOOST_PP_CAT(YGGR_TMP_PP_T_PARAMS_TITLE(), __n__) >::type

	typedef
		tuple
		<
			YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(
				YGGR_TUPLE_LIMIT_LENGTH(),
				YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER)
		> type;

#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
};

#undef YGGR_TMP_PP_T_PARAMS_TITLE

} // namespace detail

// Tie function templates -------------------------------------------------

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename detail::tie_mapper<YGGR_PP_FOO_TYPES_GROUP( __n__ )>::type \
		tie(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_REF_PARAMS)) { \
			typedef typename detail::tie_mapper< YGGR_PP_FOO_TYPES_GROUP( __n__ ) >::type ret_type; \
			return ret_type(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_TUPLE_LIMIT_LENGTH() )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

// ------------swap-------------
inline void swap(null_type&, null_type&)
{
}

template<typename HH1, typename HH2> inline
void swap(cons<HH1, null_type>& l, cons<HH2, null_type>& r)
{
	l.swap(r);
}

template<typename HH> inline
void swap(cons<HH, null_type>& l, cons<HH, null_type>& r)
{
	l.swap(r);
}

template<typename HH1, typename TT1, typename HH2, typename TT2> inline
void swap(cons<HH1, TT1>& l, cons<HH2, TT2>& r)
{
	l.swap(r);
}

template<typename HH, typename TT> inline
void swap(cons<HH, TT>& l, cons<HH, TT>& r)
{
	l.swap(r);
}

template<typename HH, typename TT, typename HH2, typename TT2> inline
void swap(cons<HH, TT>& l, boost::tuples::cons<HH2, TT2>& r)
{
	l.swap(r);
}

template<typename HH, typename TT, typename HH2, typename TT2> inline
void swap(boost::tuples::cons<HH, TT>& l, cons<HH2, TT2>& r)
{
	r.swap(l);
}

template<
	YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T1),
	YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T2) > inline
void swap(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T1)>& l,
			tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	l.swap(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) > inline
void swap(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& l,
			tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& r)
{
	l.swap(r);
}

template<
	YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T1),
	YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2) > inline
void swap(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T1)>& l,
			boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	l.swap(r);
}

template<
	YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
	YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T2) > inline
void swap(boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
			tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	r.swap(l);
}

} // namespace tuples
} // namespace yggr

namespace std
{
	using ::yggr::tuples::swap;
} // namespace std

namespace boost
{
	using ::yggr::tuples::swap;
} // namespace boost

//----------------------------------------------------
// boost extended

namespace boost
{
namespace tuples
{

// access_traits
template<int N, typename HT, typename TT> inline
typename
	access_traits
	<
		typename element<N, yggr::tuples::cons<HT, TT> >::type
	>::non_const_type
	get(yggr::tuples::cons<HT, TT>& c BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int, N))
{
		typedef typename detail::drop_front<N>::BOOST_NESTED_TEMPLATE
					apply< yggr::tuples::cons<HT, TT> > impl;
		typedef typename impl::type cons_element;
		return const_cast<cons_element&>(impl::call(c)).head;
}

template<int N, typename HT, typename TT> inline
typename
	access_traits
	<
		typename element<N, yggr::tuples::cons<HT, TT> >::type
	>::const_type
	get(const yggr::tuples::cons<HT, TT>& c BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int, N))
{
	typedef typename detail::drop_front<N>::BOOST_NESTED_TEMPLATE
				apply< yggr::tuples::cons<HT, TT> > impl;
	typedef typename impl::type cons_element;
	return impl::call(c).head;
}

// length
template<>
struct length<yggr::tuples::tuple<> >
{
	YGGR_STATIC_CONSTANT(int, value = 0);
};

template<>
struct length<yggr::tuples::tuple<> const>
{
	YGGR_STATIC_CONSTANT(int, value = 0);
};

// swap ex

template<typename HH1, typename HH2> inline
void swap(cons<HH1, null_type>& l, cons<HH2, null_type>& r)
{
	::yggr::tuples::cons_swap(l, r);
}

template<typename HH1, typename TT1, typename HH2, typename TT2> inline
void swap(cons<HH1, TT1>& l, cons<HH2, TT2>& r)
{
	::yggr::tuples::cons_swap(l, r);
}

template<
	YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
	YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2) > inline
void swap(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
			tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef typename l_type::inherited lbase_type;
	typedef typename r_type::inherited rbase_type;

	lbase_type& lbase_ref = l;
	rbase_type& rbase_ref = r;

	::yggr::tuples::cons_swap(lbase_ref, rbase_ref);
}

} // namespace tuples

using boost::tuples::swap;

} // namespace boost

namespace yggr
{
namespace tuples
{

using boost::tuples::get;

} // namespace tuples
} // namespace yggr

#endif // _YGGR_TUPLE_DETAIL_TUPLE_BASIC_HPP__
