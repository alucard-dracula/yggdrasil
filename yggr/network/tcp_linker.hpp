//tcp_linker.hpp

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

#ifndef __YGGR_NETWORK_TCP_LINKER_HPP__
#define __YGGR_NETWORK_TCP_LINKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/bind/bind.hpp>

#include <yggr/tuple/tuple.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/system_controller/system_code.hpp>

#include <yggr/network/type_traits/is_socket_option.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/address_converter.hpp>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/asio.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/smart_ptr/enable_shared_from_raw.hpp>

#include <sstream>

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
	YGGR_STATIC_CONSTANT(u32, E_NOW_IP_VERSION = config_type::E_NOW_IP_VERSION);

	typedef typename config_type::protocol_tag_type protocol_tag_type;
	BOOST_MPL_ASSERT((boost::is_same<protocol_tag_type, yggr::network::type_traits::tag_tcp>));

	typedef typename config_type::service_type service_type;
	typedef typename config_type::protocol_type protocol_type;
	typedef typename config_type::socket_type socket_type;
	typedef typename config_type::endpoint_type endpoint_type;
	typedef typename config_type::address_type link_address_type;
	typedef typename config_type::deadline_timer_type deadline_timer_type;

	typedef typename protocol_type::resolver resolver_type;
	typedef typename resolver_type::iterator resolver_iter_type;
	typedef typename resolver_type::query resolver_query_type;

	typedef service_type link_init_type;
	typedef socket_type link_type;

private:
	typedef tcp_linker this_type;

public:
	tcp_linker(link_init_type& init)
		: _socket(init), _deadline_timer(init)
	{
#ifdef _DEBUG
		std::cout << this << " create" << std::endl;
#endif // _DEBUG
	}

	~tcp_linker(void)
	{
#ifdef _DEBUG
		std::cout << this << " destroy" << std::endl;
#endif // _DEBUG
		this_type::timer_cancel();
		assert(!this_type::is_open());
	}

public:
	template<typename Tuple_Handler> inline
	void connect(const Tuple_Handler& handler)
	{
		if(!_socket.is_open())
		{
			if(this_type::is_v4())
			{
				_socket.open(protocol_type::v4());
			}
			else if(this_type::is_v6())
			{
				_socket.open(protocol_type::v6());
			}
			else
			{
				assert(false);
			}
		}
		_socket.async_connect(yggr::get<0>(handler), yggr::get<1>(handler));
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
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
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
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
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
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			}
		}
	}

	void close(void)
	{
		this_type::timer_cancel();

		if(_socket.is_open())
		{
#ifdef _DEBUG
			std::cout << this << " close" << std::endl;
#endif // _DEBUG
			try
			{
				_socket.shutdown(socket_type::shutdown_both);
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			}

			try
			{
				_socket.close();
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			}
		}
	}

public:
	template<typename SettableSocketOption> inline
	typename 
		boost::enable_if
		<
			::yggr::network::type_traits::is_settable_socket_option<SettableSocketOption>, 
			bool
		>::type
		set_option(const SettableSocketOption& opt)
	{
		try
		{
			_socket.set_option(opt);
			return true;
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			return false;
		}
	}

	template<typename Handler> inline
	typename 
		boost::disable_if
		<
			::yggr::network::type_traits::is_settable_socket_option<Handler>, 
			bool
		>::type
		set_option(const Handler& handler)
	{
		return !!handler(*this);
	}

	template<typename GettableSocketOption> inline
	typename 
		boost::enable_if
		<
			::yggr::network::type_traits::is_gettable_socket_option<GettableSocketOption>, 
			bool
		>::type
		get_option(GettableSocketOption& opt) const
	{
		try
		{
			_socket.get_option(opt);
			return true;
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
			return false;
		}
	}

	template<typename Handler> inline
	typename 
		boost::disable_if
		<
			::yggr::network::type_traits::is_gettable_socket_option<Handler>, 
			bool
		>::type
		get_option(const Handler& handler) const
	{
		return !!handler(*this);
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static bool is_v4(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return config_type::is_v4();
	}

	YGGR_CONSTEXPR_OR_INLINE static bool is_v6(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return config_type::is_v6();
	}

public:
	inline bool is_open(void) const
	{
		return _socket.is_open();
	}

	endpoint_type local_endpoint(void) const
	{
		try
		{
			return _socket.local_endpoint();
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}

		return endpoint_type();
	}

	inline link_address_type local_address(void) const
	{
		return this_type::local_endpoint().address();
	}

	inline u16 local_port(void) const
	{
		return this_type::local_endpoint().port();
	}

	template<typename Socket_Info> inline
	Socket_Info local_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_type ep = this_type::local_endpoint();

		addr_conv_type conv_addr(ep.address());
		port_conv_type conv_port(ep.port());

		return Socket_Info(conv_addr, conv_port);
	}

	inline ::yggr::string str_local_address(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;

		addr_conv_type conv(this_type::local_address());
		return conv;
	}

	template<typename String> inline
	String str_local_address(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;

		addr_conv_type conv(this_type::local_address());
		return conv;
	}

	inline ::yggr::string str_local_port(void) const
	{
		typedef port_converter<u16> port_conv_type;

		port_conv_type conv(this_type::local_port());
		return conv;
	}

	template<typename String> inline
	String str_local_port(void) const
	{
		typedef port_converter<u16> port_conv_type;

		port_conv_type conv(this_type::local_port());
		return conv;
	}

	inline ::yggr::string str_local_socket_info(void) const
	{
		typedef endpoint_converter<endpoint_type> conv_type;

		conv_type conv(this_type::local_endpoint());
		return conv;
	}

	template<typename String> inline 
	String str_local_socket_info(void) const
	{
		typedef endpoint_converter<endpoint_type> conv_type;

		conv_type conv(this_type::local_endpoint());
		return conv;
	}

	endpoint_type localhost_endpoint(void) const
	{
		try
		{
			endpoint_type lep = _socket.local_endpoint();

#if (BOOST_ASIO_VERSION < 101400) 
			resolver_type resolver(_socket.get_io_service());
#else
			resolver_type resolver(_socket.get_executor());
#endif // (BOOST_ASIO_VERSION < 101400) 
			resolver_query_type query(boost::asio::ip::host_name(), "");
			resolver_iter_type iter = resolver.resolve(query);
			
			for(resolver_iter_type i = iter, isize = resolver_iter_type(); i != isize; ++i)
			{
				endpoint_type ret(boost::move(*i));

				if((ret.address().is_v4() && lep.address().is_v4())
					|| (ret.address().is_v6() && lep.address().is_v6()) )
				{
					ret.port(lep.port());
					return ret;
				}
			}

			return endpoint_type();
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}

		return endpoint_type();
	}

	inline link_address_type localhost_address(void) const
	{
		return this_type::localhost_endpoint().address();
	}

	inline u16 localhost_port(void) const
	{
		return this_type::localhost_endpoint().port();
	}

	template<typename Socket_Info> inline
	Socket_Info localhost_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_type ep = this_type::localhost_endpoint();

		addr_conv_type conv_addr(ep.address());
		port_conv_type conv_port(ep.port());

		return Socket_Info(conv_addr, conv_port);
	}

	inline ::yggr::string str_localhost_address(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;

		addr_conv_type conv(this_type::localhost_address());
		return conv;
	}

	template<typename String> inline
	String str_localhost_address(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;

		addr_conv_type conv(this_type::localhost_address());
		return conv;
	}

	inline ::yggr::string str_localhost_port(void) const
	{
		typedef port_converter<u16> port_conv_type;

		port_conv_type conv(this_type::localhost_port());
		return conv;
	}

	template<typename String> inline
	String str_localhost_port(void) const
	{
		typedef port_converter<u16> port_conv_type;

		port_conv_type conv(this_type::localhost_port());
		return conv;
	}

	inline ::yggr::string str_localhost_socket_info(void) const
	{
		typedef endpoint_converter<endpoint_type> conv_type;

		conv_type conv(this_type::localhost_endpoint());
		return conv;
	}

	template<typename String> inline
	String str_localhost_socket_info(void) const
	{
		typedef endpoint_converter<endpoint_type> conv_type;

		conv_type conv(this_type::localhost_endpoint());
		return conv;
	}

	endpoint_type remote_endpoint(void) const
	{
		try
		{
			return _socket.remote_endpoint();
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}

		return endpoint_type();
	}

	inline link_address_type remote_address(void) const
	{
		return this_type::remote_endpoint().address();
	}

	inline u16 remote_port(void) const
	{
		return this_type::remote_endpoint().port();
	}

	template<typename Socket_Info> inline
	Socket_Info remote_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_type ep = this_type::remote_endpoint();

		addr_conv_type conv_addr(ep.address());
		port_conv_type conv_port(ep.port());

		return Socket_Info(conv_addr, conv_port);
	}

	inline ::yggr::string str_remote_address(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;

		addr_conv_type conv(this_type::remote_address());
		return conv;
	}

	template<typename String> inline
	String str_remote_address(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;

		addr_conv_type conv(this_type::remote_address());
		return conv;
	}

	inline ::yggr::string str_remote_port(void) const
	{
		typedef port_converter<u16> port_conv_type;

		port_conv_type conv(this_type::remote_port());
		return conv;
	}

	template<typename String> inline
	String str_remote_port(void) const
	{
		typedef port_converter<u16> port_conv_type;

		port_conv_type conv(this_type::remote_port());
		return conv;
	}

	inline ::yggr::string str_remote_socket_info(void) const
	{
		typedef endpoint_converter<endpoint_type> conv_type;

		conv_type conv(this_type::remote_endpoint());
		return conv;
	}

	template<typename String> inline
	String str_remote_socket_info(void) const
	{
		typedef endpoint_converter<endpoint_type> conv_type;

		conv_type conv(this_type::remote_endpoint());
		return conv;
	}

	inline bool update_remote_endpoint(const endpoint_type&)
	{
		return false;
	}

public:
	template<typename Time, typename Handler> inline
	std::size_t expires_at_timer_wait(const Time& tm, const Handler& handler)
	{
		try
		{
			std::size_t ret = _deadline_timer.expires_at(tm);
			_deadline_timer.async_wait(handler);
			return ret;
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		
		return 0;
	}

	template<typename Time, typename Handler> inline
	std::size_t expires_from_now_timer_wait(const Time& tm, const Handler& handler)
	{
		try
		{
			std::size_t ret = _deadline_timer.expires_from_now(tm);
			_deadline_timer.async_wait(handler);
			return ret;
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		
		return 0;
	}

	inline std::size_t timer_cancel(void)
	{
		try
		{
			return _deadline_timer.cancel();
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}

		return 0;
	}

	inline std::size_t timer_finish(void)
	{
		try
		{
			return _deadline_timer.expires_at(boost::posix_time::pos_infin);
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM));
		}

		return 0;
	}

	inline deadline_timer_type& deadline_timer(void) const
	{
		return _deadline_timer;
	}

public:
	inline link_type& get_link(void) const
	{
		return _socket;
	}

	inline service_type& get_service(void) const
	{
#if (BOOST_ASIO_VERSION < 101400) 
		return static_cast<service_type&>(_socket.get_io_service());
#else
		return static_cast<service_type&>(_socket.get_executor().context());
#endif // (BOOST_ASIO_VERSION < 101400) 
	}

	inline link_init_type& get_init_object(void) const
	{
		return this_type::get_service();
	}

private:
	mutable link_type _socket;
	mutable deadline_timer_type _deadline_timer;
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_TCP_LINKER_HPP__
