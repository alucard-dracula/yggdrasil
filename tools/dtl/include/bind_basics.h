// Emacs, this is -*- c++ -*- code.

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
// this module contains the basic bind operators for binding DB columns
// and SQL parameters to memory structures
// Initial: 9/6/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 11/14/2000 - MG - inherited ETIException from RootException
// Edited: 12/19/2000 - MG - added namespaces
// Edited: 03/24/2004 - Alexander Motzkau, TypeTranslationField remembers variant_row index

#ifndef DTL_BIND_BASICS_H
#define DTL_BIND_BASICS_H

#include "dtl_config.h"
#include "RootException.h"
#include "minimax.h"
#include "array_string.h"

#include "std_warn_off.h"

#ifdef DTL_NATIVE_WIN32
	#ifdef DTL_USE_MFC
		#include <afx.h>
	#else
		#include <windows.h>
	#endif
#endif

#include <sql.h>
#include <sqlucode.h>

#include <string>
#include <map>

#include <typeinfo>

#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

// Workaround broken compilers
#define DTL_TYPEID_NAME(T) typeid (T).name ( )

// remove all occurrences of the word "const" in the string
// representing a type this should only be used for field types
STD_::string StripConstFromTypename(const STD_::string &typeNm);

// what kind of type do we have
// enum TypeComplexity { TYPE_PRIMITIVE, TYPE_COMPLEX, TYPE_INVALID };

class ETIException : public RootException
{
public:

	ETIException(const tstring &meth, const tstring &msg);

   // RootException::what() sufficient for our needs
    virtual ~ETIException() throw() {};
};

// used for TCHAR arrays for binding
struct tchar_struct
{
	TCHAR *buf;
	size_t size;

	tchar_struct(TCHAR *ptr, size_t sz) : buf(ptr), size(sz) { }
};

#define DTL_CHAR_FIELD(char_array) \
	tchar_struct(char_array, sizeof(char_array))

// contains translations from SQL to C types short, unsigned short,
// int, unsigned int, long, unsigned long double, float, struct
// TIMESTAMP_STRUCT, char *, tstring, char, bool, blob, etc.
// 
// DLP: added std::wstring (Jun-12-2001)
// PCH: added TIME and DATE (Jun-20-2005)

enum {
  C_UNKNOWN = 0,
  C_SHORT = 'n',
  C_USHORT = 'N',
  C_INT = 'i',
  C_UINT = 'I',
  C_LONG = 'l',
  C_ULONG = 'L',
  C_DOUBLE = 'd',
  C_FLOAT = 'f',
  C_TIMESTAMP = 't',
  C_CHAR_STAR = 'C' - 'A' + 1,
  C_STRING = 's',
  C_BLOB = 'S',
  C_JTIME_C = 'j',
  C_BOOL = 'b',
  C_WCHAR_STAR = 'C' - 'A' + 2,
  C_WSTRING = 'w',
  C_TCSTRING = 'T',
  C_INT64 = '6',
  C_DATE = 'D',
  C_TIME = 'm'    //  m for tiMe
};

// We intentionally overflow the buffer since a real cstring<> is cast
// to this fake one so that non-template code can still use the the
// real one.  Recall that array_string adds an extra space in the
// buffer for the terminator, so it is fine to use a 0-length string.
#define MAKE_FAKE(NAME,CHAR) \
struct fake_##NAME \
{ \
  typedef CHAR char_type; \
  typedef CHAR* pointer; \
 \
  CHAR buf[1]; \
}

MAKE_FAKE (tcstring, TCHAR);

#undef MAKE_FAKE

class TypeTranslation
{
public:
   // what kind of type do we have
   enum TypeComplexity { TYPE_PRIMITIVE, TYPE_COMPLEX, TYPE_INVALID };

   STD_::string typeNm;
   char   typeId;
   SQLSMALLINT sqlType;
   SQLSMALLINT cType;
   TypeComplexity complexity;  // is the type primitive or complex
   size_t size;
   bool bParam;

   TypeTranslation();

   TypeTranslation(const STD_::string &nm, char id, SQLSMALLINT sql, SQLSMALLINT c,
				   TypeComplexity comp, size_t s);

   // exception-safe swap()
   void swap(TypeTranslation &other);

   // exception-safe assignment
   TypeTranslation &operator=(const TypeTranslation &other);

   // is this a primitive type
   bool IsPrimitive() const;

   // is this a parameter type
   bool IsParam() const;
   void SetParam(bool isParamter);

   friend bool operator==(const TypeTranslation &tt1, const TypeTranslation &tt2);

   friend bool operator!=(const TypeTranslation &tt1, const TypeTranslation &tt2);
   
   friend class ETI_Map;
};

// extended type information containing type name and SQL to C mappings
// for the appropriate types
class ETI_Map : public STD_::map<STD_::string, TypeTranslation>
{
    public:
		ETI_Map();

		ETI_Map(const ETI_Map &other);

		// build our ETI_Map of SQL to C type mappings
		void build();

		// does the stringified type represent a primitive?
		// primitive is defined as being in the map
		// if not in map, we assume the type is not primitive
		bool IsPrimitive(const STD_::string &typeNm) const;

		iterator find_by_typeid(char type);
};


// the actual type translation map is here
// extern ETI_Map SQL_types_to_C;

// returns the type translation map used by the library
// implemented as a local static in a function rather than a global variable
// to enforce proper initialization order within global variables that must use the map
// to initialize themselves
ETI_Map &GetSQL_types_to_C();

// used in GenericCmp()
extern const int TYPE_NOT_SUPPORTED;

// compare the contents of the pointers of the type specified by CTypeStr
// if (*pMember1 < *pMember2)
//	 return -1;
// else if (*pMember1 == *pMember2)
//   return 0;
// else if (*pMember1 > *pMember2)
//	 return 1;
int GenericCmp(BYTE* pMember1, BYTE* pMember2, int typeId);

// generic hash function
size_t GenericHash(BYTE* pMember1, int typeId);

// map SQL column type to C type
TypeTranslation Map_SQL_types_to_C(int sql_type);

// used for mapping variant_row fields into a BCA
class TypeTranslationField {
public:
		TypeTranslation &tt;
		BYTE* field;
		BYTE* base_addr;
		size_t base_size;
		tstring &field_name;
		int field_nr;

      TypeTranslationField(TypeTranslation &t, BYTE* f, BYTE* ba, size_t bs, tstring &fn, int fi) :
        tt(t), field(f), base_addr(ba), base_size(bs), field_name(fn), field_nr(fi) {}

};

END_DTL_NAMESPACE

#endif
