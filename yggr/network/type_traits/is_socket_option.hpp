// is_socket_option.hpp

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

#ifndef __YGGR_NETWORK_TYPE_TRAITS_IS_SOCKET_OPTION_HPP__
#define __YGGR_NETWORK_TYPE_TRAITS_IS_SOCKET_OPTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/has_member_check.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>

#include <boost/asio/detail/socket_option.hpp>

YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN()

YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(so_level, level)
YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(so_name, name)
YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(so_data, data)
YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(so_size, size)

YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END()

#define YGGR_PP_NETWORK_IS_SETTABLE_SOCKET_OPTION_IMPL_DEF( __type__ ) \
	namespace yggr { namespace network { namespace detail { \
	template<> struct is_socket_option_impl< __type__ > : public boost::mpl::true_ { }; }}}


namespace yggr
{
namespace network
{
namespace type_traits
{


template<typename T>
struct is_socket_option_impl
	: public 
		boost::mpl::and_
		<
			YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(so_level, T),
			YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(so_name, T),
			YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(so_data, T),
			YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(so_size, T)
		>::type
{
};

template<int Level, int Name>
struct is_socket_option_impl< ::boost::asio::detail::socket_option::boolean<Level, Name> >
	: public boost::mpl::true_
{
};

template<int Level, int Name>
struct is_socket_option_impl< ::boost::asio::detail::socket_option::integer<Level, Name> >
	: public boost::mpl::true_
{
};

template<int Level, int Name>
struct is_socket_option_impl< ::boost::asio::detail::socket_option::linger<Level, Name> >
	: public boost::mpl::true_
{
};

template<typename T>
struct is_settable_socket_option
	: public is_socket_option_impl<T>
{
};

template<typename T>
struct is_gettable_socket_option
	: public is_socket_option_impl<T>
{
};

template<typename T>
struct is_socket_option
	: public is_socket_option_impl<T>
{
};




} // namespace type_traits
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_TYPE_TRAITS_IS_SOCKET_OPTION_HPP__
