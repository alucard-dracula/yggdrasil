// macro_config.h

#ifndef __LUACJSON_COMPATIBILITY_MACRO_CONFIG_H__
#define __LUACJSON_COMPATIBILITY_MACRO_CONFIG_H__

#if defined(_MSC_VER)

#	if !defined(__cpluscplus)

#		if !defined(inline)
#			define inline __inline
#		endif // inline

#	endif// __cpluscplus

#	if _MSC_VER >= 1300
#		if !defined(snprintf)
#			define snprintf _snprintf
#		endif // snprintf

#		if !defined(isnan)
#			define isnan _isnan
#		endif // isnan
#	endif // _MSC_VER >= 1300

#	if _MSC_VER >= 1300
#		if !defined(snprintf)
#			define snprintf _snprintf
#		endif // snprintf

#		if !defined(isnan)
#			define isnan _isnan
#		endif // isnan
#	endif // _MSC_VER >= 1300

#	if _MSC_VER >= 1200
#		if !defined(strncasecmp)
#			define strncasecmp strnicmp
#		endif // snprintf
#	endif // _MSC_VER >= 1300

#endif // _MSC_VER

#if defined(_MSC_VER) && _MSC_VER <= 1900

#	if !defined(__cpluscplus)

#		if !defined(inline)
#			define inline __inline
#		endif // inline

#		if !defined(snprintf)
#			define snprintf _snprintf
#		endif // snprintf

#		if !defined(isnan)
#			define isnan _isnan
#		endif // isnan

#		if !defined(strncasecmp)
#			define strncasecmp strnicmp
#		endif // strncasecmp

#	endif// __cpluscplus

#endif // _MSC_VER

#endif // __LUACJSON_COMPATIBILITY_MACRO_CONFIG_H__
