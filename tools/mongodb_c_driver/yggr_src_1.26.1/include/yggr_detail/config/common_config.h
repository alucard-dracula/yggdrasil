// common_config.h

#ifndef __MONGODB_YGGR_EX_COMMON_CONFIG_H__
#define __MONGODB_YGGR_EX_COMMON_CONFIG_H__

#include <yggr_detail/c_version_support.h>
#include <yggr_detail/platform_config.h>

#if defined(_DEBUG)
#	define MONGOC_ENABLE_DEBUG_ASSERTIONS 1
#else
#	define MONGOC_ENABLE_DEBUG_ASSERTIONS 0
#endif // MONGOC_ENABLE_DEBUG_ASSERTIONS

#if MONGOC_ENABLE_DEBUG_ASSERTIONS != 1
#  undef MONGOC_ENABLE_DEBUG_ASSERTIONS
#endif

#endif // __MONGODB_YGGR_EX_COMMON_CONFIG_H__
