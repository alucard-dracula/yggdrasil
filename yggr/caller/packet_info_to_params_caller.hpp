// packet_info_to_params_caller.hpp

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

#ifndef __YGGR_CALLER_PACKET_INFO_TO_PARAMS_CALLER_HPP__
#define __YGGR_CALLER_PACKET_INFO_TO_PARAMS_CALLER_HPP__

#include <yggr/packet/packet_info.hpp>
#include <yggr/caller/tuple_to_params_caller.hpp>

namespace yggr
{
namespace caller
{

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename Handler, typename ...T> inline
typename 
	boost::enable_if
	<
		is_callable<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, packet::packet_info<T...>& t)
{
	typedef packet::packet_info<T...> packet_info_type;
	typedef typename packet_info_type::value_type tuple_type;

	tuple_type& tuple = static_cast<tuple_type&>(t);
	return to_params_call(handler, tuple);
}

template<typename Handler, typename ...T> inline
typename 
	boost::enable_if
	<
		is_callable<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, const packet::packet_info<T...>& t)
{
	typedef packet::packet_info<T...> packet_info_type;
	typedef typename packet_info_type::value_type tuple_type;

	const tuple_type& tuple = static_cast<const tuple_type&>(t);
	return to_params_call(handler, tuple);
}

#else

template<typename Handler, YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T)> inline
typename 
	boost::enable_if
	<
		is_callable<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, 
					packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>& t)
{
	typedef packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> packet_info_type;
	typedef typename packet_info_type::value_type tuple_type;
	
	tuple_type& tuple = static_cast<tuple_type&>(t);
	return to_params_call(handler, tuple);
}

template<typename Handler, YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T)> inline
typename 
	boost::enable_if
	<
		is_callable<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, 
					const packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>& t)
{
	typedef packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> packet_info_type;
	typedef typename packet_info_type::value_type tuple_type;
	
	const tuple_type& tuple = static_cast<const tuple_type&>(t);
	return to_params_call(handler, tuple);
}

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace caller
} // namespace yggr


#endif // __YGGR_CALLER_PACKET_INFO_TO_PARAMS_CALLER_HPP__
