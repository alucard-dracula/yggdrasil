//pc_nsql_database_system.hpp

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

#ifndef __YGGR_COMPILE_LINK_PC_NSQL_DATABASE_SYSTEM_HPP__
#define __YGGR_COMPILE_LINK_PC_NSQL_DATABASE_SYSTEM_HPP__

#include <yggr/compile_link/msvc_cfg.hpp>
#include <mongoc.h>

#ifdef _MSC_VER

#	pragma comment YGGR_COMPILE_LINK_LIB(yggr_nsql_database_system)
#	pragma comment YGGR_COMPILE_LINK_LIB(mongoc)
#	pragma comment YGGR_COMPILE_LINK_LIB(bson)
#	pragma comment YGGR_COMPILE_LINK_LIB(common)

#if defined(MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION) && MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION

#	pragma comment YGGR_COMPILE_LINK_LIB(mongocrypt)
#	pragma comment YGGR_COMPILE_LINK_LIB(kms-message)
#	pragma comment YGGR_COMPILE_LINK_LIB(intel_dfp_obj)

#endif // MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION

//#	if (BSON_VERSION_HEX < 0x01060000)
//#		pragma comment YGGR_COMPILE_LINK_LIB(yajl)
//#	endif // BSON_VERSION_HEX < 0x01060000

#	if !(MONGOC_VERSION_HEX < 0x01190000)
#		pragma comment YGGR_COMPILE_LINK_LIB(utf8proc)
#	endif // BSON_VERSION_HEX < 0x01060000

#	pragma comment YGGR_COMPILE_LINK_LIB(base64)

#	ifdef MONGOC_ENABLE_SSL
#		include <yggr/compile_link/pc_openssl.hpp>
#	endif // MONGOC_ENABLE_SSL

#if !(BSON_VERSION_HEX < 0x01060000)

#	include <yggr/compile_link/pc_zlib.hpp>

#	if !defined(YGGR_HAS_ICU)
#		define YGGR_HAS_ICU
#		define UNDO_YGGR_HAS_ICU
#	endif // YGGR_HAS_ICU

#	include <yggr/compile_link/pc_ibm_icu.hpp>

#	if defined(UNDO_YGGR_HAS_ICU)
#		undef UNDO_YGGR_HAS_ICU
#		undef YGGR_HAS_ICU
#	endif //UNDO_YGGR_HAS_ICU

#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(ws2_32)
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(Dnsapi)
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(Bcrypt)
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(Secur32)
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(Crypt32)

#endif // !(BSON_VERSION_HEX < 0x01060000)

#endif // _MSC_VER

#endif // __YGGR_COMPILE_LINK_PC_NSQL_DATABASE_SYSTEM_HPP__
