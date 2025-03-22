#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#if defined(BUILD_MONGODB_DRIVER_YGGR_VER)

#	include <yggr_detail/config/common_config.h>

#else

#define MONGOC_ENABLE_DEBUG_ASSERTIONS 0

#if MONGOC_ENABLE_DEBUG_ASSERTIONS != 1
#  undef MONGOC_ENABLE_DEBUG_ASSERTIONS
#endif

#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#endif
