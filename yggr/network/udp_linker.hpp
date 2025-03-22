//udp_linker.h

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

#ifndef __YGGR_NETWORK_UDP_LINKER_HPP__
#define __YGGR_NETWORK_UDP_LINKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/bind/bind.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/tuple/tuple.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/system_controller/system_code.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/address_converter.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

#include <boost/asio.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/smart_ptr/enable_shared_from_raw.hpp>

#include <boost/function.hpp>
#include <boost/function_equal.hpp>

#include <sstream>
#include <utility>

namespace yggr
{
namespace network
{
namespace detail
{

template<typename Config>
class udp_linker_link_c
{
public:
	typedef Config config_type;

	typedef typename config_type::service_type service_type;
	typedef typename config_type::protocol_type protocol_type;
	typedef typename config_type::socket_type socket_type;
	typedef typename config_type::endpoint_type endpoint_type;
	typedef typename config_type::address_type link_address_type;
	typedef typename config_type::deadline_timer_type deadline_timer_type;

	typedef typename protocol_type::resolver resolver_type;
	typedef typename resolver_type::iterator resolver_iter_type;
	typedef typename resolver_type::query resolver_query_type;

private:
	typedef safe_container::safe_wrap<endpoint_type> endpoint_wrap_type;

private:
	typedef udp_linker_link_c this_type;

public:
	udp_linker_link_c(socket_type& s)
		: _socket(s)
	{
	}

	udp_linker_link_c(socket_type& s, const endpoint_type& ep)
		: _socket(s), _ep_wrap(ep)
	{
	}

	udp_linker_link_c(const this_type& right)
		: _socket(right._socket), _ep_wrap(right._ep_wrap)
	{
	}

	~udp_linker_link_c(void)
	{
	}

public:
	inline socket_type& socket(void) const
	{
		return _socket;
	}

	endpoint_type local_endpoint(void) const
	{
		try
		{
			return _socket.local_endpoint();
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
		}

		return endpoint_type();
	}

	endpoint_type remote_endpoint(void) const
	{
		try
		{
			return _ep_wrap.load();
		}
		catch(const typename endpoint_wrap_type::error_type&)
		{
		}

		return endpoint_type();
	}

	inline const endpoint_type& remote_endpoint(const endpoint_type& ep)
	{
		_ep_wrap = ep;
		return ep;
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
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
		}

		return endpoint_type();
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
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
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
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
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
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
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
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}

			try
			{
				_socket.close();
			}
			catch(const boost::system::system_error& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
			catch(const compatibility::stl_exception& e)
			{
				exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			}
		}
	}

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
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			return false;
		}

		return true;
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
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e, static_cast<u32>(system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM));
			return false;
		}

		return true;
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
	/*mutable*/ socket_type& _socket;
	endpoint_wrap_type _ep_wrap;

};

} // namespace detail
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{

template<typename Config>
class udp_linker
	: public boost::enable_shared_from_raw,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Config config_type;
	YGGR_STATIC_CONSTANT(u32, E_NOW_IP_VERSION = config_type::E_NOW_IP_VERSION);

	typedef typename config_type::protocol_tag_type protocol_tag_type;
	BOOST_MPL_ASSERT((boost::is_same<protocol_tag_type, yggr::network::type_traits::tag_udp>));

	typedef typename config_type::service_type service_type;
	typedef typename config_type::protocol_type protocol_type;
	typedef typename config_type::socket_type socket_type;
	typedef typename config_type::endpoint_type endpoint_type;
	typedef typename config_type::address_type link_address_type;
	typedef typename config_type::deadline_timer_type deadline_timer_type;

	typedef typename protocol_type::resolver resolver_type;
	typedef typename resolver_type::iterator resolver_iter_type;
	typedef typename resolver_type::query resolver_query_type;

	typedef socket_type link_init_type;

	typedef boost::function1<void, socket_type&> socket_destroyer_type;

public:
	typedef detail::udp_linker_link_c<config_type> link_type;

private:
	typedef udp_linker this_type;

public:
	udp_linker(socket_type& init)
		: _link(init),
			_deadline_timer(
#if (BOOST_ASIO_VERSION < 101400)
				init.get_io_service()
#else
				init.get_executor()
#endif //BOOST_ASIO_VERSION < 101400)
				)
	{
#ifdef _DEBUG
		std::cout << this << " create" << std::endl;
#endif // _DEBUG
	}

	template<typename DHandler>
	udp_linker(link_init_type& init, const DHandler& d)
		: _link(init),
			_socket_destroyer(d),
			_deadline_timer(
#if (BOOST_ASIO_VERSION < 101400)
				init.get_io_service()
#else
				init.get_executor()
#endif //BOOST_ASIO_VERSION < 101400)
				)
	{
#ifdef _DEBUG
		std::cout << this << " create of D" << std::endl;
#endif // _DEBUG
	}

	udp_linker(socket_type& s, const endpoint_type& ep)
		: _link(s, ep),
			_deadline_timer(
#if (BOOST_ASIO_VERSION < 101400)
				s.get_io_service()
#else
				s.get_executor()
#endif //BOOST_ASIO_VERSION < 101400)
				)
	{
#ifdef _DEBUG
		std::cout << this << " create of ep" << std::endl;
#endif // _DEBUG
	}

	template<typename DHandler>
	udp_linker(socket_type& s, const endpoint_type& ep, const DHandler& d)
		: _link(s, ep),
		_socket_destroyer(d),
		_deadline_timer(
#if (BOOST_ASIO_VERSION < 101400)
				s.get_io_service()
#else
				s.get_executor()
#endif //BOOST_ASIO_VERSION < 101400)
			)
	{
#ifdef _DEBUG
		std::cout << this << " create of ep D" << std::endl;
#endif // _DEBUG
	}

	~udp_linker(void)
	{
#ifdef _DEBUG
		std::cout << this << " distory" << std::endl;
#endif // _DEBUG

		this_type::timer_cancel();

		if(_socket_destroyer)
		{
			_socket_destroyer(_link.socket());
		}
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
	inline void connect(const endpoint_type& ep)
	{
		_link.remote_endpoint(ep);
	}

	inline void shut_down_all(void)
	{
		_link.shut_down_all();
	}

	inline void shut_down_send(void)
	{
		_link.shut_down_send();
	}

	inline void shut_down_recv(void)
	{
		_link.shut_down_recv();
	}

	template<typename OptionSetter> inline
	bool set_option(const OptionSetter& ops_setter)
	{
		return _link.set_option(ops_setter);
	}

	template<typename OptionGetter> inline
	bool get_option(OptionGetter& ops_getter) const
	{
		return _link.get_option(ops_getter);
	}

	inline void close(void)
	{
		_link.close();
	}

	inline bool is_open(void) const
	{
		return _link.socket().is_open();
	}

	inline endpoint_type local_endpoint(void) const
	{
		return _link.local_endpoint();
	}

	inline link_address_type local_address(void) const
	{
		return _link.local_endpoint().address();
	}

	inline u16 local_port(void) const
	{
		return _link.local_endpoint().port();
	}

	template<typename Socket_Info> inline
	Socket_Info local_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_type ep = _link.local_endpoint();

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

	inline endpoint_type localhost_endpoint(void) const
	{
		return _link.localhost_endpoint();
	}

	inline link_address_type localhost_address(void) const
	{
		return _link.localhost_endpoint().address();
	}

	inline u16 localhost_port(void) const
	{
		return _link.localhost_endpoint().port();
	}

	template<typename Socket_Info> inline
	Socket_Info localhost_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_type ep = _link.localhost_endpoint();

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

	inline endpoint_type remote_endpoint(void) const
	{
		return _link.remote_endpoint();
	}

	inline link_address_type remote_address(void) const
	{
		return _link.remote_endpoint().address();
	}

	inline u16 remote_port(void) const
	{
		return _link.remote_endpoint().port();
	}

	template<typename Socket_Info> inline
	Socket_Info remote_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_type ep(_link.remote_endpoint());

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

	inline bool update_remote_endpoint(const endpoint_type& ep)
	{
		_link.remote_endpoint(ep);
		return true;
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
		return _link;
	}

	inline service_type& get_service(void) const
	{
#if (BOOST_ASIO_VERSION < 101400)
		return static_cast<service_type&>(_link.socket().get_io_service());
#else
		return static_cast<service_type&>(_link.socket().get_executor().context());
#endif // (BOOST_ASIO_VERSION < 101400)
	}

	inline link_init_type& get_init_object(void) const
	{
		return _link.socket();
	}

	inline socket_destroyer_type& get_socket_destroyer(void)
	{
		return _socket_destroyer;
	}

	inline const socket_destroyer_type& get_socket_destroyer(void) const
	{
		return _socket_destroyer;
	}

	inline void clear_socket_destroyer(void)
	{
		socket_destroyer_type tmp;
		_socket_destroyer.swap(tmp);
	}

private:
	mutable link_type _link;
	socket_destroyer_type _socket_destroyer;
	mutable deadline_timer_type _deadline_timer;

};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_UDP_LINKER_HPP__
