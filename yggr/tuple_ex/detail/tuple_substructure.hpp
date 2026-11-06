// tuple_substructure.hpp

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

#ifndef __YGGR_TUPLE_EX_DETAIL_TUPLE_SUBSTRUCTURE_HPP__
#define __YGGR_TUPLE_EX_DETAIL_TUPLE_SUBSTRUCTURE_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple/tuple_comparison.hpp>
#include <yggr/tuple/tuple_io.hpp>
#include <yggr/tuple_ex/conv_to_tuple.hpp>
#include <yggr/tuple_ex/tuple_maker.hpp>

#include <yggr/mplex/typename_caster.hpp>
#include <yggr/mplex/null_t.hpp>

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
#	include <yggr/type_traits/integer_sequence.hpp>
#else
#	include <yggr/ppex/foo_params.hpp>
#	include <yggr/ppex/symbol_if.hpp>
#	include <yggr/ppex/symbols.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/fold.hpp> // iterator_range
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/min_max.hpp>

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
#	include <boost/mpl/assert.hpp>
#	include <boost/mpl/bool.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
#	include <tuple>
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace yggr
{
namespace tuple_ex
{
namespace detail
{

// cons_to_vector_impl
template<typename Vector, typename Cons>
struct cons_to_vector_impl;

template<template<typename _HT, typename _TT> class ConsTpl,
			typename Vector, typename HT, typename TT>
struct cons_to_vector_impl< Vector, ConsTpl<HT, TT> >
	: public cons_to_vector_impl<typename boost::mpl::push_back<Vector, HT>::type, TT>
{
};

template<template<typename _HT, typename _TT> class ConsTpl,
			typename Vector, typename HT>
struct cons_to_vector_impl< Vector, ConsTpl<HT, tuples::null_type> >
{
public:
	typedef typename boost::mpl::push_back<Vector, HT>::type type;
};

template<template<typename _HT, typename _TT> class ConsTpl, typename Vector>
struct cons_to_vector_impl< Vector, ConsTpl<tuples::null_type, tuples::null_type> >
{
public:
	typedef Vector type;
};

template<typename Vector>
struct cons_to_vector_impl< Vector, tuples::null_type >
{
public:
	typedef Vector type;
};


// sub_tuple_t_impl

template<typename Vector, typename Tuple>
struct conv_to_new_tuple_impl;

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<template<typename ..._Args> class TupleTpl, typename Vector, typename ...Args>
struct conv_to_new_tuple_impl<Vector, TupleTpl<Args...> >
	: public conv_to_tuple<TupleTpl, Vector>
{
};

#else

#	define YGGR_TMP_PP_TUPLE_T_PARAMS_TITLE() _T
#	define YGGR_PP_TEMPLATE_DEFAULT_TYPE() ::yggr::tuples::null_type

#	define _YGGR_TMP_PP_CONV_TO_NEW_TUPLE_IMPL_DEF(__n__) \
		template< \
			template< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( \
					YGGR_TUPLE_LIMIT_LENGTH(), typename YGGR_TMP_PP_TUPLE_T_PARAMS_TITLE() ) \
			> class TupleTpl, \
			typename Vector YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		struct conv_to_new_tuple_impl<Vector, TupleTpl< YGGR_PP_FOO_TYPES_GROUP( __n__ ) > > \
			: public conv_to_tuple<TupleTpl, Vector> {};


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		_YGGR_TMP_PP_CONV_TO_NEW_TUPLE_IMPL_DEF( __n__ )

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_TUPLE_LIMIT_LENGTH() )
#	include BOOST_PP_LOCAL_ITERATE(  )

#	undef YGGR_TMP_PP_TUPLE_T_PARAMS_TITLE
#	undef YGGR_PP_TEMPLATE_DEFAULT_TYPE

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename Vector, typename Tuple>
struct conv_to_new_tuple
	: public conv_to_new_tuple_impl<Vector, Tuple>
{
};

template<typename Tuple, 
			std::size_t Pos, 
			std::size_t Len = static_cast<std::size_t>(-1), 
			bool ArgsValidate = (Pos < tuple_size<Tuple>::type::value) >
struct sub_tuple_t_impl
{
private:
	typedef Tuple tuple_type;
	typedef typename yggr::tuple_size<tuple_type>::type tuple_size_type;
	typedef mplex::typename_caster<tuple_type> tuple_caster_type;

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
	typedef typename tuple_caster_type::template apply<boost::mpl::vector>::type raw_tuple_vt_type;
#else
	// apply<boost::mpl::vector(N)>, the length of N needs to match the length of the tuple
	typedef typename 
		tuple_caster_type::template apply
		< 
			BOOST_PP_CAT(boost::mpl::vector, YGGR_TUPLE_LIMIT_LENGTH()) 
		>::type raw_tuple_vt_type;
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	typedef typename 
		boost::mpl::erase
		<
			raw_tuple_vt_type,
			typename 
				boost::mpl::advance
				<
					typename boost::mpl::begin<raw_tuple_vt_type>::type,
					tuple_size_type
				>::type,
			typename boost::mpl::end<raw_tuple_vt_type>::type
		>::type tuple_vt_type;
	
	typedef typename
		boost::mpl::copy
		<
			typename
				boost::mpl::iterator_range
				<
					typename 
						boost::mpl::advance
						<
							typename boost::mpl::begin<tuple_vt_type>::type,
							boost::mpl::size_t<Pos>
						>::type,
					typename 
						boost::mpl::advance
						<
							typename boost::mpl::begin<tuple_vt_type>::type,
							boost::mpl::size_t<(Len < tuple_size_type::value - Pos? Pos + Len : tuple_size_type::value)>
						>::type
				>::type,
			boost::mpl::back_inserter<boost::mpl::vector<> >
		>::type sub_tuple_vt_type;
	
	typedef typename conv_to_new_tuple<sub_tuple_vt_type, tuple_type>::type new_tuple_type;

public:
	typedef new_tuple_type type;
	
};


template<typename Tuple, std::size_t Pos, std::size_t Len>
struct sub_tuple_t_impl<Tuple, Pos, Len, false>
{
private:
	typedef Tuple tuple_type;

public:
	typedef typename conv_to_new_tuple<typename boost::mpl::vector<>::type, tuple_type>::type type;
};

} // namespace detail

// sub_tuple_t
template<typename Tuple, std::size_t Pos = 0, std::size_t Len = static_cast<std::size_t>(-1)>
struct sub_tuple_t
	: public detail::sub_tuple_t_impl<Tuple, Pos, Len>
{
};

template<typename HT, typename TT, std::size_t Pos, std::size_t Len>
struct sub_tuple_t< boost::tuples::cons<HT, TT>, Pos, Len >
	: public 
		detail::sub_tuple_t_impl
		<
			typename
				conv_to_tuple
				<
					boost::tuple,
					typename 
						detail::cons_to_vector_impl
						< 
							boost::mpl::vector<>,
							boost::tuples::cons<HT, TT> 
						>::type
				>::type,
			Pos, 
			Len
		>
{
};

template<typename HT, typename TT, std::size_t Pos, std::size_t Len>
struct sub_tuple_t< yggr::tuples::cons<HT, TT>, Pos, Len >
	: public 
		detail::sub_tuple_t_impl
		<
			typename
				conv_to_tuple
				<
					yggr::tuple,
					typename 
						detail::cons_to_vector_impl
						< 
							boost::mpl::vector<>,
							boost::tuples::cons<HT, TT> 
						>::type
				>::type,
			Pos, 
			Len
		>
{
};

} // namespace tuple_ex
} // namespace yggr


namespace yggr
{
namespace tuple_ex
{
namespace detail
{

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename DstTuple, typename SrcTuple, 
			std::size_t SrcSubPos, 
			std::size_t SrcSubLen = tuple_size<SrcTuple>::type::value - SrcSubPos,
			bool ArgsValidate = (SrcSubPos + SrcSubLen <= tuple_size<SrcTuple>::type::value) >
struct sub_tuple_impl_helper
{
public:
	typedef DstTuple dst_tuple_type;
	typedef SrcTuple src_tuple_type;

private:
	typedef sub_tuple_impl_helper this_type;

private:
	template<typename TDstTuple, std::size_t ...Idx> inline
	TDstTuple prv_make_sub_tuple(const src_tuple_type& src, yggr::index_sequence< Idx... >) const
	{
		typedef TDstTuple result_type;
		typedef tuple_maker<result_type> tuple_maker_type;
		tuple_maker_type maker;
		return maker.make_tuple(yggr::get<(SrcSubPos + Idx)>(src)...);
	}

public:
	inline dst_tuple_type operator()(const src_tuple_type& src) const
	{
		return 
			this_type::template prv_make_sub_tuple<dst_tuple_type>(
				src, yggr::make_index_sequence<SrcSubLen>());
	}
};

#else

template<typename DstTuple, typename SrcTuple, std::size_t SrcSubPos, std::size_t SrcSubLen>
struct sub_tuple_impl_helper_variadic;

// _YGGR_TMP_PP_TUPLE_GET_SUB_OP(__n__, __val__ )
// _YGGR_TMP_PP_TUPLE_GET_SUB_POS ()
// _YGGR_TMP_PP_TUPLE_GET_VALUE ()

#	define _YGGR_TMP_PP_TUPLE_GET_ARGS_OP( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			_YGGR_TMP_PP_TUPLE_GET_SUB_OP( __n__, YGGR_PP_OPEN( _YGGR_TMP_PP_TUPLE_GET_VALUE ) )

#	define  _YGGR_TMP_PP_TUPLE_GET_SUB_POS() SrcSubPos

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename DstTuple, typename SrcTuple, std::size_t YGGR_PP_OPEN( _YGGR_TMP_PP_TUPLE_GET_SUB_POS ) > \
		struct sub_tuple_impl_helper_variadic<DstTuple, SrcTuple, YGGR_PP_OPEN( _YGGR_TMP_PP_TUPLE_GET_SUB_POS ) , __n__ > { \
		public: \
			typedef DstTuple dst_tuple_type; \
			typedef SrcTuple src_tuple_type; \
		private: \
			typedef tuple_maker<dst_tuple_type> tuple_maker_type; \
		private: \
			typedef sub_tuple_impl_helper_variadic this_type; \
		public: \
			inline dst_tuple_type operator()(const src_tuple_type& YGGR_PP_OPEN( _YGGR_TMP_PP_TUPLE_GET_VALUE ) ) const { \
				tuple_maker_type maker; \
				return maker.make_tuple(YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA )); } };

#	define _YGGR_TMP_PP_TUPLE_GET_SUB_OP( __n__, __val__) \
		( yggr::get<( YGGR_PP_OPEN( _YGGR_TMP_PP_TUPLE_GET_SUB_POS ) + __n__ )>( __val__ ) )

#	define _YGGR_TMP_PP_TUPLE_GET_VALUE() src
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_TUPLE_GET_ARGS_OP

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_TUPLE_LIMIT_LENGTH() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef _YGGR_TMP_PP_TUPLE_GET_VALUE
#	undef _YGGR_TMP_PP_TUPLE_GET_SUB_POS
#	undef _YGGR_TMP_PP_TUPLE_GET_SUB_OP
#	undef _YGGR_TMP_PP_TUPLE_GET_ARGS_OP

template<typename DstTuple, typename SrcTuple, 
			std::size_t SrcSubPos, 
			std::size_t SrcSubLen = tuple_size<SrcTuple>::type::value - SrcSubPos,
			bool ArgsValidate = (SrcSubPos + SrcSubLen <= tuple_size<SrcTuple>::type::value) >
struct sub_tuple_impl_helper
{
public:
	typedef DstTuple dst_tuple_type;
	typedef SrcTuple src_tuple_type;

private:
	typedef 
		sub_tuple_impl_helper_variadic
		<
			dst_tuple_type, 
			src_tuple_type, 
			SrcSubPos, 
			SrcSubLen
		> helper_variadic_type;

private:
	typedef sub_tuple_impl_helper this_type;

public:
	inline dst_tuple_type operator()(const src_tuple_type& src) const
	{
		helper_variadic_type variadic;
		return variadic(src);
	}
};

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


template<typename DstTuple, typename SrcTuple, std::size_t SrcSubPos, std::size_t SrcSubLen >
struct sub_tuple_impl_helper<DstTuple, SrcTuple, SrcSubPos, SrcSubLen, false>
{
public:
	typedef DstTuple dst_tuple_type;
	typedef SrcTuple stc_tuple_type;

public:
	inline dst_tuple_type operator()(const stc_tuple_type& src_tuple) const
	{
		return dst_tuple_type();
	}
};

template<std::size_t SubPos, std::size_t SubLen, typename SrcTuple> inline
typename sub_tuple_t<SrcTuple, SubPos, SubLen>::type 
	sub_tuple_impl(const SrcTuple& src)
{
	typedef SrcTuple src_tuple_type;
	typedef typename sub_tuple_t<src_tuple_type, SubPos, SubLen>::type dst_tuple_type;

	typedef 
		sub_tuple_impl_helper
		<
			dst_tuple_type,
			src_tuple_type,
			SubPos,
			boost::mpl::min
			<
				boost::mpl::size_t<(tuple_size<src_tuple_type>::type::value - SubPos)>,
				boost::mpl::size_t<SubLen>
			>::type::value
		> helper_type;

	helper_type helper;

	return helper(src);
}

} // namespace detail


template<std::size_t SubPos, std::size_t SubLen, typename SrcTuple> inline
typename sub_tuple_t<SrcTuple, SubPos, SubLen>::type 
	sub_tuple(const SrcTuple& src)
{
	return detail::sub_tuple_impl<SubPos, SubLen>(src);
}

} // namespace tuple_ex
} // namespace yggr

#endif // __YGGR_TUPLE_EX_DETAIL_TUPLE_SUBSTRUCTURE_HPP__