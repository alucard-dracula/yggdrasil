// double_limit.h

#ifndef __MONGODB_YGGR_EX_DOUBLE_LIMIT_H__
#define __MONGODB_YGGR_EX_DOUBLE_LIMIT_H__

#include <yggr_detail/mongo_c_inline.h>

#include <math.h>
#include <stdint.h>

#if !defined(NAN)

#	if !defined(__cplusplus)

static MONGOC_INLINE double s_make_nan(void)
{
	static const uint64_t nan_val = 0x7fffffffffffffffui64;
	return *((double *)(&nan_val));
}

#	else

inline static double s_make_nan(void)
{
	static const uint64_t nan_val = 0x7fffffffffffffffui64;
	return *reinterpret_cast<double*>(&nan_val);
}


#endif // #	if !defined(__cplusplus)

#define NAN s_make_nan()

#endif // NAN

#if !defined(INFINITY)

#	if !defined(__cplusplus)

static MONGOC_INLINE double s_make_infinity(void)
{
	static const uint64_t info_val = 0x7ff0000000000000ui64;
	return *((double *)&info_val);
}

static MONGOC_INLINE double s_make_negative_infinity(void)
{
	static const uint64_t info_val = 0xfff0000000000000ui64;
	return *((double *)&info_val);
}

#else

inline static double s_make_infinity(void)
{
	static const uint64_t info_val = 0x7ff0000000000000ui64;
	return *reinterpret_cast<double*>(&info_val);
}

inline static double s_make_negative_infinity(void)
{
	static const uint64_t info_val = 0xfff0000000000000ui64;
	return *reinterpret_cast<double*>(&info_val);
}

#endif // #	if !defined(__cplusplus)

#define INFINITY s_make_infinity()
#define NEGATIVE_INFINITY s_make_negative_infinity()

#endif // INFINITY

#endif // __MONGODB_YGGR_EX_DOUBLE_LIMIT_H__