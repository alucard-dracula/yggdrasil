// wrapper_emplace_helper.hpp

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

#ifndef __YGGR_CONTAINER_WRAPPER_EMPLACE_HELPER_HPP__
#define __YGGR_CONTAINER_WRAPPER_EMPLACE_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/detail/has_emplace.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Cont,
			bool has_emplace_back = has_any_emplace_back<Cont>::value>
struct wrapper_emplace_helper;

template<typename Cont>
struct wrapper_emplace_helper<Cont, true>
{
	typedef Cont cont_type;
	typedef typename cont_type::value_type value_type;


#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename ...Args> inline
	void operator()(cont_type& cont, BOOST_FWD_REF(Args)... args) const
	{
		cont.emplace_back(boost::forward<Args>(args)...);
	}

#else

	// compatibility msvc100 deque emplace_back
	inline void operator()(cont_type& cont) const
	{
		cont.push_back(value_type());
	}

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void operator()(cont_type& cont \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) const { \
			cont.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void operator()(cont_type& cont \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) const { \
			cont.push_back(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

};

template<typename Cont>
struct wrapper_emplace_helper<Cont, false>
{
	typedef Cont cont_type;
	typedef typename cont_type::value_type value_type;

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename ...Args> inline
	void operator()(cont_type& cont, BOOST_FWD_REF(Args)... args) const
	{
		cont.push_back(value_type(boost::forward<Args>(args)...));
	}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void operator()(cont_type& cont \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) const { \
			cont.push_back(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_WRAPPER_EMPLACE_HELPER_HPP__
