//array_to_params_caller.hpp

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

#ifndef __YGGR_CALLER_ARRAY_TO_PARAMS_CALLER_HPP__
#define __YGGR_CALLER_ARRAY_TO_PARAMS_CALLER_HPP__

#include <yggr/caller/detail/caller_callable.hpp>
#include <yggr/container/array.hpp>

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	include <yggr/type_traits/integer_sequence.hpp>
#else
#	include <yggr/ppex/foo_params.hpp>
#	include <yggr/ppex/symbols.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <yggr/utility/to_pointer.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <cassert>
#include <stdexcept>

namespace yggr
{
namespace caller
{
namespace detail
{

template<typename Handler, std::size_t N, 
			bool is_callable = func::foo_t_info<Handler>::is_callable_type::value,
			bool is_native_foo = func::foo_t_info<Handler>::is_native_foo_type::value,
			bool is_mem_foo = func::foo_t_info<Handler>::is_member_foo_type::value >
struct array_to_params_call_helper;

} // namepsce detail

#ifndef YGGR_NO_CXX11_HDR_ARRAY

template<typename Handler, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		detail::is_caller_callable_foo<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, std::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	h_type h;
	return h(handler, arr);
}

template<typename Handler, typename Obj, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		typename
			boost::mpl::and_
			<
				detail::is_caller_callable_native_mem_foo<Handler>,
				boost::mpl::not_<typename boost::is_pointer<Obj>::type>
			>::type,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, Obj& obj, std::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	h_type h;
	return h(handler, obj, arr);
}

template<typename Handler, typename Obj, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		typename
			boost::mpl::and_
			<
				detail::is_caller_callable_native_mem_foo<Handler>,
				boost::is_pointer<Obj>
			>::type,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, Obj pobj, std::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	assert(pobj);
	if(!pobj)
	{
		throw std::invalid_argument("Invalid argument encountered, object pointer is null!");
	}

	h_type h;
	return h(handler, pobj, arr);
}

template<typename Handler, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		detail::is_caller_callable_foo<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, const std::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	h_type h;
	return h(handler, arr);
}

template<typename Handler, typename Obj, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		typename
			boost::mpl::and_
			<
				detail::is_caller_callable_native_mem_foo<Handler>,
				boost::mpl::not_<typename boost::is_pointer<Obj>::type>
			>::type,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, Obj& obj, const std::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	h_type h;
	return h(handler, obj, arr);
}

template<typename Handler, typename Obj, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		typename
			boost::mpl::and_
			<
				detail::is_caller_callable_native_mem_foo<Handler>,
				boost::is_pointer<Obj>
			>::type,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, Obj pobj, const std::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	assert(pobj);
	if(!pobj)
	{
		throw std::invalid_argument("Invalid argument encountered, object pointer is null!");
	}

	h_type h;
	return h(handler, pobj, arr);
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

template<typename Handler, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		detail::is_caller_callable_foo<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, boost::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	h_type h;
	return h(handler, arr);
}

template<typename Handler, typename Obj, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		typename
			boost::mpl::and_
			<
				detail::is_caller_callable_native_mem_foo<Handler>,
				boost::mpl::not_<typename boost::is_pointer<Obj>::type>
			>::type,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, Obj& obj, boost::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	h_type h;
	return h(handler, obj, arr);
}

template<typename Handler, typename Obj, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		typename
			boost::mpl::and_
			<
				detail::is_caller_callable_native_mem_foo<Handler>,
				boost::is_pointer<Obj>
			>::type,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, Obj pobj, boost::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	assert(pobj);
	if(!pobj)
	{
		throw std::invalid_argument("Invalid argument encountered, object pointer is null!");
	}

	h_type h;
	return h(handler, pobj, arr);
}

template<typename Handler, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		detail::is_caller_callable_foo<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, const boost::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	h_type h;
	return h(handler, arr);
}

template<typename Handler, typename Obj, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		typename
			boost::mpl::and_
			<
				detail::is_caller_callable_native_mem_foo<Handler>,
				boost::mpl::not_<typename boost::is_pointer<Obj>::type>
			>::type,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, Obj& obj, const boost::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	h_type h;
	return h(handler, obj, arr);
}

template<typename Handler, typename Obj, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		typename
			boost::mpl::and_
			<
				detail::is_caller_callable_native_mem_foo<Handler>,
				boost::is_pointer<Obj>
			>::type,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, Obj pobj, const boost::array<Val, N>& arr)
{
	typedef Handler handler_type;
	typedef detail::array_to_params_call_helper<handler_type, N> h_type;

	assert(pobj);
	if(!pobj)
	{
		throw std::invalid_argument("Invalid argument encountered, object pointer is null!");
	}

	h_type h;
	return h(handler, pobj, arr);
}

} // namespace caller
} // namespace yggr

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

namespace yggr
{
namespace caller
{
namespace detail
{

template<typename Handler, std::size_t N, bool is_callable, bool is_native_foo, bool is_mem_foo>
struct array_to_params_call_helper
{
public:
	typedef Handler handler_type;

private:
	typedef array_to_params_call_helper this_type;

private:
	template<typename Array, std::size_t ...I> inline
	typename func::foo_t_info<handler_type>::result_type
		operator_detail(const handler_type& handler, Array& arr, ::yggr::index_sequence<I...>) const
	{
		return (handler)(arr[I]...);
	}

public:
	template<typename Array> inline
	typename func::foo_t_info<handler_type>::result_type
		operator()(const handler_type& handler, Array& arr) const
	{
		return this_type::operator_detail(handler, arr, yggr::make_index_sequence<N>{});
	}

};

template<typename Handler, std::size_t N>
struct array_to_params_call_helper<Handler, N, true, true, true>
{
public:
	typedef Handler handler_type;

private:
	typedef array_to_params_call_helper this_type;

private:
	template<typename Obj, typename Array, std::size_t ...I> inline
	typename func::foo_t_info<Handler>::result_type
		operator_detail(handler_type handler, Obj& obj, Array& arr, ::yggr::index_sequence<I...>) const
	{
		return (utility::to_pointer(obj)->*handler)(arr[I]...);
	}

public:
	template<typename Obj, typename Array> inline
	typename func::foo_t_info<handler_type>::result_type
		operator()(handler_type handler, Obj& obj, Array& arr) const
	{
		return this_type::operator_detail(handler, obj, arr, yggr::make_index_sequence<N>{});
	}

};

} // namespace detail
} // namespace caller
} // namespace yggr

#else

// _YGGR_TMP_PP_ARRAY_SUB_GET(__n__, __val__)
// _YGGR_TMP_PP_ARRAY_VAL ()
#	define _YGGR_TMP_PP_ARRAY_ARGS_GET( __z__, __n__, __op__ ) \
			YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
			_YGGR_TMP_PP_ARRAY_SUB_GET( \
				__n__, YGGR_PP_OPEN( _YGGR_TMP_PP_ARRAY_VAL  ) )

#	define YGGR_TMP_PP_ARRAY_TO_PARAMS_CALL_HELPER( __n__ ) \
		namespace yggr{ namespace caller { namespace detail { \
		template<typename Handler, bool is_callable, bool is_native_foo, bool is_mem_foo> \
		struct array_to_params_call_helper< Handler, __n__, is_callable, is_native_foo, is_mem_foo > { \
		public: \
			typedef Handler handler_type; \
		public: \
			template<typename Array> inline \
			typename func::foo_t_info<handler_type>::result_type \
				operator()(const handler_type& handler, Array& _YGGR_TMP_PP_ARRAY_VAL()) const { \
					return (handler)(YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA )); } }; \
		\
		template<typename Handler> \
		struct array_to_params_call_helper< Handler, __n__, true, true, true > { \
		public: \
			typedef Handler handler_type; \
		public: \
			template<typename Obj, typename Array> inline \
			typename func::foo_t_info<handler_type>::result_type \
				operator()(const handler_type& handler, Obj& obj, Array& _YGGR_TMP_PP_ARRAY_VAL()) const { \
					return (utility::to_pointer(obj)->*handler)(YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA )); } };}}}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_PP_ARRAY_TO_PARAMS_CALL_HELPER( __n__ )

#	define _YGGR_TMP_PP_ARRAY_SUB_GET( __n__, __val__ ) __val__[ __n__ ]

#	define _YGGR_TMP_PP_ARRAY_VAL() arr
#	define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_ARRAY_ARGS_GET


#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef _YGGR_TMP_PP_ARRAY_SUB_GET
#	undef _YGGR_TMP_PP_ARRAY_VAL
#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_TMP_PP_ARRAY_TO_PARAMS_CALL_HELPER
#	undef _YGGR_TMP_PP_ARRAY_ARGS_GET

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#endif // __YGGR_CALLER_ARRAY_TO_PARAMS_CALLER_HPP__
