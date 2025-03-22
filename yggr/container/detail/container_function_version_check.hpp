//container_function_version_check.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_CONTAINER_FUNCTION_VERSION_CHECK_HPP__
#define __YGGR_CONTAINER_DETAIL_CONTAINER_FUNCTION_VERSION_CHECK_HPP__

#include <yggr/ppex/has_member_check.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/void.hpp>


namespace yggr
{
namespace container
{
namespace detail
{

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _FOO_PARAMS() YGGR_PP_CONCAT(typename T::iterator)
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
#	define _ARGS typename T::iterator 
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

YGGR_PP_HAS_MEMBER_FOO_DEF(erase_i1_void, T, void, erase, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _FOO_PARAMS() YGGR_PP_CONCAT(typename T::const_iterator)
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
#	define _ARGS typename T::const_iterator 
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

YGGR_PP_HAS_MEMBER_FOO_DEF(erase_ci1_void, T, void, erase, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _FOO_PARAMS() YGGR_PP_CONCAT(typename T::iterator, typename T::iterator)
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
#	define _ARGS typename T::iterator, typename T::iterator 
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

YGGR_PP_HAS_MEMBER_FOO_DEF(erase_i2_void, T, void, erase, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _FOO_PARAMS() YGGR_PP_CONCAT(typename T::const_iterator, typename T::const_iterator)
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
#	define _ARGS typename T::const_iterator, typename T::const_iterator 
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

YGGR_PP_HAS_MEMBER_FOO_DEF(erase_ci2_void, T, void, erase, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

template<typename T>
struct erase_result
	: public
		boost::mpl::if_
		<
			typename 
				boost::mpl::or_
				<
					typename has_erase_i1_void<T>::type,
					typename has_erase_ci1_void<T>::type,
					typename has_erase_i2_void<T>::type,
					typename has_erase_ci2_void<T>::type
				>::type,
			boost::mpl::void_,
			typename T::iterator
		>
{
};


#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _FOO_PARAMS() YGGR_PP_CONCAT(typename T::iterator, typename T::size_type, const typename T::value_type&)
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
#	define _ARGS typename T::iterator, typename T::size_type, const typename T::value_type&
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

YGGR_PP_HAS_MEMBER_FOO_DEF(insert_i1_2_void, T, void, insert, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _FOO_PARAMS() YGGR_PP_CONCAT(typename T::const_iterator, typename T::size_type, const typename T::value_type&)
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
#	define _ARGS typename T::const_iterator, typename T::size_type, const typename T::value_type&
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

YGGR_PP_HAS_MEMBER_FOO_DEF(insert_ci1_2_void, T, void, insert, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

template<typename Container, typename Iterator> 
struct _has_insert_i3_void
{ 
private: 
	typedef Container cont_type; 
	typedef Iterator iter_type;

	typedef u8 true_type;
	typedef u32 false_type;
		
	template<typename T, void (T::*)(typename T::iterator, iter_type, iter_type) > 
	struct check_t; 
		
	template<typename T> 
	static true_type chk_foo(const check_t<T, &T::insert >* ); 
		
	template<typename T> 
	static false_type chk_foo(...); 
		
public: 
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type;
};

template<typename Container, typename Iterator> 
struct has_insert_i3_void 
	: public _has_insert_i3_void<Container, Iterator>::type
{ 
};

template<typename Container, typename Iterator> 
struct _has_insert_ci3_void
{ 
private: 
	typedef Container cont_type; 
	typedef Iterator iter_type;
	typedef u8 true_type;
	typedef u32 false_type;
		
	template<typename T, void (T::*)(typename T::const_iterator, iter_type, iter_type) > 
	struct check_t; 
		
	template<typename T> 
	static true_type chk_foo(const check_t<T, &T::insert>* ); 
		
	template<typename T> 
	static false_type chk_foo(...); 
		
public: 
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type; 
};

template<typename Container, typename Iterator> 
struct has_insert_ci3_void 
	: public _has_insert_ci3_void<Container, Iterator>::type
{
};

template<typename T, typename Iter = T>
struct insert_result
	: public
		boost::mpl::if_
		<
			typename
				boost::mpl::or_
				<
					typename has_insert_i3_void<T, Iter>::type,
					typename has_insert_ci3_void<T, Iter>::type
				>::type,
			boost::mpl::void_,
			typename T::iterator
		>
{
};

template<typename T>
struct insert_result<T, T>
	: public 
		boost::mpl::if_
		<
			typename 
				boost::mpl::or_
				<
					typename has_insert_i1_2_void<T>::type,
					typename has_insert_ci1_2_void<T>::type
				>::type, 
			boost::mpl::void_,
			typename T::iterator
		>
{
};


} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_CONTAINER_FUNCTION_VERSION_CHECK_HPP__
