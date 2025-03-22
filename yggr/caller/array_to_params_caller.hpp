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

#include <yggr/base/yggrdef.h>
#include <yggr/container/array.hpp>

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	include <yggr/type_traits/integer_sequence.hpp>
#else
#	include <yggr/ppex/foo_params.hpp>
#	include <yggr/ppex/symbols.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <yggr/func/is_callable.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace caller
{
namespace detail
{

template<std::size_t>
struct array_to_params_call_helper;

} // namepsce detail

#ifndef YGGR_NO_CXX11_HDR_ARRAY

template<typename Handler, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		is_callable<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, std::array<Val, N>& arr)
{
	typedef detail::array_to_params_call_helper<N> h_type;

	h_type h;
	return h(handler, arr);
}

template<typename Handler, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		is_callable<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, const std::array<Val, N>& arr)
{
	typedef detail::array_to_params_call_helper<N> h_type;

	h_type h;
	return h(handler, arr);
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

template<typename Handler, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		is_callable<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, boost::array<Val, N>& arr)
{
	typedef detail::array_to_params_call_helper<N> h_type;

	h_type h;
	return h(handler, arr);
}

template<typename Handler, typename Val, std::size_t N> inline
typename
	boost::enable_if
	<
		is_callable<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, const boost::array<Val, N>& arr)
{
	typedef detail::array_to_params_call_helper<N> h_type;

	h_type h;
	return h(handler, arr);
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

template<std::size_t N>
struct array_to_params_call_helper
{
private:
	typedef array_to_params_call_helper this_type;

private:
	template<typename Handler, typename Array, std::size_t ...I> inline
	typename
		boost::enable_if
		<
			is_callable<Handler>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		operator_detail(const Handler& handler, Array& arr, ::yggr::index_sequence<I...>) const
	{
		return (handler)(arr[I]...);
	}

public:
	template<typename Handler, typename Array> inline
	typename
		boost::enable_if
		<
			is_callable<Handler>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		operator()(const Handler& handler, Array& arr) const
	{
		return this_type::operator_detail(handler, arr, yggr::make_index_sequence<N>{});
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
		template<> \
		struct array_to_params_call_helper< __n__ > { \
			template<typename Handler, typename Array> inline \
			typename boost::enable_if<is_callable<Handler>, typename func::foo_t_info<Handler>::result_type >::type \
				operator()(const Handler& handler, Array& _YGGR_TMP_PP_ARRAY_VAL()) const { \
					return (handler)(YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER( __n__, YGGR_PP_SYMBOL_COMMA )); } }; }}}

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
