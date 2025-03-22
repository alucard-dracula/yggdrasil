//bson_config.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_CONFIG_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_CONFIG_HPP__

#ifndef BUILD_MONGODB_DRIVER_YGGR_VER
#	define BUILD_MONGODB_DRIVER_YGGR_VER
#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#include <yggr/base/yggrdef.h>

//#ifdef _MSC_VER
//#	if defined (YGGR_SYSTEM_64) || defined(WIN64)
//#		ifndef _WIN64
//#			define _WIN64
//#		endif // _WIN64
//#	endif // YGGR_SYSTEM_64
//#endif // _MSC_VER

#if !defined(BSON_INSIDE)
#	define BSON_INSIDE
#	define YGGR_USING_BSON_INSIDE
#endif // BSON_INSIDE

#include <bson/bson-version.h>

#if defined(YGGR_USING_BSON_INSIDE)
#	undef BSON_INSIDE
#	undef YGGR_USING_BSON_INSIDE
#endif // YGGR_USING_BSON_INSIDE


#if !(BSON_VERSION_HEX < 0x01070000)
#	if !defined(BSON_STATIC)
#		define BSON_STATIC
#	endif // BSON_STATIC
#endif // BSON_VERSION_HEX < 0x01070000

#include <bson.h>

#if (YGGR_BSON_HEX() < (YGGR_MAKE_BSON_HEX(1, 23, 0)))
#	error "yggdrasil not support now bson version."
#endif // (YGGR_BSON_HEX() < (YGGR_MAKE_BSON_HEX(1, 15, 0)))

#define YGGR_BSON_EMPTY_MARK() ("\005\0\0\0\0")
#define YGGR_BSON_EMPTY_MARK_LENGTH() (5)

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_CONFIG_HPP__
