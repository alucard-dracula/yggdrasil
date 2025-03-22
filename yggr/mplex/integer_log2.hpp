//integer_log2.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_MPLEX_INTEGER_LOG2_HPP__
#define __YGGR_MPLEX_INTEGER_LOG2_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/null_t.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size_t.hpp>

namespace yggr
{
namespace mplex
{

namespace detail
{

typedef null_type interger_log2_failed;

template<std::size_t N>
struct integer_log2_chk;

template<std::size_t N>
struct integer_log2_chk
	: public 
		boost::mpl::if_c
		<
			0 == N % 2,
			typename integer_log2_chk<(N >> 1)>::type,
			boost::mpl::bool_<false>
		>::type
{
};

template<>
struct integer_log2_chk<1>
	: public boost::mpl::true_
{
};

template<>
struct integer_log2_chk<0>
	: public boost::mpl::false_
{
};

template<typename Chk, std::size_t N>
struct integer_log2_cal;

template<std::size_t N>
struct integer_log2_cal<boost::mpl::false_, N>
	: public interger_log2_failed
{
};

template<std::size_t N>
struct integer_log2_cal<boost::mpl::true_, N>
	: public boost::mpl::size_t<(1 + integer_log2_cal<boost::mpl::true_, (N >> 1)>::type::value)>
{
};

template<>
struct integer_log2_cal<boost::mpl::true_, 1>
	: public boost::mpl::size_t<0>
{
};

} // namespace detail

using detail::interger_log2_failed;

template<std::size_t N>
struct integer_log2
	: public
		detail::integer_log2_cal
		<
			typename detail::integer_log2_chk<N>::type,
			N
		>::type
{
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_INTEGER_LOG2_HPP__
