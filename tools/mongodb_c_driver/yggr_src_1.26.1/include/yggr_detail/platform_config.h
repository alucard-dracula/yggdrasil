// platform_config.h

#ifndef __MONGODB_YGGR_EX_PLATFORM_CONFIG_H__
#define __MONGODB_YGGR_EX_PLATFORM_CONFIG_H__

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(WINDOWS)
#	define YGGR_EX_PLATFORM_WINDOWS 1
#endif // #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(WINDOWS)

#if defined(__MINGW32__)
#	define YGGR_EX_PLATFORM_MINGW32 1
#endif // #if defined(__MINGW32__)

#if defined(__CYGWIN__)
#	define YGGR_EX_PLATFORM_CYGWIN 1
#endif // #if defined(__CYGWIN__)


#if defined(__APPLE__) && defined(__MACH__) && !defined(__unix__)
#	define YGGR_EX_PLATFORM_DARWIN 1
#endif // #if defined(__APPLE__) && defined(__MACH__) && !defined(__unix__)

#if defined(__unix__) || defined(__unix)
#	include <sys/param.h>
#	if defined(__linux__)
#		define YGGR_EX_PLATFORM_LINUX 1
#	endif // #	if defined(__linux__)
#	if defined(__ANDROID__)
#		define YGGR_EX_PLATFORM_ANDROID 1 
#	endif // __ANDROID__
#endif // #if defined(__unix__) || defined(__unix)

#if defined(BSD)
#	define YGGR_EX_PLATFORM_BSD 1
#endif // #if defined(BSD)

#if !(defined(YGGR_EX_PLATFORM_WINDOWS) || defined(YGGR_EX_PLATFORM_MINGW32) \
		|| defined(YGGR_EX_PLATFORM_DARWIN) || defined(YGGR_EX_PLATFORM_LINUX) \
		|| defined(YGGR_EX_PLATFORM_ANDROID) || defined(YGGR_EX_PLATFORM_BSD) )

#	define YGGR_EX_PLATFORM_UNIX 1

#endif // !(defined ...)


#endif // __MONGODB_YGGR_EX_PLATFORM_CONFIG_H__
