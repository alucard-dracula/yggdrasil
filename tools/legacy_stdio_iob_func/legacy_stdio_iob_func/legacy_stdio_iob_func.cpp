// legacy_stdio_iob_func.cpp

#if 1800 < _MSC_VER

#include <stdio.h>

#if defined(__cplusplus)

extern "C"
{
	
FILE __iob_func[3] = {*stdin, *stdout, *stderr};

} //extern "C"

#endif // __cplusplus

#endif // 1800 < _MSC_VER

