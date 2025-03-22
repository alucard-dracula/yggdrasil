//pp_debug.hpp

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

#ifndef __YGGR_PPEX_PP_DEBUG_HPP__
#define __YGGR_PPEX_PP_DEBUG_HPP__

#include <yggr/base/yggrdef.h>

#if defined(_DEBUG) || defined(YGGR_TEST_PP_DEBUG)

#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>

#define YGGR_PP_DEBUG( __x__ ) YGGR_PP_CAST( ( __x__ ), YGGR_PP_CAST_TO_STRING )
#define YGGR_PP_WDEBUG( __x__ ) YGGR_PP_CAST( ( __x__ ), YGGR_PP_CAST_TO_WSTRING )
#define YGGR_PP_DEBUG_AORW( __x__, __a_or_w__ ) YGGR_PP_CAST( ( __x__ ), __a_or_w__ )

#else

#error "release version do not use YGGR_PP_DEBUG series"

#endif // _DEBUG

// use:
// YGGR_PP_DEBUG(macro)
// YGGR_PP_WDEBUG(macro)
// YGGR_PP_DEBUG_AORW(macro, YGGR_PP_CAST_TO_STRING)

namespace yggr
{
namespace ppex
{
} // namespace ppex
} // namespace yggr

#endif // __YGGR_PPEX_PP_DEBUG_HPP__
