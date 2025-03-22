// detail_packet_info_caster.hpp

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

#ifndef __YGGR_PACKET_DETAIL_DETAIL_PACKET_INFO_CASTER_HPP__
#define __YGGR_PACKET_DETAIL_DETAIL_PACKET_INFO_CASTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/typedef.hpp>
#include <yggr/type_traits/reference_trans.hpp>
#include <yggr/packet/packet_info.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace packet
{
namespace detail
{

template<typename T>
struct detail_packet_info_caster;

} // namespace detail
} // namespace packet
} // namespace yggr

namespace yggr
{
namespace packet
{
namespace detail
{

//#if !defined(_MSC_VER)
//template<typename T>
//struct detail_packet_info_caster
//{
//	typedef ::yggr::packet::packet_info<> type;
//};
//
//#endif // _MSC_VER

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args,
				template<typename ..._Args> class Src>
	struct detail_packet_info_caster< Src< Args... > >
	{
		typedef ::yggr::packet::packet_info< typename reference_trans<Args>::type... > type;
	};

#else

#	define YGGR_TMP_PP_TEMPLATE_PARAMS_REF_TRANS_IMPL( __z__, __n__, __class_name__ ) \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				typename reference_trans< BOOST_PP_CAT( __class_name__, __n__ ) >::type

#	define YGGR_TMP_PP_TEMPLATE_PARAMS_REF_TRANS( __count__, __class_name__ ) \
				BOOST_PP_REPEAT( __count__, YGGR_TMP_PP_TEMPLATE_PARAMS_REF_TRANS_IMPL, __class_name__ )

#	define YGGR_PP_HANDLER_CENTER_PARAME_CASTER_IMPL( __n__ ) \
		template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T), \
					template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T)> class Src > \
		struct detail_packet_info_caster< Src< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > > { \
			typedef ::yggr::packet::packet_info<YGGR_TMP_PP_TEMPLATE_PARAMS_REF_TRANS(__n__, T)> type; };


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_PP_HANDLER_CENTER_PARAME_CASTER_IMPL( __n__ )

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE( )


#	undef YGGR_PP_CAST_TO_PACKET_INFO_IMPL

#	undef YGGR_TMP_PP_TEMPLATE_PARAMS_REF_TRANS
#	undef YGGR_TMP_PP_TEMPLATE_PARAMS_REF_TRANS_IMPL

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


} // namespace detail
} // namespace packet
} // namespace yggr


#endif // __YGGR_PACKET_DETAIL_DETAIL_PACKET_INFO_CASTER_HPP__
