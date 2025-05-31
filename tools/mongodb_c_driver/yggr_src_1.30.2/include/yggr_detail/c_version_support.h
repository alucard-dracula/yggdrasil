// c_version_support.h

#ifndef __MONGODB_YGGR_EX_C_VERSION_SUPPORT_H__
#define __MONGODB_YGGR_EX_C_VERSION_SUPPORT_H__

#if defined(__STDC_VERSION__)
#	if (__STDC_VERSION__ < 201112L)
#		define YGGR_EX_C99_SUPPORTED 1 // 199901L
#	elif (__STDC_VERSION__ < 201710L)
#		define YGGR_EX_C99_SUPPORTED 1 // 199901L
#		define YGGR_EX_C11_SUPPORTED 1 // 201112L
#	else
#		define YGGR_EX_C99_SUPPORTED 1 // 199901L
#		define YGGR_EX_C11_SUPPORTED 1 // 201112L
#		define YGGR_EX_C17_SUPPORTED 1 // 201710L
#	endif //__STDC_VERSION__
#else
#	if defined(_MSC_VER)
#		if (_MSC_VER >= 1900)
#			define YGGR_EX_C99_SUPPORTED 1 // 199901L
#		endif // if (_MSC_VER >= 1900)
#	endif // _MSC_VER
#endif // __STDC_VERSION__

#endif // __MONGODB_YGGR_EX_C_VERSION_SUPPORT_H__
