//flash_security_request_pak.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_PACKET_FLASH_SECURITY_REQUEST_PAK_HPP__
#define __YGGR_NETWORK_NETWORK_PACKET_FLASH_SECURITY_REQUEST_PAK_HPP__

//<policy-file-request/>

#include <yggr/base/yggrdef.h>

#include <yggr/move/move.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/network/type_traits/tags.hpp>

#include <yggr/type_traits/native_t.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <utility>
#include <memory>
#include <iterator>


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

	typedef utf8_string inner_string_type;

private:
	typedef mplex::sfinae_type sfinae_type;

private:
	typedef flash_security_request_pak this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	flash_security_request_pak(void)
	{
		memset(_request, 0, def_type::request_length());
	}

	template<typename String>
	flash_security_request_pak(const String& str, 
								typename
									boost::enable_if
									<
										charset::utf8_string_constructable<String>,
										sfinae_type
									>::type sfinae = 0)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type utf8_str((charset::string_charset_helper_data(str)));
		assert(utf8_str.size() == def_type::E_Request_Length - 1);

		memset(_request, 0, def_type::request_length());
		memcpy(_request, utf8_str.data(), def_type::E_Request_Length - 1);
	}

	template<typename InputIter>
	flash_security_request_pak(InputIter start, InputIter end)
	{
		assert(((end - start) * sizeof(*start) == def_type::request_length()));

		memset(_request, 0, def_type::request_length());
		memcpy(_request, boost::addressof(*start), def_type::request_length());
	}

	flash_security_request_pak(BOOST_RV_REF(this_type) right)
	{
		memcpy(_request, right._request, def_type::request_length());
	}

	flash_security_request_pak(const this_type& right)
	{
		memcpy(_request, right._request, def_type::request_length());
	}

	~flash_security_request_pak(void)
	{
	}

public:
	template<typename String>
	typename boost::enable_if<charset::utf8_string_constructable<String>, this_type&>::type
		operator=(const String& right)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type utf8_str((charset::string_charset_helper_data(str)));
		assert(utf8_str.size() == def_type::E_Request_Length - 1);

		memset(_request, 0, def_type::request_length());
		memcpy(_request, utf8_str.data(), def_type::E_Request_Length - 1);

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref) 
		{
			return *this;
		}

		memcpy(_request, right_ref._request, def_type::request_length());
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		memcpy(_request, right._request, def_type::request_length());
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& rihgt)
	{
		if(this == &right)
		{
			return;
		}

		u8 tmp[def_type::E_Request_Length] = {0};
		memcpy(tmp, _request, def_type::request_length());
		memcpy(_request, right._request, def_type::request_length());
		memcpy(right._request, tmp, def_type::request_length());
	}

public:
	template<typename String>
	typename boost::enable_if<charset::utf8_string_constructable<String>, bool>::type
		compare_eq(const String& str) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type utf8_str(charset::string_charset_helper_data(str));

		return 
			(utf8_str.size() == def_type::E_Request_Length - 1)
			&&( 0 == memcmp(utf8_str.data(), _request, def_type::E_Request_Length - 1));
	}

	inline bool compare_eq(const this_type& right) const
	{
		return 
			(this == boost::addressof(right))
			|| (0 == memcmp(&str[0], _request, def_type::E_Request_Length - 1));
	}

	inline bool empty(void) const
	{
		static const u8 tmp[def_type::E_Request_Length] = {0};
		return 0 == memcmp(tmp, _request, def_type::E_Request_Length);
	}

public:
	inline static size_type limit_buf_size(void)
	{
		return def_type::request_length();
	}

	inline static size_type buf_size(void)
	{
		return def_type::request_length();
	}

	inline size_type size(void) const
	{
		return def_type::request_length();
	}

	inline u8* data_buf(void)
	{
		return reinterpret_cast<u8*>(_request);
	}

	inline const u8* data_buf(void) const
	{
		return reinterpret_cast<const u8*>(_request);
	}

private:
	u8 _request[def_type::E_Request_Length];
};

template<typename Char, typename Traits, typename Alloc, typename Def, typename Size,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
bool operator==(const flash_security_request_pak<Def, Size>& l, 
					const Basic_String<Char, Traits, Alloc>& r) const
{
	return l.compare_eq(r);
}

template<typename Char, typename Traits, typename Alloc, typename Def, typename Size,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
bool operator==(const Basic_String<Char, Traits, Alloc>& l,
				const flash_security_request_pak<Def, Size>& r)
{
	return r.compare_eq(l);
}

template<typename DefL, typename SizeL, typename DefR, typename SizeR> inline
bool operator==(const flash_security_request_pak<DefL, SizeL>& l,
				const flash_security_request_pak<DefR, SizeR>& r)
{
	return l.compare_eq(r);
}

template<typename Char, typename Traits, typename Alloc, typename Def, typename Size,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
bool operator!=(const flash_security_request_pak<Def, Size>& l, 
					const Basic_String<Char, Traits, Alloc>& r) const
{
	return !(l.compare_eq(r));
}

template<typename Char, typename Traits, typename Alloc, typename Def, typename Size,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
bool operator!=(const Basic_String<Char, Traits, Alloc>& l,
				const flash_security_request_pak<Def, Size>& r)
{
	return !(r.compare_eq(l));
}

template<typename DefL, typename SizeL, typename DefR, typename SizeR> inline
bool operator!=(const flash_security_request_pak<DefL, SizeL>& l,
				const flash_security_request_pak<DefR, SizeR>& r)
{
	return !(l.compare_eq(r));
}

} // namespace network_packet
} // namespace network 
} // namespace yggr

namespace yggr
{
namespace network
{
namespace network_packet
{
namespace swap_support
{

template<typename Def, typename Size> inline 
void swap(flash_security_request_pak<Def, Size>& l, 
			flash_security_request_pak<Def, Size>& r) 
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace network_packet
} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::network_packet::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::network_packet::swap_support::swap;
} // namespace boost


#endif //__YGGR_NETWORK_NETWORK_PACKET_FLASH_SECURITY_REQUEST_PAK_HPP__
