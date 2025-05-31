/*
 * Copyright 2019-present MongoDB Inc.
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

#ifndef __MONGODB_YGGR_EX_MONGOCRYPT_CONFIG_H__
#define __MONGODB_YGGR_EX_MONGOCRYPT_CONFIG_H__

#include <yggr_detail/c_version_support.h>
#include <yggr_detail/platform_config.h>
#include <yggr_detail/inteldfp_config.h>

/* clang-format off */

/**
 * @def MONGOCRYPT_VERSION
 * @brief The version string describing libmongocrypt.
 * Has the form x.y.z-<pre>+<date>+git<sha>.
 */
#define MONGOCRYPT_VERSION "1.9.1"

/*
 * Define to 1 if you have stdbool.h
 */

#if !defined(MONGOCRYPT_HAVE_STDBOOL_H)
#	if defined(YGGR_EX_C99_SUPPORTED)
#		define MONGOCRYPT_HAVE_STDBOOL_H 1
#	else
#		define MONGOCRYPT_HAVE_STDBOOL_H 0
#	endif /* YGGR_EX_C99_SUPPORTED*/
#endif //MONGOCRYPT_HAVE_STDBOOL_H

#if MONGOCRYPT_HAVE_STDBOOL_H != 1
#	undef MONGOCRYPT_HAVE_STDBOOL_H
#endif // MONGOCRYPT_HAVE_STDBOOL_H

/*
 * MONGOC_HAVE_BCRYPT_PBKDF2 is set from configure to determine if 
 * our Bcrypt Windows library supports PBKDF2 
 */
#if !defined(MONGOC_HAVE_BCRYPT_PBKDF2)
#	if defined(MINGW_USING_WIN_API)
#		define MONGOC_HAVE_BCRYPT_PBKDF2 1
#	else
#		define MONGOC_HAVE_BCRYPT_PBKDF2 0
#	endif // MINGW_USING_WIN_API
#endif // MONGOC_HAVE_BCRYPT_PBKDF2

#if MONGOC_HAVE_BCRYPT_PBKDF2 != 1
#	undef MONGOC_HAVE_BCRYPT_PBKDF2
#endif // MONGOC_HAVE_BCRYPT_PBKDF2

/*
 * MONGOCRYPT_ENABLE_CRYPTO_CNG is set from configure to determine if we are
 * compiled with Native Crypto support on Windows
 */
#if !defined(MONGOCRYPT_ENABLE_CRYPTO_CNG)
#	if defined(MINGW_USING_WIN_API)
#		define MONGOCRYPT_ENABLE_CRYPTO_CNG 1
#	else
#		define MONGOCRYPT_ENABLE_CRYPTO_CNG 0
#	endif // MINGW_USING_WIN_API
#endif // MONGOCRYPT_ENABLE_CRYPTO_CNG

#if MONGOCRYPT_ENABLE_CRYPTO_CNG != 1
#	undef MONGOCRYPT_ENABLE_CRYPTO_CNG
#endif // MONGOCRYPT_ENABLE_CRYPTO_CNG

/*
 * MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO is set from configure to determine if we are
 * compiled with Native Crypto support on Darwin
 */

#if !defined(MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO)
#	define MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO 0
#endif // MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO

#if MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO != 1
#	undef MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO
#endif// MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO


/*
 * MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO is set from configure to determine if we are
 * compiled with OpenSSL/LibreSSL (which both use libcrypto) support.
 */
#if !defined(MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO)
#	if defined(MINGW_USING_WIN_API)
#		define MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO 0
#		define MONGOCRYPT_ENABLE_CRYPTO_OPENSSL 0
#	else
#		define MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO 1
#		define MONGOCRYPT_ENABLE_CRYPTO_OPENSSL 1
#	endif // MINGW_USING_WIN_API
#endif // MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO

#if MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO != 1
#	undef MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO
#	undef MONGOCRYPT_ENABLE_CRYPTO_OPENSSL
#endif // MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO


/*
 * MONGOCRYPT_ENABLE_CRYPTO is set from configure to determine if we are
 * compiled with any crypto support.
 */
#if !defined(MONGOCRYPT_ENABLE_CRYPTO)
#	define MONGOCRYPT_ENABLE_CRYPTO 1
#endif // MONGOCRYPT_ENABLE_CRYPTO

#if MONGOCRYPT_ENABLE_CRYPTO != 1
#	undef MONGOCRYPT_ENABLE_CRYPTO
#endif //MONGOCRYPT_ENABLE_CRYPTO


/*
 * MONGOCRYPT_ENABLE_TRACE is set from configure to determine if we are
 * compiled with tracing support.
 */
#if !defined(MONGOCRYPT_ENABLE_TRACE)
#	define MONGOCRYPT_ENABLE_TRACE 0
#endif // MONGOCRYPT_ENABLE_TRACE

#if MONGOCRYPT_ENABLE_TRACE != 1
#	undef MONGOCRYPT_ENABLE_TRACE
#endif // MONGOCRYPT_ENABLE_TRACE

/* clang-format on */

#endif // __MONGODB_YGGR_EX_MONGOCRYPT_CONFIG_H__
