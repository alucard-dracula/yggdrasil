//has_operator_pointer_member_access.hpp

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

#ifndef __YGGR_TYPE_TRAITS_HAS_OPERATOR_POINTER_MEMBER_ACCESS_HPP__
#define __YGGR_TYPE_TRAITS_HAS_OPERATOR_POINTER_MEMBER_ACCESS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/is_class_pointer.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{

namespace type_traits
{
namespace detail
{

template<typename F, bool is_class = boost::is_class<F>::value>
struct _has_op_ptr_mem_acc_class;

template<typename F, bool is_pointer = boost::is_pointer<F>::value>
struct _has_op_ptr_mem_acc;

template<typename F>
struct _has_op_ptr_mem_acc_class<F, true>
{
private:
	typedef F function_type;
	typedef yggr::u8 true_type;
	typedef yggr::u32 false_type;
	
	struct fall_t {};

	struct pod_inherit_t
	{
	};

	struct fallback_t
	{
		fall_t* operator->(void);
	};

	typedef typename 
		boost::mpl::if_
		<
			boost::is_class<function_type>,
			function_type,
			pod_inherit_t
		>::type first_inherit_type;

	struct derived_t
		: public first_inherit_type, public fallback_t
	{
	};

	template<typename T, T, typename Nil = void >
	struct check_t;

	template<typename T>
	static false_type chk_foo(const check_t<fall_t* (fallback_t::*)(void), &T::operator->, void>* );
	
	template<typename T>
	static true_type chk_foo(...);
	
public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<derived_t>(0)))> type;
};

template<typename F>
struct _has_op_ptr_mem_acc_class<F, false>
	: public boost::mpl::false_
{
};

template<typename F>
struct _has_op_ptr_mem_acc<F, true>
	: public is_class_pointer<F>::type
{
};

template<typename F>
struct _has_op_ptr_mem_acc<F, false>
	: public _has_op_ptr_mem_acc_class<F>::type
{
};

} // namespace detail
} // namespace type_traits

template<typename F>
struct has_operator_pointer_member_access_class
	: public 
		type_traits::detail::_has_op_ptr_mem_acc_class
		<
			typename 
				boost::remove_reference
				<
					typename boost::remove_cv<F>::type
				>::type
		>::type
{
};

template<typename F>
struct has_operator_pointer_member_access
	: public 
		type_traits::detail::_has_op_ptr_mem_acc
		<
			typename 
				boost::remove_reference
				<
					typename boost::remove_cv<F>::type
				>::type
		>::type
{
};

} // namespace yggr

#endif // __YGGR_TYPE_TRAITS_HAS_OPERATOR_POINTER_MEMBER_ACCESS_HPP__
