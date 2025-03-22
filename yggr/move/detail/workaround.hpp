// workaround.hpp

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

#ifndef __YGGR_MOVE_DETAIL_WORKAROUND_HPP__
#define __YGGR_MOVE_DETAIL_WORKAROUND_HPP__

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

//#define BOOST_MOVE_DISABLE_FORCEINLINE
//#define BOOST_MOVE_FORCEINLINE_IS_BOOST_FORCELINE

#if defined(BOOST_MOVE_DISABLE_FORCEINLINE)
#	define YGGR_MOVE_DISABLE_FORCEINLINE
#endif // BOOST_MOVE_DISABLE_FORCEINLINE

#if defined(BOOST_MOVE_FORCEINLINE_IS_BOOST_FORCELINE)
#	define YGGR_MOVE_FORCEINLINE_IS_BOOST_FORCELINE
#endif // BOOST_MOVE_FORCEINLINE_IS_BOOST_FORCELINE

#if defined(YGGR_MOVE_DISABLE_FORCEINLINE)
   #define YGGR_MOVE_FORCEINLINE inline
#elif defined(YGGR_MOVE_FORCEINLINE_IS_BOOST_FORCELINE)
   #define YGGR_MOVE_FORCEINLINE BOOST_FORCEINLINE
#elif defined(BOOST_MSVC) && defined(_DEBUG)
   //"__forceinline" and MSVC seems to have some bugs in debug mode
   #define YGGR_MOVE_FORCEINLINE inline
#elif defined(__GNUC__) && ((__GNUC__ < 4) || (__GNUC__ == 4 && (__GNUC_MINOR__ < 5)))
   //Older GCCs have problems with forceinline
   #define YGGR_MOVE_FORCEINLINE inline
#else
   #define YGGR_MOVE_FORCEINLINE BOOST_FORCEINLINE
#endif // YGGR_MOVE_DISABLE_FORCEINLINE

#endif // __YGGR_MOVE_DETAIL_WORKAROUND_HPP__