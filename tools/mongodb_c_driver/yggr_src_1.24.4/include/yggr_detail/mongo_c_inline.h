// mongo_c_inline.h

#ifndef __MONGODB_YGGR_EX_MONGO_C_INLINE_H__
#define __MONGODB_YGGR_EX_MONGO_C_INLINE_H__

#ifndef MONGOC_INLINE
#	if defined(_MSC_VER)
#		define MONGOC_INLINE __inline
#	elif defined(__GNUC__)
#		define MONGOC_INLINE __inline__
#	else
#		define MONGOC_INLINE inline
#	endif /* _MSC_VER or __GNUC__ */
#endif // MONGO_INLINE

#endif // __MONGODB_YGGR_EX_MONGO_C_INLINE_H__