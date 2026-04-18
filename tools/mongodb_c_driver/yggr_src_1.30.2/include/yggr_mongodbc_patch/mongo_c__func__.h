// mongo_c__func__.h

#ifndef __MONGODB_YGGR_MONGODBC_PATCH_MONGO_C__FUNC__H__
#define __MONGODB_YGGR_MONGODBC_PATCH_MONGO_C__FUNC__H__

#include <yggr_mongodbc_patch/c_version_support.h>

#if defined(_MSC_VER)
#	define MONGO_C__FUNC__ __FUNCTION__
#else
#	define MONGO_C__FUNC__ __func__
#endif // _MSC_VER

#endif // __MONGODB_YGGR_MONGODBC_PATCH_MONGO_C__FUNC__H__
