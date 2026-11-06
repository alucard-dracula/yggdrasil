// tuple_maker.hpp

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

#ifndef __YGGR_TUPLE_EX_TUPLE_MAKER_HPP__
#define __YGGR_TUPLE_EX_TUPLE_MAKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/tuple/tuple.hpp>
#include <yggr/move/move.hpp>

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
#	include <tuple>
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

#if defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
#	include <yggr/ppex/foo_params.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <yggr/type_traits/remove_cv_ref.hpp>


namespace yggr
{
namespace tuple_ex
{
namespace detail
{

template<typename Tuple>
struct tuple_maker_impl;

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	if !defined(YGGR_NO_CXX11_HDR_TUPLE)
template<typename ...TArgs>
struct tuple_maker_impl< std::tuple< TArgs... > >
{
public:
	template<typename ...Args> inline
	std::tuple <typename yggr::remove_cv_ref<Args>::type... >
		make_tuple(BOOST_FWD_REF(Args) ...args) const
	{
		return std::make_tuple(boost::forward<Args>(args)...);
	}
};
#	endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

template<typename ...TArgs>
struct tuple_maker_impl< boost::tuple< TArgs... > >
{
public:
	template<typename ...Args> inline
	boost::tuple <typename yggr::remove_cv_ref<Args>::type... > 
		make_tuple(BOOST_FWD_REF(Args) ...args) const
	{
		return boost::make_tuple(boost::forward<Args>(args)...);
	}
};

template<typename ...TArgs>
struct tuple_maker_impl< yggr::tuple< TArgs... > >
{
public:
	template<typename ...Args> inline
	yggr::tuple <typename yggr::remove_cv_ref<Args>::type... > 
		make_tuple(BOOST_FWD_REF(Args) ...args) const
	{
		return yggr::make_tuple(boost::forward<Args>(args)...);
	}
};

#else

// _YGGR_TMP_PP_MAKE_TUPLE_SUB_OP(__n__, __val__ )
// _YGGR_TMP_PP_MAKE_TUPLE_VALUE ()

#	define _YGGR_TMP_PP_MAKE_TUPLE_ARGS_OP( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			_YGGR_TMP_PP_MAKE_TUPLE_SUB_OP( __n__, YGGR_PP_OPEN( _YGGR_TMP_PP_MAKE_TUPLE_VALUE ) )

#	define _YGGR_TMP_PP_MAKE_TUPLE_SUB_OP( __n__, __val__) \
		typename yggr::remove_cv_ref< BOOST_PP_CAT( __val__, __n__ ) >::type


#	define YGGR_PP_TEMPLATE_DEFAULT_TYPE() ::yggr::tuples::null_type
template<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename Val )>
#	undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct tuple_maker_impl< boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), Val) > >
{
public:
	inline boost::tuple<> make_tuple(void) const
	{
		return boost::make_tuple();
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		boost::tuple< YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) > \
			make_tuple( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_FWDREF_PARAMS ) ) { \
			return boost::make_tuple( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } 

#	define _YGGR_TMP_PP_MAKE_TUPLE_VALUE() T
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAKE_TUPLE_ARGS_OP
#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_TUPLE_LIMIT_LENGTH() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef _YGGR_TMP_PP_MAKE_TUPLE_VALUE

};

#	define YGGR_PP_TEMPLATE_DEFAULT_TYPE() ::yggr::tuples::null_type
template<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename Val )>
#	undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct tuple_maker_impl< yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), Val) > >
{
public:
	inline yggr::tuple<> make_tuple(void) const
	{
		return yggr::make_tuple();
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		yggr::tuple< YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) > \
			make_tuple( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_FWDREF_PARAMS ) ) { \
			return yggr::make_tuple( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } 

#	define _YGGR_TMP_PP_MAKE_TUPLE_VALUE() T
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_MAKE_TUPLE_ARGS_OP
#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_TUPLE_LIMIT_LENGTH() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef _YGGR_TMP_PP_MAKE_TUPLE_VALUE

};

#	undef _YGGR_TMP_PP_MAKE_TUPLE_SUB_OP
#	undef _YGGR_TMP_PP_MAKE_TUPLE_ARGS_OP

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace detail

template<typename Tuple>
struct tuple_maker
	: public detail::tuple_maker_impl<Tuple>
{
};

} // namespace tuple_ex
} // namespace yggr

#endif // __YGGR_TUPLE_EX_TUPLE_MAKER_HPP__
