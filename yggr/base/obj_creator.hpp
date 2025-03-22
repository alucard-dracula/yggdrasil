//obj_creator.hpp

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

#ifndef __YGGR_OBJ_CREATER_HPP__
#define __YGGR_OBJ_CREATER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable : 4624 )
#endif //_MSC_VER
namespace yggr
{

template<typename T>
class obj_creator 
	: private nonable::noncreateable
{
public:
    typedef T value_type;
	typedef typename boost::add_reference<value_type>::type reference_type;
	typedef typename boost::add_const<reference_type>::type const_reference_type;

	typedef value_type create_result_type;
	typedef reference_type static_create_result_type;
	typedef const_reference_type static_const_create_result_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	
	template<typename ...Args> inline
	static create_result_type create(BOOST_FWD_REF(Args)... args)
	{
		value_type val(boost::forward<Args>(args)...);
		return val;
	}

	template<typename ...Args> inline
	static static_create_result_type static_create(BOOST_FWD_REF(Args)... args)
	{
		static value_type val(boost::forward<Args>(args)...);
		return val;
	}

	template<typename ...Args> inline
	static static_const_create_result_type static_const_create(BOOST_FWD_REF(Args)... args)
	{
		static value_type val(boost::forward<Args>(args)...);
		return val;
	}

#else

	inline static create_result_type create(void)
	{
		return value_type(); // must using this style in msvc10
	}

	inline static static_create_result_type static_create(void)
	{
		static value_type val;
		return val;
	}

	inline static static_const_create_result_type static_const_create(void)
	{
		static value_type val;
		return val;
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static create_result_type create( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return value_type( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static static_create_result_type static_create( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			static value_type val( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return val; } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static static_const_create_result_type static_const_create( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			static value_type val( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return val; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

};

template<>
class obj_creator<void>
	: private nonable::noncreateable
{
public:
    typedef void value_type;
	typedef value_type reference_type;
	typedef value_type const_reference_type;

	typedef value_type create_result_type;
	typedef value_type static_create_result_type;
	typedef value_type static_const_create_result_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	
	template<typename ...Args> inline
	static create_result_type create(BOOST_FWD_REF(Args)... args)
	{
		return;
	}

	template<typename ...Args> inline
	static static_create_result_type static_create(BOOST_FWD_REF(Args)... args)
	{
		return;
	}

	template<typename ...Args> inline
	static static_const_create_result_type static_const_create(BOOST_FWD_REF(Args)... args)
	{
		return;
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static create_result_type create( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return;} \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static static_create_result_type static_create( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return; } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static static_const_create_result_type static_const_create( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename T, typename ...Args> inline
typename obj_creator<T>::create_result_type
	create_object(BOOST_FWD_REF(Args)... args)
{
	typedef obj_creator<T> creator_type;
	return creator_type::create(boost::forward<Args>(args)...);
}

template<typename T, typename ...Args> inline
typename obj_creator<T>::static_create_result_type
	static_create_object(BOOST_FWD_REF(Args)... args)
{
	typedef obj_creator<T> creator_type;
	return creator_type::static_create(boost::forward<Args>(args)...);
}

template<typename T, typename ...Args> inline
typename obj_creator<T>::static_const_create_result_type
	static_const_create_object(BOOST_FWD_REF(Args)... args)
{
	typedef obj_creator<T> creator_type;
	return creator_type::static_const_create(boost::forward<Args>(args)...);
}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename T \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename obj_creator<T>::create_result_type \
		create_object( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		typedef obj_creator<T> creator_type; \
		return creator_type::create( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template<typename T \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename obj_creator<T>::static_create_result_type \
		static_create_object( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		typedef obj_creator<T> creator_type; \
		return creator_type::static_create( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template<typename T \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename obj_creator<T>::static_const_create_result_type \
		static_const_create_object( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		typedef obj_creator<T> creator_type; \
		return creator_type::static_const_create( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_OBJ_CREATER_HPP__
