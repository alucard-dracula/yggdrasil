//concat.hpp

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

#ifndef __YGGR_PPEX_CONCAT_HPP__
#define __YGGR_PPEX_CONCAT_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define YGGR_PP_CONCAT(...) __VA_ARGS__
#else
//	self def __args__
#	define YGGR_PP_CONCAT( __args__ ) __args__
#endif // YGGR_NO_CXX11_VARIADIC_MACROS


namespace yggr
{
namespace ppex
{
} // namespace ppex
} // namespace yggr

#endif //__YGGR_PPEX_CONCAT_HPP__
