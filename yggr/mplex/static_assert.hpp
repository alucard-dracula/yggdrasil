// assert.hpp (mplex/assert.hpp)

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

#ifndef __YGGR_MPLEX_STATIC_ASSERT_HPP__
#define __YGGR_MPLEX_STATIC_ASSERT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/concat.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#include <cassert>

// BOOST_MPL_ASSERT not support constexpr, static_assert<false> not support no used template class

#if defined(__cpp_static_assert)
#	if !(__cpp_static_assert) < 200410
#		define YGGR_MPLEX_STATIC_ASSERT( __x__ ) static_assert( yggr::traits< YGGR_PP_CONCAT __x__ >::type::value )
#	else
#		define YGGR_MPLEX_STATIC_ASSERT BOOST_MPL_ASSERT
#	endif // !(__cpp_static_assert) < 200410
#else
#	define YGGR_MPLEX_STATIC_ASSERT BOOST_MPL_ASSERT
#endif // #if defined(__cpp_static_assert)

#if defined(_MSC_VER)
#	define YGGR_MPLEX_FXIED_ASSERT_FALSE() BOOST_MPL_ASSERT((boost::mpl::false_))
#else
#	define YGGR_MPLEX_FXIED_ASSERT_FALSE() assert((false)) // gcc still check BOOST_MPL_ASSERT((boost::mpl::false_))
#endif // _MSC_VER

#endif // __YGGR_MPLEX_STATIC_ASSERT_HPP__
