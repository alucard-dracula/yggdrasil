/* Copyright ?2000 
Michael Gradman and Corwin Joy 

Permission to use, copy, modify, distribute and sell this software and 
its documentation for any purpose is hereby granted without fee, provided 
that the above copyright notice appears in all copies and that both that 
copyright notice and this permission notice appear in supporting documentation. 
Corwin Joy and Michael Gradman make no representations about the suitability 
of this software for any purpose. 
It is provided "as is" without express or implied warranty.
*/ 
// misc. tstring utilities for our DB abstractions
// Initial: 9/7/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 12/19/2000 - MG - added namespaces

#include "string_util.h"

#include "RootException.h"
#include "clib_fwd.h"

#include "std_warn_off.h"
#include <algorithm>
#include <cassert>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE
// parse a comma-separated list of strings, packaging the result in a STD_::vector
// of the strings
STD_::vector<tstring> ParseCommaDelimitedList(tstring commaList)
{
	  tstring commaListCopy = commaList; // make copy of tstring as strtok() may destroy the 
											  // original
	  STD_::vector<tstring> resultList;
	  TCHAR *tokenName;

	  TCHAR *c_str = new TCHAR[commaListCopy.length() + 1];

	  if (c_str == NULL)
		  DTL_THROW RootException(_TEXT("ParseCommaDelimetedList()"), _TEXT("out of memory"));

	  std_tstrncpy(c_str, commaListCopy.c_str(), commaListCopy.length() + 1);

	  // grab first entry
	  TCHAR *savept = NULL;
	  tokenName = std_tstrtok(c_str, _TEXT(","), &savept);

	  // add latest grabbed name to the set and grab next token
	  // while there are more names to be parsed
	  while (tokenName != NULL)
	  {
	    ltrim(tokenName);
	    resultList.push_back(tokenName);
	    tokenName = std_tstrtok(NULL, _TEXT(","), &savept);
	  }

	  delete[] c_str;

	  return resultList;
}


// finds the number of occurrences of the character in the STD_string
unsigned int numOfOccurrences(TCHAR c, tstring inString)
{
	unsigned int occurrences = 0;

	for (size_t i = 0; i < inString.length(); i++)
	{
		if (c == inString[i])
		{
			occurrences++;
		}
	}

	return occurrences;
}

const TCHAR* spaces = _TEXT(" \t\r\n\v\f");

void
rtrim (TCHAR* s) // to complex for inline
{
  TCHAR* p = s + std_tstrlen (s) - 1;
  while (std_tisspace (*p)) --p;
  ++p; // looking at non-space; move forward again onto space
  *p = 0;
}

// returns str with all spaces removed
tstring EliminateSpaces(const tstring &str)
{
   tstring result;

   result.reserve(str.size());

   for (size_t i = 0; i < str.length(); i++)
   {
	   if (!std_tisspace(str[i]))
		    // result.push_back(str[i]);
			*(STD_::inserter(result, result.end()))++ = str[i];
   }

   return result;
}



#ifndef _MSC_VER
// define case insensitive STD_string compare routine
// many compilers may already have this

int stricmp(const TCHAR *s1, const TCHAR *s2) {
	assert(s1 != NULL && s2 != NULL);
	TCHAR v1, v2;
	int d=0;
	do{
		v1 = *s1;
		v2 = *s2;
		d = (int)dtl::std_ttoupper(v1) - (int)dtl::std_ttoupper(v2);
		s1++;
		s2++;
	} while (d==0 && v1 > 0);
	return d;
}

int strnicmp(const TCHAR *s1, const TCHAR *s2, size_t len) {
	assert(s1 != NULL && s2 != NULL && len > 0);
	TCHAR v1, v2;
	int d=0;
  size_t i=0;
	do{
		v1 = *s1;
		v2 = *s2;
		d = (int)dtl::std_ttoupper(v1) - (int)dtl::std_ttoupper(v2);
		++s1;
		++s2;
		++i;
	} while (d==0 && v1 > 0 && i < len);
	return d;
}
#endif

// caseifies a SQL Query based on the dtl_config flags
tstring QueryCaseify(const tstring &Query, 
					 const tstring &QuoteChar)
{
#ifdef DTL_LC

	 return QueryToLowercase(Query, QuoteChar);

#elif defined(DTL_UC)

	 return QueryToUppercase(Query, QuoteChar);
#else
     return Query;
#endif
}

// new version of QueryToLowercase()
tstring QueryToLowercase(const tstring &Query, const tstring &QuoteChar)
{
  // q = in single quotes, qq = in double quotes, nq = not in quoted region
  enum qstate { q, qq, nq };

  qstate state = nq;

  tstring NewQuery(Query);

  for (tstring::size_type i = 0, x = NewQuery.size ( ); i < x; ++i) {
    switch (state) {
    case q: i = NewQuery.find(_TEXT('\''), i);
	  if (i == tstring::npos) throw; // missing close single quote
      state = nq;
      break; // ignore quoted region; look again
    case qq: i = NewQuery.find(QuoteChar, i);
	  if (i == tstring::npos) throw; // missing close double quote
      state = nq;
      i += QuoteChar.length() - 1; // handle multicharacter quotes
	  break; // ignore quoted region; look again
    case nq: 
	{
	  // find next quote mark and lower caseify up to that next quote in string
	  tstring::size_type j1 = NewQuery.find(_TEXT('\''), i);
	  tstring::size_type j2 = NewQuery.find(QuoteChar, i);
	  
	  tstring::size_type j = 0;

	  // change state based on whether single or double quote is found
	  if (j1 == tstring::npos && j2 == tstring::npos)
	  {
		j = x; // no more quotes in string
	  }

	  else if (j1 < j2) {
		state = q;
		j = j1;
	  } 
	  else
	  {
		state = qq;
		j = j2;
	  }
	  
	  tstring::iterator first = NewQuery.begin() + i;
	  tstring::iterator last  = NewQuery.begin() + j;

      lower_caseify(first, last);
	  
	  i = j;

	  if (state == qq)
	  {
		 i += QuoteChar.length() - 1; // handle multicharacter quotes
	  }
	}
	
	} // end switch
  } // end for

  return NewQuery;
}

// new version of QueryToUppercase()
tstring QueryToUppercase(const tstring &Query, const tstring &QuoteChar)
{
  // q = in single quotes, qq = in double quotes, nq = not in quoted region
  enum qstate { q, qq, nq };

  qstate state = nq;

  tstring NewQuery(Query);

  for (tstring::size_type i = 0, x = NewQuery.size ( ); i < x; ++i) {
    switch (state) {
    case q: i = NewQuery.find(_TEXT('\''), i);
	  if (i == tstring::npos) throw; // missing close single quote
      state = nq;
      break; // ignore quoted region; look again
    case qq: i = NewQuery.find(QuoteChar, i);
	  if (i == tstring::npos) throw; // missing close double quote
      state = nq;
      i += QuoteChar.length() - 1; // handle multicharacter quotes
	  break; // ignore quoted region; look again
    case nq: 
	{
	  // find next quote mark and lower caseify up to that next quote in string
	  tstring::size_type j1 = NewQuery.find(_TEXT('\''), i);
	  tstring::size_type j2 = NewQuery.find(QuoteChar, i);
	  
	  tstring::size_type j = 0;

	  // change state based on whether single or double quote is found
	  if (j1 == tstring::npos && j2 == tstring::npos)
	  {
		j = x; // no more quotes in string
	  }

	  else if (j1 < j2) {
		state = q;
		j = j1;
	  } 
	  else
	  {
		state = qq;
		j = j2;
	  }
	  
	  tstring::iterator first = NewQuery.begin() + i;
	  tstring::iterator last  = NewQuery.begin() + j;

      upper_caseify(first, last);
	  
	  i = j;

	  if (state == qq)
	  {
		 i += QuoteChar.length() - 1; // handle multicharacter quotes
	  }
	}
	
	} // end switch
  } // end for

  return NewQuery;
}

END_DTL_NAMESPACE


