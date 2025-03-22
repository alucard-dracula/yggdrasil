//conv_params_and_packet_info_vmacro.hpp

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

#ifndef __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_VMACRO_HPP__
#define __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_VMACRO_HPP__

#ifndef __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_HPP__
#	error "please include conv_params_and_packet_info.hpp"
#endif // __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_HPP__

#include <yggr/ppex/foo_params.hpp>
#include <yggr/packet/packet_info.hpp>
#include <yggr/any_val/params.hpp>

// _YGGR_TMP_PP_PARAMS_SUB_OP(__n__, __key_name__, __val__)
// _YGGR_TMP_PP_SUB_PAIR ()
// _YGGR_TMP_PP_SUB_TYPE ()
// _YGGR_TMP_PP_KET_TYPE ()
// _YGGR_TMP_PP_KET_NAME ()
// _YGGR_TMP_PP_VAL ()
#define _YGGR_TMP_PP_PARAMS_ARGS_OP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						_YGGR_TMP_PP_PARAMS_SUB_OP( __n__, \
													YGGR_PP_OPEN( _YGGR_TMP_PP_SUB_PAIR ), \
													YGGR_PP_OPEN( _YGGR_TMP_PP_SUB_TYPE ), \
													YGGR_PP_OPEN( _YGGR_TMP_PP_KET_TYPE ), \
													YGGR_PP_OPEN( _YGGR_TMP_PP_KET_NAME ), \
													YGGR_PP_OPEN( _YGGR_TMP_PP_VAL ) )

// _YGGR_TMP_PP_PACKET_INFO_SUB_GET( __n__, __params__, __val_type__, __val__ )
// _YGGR_TMP_PP_PARAMS_NAME ()
// _YGGR_TMP_PP_PACKET_INFO_TYPE ()
// _YGGR_TMP_PP_PACKET_INFO_VAL ()

#define _YGGR_TMP_PP_PACKET_INFO_ARGS_OP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						_YGGR_TMP_PP_PACKET_INFO_SUB_SET( __n__, \
															YGGR_PP_OPEN( _YGGR_TMP_PP_PARAMS_NAME ), \
															YGGR_PP_OPEN( _YGGR_TMP_PP_PACKET_INFO_TYPE ), \
															YGGR_PP_OPEN( _YGGR_TMP_PP_PACKET_INFO_VAL ) )


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

	// throw params_type::error_type;
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< template< typename _First, typename _Second > class _YGGR_TMP_PP_SUB_PAIR(), \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename _YGGR_TMP_PP_SUB_TYPE() ), \
				typename _YGGR_TMP_PP_KET_TYPE(), typename Op, typename Cmp, typename Alloc, \
					template< typename _Key, typename _Val, typename _Cmp, typename _Alloc > class Map > inline \
	static packet::packet_info< \
				YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) > \
			conv( const any_val::params< _YGGR_TMP_PP_KET_TYPE(), Op, Cmp, Alloc, Map >& params, \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, const _YGGR_TMP_PP_KET_TYPE()& _YGGR_TMP_PP_KET_NAME() ) ) { \
		typedef packet::packet_info< \
					YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES( __n__, YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER ) > packet_info_type; \
		typedef any_val::params< _YGGR_TMP_PP_KET_TYPE(), Op, Cmp, Alloc, Map > params_type; \
		return packet_info_type( YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA ) ); }


#define _YGGR_TMP_PP_PARAMS_SUB_OP( __n__, __pair__, __type__, __key_type__, __key_name__, __val__ ) \
		( __pair__< __key_type__, BOOST_PP_CAT( __type__, __n__ ) >( \
				BOOST_PP_CAT( __key_name__, __n__ ), \
				( __val__.template get_value< BOOST_PP_CAT( __type__, __n__ ) >( BOOST_PP_CAT( __key_name__, __n__ ) ) ) ) )

#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) \
	_YGGR_TMP_PP_SUB_PAIR()< _YGGR_TMP_PP_KET_TYPE() YGGR_PP_SYMBOL_COMMA() \
								BOOST_PP_CAT( _YGGR_TMP_PP_SUB_TYPE(), __n__) >

#define _YGGR_TMP_PP_SUB_PAIR() Pair
#define _YGGR_TMP_PP_SUB_TYPE() T
#define _YGGR_TMP_PP_KET_TYPE() Key
#define _YGGR_TMP_PP_KET_NAME() key_name
#define _YGGR_TMP_PP_VAL() params

#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_PARAMS_ARGS_OP

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE()

#undef _YGGR_TMP_PP_PARAMS_SUB_OP
#undef _YGGR_TMP_PP_SUB_PAIR
#undef _YGGR_TMP_PP_SUB_TYPE
#undef _YGGR_TMP_PP_KET_TYPE
#undef _YGGR_TMP_PP_KET_NAME
#undef _YGGR_TMP_PP_VAL
#undef _YGGR_TMP_PP_PARAMS_SUB_OP
#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
#undef YGGR_PP_FOO_CUSTOM_OP

	//------------------packet_info to params----------------------

	template<typename Params> inline
	static Params conv(const packet::packet_info<>& pak_info)
	{
		typedef Params params_type;
		return params_type();
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename _YGGR_TMP_PP_PARAMS_TYPE(), \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) > inline \
	static _YGGR_TMP_PP_PARAMS_TYPE() conv(const packet::packet_info< \
												YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_PACKET_INFO_VAL() ) { \
		typedef _YGGR_TMP_PP_PARAMS_TYPE() params_type; \
		typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > _YGGR_TMP_PP_PACKET_INFO_TYPE(); \
		params_type _YGGR_TMP_PP_PARAMS_NAME(); \
		YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_EMPTY ) \
		return _YGGR_TMP_PP_PARAMS_NAME(); }

#define _YGGR_TMP_PP_PACKET_INFO_SUB_SET( __n__, __params__, __val_type__, __val__ ) \
		{	typedef typename __val_type__::template arg< __n__ >::type BOOST_PP_CAT( type, __n__ ); \
			const BOOST_PP_CAT( type, __n__ )& cref = __val__.template get< __n__ >(); \
			_YGGR_TMP_PP_PARAMS_NAME()[cref.first] = ANY_SET( typename BOOST_PP_CAT( type, __n__ )::second_type, cref.second ); }

#define _YGGR_TMP_PP_PARAMS_TYPE() Params
#define _YGGR_TMP_PP_PARAMS_NAME() params
#define _YGGR_TMP_PP_PACKET_INFO_TYPE() packet_info_type
#define _YGGR_TMP_PP_PACKET_INFO_VAL() pak_info

#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_PACKET_INFO_ARGS_OP

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE()

#undef _YGGR_TMP_PP_PACKET_INFO_SUB_GET
#undef _YGGR_TMP_PP_PARAMS_TYPE
#undef _YGGR_TMP_PP_PARAMS_NAME
#undef _YGGR_TMP_PP_PACKET_INFO_TYPE
#undef _YGGR_TMP_PP_PACKET_INFO_VAL
#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
#undef YGGR_PP_FOO_CUSTOM_OP

};

} // namespace struct_conv
} // namespace yggr

#undef _YGGR_TMP_PP_PARAMS_ARGS_OP
#undef _YGGR_TMP_PP_PACKET_INFO_ARGS_OP

#endif // __YGGR_STRUCT_CONV_CONV_PARAMS_AND_PACKET_INFO_VMACRO_HPP__
