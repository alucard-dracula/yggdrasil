// operator_result_stream_patch.hpp

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

#ifndef __YGGR_TYPE_TRAITS_OPERATOR_RESUTL_STREAM_PATH_HPP__
#define __YGGR_TYPE_TRAITS_OPERATOR_RESUTL_STREAM_PATH_HPP__

#include <yggr/type_traits/operator_result.hpp>

#if !defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)


#include <iostream>

namespace yggr
{

template<typename Char, typename Traits, typename R>
struct bitwise_left_shift_operator_result< std::basic_ostream<Char, Traits>, R >
{
public:
    typedef std::basic_ostream<Char, Traits>& type;
};

template<typename Char, typename Traits, typename R>
struct bitwise_right_shift_operator_result< std::basic_istream<Char, Traits>, R >
{
public:
    typedef std::basic_istream<Char, Traits>& type;
};

} // namespace yggr

#endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#endif // __YGGR_TYPE_TRAITS_OPERATOR_RESUTL_STREAM_PATH_HPP__
