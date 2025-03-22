/******************************************************************************
  Copyright (c) 2007-2018, Intel Corp.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef _COMPATIBLE_BID_USER_CONF_H
#define _COMPATIBLE_BID_USER_CONF_H

#if !defined(BID_USING_IN_MONGODB)

#	if defined(BUILD_MONGODB_DRIVER_YGGR_VER) \
		|| defined(MONGOC_COMPILATION) \
		|| defined(BSON_STATIC) \
		|| defined(MONGOC_STATIC)

#		define BID_USING_IN_MONGODB

#	endif // BUILD_MONGODB_DRIVER_YGGR_VER

#endif // !defined(BID_USING_IN_MONGODB)

//#if defined(BID_USING_IN_MONGODB)
//#	error "using in mongodb"
//#else
////#	error "not using in mongodb"
//#endif // BID_USING_IN_MONGODB


#if defined(_MSC_VER) \
		|| defined(WIN32) \
		|| defined(WIN64) \
		|| defined(_WINDOWS)

#	if !defined(WINDOWS)
#		define WINDOWS 1
#	endif // WINDOWS

#	if !defined(WNT)
#		define WNT 1
#	endif // WNT

#	if !defined(wnt)
#		define wnt 1
#	endif // wnt

#	if defined(WIN64)
#		if !defined(win64)
#			define win64
#		endif // win64
#	endif // WIN64

#endif // WIN32 || WIN64


#if defined(__GNUC__)
# if !(defined(__APPLE__) || defined(macintosh) || defined(WINDOWS)) \
      || defined(__linux__)
#   define LINUX
# endif // # if !(defined(__APPLE__) || defined(macintosh))

#if (defined(_X86_) && (_X86_ == 1)) \
      || (defined(__i686__) && (__i686__ == 1)) \
      || (defined(__i386__) && (__i386__ == 1)) \
      && !defined(__x86_64__)
#	define IX86
#endif //  defined(_X86_) && (_X86_ == 1) && !defined(__x86_64__)

#endif // __GNUC__

#if !defined(DECIMAL_CALL_BY_REFERENCE)
#	define DECIMAL_CALL_BY_REFERENCE 0
#endif // DECIMAL_CALL_BY_REFERENCE

#if !defined(DECIMAL_GLOBAL_ROUNDING)
#	define DECIMAL_GLOBAL_ROUNDING 0
#endif // DECIMAL_GLOBAL_ROUNDING

#if !defined(DECIMAL_GLOBAL_EXCEPTION_FLAGS)
#	define DECIMAL_GLOBAL_EXCEPTION_FLAGS 0
#endif // DECIMAL_GLOBAL_EXCEPTION_FLAGS

#if !defined(UNCHANGED_BINARY_STATUS_FLAGS)
#	define UNCHANGED_BINARY_STATUS_FLAGS 0
#endif // UNCHANGED_BINARY_STATUS_FLAGS

#if !defined(USE_COMPILER_F128_TYPE)
#	define USE_COMPILER_F128_TYPE 0
#endif // USE_COMPILER_F128_TYPE

#if !defined(USE_COMPILER_F80_TYPE)
#	define USE_COMPILER_F80_TYPE 0
#endif // USE_COMPILER_F80_TYPE

#if !defined(USE_NATIVE_QUAD_TYPE)
#	define USE_NATIVE_QUAD_TYPE 0
#endif // USE_NATIVE_QUAD_TYPE

#endif // _COMPATIBLE_BID_USER_CONF_H
