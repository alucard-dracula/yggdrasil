//socket_info.hpp

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

#ifndef __YGGR_NETWORK_SOCKET_INFO_HPP__
#define __YGGR_NETWORK_SOCKET_INFO_HPP__

#include <yggr/move/move.hpp>
#include <yggr/ppex/symbols.hpp>

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

public:
	socket_info(void)
		: _host(), _port()
	{
	}

	socket_info(const host_type& ip, const port_type& port)
		: _host(ip), _port(port)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	socket_info(BOOST_RV_REF(this_type) right)
		: _host(boost::forward<host_type>(right._host)),
			_port(boost::forward<port_type>(right._port))
	{
	}
#else
	socket_info(BOOST_RV_REF(this_type) right)
		: _host(), _port()
	{
		this_type& right_ref = right;
		std::swap(_host, right_ref._host);
		std::swap(_port, right_ref._port);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	socket_info(const this_type& right)
		: _host(right._host), _port(right._port)
	{
	}

	~socket_info(void)
	{
	}

	host_type& host(void)
	{
		return _host;
	}

	const host_type& host(void) const
	{
		return _host;
	}

	port_type& port(void)
	{
		return _port;
	}

	const port_type& port(void) const
	{
		return _port;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_host = boost::forward<host_type>(right._host);
		_port = boost::forward<port_type>(right._port);
#else
		this_type& right_ref = right;
		std::swap(_host, right_ref._host);
		std::swap(_port, right_ref._port);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_host = right._host;
		_port = right._port;
		return *this;
	}

	template<typename OHost, typename OPort>
	this_type& operator=(const socket_info<OHost, OPort>& right)
	{
		_host = right.host();
		_port = right.port();
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_host = boost::forward<host_type>(right._host);
		_port = boost::forward<port_type>(right._port);
#else
		this_type& right_ref = right;
		std::swap(_host, right_ref._host);
		std::swap(_port, right_ref._port);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_host, right._host);
		std::swap(_port, right._port);
	}

	bool operator==(const this_type& right) const
	{
		return (_host == right._host) && (_port == right._port);
	}

	template<typename OHost, typename OPort>
	bool operator==(const socket_info<OHost, OPort>& right) const
	{
		return (_host == right.host()) && (_port == right.port());
	}

	bool operator!=(const this_type& right) const
	{
		return !operator==(right);
	}

	template<typename OHost, typename OPort>
	bool operator!=(const socket_info<OHost, OPort>& right)
	{
		return !operator==(right);
	}

	bool operator<(const this_type& right) const
	{
		return (_host == right._host && _port < right._port) || _host < right._host;
	}

	template<typename OHost, typename OPort>
	bool operator<(const socket_info<OHost, OPort>& right) const
	{
		return (_host == right.host() && _port < right.port()) || _host < right.host();
	}

	bool operator>(const this_type& right) const
	{
		return (_host == right._host && _port > right._port) || _host > right._host;

	}

	template<typename OHost, typename OPort>
	bool operator>(const socket_info<OHost, OPort>& right) const
	{
		return (_host == right.host() && _port > right.port()) || _host > right.host();
	}

	bool operator<=(const this_type& right) const
	{
		return !operator>(right);
	}

	template<typename OHost, typename OPort>
	bool operator<=(const socket_info<OHost, OPort>& right) const
	{
		typedef OHost ohost_type;
		typedef OPort oport_type;
		return !operator><ohost_type, oport_type>(right);
	}

	bool operator>=(const this_type& right) const
	{
		return !operator<(right);
	}

	template<typename OHost, typename OPort>
	bool operator>=(const socket_info<OHost, OPort>& right) const
	{
		typedef OHost ohost_type;
		typedef OPort oport_type;
		return !operator< <ohost_type, oport_type>(right);
	}

private:
	host_type _host;
	port_type _port;
};

} // namespace network
} // namespace yggr

#define _YGGR_TMP_PP_SOCKET_INFO_SWAP() \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T)> inline \
	void swap(yggr::network::socket_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T)>& left, \
				yggr::network::socket_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T)>& right) { \
		left.swap(right); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T)> inline \
	void swap(BOOST_RV_REF(yggr::network::socket_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T)>) left, \
				yggr::network::socket_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T)>& right) { \
		right.swap(left); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T)> inline \
	void swap(yggr::network::socket_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T)>& left, \
				BOOST_RV_REF(yggr::network::socket_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T)>) right) { \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_SOCKET_INFO_SWAP()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_SOCKET_INFO_SWAP()
} // namespace boost

#undef _YGGR_TMP_PP_SOCKET_INFO_SWAP

#endif // __YGGR_NETWORK_SOCKET_INFO_HPP__
