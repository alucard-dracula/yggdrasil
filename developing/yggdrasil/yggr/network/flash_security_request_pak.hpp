//flash_security_request_pak.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_PACKET_FLASH_SECURITY_REQUEST_PAK_HPP__
#define __YGGR_NETWORK_NETWORK_PACKET_FLASH_SECURITY_REQUEST_PAK_HPP__

//<policy-file-request/>
#include <yggr/charset/string.hpp>
#include <utility>
#include <yggr/base/yggrdef.h>
#include <memory>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/move/move.hpp>

namespace yggr
{
namespace network
{
namespace network_packet
{

template<typename Def, typename Size = yggr::u32>
class flash_security_request_pak
{
public:
	typedef yggr::network::type_traits::tag_pak_static packet_tag_type;

	typedef Size size_type;
	typedef Def def_type;

private:
	typedef flash_security_request_pak this_type;

public:

	flash_security_request_pak(void)
	{
		memset(_request, 0, def_type::request_length());
	}

	template<typename InputIter>
	flash_security_request_pak(InputIter start, InputIter end)
	{
		assert(((end - start) * sizeof(*start) == def_type::request_length()));
		memcpy(_request, &(*start), def_type::request_length());
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename Alloc>
					class Basic_String>
	flash_security_request_pak(const Basic_String<char, Traits, Alloc>& buf)
	{
		size_type size = def_type::request_length();
		memset(_request, 0, size);
		if(buf.length() != size)
		{
			return;
		}

		memcpy(_request, &buf[0], size);
	}

	//flash_security_request_pak(BOOST_RV_REF(this_type) right)
	//{
	//	memcpy(_request, right._request, def_type::request_length());
	//}

	flash_security_request_pak(const this_type& right)
	{
		memcpy(_request, right._request, def_type::request_length());
	}

	~flash_security_request_pak(void)
	{
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		memcpy(_request, right._request, def_type::request_length());
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename Alloc>
					class Basic_String>
	bool operator==(const Basic_String<char, Traits, Alloc>& str) const
	{
		return !memcmp(&str[0], _request, def_type::E_Request_Length - 1);
	}

	bool operator==(const this_type& right) const
	{
		return !memcmp(&str[0], _request, def_type::E_Request_Length - 1);
	}

	bool empty(void) const
	{
		u8 tmp[def_type::E_Request_Length] = {0};
		return 0 == memcmp(tmp, _request, def_type::E_Request_Length);
	}

	size_type limit_buf_size(void) const
	{
		return def_type::request_length();
	}

	static size_type s_limit_buf_size(void)
	{
		return def_type::request_length();
	}

	static size_type s_buf_size(void)
	{
		return def_type::request_length();
	}

	size_type size(void) const
	{
		return def_type::request_length();
	}

	u8* data_buf(void)
	{
		return reinterpret_cast<u8*>(_request);
	}

	const u8* data_buf(void) const
	{
		return (const u8*)_request;
	}

private:
	u8 _request[def_type::E_Request_Length];
};

} // namespace network_packet
} // namespace network 
} // namespace yggr

template< typename Traits, typename Alloc, typename Def, typename Size, 
			template<typename _Char, typename _Traits, typename Alloc>
				class Basic_String>
bool operator==(const Basic_String<char, Traits, Alloc>& left,
				const yggr::network::flash_security_request_pak<Def, Size>& right)
{
	return right.operator==(left);
}

#endif //__YGGR_NETWORK_NETWORK_PACKET_FLASH_SECURITY_REQUEST_PAK_HPP__
