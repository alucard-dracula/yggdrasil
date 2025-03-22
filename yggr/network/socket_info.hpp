//socket_info.hpp

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

#ifndef __YGGR_NETWORK_SOCKET_INFO_HPP__
#define __YGGR_NETWORK_SOCKET_INFO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/support/return_arg.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

#include <boost/asio/ip/address.hpp>
#include <boost/ref.hpp>

#include <ostream>
#include <sstream>

namespace yggr
{
namespace network
{

template<typename Host, typename Port>
class socket_info
{
public:
	typedef Host host_type;
	typedef Port port_type;

private:
	typedef socket_info this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_host);
		ar & YGGR_SERIALIZATION_NVP(_port);
	}


private:
	template<typename Ret, typename Arg> inline
	static Ret prv_s_conv(const Arg& arg)
	{
		return arg;
	}

public:
	socket_info(void)
		: _host(), _port()
	{
	}
	
	template<typename OHost, typename OPort>
	socket_info(const OHost& ip, const OPort& port)
		: _host(this_type::prv_s_conv<host_type>(ip)), 
			_port(this_type::prv_s_conv<port_type>(port))
	{
	}

	socket_info(const host_type& host, const port_type& port)
		: _host(host), _port(port)
	{
	}

	socket_info(BOOST_RV_REF(this_type) right)
		: _host(boost::move(right._host)),
			_port(boost::move(right._port))
	{
	}

	socket_info(const this_type& right)
		: _host(right._host), _port(right._port)
	{
	}

	~socket_info(void)
	{
	}

public:
	template<typename OHost, typename OPort>
	this_type& operator=(const socket_info<OHost, OPort>& right)
	{
		_host = right.host();
		_port = right.port();
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_host, boost::move(right._host));
		copy_or_move_or_swap(_port, boost::move(right._port));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		_host = right._host;
		_port = right._port;
		return *this;
	}

public:

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		yggr::swap(_host, right._host);
		yggr::swap(_port, right._port);
	}

public:
	inline host_type& host(void)
	{
		return _host;
	}

	inline const host_type& host(void) const
	{
		return _host;
	}

	inline port_type& port(void)
	{
		return _port;
	}

	inline const port_type& port(void) const
	{
		return _port;
	}

public:
	template<typename OHost, typename OPort> inline
	bool compare_eq(const socket_info<OHost, OPort>& right) const
	{
		return (_host == right.host()) && (_port == right.port());
	}

	inline bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| ((_host == right._host) && (_port == right._port));
	}

	template<typename OHost, typename OPort> inline
	s32 operator<(const socket_info<OHost, OPort>& right) const
	{
		return 
			_host == right.host()?
				_port == right.port()? 
					0
					: _port < right.port()? -1 : 1
				: _host < right.host()? -1 : 1;
	}

	inline s32 compare(const this_type& right) const
	{
		return 
			_host == right._host?
				_port == right._port? 
					0
					: _port < right._port? -1 : 1
				: _host < right._host? -1 : 1;
	}

public:
	inline std::size_t hash(void) const
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, _host);
		boost::hash_combine(seed, _port);
		return seed;
	}

private:
	host_type _host;
	port_type _port;
};

// non-member function

template<typename HostL, typename PortL, typename HostR, typename PortR> inline
bool operator==(const socket_info<HostL, PortL>& l, const socket_info<HostR, PortR>& r)
{
	return l.compare_eq(r);
}

template<typename HostL, typename PortL, typename HostR, typename PortR> inline
bool operator!=(const socket_info<HostL, PortL>& l, const socket_info<HostR, PortR>& r)
{
	return !l.compare_eq(r);
}


template<typename HostL, typename PortL, typename HostR, typename PortR> inline
bool operator<(const socket_info<HostL, PortL>& l, const socket_info<HostR, PortR>& r)
{
	return l.compare(r) < 0;
}

template<typename HostL, typename PortL, typename HostR, typename PortR> inline
bool operator<=(const socket_info<HostL, PortL>& l, const socket_info<HostR, PortR>& r)
{
	return l.compare(r) <= 0;
}

template<typename HostL, typename PortL, typename HostR, typename PortR> inline
bool operator>(const socket_info<HostL, PortL>& l, const socket_info<HostR, PortR>& r)
{
	return l.compare(r) > 0;
}

template<typename HostL, typename PortL, typename HostR, typename PortR> inline
bool operator>=(const socket_info<HostL, PortL>& l, const socket_info<HostR, PortR>& r)
{
	return l.compare(r) >= 0;
}

// operator<<

template<typename Char, typename Traits, typename Host, typename Port>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const socket_info<Host, Port>& val)
{
	os << val.host() << ":" << val.port();
	return os;
}

} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace swap_support
{

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T)> inline 
void swap(socket_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T)>& l, 
			socket_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T)>& r) 
{ 
	l.swap(r); 
}


} // namespace swap_support

using swap_support::swap;

} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::swap_support::swap;
} // namespace boost

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace network
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename Host, typename Port>
std::size_t hash_value(const yggr::network::socket_info<Host, Port>& val)
{
	return val.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace network
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<typename Host, typename Port>
struct hash<yggr::network::socket_info<Host, Port> >
{
	typedef yggr::network::socket_info<Host, Port> type;

	inline std::size_t operator()(const type& val) const
	{
		return val.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif // __YGGR_NETWORK_SOCKET_INFO_HPP__
