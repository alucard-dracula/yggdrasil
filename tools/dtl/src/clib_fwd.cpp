#include "clib_fwd.h"
#include "string_util.h"

#include "std_warn_off.h"

#include <cstring>
#include <cctype>

#include <wchar.h>
#include <wctype.h>

#include "std_warn_on.h"

using namespace std;

BEGIN_DTL_NAMESPACE

void *std_memset(void *dest, int c, size_t sz)
{
   return memset(dest, c, sz);
}

// forwarding functions for portability of C standard library functions
// as GNU doesn't support :: for global namespace
int std_tstrcmp(const TCHAR *cstr1, const TCHAR *cstr2)
{
   return _tcscmp(cstr1, cstr2);
}

size_t std_tstrlen(const TCHAR *cstr)

{
   return _tcslen(cstr);
}

// forwarding functions for portability of C standard library functions
// as GNU doesn't support :: for global namespace
int std_strcmp(const char *cstr1, const char *cstr2)
{
   return strcmp(cstr1, cstr2);
}

size_t std_strlen(const char *cstr)
{
   return strlen(cstr);
}

int std_isspace(int ch)
{
	return isspace(ch);
}

TCHAR *std_tstrncpy(TCHAR *dest, const TCHAR *src, size_t n)
{
   return _tcsncpy(dest, src, n);
}

char *std_strncpy(char *dest, const char *src, size_t n)
{
   return strncpy(dest, src, n);
}

TCHAR *std_tstrtok(TCHAR *cstr, const TCHAR *delim, TCHAR **savept)
{
#ifdef WIN32
  return _tcstok(cstr, delim);
#else
  return strtok_r(cstr, delim, savept);
#endif
}

TCHAR *std_tstrcat(TCHAR *dest, const TCHAR *src)
{
	return _tcscat(dest, src);
}

int std_wcscmp(const wchar_t *cstr1, const wchar_t *cstr2)
{
	return wcscmp(cstr1, cstr2);
}

size_t std_wcslen(const wchar_t *cstr)
{
	return wcslen(cstr);
}

int std_tisalpha(int c)

{
#ifdef UNICODE // MFC workaround has conflict with _iswalpha already defined
 const wctype_t alpha(wctype("alpha"));
 return iswctype(c,alpha);
#else
 return isalpha(c);
#endif
}

int std_tisspace(int c)
{
#ifdef UNICODE // MFC workaround has conflict with _iswalpha already defined
 const wctype_t space(wctype("space"));
 return iswctype(c,space);
#else
 return isspace(c);
#endif
}


TCHAR *std_tstrcpy(TCHAR *dest, const TCHAR *src)
{
   return _tcscpy(dest, src);

}


int std_ttoupper(int c)
{
   return _totupper(c);
}

int std_ttolower(int c)
{
  return _totlower(c);
}



double std_atof(const char *cstr)
{
  return atof(cstr);
}

double std_tatof(const TCHAR *cstr)
{
  	#ifndef DTL_NO_UNICODE
	  double val;
	  TCHAR *stopstr;
	  val = _tcstod(cstr, &stopstr);
	  return val;
	#else
	  return atof(cstr);
	#endif
}

int std_tatoi(const TCHAR *cstr)
{
  return _ttoi(cstr);
}

// forwarding functions for portability of C standard library functions
// as GNU doesn't support :: for global namespace

int std_tstricmp(const TCHAR *s1, const TCHAR *s2)
{
#ifdef _MSC_VER
   return _tcsicmp(s1, s2);
#else
	 return stricmp(s1, s2);
#endif
}

int std_tstrnicmp(const TCHAR *s1, const TCHAR *s2, size_t n)
{
#ifdef _MSC_VER
   return _tcsnicmp(s1, s2, n);
#else
	 return strnicmp(s1, s2,  n);
#endif
}

const TCHAR*
std_tstristr (const TCHAR* haystack, const TCHAR* needle)
{
  // XXX Naive implementation; see
  // <http://www.concentric.net/~Ttwang/tech/stringscan.htm>
  while (haystack[0] != 0) {
    int i = 0;

    while (std_ttolower (haystack[i]) == std_ttolower (needle[i])
	   && haystack[i] != 0 && needle[i] != 0) ++i;

    if (needle[i] == 0)
      return (TCHAR*) haystack;

    ++haystack;
  }

  return (TCHAR*) (NULL);
}

wchar_t *std_wcsncpy(wchar_t *dest, const wchar_t *src, size_t n)
{
   return wcsncpy(dest, src, n);
}

void *std_memcpy(void *dest, const void *src, size_t n)
{
   return memcpy(dest, src, n);
}

char *std_strcat(char *dest, const char *src)
{
   return strcat(dest, src);
}

char *std_strcpy(char *dest, const char *src)
{
   return strcpy(dest, src);
}

wchar_t *std_wcscat(wchar_t *dest, const wchar_t *src)
{
   return wcscat(dest, src);
}

#if !defined(DTL_NO_UNICODE)
// DLP: added to support STD_::wstring 12/6/2001
STD_::wstring tstring_cast(const STD_::wstring *dmy, const STD_::string &b) {
	STD_::wstring strwide;	
	strwide.reserve(b.length());
	

	STD_::string::const_iterator i(b.begin());
	while (i != b.end()) {
		strwide += btowc(*i);
		i++;
	}
	return strwide;

}

STD_::string tstring_cast(const STD_::string *dmy, const STD_::wstring &b) {
	STD_::string strnarrow;	
	strnarrow.reserve(b.length());
	
	STD_::wstring::const_iterator i(b.begin());
	while (i != b.end()) {
		strnarrow += wctob(*i);
		i++;
	}
	return strnarrow;
}
#endif

END_DTL_NAMESPACE
