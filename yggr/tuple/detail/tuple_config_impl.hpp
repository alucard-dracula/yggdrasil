//tuple_config_impl.hpp

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

#ifndef __YGGR_TUPLE_DETAIL_TUPLE_CONFIG_IMPL_HPP__
#define __YGGR_TUPLE_DETAIL_TUPLE_CONFIG_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

// if you modify boost tuple params length please define this macro
#ifndef BOOST_TUPLE_LIMIT_LENGTH_CFG
#	define BOOST_TUPLE_LIMIT_LENGTH_CFG 10
#endif // BOOST_TUPLE_LIMIT_LENGTH_CFG

#define BOOST_TUPLE_LIMIT_LENGTH() BOOST_TUPLE_LIMIT_LENGTH_CFG

#if (BOOST_TUPLE_LIMIT_LENGTH() < 2)
#	error "the boost tuple length not less 2 !!!!!!"
#endif // YGGR_TUPLE_LIMIT_LENGTH

#define YGGR_TUPLE_LIMIT_DEFAULT_LENGTH() BOOST_TUPLE_LIMIT_LENGTH()

#ifndef YGGR_TUPLE_LIMIT_LENGTH_CFG
#	define YGGR_TUPLE_LIMIT_LENGTH_CFG YGGR_TUPLE_LIMIT_DEFAULT_LENGTH()
#endif // YGGR_TUPLE_LIMIT_LENGTH

#define YGGR_TUPLE_LIMIT_LENGTH() YGGR_TUPLE_LIMIT_LENGTH_CFG

#if (YGGR_TUPLE_LIMIT_LENGTH() < 2)
#	error "the yggr tuple length not less 2 !!!!!!"
#endif // YGGR_TUPLE_LIMIT_LENGTH


namespace yggr
{
namespace tuples
{

struct tuple_cfg
{
	YGGR_STATIC_CONSTANT(std::size_t, E_boost_tuple_limit_length = BOOST_TUPLE_LIMIT_LENGTH());
	YGGR_STATIC_CONSTANT(std::size_t, E_yggr_tuple_limit_length = YGGR_TUPLE_LIMIT_LENGTH());
};

} // namespace tuple
} // namespace yggr

#endif // __YGGR_TUPLE_DETAIL_TUPLE_CONFIG_IMPL_HPP__