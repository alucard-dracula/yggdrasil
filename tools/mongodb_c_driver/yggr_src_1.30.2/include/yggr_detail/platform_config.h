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
#	if (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) \
		|| (defined(TARGET_OS_IOS) && TARGET_OS_IOS)
#		define YGGR_EX_PLATFORM_IOS 1
#	endif // TARGET_OS_IPHONE

#endif // #if defined(__APPLE__) && defined(__MACH__) && !defined(__unix__)

#if defined(__unix__) || defined(__unix)
#	include <sys/param.h>
#	if defined(__linux__)
#		define YGGR_EX_PLATFORM_LINUX 1
#	endif // #	if defined(__linux__)
#	if defined(__ANDROID__) || defined(ANDROID) 
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

// arch
#if (defined(__arm__) && __arm__) || (defined(__arm) && (__arm))
#	define YGGR_EX_ARM 1
#endif // #if (defined(__arm__) && __arm__) || (defined(__arm) && (__arm))

#if (defined(__arm64__) && __arm64__) || (defined(__arm64) && (__arm64))
#	define YGGR_EX_ARM64 1
#endif // #if (defined(__arm__) && __arm__) || (defined(__arm) && (__arm))

#if defined(YGGR_EX_ARM) || defined(YGGR_EX_ARM64)
#	define YGGR_EX_ARM_PLATFORM 1
#endif // #if defined(YGGR_EX_ARM) || defined(YGGR_EX_ARM64)

#if (defined(__riscv) && __riscv)
#	define YGGR_EX_RISCV 1
#endif // #if (defined(__riscv) && __riscv)

#if defined(YGGR_EX_RISCV)
#	define YGGR_RISCV_PLATFORM 1
#endif // #if defined(YGGR_EX_ARM) || defined(YGGR_EX_ARM64)

#if (defined(_M_IX86) && _M_IX86) \
	|| (defined(__i386) && __i386) ||  (defined(__i386__) && __i386__) \
	|| (defined(__i486) && __i486) ||  (defined(__i486__) && __i486__) \
	|| (defined(__i586) && __i586) ||  (defined(__i586__) && __i586__) \
	|| (defined(__i686) && __i686) ||  (defined(__i686__) && __i686__)
#	define YGGR_EX_X86 1
#endif // i386 i686

#if (defined(_M_X64) && _M_X64) \
	|| (defined(__amd64) && __amd64) ||  (defined(__amd64__) && __amd64__) \
	|| (defined(__x86_64) && __x86_64) ||  (defined(__x86_64__) && __x86_64__)
#	define YGGR_EX_X86_64 1
#endif // amd64 x86_64

#if defined(YGGR_EX_X86) || defined(YGGR_EX_X86_64)
#	define YGGR_X86_PLATFORM 1
#endif // #if defined(YGGR_EX_ARM) || defined(YGGR_EX_ARM64)

#if !defined(YGGR_EX_SYSTEM_64)
#	if defined(YGGR_EX_ARM64) || defined(YGGR_EX_X86_64) \
		|| (defined(__UINTPTR_WIDTH__) && (__UINTPTR_WIDTH__ == 64))
#		define YGGR_EX_SYSTEM_64 1
#	endif //  arm64 x86_64 __UINTPTR_WIDTH__
#endif // YGGR_EX_SYSTEM_64


#endif // __MONGODB_YGGR_EX_PLATFORM_CONFIG_H__
