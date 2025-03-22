//archive_inner_keys.hpp

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

#ifndef __YGGR_ARCHIVE_ARCHIVE_INNER_KEYS_HPP__
#define __YGGR_ARCHIVE_ARCHIVE_INNER_KEYS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/const_strings.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>


#if !defined(YGGR_NO_CWCHAR)

#define YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_WCHAR_T() \
	inline operator wchar_t* const(void) const { \
		return const_cast<wchar_t* const>(YGGR_PP_CAST(__key__, YGGR_PP_CAST_TO_WSTRING)); }

#else

#	define YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_WCHAR_T()

#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)

#define YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR8() \
	inline operator ::yggr::c8* const(void) const { \
		return const_cast< ::yggr::c8* const>(YGGR_PP_CAST(__key__, YGGR_PP_CAST_TO_U8_STRING)); }

#else

#	define YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR8()

#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)

#define YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR16() \
	inline operator ::yggr::c16* const(void) const { \
		return const_cast< ::yggr::c16* const>(YGGR_PP_CAST(__key__, YGGR_PP_CAST_TO_U16_STRING)); }

#else

#	define YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR16()

#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)

#define YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR32() \
	inline operator ::yggr::c32* const(void) const { \
		return const_cast< ::yggr::c32* const>(YGGR_PP_CAST(__key__, YGGR_PP_CAST_TO_U32_STRING)); }

#else

#	define YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR32()

#endif // YGGR_NO_CHAR32_T


#define YGGR_ARCHIVE_INNER_KEYS_DEF( __key__ ) \
namespace yggr { namespace archive { namespace detail { \
struct BOOST_PP_CAT(archive_inner_keys_, __key__) { \
	inline operator char* const(void) const { \
		return const_cast<char* const>(YGGR_PP_CAST(__key__, YGGR_PP_CAST_TO_STRING)); } \
	\
	YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_WCHAR_T() \
	YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR8() \
	YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR16() \
	YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR32() \
	\
	template<typename Char, typename Traits, typename Alloc, \
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline \
	operator const Basic_String<Char, Traits, Alloc>&(void) const { \
		typedef Basic_String<Char, Traits, Alloc> now_string_type; \
		BOOST_MPL_ASSERT((charset::is_string_t<now_string_type>)); \
		YGGR_PP_CONST_STRING_LOCAL_DEF(now_string, YGGR_PP_CAST(__key__, YGGR_PP_CAST_TO_STRING) ) \
		static const now_string_type s_var(YGGR_PP_CONST_STRING_LOCAL_GET(now_string, Char)); \
		return s_var; } \
	\
	template<typename String> inline \
	operator const String&(void) const { \
		typedef String now_string_type; \
		typedef typename now_string_type::value_type char_type; \
		YGGR_ASSERT_CHAR_TYPE(char_type); \
		YGGR_PP_CONST_STRING_LOCAL_DEF(now_string, YGGR_PP_CAST(__key__, YGGR_PP_CAST_TO_STRING) ) \
		static const now_string_type s_var(YGGR_PP_CONST_STRING_LOCAL_GET(now_string, char_type)); \
		return s_var; } }; \
	\
template<typename T> inline \
typename \
	boost::mpl::if_< \
		boost::is_pointer<T>, \
		typename boost::add_const<T>::type, \
		typename boost::add_reference< typename boost::add_const<T>::type>::type >::type \
	BOOST_PP_CAT(inner_key_, __key__)(void) { \
	typedef typename boost::mpl::if_< \
										boost::is_pointer<T>, \
										typename boost::add_const<T>::type, \
										typename boost::add_reference< \
													typename boost::add_const<T>::type>::type \
									>::type type; \
	BOOST_PP_CAT(archive_inner_keys_, __key__) getter; \
	type t = getter; \
	return t; } } using detail::BOOST_PP_CAT(inner_key_, __key__); }}


YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_obj_id)
YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_obj_id_ref)
YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_ver)
YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_library_ver)
YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_class_id)
YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_class_id_ref)
YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_class_name)
YGGR_ARCHIVE_INNER_KEYS_DEF(boost_ar_tracking)

#undef YGGR_ARCHIVE_INNER_KEYS_DEF
#undef YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR32
#undef YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR16
#undef YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_CHAR8
#undef YGGR_ARCHIVE_INNER_KEYS_OPERATOR_POINTER_WCHAR_T

#define YGGR_ARCHIVE_GET_INNER_KEY( __type__, __key__ ) \
	yggr::archive::BOOST_PP_CAT(inner_key_, __key__)< __type__ >()

#endif // __YGGR_ARCHIVE_ARCHIVE_INNER_KEYS_HPP__
