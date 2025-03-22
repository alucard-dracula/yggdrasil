//pc_openssl.hpp

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

#ifndef __YGGR_COMPILE_LINK_PC_OPENSSL_HPP__
#define __YGGR_COMPILE_LINK_PC_OPENSSL_HPP__

#include <yggr/compile_link/msvc_cfg.hpp>
#include <openssl/opensslv.h>

#ifdef _MSC_VER

#if OPENSSL_VERSION_NUMBER < 0x1010000L
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(ssleay32)
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(libeay32)
#else
#	if defined(YGGR_OPENSSL_USING_STATIC_LIB)
#		pragma comment YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG(crypto_static)
#		pragma comment YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG(ssl_static)
#	else
#		pragma comment YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG(crypto)
#		pragma comment YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG(ssl)
#	endif // 0,1
#endif // OPENSSL_VERSION_NUMBER < 0x1010000L

#endif // _MSC_VER

#endif // __YGGR_COMPILE_LINK_PC_OPENSSL_HPP__
