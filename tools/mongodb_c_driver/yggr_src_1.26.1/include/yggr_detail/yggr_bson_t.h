// yggr_bson_t.h

// this is ipp file
// if use it please append BUILD_MONGODB_DRIVER_YGGR_VER to build maroc property
#ifndef __MONGODB_YGGR_EX_BSON_H__
#define __MONGODB_YGGR_EX_BSON_H__

#ifndef BSON_H
#	error "don\'t include this file <yggr_bson_t.h>, please include <bson.h>."
#endif // BSON_H

#define YGGR_BSON_MAJOR BSON_MAJOR_VERSION
#define YGGR_BSON_MINOR BSON_MINOR_VERSION
#define YGGR_BSON_PATCH BSON_MICRO_VERSION

#define YGGR_BSON_HEX() (YGGR_BSON_MAJOR << 24 | \
                          YGGR_BSON_MINOR << 16 | \
                          YGGR_BSON_PATCH << 8)

#define YGGR_MAKE_BSON_HEX( __major__, __minor__, __micro__) \
			(	__major__ << 24 | \
				__minor__ << 16 | \
				__micro__ << 8 )

#ifndef MONGODB_DRIVER_INCLUDED_YGGR_VER
#	define MONGODB_DRIVER_INCLUDED_YGGR_VER
#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#endif // __MONGODB_YGGR_EX_BSON_H__
