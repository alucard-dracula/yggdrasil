//bson.h

#ifndef __BSON_H__
#define __BSON_H__


#ifdef MONGO_DLL_BUILD
#	error "now ver must use static lib ver."
#endif //MONGO_DLL_BUILD

#ifdef MONGO_STATIC_BUILD
#	undef MONGO_STATIC_BUILD
#endif //MONGO_STATIC_BUILD


#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
//#	if defined(MONGO_USE_STATIC_LIB)
//#		define MONGO_STATIC_BUILD
//#	elif defined(MONGO_BUILD)
//#		define MONGO_DLL_BUILD
//#	endif //MONGO_DLL_BUILD
#define MONGO_STATIC_BUILD //now ver static is best of select
#endif //WIN32

#if defined(LINUX) || defined(LINUX64)
#define MONGO_HAVE_STDINT
#endif // defined(LINUX) || defined(LINUX64)

#if !defined(MONGO_HAVE_STDINT) && !defined(MONGO_HAVE_UNISTD) && !defined(MONGO_USE__INT64) && !defined(MONGO_USE_LONG_LONG_INT)
	#define MONGO_USE_LONG_LONG_INT
#endif

#include <bson_t.h>

#endif // __BSON_H__
