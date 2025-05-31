//basic_mongodb_config.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGODB_CONFIG_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGODB_CONFIG_HPP__

#include <yggr/base/yggrdef.h>

#if !defined(BUILD_MONGODB_DRIVER_YGGR_VER)
#	define BUILD_MONGODB_DRIVER_YGGR_VER
#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#include <yggr/nsql_database_system/bson_config.hpp>

#if !defined(MONGOC_INSIDE)
#	define MONGOC_INSIDE
#	define YGGR_USING_MONGOC_INSIDE
#endif // MONGOC_INSIDE

#include <mongoc-version.h>
#include <mongoc-config.h>

#if defined(YGGR_USING_MONGOC_INSIDE)
#	undef MONGOC_INSIDE
#	undef YGGR_USING_MONGOC_INSIDE
#endif // YGGR_USING_MONGOC_INSIDE

#if !(MONGOC_VERSION_HEX < 0x01070000)
#	if !defined(MONGOC_STATIC)
#		define MONGOC_STATIC
#	endif // MONGOC_STATIC
#endif // MONGOC_VERSION_HEX < 0x01070000

#if defined(MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION) \
	&& (MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION)

//mlib
#	if !defined(BID_USING_IN_MONGODB)
#		define BID_USING_IN_MONGODB
#	endif // BID_USING_IN_MONGODB

//mlib
#	if !defined(MLIB_USER)
#		define MLIB_USER
#	endif // MLIB_USER

// kms-message
#	if !defined(KMS_MSG_STATIC)
#		define KMS_MSG_STATIC
#	endif // KMS_MSG_STATIC

//mongocrypt
#	if !defined(MONGOCRYPT_STATIC_DEFINE)
#		define MONGOCRYPT_STATIC_DEFINE
#	endif // MONGOCRYPT_STATIC_DEFINE

#	if !defined(MONGOCRYPT_BIG_ENDIAN)
#		if !defined(MONGOCRYPT_LITTLE_ENDIAN)
#			define MONGOCRYPT_LITTLE_ENDIAN
#		endif // MONGOCRYPT_LITTLE_ENDIAN
#	endif // MONGOCRYPT_BIG_ENDIAN

#	if !defined(MONGOCRYPT_INTELDFP) \
		&& defined(YGGR_X86_PLATFORM) \
		&& !defined(YGGR_MONGODB_NO_DECIMAL128)
#		define MONGOCRYPT_INTELDFP
#	endif // MONGOCRYPT_INTELDFP

#endif // MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION


#include <mongoc.h>

#if (YGGR_MONGOC_HEX() < (YGGR_MAKE_MONGOC_HEX(1, 24, 4)))
#	error "yggdrasil not support now mongoc version."
#endif // (YGGR_MONGOC_HEX() < (YGGR_MAKE_MONGOC_HEX(1, 24, 4)))

#if !(YGGR_BSON_HEX() == YGGR_MONGOC_HEX())
#	error "bson version not equal mongoc version!!!"
#endif // !(YGGR_BSON_HEX() == YGGR_MONGOC_HEX())

#if !defined(MONGODB_DRIVER_INCLUDED_YGGR_VER)
#	error "this driver is not support yggdrasil!!!!"
#endif // !defined(MONGODB_DRIVER_INCLUDED_YGGR_VER)

#ifdef MONGOC_ENABLE_SSL
#	include <openssl/ssl.h>
#	include <openssl/err.h>
#	include <openssl/engine.h>
#	include <openssl/conf.h>
#endif // MONGOC_ENABLE_SSL

#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/nsql_database_system/bson_typeid_def.hpp>

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
#	include <yggr/network/socket_conflict_fixer.hpp>
#	include <windows.h>
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#if defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS)

#	if !defined(YGGR_MONGODB_NOT_SUPPORT_CSFLE)
#		define YGGR_MONGODB_NOT_SUPPORT_CSFLE 1
#	endif // YGGR_MONGODB_NOT_SUPPORT_CSFLE

#endif // #if defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS)

#endif //__YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGODB_CONFIG_HPP__
