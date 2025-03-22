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

//#if !(defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64))
//#	pragma option -Jgd
//#endif // _MSC_VER

//template class std::basic_string<unsigned char>;

#if !(defined(__ANDROID__) || defined(ANDROID))

template class std::basic_string<unsigned char>;

#endif // __ANDROID__

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
