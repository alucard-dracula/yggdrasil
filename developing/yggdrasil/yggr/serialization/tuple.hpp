//tuple.hpp

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

#ifndef __YGGR_SERIALIZATION_TUPLE_HPP__
#define __YGGR_SERIALIZATION_TUPLE_HPP__

#include <boost/serialization/split_free.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/struct_support/tuple.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/serialization/nvp.hpp>

// _YGGR_TMP_PP_TEP_SUB_OP(__n__, __left__, __right__)
// _YGGR_TMP_PP_ARCHIVE_NAME ()
// _YGGR_TMP_PP_TEP_NAME ()

#define _YGGR_TMP_PP_TUPLE_ARCHIVE_OP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						( _YGGR_TMP_PP_TEP_SUB_OP( __n__, \
													YGGR_PP_OPEN( _YGGR_TMP_PP_ARCHIVE_NAME  ), \
													YGGR_PP_OPEN( _YGGR_TMP_PP_TEP_NAME ) ) )

#define _YGGR_TMP_PP_TUPLE_PARAMS_OP( __count__, __op__ ) BOOST_PP_REPEAT( __count__, _YGGR_TMP_PP_TUPLE_ARCHIVE_OP, __op__ )

namespace boost
{
namespace serialization
{
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Archive YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T) > \
	inline void save(Archive & _YGGR_TMP_PP_ARCHIVE_NAME(), \
						const boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_NAME(), \
						const unsigned int ver) { \
		typedef boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > tuple_type; \
		yggr::ser_size_type count = boost::tuples::length<tuple_type>::value; \
		ar << YGGR_SERIALIZE_NVP(count); \
		_YGGR_TMP_PP_TUPLE_PARAMS_OP( __n__, YGGR_PP_SYMBOL_SEMICOLON ); }

#define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __left__, __right__ ) \
	( __left__ << YGGR_SERIALIZE_NAME_NVP( "item", boost::get< __n__ >( __right__ ) ) )

#define _YGGR_TMP_PP_ARCHIVE_NAME() ar
#define _YGGR_TMP_PP_TEP_NAME() t

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )

#undef _YGGR_TMP_PP_TEP_NAME
#undef _YGGR_TMP_PP_ARCHIVE_NAME
#undef _YGGR_TMP_PP_TEP_SUB_OP


#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Archive YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T) > \
	inline void load(Archive & _YGGR_TMP_PP_ARCHIVE_NAME(), \
						boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_NAME(), \
						const unsigned int ver) { \
		typedef boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > tuple_type; \
		yggr::ser_size_type count = 0; \
		ar >> YGGR_SERIALIZE_NVP(count); \
		assert(count == boost::tuples::length<tuple_type>::value); \
		_YGGR_TMP_PP_TUPLE_PARAMS_OP( __n__, YGGR_PP_SYMBOL_SEMICOLON ); }

#define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __left__, __right__ ) \
	( __left__ >> YGGR_SERIALIZE_NAME_NVP( "item", boost::get< __n__ >( __right__ ) ) )

#define _YGGR_TMP_PP_ARCHIVE_NAME() ar
#define _YGGR_TMP_PP_TEP_NAME() t

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )

#undef _YGGR_TMP_PP_TEP_NAME
#undef _YGGR_TMP_PP_ARCHIVE_NAME
#undef _YGGR_TMP_PP_TEP_SUB_OP

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Archive YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T) > \
	inline void serialize(Archive & ar, \
							boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& t, \
							const unsigned int ver) { \
		boost::serialization::split_free(ar, t, ver); }


#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE( )

} // namespace serialization
} // namespace boost

#undef _YGGR_TMP_PP_TUPLE_PARAMS_OP
#undef _YGGR_TMP_PP_TUPLE_ARCHIVE_OP

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(__n__, boost::tuple)

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE( )

#endif // __YGGR_SERIALIZATION_TUPLE_HPP__

#include <yggr/serialization/tuple_bson_impl.hpp>
