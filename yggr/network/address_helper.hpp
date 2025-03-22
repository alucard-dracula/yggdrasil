//address_helper.hpp

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

#ifndef __YGGR_NETWORK_ADDRESS_HELPER_HPP__
#define __YGGR_NETWORK_ADDRESS_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/charset/other_string.hpp>
#include <yggr/charset/string_converter.hpp>
#include <yggr/charset/string.hpp>

#include <boost/asio.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace network
{
namespace detail
{

template<typename Char>
struct address_from_string_impl_sizeof_1
{
public:
	typedef Char now_char_type;
	typedef boost::asio::ip::address address_type;

private:
	typedef address_from_string_impl_sizeof_1 this_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(now_char_type) == 1)>));

public:
	inline address_type operator()(const now_char_type* str_addr) const
	{
		boost::system::error_code ec;
		address_type addr(address_type::from_string(reinterpret_cast<const char*>(str_addr), ec));

		return !!ec? addr : address_type();
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	address_type operator()(const Basic_String<now_char_type, Traits, Alloc>& str_addr) const
	{
		return this_type::operator()(str_addr.data());
	}
};

template<typename Char>
struct address_from_string_impl_sizeof_n
{
private:
	typedef Char now_char_type;
public:
	typedef boost::asio::ip::address address_type;

private:
	typedef address_from_string_impl_sizeof_n this_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(now_char_type) != 1)>));

public:
	inline address_type operator()(const now_char_type* str_addr) const
	{
		typedef yggr::basic_string<char> using_string_type;
		typedef address_from_string_impl_sizeof_1<char> using_impl_type;

		using_string_type str;
		charset::string_converter::s_conv<using_string_type>(str, str_addr); 

		using_impl_type impl;
		return impl(str);
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	address_type operator()(const Basic_String<now_char_type, Traits, Alloc>& str_addr) const
	{
		typedef Basic_String<now_char_type, Traits, Alloc> now_string_type;
		typedef yggr::basic_string<char> using_string_type;
		typedef address_from_string_impl_sizeof_1<char> using_impl_type;

		using_string_type str;
		charset::string_converter::s_conv<using_string_type>(str, str_addr); 

		using_impl_type impl;
		return impl(str);
	}
};


template<typename Char, std::size_t N = sizeof(Char)>
struct address_from_string_impl
	: public address_from_string_impl_sizeof_n<Char>
{
public:
	typedef address_from_string_impl_sizeof_n<Char> base_type;
	typedef typename base_type::now_char_type now_char_type;
	typedef typename base_type::address_type address_type;

public:
	using base_type::operator();

};

template<typename Char>
struct address_from_string_impl<Char, 1>
	: public address_from_string_impl_sizeof_1<Char>
{
public:
	typedef address_from_string_impl_sizeof_1<Char> base_type;
	typedef typename base_type::now_char_type now_char_type;
	typedef typename base_type::address_type address_type;

public:
	using base_type::operator();
};

} // namespace detail
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{

class address_helper
{
public:
	typedef boost::asio::ip::address address_type;

public:
	template<typename Char> inline
	static address_type address_from_string(const Char* str_addr)
	{
		typedef detail::address_from_string_impl<Char> h_type;

		h_type h;
		return h(str_addr);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	static address_type address_from_string(const Basic_String<Char, Traits, Alloc>& str_addr)
	{
		typedef detail::address_from_string_impl<Char> h_type;

		h_type h;
		return h(str_addr);
	}

public:
	address_helper(void)
	{
	}

	~address_helper(void)
	{
	}
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_ADDRESS_HELPER_HPP__
