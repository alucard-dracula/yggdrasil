//is_functional.hpp

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

#ifndef __YGGR_FUNC_IS_FUNCTIONAL_HPP__
#define __YGGR_FUNC_IS_FUNCTIONAL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>
#include <yggr/func/has_functional_member_t.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <functional>

namespace yggr
{
namespace func
{

#if YGGR_CPP_VERSION < YGGR_CPP_VER_17

namespace detail
{

template<typename T, int N, bool Chk>
struct make_base_functional
	: public mplex::null_type
{
};

template<typename T>
struct make_base_functional<T, 1, true>
{
	typedef 
		std::unary_function
		<
			typename T::argument_type, 
			typename T::result_type
		> type;
};

template<typename T>
struct make_base_functional<T, 2, true>
{
	typedef 
		std::binary_function
		<
			typename T::first_argument_type, 
			typename T::second_argument_type, 
			typename T::result_type
		> type;
};

} // namespace detail

template<typename T>
struct is_base_of_unary_function
	: public 
		boost::mpl::if_
		<
			boost::is_base_of<mplex::null_type, T>,
			boost::mpl::false_,
			typename 
				boost::is_base_of
				<
					typename 
						detail::make_base_functional
						<
							T, 1, 
							boost::mpl::and_
							<
								has_argument_t<T>,
								has_result_t<T>
							>::type::value
						>::type,
					T
				>::type
		>::type
{
};

template<typename T>
struct is_base_of_binary_function
	: public 
		boost::mpl::if_
		<
			boost::is_base_of<mplex::null_type, T>,
			boost::mpl::false_,
			typename
				boost::is_base_of
				<
					typename 
						detail::make_base_functional
						<
							T, 2, 
							boost::mpl::and_
							<
								has_first_argument_t<T>,
								has_second_argument_t<T>,
								has_result_t<T>
							>::type::value
						>::type,
					T
				>::type
		>::type
{
};

template<typename T>
struct is_functional
	: public
		boost::mpl::or_
		<
			is_base_of_unary_function<T>,
			is_base_of_binary_function<T>
		>::type
{
};

#else

template<typename T>
struct is_base_of_unary_function
	: public boost::mpl::false_
{
};

template<typename T>
struct is_base_of_binary_function
	: public boost::mpl::false_
{
};

template<typename T>
struct is_functional
	: public boost::mpl::false_
{
};

#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17

} // namespace func

using func::is_base_of_unary_function;
using func::is_base_of_binary_function;
using func::is_functional;

} // namespace yggr

#endif // __YGGR_FUNC_IS_FUNCTIONAL_HPP__
