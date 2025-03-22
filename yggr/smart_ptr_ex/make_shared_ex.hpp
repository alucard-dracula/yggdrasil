// make_shared_ex.hpp

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

#ifndef __YGGR_SMART_PTR_EX_MAKE_SHARED_EX_HPP__
#define __YGGR_SMART_PTR_EX_MAKE_SHARED_EX_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/smart_ptr_ex/detail/make_shared_ex_boost.hpp>
#include <yggr/smart_ptr_ex/detail/make_shared_ex_stl.hpp>

namespace yggr
{
namespace smart_ptr_ex
{

using detail::make_shared_boost;

#ifndef YGGR_NO_CXX11_SMART_PTR
	using detail::make_shared_stl;
#endif // YGGR_NO_CXX11_SMART_PTR

#if !defined(YGGR_NO_CXX11_SMART_PTR) && defined(YGGR_SMART_PTR_USING_STL)
// stl version
#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename ...Args> inline
std::shared_ptr<T> make_shared(BOOST_FWD_REF(Args)... args)
{
	return make_shared_stl<T>(boost::forward<Args>(args)...);
}

#	else

#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
			template<typename T \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_TYPES_DEF( __n__ )> inline \
			std::shared_ptr<T> make_shared(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
				return make_shared_stl<T>(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#		define YGGR_PP_FOO_ARG_NAME() init_arg
#		define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#		include BOOST_PP_LOCAL_ITERATE(  )
#		undef YGGR_PP_FOO_ARG_NAME

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES
#else
// boost version
#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename ...Args> inline
boost::shared_ptr<T> make_shared(BOOST_FWD_REF(Args)... args)
{
	return make_shared_boost<T>(boost::forward<Args>(args)...);
}

#	else

#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
			template<typename T \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_TYPES_DEF( __n__ )> inline \
			boost::shared_ptr<T> make_shared(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
				return make_shared_boost<T>(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#		define YGGR_PP_FOO_ARG_NAME() init_arg
#		define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#		include BOOST_PP_LOCAL_ITERATE(  )
#		undef YGGR_PP_FOO_ARG_NAME

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#endif // YGGR_MAKE_SHARED_DEFAULT_USING_STL

} // namespace smart_ptr_ex

using smart_ptr_ex::make_shared;

} // namespace yggr

#endif // __YGGR_SMART_PTR_EX_MAKE_SHARED_EX_HPP__

