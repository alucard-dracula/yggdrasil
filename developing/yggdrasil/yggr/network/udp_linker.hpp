//udp_linker.h

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

#ifndef __YGGR_NETWORK_UDP_LINKER_HPP__
#define __YGGR_NETWORK_UDP_LINKER_HPP__

#include <vector>
#include <sstream>
#include <utility>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/smart_ptr/enable_shared_from_raw.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/network/type_traits/tags.hpp>


namespace yggr
{
namespace network
{
template<typename Config>
class udp_linker 
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
	BOOST_MPL_ASSERT((boost::is_same<protocol_tag_type, yggr::network::type_traits::tag_udp>));

	typedef typename config_type::service_type service_type;
	typedef typename config_type::socket_type socket_type;
	typedef socket_type link_init_type;
	typedef typename config_type::endpoint_type endpoint_type;
	
	typedef typename config_type::address_type link_address_type;

	typedef typename config_type::protocol_type protocol_type;

	typedef class _link_c
	{
	public:
		_link_c(socket_type& s)
			: _socket(s)
		{
		}

		_link_c(socket_type& s, const endpoint_type& ep)
			: _socket(s), _ep(ep)
		{
		}

		_link_c(const _link_c& right)
			: _socket(right._socket), _ep(right._ep)
		{
		}

		~_link_c(void)
		{
		}

		socket_type& socket(void)
		{
			return _socket;
		}

		const socket_type& socket(void) const
		{
			return _socket;
		}

		endpoint_type& endpoint(void)
		{
			return _ep;
		}

		const endpoint_type& endpoint(void) const
		{
			return _ep;
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
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
				catch(const compatibility::stl_exception& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
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
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
				catch(const compatibility::stl_exception& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
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
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
				catch(const compatibility::stl_exception& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
			}
		}

		void close(void)
		{
			if(_socket.is_open())
			{
				try
				{
					_socket.shutdown(socket_type::shutdown_both);
				}
				catch(const boost::system::system_error& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
				catch(const compatibility::stl_exception& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
						
				try
				{
					_socket.close();
				}
				catch(const boost::system::system_error& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
				catch(const compatibility::stl_exception& e)
				{
					exception::exception::throw_error(e, (u32)system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
				}
			}
		}

		socket_type& _socket;
		endpoint_type _ep;

	} link_type;

public:
	udp_linker(link_init_type& init)
		: _link(init)
	{
		std::cout << this << " create" << std::endl;
	}

	~udp_linker(void)
	{
		std::cout << this << " distory" << std::endl;
	}

	static bool is_v4(void)
	{
		return config_type::is_v4();
	}

	static bool is_v6(void)
	{
		return config_type::is_v6();
	}

	void connect(const endpoint_type& ep)
	{
		_link.endpoint() = ep;
	}

	void shut_down_all(void)
	{
		_link.shut_down_all();
	}

	void shut_down_send(void)
	{
		_link.shut_down_send();
	}

	void shut_down_recv(void)
	{
		_link.shut_down_recv();
	}

	void close(void)
	{
		_link.close();
	}

	bool is_open(void) const
	{
		//return true;
		return _link.socket().is_open();
	}

	endpoint_type local_endpoint(void) const
	{
		return _link.socket().local_endpoint();
	}

	link_address_type local_address(void) const
	{
		return _link.socket().local_endpoint().address();
	}

	yggr::u16 local_port(void) const
	{
		return _link.socket().local_endpoint().port();
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
		return _link.endpoint();
	}

	link_address_type remote_address(void) const
	{
		return _link.endpoint().address();
	}

	yggr::u16 remote_port(void) const
	{
		return _link.endpoint().port();
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
		return _link;
	}

	service_type& get_service(void)
	{
		return reinterpret_cast<service_type&>(_link.socket().get_io_service());
	}

	link_init_type& get_init_object(void)
	{
		return _link.socket();
	}
private:
	link_type _link;
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_UDP_LINKER_HPP__
