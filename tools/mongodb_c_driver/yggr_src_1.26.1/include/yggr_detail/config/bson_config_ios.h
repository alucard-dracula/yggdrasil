// bson_config_ios.h
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

#if !defined(BSON_INSIDE) && !defined(BSON_COMPILATION)
#error "Only <bson/bson.h> can be included directly."
#endif // #if !defined(BSON_INSIDE) && !defined(BSON_COMPILATION)

#ifndef __MONGODB_YGGR_EX_BSON_CONFIG_PLATFORM_H__
#define __MONGODB_YGGR_EX_BSON_CONFIG_PLATFORM_H__

//#error "need config"

#include <yggr_detail/c_version_support.h>
#include <yggr_detail/platform_config.h>
#include <yggr_detail/inteldfp_config.h>


/*
 * Define to 1234 for Little Endian, 4321 for Big Endian.
 */
#if !defined(BSON_BYTE_ORDER)
#	define BSON_BYTE_ORDER 1234
#endif //BSON_BYTE_ORDER

/*
 * Define to 1 if you have stdbool.h
 */

#if !defined(BSON_HAVE_STDBOOL_H)
#	if defined(YGGR_EX_C99_SUPPORTED)
#		define BSON_HAVE_STDBOOL_H 1
#	else
#		define BSON_HAVE_STDBOOL_H 0
#	endif /* YGGR_EX_C99_SUPPORTED*/
#endif //BSON_HAVE_STDBOOL_H

#if BSON_HAVE_STDBOOL_H != 1
#	undef BSON_HAVE_STDBOOL_H
#endif // BSON_HAVE_STDBOOL_H

/*
 * Define to 1 for POSIX-like systems, 2 for Windows.
 */

#if !defined(BSON_OS)
#	define BSON_OS 1
#endif // BSON_OS


/*
 * Define to 1 if you have clock_gettime() available.
 */

#if !defined(BSON_HAVE_CLOCK_GETTIME)
#	define BSON_HAVE_CLOCK_GETTIME 1
#endif //BSON_HAVE_CLOCK_GETTIME

#if BSON_HAVE_CLOCK_GETTIME != 1
#	undef BSON_HAVE_CLOCK_GETTIME
#endif // BSON_HAVE_CLOCK_GETTIME


/*
 * Define to 1 if you have strings.h available on your platform.
 */
#if !defined(BSON_HAVE_STRINGS_H)
#	define BSON_HAVE_STRINGS_H 1
#endif // BSON_HAVE_STRINGS_H

#if BSON_HAVE_STRINGS_H != 1
#	undef BSON_HAVE_STRINGS_H
#endif // BSON_HAVE_STRINGS_H


/*
 * Define to 1 if you have strnlen available on your platform.
 */
#if !defined(BSON_HAVE_STRNLEN)
#	define BSON_HAVE_STRNLEN 1
#endif // BSON_HAVE_STRNLEN

#if BSON_HAVE_STRNLEN != 1
#	undef BSON_HAVE_STRNLEN
#endif // BSON_HAVE_STRNLEN


/*
 * Define to 1 if you have snprintf available on your platform.
 */

#if !defined(BSON_HAVE_SNPRINTF)
#	define BSON_HAVE_SNPRINTF 1
#endif // BSON_HAVE_SNPRINTF

#if BSON_HAVE_SNPRINTF != 1
#	undef BSON_HAVE_SNPRINTF
#endif // BSON_HAVE_SNPRINTF


/*
 * Define to 1 if you have gmtime_r available on your platform.
 */

#if !defined(BSON_HAVE_GMTIME_R)
#	define BSON_HAVE_GMTIME_R 1
#endif // BSON_HAVE_GMTIME_R

#if BSON_HAVE_GMTIME_R != 1
#	undef BSON_HAVE_GMTIME_R
#endif // BSON_HAVE_GMTIME_R


/*
 * Define to 1 if you have struct timespec available on your platform.
 */

#if !defined(BSON_HAVE_GMTIME_R)
#   define BSON_HAVE_TIMESPEC 0
#else
#   define BSON_HAVE_TIMESPEC 1
#endif // BSON_HAVE_GMTIME_R

#if BSON_HAVE_TIMESPEC != 1
#	undef BSON_HAVE_TIMESPEC
#endif // BSON_HAVE_TIMESPEC


/*
 * Define to 1 if you want extra aligned types in libbson
 */
#if !defined(BSON_EXTRA_ALIGN)
#	define BSON_EXTRA_ALIGN 1
#endif // BSON_EXTRA_ALIGN

#if BSON_EXTRA_ALIGN != 1
#	undef BSON_EXTRA_ALIGN
#endif // BSON_EXTRA_ALIGN


/*
 * Define to 1 if you have rand_r available on your platform.
 */

#if !defined(BSON_HAVE_RAND_R)
#	define BSON_HAVE_RAND_R 1
#endif // BSON_EXTRA_ALIGN

#if BSON_HAVE_RAND_R != 1
#	undef BSON_HAVE_RAND_R
#endif // BSON_HAVE_RAND_R


/*
 * Define to 1 if you have strlcpy available on your platform.
 */

#if !defined(BSON_HAVE_STRLCPY)
#	define BSON_HAVE_STRLCPY 1
#endif // BSON_HAVE_STRLCPY

#if BSON_HAVE_STRLCPY != 1
#	undef BSON_HAVE_STRLCPY
#endif // BSON_HAVE_STRLCPY

#endif // __MONGODB_YGGR_EX_BSON_CONFIG_PLATFORM_H__
