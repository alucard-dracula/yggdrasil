//stroull.h

#ifndef _MSC_VER // [
#error "Use this header only with Microsoft Visual C++ compilers!"
#endif // _MSC_VER ]

#ifndef __STROULL_H__
#define __STROULL_H__

#if defined(_MSC_VER) && (_MSC_VER < 1800)

#include <mscompatibility/stdint.h>

int64_t strtoll(const char *nptr, char **endptr, register int base);
uint64_t strtoull(const char *nptr, char **endptr, register int base);

//__inline uint64_t atoll(const char* nptr) { return strtoll(nptr, 0, 10); } 

#endif // _MSC_VER

#endif // __STROULL_H__