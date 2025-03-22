// make_interface.hpp

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

#ifndef __YGGR_SMART_PTR_EX_DETAIL_MAKE_INTERFACE_HPP__
#define __YGGR_SMART_PTR_EX_DETAIL_MAKE_INTERFACE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#ifdef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	include <yggr/ppex/foo_params.hpp>
#	include <yggr/ppex/symbols.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <yggr/smart_ptr_ex/detail/interface_ptr.hpp>

namespace yggr
{
namespace smart_ptr_ex
{
namespace detail
{

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename T, typename Derived,
			template<typename _T> class SPtr,
			typename ...Args> inline
interface_ptr<T, SPtr> make_interface(BOOST_FWD_REF(Args)... args)
{
	typedef interface_ptr<T, SPtr> ret_type;
	typedef Derived derived_type;

	try
	{
		return ret_type(yggr_nothrow_new derived_type(boost::forward<Args>(args)...));
	}
	catch(...)
	{
		return ret_type();
	}
}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename T, typename Derived, \
					template<typename _T> class SPtr \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF( __n__ )> inline \
		interface_ptr<T, SPtr> make_interface(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			typedef interface_ptr<T, SPtr> ret_type; \
			typedef Derived derived_type; \
			try { return ret_type(yggr_nothrow_new derived_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
			catch(...) { return ret_type(); } }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME


#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace detail
} // namespace smpart_ptr_ex
} // namespace yggr

#endif // __YGGR_SMART_PTR_EX_DETAIL_MAKE_INTERFACE_HPP__
