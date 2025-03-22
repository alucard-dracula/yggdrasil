// bson.h

#ifndef ___BSON_H___
#define ___BSON_H___

#ifdef BUILD_MONGODB_DRIVER_YGGR_VER
#	include <yggr_detail/c_version_support.h>
#	include <yggr_detail/platform_config.h>
#endif //BUILD_MONGODB_DRIVER_YGGR_VER

#include <bson/bson.h>

#ifdef BUILD_MONGODB_DRIVER_YGGR_VER
#	include <yggr_detail/yggr_bson_t.h>
#endif //BUILD_MONGODB_DRIVER_YGGR_VER

#endif // ___BSON_H___