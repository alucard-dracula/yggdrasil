#ifndef MLIB_MACROS_H_INCLUDED
#define MLIB_MACROS_H_INCLUDED

#include "./user-check.h"
#include <yggr_detail/mongo_c_inline.h>
#include <yggr_detail/c_version_support.h>
#include <yggr_detail/warning_check.h>

/**
 * @brief Cross-C/C++ compatibility for a compound initializer to be treated as
 * a braced initializer
 *
 */
//#ifdef __cplusplus
//#define MLIB_INIT(T) T
//#else
//#define MLIB_INIT(T) (T)
//#endif

#ifdef __cplusplus
#	define MLIB_INIT(T) T
#else
#	if defined(YGGR_EX_C99_SUPPORTED)
#		define MLIB_INIT(T) (T)
#	else
#		define MLIB_INIT(T) 
#	endif // YGGR_EX_C99_SUPPORTED
#endif


#ifdef __cplusplus
#define _mlibCLinkageBegin extern "C" {
#define _mlibCLinkageEnd }
#else
#define _mlibCLinkageBegin
#define _mlibCLinkageEnd
#endif

/// Mark the beginning of a C-language-linkage section
#define MLIB_C_LINKAGE_BEGIN _mlibCLinkageBegin
/// End a C-language-linkage section
#define MLIB_C_LINKAGE_END _mlibCLinkageEnd

//#if (defined(__cpp_constexpr) && __cpp_constexpr >= 201304L) || (defined(__cplusplus) && __cplusplus >= 201402L)       \
//    || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L)
//#define _mlibConstexprFn constexpr inline
//#else
//#define _mlibConstexprFn inline
//#endif

#if (defined(__cpp_constexpr) && __cpp_constexpr >= 201304L) || (defined(__cplusplus) && __cplusplus >= 201402L)       \
    || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L)
#define _mlibConstexprFn constexpr inline
#else
#define _mlibConstexprFn MONGOC_INLINE
#endif

/**
 * @brief Mark a function as constexpr
 *
 * Expands to `constexpr inline` in C++14 and above (and someday C26...?).
 * "inline" otherwise.
 */
#define mlib_constexpr_fn _mlibConstexprFn

#ifdef __GNUC__
#define MLIB_ANNOTATE_PRINTF(FStringArgAt, VarArgsStartAt)                                                             \
    __attribute__((format(__printf__, FStringArgAt, VarArgsStartAt)))
#else
#define MLIB_ANNOTATE_PRINTF(FStringArgAt, VarArgsStartAt) /* no-op */
#endif

#endif // MLIB_MACROS_H_INCLUDED
