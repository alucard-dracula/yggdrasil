// mongoc_config_darwin.h
/*
 * Copyright 2013 MongoDB Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// mingw using libopenssl default, if you wang to using windows api, define MINGW_USING_WIN_API

#if !defined(MONGOC_INSIDE) && !defined(MONGOC_COMPILATION)
#error "Only <mongoc/mongoc.h> can be included directly."
#endif

#ifndef __MONGODB_YGGR_EX_MONGOC_CONFIG_PLATFORM_H__
#define __MONGODB_YGGR_EX_MONGOC_CONFIG_PLATFORM_H__

//#error "need config"


#include <yggr_detail/c_version_support.h>
#include <yggr_detail/platform_config.h>
#include <yggr_detail/inteldfp_config.h>

/* clang-format off */

/*
 * NOTICE:
 * If you're about to update this file and add a config flag, make sure to
 * update:
 * o The bitfield in mongoc-handshake-private.h
 * o _mongoc_handshake_get_config_hex_string() in mongoc-handshake.c
 * o examples/parse_handshake_cfg.py
 * o test_handshake_platform_config in test-mongoc-handshake.c
 */

/* MONGOC_USER_SET_CFLAGS is set from config based on what compiler flags were
 * used to compile mongoc */

#if YGGR_EX_SYSTEM_64
#	if defined(_DEBUG)
#		define MONGOC_USER_SET_CFLAGS \
			-D_DEBUG -D_WINDOWS -D_LIB \
			-DBSON_STATIC -DBUILD_MONGODB_DRIVER_YGGR_VER -DKMS_MSG_STATIC \
			-DMLIB_USER -DMONGOCRYPT_LITTLE_ENDIAN \
			-DMONGOCRYPT_STATIC_DEFINE -DMONGOC_COMPILATION -DMONGOC_STATIC -DUTF8PROC_STATIC \
			-g -O0 -m64 -Wall -fexceptions -Wno-sign-compare -Wno-comment -Wno-enum-compare \
			-Wno-unused-local-typedefs -Wno-multichar -Werror=return-type -Werror=endif-labels \
			-fdata-sections -ffunction-sections -fno-strict-aliasing -fPIC

#		define MONGOC_USER_SET_LDFLAGS -m64

#	else

#		define MONGOC_USER_SET_CFLAGS \
			-DNDEBUG -D_WINDOWS -D_LIB \
			-DBSON_STATIC -DBUILD_MONGODB_DRIVER_YGGR_VER -DKMS_MSG_STATIC \
			-DMLIB_USER -DMONGOCRYPT_LITTLE_ENDIAN \
			-DMONGOCRYPT_STATIC_DEFINE -DMONGOC_COMPILATION -DMONGOC_STATIC -DUTF8PROC_STATIC \
			-O3 -Wall -m64 -fexceptions -Wno-sign-compare -Wno-comment -Wno-enum-compare \
			-Wno-unused-local-typedefs -Wno-multichar -Werror=return-type -Werror=endif-labels \
			-fdata-sections -ffunction-sections -fno-strict-aliasing -fPIC

#		define MONGOC_USER_SET_LDFLAGS -m64 -s
#	endif // _DEBUG
#else
#	if defined(_DEBUG)

#		define MONGOC_USER_SET_CFLAGS \
			-D_DEBUG -D_WINDOWS -D_LIB \
			-DBSON_STATIC -DBUILD_MONGODB_DRIVER_YGGR_VER -DKMS_MSG_STATIC \
			-DMLIB_USER -DMONGOCRYPT_LITTLE_ENDIAN \
			-DMONGOCRYPT_STATIC_DEFINE -DMONGOC_COMPILATION -DMONGOC_STATIC -DUTF8PROC_STATIC \
			-g -O0 -m32 -Wall -fexceptions -Wno-sign-compare -Wno-comment -Wno-enum-compare \
			-Wno-unused-local-typedefs -Wno-multichar -Werror=return-type -Werror=endif-labels \
			-fdata-sections -ffunction-sections -fno-strict-aliasing -fPIC

#		define MONGOC_USER_SET_LDFLAGS -m32
#	else

#		define MONGOC_USER_SET_CFLAGS \
			-DNDEBUG -D_WINDOWS -D_LIB \
			-DBSON_STATIC -DBUILD_MONGODB_DRIVER_YGGR_VER -DKMS_MSG_STATIC \
			-DMLIB_USER -DMONGOCRYPT_LITTLE_ENDIAN \
			-DMONGOCRYPT_STATIC_DEFINE -DMONGOC_COMPILATION -DMONGOC_STATIC -DUTF8PROC_STATIC \
			-O3 -m32 -Wall -fexceptions -Wno-sign-compare -Wno-comment -Wno-enum-compare \
			-Wno-unused-local-typedefs -Wno-multichar -Werror=return-type -Werror=endif-labels \
			-fdata-sections -ffunction-sections -fno-strict-aliasing -fPIC

#		define MONGOC_USER_SET_LDFLAGS -m32 -s
#	endif // _DEBUG
#endif //YGGR_EX_SYSTEM_64

/* MONGOC_CC is used to determine what C compiler was used to compile mongoc */
#define MONGOC_CC "clang"

/*
 * Define to 1 if you have stdbool.h
 */

#if !defined(MONGOC_HAVE_STDBOOL_H)
#	if defined(YGGR_EX_C99_SUPPORTED)
#		define MONGOC_HAVE_STDBOOL_H 1
#	else
#		define MONGOC_HAVE_STDBOOL_H 0
#	endif /* YGGR_EX_C99_SUPPORTED*/
#endif //MONGOC_HAVE_STDBOOL_H

#if MONGOC_HAVE_STDBOOL_H != 1
#	undef MONGOC_HAVE_STDBOOL_H
#endif // MONGOC_HAVE_STDBOOL_H

/*
 * MONGOC_ENABLE_SSL_SECURE_CHANNEL is set from configure to determine if we are
 * compiled with Native SSL support on Windows
 */

#if !defined(MONGOC_ENABLE_SSL_SECURE_CHANNEL)
#	if defined(MINGW_USING_WIN_API)
#		define MONGOC_ENABLE_SSL_SECURE_CHANNEL 1
#	else
#		define MONGOC_ENABLE_SSL_SECURE_CHANNEL 0
#	endif // MINGW_USING_WIN_API
#endif // MONGOC_ENABLE_SSL_SECURE_CHANNEL

#if MONGOC_ENABLE_SSL_SECURE_CHANNEL != 1
#	undef MONGOC_ENABLE_SSL_SECURE_CHANNEL
#endif // MONGOC_ENABLE_SSL_SECURE_CHANNEL


/*
 * MONGOC_ENABLE_CRYPTO_CNG is set from configure to determine if we are
 * compiled with Native Crypto support on Windows
 */

#if !defined(MONGOC_ENABLE_CRYPTO_CNG)
#		define MONGOC_ENABLE_CRYPTO_CNG 0
#endif // MONGOC_ENABLE_CRYPTO_CNG

#if MONGOC_ENABLE_CRYPTO_CNG != 1
#	undef MONGOC_ENABLE_CRYPTO_CNG
#endif // MONGOC_ENABLE_CRYPTO_CNG

/*
 * MONGOC_HAVE_BCRYPT_PBKDF2 is set from configure to determine if 
 * our Bcrypt Windows library supports PBKDF2 
 */
#define MONGOC_HAVE_BCRYPT_PBKDF2 0

#if MONGOC_HAVE_BCRYPT_PBKDF2 != 1
#  undef MONGOC_HAVE_BCRYPT_PBKDF2
#endif

/*
 * MONGOC_ENABLE_SSL_SECURE_TRANSPORT is set from configure to determine if we are
 * compiled with Native SSL support on Darwin
 */

#if !defined(MONGOC_ENABLE_SSL_SECURE_TRANSPORT)
#	define MONGOC_ENABLE_SSL_SECURE_TRANSPORT 0
#endif // MONGOC_ENABLE_SSL_SECURE_TRANSPORT

#if MONGOC_ENABLE_SSL_SECURE_TRANSPORT != 1
#	undef MONGOC_ENABLE_SSL_SECURE_TRANSPORT
#endif // MONGOC_ENABLE_SSL_SECURE_TRANSPORT


/*
 * MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO is set from configure to determine if we are
 * compiled with Native Crypto support on Darwin
 */

#if !defined(MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO)
#	define MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO 0
#endif // MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO

#if MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO != 1
#	undef MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO
#endif // MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO


/*
 * MONGOC_ENABLE_SSL_LIBRESSL is set from configure to determine if we are
 * compiled with LibreSSL support.
 */
#if !defined(MONGOC_ENABLE_SSL_LIBRESSL)
#	define MONGOC_ENABLE_SSL_LIBRESSL 0
#endif // MONGOC_ENABLE_SSL_LIBRESSL

#if MONGOC_ENABLE_SSL_LIBRESSL != 1
#	undef MONGOC_ENABLE_SSL_LIBRESSL
#endif // MONGOC_ENABLE_SSL_LIBRESSL


/*
 * MONGOC_ENABLE_SSL_OPENSSL is set from configure to determine if we are
 * compiled with OpenSSL support.
 */
#if !defined(MONGOC_ENABLE_SSL_OPENSSL)
#		define MONGOC_ENABLE_SSL_OPENSSL 1
#endif // MONGOC_ENABLE_SSL_OPENSSL

#if MONGOC_ENABLE_SSL_OPENSSL != 1
#	undef MONGOC_ENABLE_SSL_OPENSSL
#else
#  include <openssl/opensslv.h>
#endif // MONGOC_ENABLE_SSL_OPENSSL


/*
 * MONGOC_ENABLE_CRYPTO_LIBCRYPTO is set from configure to determine if we are
 * compiled with OpenSSL support.
 */

#if !defined(MONGOC_ENABLE_CRYPTO_LIBCRYPTO)
#		define MONGOC_ENABLE_CRYPTO_LIBCRYPTO 1
#endif // MONGOC_ENABLE_CRYPTO_LIBCRYPTO

#if MONGOC_ENABLE_CRYPTO_LIBCRYPTO != 1
#	undef MONGOC_ENABLE_CRYPTO_LIBCRYPTO
#endif // MONGOC_ENABLE_CRYPTO_LIBCRYPTO


/*
 * MONGOC_ENABLE_SSL is set from configure to determine if we are
 * compiled with any SSL support.
 */

#if !defined(MONGOC_ENABLE_SSL)
#	define MONGOC_ENABLE_SSL 1
#endif // MONGOC_ENABLE_SSL

#if MONGOC_ENABLE_SSL != 1
#	undef MONGOC_ENABLE_SSL
#endif // MONGOC_ENABLE_SSL


/*
 * MONGOC_ENABLE_CRYPTO is set from configure to determine if we are
 * compiled with any crypto support.
 */
#if !defined(MONGOC_ENABLE_CRYPTO)
#	define MONGOC_ENABLE_CRYPTO 1
#endif // MONGOC_ENABLE_CRYPTO

#if MONGOC_ENABLE_CRYPTO != 1
#	undef MONGOC_ENABLE_CRYPTO
#endif // MONGOC_ENABLE_CRYPTO


/*
 * Use system crypto profile
 */
#if !defined(MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE)
#	define MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE 0
#endif // MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE

#if MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE != 1
#	undef MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE
#endif // MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE


/*
 * Use ASN1_STRING_get0_data () rather than the deprecated ASN1_STRING_data
 */

#if !defined(MONGOC_HAVE_ASN1_STRING_GET0_DATA)
#  if OPENSSL_VERSION_MAJOR < 3
#	   define MONGOC_HAVE_ASN1_STRING_GET0_DATA 0
#  else
#	   define MONGOC_HAVE_ASN1_STRING_GET0_DATA 1
#  endif // OPENSSL_VERSION_MAJOR < 3
#endif // MONGOC_HAVE_ASN1_STRING_GET0_DATA

#if MONGOC_HAVE_ASN1_STRING_GET0_DATA != 1
#	undef MONGOC_HAVE_ASN1_STRING_GET0_DATA
#endif // MONGOC_HAVE_ASN1_STRING_GET0_DATA


/*
 * MONGOC_ENABLE_SASL is set from configure to determine if we are
 * compiled with SASL support.
 */

#if !defined(MONGOC_ENABLE_SASL)
#	define MONGOC_ENABLE_SASL 0
#endif // MONGOC_ENABLE_SASL

#if MONGOC_ENABLE_SASL != 1
#	undef MONGOC_ENABLE_SASL
#endif // MONGOC_ENABLE_SASL


/*
 * MONGOC_ENABLE_SASL_CYRUS is set from configure to determine if we are
 * compiled with Cyrus SASL support.
 */
#if !defined(MONGOC_ENABLE_SASL_CYRUS)
#	define MONGOC_ENABLE_SASL_CYRUS 0
#endif // MONGOC_ENABLE_SASL_CYRUS

#if MONGOC_ENABLE_SASL_CYRUS != 1
#	undef MONGOC_ENABLE_SASL_CYRUS
#endif // MONGOC_ENABLE_SASL_CYRUS

//#if MONGOC_ENABLE_SASL_CYRUS
//#	error "need configure"
//#	if (YGGR_EX_SYSTEM_64)
//#		define MONGOC_CYRUS_PLUGIN_PATH_PREFIX "/usr/lib/sasl2"
//#	else
//#		error "macos not support 32bit app"
//#	endif // #	if (YGGR_EX_SYSTEM_64)
//#endif //MONGOC_ENABLE_SASL_CYRUS

/*
 * MONGOC_ENABLE_SASL_SSPI is set from configure to determine if we are
 * compiled with SSPI support.
 */
#if !defined(MONGOC_ENABLE_SASL_SSPI)
#	define MONGOC_ENABLE_SASL_SSPI 0
#endif // MONGOC_ENABLE_SASL_SSPI

#if MONGOC_ENABLE_SASL_SSPI != 1
#	undef MONGOC_ENABLE_SASL_SSPI
#endif // MONGOC_ENABLE_SASL_SSPI

/*
 * MONGOC_HAVE_SASL_CLIENT_DONE is set from configure to determine if we
 * have SASL and its version is new enough to use sasl_client_done (),
 * which supersedes sasl_done ().
 */
#if !defined(MONGOC_HAVE_SASL_CLIENT_DONE)
#	define MONGOC_HAVE_SASL_CLIENT_DONE 1
#endif // MONGOC_HAVE_SASL_CLIENT_DONE

#if MONGOC_HAVE_SASL_CLIENT_DONE != 1
#	undef MONGOC_HAVE_SASL_CLIENT_DONE
#endif // MONGOC_HAVE_SASL_CLIENT_DONE

/*
 * Disable automatic calls to mongoc_init() and mongoc_cleanup()
 * before main() is called, and after exit() (respectively).
 */

#if !defined(MONGOC_NO_AUTOMATIC_GLOBALS)
#	define MONGOC_NO_AUTOMATIC_GLOBALS 1
#endif // MONGOC_NO_AUTOMATIC_GLOBALS

#if MONGOC_NO_AUTOMATIC_GLOBALS != 1
#	undef MONGOC_NO_AUTOMATIC_GLOBALS
#endif // MONGOC_NO_AUTOMATIC_GLOBALS

/*
 * MONGOC_HAVE_SOCKLEN is set from configure to determine if we
 * need to emulate the type.
 */
#if !defined(MONGOC_HAVE_SOCKLEN)
#	define MONGOC_HAVE_SOCKLEN 1
#endif // MONGOC_HAVE_SOCKLEN

#if MONGOC_HAVE_SOCKLEN != 1
#	undef MONGOC_HAVE_SOCKLEN
#endif // MONGOC_HAVE_SOCKLEN

/**
 * @brief Defined to 0/1 for whether we were configured with ENABLE_SRV
 */
#if !defined(MONGOC_ENABLE_SRV)
#	define MONGOC_ENABLE_SRV 1
#endif // MONGOC_ENABLE_SRV

/*
 * MONGOC_HAVE_DNSAPI is set from configure to determine if we should use the
 * Windows dnsapi for SRV record lookups.
 */

#if !defined(MONGOC_HAVE_DNSAPI)
#	define MONGOC_HAVE_DNSAPI 0
#endif // MONGOC_HAVE_DNSAPI

#if MONGOC_HAVE_DNSAPI != 1
#	undef MONGOC_HAVE_DNSAPI
#endif // MONGOC_HAVE_DNSAPI


/*
 * MONGOC_HAVE_RES_NSEARCH is set from configure to determine if we
 * have thread-safe res_nsearch().
 */
#if !defined(MONGOC_HAVE_RES_NSEARCH)
#	define MONGOC_HAVE_RES_NSEARCH 0
#endif // MONGOC_HAVE_RES_NSEARCH

#if MONGOC_HAVE_RES_NSEARCH != 1
#	undef MONGOC_HAVE_RES_NSEARCH
#endif // MONGOC_HAVE_RES_NSEARCH


/*
 * MONGOC_HAVE_RES_NDESTROY is set from configure to determine if we
 * have BSD / Darwin's res_ndestroy().
 */
#if !defined(MONGOC_HAVE_RES_NDESTROY)
#	define MONGOC_HAVE_RES_NDESTROY 0
#endif // MONGOC_HAVE_RES_NDESTROY

#if MONGOC_HAVE_RES_NDESTROY != 1
#	undef MONGOC_HAVE_RES_NDESTROY
#endif // MONGOC_HAVE_RES_NDESTROY


/*
 * MONGOC_HAVE_RES_NCLOSE is set from configure to determine if we
 * have Linux's res_nclose().
 */
#if !defined(MONGOC_HAVE_RES_NCLOSE)
#	define MONGOC_HAVE_RES_NCLOSE 1
#endif // MONGOC_HAVE_RES_NCLOSE

#if MONGOC_HAVE_RES_NCLOSE != 1
#	undef MONGOC_HAVE_RES_NCLOSE
#endif // MONGOC_HAVE_RES_NCLOSE


/*
 * MONGOC_HAVE_RES_SEARCH is set from configure to determine if we
 * have thread-unsafe res_search(). It's unset if we have the preferred
 * res_nsearch().
 */
#if !defined(MONGOC_HAVE_RES_SEARCH)
#	define MONGOC_HAVE_RES_SEARCH 1
#endif // MONGOC_HAVE_RES_SEARCH

#if MONGOC_HAVE_RES_SEARCH != 1
#	undef MONGOC_HAVE_RES_SEARCH
#endif // MONGOC_HAVE_RES_SEARCH


/*
 * Set from configure, see
 * https://curl.haxx.se/mail/lib-2009-04/0287.html
 */
#if !defined(MONGOC_SOCKET_ARG2)
#	define MONGOC_SOCKET_ARG2 struct sockaddr
#endif // MONGOC_SOCKET_ARG2

#if !defined(MONGOC_SOCKET_ARG3)
#	define MONGOC_SOCKET_ARG3 socklen_t
#endif // MONGOC_SOCKET_ARG3

/*
 * Enable wire protocol compression negotiation
 *
 */
#if !defined(MONGOC_ENABLE_COMPRESSION)
#	define MONGOC_ENABLE_COMPRESSION 1
#endif // MONGOC_ENABLE_COMPRESSION

#if MONGOC_ENABLE_COMPRESSION != 1
#	undef MONGOC_ENABLE_COMPRESSION
#endif // MONGOC_ENABLE_COMPRESSION

/*
 * Set if we have snappy compression support
 *
 */
#if !defined(MONGOC_ENABLE_COMPRESSION_SNAPPY)
#	define MONGOC_ENABLE_COMPRESSION_SNAPPY 0
#endif // MONGOC_ENABLE_COMPRESSION_SNAPPY

#if MONGOC_ENABLE_COMPRESSION_SNAPPY != 1
#	undef MONGOC_ENABLE_COMPRESSION_SNAPPY
#endif // MONGOC_ENABLE_COMPRESSION_SNAPPY


/*
 * Set if we have zlib compression support
 *
 */
#if !defined(MONGOC_ENABLE_COMPRESSION_ZLIB)
#	define MONGOC_ENABLE_COMPRESSION_ZLIB 1
#endif // MONGOC_ENABLE_COMPRESSION_ZLIB

#if MONGOC_ENABLE_COMPRESSION_ZLIB != 1
#	undef MONGOC_ENABLE_COMPRESSION_ZLIB
#endif // MONGOC_ENABLE_COMPRESSION_ZLIB

/*
 * Set if we have zstd compression support
 *
 */
#if !defined(MONGOC_ENABLE_COMPRESSION_ZSTD)
#	define MONGOC_ENABLE_COMPRESSION_ZSTD 0
#endif // MONGOC_ENABLE_COMPRESSION_ZSTD

#if MONGOC_ENABLE_COMPRESSION_ZSTD != 1
#	undef MONGOC_ENABLE_COMPRESSION_ZSTD
#endif

/*
 * Set if performance counters are available and not disabled.
 *
 */
#if !defined(MONGOC_ENABLE_SHM_COUNTERS)
#	define MONGOC_ENABLE_SHM_COUNTERS 0
#endif // MONGOC_ENABLE_SHM_COUNTERS

#if MONGOC_ENABLE_SHM_COUNTERS != 1
#	undef MONGOC_ENABLE_SHM_COUNTERS
#endif // MONGOC_ENABLE_SHM_COUNTERS

/*
 * Set if we have enabled fast counters on Intel using the RDTSCP instruction
 *
 */
#if !defined(MONGOC_ENABLE_RDTSCP)
#	define MONGOC_ENABLE_RDTSCP 0
#endif // MONGOC_ENABLE_RDTSCP

#if MONGOC_ENABLE_RDTSCP != 1
#	undef MONGOC_ENABLE_RDTSCP
#endif // MONGOC_ENABLE_RDTSCP


/*
 * Set if we have the sched_getcpu() function for use with counters
 *
 */
#if !defined(MONGOC_HAVE_SCHED_GETCPU)
#	define MONGOC_HAVE_SCHED_GETCPU 1
#endif // MONGOC_HAVE_SCHED_GETCPU

#if MONGOC_HAVE_SCHED_GETCPU != 1
#	undef MONGOC_HAVE_SCHED_GETCPU
#endif // MONGOC_HAVE_SCHED_GETCPU

/*
 * Set if tracing is enabled. Logs things like network communication and
 * entry/exit of certain functions.
 */
#if !defined(MONGOC_TRACE)
#	define MONGOC_TRACE 0
#endif // MONGOC_TRACE

/*
 * Set if we have Client Side Encryption support.
 */

//#define MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION 0 // in msvc org is 0

#if !defined(MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION)
#	define MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION 1
#endif // MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION

#if MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION != 1
#	undef MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION
#endif // MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION

/*
 * Set if struct sockaddr_storage has __ss_family (instead of ss_family)
 */

#if !defined(MONGOC_HAVE_SS_FAMILY)
#	define MONGOC_HAVE_SS_FAMILY 0
#endif // MONGOC_HAVE_SS_FAMILY

#if MONGOC_HAVE_SS_FAMILY != 1
#	undef MONGOC_HAVE_SS_FAMILY
#endif

/*
 * Set if building with AWS IAM support.
 */
//#define MONGOC_ENABLE_MONGODB_AWS_AUTH 0 // in msvc100 org is 0
#if !defined(MONGOC_ENABLE_MONGODB_AWS_AUTH)
#	define MONGOC_ENABLE_MONGODB_AWS_AUTH 1
#endif // MONGOC_ENABLE_MONGODB_AWS_AUTH

#if MONGOC_ENABLE_MONGODB_AWS_AUTH != 1
#	undef MONGOC_ENABLE_MONGODB_AWS_AUTH
#endif // MONGOC_ENABLE_MONGODB_AWS_AUTH

enum {
   /**
    * @brief Compile-time constant determining whether the mongoc library was
    * compiled with tracing enabled.
    *
    * Can be controlled with the ��ENABLE_TRACING�� configure-time boolean option
    */
   MONGOC_TRACE_ENABLED = MONGOC_TRACE,
   /**
    * @brief Compile-time constant indicating whether the mongoc library was
    * compiled with SRV server discovery support.
    *
    * Can be controled with the ��ENABLE_SRV�� configure-time boolean option.
    */
   MONGOC_SRV_ENABLED = MONGOC_ENABLE_SRV,
};

/* clang-format on */

#endif //__MONGODB_YGGR_EX_MONGOC_CONFIG_PLATFORM_H__
