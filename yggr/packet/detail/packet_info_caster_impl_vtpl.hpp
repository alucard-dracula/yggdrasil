// packet_info_caster_impl_vtpl.hpp

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

#ifndef __YGGR_PACKET_DETAIL_PACKET_INFO_CASTER_IMPL_VTPL_HPP__
#define __YGGR_PACKET_DETAIL_PACKET_INFO_CASTER_IMPL_VTPL_HPP__

#include <yggr/base/yggrdef.h>

#if !(defined(__YGGR_PACKET_PACKET_INFO_CASTER_HPP__) || defined(__YGGR_PACKET_PACKET_INFO_VTPL_HPP__))
#	error "please include packet_info_caster.hpp.hpp"
#endif // __YGGR_PACKET_PACKET_INFO_CASTER_HPP__

#ifdef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	error "this file is using for cxx11 variadic temples, please check environment."
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <yggr/packet/detail/packet_info_decl_vtpl.hpp>
#include <yggr/packet/detail/compatibility_nmpl_seq_size.hpp>

#include <yggr/type_traits/reference_trans.hpp>
#include <yggr/type_traits/integer_sequence.hpp>
#include <yggr/packet/packet_info.hpp>
#include <yggr/mplex/identity.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

namespace yggr
{
namespace packet
{
namespace detail
{

// In GCC or some other CPP compilers, 
// boost::mpl::vector<...>::type gets broken down into a form like 
// "seq_view_impl< boost::mpl::vector(N)<T1, boost::mpl::vector(N-1)<T2, T3...>... > >", 
// so you need to use indices to access it.

template<typename T, typename Trans, typename Index, typename SeqTag = typename boost::mpl::sequence_tag<T>::type>
struct packet_info_caster_no_variadic_impl;

template<typename T, typename Trans, typename SeqTag, std::size_t ...Idx>
struct packet_info_caster_no_variadic_impl<T, Trans, yggr::index_sequence<Idx...>, SeqTag>
{
public:
	typedef
		::yggr::packet::packet_info
		<
			typename
				reference_trans
				<
					typename
						Trans::template apply
						<
							typename boost::mpl::at_c<T, Idx>::type
						>::type
				>::type...
		> type;
};

template<typename T, typename Trans>
struct packet_info_caster_no_variadic_impl<T, Trans, yggr::index_sequence<>, boost::mpl::non_sequence_tag>
{
public:
	typedef ::yggr::packet::packet_info<> type;
};

template<typename T, typename Trans>
struct packet_info_caster_no_variadic
	: public
		packet_info_caster_no_variadic_impl
		<
			T,
			Trans,
			make_index_sequence<compatibility_nmpl_seq_size<T>::type::value>
		>
{
};

} // namespace detail
} // namespace packet
} // namespace yggr

namespace yggr
{
namespace packet
{
namespace detail
{

template<typename T, typename Trans = mplex::identity_meta_foo>
struct packet_info_caster_impl
	: public packet_info_caster_no_variadic<T, Trans>
{
};

template<typename ...Args,
			template<typename ..._Args> class Src, typename Trans>
struct packet_info_caster_impl< Src< Args... >, Trans >
{
	typedef
		::yggr::packet::packet_info
		<
			typename
				reference_trans
				<
					typename Trans::template apply<Args>::type
				>::type...
		> type;
};

} // namespace detail
} // namespace packet
} // namespace yggr


#endif // __YGGR_PACKET_DETAIL_PACKET_INFO_CASTER_IMPL_VTPL_HPP__
