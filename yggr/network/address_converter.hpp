//address_converter.hpp

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

#ifndef __YGGR_NETWORK_ADDRESS_CONVERTER_HPP__
#define __YGGR_NETWORK_ADDRESS_CONVERTER_HPP__

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/charset/string_converter.hpp> 

#include <boost/type_traits/is_class.hpp>
#include <boost/mpl/if.hpp>

#include <sstream>

namespace yggr
{
namespace network
{

template<typename Address>
struct address_converter
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Address address_type;
private:
	typedef address_converter this_type;

public:
	address_converter(const address_type& addr)
		: _addr(addr)
	{
	}

	~address_converter(void)
	{
	}

public:
	inline operator const address_type&(void) const
	{
		return _addr;
	}

	template<typename String> inline
	operator String(void) const
	{
		typedef String now_string_type;
		try
		{
			return charset::string_converter::s_conv<now_string_type, std::string>(_addr.to_string());
		}
		catch(const boost::system::error_code&)
		{
			return now_string_type();
		}
	}

private:
	address_type _addr;
};

template<typename Port>
struct port_converter
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Port port_type;
	
private:
	typedef typename
		boost::mpl::if_
		<
			boost::is_class<port_type>,
			const port_type&,
			port_type
		>::type port_arg_type;

private:
	typedef port_converter this_type;

public:
	port_converter(port_arg_type port)
		: _port(port)
	{
	}

	~port_converter(void)
	{
	}

public:
	inline operator port_arg_type(void) const
	{
		return _port;
	}

	template<typename String> inline
	operator String(void) const
	{
		typedef String now_string_type;
		std::stringstream ss;
		ss << _port;
		return charset::string_converter::s_conv<now_string_type, std::string>(ss.str());
	}

private:
	port_type _port;
};

template<typename EndPoint>
struct endpoint_converter
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef EndPoint endpoint_type;
private:
	typedef endpoint_converter this_type;

public:
	endpoint_converter(const endpoint_type& ep)
		: _ep(ep)
	{
	}

	~endpoint_converter(void)
	{
	}

	inline operator const endpoint_type&(void) const
	{
		return _ep;
	}

	template<typename String> inline
	operator String(void) const
	{
		typedef String now_string_type;
		std::stringstream ss;
		ss << _ep;
		return charset::string_converter::s_conv<now_string_type, std::string>(ss.str());
	}

private:
	endpoint_type _ep;
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_ADDRESS_CONVERTER_HPP__