//conv_params_and_packet_info_vtpl.hpp

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

#ifndef __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_VTPL_HPP__
#define __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_VTPL_HPP__

#ifndef __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_HPP__
#	error "please include conv_params_and_packet_info.hpp"
#endif // __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_HPP__

#ifdef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	error "this file is using for cxx11 variadic temples, please check environment."
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <yggr/type_traits/integer_sequence.hpp>
#include <yggr/packet/packet_info.hpp>
#include <yggr/any_val/params.hpp>
#include <yggr/support/return_arg.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/pair.hpp>

#include <utility>

namespace yggr
{
namespace struct_conv
{

struct conv_params_and_packet_info
{
private:
	typedef conv_params_and_packet_info this_type;

public:
	//------------------params to packet_info----------------------
	template< typename Key, typename Op, typename Cmp, typename Alloc,
					template<typename _Key, typename _Val, typename _Cmp, typename _Alloc > class Map > inline
	static packet::packet_info<> 
		conv(const any_val::params< Key, Op, Cmp, Alloc, Map >& params)
	{
		typedef packet::packet_info<> packet_info_type;
		return packet_info_type();
	}

	template<template<typename _First, typename _Second> class Pair,
				typename ...Args,
				typename Key, typename Op, typename Cmp, typename Alloc,
				template<typename _Key, typename _Val, typename _Cmd, typename _Alloc> class Map> inline
	static packet::packet_info< Pair<Key, Args>... >
		conv(const any_val::params< Key, Op, Cmp, Alloc, Map >& params,
				const typename boost::mpl::pair<Args, Key>::second& ...keys)
	{
		typedef packet::packet_info< Pair<Key, Args>... > packet_info_type;
		typedef any_val::params< Key, Op, Cmp, Alloc, Map > params_type;
		return 
			packet_info_type(
				Pair<Key, Args>(keys, params.template get_value<Args>(keys))...);
	}


	//------------------packet_info to params----------------------

	template<typename Params> inline
	static Params conv(const packet::packet_info<>& pak_info)
	{
		typedef Params params_type;
		return params_type();
	}

private:
	template<std::size_t I, typename Params, typename PacketInfo> inline
	static Params& prv_s_set_params_detail(Params& params, const PacketInfo& pak_info)
	{
		typedef PacketInfo packet_info_type;

		typedef typename packet_info_type::template arg<static_cast<yggr::size_type>(I)>::type type;
		const type& cref = pak_info.template get<static_cast<yggr::size_type>(I)>();
		params[cref.first] = ANY_SET(typename type::second_type, cref.second);
		return params;

	}

	template<typename Params, typename PacketInfo, std::size_t ...I> inline
	static Params& prv_s_set_params(Params& params, const PacketInfo& pak_info, ::yggr::index_sequence<I...> )
	{
		return support::return_first_arg(params, this_type::prv_s_set_params_detail<I>(params, pak_info)...);
	}

public:

	template<typename Params, typename ...Args> inline
	static Params conv(const packet::packet_info<Args...>& pak_info)
	{
		typedef Params params_type;
		typedef packet::packet_info<Args...> packet_info_type;

		params_type params;
		this_type::prv_s_set_params(params, pak_info, ::yggr::make_index_sequence<(sizeof...(Args))>{});
		return params;
	}

};

} // namespace struct_conv
} // namespace yggr

#endif // __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_VTPL_HPP__
