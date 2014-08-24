//ex_linker.hpp

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

#ifndef __YGGR_NETWORK_EX_LINKER_HPP__
#define __YGGR_NETWORK_EX_LINKER_HPP__

#include <yggr/network/tcp_linker.hpp>
#include <yggr/network/udp_linker.hpp>

namespace yggr
{
namespace network
{

template<typename Config,
			template<typename _Config> 
				class Linker = tcp_linker>
class ex_linker
	: public Linker<Config>
{
public:

	typedef Config config_type;
	typedef Linker<config_type> base_type;
	typedef typename base_type::protocol_tag_type protocol_tag_type;
	typedef typename base_type::service_type service_type;
	typedef typename base_type::link_init_type link_init_type;
	typedef typename base_type::link_type link_type;
	typedef typename base_type::link_address_type link_address_type;
	typedef typename base_type::socket_type socket_type;

	typedef typename base_type::protocol_type protocol_type;

private:
	typedef ex_linker this_type;

public:
	ex_linker(link_init_type& init)
		: base_type(init)
	{
		base_type::get_service().ex();
	}

	void close(void)
	{
		base_type::get_service().unex();
		base_type::close();
	}

	~ex_linker(void)
	{
	}
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_EX_LINKER_HPP__
