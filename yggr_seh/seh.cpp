// seh.cpp

#include <yggr/base/yggrdef.h>

#if defined(_MSC_VER)
#	include <yggr/seh/ipps/seh_helper_win.ipp>
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_ANDROID)
#	if defined(YGGR_AT_ANDROID)
#		include <yggr/seh/ipps/seh_env_android.ipp>
#		include <yggr/seh/ipps/seh_helper_android.ipp>
#		include <yggr/seh/ipps/seh_android.ipp>
#	else
#		include <yggr/seh/ipps/seh_env_linux.ipp>
#		include <yggr/seh/ipps/seh_helper_linux.ipp>
#		include <yggr/seh/ipps/seh_linux.ipp>
#	endif // #	if defined(YGGR_AT_ANDROID)
#elif defined(YGGR_AT_DARWIN)
#	include <yggr/seh/ipps/seh_env_darwin.ipp>
#   include <yggr/seh/ipps/seh_helper_darwin.ipp>
#   include <yggr/seh/ipps/seh_darwin.ipp>
#else
//#	error "append other platform code"
#endif // _MSC_VER
