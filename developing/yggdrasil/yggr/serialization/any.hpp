// yggr serialization any.hpp

#ifndef __YGGR_SERIALIZATION_ANY_HPP__
#define __YGGR_SERIALIZATION_ANY_HPP__

#include <yggr/serialization/export.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/any_val/any.hpp>
#include <yggr/ppex/concat.hpp>

#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#define YGGR_ANY_EXPORT( __type__ ) \
	YGGR_CLASS_EXPORT( yggr::any::holder< __type__ > )

#define YGGR_ANY_EXPORT_DECLEAR( __type__ ) \
	YGGR_CLASS_EXPORT_KEY( yggr::any::holder< __type__ > )

#define YGGR_ANY_EXPORT_IMPL( __type__ ) \
	YGGR_CLASS_EXPORT_IMPLEMENT( yggr::any::holder< __type__ > )

//#define YGGR_ANY_EXPORT( __type__ ) \
//	YGGR_CLASS_EXPORT(YGGR_PP_CONCAT( yggr::any::holder< __type__ > ))
//
//#define YGGR_ANY_EXPORT_DECLEAR( __type__ ) \
//	YGGR_CLASS_EXPORT_KEY( YGGR_PP_CONCAT( yggr::any::holder< __type__ > ) )
//
//#define YGGR_ANY_EXPORT_IMPL( __type__ ) \
//	YGGR_CLASS_EXPORT_IMPLEMENT( YGGR_PP_CONCAT( yggr::any::holder< __type__ > ) )

// don't export this place 
//YGGR_ANY_EXPORT(char)
//
//#if !(defined(__ANDROID__) || defined(__MINGW32__))
//YGGR_ANY_EXPORT(wchar_t)
//#endif // !(defined(__ANDROID__) || defined(__MINGW32__))
//
//YGGR_ANY_EXPORT(yggr::s8)
//YGGR_ANY_EXPORT(yggr::u8)
//YGGR_ANY_EXPORT(yggr::s16)
//YGGR_ANY_EXPORT(yggr::u16)
////YGGR_ANY_EXPORT(yggr::sint)
////YGGR_ANY_EXPORT(yggr::uint)
//YGGR_ANY_EXPORT(yggr::s32)
//YGGR_ANY_EXPORT(yggr::u32)
//YGGR_ANY_EXPORT(yggr::s64)
//YGGR_ANY_EXPORT(yggr::u64)
//YGGR_ANY_EXPORT(yggr::f32)
//YGGR_ANY_EXPORT(yggr::f64)
//YGGR_ANY_EXPORT(std::string)
//YGGR_ANY_EXPORT(boost::container::string)
//YGGR_ANY_EXPORT(yggr::charset::utf8_string)
//    
//#ifdef __ANDROID__
//YGGR_ANY_EXPORT(long int)
//YGGR_ANY_EXPORT(unsigned long int)
//#endif // __ANDROID__

namespace yggr
{
namespace serialization
{
} // serialization
} // yggr

#endif //__YGGR_SERIALIZATION_ANY_HPP__