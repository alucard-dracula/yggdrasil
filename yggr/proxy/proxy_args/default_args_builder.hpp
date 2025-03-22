//default_args_builder.hpp

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

#ifndef __YGGR_PROXY_PROXY_ARGS_DEFAULT_ARGS_BUILDER_HPP__
#define __YGGR_PROXY_PROXY_ARGS_DEFAULT_ARGS_BUILDER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace proxy
{

namespace proxy_args
{

template<typename OwnerInfo,
			typename ProxyData>
struct default_args_builder
{
public:
	typedef OwnerInfo owner_info_type;
	typedef ProxyData proxy_data_type;

private:
	typedef default_args_builder this_type;

public:
	default_args_builder(void)
	{
	}

	~default_args_builder(void)
	{
	}

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Tag, typename ...Args> inline
	bool modify_rule(BOOST_FWD_REF(Args)... args)
	{
		assert(false);
		return false;
	}

	template<typename Tag, typename ...Args> inline
	bool check_rule(BOOST_FWD_REF(Args)... args) const
	{
		assert(false);
		return false;
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< \
		typename Tag \
		YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
		YGGR_PP_FOO_TYPES_DEF( __n__ )  > inline \
	bool modify_rule( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		assert(false); \
		return false; } \
	\
	template< \
		typename Tag \
		YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
		YGGR_PP_FOO_TYPES_DEF( __n__ )  > inline \
	bool check_rule( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		assert(false); \
		return false; }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename RouterPtr, typename Runner, typename Handler> inline
	void route(RouterPtr prouter, 
				const owner_info_type& owner,
				const proxy_data_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type) const
	{
		if(prouter)
		{
			prouter->route(owner, proxy_data, prunner, handler, cal_type);
		}
	}

	template<typename RouterPtr, typename Runner, typename Handler, typename Args> inline
	void route(RouterPtr prouter, 
				const owner_info_type& owner,
				const proxy_data_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type,
				Args& args) const
	{
		if(prouter)
		{
			prouter->route(owner, proxy_data, prunner, handler, cal_type);
		}
	}

};

} // namespace proxy_args

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_ARGS_DEFAULT_ARGS_BUILDER_HPP__
