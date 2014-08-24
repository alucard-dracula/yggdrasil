//tcp_linker.hpp

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

#ifndef __YGGR_NETWORK_TCP_LINKER_HPP__
#define __YGGR_NETWORK_TCP_LINKER_HPP__

#include <sstream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/smart_ptr/enable_shared_from_raw.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace network
{

template<typename Config>
class tcp_linker
	: public boost::enable_shared_from_raw,
		private nonable::noncopyable
{
public:

	typedef Config config_type;
	enum
	{
		E_NOW_IP_VERSION = config_type::E_NOW_IP_VERSION,
		E_compile_u32 = 0xffffffff
	};

	typedef typename config_type::protocol_tag_type protocol_tag_type;
	BOOST_MPL_ASSERT((boost::is_same<protocol_tag_type, yggr::network::type_traits::tag_tcp>));

	typedef typename config_type::service_type service_type;
	typedef service_type link_init_type;
	typedef typename config_type::socket_type socket_type;
	typedef typename config_type::address_type link_address_type;
	typedef socket_type link_type;
	typedef typename config_type::endpoint_type endpoint_type;

	typedef typename config_type::protocol_type protocol_type;

private:
	typedef tcp_linker this_type;

public:
	tcp_linker(link_init_type& init)
		: _socket(init)
	{
		std::cout << this << " create" << std::endl;
	}

	~tcp_linker(void)
	{
		std::cout << this << " distory" << std::endl;
		assert(!this_type::is_open());
	}

	template<typename Tuple_Handler>
	void connect(const Tuple_Handler& handler)
	{
		if(!_socket.is_open())
		{
			if(is_v4())
			{
				_socket.open(protocol_type::v4());
			}

			if(is_v6())
			{
				_socket.open(protocol_type::v6());
			}
		}
		_socket.async_connect(boost::get<0>(handler), boost::get<1>(handler));
	}

	void shut_down_all(void)
	{
		if(_socket.is_open())
		{
			try
			{
				_socket.shutdown(socket_type::shutdown_both);
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
		}
	}

	void shut_down_send(void)
	{
		if(_socket.is_open())
		{
			try
			{
				_socket.shutdown(socket_type::shutdown_send);
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
		}
	}

	void shut_down_recv(void)
	{
		if(_socket.is_open())
		{
			try
			{
				_socket.shutdown(socket_type::shutdown_receive);
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
		}
	}

	void close(void)
	{
		if(_socket.is_open())
		{
			std::cout << this << " close" << std::endl;
			try
			{
				_socket.shutdown(socket_type::shutdown_both);
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}

			try
			{
				_socket.close();
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, (u32)system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}
		}
	}

	static bool is_v4(void)
	{
		return config_type::is_v4();
	}

	static bool is_v6(void)
	{
		return config_type::is_v6();
	}

	bool is_open(void) const
	{
		return _socket.is_open();
	}

	endpoint_type local_endpoint(void) const
	{
		return _socket.local_endpoint();
	}

	link_address_type local_address(void) const
	{
		return _socket.local_endpoint().address();
	}

	yggr::u16 local_port(void) const
	{
		return _socket.local_endpoint().port();
	}

	template<typename Socket_Info>
	Socket_Info local_socket_info(void) const
	{
		return Socket_Info(local_address(), local_port());
	}

	std::string str_local_address(void) const
	{
		return local_address().to_string();
	}

	std::string str_local_port(void) const
	{
		std::stringstream ss;
		ss << local_port();
		return ss.str();
	}

	std::string str_local_socket_info(void) const
	{
		return str_local_address() + ":" + str_local_port();
	}

	endpoint_type remote_endpoint(void) const
	{
		return _socket.remote_endpoint();
	}

	link_address_type remote_address(void) const
	{
		return _socket.remote_endpoint().address();
	}

	yggr::u16 remote_port(void) const
	{
		return _socket.remote_endpoint().port();
	}

	template<typename Socket_Info>
	Socket_Info remote_socket_info(void) const
	{
		return Socket_Info(remote_address(), remote_port());
	}

	std::string str_remote_address(void) const
	{
		return remote_address().to_string();
	}

	std::string str_remote_port(void) const
	{
		std::stringstream ss;
		ss << remote_port();
		return ss.str();
	}

	std::string str_remote_socket_info(void) const
	{
		return str_remote_address() + ":" + str_remote_port();
	}

	link_type& get_link(void)
	{
		return _socket;
	}

	service_type& get_service(void)
	{
		return reinterpret_cast<service_type&>(_socket.get_io_service());
	}

	link_init_type& get_init_object(void)
	{
		return reinterpret_cast<service_type&>(_socket.get_io_service());
	}

private:
	link_type _socket;
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_TCP_LINKER_HPP__
