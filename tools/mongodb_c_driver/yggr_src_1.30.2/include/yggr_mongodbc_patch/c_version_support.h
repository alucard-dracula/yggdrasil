// c_version_support.h

#ifndef __MONGODB_YGGR_MONGODBC_PATCH_C_VERSION_SUPPORT_H__
#define __MONGODB_YGGR_MONGODBC_PATCH_C_VERSION_SUPPORT_H__

#if defined(__STDC_VERSION__)
#	if (__STDC_VERSION__ < 201112L)
#       if !defined(YGGR_EX_C99_SUPPORTED)
#		    define YGGR_EX_C99_SUPPORTED 1 // 199901L
#       endif // YGGR_EX_C99_SUPPORTED
#	elif (__STDC_VERSION__ < 201710L)
#       if !defined(YGGR_EX_C99_SUPPORTED)
#		    define YGGR_EX_C99_SUPPORTED 1 // 199901L
#       endif // YGGR_EX_C99_SUPPORTED
#       if !defined(YGGR_EX_C11_SUPPORTED)
#		    define YGGR_EX_C11_SUPPORTED 1 // 201112L
#       endif // YGGR_EX_C11_SUPPORTED
#	else
#       if !defined(YGGR_EX_C99_SUPPORTED)
#		    define YGGR_EX_C99_SUPPORTED 1 // 199901L
#       endif // YGGR_EX_C99_SUPPORTED
#       if !defined(YGGR_EX_C11_SUPPORTED)
#		    define YGGR_EX_C11_SUPPORTED 1 // 201112L
#       endif // YGGR_EX_C11_SUPPORTED
#       if !defined(YGGR_EX_C17_SUPPORTED)
#		    define YGGR_EX_C17_SUPPORTED 1 // 201710L
#       endif // YGGR_EX_C17_SUPPORTED
#	endif //__STDC_VERSION__
#else
#	if defined(_MSC_VER)
#		if (_MSC_VER >= 1900)
#           if !defined(YGGR_EX_C99_SUPPORTED)
#			    define YGGR_EX_C99_SUPPORTED 1 // 199901L
#           endif // YGGR_EX_C99_SUPPORTED
#		endif // if (_MSC_VER >= 1900)
#	endif // _MSC_VER
#endif // __STDC_VERSION__

#endif // __MONGODB_YGGR_MONGODBC_PATCH_C_VERSION_SUPPORT_H__
