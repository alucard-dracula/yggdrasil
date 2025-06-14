/*
 * Copyright 2022-present MongoDB, Inc.
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

#ifndef MC_OPTIONAL_PRIVATE_H
#define MC_OPTIONAL_PRIVATE_H

//#include <stdbool.h>
#include <yggr_detail/stdbool.h>
#include <stdint.h>

#include "mc-dec128.h"
#include "mlib/int128.h"

#if !defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_ANY_C(...) { true, __VA_ARGS__ }

#	define OPT_ANY OPT_ANY_C

#endif // YGGR_EX_C99_SUPPORTED


typedef struct {
    bool set;
    bool value;
} mc_optional_bool_t;

//#define OPT_BOOL(val)                                                                                                  \
//    (mc_optional_bool_t) { .set = true, .value = val }
//
//#define OPT_BOOL_C(val)                                                                                                \
//    { .set = true, .value = val }

#if defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_BOOL(val)                                                                                                   \
		(mc_optional_bool_t) { .set = true, .value = val }

#	define OPT_BOOL_C(val)                                                                                                 \
		{ .set = true, .value = val }

#else

#	define OPT_BOOL OPT_ANY

#	define OPT_BOOL_C OPT_ANY_C

#endif // YGGR_EX_C99_SUPPORTED

typedef struct {
    bool set;
    int32_t value;
} mc_optional_int32_t;

//#define OPT_I32(val)                                                                                                   \
//    (mc_optional_int32_t) { .set = true, .value = val }
//
//#define OPT_I32_C(val)                                                                                                 \
//    { .set = true, .value = val }

#if defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_I32(val)                                                                                                   \
		(mc_optional_int32_t) { .set = true, .value = val }

#	define OPT_I32_C(val)                                                                                                 \
		{ .set = true, .value = val }

#else

#	define OPT_I32 OPT_ANY

#	define OPT_I32_C OPT_ANY_C

#endif // YGGR_EX_C99_SUPPORTED

typedef struct {
    bool set;
    uint32_t value;
} mc_optional_uint32_t;

//#define OPT_U32(val)                                                                                                   \
//    (mc_optional_uint32_t) { .set = true, .value = val }
//
//#define OPT_U32_C(val)                                                                                                 \
//    { .set = true, .value = val }

#if defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_U32(val)                                                                                                   \
		(mc_optional_uint32_t) { .set = true, .value = val }

#	define OPT_U32_C(val)                                                                                                 \
		{ .set = true, .value = val }

#else

#	define OPT_U32 OPT_ANY

#	define OPT_U32_C OPT_ANY_C

#endif // YGGR_EX_C99_SUPPORTED

typedef struct {
    bool set;
    int64_t value;
} mc_optional_int64_t;

//#define OPT_I64(val)                                                                                                   \
//    (mc_optional_int64_t) { .set = true, .value = val }
//
//#define OPT_I64_C(val)                                                                                                 \
//    { .set = true, .value = val }

#if defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_I64(val)                                                                                                   \
		(mc_optional_int64_t) { .set = true, .value = val }

#	define OPT_I64_C(val)                                                                                                 \
		{ .set = true, .value = val }

#else

#	define OPT_I64 OPT_ANY

#	define OPT_I64_C OPT_ANY_C

#endif // YGGR_EX_C99_SUPPORTED


typedef struct {
    bool set;
    uint64_t value;
} mc_optional_uint64_t;

//#define OPT_U64(val)                                                                                                   \
//    (mc_optional_uint64_t) { .set = true, .value = val }
//
//#define OPT_U64_C(val)                                                                                                 \
//    { .set = true, .value = val }

#if defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_U64(val)                                                                                                   \
		(mc_optional_uint64_t) { .set = true, .value = val }

#	define OPT_U64_C(val)                                                                                                 \
		{ .set = true, .value = val }

#else

#	define OPT_U64 OPT_ANY

#	define OPT_U64_C OPT_ANY_C

#endif // YGGR_EX_C99_SUPPORTED

typedef struct {
    bool set;
    double value;
} mc_optional_double_t;

//#define OPT_DOUBLE(val)                                                                                                \
//    (mc_optional_double_t) { .set = true, .value = val }
//
//#define OPT_DOUBLE_C(val)                                                                                              \
//    { .set = true, .value = val }

#if defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_DOUBLE(val)                                                                                                \
		(mc_optional_double_t) { .set = true, .value = val }

#	define OPT_DOUBLE_C(val)                                                                                              \
		{ .set = true, .value = val }

#else

#	define OPT_DOUBLE OPT_ANY

#	define OPT_DOUBLE_C OPT_ANY_C

#endif // YGGR_EX_C99_SUPPORTED

#if MONGOCRYPT_HAVE_DECIMAL128_SUPPORT
typedef struct {
    bool set;
    mc_dec128 value;
} mc_optional_dec128_t;

//#define OPT_MC_DEC128(...)                                                                                             \
//    (mc_optional_dec128_t) { .set = true, .value = __VA_ARGS__ }
//#endif // MONGOCRYPT_HAVE_DECIMAL128_SUPPORT

#if defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_MC_DEC128(...)                                                                                             \
		(mc_optional_dec128_t) { .set = true, .value = __VA_ARGS__ }

#else

#	define OPT_MC_DEC128 OPT_ANY

#endif // YGGR_EX_C99_SUPPORTED

#endif // MONGOCRYPT_HAVE_DECIMAL128_SUPPORT

//#define OPT_NULLOPT                                                                                                    \
//    { .set = false }

#if defined(YGGR_EX_C99_SUPPORTED)

#	define OPT_NULLOPT                                                                                                    \
		{ .set = false }

#else

#	define OPT_NULLOPT { false }

#endif // YGGR_EX_C99_SUPPORTED

#endif /* MC_OPTIONAL_PRIVATE_H */
