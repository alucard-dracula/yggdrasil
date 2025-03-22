// pfto_compatible.hpp

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

#ifndef __YGGR_ARCHIVE_DETAIL_PFTO_COMPATIBLE_HPP__
#define __YGGR_ARCHIVE_DETAIL_PFTO_COMPATIBLE_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 105900)

#if !defined(BOOST_MAKE_PFTO_WRAPPER)
#	define BOOST_MAKE_PFTO_WRAPPER( __x__ ) __x__
#endif // BOOST_MAKE_PFTO_WRAPPER

#endif // BOOST_VERSION < 105900

#endif // __YGGR_ARCHIVE_DETAIL_PFTO_COMPATIBLE_HPP__

