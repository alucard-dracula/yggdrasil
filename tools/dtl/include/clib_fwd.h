// contains prototypes for all functions which must be forwarded to C standard library

#ifndef DTL_CLIB_FWD_H
#define DTL_CLIB_FWD_H

#include "dtl_config.h"

BEGIN_DTL_NAMESPACE

void debug_me();

// forwarding functions for portability of C standard library functions
// as GNU doesn't support :: for global namespace

void *std_memset(void *dest, int c, size_t sz);

int std_tstrcmp(const TCHAR *cstr1, const TCHAR *cstr2);
size_t std_tstrlen(const TCHAR *cstr);
int std_strcmp(const char *cstr1, const char *cstr2);
size_t std_strlen(const char *cstr);

int std_wcscmp(const wchar_t *cstr1, const wchar_t *cstr2);

size_t std_wcslen(const wchar_t *cstr);

int std_isspace(int ch);

TCHAR *std_tstrtok(TCHAR *cstr, const TCHAR *delim, TCHAR **savept);
int std_tisalpha(int c);
TCHAR *std_tstrcpy(TCHAR *dest, const TCHAR *src);
int std_tisspace(int c);
int std_ttoupper(int c);
int std_ttolower(int c);
// DLP: added to support STD_::wstring 12/6/2001
TCHAR *std_tstrncpy(TCHAR *dest, const TCHAR *src, size_t n);
TCHAR *std_tstrcat(TCHAR *dest, const TCHAR *src);

char *std_strncpy(char *dest, const char *src, size_t n);

char *std_strcat(char *dest, const char *src);
char *std_strcpy(char *dest, const char *src);

wchar_t *std_wcscat(wchar_t *dest, const wchar_t *src);

void *std_memcpy(void *dest, const void *src, size_t n);


double std_atof(const char *cstr);
double std_tatof(const TCHAR *cstr);
int std_tatoi(const TCHAR *cstr);

// forwarding functions for portability of C standard library functions
// as GNU doesn't support :: for global namespace
int std_tstricmp(const TCHAR *s1, const TCHAR *s2);
int std_tstrnicmp(const TCHAR *s1, const TCHAR *s2, size_t n);
const TCHAR* std_tstristr (const TCHAR* s1, const TCHAR* s2);

inline STD_::string tstring_cast(const STD_::string * DTL_DUMMY_PARAM_1, const STD_::string &b) {return b;}

#ifndef DTL_NO_UNICODE
wchar_t * std_wcsncpy(wchar_t *dest, const wchar_t *src, size_t n);
STD_::string tstring_cast(const STD_::string *dmy, const STD_::wstring &b);
STD_::wstring tstring_cast(const STD_::wstring *dmy, const STD_::string &b);
inline STD_::wstring tstring_cast(const STD_::wstring * DTL_DUMMY_PARAM_1, const STD_::wstring &b) {return b;}
#endif


template <class InputIterator>
inline std::ptrdiff_t std_distance(InputIterator first,
                     InputIterator last)
{
#ifndef DTL_SOLARIS_DISTANCE
		return static_cast<std::ptrdiff_t>(STD_::distance(first, last));
#else
		std::ptrdiff_t diff = 0;

		STD_::distance(first, last, diff);
		return diff;
#endif

}


END_DTL_NAMESPACE

#endif
