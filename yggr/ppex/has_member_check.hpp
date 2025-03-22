// has_member_check.hpp

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

#ifndef __YGGR_PPEX_HAS_MEMBER_CHECK_HPP__
#define __YGGR_PPEX_HAS_MEMBER_CHECK_HPP__

// thank you for zouzhizhang(QQ304321141) provide the algorithm
// thank you for [C++/ObjC/Java]ZERO (QQ365203662 ZFFramework's Master) provide optimization of the algorithm
// using C++ SFINAE

#include <yggr/base/yggrdef.h>
#include <yggr/typeof/typeof.hpp>

#include <yggr/ppex/concat.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <yggr/type_traits/native_t.hpp>

#include <boost/type_traits/is_class.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

#include <boost/preprocessor/cat.hpp>

#if YGGR_CPP_VERSION < YGGR_CPP_VER_11
#	if defined(__MINGW32__)
#		define YGGR_PP_MAKE_FOO_POINTER( __ptr__ ) __ptr__
#	else
#		define YGGR_PP_MAKE_FOO_POINTER( __ptr__ ) &__ptr__
#	endif // __GNUC__
#else
#	define YGGR_PP_MAKE_FOO_POINTER( __ptr__ ) &__ptr__
#endif // YGGR_CPP_VERSION

#define YGGR_PP_MAKE_VAR_POINTER( __var__ ) &__var__


#define YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN() \
	namespace yggr { namespace has_some {

// ---------------member_foo---------------------
#define YGGR_PP_HAS_MEMBER_FOO_DEF(__name__, __type__, __ret_type__, __foo_name__, __is_const__, __params__ ) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_mfoo_, __name__) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		\
		template<typename __type__, \
					__ret_type__ (__type__::*)(__params__) BOOST_PP_IF(__is_const__, const, BOOST_PP_EMPTY()), \
					typename Nil = void > struct check_t; \
		\
		template<typename T> \
		static true_type chk_foo(const check_t<T, YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__), void>* ); \
		\
		template<typename T> \
		static false_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_, __name__) \
		: public BOOST_PP_CAT(__detail_has_mfoo_, __name__)<Container> :: type {};

#define YGGR_PP_HAS_MEMBER_FOO_TYPE(__name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_, __name__)< __type__ >

#define YGGR_PP_HAS_MEMBER_FOO_TYPE_RESULT(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_FOO_TYPE(__name__, __type__) :: type

#define YGGR_PP_HAS_MEMBER_FOO_TYPENAME_RESULT(__name__, __type__) \
	typename YGGR_PP_HAS_MEMBER_FOO_TYPE_RESULT(__name__, __type__)

#define YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_FOO_TYPE(__name__, __type__) :: value

#define YGGR_PP_HAS_MEMBER_FOO_TYPENAME_VALUE(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(__name__, __type__)

// member foo any
#define YGGR_PP_HAS_ANY_MEMBER_FOO_DEF( __name__, __foo_name__ ) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_any_foo_, __name__) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		struct fall_t {}; \
		struct pod_inherit_t {}; \
		struct fallback_t { fall_t __foo_name__(fall_t); }; \
		\
		typedef typename \
			boost::mpl::if_< \
				boost::is_class<cont_type>, \
				cont_type, \
				pod_inherit_t \
			> :: type first_inherit_type; \
		\
		struct derived_t \
			: public first_inherit_type, \
				public fallback_t {}; \
		\
		template<typename T, T, typename Nil = void > \
		struct check_t; \
		\
		template<typename T> \
		static false_type chk_foo(const check_t<fall_t (fallback_t::*)(fall_t), YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__), void>* ); \
		\
		template<typename T> \
		static true_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<derived_t>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_any_, __name__) \
		: public BOOST_PP_CAT(__detail_has_any_foo_, __name__)<Container> :: type {};

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(__name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_any_, __name__)< __type__ >

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_RESULT(__name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(__name__, __type__) :: type

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPENAME_RESULT(__name__, __type__) \
	typename YGGR_PP_HAS_ANT_MEMBER_FOO_TYPE_RESULT(__name__, __type__)

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_VALUE(__name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(__name__, __type__) :: value

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPENAME_VALUE(__name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_VALUE(__name__, __type__)


#if defined(BOOST_MEM_FN_ENABLE_CDECL) \
		|| defined(BOOST_MEM_FN_ENABLE_STDCALL) \
		|| defined(BOOST_MEM_FN_ENABLE_FASTCALL)

// cc version cc = __cdecl or __stdcall or __fastcall
#define YGGR_PP_HAS_MEMBER_FOO_DEF_CC(__name__, __type__, __ret_type__, __cc__, __foo_name__, __is_const__, __params__ ) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_mfoo_, BOOST_PP_CAT(__cc__, __name__)) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		\
		template<typename __type__, \
					__ret_type__ ( __cc__ __type__::* )(__params__) BOOST_PP_IF(__is_const__, const, BOOST_PP_EMPTY()), \
					typename Nil = void > struct check_t; \
		\
		template<typename T> \
		static true_type chk_foo(const check_t<T, YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__), void>* ); \
		\
		template<typename T> \
		static false_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_, BOOST_PP_CAT(__cc__, __name__)) \
		: public BOOST_PP_CAT(__detail_has_mfoo_, BOOST_PP_CAT(__cc__, __name__))< Container > :: type {};

#define YGGR_PP_HAS_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_, BOOST_PP_CAT(__cc__, __name__))< __type__ >

#define YGGR_PP_HAS_MEMBER_FOO_TYPE_RESULT_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) :: type

#define YGGR_PP_HAS_MEMBER_FOO_TYPENAME_RESULT_CC(__cc__, __name__, __type__) \
	typename YGGR_PP_HAS_MEMBER_FOO_TYPE_RESULT_CC(__cc__, __name__, __type__)

#define YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) :: value

#define YGGR_PP_HAS_MEMBER_FOO_TYPENAME_VALUE_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE_CC(__cc__, __name__, __type__)

// member foo_any cc
#define YGGR_PP_HAS_ANY_MEMBER_FOO_DEF_CC( __name__, __foo_name__, __cc__) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_any_foo_, BOOST_PP_CAT(__cc__, __name__)) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		struct fall_t {}; \
		struct pod_inherit_t {}; \
		struct fallback_t { fall_t __cc__ __foo_name__(fall_t); }; \
		\
		typedef typename \
			boost::mpl::if_< \
				boost::is_class<cont_type>, \
				cont_type, \
				pod_inherit_t \
			> :: type first_inherit_type; \
		\
		struct derived_t \
			: public first_inherit_type, \
				public fallback_t {}; \
		\
		template<typename T, T, typename Nil = void > \
		struct check_t; \
		\
		template<typename T> \
		static false_type chk_foo(const check_t<fall_t (__cc__ fallback_t::*)(fall_t), YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__), void>* ); \
		\
		template<typename T> \
		static true_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<derived_t>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_any_, BOOST_PP_CAT(__cc__, __name__)) \
		: public BOOST_PP_CAT(__detail_has_any_foo_, BOOST_PP_CAT(__cc__, __name__))<Container> :: type {};

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_any_, BOOST_PP_CAT(__cc__, __name__))< __type__ >

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_RESULT_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) :: type

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPENAME_RESULT_CC(__cc__, __name__, __type__) \
	typename YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_RESULT_CC(__cc__, __name__, __type__)

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_VALUE_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) :: value

#define YGGR_PP_HAS_ANY_MEMBER_FOO_TYPENAME_VALUE_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_VALUE_CC(__cc__, __name__, __type__)

#endif // BOOST_MEM_FN_ENABLE_CDECL

#define YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END() }}

//--------------static_member_foo---------------

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_NAMESPACE_BEGIN YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF(__name__, __ret_type__, __foo_name__, __params__) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_static_foo_, __name__) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		template< __ret_type__ (*)(__params__) > struct check_t; \
		\
		template<typename T> \
		static true_type chk_foo(const check_t< YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__) >* ); \
		\
		template<typename T> \
		static false_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_static_, __name__) \
		: public BOOST_PP_CAT(__detail_has_static_foo_, __name__)<Container> :: type {};


#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE(__name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_static_, __name__)< __type__ >

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_RESULT(__name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE(__name__, __type__) :: type

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPENAME_RESULT(__name__, __type__) \
	typename YGGR_PP_HAS_MEMBER_FOO_TYPE_RESULT(__name__, __type__)

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_VALUE(__name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE(__name__, __type__) :: value

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPENAME_VALUE(__name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_VALUE(__name__, __type__)

#if defined(BOOST_MEM_FN_ENABLE_CDECL) \
		|| defined(BOOST_MEM_FN_ENABLE_STDCALL) \
		|| defined(BOOST_MEM_FN_ENABLE_FASTCALL)

/*
static true_type chk_foo(const check_t< (YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__)) >* );
linux gcc14 -stdc++98 not support  "(YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__))" //note the outermost brackets
using YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__)
e.g static true_type chk_foo(const check_t< YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__) >* );
*/

// cc_version cc = __cdecl or __stdcall or __fastcall
#define YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF_CC(__name__, __ret_type__, __cc__, __foo_name__, __params__) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_static_foo_, BOOST_PP_CAT(__cc__, __name__)) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		template< __ret_type__ (__cc__ *)(__params__) > struct check_t; \
		\
		template<typename T> \
		static true_type chk_foo(const check_t< YGGR_PP_MAKE_FOO_POINTER(T::__foo_name__) >* ); \
		\
		template<typename T> \
		static false_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_static_, BOOST_PP_CAT(__cc__, __name__)) \
		: public BOOST_PP_CAT(__detail_has_static_foo_, BOOST_PP_CAT(__cc__, __name__))<Container> :: type {};

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_static_, BOOST_PP_CAT(__cc__, __name__))< __type__ >

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_RESULT_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) :: type

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPENAME_RESULT_CC(__cc__, __name__, __type__) \
	typename YGGR_PP_HAS_MEMBER_FOO_TYPE_RESULT_CC(__cc__, __name__, __type__)

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_VALUE_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_CC(__cc__, __name__, __type__) :: value

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPENAME_VALUE_CC(__cc__, __name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_VALUE_CC(__cc__, __name__, __type__)

#endif // BOOST_MEM_FN_ENABLE_CDECL

#define YGGR_PP_HAS_STATIC_MEMBER_FOO_NAMESPACE_END YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END

//--------------member_type---------------

#define YGGR_PP_HAS_MEMBER_TYPE_NAMESPACE_BEGIN YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN

#define YGGR_PP_HAS_MEMBER_TYPE_DEF(__name__, __type_name__) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_member_t_, __name__) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		template<typename T> \
		static true_type chk_foo(typename ::yggr::native_t<typename T::__type_name__> :: type*); \
		template<typename T> \
		static false_type chk_foo(...); \
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_member_t_, __name__) \
		: public BOOST_PP_CAT(__detail_has_member_t_, __name__)<Container> :: type {};

#define YGGR_PP_HAS_MEMBER_T_TYPE(__name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_member_t_, __name__)< __type__ >

#define YGGR_PP_HAS_MEMBER_T_TYPE_RESULT(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_T_TYPE(__name__, __type__) :: type

#define YGGR_PP_HAS_MEMBER_T_TYPENAME_RESULT(__name__, __type__) \
	typename YGGR_PP_HAS_MEMBER_T_TYPE_RESULT(__name__, __type__)

#define YGGR_PP_HAS_MEMBER_T_TYPE_VALUE(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_T_TYPE(__name__, __type__) :: value

#define YGGR_PP_HAS_MEMBER_T_TYPENAME_VALUE(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_T_TYPE_VALUE(__name__, __type__)

#define YGGR_PP_HAS_MEMBER_TYPE_NAMESPACE_END YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END

// ---------------member_var---------------------

#define YGGR_PP_HAS_MEMBER_VAR_NAMESPACE_BEGIN YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN

#define YGGR_PP_HAS_MEMBER_VAR_DEF(__name__, __type__, __mem_var_type__, __var_name__) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_member_var_, __name__) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		\
		template<typename __type__, \
					__mem_var_type__ (__type__::*), \
					typename Nil = void > struct check_t; \
		\
		template<typename T> \
		static true_type chk_foo(const check_t<T, YGGR_PP_MAKE_VAR_POINTER(T::__var_name__), void>* ); \
		\
		template<typename T> \
		static false_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_member_var_, __name__) \
		: public BOOST_PP_CAT(__detail_has_member_var_, __name__)<Container> :: type {};

#define YGGR_PP_HAS_MEMBER_VAR_TYPE(__name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_member_var_, __name__)< __type__ >

#define YGGR_PP_HAS_MEMBER_VAR_TYPE_RESULT(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_VAR_TYPE(__name__, __type__) :: type

#define YGGR_PP_HAS_MEMBER_VAR_TYPENAME_RESULT(__name__, __type__) \
	typename YGGR_PP_HAS_MEMBER_VAR_TYPE_RESULT(__name__, __type__)

#define YGGR_PP_HAS_MEMBER_VAR_TYPE_VALUE(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_VAR_TYPE(__name__, __type__) :: value

#define YGGR_PP_HAS_MEMBER_VAR_TYPENAME_VALUE(__name__, __type__) \
	YGGR_PP_HAS_MEMBER_VAR_TYPE_VALUE(__name__, __type__)

// member var any
#define YGGR_PP_HAS_ANY_MEMBER_VAR_DEF( __name__, __var_name__ ) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_any_var_, __name__) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		typedef std::size_t fall_t; \
		struct pod_inherit_t {}; \
		struct fallback_t { fall_t __var_name__; }; \
		\
		typedef typename \
			boost::mpl::if_< \
				boost::is_class<cont_type>, \
				cont_type, \
				pod_inherit_t \
			> :: type first_inherit_type; \
		\
		struct derived_t \
			: public first_inherit_type, \
				public fallback_t {}; \
		\
		template<typename T, T, typename Nil = void > \
		struct check_t; \
		\
		template<typename T> \
		static false_type chk_foo(const check_t<fall_t (fallback_t::*), YGGR_PP_MAKE_VAR_POINTER(T::__var_name__), void>* ); \
		\
		template<typename T> \
		static true_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<derived_t>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_any_var_, __name__) \
		: public BOOST_PP_CAT(__detail_has_any_var_, __name__)<Container> :: type {};

#define YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE(__name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_any_var_, __name__)< __type__ >

#define YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE_RESULT(__name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE(__name__, __type__) :: type

#define YGGR_PP_HAS_ANY_MEMBER_VAR_TYPENAME_RESULT(__name__, __type__) \
	typename YGGR_PP_HAS_ANT_MEMBER_FOO_TYPE_RESULT(__name__, __type__)

#define YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE_VALUE(__name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE(__name__, __type__) :: value

#define YGGR_PP_HAS_ANY_MEMBER_VAR_TYPENAME_VALUE(__name__, __type__) \
	YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE_VALUE(__name__, __type__)

#define YGGR_PP_HAS_MEMBER_VAR_NAMESPACE_END YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END

//--------------static_member_var---------------

#define YGGR_PP_HAS_STATIC_MEMBER_VAR_NAMESPACE_BEGIN YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN

#define YGGR_PP_HAS_STATIC_MEMBER_VAR_DEF(__name__, __var_type__, __var_name__) \
	template<typename Container> \
	struct BOOST_PP_CAT(__detail_has_static_var_, __name__) { \
	private: \
		typedef Container cont_type; \
		typedef ::yggr::u8 true_type; \
		typedef ::yggr::u32 false_type; \
		template< __var_type__ *  > struct check_t; \
		\
		template<typename T> \
		static true_type chk_foo(const check_t< YGGR_PP_MAKE_VAR_POINTER(T::__var_name__) >* ); \
		\
		template<typename T> \
		static false_type chk_foo(...); \
		\
	public: \
		typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; }; \
	\
	template<typename Container> \
	struct BOOST_PP_CAT(has_static_var_, __name__) \
		: public BOOST_PP_CAT(__detail_has_static_var_, __name__)<Container> :: type {};


#define YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE(__name__, __type__) \
	yggr::has_some::BOOST_PP_CAT(has_static_var_, __name__)< __type__ >

#define YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE_RESULT(__name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE(__name__, __type__) :: type

#define YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPENAME_RESULT(__name__, __type__) \
	typename YGGR_PP_HAS_MEMBER_VAR_TYPE_RESULT(__name__, __type__)

#define YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE_VALUE(__name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE(__name__, __type__) :: value

#define YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPENAME_VALUE(__name__, __type__) \
	YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE_VALUE(__name__, __type__)

#define YGGR_PP_HAS_STATIC_MEMBER_VAR_NAMESPACE_END YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END

// use example
//------------ member_foo--------------------
//YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN()
//
//#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
//#	define _FOO_PARAMS() YGGR_PP_CONCAT(void) // c99 version
//#else
//#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS) // not c99 verion
//#	define  _ARGS void
//#endif // YGGR_NO_CXX11_VARIADIC_MACROS
//
//YGGR_PP_HAS_MEMBER_FOO_DEF(foo, T, void, foo, YGGR_PP_SYMBOL_BOOL_TRUE(), _FOO_PARAMS())
//
//#undef _FOO_PARAMS
//#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
//#	undef _ARGS
//#endif // YGGR_NO_CXX11_VARIADIC_MACROS
//
//#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
//#	define _FOO_PARAMS() YGGR_PP_CONCAT(int)
//#else
//#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
//#	define  _ARGS int
//#endif // YGGR_NO_CXX11_VARIADIC_MACROS
//
//YGGR_PP_HAS_MEMBER_FOO_DEF(foo2, T, int, foo, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())
//
//#undef _FOO_PARAMS
//#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
//#	undef _ARGS
//#endif // YGGR_NO_CXX11_VARIADIC_MACROS
//
//YGGR_PP_HAS_MEMBER_FOO_DEF(begin, T, typename T::iterator, begin, 0, YGGR_PP_CONCAT(void)) // c99 simplified grammar
//YGGR_PP_HAS_MEMBER_FOO_DEF(cbegin, T, typename T::const_iterator, begin, 1, YGGR_PP_CONCAT(void))
//
//
//YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END()

// call
//assert(!(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(begin, int)));
//assert((YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(begin, vt_type)));

//--------------static_member_foo------------------
//YGGR_PP_HAS_STATIC_MEMBER_FOO_NAMESPACE_BEGIN()
//
//YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF(s_foo, void, s_foo, YGGR_PP_CONCAT(void))
//
//YGGR_PP_HAS_STATIC_MEMBER_FOO_NAMESPACE_END()

// call
//assert(!(YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_VALUE(s_foo, int)));
//assert((YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_VALUE(s_foo, A)));

//--------------member_type---------------
//YGGR_PP_HAS_MEMBER_TYPE_NAMESPACE_BEGIN()
//
//YGGR_PP_HAS_MEMBER_TYPE_DEF(t_name)
//
//YGGR_PP_HAS_MEMBER_TYPE_NAMESPACE_END()

// call
//assert(!(YGGR_PP_HAS_MEMBER_T_TYPE_VALUE(value_type, int)));
//assert((YGGR_PP_HAS_MEMBER_T_TYPE_VALUE(value_type, vt_type)));

namespace yggr
{
namespace ppex
{
} // namespace ppex
} // namespace ygr

#endif // __YGGR_PPEX_HAS_MEMBER_CHECK_HPP__
