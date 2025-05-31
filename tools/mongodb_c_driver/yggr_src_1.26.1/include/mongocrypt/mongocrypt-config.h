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

#ifndef MONGOCRYPT_CONFIG_H
#define MONGOCRYPT_CONFIG_H

#if defined(BUILD_MONGODB_DRIVER_YGGR_VER)

#include <yggr_detail/mongo_c_inline.h>
#include <yggr_detail/c_version_support.h>
#include <yggr_detail/warning_check.h>

#if defined(_MSC_VER)
#	include <yggr_detail/config/mongocrypt_config_msvc.h>
#elif defined(__MINGW32__)
#	include <yggr_detail/config/mongocrypt_config_mingw.h>
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#	if (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) \
		|| (defined(TARGET_OS_IOS) && TARGET_OS_IOS)
#		include <yggr_detail/config/mongocrypt_config_ios.h>
#	else
#		include <yggr_detail/config/mongocrypt_config_darwin.h>
#	endif // TARGET_OS_IPHONE
#else
#	if defined(__ANDROID__) || defined(ANDROID)
#		include <yggr_detail/config/mongocrypt_config_android.h>
#	else
#		include <yggr_detail/config/mongocrypt_config_linux.h>
#	endif // __ANDROID__
#endif // _MSC_VER

#else

/* clang-format off */

/**
 * @def MONGOCRYPT_VERSION
 * @brief The version string describing libmongocrypt.
 * Has the form x.y.z-<pre>+<date>+git<sha>.
 */
#define MONGOCRYPT_VERSION "1.9.1"

/*
 * MONGOCRYPT_ENABLE_CRYPTO_CNG is set from configure to determine if we are
 * compiled with Native Crypto support on Windows
 */
#define MONGOCRYPT_ENABLE_CRYPTO_CNG 1

#if MONGOCRYPT_ENABLE_CRYPTO_CNG != 1
#  undef MONGOCRYPT_ENABLE_CRYPTO_CNG
#endif


/*
 * MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO is set from configure to determine if we are
 * compiled with Native Crypto support on Darwin
 */
#define MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO 0

#if MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO != 1
#  undef MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO
#endif


/*
 * MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO is set from configure to determine if we are
 * compiled with OpenSSL/LibreSSL (which both use libcrypto) support.
 */
#define MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO 0
#define MONGOCRYPT_ENABLE_CRYPTO_OPENSSL 0

#if MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO != 1
#  undef MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO
#  undef MONGOCRYPT_ENABLE_CRYPTO_OPENSSL
#endif


/*
 * MONGOCRYPT_ENABLE_CRYPTO is set from configure to determine if we are
 * compiled with any crypto support.
 */
#define MONGOCRYPT_ENABLE_CRYPTO 1

#if MONGOCRYPT_ENABLE_CRYPTO != 1
#  undef MONGOCRYPT_ENABLE_CRYPTO
#endif


/*
 * MONGOCRYPT_ENABLE_TRACE is set from configure to determine if we are
 * compiled with tracing support.
 */
#define MONGOCRYPT_ENABLE_TRACE 0

#if MONGOCRYPT_ENABLE_TRACE != 1
#  undef MONGOCRYPT_ENABLE_TRACE
#endif

/* clang-format on */

#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#endif /* MONGOCRYPT_CONFIG_H */
