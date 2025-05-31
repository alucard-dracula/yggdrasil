/*
 * Copyright 2018-present MongoDB, Inc.
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

#ifndef MONGOCRYPT_MUTEX_PRIVATE_H
#define MONGOCRYPT_MUTEX_PRIVATE_H

#include <bson/bson.h>

#if defined(BSON_OS_UNIX)
#include <pthread.h>
#define mongocrypt_mutex_t pthread_mutex_t
#else
#define mongocrypt_mutex_t CRITICAL_SECTION
#endif

#include <yggr_detail/c_version_support.h>

void _mongocrypt_mutex_init(mongocrypt_mutex_t *mutex);

void _mongocrypt_mutex_cleanup(mongocrypt_mutex_t *mutex);

void _mongocrypt_mutex_lock(mongocrypt_mutex_t *mutex);

void _mongocrypt_mutex_unlock(mongocrypt_mutex_t *mutex);

#if defined(YGGR_EX_C99_SUPPORTED)

#define MONGOCRYPT_WITH_MUTEX(Mutex)                                                                                   \
    for (int only_once = (_mongocrypt_mutex_lock(&(Mutex)), 1); only_once; _mongocrypt_mutex_unlock(&(Mutex)))         \
        for (; only_once; only_once = 0)

#else

#define MONGOCRYPT_WITH_MUTEX(Mutex, __loop_val__)                                                                                   \
    for (__loop_val__ = (_mongocrypt_mutex_lock(&(Mutex)), 1); __loop_val__; _mongocrypt_mutex_unlock(&(Mutex)))         \
        for (; __loop_val__; __loop_val__ = 0)

#endif // YGGR_EX_C99_SUPPORTED

#endif /* MONGOCRYPT_MUTEX_PRIVATE_H */
