//ex_linker.hpp

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

#ifndef __YGGR_NETWORK_EX_LINKER_HPP__
#define __YGGR_NETWORK_EX_LINKER_HPP__

#include <yggr/ppex/foo_params.hpp>

#include <yggr/network/tcp_linker.hpp>
#include <yggr/network/udp_linker.hpp>

namespace yggr
{
namespace network
{

template<typename Config,
			template<typename _Config> class Linker = tcp_linker>
class ex_linker
	: public Linker<Config>
{
public:
	typedef Config config_type;
	typedef Linker<config_type> base_type;

	typedef typename base_type::protocol_tag_type protocol_tag_type;
	typedef typename base_type::service_type service_type;
	typedef typename base_type::protocol_type protocol_type;
	typedef typename base_type::socket_type socket_type;
	typedef typename base_type::link_address_type link_address_type;
	typedef typename base_type::deadline_timer_type deadline_timer_type;

	typedef typename base_type::link_init_type link_init_type;
	typedef typename base_type::link_type link_type;

private:
	typedef ex_linker this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	ex_linker(link_init_type& init, BOOST_FWD_REF(Args)... args)
		: base_type(init, boost::forward<Args>(args)...)
	{
		base_type::get_service().ex();
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, > ) \
		ex_linker(link_init_type& init \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: base_type(init \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )){ \
			base_type::get_service().ex(); } 

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~ex_linker(void)
	{
	}

public:
	inline void close(void)
	{
		base_type::get_service().unex();
		base_type::close();
	}

	
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_EX_LINKER_HPP__
