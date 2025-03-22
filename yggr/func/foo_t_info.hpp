//foo_t_info_detail.hpp

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

#ifndef __YGGR_FUNC_FOO_T_INFO_HPP__
#define __YGGR_FUNC_FOO_T_INFO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/type_traits/native_t.hpp>

#include <yggr/ppex/typedef.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/mplex/args.hpp>
#include <yggr/mplex/revert_to_vector.hpp>

#include <yggr/func/is_callable.hpp>
#include <yggr/func/bind_av_list_cast.hpp>
#include <yggr/func/cast_to_functional_base.hpp>

#include <yggr/func/remove_noexcept.hpp>
#include <yggr/func/is_noexcept.hpp>

#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/add_reference.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/size.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

#include <boost/mpl/sort.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/function.hpp>

#include <boost/function_types/is_function.hpp>
#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/is_function_reference.hpp>
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

namespace yggr
{
namespace func
{
namespace detail
{

template<typename T>
struct foo_t_info_detail;

} // namespace detail

template<typename T>
struct foo_t_info;

} // namespace func
} // namespace yggr

namespace yggr
{
namespace func
{

struct tag_va_list
{
	typedef char* type;
};

template<typename T>
struct tag_object_reference
{
	typedef T* pointer_type;
	typedef T& reference_type;
};

template<typename T>
struct is_tag_object_reference
	: public boost::mpl::false_
{
};

template<typename T>
struct is_tag_object_reference< tag_object_reference<T> >
	: public boost::mpl::true_
{
};

namespace detail
{

template<typename T, bool is_tag_object_ref_chk>
struct tag_object_reference_to_pointer_if
	: public ::yggr::mplex::null_type
{
};

template<typename T>
struct tag_object_reference_to_pointer_if<T, true>
{
	typedef typename T::pointer_type type;
};


} // namespace detail

template<typename T>
struct tag_object_reference_to_pointer
	: public detail::tag_object_reference_to_pointer_if<T, is_tag_object_reference<T>::value>
{
};

template<typename T>
struct tag_object_reference_to_reference
	: public
		boost::mpl::if_
		<
			is_tag_object_reference<T>,
			typename T::reference_type,
			::yggr::mplex::null_type
		>
{
};

namespace detail
{

// boost::bind holder_list helper
template<typename T>
struct is_boost_arg
	: public boost::mpl::false_
{
};

template<int N>
struct is_boost_arg< boost::arg<N> >
	: public boost::mpl::true_
{
};

template<typename T>
struct boost_arg_N
	: public boost::mpl::int_<-1>
{
};

template<int N>
struct boost_arg_N<boost::arg<N> >
	: public boost::mpl::int_<N>
{
};

template<typename L, typename R>
struct boost_arg_less
	: public
		boost::mpl::less
		<
			typename boost_arg_N<L>::type,
			typename boost_arg_N<R>::type
		>
{
};

template<typename Arg_Holder, typename Arg_Holder_List, typename Arg_List>
struct conv_to_arg
{
	typedef Arg_Holder arg_holder;
	typedef Arg_Holder_List arg_holder_list_type;
	typedef Arg_List arg_list_type;

	BOOST_MPL_ASSERT((
		boost::is_same<typename boost::mpl::size<arg_holder_list_type>::type,
						typename boost::mpl::size<arg_list_type>::type >));

	typedef typename
		boost::mpl::distance
		<
			typename boost::mpl::begin<arg_holder_list_type>::type,
			typename boost::mpl::find<arg_holder_list_type, arg_holder>::type
		>::type idx_type;

	typedef typename boost::mpl::at_c<arg_list_type, idx_type::value>::type type;

};

// non foo
template<typename T>
struct non_foo_t_info
{
	typedef ::yggr::mplex::null_type null_type;

	typedef boost::mpl::false_ is_callable_type;
	typedef boost::mpl::false_ is_native_foo_type;
	typedef boost::mpl::false_ is_member_foo_type;
	typedef boost::mpl::false_ has_va_list_type;

	typedef null_type result_type;
	typedef null_type class_type;
	typedef null_type arg_list_type;
	typedef null_type arg_holder_list_type;

	typedef boost::mpl::false_ is_const_type;
	typedef boost::mpl::false_ is_volatile_type;
	typedef boost::mpl::false_ is_noexcept_type;
	typedef boost::mpl::size_t<0> arg_list_size_type;
	typedef boost::mpl::size_t<0> arg_holder_list_size_type;

	typedef null_type foo_type;
	typedef null_type foo_pointer_type;
	typedef null_type foo_reference_type;

	typedef null_type native_foo_type;
	typedef null_type native_foo_pointer_type;
	typedef null_type native_foo_reference_type;

	typedef null_type type;
};

// native foo
template<typename T>
struct native_foo_t_info
{
private:
	typedef T tpl_arg_type;
	typedef typename remove_noexcept<tpl_arg_type>::type noexcept_tpl_arg_type;
	BOOST_MPL_ASSERT((boost::function_types::is_function<noexcept_tpl_arg_type>));

public:
	typedef ::yggr::mplex::null_type null_type;

	typedef boost::mpl::true_ is_callable_type;
	typedef boost::mpl::true_ is_native_foo_type;
	typedef boost::mpl::false_ is_member_foo_type;

	typedef
		boost::mpl::bool_
		<
			boost::function_types::is_function
			<
				noexcept_tpl_arg_type,
				boost::function_types::variadic
			>::value
		> has_va_list_type;

	typedef typename boost::function_types::result_type<noexcept_tpl_arg_type>::type result_type;
	typedef null_type class_type;
	typedef typename
		mplex::revert_to_vector
		<
			typename boost::function_types::parameter_types<noexcept_tpl_arg_type>::type 
		>::type arg_list_type;


	typedef arg_list_type arg_holder_list_type;

	typedef
		boost::mpl::bool_
		<
			boost::function_types::is_function
			<
				noexcept_tpl_arg_type,
				boost::function_types::const_qualified
			>::value
		> is_const_type;

	typedef
		boost::mpl::bool_
		<
			boost::function_types::is_function
			<
				noexcept_tpl_arg_type,
				boost::function_types::volatile_qualified
			>::value
		> is_volatile_type;

	typedef typename is_noexcept<tpl_arg_type>::type is_noexcept_type;

	typedef boost::mpl::size_t<boost::mpl::size<arg_list_type>::value> arg_list_size_type;
	typedef boost::mpl::size_t<boost::mpl::size<arg_holder_list_type>::value> arg_holder_list_size_type;

	typedef tpl_arg_type foo_type;
	typedef typename boost::add_pointer<foo_type>::type foo_pointer_type;
	typedef typename boost::add_reference<foo_type>::type foo_reference_type;

	typedef foo_type native_foo_type;
	typedef foo_pointer_type native_foo_pointer_type;
	typedef foo_reference_type native_foo_reference_type;

	typedef foo_pointer_type type;
};

// native member foo

template<typename T>
struct native_member_foo_t_info
{
private:
	typedef T tpl_arg_type;
	typedef typename remove_noexcept<tpl_arg_type>::type noexcept_tpl_arg_type;
	BOOST_MPL_ASSERT((boost::function_types::is_member_function_pointer<noexcept_tpl_arg_type>));

public:
	typedef ::yggr::mplex::null_type null_type;

	typedef boost::mpl::true_ is_callable_type;
	typedef boost::mpl::true_ is_native_foo_type;
	typedef boost::mpl::true_ is_member_foo_type;
	typedef
		boost::mpl::bool_
		<
			boost::function_types::is_member_function_pointer
			<
				noexcept_tpl_arg_type,
				boost::function_types::variadic
			>::value
		> has_va_list_type;

private:
	typedef typename boost::function_types::parameter_types<noexcept_tpl_arg_type>::type tpl_arg_params_list_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<(!!boost::mpl::size<tpl_arg_params_list_type>::value)>));

public:
	typedef typename boost::function_types::result_type<noexcept_tpl_arg_type>::type result_type;

	typedef typename
		::yggr::native_t
		<
			typename boost::mpl::front<tpl_arg_params_list_type>::type
		>::type class_type;

	typedef typename 
		mplex::revert_to_vector
		<
			typename boost::mpl::pop_front<tpl_arg_params_list_type>::type
		>::type arg_list_type;
	typedef arg_list_type arg_holder_list_type;

	typedef
		boost::mpl::bool_
		<
			boost::function_types::is_member_function_pointer
			<
				noexcept_tpl_arg_type,
				boost::function_types::const_qualified
			>::value
		> is_const_type;

	typedef
		boost::mpl::bool_
		<
			boost::function_types::is_member_function_pointer
			<
				noexcept_tpl_arg_type,
				boost::function_types::volatile_qualified
			>::value
		> is_volatile_type;

	typedef typename is_noexcept<tpl_arg_type>::type is_noexcept_type;

	typedef boost::mpl::size_t<boost::mpl::size<arg_list_type>::value> arg_list_size_type;
	typedef boost::mpl::size_t<boost::mpl::size<arg_holder_list_type>::value> arg_holder_list_size_type;

	typedef tpl_arg_type foo_type;
	typedef foo_type foo_pointer_type;
	typedef typename boost::add_reference<foo_type>::type foo_reference_type;

	typedef foo_type native_foo_type;
	typedef foo_pointer_type native_foo_pointer_type;
	typedef foo_reference_type native_foo_reference_type;

	typedef foo_pointer_type type;
};

// callable_foo_t_info
template<typename T, bool has_result = yggr::is_callable<T>::value>
struct callable_foo_t_info;

template<typename T>
struct callable_foo_t_info<T, true>
{
private:
	typedef T tpl_arg_type;
	typedef YGGR_TYPEOF_TPL(::yggr::declval(&tpl_arg_type::operator())) tpl_arg_mem_foo_ptr_type;

private:
	typedef native_member_foo_t_info<tpl_arg_mem_foo_ptr_type> base_type;

public:
	typedef typename base_type::null_type null_type;

	typedef typename base_type::is_callable_type is_callable_type;
	typedef boost::mpl::false_ is_native_foo_type;
	typedef boost::mpl::false_ is_member_foo_type;
	typedef typename base_type::has_va_list_type has_va_list_type;

	typedef typename base_type::result_type result_type;
	typedef typename base_type::class_type class_type;
	typedef typename base_type::arg_list_type arg_list_type;
	typedef typename base_type::arg_holder_list_type arg_holder_list_type;
	typedef typename base_type::is_const_type is_const_type;
	typedef typename base_type::is_volatile_type is_volatile_type;
	typedef typename base_type::is_noexcept_type is_noexcept_type;
	typedef typename base_type::arg_list_size_type arg_list_size_type;
	typedef typename base_type::arg_holder_list_size_type arg_holder_list_size_type;

	typedef tpl_arg_type foo_type;
	typedef typename boost::add_pointer<foo_type>::type foo_pointer_type;
	typedef typename boost::add_reference<foo_type>::type foo_reference_type;

	typedef null_type native_foo_type;
	typedef null_type native_foo_pointer_type;
	typedef null_type native_foo_reference_type;

	typedef foo_type type;
};

template<typename T>
struct callable_foo_t_info<T, false>
	: public non_foo_t_info<T>
{
};

template<typename T>
struct functional_foo_t_info_detail
	: public non_foo_t_info<T>
{
};

#if YGGR_CPP_VERSION < YGGR_CPP_VER_17

// std::unary_function<T0,R>
template<typename T0, typename R>
struct functional_foo_t_info_detail< std::unary_function<T0, R> >
{
	typedef ::yggr::mplex::null_type null_type;

	typedef boost::mpl::false_ is_callable_type;
	typedef boost::mpl::false_ is_native_foo_type;
	typedef boost::mpl::false_ is_member_foo_type;
	typedef boost::mpl::false_ has_va_list_type;

	typedef R result_type;
	typedef null_type class_type;
	typedef typename boost::mpl::vector<T0>::type arg_list_type;
	typedef arg_list_type arg_holder_list_type;
	typedef boost::mpl::false_ is_const_type;
	typedef boost::mpl::false_ is_volatile_type;
	typedef boost::mpl::false_ is_noexcept_type;
	typedef boost::mpl::size_t<boost::mpl::size<arg_list_type>::value> arg_list_size_type;
	typedef boost::mpl::size_t<boost::mpl::size<arg_holder_list_type>::value> arg_holder_list_size_type;

	typedef std::unary_function<T0, R> foo_type;
	typedef typename boost::add_pointer<foo_type>::type foo_pointer_type;
	typedef typename boost::add_reference<foo_type>::type foo_reference_type;

	typedef null_type native_foo_type;
	typedef null_type native_foo_pointer_type;
	typedef null_type native_foo_reference_type;

	typedef null_type type;
};

#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17


#if YGGR_CPP_VERSION < YGGR_CPP_VER_17

// std::binary_function<T0,T1,R>
template<typename T0, typename T1, typename R>
struct functional_foo_t_info_detail< std::binary_function<T0, T1, R> >
{
	typedef ::yggr::mplex::null_type null_type;

	typedef boost::mpl::false_ is_callable_type;
	typedef boost::mpl::false_ is_native_foo_type;
	typedef boost::mpl::false_ is_member_foo_type;
	typedef boost::mpl::false_ has_va_list_type;

	typedef R result_type;
	typedef null_type class_type;
	typedef typename boost::mpl::vector< T0, T1 >::type arg_list_type;
	typedef arg_list_type arg_holder_list_type;
	typedef boost::mpl::false_ is_const_type;
	typedef boost::mpl::false_ is_volatile_type;
	typedef boost::mpl::false_ is_noexcept_type;
	typedef boost::mpl::size_t<boost::mpl::size<arg_list_type>::value> arg_list_size_type;
	typedef boost::mpl::size_t<boost::mpl::size<arg_holder_list_type>::value> arg_holder_list_size_type;

	typedef std::binary_function< T0, T1, R > foo_type;
	typedef typename boost::add_pointer<foo_type>::type foo_pointer_type;
	typedef typename boost::add_reference<foo_type>::type foo_reference_type;

	typedef null_type native_foo_type;
	typedef null_type native_foo_pointer_type;
	typedef null_type native_foo_reference_type;

	typedef null_type type;
};

#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17

#if YGGR_CPP_VERSION < YGGR_CPP_VER_17

template<typename T>
struct functional_foo_t_info
{
private:
	typedef T tpl_arg_type;
	typedef functional_foo_t_info_detail<typename cast_to_functional_base<tpl_arg_type>::type> base1_type;
	typedef callable_foo_t_info<tpl_arg_type> base2_type;

public:
	typedef ::yggr::mplex::null_type null_type;

	typedef typename base2_type::is_callable_type is_callable_type;
	typedef typename base1_type::is_native_foo_type is_native_foo_type;
	typedef typename base1_type::is_member_foo_type is_member_foo_type;
	typedef typename base1_type::has_va_list_type has_va_list_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_same<typename base2_type::result_type, null_type>,
			typename base1_type::result_type,
			typename base2_type::result_type
		>::type result_type;

	typedef typename base2_type::class_type class_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_same<typename base2_type::arg_list_type, null_type>,
			typename base1_type::arg_list_type,
			typename base2_type::arg_list_type
		>::type arg_list_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_same<typename base2_type::arg_holder_list_type, null_type>,
			typename base1_type::arg_holder_list_type,
			typename base2_type::arg_holder_list_type
		>::type arg_holder_list_type;

	typedef typename base2_type::is_const_type is_const_type;
	typedef typename base2_type::is_volatile_type is_volatile_type;
	typedef typename base2_type::is_noexcept_type is_noexcept_type;

	typedef boost::mpl::size_t<boost::mpl::size<arg_list_type>::value> arg_list_size_type;
	typedef boost::mpl::size_t<boost::mpl::size<arg_holder_list_type>::value> arg_holder_list_size_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_same<typename base2_type::foo_type, null_type>,
			typename base1_type::foo_type,
			typename base2_type::foo_type
		>::type foo_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_same<typename base2_type::foo_pointer_type, null_type>,
			typename base1_type::foo_pointer_type,
			typename base2_type::foo_pointer_type
		>::type foo_pointer_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_same<typename base2_type::foo_reference_type, null_type>,
			typename base1_type::foo_reference_type,
			typename base2_type::foo_reference_type
		>::type foo_reference_type;

	typedef null_type native_foo_type;
	typedef null_type native_foo_pointer_type;
	typedef null_type native_foo_reference_type;

	typedef typename base2_type::type type;
};

#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17

// foo_t_info_detail
template<
	typename T,
	bool is_functional_chk = !!::yggr::is_functional<T>::type::value >
struct foo_t_info_detail_is_functional_if
	: callable_foo_t_info<T>
{
	// false
};

#if YGGR_CPP_VERSION < YGGR_CPP_VER_17

template<typename T>
struct foo_t_info_detail_is_functional_if<T, true>
	: functional_foo_t_info<T>
{
	// true
};

#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17

template<
	typename T,
	bool is_member_function_pointer_chk =
		boost::function_types::is_member_function_pointer
		<
			typename
				remove_noexcept
				<
					typename boost::remove_reference<T>::type
				>::type
		>::value >
struct foo_t_info_detail_is_member_function_pointer_if
	: public foo_t_info_detail_is_functional_if<T>
{
	// false
};

template<typename T>
struct foo_t_info_detail_is_member_function_pointer_if<T, true>
	: public native_member_foo_t_info< typename boost::remove_reference<T>::type >
{
	// true
};

template<
	typename T,
	bool or_chk =
		boost::mpl::or_
		<
			boost::function_types::is_function<typename remove_noexcept<T>::type >,
			boost::function_types::is_function_pointer< typename remove_noexcept<T>::type >,
			boost::function_types::is_function_reference< typename remove_noexcept<T>::type >
		>::type::value >
struct foo_t_info_detail_is_any_func_if
	: public foo_t_info_detail_is_member_function_pointer_if<T>
{
	// false
};

template<typename T >
struct foo_t_info_detail_is_any_func_if<T, true >
	: public native_foo_t_info<typename ::yggr::native_t<T>::type >
{
	// true
};

template<typename T>
struct foo_t_info_detail
{
private:
	typedef foo_t_info_detail this_type;

private:
	typedef foo_t_info_detail_is_any_func_if<T> base_type;

public:
	typedef typename base_type::null_type null_type;

	typedef typename base_type::is_callable_type is_callable_type;
	typedef typename base_type::is_native_foo_type is_native_foo_type;
	typedef typename base_type::is_member_foo_type is_member_foo_type;
	typedef typename base_type::has_va_list_type has_va_list_type;

	typedef typename base_type::result_type result_type;
	typedef typename base_type::class_type class_type;
	typedef typename base_type::arg_list_type arg_list_type;
	typedef typename base_type::arg_holder_list_type arg_holder_list_type;
	typedef typename base_type::is_const_type is_const_type;
	typedef typename base_type::is_volatile_type is_volatile_type;
	typedef typename base_type::is_noexcept_type is_noexcept_type;

	typedef typename base_type::arg_list_size_type arg_list_size_type;
	typedef typename base_type::arg_holder_list_size_type arg_holder_list_size_type;

	typedef typename base_type::foo_type foo_type;
	typedef typename base_type::foo_pointer_type foo_pointer_type;
	typedef typename base_type::foo_reference_type foo_reference_type;

	typedef typename base_type::native_foo_type native_foo_type;
	typedef typename base_type::native_foo_pointer_type native_foo_pointer_type;
	typedef typename base_type::native_foo_reference_type native_foo_reference_type;

	typedef typename base_type::type type;
};


// boost function // not need cc
#define YGGR_FOO_BOOST_FUNCTION_T_INFO_N( __n__ ) \
	template< typename R \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) > \
	struct foo_t_info_detail< \
				BOOST_PP_CAT(boost::function, __n__)< \
					R \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > > { \
	private: \
		typedef BOOST_PP_CAT( boost::function, __n__ )< \
						R \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > tpl_arg_type; \
		typedef callable_foo_t_info<tpl_arg_type> base_type; \
	\
	public: \
		typedef ::yggr::mplex::null_type null_type; \
		\
		typedef typename base_type::is_callable_type is_callable_type; \
		typedef typename base_type::is_native_foo_type is_native_foo_type; \
		typedef typename base_type::is_member_foo_type is_member_foo_type; \
		typedef typename base_type::has_va_list_type has_va_list_type; \
		\
		typedef typename tpl_arg_type::result_type result_type; \
		typedef typename base_type::class_type class_type; \
		typedef typename base_type::arg_list_type arg_list_type; \
		typedef typename base_type::arg_holder_list_type arg_holder_list_type; \
		typedef typename base_type::is_const_type is_const_type; \
		typedef typename base_type::is_volatile_type is_volatile_type; \
		typedef typename base_type::is_noexcept_type is_noexcept_type; \
		typedef typename base_type::arg_list_size_type arg_list_size_type; \
		typedef typename base_type::arg_holder_list_size_type arg_holder_list_size_type; \
		\
		typedef typename base_type::foo_type foo_type; \
		typedef typename base_type::foo_pointer_type foo_pointer_type; \
		typedef typename base_type::foo_reference_type foo_reference_type; \
		\
		typedef null_type native_foo_type; \
		typedef null_type native_foo_pointer_type; \
		typedef null_type native_foo_reference_type; \
		\
		typedef typename base_type::type type; };

// boost bind _mf // not need cc

#define YGGR_FOOT_T_BOOST__MF_N( __n__, __is_const__ ) \
	template<typename R, typename T \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename A ) \
			YGGR_MEM_FN_CLASS_F() > \
	struct foo_t_info_detail< \
			boost::_mfi::YGGR_BIND_MF_NAME( \
				BOOST_PP_CAT(BOOST_PP_IF(__is_const__, cmf, mf), __n__))< \
					R, T \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, A) \
					YGGR_MEM_FN_F() > > { \
	private: \
		typedef \
			R(YGGR_MEM_FN_CC() T::* tpl_arg_foo_type)( \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, A) ) \
				YGGR_PP_SYMBOL_IF(__is_const__, YGGR_TMP_PP_SYMBOL_CONST); \
		\
		typedef boost::_mfi::YGGR_BIND_MF_NAME( \
					BOOST_PP_CAT(BOOST_PP_IF(__is_const__, cmf, mf), __n__))< \
						R, T \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, A) YGGR_MEM_FN_F() > tpl_arg_type; \
		\
		typedef native_member_foo_t_info< tpl_arg_foo_type > base_type; \
	public: \
		typedef typename base_type::null_type null_type; \
		\
		typedef typename tpl_arg_type::result_type result_type; \
		\
		typedef boost::mpl::false_ is_callable_type; \
		typedef boost::mpl::false_ is_native_foo_type; \
		typedef typename base_type::is_member_foo_type is_member_foo_type; \
		typedef typename base_type::has_va_list_type has_va_list_type; \
		\
		typedef typename base_type::class_type class_type; \
		typedef typename base_type::arg_list_type arg_list_type; \
		typedef typename base_type::arg_holder_list_type arg_holder_list_type; \
		typedef typename base_type::is_const_type is_const_type; \
		typedef typename base_type::is_volatile_type is_volatile_type; \
		typedef typename base_type::is_noexcept_type is_noexcept_type; \
		typedef typename base_type::arg_list_size_type arg_list_size_type; \
		typedef typename base_type::arg_holder_list_size_type arg_holder_list_size_type; \
		typedef typename base_type::foo_type foo_type; \
		typedef typename base_type::foo_pointer_type foo_pointer_type; \
		typedef typename base_type::foo_reference_type foo_reference_type; \
		typedef typename base_type::native_foo_type native_foo_type; \
		typedef typename base_type::native_foo_pointer_type native_foo_pointer_type; \
		typedef typename base_type::native_foo_reference_type native_foo_reference_type; \
		typedef null_type type; };


#if defined(BOOST_NO_VOID_RETURNS)
#	define YGGR_MEM_FN_CLASS_F() , typename F
#	define YGGR_MEM_FN_F() , F
#else
#	define YGGR_MEM_FN_CLASS_F()
#	define YGGR_MEM_FN_F()
#endif // BOOST_NO_VOID_RETURNS

#define YGGR_TMP_PP_SYMBOL_CONST() const
//#define YGGR_TMP_PP_SYMBOL_VOLATILE() volatile

#define YGGR_BIND_MF_NAME(X) X
#define YGGR_MEM_FN_CC()

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_FOO_BOOST_FUNCTION_T_INFO_N( __n__ )

#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE(  )

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_FOOT_T_BOOST__MF_N( __n__, 0 ) \
			YGGR_FOOT_T_BOOST__MF_N( __n__, 1 )

#define BOOST_PP_LOCAL_LIMITS ( 0, 8 )
#include BOOST_PP_LOCAL_ITERATE(  )

#undef YGGR_MEM_FN_CC
#undef YGGR_BIND_MF_NAME

#undef YGGR_TMP_PP_SYMBOL_CONST

#undef YGGR_MEM_FN_CLASS_F
#undef YGGR_MEM_FN_F

#undef YGGR_FOO_BOOST_FUNCTION_T_INFO_N
#undef YGGR_FOOT_T_BOOST__MF_N

// boost::function
template<typename Signature>
struct foo_t_info_detail< boost::function<Signature> >
{
private:
	typedef foo_t_info_detail<Signature> base_type;
	typedef boost::function<Signature> tpl_arg_type;

public:
	typedef typename base_type::null_type null_type;

	typedef typename base_type::is_callable_type is_callable_type;
	typedef boost::mpl::false_ is_native_foo_type;
	typedef typename base_type::is_member_foo_type is_member_foo_type;
	typedef typename base_type::has_va_list_type has_va_list_type;

	typedef typename base_type::result_type result_type;
	typedef typename base_type::class_type class_type;
	typedef typename base_type::arg_list_type arg_list_type;
	typedef typename base_type::arg_holder_list_type arg_holder_list_type;
	typedef typename base_type::is_const_type is_const_type;
	typedef typename base_type::is_volatile_type is_volatile_type;
	typedef typename base_type::is_noexcept_type is_noexcept_type;
	typedef typename base_type::arg_list_size_type arg_list_size_type;
	typedef typename base_type::arg_holder_list_size_type arg_holder_list_size_type;

	typedef tpl_arg_type foo_type;
	typedef typename boost::add_pointer<tpl_arg_type>::type foo_pointer_type;
	typedef typename boost::add_reference<tpl_arg_type>::type foo_reference_type;

	typedef typename base_type::native_foo_type native_foo_type;
	typedef typename base_type::native_foo_pointer_type native_foo_pointer_type;
	typedef typename base_type::native_foo_reference_type native_foo_reference_type;

	typedef foo_type type;
};

// bind
template<typename R, typename F, typename L>
struct foo_t_info_detail< boost::_bi::bind_t<R, F, L> >
{
private:
	typedef foo_t_info_detail<F> base_type;
	typedef boost::_bi::bind_t<R, F, L> tpl_arg_type;

public:
	typedef typename base_type::null_type null_type;

	typedef typename tpl_arg_type::result_type result_type;

	typedef boost::mpl::true_ is_callable_type;
	typedef boost::mpl::false_ is_native_foo_type;
	typedef typename base_type::is_member_foo_type is_member_foo_type;
	typedef typename base_type::has_va_list_type has_va_list_type;

	typedef typename base_type::class_type class_type;

	typedef typename base_type::arg_list_type arg_list_type;

private:
	typedef typename ::yggr::func::bind_av_list_cast<tpl_arg_type>::type av_list_type;

	typedef typename
		boost::mpl::copy_if
		<
			av_list_type,
			detail::is_boost_arg<boost::mpl::_>,
			boost::mpl::back_inserter< boost::mpl::vector<> >
		>::type nord_av_arg_list_type;

	typedef typename
		boost::mpl::sort
		<
			nord_av_arg_list_type,
			detail::boost_arg_less<boost::mpl::_1, boost::mpl::_2>
		>::type ord_av_arg_list_type;

	typedef typename
		boost::mpl::if_
		<
			is_member_foo_type,
			typename
				boost::mpl::push_front
				<
					arg_list_type,
					tag_object_reference
					<
						typename
							boost::mpl::if_
							<
								typename base_type::is_const_type,
								const class_type,
								class_type
							>::type
					>
				>::type,
			arg_list_type
		>::type av_arg_list_type;

public:
	typedef typename
		mplex::revert_to_vector
		<
			typename
				boost::mpl::transform
				<
					ord_av_arg_list_type,
					detail::conv_to_arg
					<
						boost::mpl::_1,
						av_list_type,
						av_arg_list_type
					>
				>::type 
		>::type arg_holder_list_type;

	typedef typename base_type::is_const_type is_const_type;
	typedef typename base_type::is_volatile_type is_volatile_type;
	typedef typename base_type::is_noexcept_type is_noexcept_type;
	typedef typename base_type::arg_list_size_type arg_list_size_type;
	typedef boost::mpl::size_t<boost::mpl::size<arg_holder_list_type>::value> arg_holder_list_size_type;

	typedef tpl_arg_type foo_type;
	typedef typename boost::add_pointer<foo_type>::type foo_pointer_type;
	typedef typename boost::add_reference<foo_type>::type foo_reference_type;

	typedef typename base_type::foo_type native_foo_type;
	typedef typename base_type::foo_pointer_type native_foo_pointer_type;
	typedef typename base_type::foo_reference_type native_foo_reference_type;

	typedef foo_type type;
};



} // namespace detail

template<typename T>
struct foo_t_info
{
protected:
	typedef detail::foo_t_info_detail<T> base_type;

public:
	typedef typename base_type::null_type null_type;

	typedef typename base_type::is_callable_type is_callable_type;
	typedef typename base_type::is_native_foo_type is_native_foo_type;
	typedef typename base_type::is_member_foo_type is_member_foo_type;
	typedef typename base_type::has_va_list_type has_va_list_type;

	typedef typename base_type::result_type result_type;
	typedef typename base_type::class_type class_type;
	typedef typename base_type::arg_list_type arg_list_type;
	typedef typename base_type::arg_holder_list_type arg_holder_list_type;
	typedef typename base_type::is_const_type is_const_type;
	typedef typename base_type::is_volatile_type is_volatile_type;
	typedef typename base_type::is_noexcept_type is_noexcept_type;
	typedef typename base_type::arg_list_size_type arg_list_size_type;
	typedef typename base_type::arg_holder_list_size_type arg_holder_list_size_type;

	typedef typename base_type::foo_type foo_type;
	typedef typename base_type::foo_pointer_type foo_pointer_type;
	typedef typename base_type::foo_reference_type foo_reference_type;

	typedef typename base_type::native_foo_type native_foo_type;
	typedef typename base_type::native_foo_pointer_type native_foo_pointer_type;
	typedef typename base_type::native_foo_reference_type native_foo_reference_type;

	typedef typename base_type::type type;

	YGGR_STATIC_CONSTANT(std::size_t, arg_list_size = arg_list_size_type::value);
	YGGR_STATIC_CONSTANT(std::size_t, arg_holder_list_size = arg_holder_list_size_type::value);

	YGGR_STATIC_CONSTANT(bool, is_callable = is_callable_type::value);
	YGGR_STATIC_CONSTANT(bool, is_native_foo = is_native_foo_type::value);
	YGGR_STATIC_CONSTANT(bool, is_member_foo = is_member_foo_type::value);
	YGGR_STATIC_CONSTANT(bool, has_va_list = has_va_list_type::value);

	YGGR_STATIC_CONSTANT(bool, is_const = is_const_type::value);
	YGGR_STATIC_CONSTANT(bool, is_volatile = is_volatile_type::value);

	template<std::size_t N>
	struct arg
		: public
			boost::mpl::if_
			<
				boost::is_same<arg_list_type, null_type>,
				null_type,
				typename mplex::get_arg<arg_list_type, N, null_type>::type
			>
	{
	};

	template<std::size_t N>
	struct arg_holder
		: public
			boost::mpl::if_
			<
				boost::is_same<arg_holder_list_type, null_type>,
				null_type,
				typename mplex::get_arg<arg_holder_list_type, N, null_type>::type
			>
	{
	};

};

} // namespace func
} // namespace yggr

#include <typeinfo>
#include <iostream>

namespace yggr
{
namespace func
{

template<typename Char, typename Traits, typename T>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const foo_t_info<T>&)
{
	typedef foo_t_info<T> foo_info_type;

	typedef typename foo_info_type::is_callable_type is_callable_type;
	typedef typename foo_info_type::is_native_foo_type is_native_foo_type;
	typedef typename foo_info_type::is_member_foo_type is_member_foo_type;
	typedef typename foo_info_type::has_va_list_type has_va_list_type;

	typedef typename foo_info_type::foo_type foo_type;
	typedef typename foo_info_type::foo_pointer_type foo_pointer_type;
	typedef typename foo_info_type::foo_reference_type foo_reference_type;

	typedef typename foo_info_type::native_foo_type native_foo_type;
	typedef typename foo_info_type::native_foo_pointer_type native_foo_pointer_type;
	typedef typename foo_info_type::native_foo_reference_type native_foo_reference_type;

	typedef typename foo_info_type::type type;

	typedef typename foo_info_type::result_type result_type;
	typedef typename foo_info_type::class_type class_type;
	typedef typename foo_info_type::arg_list_type arg_list_type;
	typedef typename foo_info_type::arg_holder_list_type arg_holder_list_type;
	typedef typename foo_info_type::is_const_type is_const_type;
	typedef typename foo_info_type::is_volatile_type is_volatile_type;
	typedef typename foo_info_type::is_noexcept_type is_noexcept_type;
	typedef typename foo_info_type::arg_list_size_type arg_list_size_type;
	typedef typename foo_info_type::arg_holder_list_size_type arg_holder_list_size_type;

	os	<< "-------------------------------------------------------------\n"
		<< "is_callable_type = { " << typeid(is_callable_type).name() << " };\n"
		<< "is_native_foo_type = { " << typeid(is_native_foo_type).name() << " };\n"
		<< "is_member_foo_type = { " << typeid(is_member_foo_type).name() << " };\n"
		<< "has_va_list_type = { " << typeid(has_va_list_type).name() << " };\n\n"

		<< "foo_type = { " << typeid(foo_type).name() << " };\n"
		<< "foo_pointer_type = { " << typeid(foo_pointer_type).name() << " };\n"
		<< "foo_reference_type = { " << typeid(foo_reference_type).name() << " };\n\n"

		<< "native_foo_type = { " << typeid(native_foo_type).name() << " };\n"
		<< "native_foo_pointer_type = { " << typeid(native_foo_pointer_type).name() << " };\n"
		<< "native_foo_reference_type = { " << typeid(native_foo_reference_type).name() << " };\n\n"

		<< "type = { " << typeid(type).name() << " };\n\n"

		<< "result_type = { " << typeid(result_type).name() << " };\n"
		<< "class_type = { " << typeid(class_type).name() << " };\n"
		<< "arg_list_type = { " << typeid(arg_list_type).name() << " };\n"
		<< "arg_holder_list_type = { " << typeid(arg_holder_list_type).name() << " };\n"
		<< "is_const_type = { " << typeid(is_const_type).name() << " };\n"
		<< "is_volatile_type = { " << typeid(is_volatile_type).name() << " };\n"
		<< "is_volatile_type = { " << typeid(is_noexcept_type).name() << " };\n"
		<< "arg_list_size_type = { " << typeid(arg_list_size_type).name() << " };\n"
		<< "arg_holder_list_size_type = { " << typeid(arg_holder_list_size_type).name() << " };\n"
		<< "-------------------------------------------------------------\n";

	return os;
}

} // namespace func
} // namespace yggr

#endif // __YGGR_FUNC_FOO_T_INFO_HPP__
