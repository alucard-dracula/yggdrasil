// pair_to_params_caller.hpp

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

#ifndef __YGGR_CALLER_PAIR_TO_PARAMS_CALLER_HPP__
#define __YGGR_CALLER_PAIR_TO_PARAMS_CALLER_HPP__

#include <yggr/caller/detail/caller_callable.hpp>

#include <yggr/utility/to_pointer.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <utility>
#include <cassert>

namespace yggr
{
namespace caller
{

template<typename Handler, typename T1, typename T2> inline
typename 
	boost::enable_if
	<
		detail::is_caller_callable_foo<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, std::pair<T1, T2>& params)
{
	return (handler)(params.first, params.second);
}

template<typename Handler, typename Obj, typename T1, typename T2> inline
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
	to_params_call(const Handler& handler, Obj& obj, std::pair<T1, T2>& params)
{
	return (utility::to_pointer(obj)->*handler)(params.first, params.second);
}

template<typename Handler, typename Obj, typename T1, typename T2> inline
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
	to_params_call(const Handler& handler, Obj pobj, std::pair<T1, T2>& params)
{
	assert(pobj);
	if(!pobj)
	{
		throw std::invalid_argument("Invalid argument encountered, object pointer is null!");
	}

	return (pobj->*handler)(params.first, params.second);
}

template<typename Handler, typename T1, typename T2> inline
typename 
	boost::enable_if
	<
		detail::is_caller_callable_foo<Handler>,
		typename func::foo_t_info<Handler>::result_type
	>::type
	to_params_call(const Handler& handler, const std::pair<T1, T2>& params)
{
	return (handler)(params.first, params.second);
}

template<typename Handler, typename Obj, typename T1, typename T2> inline
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
	to_params_call(const Handler& handler, Obj& obj, const std::pair<T1, T2>& params)
{
	return (utility::to_pointer(obj)->*handler)(params.first, params.second);
}

template<typename Handler, typename Obj, typename T1, typename T2> inline
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
	to_params_call(const Handler& handler, Obj pobj, const std::pair<T1, T2>& params)
{
	assert(pobj);
	if(!pobj)
	{
		throw std::invalid_argument("Invalid argument encountered, object pointer is null!");
	}

	return (pobj->*handler)(params.first, params.second);
}


} // namespace caller
} // namespace yggr

#endif // __YGGR_CALLER_PAIR_TO_PARAMS_CALLER_HPP__
