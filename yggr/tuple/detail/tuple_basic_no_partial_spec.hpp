//tuple_basic_no_partial_spec.hpp

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

#ifndef __YGGR_TUPLE_DETAIL_TUPLE_BASIC_NO_PARTIAL_SPEC_HPP__
#define __YGGR_TUPLE_DETAIL_TUPLE_BASIC_NO_PARTIAL_SPEC_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/tuple/detail/tuple_config.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/ref.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/selection/max.hpp>
#include <boost/preprocessor/selection/min.hpp>
#include <boost/preprocessor/control/if.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/int.hpp>

namespace yggr 
{
namespace tuples 
{

using boost::tuples::null_type;

// a helper function to provide a const null_type type temporary

using boost::tuples::cnull_type;
inline const null_type cnull() { return null_type(); }


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


// forward declaration of cons
template<typename HT, typename TT = null_type>
struct cons;

namespace detail 
{

using boost::tuples::detail::assign_to_pointee;

using boost::tuples::detail::swallow_assign;
using boost::tuples::detail::add_const_reference;

template<typename T>
struct is_assign_to_pointee
	: public boost::mpl::false_
{
};

template<typename T>
struct is_assign_to_pointee< assign_to_pointee<T> >
	: public boost::mpl::true_
{
};

template<typename T>
struct is_assign_to_pointee< assign_to_pointee<T> const >
	: public boost::mpl::true_
{
};

template<typename T>
struct is_tie_cons
	: public boost::mpl::false_
{
};

template<typename HH, typename HT,
			template<typename _HH, typename _HT> class Cons>
struct is_tie_cons< Cons<HH, HT> >
	: public 
		boost::mpl::or_
		<
			typename is_assign_to_pointee<HH>::type,
			typename is_tie_cons<HT>::type
		>::type
{
};

template<typename HH, typename HT,
			template<typename _HH, typename _HT> class Cons>
struct is_tie_cons< Cons<HH, HT> const >
	: public 
		boost::mpl::or_
		<
			typename is_assign_to_pointee<HH>::type,
			typename is_tie_cons<HT>::type
		>::type
{
};

template<>
struct is_tie_cons<null_type>
	: public boost::mpl::false_
{
};

//using boost::tuples::detail::init_tail;
//using boost::tuples::detail::init_head;

template <class MyTail>
struct init_tail
{
	// Each of vc6 and vc7 seem to require a different formulation
	// of this return type
	template <typename H, typename T> inline
#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
	static typename add_reference<typename add_const<T>::type>::type
#else
	static typename add_const_reference<T>::type
#endif
	execute( cons<H,T> const& u, long )
	{
		return u.get_tail();
	}

	template <typename H, typename T> inline
#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
	static typename add_reference<typename add_const<T>::type>::type
#else
	static typename add_const_reference<T>::type
#endif
	execute( boost::tuples::cons<H,T> const& u, long )
	{
		return u.get_tail();
	}

};

template <>
struct init_tail<null_type>
{
	template <typename H> inline
	static null_type execute( cons<H,null_type> const& u, long )
	{
		return null_type();
	}

	template <typename H> inline
	static null_type execute( boost::tuples::cons<H,null_type> const& u, long )
	{
		return null_type();
	}

	template <typename U> inline
	static null_type execute(U const&, ...)
	{
		return null_type();
	}

private:
	//template <typename H, typename T>
	//void execute( cons<H,T> const&, int);

	//template <typename H, typename T>
	//void execute( boost::tuples::cons<H,T> const&, int);
};

template <class Other> inline
Other const& init_head( Other const& u, ... )
{
	return u;
}

template <typename H, typename T> inline
typename boost::add_reference<typename boost::add_const<H>::type>::type
	init_head( cons<H,T> const& u, int )
{
	return u.get_head();
}

template <typename H, typename T> inline
typename boost::add_reference<typename boost::add_const<H>::type>::type
	init_head( boost::tuples::cons<H,T> const& u, int )
{
	return u.get_head();
}

inline char**** init_head(null_type const&, int);

} //namespace detail

template<typename Tuple>
struct tuple_head_t
{
	typedef typename Tuple::head_type type;
};

template<typename Tuple>
struct tuple_tail_t
{
	typedef typename Tuple::tail_type type;
};


// get_head and get_tail
// get_head
template<typename HT, typename TT> inline
typename boost::add_reference<typename yggr::tuples::cons<HT, TT>::head_type>::type 
	get_head(::yggr::tuples::cons<HT, TT>& val)
{
	return val.head;
}

template<typename HT, typename TT> inline
typename detail::add_const_reference
			<
				typename ::yggr::tuples::cons<HT, TT>::head_type
			>::type 
	get_head(const ::yggr::tuples::cons<HT, TT>& val)
{
	return val.head;
}

template<typename HT, typename TT> inline
typename boost::add_reference<typename boost::tuples::cons<HT, TT>::head_type>::type
	get_head(::boost::tuples::cons<HT, TT>& val)
{
	return val.head;
}

template<typename HT, typename TT> inline
typename detail::add_const_reference
			<
				typename ::boost::tuples::cons<HT, TT>::head_type
			>::type 
	get_head(const ::boost::tuples::cons<HT, TT>& val)
{
	return val.head;
}

template<typename F, typename S> inline
typename boost::add_reference<F>::type
	get_head(::std::pair<F, S>& val)
{
	return val.first;
}

template<typename F, typename S> inline
typename detail::add_const_reference<F>::type
	get_head(const ::std::pair<F, S>& val)
{
	return val.first;
}

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
	return val;
}

template<YGGR_PP_FOO_TYPES_DEF(BOOST_TUPLE_LIMIT_LENGTH())> inline
const typename ::boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>::inherited&
	get_inherited(const ::boost::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(BOOST_TUPLE_LIMIT_LENGTH())>& val)
{
	return val;
}

template<YGGR_PP_FOO_TYPES_DEF(YGGR_TUPLE_LIMIT_LENGTH())> inline
typename ::yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>::inherited&
	get_inherited(::yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>& val)
{
	return val;
}

template<YGGR_PP_FOO_TYPES_DEF(YGGR_TUPLE_LIMIT_LENGTH())> inline
const typename ::yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>::inherited&
	get_inherited(const ::yggr::tuples::tuple<YGGR_PP_FOO_TYPES_GROUP(YGGR_TUPLE_LIMIT_LENGTH())>& val)
{
	return val;
}


// get_tail

template<typename HT, typename TT> inline
typename boost::add_reference<typename yggr::tuples::cons<HT, TT>::tail_type>::type 
	get_tail(::yggr::tuples::cons<HT, TT>& val)
{
	return val.tail;
}

template<typename HT, typename TT> inline
typename 
	detail::add_const_reference
	<
		typename ::yggr::tuples::cons<HT, TT>::tail_type
	>::type 
	get_tail(const ::yggr::tuples::cons<HT, TT>& val)
{
	return val.tail;
}

template<typename HT, typename TT> inline
typename boost::add_reference<typename boost::tuples::cons<HT, TT>::tail_type>::type 
	get_tail(::boost::tuples::cons<HT, TT>& val)
{
	return val.tail;
}

template<typename HT, typename TT> inline
typename 
	detail::add_const_reference
	<
		typename ::boost::tuples::cons<HT, TT>::tail_type
	>::type 
	get_tail(const ::boost::tuples::cons<HT, TT>& val)
{
	return val.tail;
}

template<typename F, typename S> inline
typename boost::add_reference<S>::type
	get_tail(::std::pair<F, S>& val)
{
	return val.second;
}

template<typename F, typename S> inline
typename detail::add_const_reference<S>::type
	get_tail(const ::std::pair<F, S>& val)
{
	return val.second;
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

template<typename HT, typename TT, typename HT2, typename TT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline 
Cons_Dst<HT, TT>& 
	cons_conv_impl(Cons_Dst<HT, TT>& dst, 
					BOOST_RV_REF_BEG Cons_Src<HT2, TT2> BOOST_RV_REF_END src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT2, TT2> src_cons_type;

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

template<typename HT, typename HT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline 
Cons_Dst<HT, null_type>&
	cons_conv_impl(Cons_Dst<HT, null_type>& dst, 
					BOOST_RV_REF_BEG Cons_Src<HT2, null_type> BOOST_RV_REF_END src)
{
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

inline null_type& cons_conv_impl(null_type& dst, const null_type& src)
{
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
	typedef Cons_Src<HT2, TT2> src_cons_type;

	src_cons_type src_ref = src;

	if(boost::addressof(dst) == reinterpret_cast<const dst_cons_type*>(boost::addressof(src_ref)))
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
	typedef Cons_Src<HT2, TT2> src_cons_type;

	if(boost::addressof(dst) == reinterpret_cast<const dst_cons_type*>(boost::addressof(src)))
	{
		return dst;
	}

	return detail::cons_conv_impl(dst, src);
}

template<typename HT, typename TT, typename F, typename S,
			template<typename _HT, typename _TT> class Cons_Dst> inline 
Cons_Dst<HT, TT>&
	cons_conv(Cons_Dst<HT, TT>& dst, 
				BOOST_RV_REF_BEG ::std::pair< F, S > BOOST_RV_REF_END src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef ::std::pair<F, S> src_pair_type;

	BOOST_STATIC_ASSERT((length<dst_cons_type>::value == 2));

	src_pair_type& src_ref = src;
	if(boost::addressof(dst) == reinterpret_cast<dst_cons_type*>(boost::addressof(src_ref)))
	{
		return *this;
	}

	get_head(dst) = boost::foreard<F>(get_head(src));
	get_head(get_tail(dst)) = boost::forared<S>(get_tail(src));
	return dst;
}

template<typename HT, typename TT, typename F, typename S,
			template<typename _HT, typename _TT> class Cons_Dst> inline
Cons_Dst<HT, TT>&
	cons_conv(Cons_Dst<HT, TT>& dst, const ::std::pair< F, S >& src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;

	BOOST_STATIC_ASSERT((length<dst_cons_type>::value == 2));
	
	if(boost::addressof(dst) == reinterpret_cast<const dst_cons_type*>(boost::addressof(src)))
	{
		return *this;
	}

	get_head(dst) = get_head(src);
	get_head(get_tail(dst)) = get_tail(src);
	return dst;
}

template<typename HT, typename F,
			template<typename _HT, typename _TT> class Cons_Dst> inline 
Cons_Dst<HT, null_type>&
	cons_conv(Cons_Dst<HT, null_type>& dst, 
				BOOST_RV_REF_BEG ::std::pair< F, null_type > BOOST_RV_REF_END src)
{
	typedef ::std::pair<F, null_type> src_pair_type;

	src_pair_type& src_ref = src;
	if(boost::addressof(dst) == reinterpret_cast<dst_cons_type*>(boost::addressof(src_ref)))
	{
		return *this;
	}

	get_head(dst) = boost::move(get_head(src_ref));
	return dst;
}

template<typename HT, typename F,
			template<typename _HT, typename _TT> class Cons_Dst> inline 
Cons_Dst<HT, null_type>&
	cons_conv(Cons_Dst<HT, null_type>& dst, const ::std::pair< F, null_type >& src)
{
	typedef Cons_Dst<HT, null_type> dst_cons_type;

	if(boost::addressof(dst) == reinterpret_cast<const dst_cons_type*>(boost::addressof(src)))
	{
		return *this;
	}

	get_head(dst) = get_head(src);
	return dst;
}

template<typename F, typename S, typename HT, typename TT,
			template<typename _HT, typename _TT> class Cons_Src> inline 
::std::pair< F, S >&
	cons_conv(::std::pair< F, S >& dst, 
				BOOST_RV_REF_BEG Cons_Src<HT, TT> BOOST_RV_REF_END src)
{

	typedef Cons_Src<HT, TT> src_cons_type;
	typedef typename src_cons_type::stored_head_type src_head_type;
	typedef typename src_cons_type::tail_type::stored_head_type src_tail_head_type;


	BOOST_STATIC_ASSERT((length<src_cons_type>::value == 2));

	src_cons_type src_ref = src;

	if(boost::addressof(src_ref) == reinterpret_cast<src_cons_type*>(boost::addressof(dst)))
	{
		return dst;
	}

	get_head(dst) = boost::move(get_head(src_ref));
	get_tail(dst) = boost::move(get_head(get_tail(src_ref)));

	return dst;
}

template<typename F, typename S, typename HT, typename TT,
			template<typename _HT, typename _TT> class Cons_Src> inline
::std::pair< F, S >&
	cons_conv(::std::pair< F, S >& dst, const Cons_Src<HT, TT>& src)
{
	typedef Cons_Src<HT, TT> src_cons_type;

	BOOST_STATIC_ASSERT((length<src_cons_type>::value == 2));

	if(boost::addressof(src) == reinterpret_cast<src_cons_type*>(boost::addressof(dst)))
	{
		return dst;
	}

	get_head(dst) = get_head(src);
	get_tail(dst) = get_head(get_tail(src));
	return dst;
}

template<typename F, typename HT,
			template<typename _HT, typename _TT> class Cons_Src> inline 
::std::pair< F, null_type >&
	cons_conv(::std::pair< F, null_type >& dst, 
				BOOST_RV_REF_BEG Cons_Src<HT, null_type> BOOST_RV_REF_END src)
{

	typedef Cons_Src<HT, null_type> src_cons_type;
	typedef typename src_cons_type::head_type src_head_type;

	src_cons_type src_ref = src;

	if(boost::addressof(src_ref) == reinterpret_cast<src_cons_type*>(boost::addressof(dst)))
	{
		return dst;
	}

	get_head(dst) = boost::move(get_head(src));
	return dst;
}

template<typename F, typename HT,
			template<typename _HT, typename _TT> class Cons_Src> inline
::std::pair< F, null_type >&
	cons_conv(::std::pair< F, null_type >& dst, const Cons_Src<HT, null_type>& src)
{
	typedef Cons_Src<HT, null_type> src_cons_type;

	if(boost::addressof(src) == reinterpret_cast<src_cons_type*>(boost::addressof(dst)))
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

template<typename HT, typename TT, typename HT2, typename TT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
void cons_swap_impl(Cons_Dst<HT, TT>& dst, Cons_Src<HT2, TT2>& src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT, TT> src_cons_type;

	BOOST_STATIC_ASSERT((length<dst_cons_type>::value == length<src_cons_type>::value));
	boost::swap(get_head(dst), get_head(src));
	cons_swap_impl(get_tail(dst), get_tail(src));
}

template<typename HT, typename HT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
void cons_swap_impl(Cons_Dst<HT, null_type>& dst, Cons_Src<HT2, null_type>& src)
{
	boost::swap(get_head(dst), get_head(src));
}

inline void cons_swap_impl(null_type& dst, null_type& src)
{
	return;
}

} // namespace detail

template<typename HT, typename TT, typename HT2, typename TT2,
			template<typename _HT, typename _TT> class Cons_Dst,
			template<typename _HT, typename _TT> class Cons_Src> inline
void cons_swap(Cons_Dst<HT, TT>& dst, Cons_Src<HT2, TT2>& src)
{
	typedef Cons_Dst<HT, TT> dst_cons_type;
	typedef Cons_Src<HT, TT> src_cons_type;

	if(boost::addressof(dst) == reinterpret_cast<dst_cons_type*>(boost::addressof(src)))
	{
		return;
	}

	detail::cons_swap_impl(src, dst);
}


inline void cons_swap(null_type& dst, null_type& src)
{
	return;
}

// cons builds a heterogenous list of types
template<typename HT, typename TT>
struct cons
{
	typedef cons self_type;
	typedef HT head_type;
	typedef TT tail_type;

private:
	typedef cons this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef typename boost::add_reference<head_type>::type head_ref;
	typedef typename boost::add_reference<tail_type>::type tail_ref;
	typedef typename detail::add_const_reference<head_type>::type head_cref;
	typedef typename detail::add_const_reference<tail_type>::type tail_cref;

public:
	head_type head;
	tail_type tail;

	inline head_ref get_head(void) 
	{ 
		return head; 
	}

	inline tail_ref get_tail(void) 
	{
		return tail; 
	}

	inline head_cref get_head(void) const 
	{ 
		return head; 
	}

	inline tail_cref get_tail(void) const 
	{ 
		return tail; 
	}

public:
	cons(void) 
		: head(), tail() 
	{
	}

#if defined BOOST_MSVC
	template<typename TT2>
	cons(head_cref h /* = head_type() */, // causes MSVC 6.5 to barf.
			const TT2& t) 
		: head(h), tail(t.head, t.tail)
	{
	}

	cons(head_cref h /* = head_type() */, // causes MSVC 6.5 to barf.
			const null_type& t) 
		: head(h), tail(t)
	{
	}

#else
	template<typename TT2>
	explicit cons(head_cref h, const TT2& t)
		: head(h), tail(t.head, t.tail)
	{
	}

	explicit cons(head_cref h = head_type(),
					tail_cref t = tail_type())
		: head(h), tail(t)
	{
	}
#endif // BOOST_MSVC

	template<typename U>
	cons(const U& u)
		: head(detail::init_head(u, 0)), 
			tail(detail::init_tail<tail_type>::execute(u, 0L))
	{
	}

	template<typename HT2, typename TT2>
	cons(const BOOST_RV_REF_BEG cons<HT2, TT2> BOOST_RV_REF_END right)
		: head(boost::move(right.head)), 
			tail(boost::move(right.tail)) 
	{
	}

	template<typename HT2, typename TT2>
	cons(const cons<HT2, TT2>& right)
		: head(right.head), tail(right.tail) 
	{
	}

	template<typename HT2, typename TT2>
	cons(const ::boost::tuples::cons<HT2, TT2>& right)
		: head(right.head), tail(cons_conv(right.tail)) 
	{
	}

	cons(BOOST_RV_REF(this_type) right)
		: head(boost::move(right.head)),
			tail(boost::move(right.tail))
	{
	}

	cons(const this_type& right)
		: head(right.head),
			tail(right.tail)
	{
	}

public:
	inline operator typename cons_conv_helper<this_type>::type(void) const
	{
		return cons_conv(*this);
	}

public:
	template<typename Other> inline
	this_type& operator=(BOOST_RV_REF(Other) other)
	{
		cons_conv(*this, other);
		return *this;
	}

	template<typename Other>
	this_type& operator=(const Other& other)
	{
		cons_conv(*this, other);
		return *this;
	}

	template<typename F, typename S> inline
	this_type& operator=(BOOST_RV_REF_BEG std::pair<F, S> BOOST_RV_REF_END right)
	{
		cons_conv(*this, right);
		return *this;
	}

	template<typename F, typename S>
	this_type& operator=(const std::pair<F, S>& right)
	{
		cons_conv(*this, right);
		return *this;
	}

	template<typename HT2, typename TT2>
	this_type& operator=(const boost::tuples::cons<HT2, TT2>& right)
	{
		cons_conv(*this, right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(head, boost::move(right.head));
		copy_or_move_or_swap(tail, boost::move(right.tail));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		head = right.head;
		tail = right.tail;

		return *this;
	}

public:
	template<typename HT2, typename TT2> inline
	void swap(BOOST_RV_REF_BEG ::boost::tuples::cons<HT2, TT2> BOOST_RV_REF_END right)
	{
		typedef ::boost::tuples::cons<HT2, TT2> right_type;
		
		right_type& right_ref = right;
		cons_swap(*this, right_ref);
	}

	template<typename HT2, typename TT2>
	void swap(::boost::tuples::cons<HT2, TT2>& right)
	{
		cons_swap(*this, right);
	}

	template<typename HT2, typename TT2> inline
	void swap(BOOST_RV_REF_BEG cons<HT2, TT2> BOOST_RV_REF_END right)
	{
		typedef cons<HT2, TT2> right_type;

		right_type& right_ref = right;
		cons_swap(*this, right_ref);
	}

	template<typename HT2, typename TT2>
	void swap(cons<HT2, TT2>& right)
	{
		cons_swap(*this, right);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		cons_swap(*this, right);
	}
};

namespace detail 
{

using boost::tuples::detail::is_null_type;

template<typename Head, typename Tail>
struct build_cons
{
private:
	YGGR_STATIC_CONSTANT(bool, tail_is_null_type = is_null_type<Tail>::RET);
public:
	typedef cons<Head, Tail> RET;
};

template<>
struct build_cons<null_type, null_type>
{
	typedef null_type RET;
};

#define YGGR_PARAMS_NAME() T
#define YGGR_TUPLE_INDEX( __n__ ) BOOST_PP_SUB( __n__, 1 )

template<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename YGGR_PARAMS_NAME() )>
struct map_tuple_to_cons
{
#define YGGR_TMP_PP_MAKE_BUILD_CONS( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			typedef typename detail::build_cons < \
					BOOST_PP_CAT(YGGR_PARAMS_NAME(), BOOST_PP_SUB( YGGR_TUPLE_INDEX(YGGR_TUPLE_LIMIT_LENGTH()), __n__ )), \
					BOOST_PP_IF( __n__, \
									BOOST_PP_CAT( cons, \
										BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), __n__ ) ), \
									null_type ) \
				>::RET BOOST_PP_CAT(cons, BOOST_PP_SUB( YGGR_TUPLE_INDEX(YGGR_TUPLE_LIMIT_LENGTH()), __n__ ))

	YGGR_PP_REPEAT_CUSTOM_OP_NOW(YGGR_TUPLE_LIMIT_LENGTH(), 
									YGGR_TMP_PP_MAKE_BUILD_CONS, 
									YGGR_PP_SYMBOL_SEMICOLON)
#undef YGGR_TMP_PP_MAKE_BUILD_CONS

	YGGR_PP_SYMBOL_SEMICOLON() // the last senicolon

	typedef cons0 type;

};

#undef YGGR_PARAMS_NAME

using boost::tuples::detail::_element_type;

} // namespace detail

using boost::tuples::element;

namespace detail 
{

#if defined(BOOST_MSVC) && (BOOST_MSVC == 1300)

// special workaround for vc7:
using boost::tuples::detail::reference_adder;

#endif // BOOST_MSVC

using boost::tuples::detail::element_ref;
using boost::tuples::detail::element_const_ref;

} // namespace detail
	
// Get length of this tuple
using boost::tuples::length;

namespace detail 
{

// Reference the Nth element in a tuple and retrieve it with "get"
//using boost::tuples::detail::get_class;

template<int N>
struct get_class
{
	template<typename Head, typename Tail> inline 
	static typename detail::element_ref<N, cons<Head, Tail> >::RET
		get(cons<Head, Tail>& t)
	{
		return get_class<N-1>::get(t.tail);
	}

	template<typename Head, typename Tail> inline 
	static typename detail::element_const_ref<N, cons<Head, Tail> >::RET
		get(const cons<Head, Tail>& t)
	{
		return get_class<N-1>::get(t.tail);
	}
};

template<>
struct get_class<0>
{
	template<typename Head, typename Tail> inline 
	static typename boost::add_reference<Head>::type
		get(cons<Head, Tail>& t)
	{
		return t.head;
	}

	template<typename Head, typename Tail> inline 
	static typename boost::add_reference<const Head>::type
		get(const cons<Head, Tail>& t)
	{
		return t.head;
	}
};

} // namespace detail

// tuple class
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
private:
	typedef
		detail::map_tuple_to_cons
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(
				YGGR_TUPLE_LIMIT_LENGTH(), 
				YGGR_TMP_PP_T_PARAMS_TITLE())
		> mapped_tuple;


#define YGGR_TMP_PP_MAKE_CONS_T( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			typedef typename \
				mapped_tuple::BOOST_PP_CAT(cons, __n__ ) BOOST_PP_CAT(cons, __n__ )

	YGGR_PP_REPEAT_CUSTOM_OP_NOW(YGGR_TUPLE_LIMIT_LENGTH(), 
									YGGR_TMP_PP_MAKE_CONS_T, 
									YGGR_PP_SYMBOL_SEMICOLON);

#undef YGGR_TMP_PP_MAKE_CONS_T

//YGGR_PP_SYMBOL_SEMICOLON() // the last senicolon

#define YGGR_TMP_PP_V_PARAMS( __n__ ) BOOST_PP_CAT( BOOST_PP_CAT( t, __n__ ), _cref )
#define YGGR_TMP_PP_MAKE_CONS_CREF( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			typedef typename detail::add_const_reference< \
				BOOST_PP_CAT(YGGR_TMP_PP_T_PARAMS_TITLE(), __n__ ) >::type \
					YGGR_TMP_PP_V_PARAMS( __n__ )

	YGGR_PP_REPEAT_CUSTOM_OP_NOW(YGGR_TUPLE_LIMIT_LENGTH(), 
									YGGR_TMP_PP_MAKE_CONS_CREF, 
									YGGR_PP_SYMBOL_SEMICOLON);

#undef YGGR_TMP_PP_MAKE_CONS_CREF
#undef YGGR_TMP_PP_V_PARAMS

YGGR_PP_SYMBOL_SEMICOLON() // the last senicolon


public:
	typedef typename mapped_tuple::type inherited;
	typedef tuple self_type;

private:
	typedef tuple this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

	// tuple(void)
	tuple(void) :
// cons1(T1(), cons2(T2(), cons3(T3(), cons4(T4(), cons5(T5(), cons6(T6(),cons7(T7(),cons8(T8(),cons9(T9(),cons10(T10()
#define YGGR_TMP_PP_MAKE_CONS_INIT_LEFT( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			BOOST_PP_CAT(cons, __n__) YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES_L) \
				BOOST_PP_CAT(YGGR_TMP_PP_T_PARAMS_TITLE(), __n__ ) \
				YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES)

	YGGR_PP_REPEAT_CUSTOM_OP_NOW(YGGR_TUPLE_LIMIT_LENGTH(), 
									YGGR_TMP_PP_MAKE_CONS_INIT_LEFT, 
									YGGR_PP_SYMBOL_COMMA)

#undef YGGR_TMP_PP_MAKE_CONS_INIT_LEFT

//))))))))))
#define YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES_R)

	YGGR_PP_REPEAT_CUSTOM_OP_NOW(YGGR_TUPLE_LIMIT_LENGTH(), 
									YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT, 
									YGGR_PP_SYMBOL_EMPTY)

#undef YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT
	YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_BRACES) // foo body { }

	// tuple(t0_cref t0, t1_cref t1, ...)
	tuple(
			t0_cref t0,
			t1_cref t1,
#define YGGR_TMP_PP_MAKE_CONSTRUCT_PARAMS( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			BOOST_PP_CAT(BOOST_PP_CAT( t, BOOST_PP_ADD(__n__, 2) ), _cref) \
				BOOST_PP_CAT(t, BOOST_PP_ADD( __n__, 2 )) = \
					BOOST_PP_CAT( YGGR_TMP_PP_T_PARAMS_TITLE(), BOOST_PP_ADD(__n__, 2) ) \
						YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES)

		YGGR_PP_REPEAT_CUSTOM_OP_NOW(BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), 2), 
										YGGR_TMP_PP_MAKE_CONSTRUCT_PARAMS, 
										YGGR_PP_SYMBOL_COMMA)

#undef YGGR_TMP_PP_MAKE_CONSTRUCT_PARAMS
	) :
//cons1(t1, cons2(t2, cons3(t3, cons4(t4, cons5(t5, cons6(t6,cons7(t7,cons8(t8,cons9(t9,cons10(t10 // real start is cons0
#define YGGR_TMP_PP_MAKE_CONS_INIT_LEFT( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			BOOST_PP_CAT( cons, __n__ ) YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES_L) \
				BOOST_PP_CAT(t, __n__ )

	YGGR_PP_REPEAT_CUSTOM_OP_NOW(YGGR_TUPLE_LIMIT_LENGTH(), 
									YGGR_TMP_PP_MAKE_CONS_INIT_LEFT, 
									YGGR_PP_SYMBOL_COMMA)

#undef YGGR_TMP_PP_MAKE_CONS_INIT_LEFT

//))))))))))
#define YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			YGGR_PP_SYMBOL( YGGR_PP_SYMBOL_PARENTHESES_R )

	YGGR_PP_REPEAT_CUSTOM_OP_NOW(YGGR_TUPLE_LIMIT_LENGTH(), 
									YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT, 
									YGGR_PP_SYMBOL_EMPTY)

#undef YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT
	YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_BRACES) // foo body { }

	// explicit tuple(t0_cref t0)
	explicit tuple(t0_cref t0)
		: inherited(t0, 
//cons2(T2(), cons3(T3(), cons4(T4(), cons5(T5(), cons6(T6(),cons7(T7(),cons8(T8(),cons9(T9(),cons10(T10() // real start is cons0
#define YGGR_TMP_PP_MAKE_CONS_INIT_LEFT( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			BOOST_PP_CAT(cons, BOOST_PP_ADD(__n__, 1)) YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES_L) \
				BOOST_PP_CAT(YGGR_TMP_PP_T_PARAMS_TITLE(), BOOST_PP_ADD(__n__, 1) ) \
				YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES)

	YGGR_PP_REPEAT_CUSTOM_OP_NOW(BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), 1), 
									YGGR_TMP_PP_MAKE_CONS_INIT_LEFT, 
									YGGR_PP_SYMBOL_COMMA)

#undef YGGR_TMP_PP_MAKE_CONS_INIT_LEFT

//)))))))))
#define YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES_R)
	
	YGGR_PP_REPEAT_CUSTOM_OP_NOW(BOOST_PP_SUB(YGGR_TUPLE_LIMIT_LENGTH(), 1), 
									YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT, 
									YGGR_PP_SYMBOL_EMPTY)

#undef YGGR_TMP_PP_MAKE_CONS_INIT_RIGHT
			){}

	template<typename HT2, typename TT2>
	tuple(BOOST_RV_REF_BEG cons<HT2, TT2> BOOST_RV_REF_END other)
		: inherited(other)
	{
	}

	template<typename HT2, typename TT2>
	tuple(const cons<HT2, TT2>& other)
		: inherited(other)
	{
	}

	template<typename HT2, typename TT2>
	tuple(const ::boost::tuples::cons<HT2, TT2>& right) 
		: inherited(cons_conv(right))
	{
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)>
	tuple(const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>& p) 
		: inherited(
				cons_conv(
					static_cast
						<
							const typename boost::tuples::tuple
								<
									YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)
								>::inherited&
						>(p)))
	{
	}

	tuple(BOOST_RV_REF(this_type) right)
		: inherited(boost::move(static_cast<inherited&>(right)))
	{
	}

	tuple(const this_type& right)
		: inherited(static_cast<const inherited&>(right))
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

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)> inline
	operator 
		boost::tuples::tuple
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), BT)
		>(void) const
	{
		typedef boost::tuples::tuple
				<
					YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), BT)
				> rst_type;
		typedef typename rst_type::inherited rst_base_type;

		// if compiler error stop this place, it means this foo is failed, 
		//		because
		//			boost::tuple and 
		//			boost::tuples::cons and
		//			boost::tuples::detail::assign_to_pointee
		//		not write self's operator=,
		// so without modifying the BOOST premise, it is impossible to support tie(...) = tie(...)
		BOOST_MPL_ASSERT_MSG((boost::mpl::not_<typename detail::is_tie_cons<rst_base_type>::type>::value),
								IS_TIE_CONS_TYPE,
								(boost::tuples::tuple<
										YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), BT)>) );
		const inherited& base = *this;
		return rst_type(cons_conv(base));
	}

	template<typename F, typename S>
	inline this_type& operator=(BOOST_RV_REF_BEG std::pair<F, S> BOOST_RV_REF_END other)
	{
		inherited& base = *this;
		cons_conv(base, other);
		return *this;
	}

	template<typename F, typename S>
	this_type& operator=(const std::pair<F, S>& other)
	{
		inherited& base = *this;
		cons_conv(base, other);
		return *this;
	}

	template<typename HT2, typename TT2> inline
	this_type& operator=(BOOST_RV_REF_BEG cons<HT2, TT2> BOOST_RV_REF_END other)
	{
		inherited::operator=(other);
		return *this;
	}

	template<typename HT2, typename TT2>
	this_type& operator=(const cons<HT2, TT2>& other)
	{
		inherited::operator=(other);
		return *this;
	}

	template<typename U1, typename U2> inline
	this_type& operator=(BOOST_RV_REF_BEG boost::tuples::cons<U1, U2> BOOST_RV_REF_END k) 
	{
		inherited& base = *this;
		cons_conv(base, k);
		return *this;
	}

	template<typename U1, typename U2>
	this_type& operator=(const boost::tuples::cons<U1, U2>& k) 
	{
		inherited& base = *this;
		cons_conv(base, k);
		return *this;
	}

	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename BT)> inline
	this_type& operator=(BOOST_RV_REF(boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)>) p)
	{
		typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), BT)> other_type;
		typedef typename other_type::inherited other_inherited;

		other_type& other_ref = p;
		inherited& base = *this;
		cons_conv(base, boost::move(static_cast<other_inherited&>(other)));
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


	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		inherited& base = *this;
		copy_or_move_or_swap(base, boost::move(static_cast<inherited&>(right)));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		inherited::head = right.head;
		inherited::tail = right.tail;

		return *this;
	}

	using inherited::swap;

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

//--------------

namespace detail 
{
	
using boost::tuples::detail::workaround_holder;

} // namespace detail

// make_tuple
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	tuple<YGGR_PP_FOO_TYPES_GROUP( __n__ )> \
		make_tuple(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS )) { \
			typedef tuple< YGGR_PP_FOO_TYPES_GROUP( __n__ ) > ret_type; \
			return ret_type(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }

#define YGGR_PP_FOO_ARG_NAME() t
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_TUPLE_LIMIT_LENGTH() )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

// tie
#define YGGR_TMP_PP_MAKE_TIE_RET_T_PARAMS( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			detail::assign_to_pointee< BOOST_PP_CAT(T, __n__) >

#define YGGR_TMP_PP_MAKE_TIE_RET_V_PARAMS( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			detail::assign_to_pointee< BOOST_PP_CAT(T, __n__) >( \
				&BOOST_PP_CAT(YGGR_PP_FOO_ARG_NAME(), __n__) )

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	tuple< YGGR_PP_REPEAT_CUSTOM_OP_NOW(__n__, YGGR_TMP_PP_MAKE_TIE_RET_T_PARAMS, YGGR_PP_SYMBOL_COMMA) > \
		tie( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_REF_PARAMS ) ) { \
			return make_tuple( \
					YGGR_PP_REPEAT_CUSTOM_OP_NOW( \
						__n__, YGGR_TMP_PP_MAKE_TIE_RET_V_PARAMS, YGGR_PP_SYMBOL_COMMA)); }

//#define YGGR_PP_REPEAT_CUSTOM_OP() YGGR_TMP_PP_MAKE_TIE_RET_T_PARAMS
#define YGGR_PP_FOO_ARG_NAME() t

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_TUPLE_LIMIT_LENGTH() )
#include BOOST_PP_LOCAL_ITERATE(  )

#undef YGGR_PP_FOO_ARG_NAME
//#undef YGGR_PP_REPEAT_CUSTOM_OP
#undef YGGR_TMP_PP_MAKE_TIE_RET_T_PARAMS

using boost::tuples::ignore;

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

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
void swap(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T1)>& l,
					tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T2)>& r) 
{
	l.swap(r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)
	> inline
void swap(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& l,
					tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& r) 
{
	l.swap(r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
void swap(tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T1)>& l,
					boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r) 
{
	l.swap(r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
void swap(boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
					tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T2)>& r) 
{
	r.swap(l);
}

} // namespace tuples
} // namespace yggr

namespace yggr
{
namespace tuples
{
namespace swap_support
{


template<typename HH, typename TT, typename HH2, typename TT2> inline 
void swap(yggr::tuples::cons<HH, TT>& l, yggr::tuples::cons<HH2, TT2>& r)
{ 
	::yggr::tuples::swap(l, r); 
} 

template<typename HH, typename TT> inline 
void swap(yggr::tuples::cons<HH, TT>& l, yggr::tuples::cons<HH, TT>& r) 
{ 
	::yggr::tuples::swap(l, r);
} 

template<typename HH, typename TT, typename HH2, typename TT2> inline 
void swap(yggr::tuples::cons<HH, TT>& l, boost::tuples::cons<HH2, TT2>& r) 
{ 
	::yggr::tuples::swap(l, r); 
} 

template<typename HH, typename TT, typename HH2, typename TT2> inline 
void swap(boost::tuples::cons<HH, TT>& l, yggr::tuples::cons<HH2, TT2>& r) 
{ 
	::yggr::tuples::swap(l, r);
} 

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T1), 
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T2) > inline 
void swap(yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T1)>& l, 
					yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T2)>& r)
{ 
	::yggr::tuples::swap(l, r); 
} 

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) > inline 
void swap(yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& l, 
					yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& r) 
{ 
	::yggr::tuples::swap(l, r); 
} 

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T1), 
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2) > inline 
void swap(yggr::tuples::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T1) >& l, 
					boost::tuples::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2) >& r) 
{ 
	::yggr::tuples::swap(l, r);
} 

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1), 
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T2) > inline 
void swap(boost::tuples::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1) >& l, 
					yggr::tuples::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T2) >& r)
{ 
	::yggr::tuples::swap(l, r); 
}


} // namespace swap_support

using swap_support::swap;

} // namespace tuples
} // namespace yggr

namespace std
{
	using ::yggr::tuples::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::tuples::swap_support::swap;
} // namespace boost

//----------------------------------------------------
// boost extended

namespace boost
{
namespace tuples
{

// get 
template<int N, typename Head, typename Tail> inline 
typename detail::element_ref<N, yggr::tuples::cons<Head, Tail> >::RET
//int
	get(yggr::tuples::cons<Head, Tail>& t, 
			detail::workaround_holder<N>* = 0)
{
	return yggr::tuples::detail::get_class<N>::get(t);
}

template<int N, typename Head, typename Tail> inline
typename detail::element_const_ref<N, yggr::tuples::cons<Head, Tail> >::RET
//int
	get(const yggr::tuples::cons<Head, Tail>& t, 
			detail::workaround_holder<N>* = 0)
{
	return yggr::tuples::detail::get_class<N>::get(t);
}

// length
template<> 
struct length< yggr::tuples::tuple<> > 
	: public boost::mpl::int_<0>
{
};

// boost tuple no_partial_spec version not declear this partial spec
template<>
struct length<yggr::tuples::tuple<> const> 
	: public boost::mpl::int_<0>
{
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

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
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

#endif // __YGGR_TUPLE_DETAIL_TUPLE_BASIC_NO_PARTIAL_SPEC_HPP__
