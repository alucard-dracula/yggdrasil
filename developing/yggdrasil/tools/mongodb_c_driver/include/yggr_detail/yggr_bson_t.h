// yggr_ex_foo.h 

// this is ipp file
// if use it please append BUILD_MONGODB_DRIVER_YGGR_VER to build maroc property 
#ifndef __MONGODB_YGGR_EX_FOO_H__
#define __MONGODB_YGGR_EX_FOO_H__

#ifndef MONGODB_DRIVER_INCLUDED_YGGR_VER
#	define MONGODB_DRIVER_INCLUDED_YGGR_VER
#endif // BUILD_MONGODB_DRIVER_YGGR_VER

MONGO_EXPORT int bson_append_estart(bson*, int, const char*, const int);
MONGO_EXPORT void bson_append_byte(bson*, char);
MONGO_EXPORT void bson_append(bson*, const void*, int);
MONGO_EXPORT void bson_append32(bson*, const void*);
MONGO_EXPORT void bson_append64(bson*, const void*);

#endif // __MONGODB_YGGR_EX_FOO_H__