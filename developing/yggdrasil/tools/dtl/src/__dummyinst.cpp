#ifdef _RTLDLL
#define NEEDCHANGE
#undef _RTLDLL
#endif

#ifdef __BORLANDC__
#  ifndef _RWSTD_COMPILE_INSTANTIATE
#    define _RWSTD_COMPILE_INSTANTIATE 1
#    define __UNDEF_LATER 1
#  endif
#endif

#include <string>

#pragma option -Jgd
template class std::basic_string<unsigned char>;


#ifdef __BORLANDC__
#  ifdef __UNDEF_LATER
#    undef _RWSTD_COMPILE_INSTANTIATE
#    undef __UNDEF_LATER
#  endif
#endif

#ifdef NEEDCHANGE
#undef NEEDCHANGE
#define _RTLDLL
#endif
