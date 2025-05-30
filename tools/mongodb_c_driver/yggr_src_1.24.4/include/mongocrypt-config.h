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

/* clang-format off */

/**
 * @def MONGOCRYPT_VERSION
 * @brief The version string describing libmongocrypt.
 * Has the form x.y.z-<pre>+<date>+git<sha>.
 */
#define MONGOCRYPT_VERSION "2.7.15"

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

#endif /* MONGOCRYPT_CONFIG_H */
