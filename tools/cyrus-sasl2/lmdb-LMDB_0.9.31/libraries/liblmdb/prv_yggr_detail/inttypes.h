#ifndef HEADER_A06026A423D62438
#define HEADER_A06026A423D62438

// inttypes.h

#ifndef __LMDB_PRV_YGGR_EX_INTTYPES_H__
#define __LMDB_PRV_YGGR_EX_INTTYPES_H__

#include "c_version_support.h"

#if defined(YGGR_EX_C99_SUPPORTED)
#	include <inttypes.h>
#else

#	include <stdint.h>

#ifndef PRIi8
#	define PRIi8 "d"
#endif // PRIi8

#ifndef PRIi16
#	define PRIi16 "d"
#endif // PRIi16

#ifndef PRIi32
#	define PRIi32 "d"
#endif // PRIi32

#ifndef PRIi64
#	define PRIi64 "I64i"
#endif // PRIi64


#ifndef PRId8
#	define PRId8 "d"
#endif // PRId8

#ifndef PRId16
#	define PRId16 "d"
#endif // PRId16

#ifndef PRId32
#	define PRId32 "d"
#endif // PRId32

#ifndef PRId64
#	define PRId64 "I64i"
#endif // PRId64


#ifndef PRIu8
#	define PRIu8 "u"
#endif // PRIu8

#ifndef PRIu16
#	define PRIu16 "u"
#endif // PRIu16

#ifndef PRIu32
#	define PRIu32 "u"
#endif // PRIu32

#ifndef PRIu64
#	define PRIu64 "I64u"
#endif //PRIu64

#endif // YGGR_EX_C99_SUPPORTED

#endif // __LMDB_PRV_YGGR_EX_INTTYPES_H__
#endif // header guard 

