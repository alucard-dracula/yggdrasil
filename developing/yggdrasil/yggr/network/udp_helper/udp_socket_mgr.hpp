//udp_socket_mgr.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_NETWORK_UDP_CONNECTION_MGR_HPP__
#define __YGGR_NETWORK_UDP_CONNECTION_MGR_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/shared_ptr.hpp>

#include <yggr/mplex/get_container_value_t.hpp>
#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace network
{
namespace udp_helper
{

template<typename Socket_Container>
class udp_socket_mgr
	: protected Socket_Container
{
public:
	typedef Socket_Container base_type;
	typedef typename base_type::socket_type socket_type;
	typedef typename base_type::socket_ptr_type socket_ptr_type;
	typedef typename base_type::service_type service_type;
	//typedef typename base_type::ports_type ports_type;

private:
	typedef udp_socket_mgr this_type;

public:

//#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
//	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
//	udp_socket_mgr( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) \
//		: base_type(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )) {}
//
//#	define YGGR_PP_FOO_ARG_NAME() init_arg
//#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
//#	include BOOST_PP_LOCAL_ITERATE(  )
//#	undef YGGR_PP_FOO_ARG_NAME

	udp_socket_mgr(void)
	{
	}

	~udp_socket_mgr(void)
	{
	}

	bool empty(void) const
	{
		return base_type::empty();
	}

	socket_ptr_type get(service_type& s, u16 port = 0)
	{
		return base_type::get(s, port);
	}

	void back(const socket_ptr_type& ptr)
	{
		return base_type::back(ptr);
	}

	void close(void)
	{
		base_type::close();
	}
};

} // namespace udp_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_UDP_CONNECTION_MGR_HPP__