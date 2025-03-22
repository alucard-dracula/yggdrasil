//server.hpp

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

#ifndef __YGGR_NETWORK_SERVER_HPP__
#define __YGGR_NETWORK_SERVER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace server
{

template< typename Handler >
class server : public Handler
{
public:
	typedef Handler base_type;

private:
	typedef server this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	
	template<typename ...Args>
	explicit server(BOOST_FWD_REF(Args) ...args)
		: base_type(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		explicit server(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) \
			: base_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)) {}

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~server(void)
	{
	}

public:
	using base_type::join;
	using base_type::run;
	using base_type::stop;
};

} // namespace server
} // namespace yggr

#endif //__YGGR_NETWORK_SERVER_HPP__
