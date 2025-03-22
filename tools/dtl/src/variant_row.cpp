/* Copyright (c) 2000
Michael Gradman and Corwin Joy

Permission to use, copy, modify, distribute and sell this software and
its documentation for any purpose is hereby granted without fee, provided
that the above copyright notice appears in all copies and that both that
copyright notice and this permission notice appear in supporting documentation.
Corwin Joy and Michael Gradman make no representations about the suitability
of this software for any purpose.
It is provided "as is" without express or implied warranty.
*/
// (C) 2000, Fernando Luis Cacciola Carballal.
// (C) 2000, Modifications & Extensions, Corwin Joy & Michael Gradman
// Major modifications are addition of a typeId + casts from the variant
// to desired target types.  These casts are perhaps not completely type-safe
// but are a major convenience to end users.  As before, one can always examine
// the original and not cast if so desired.
// For our purposes, we also limited the set of types supported in order to get
// these cast operations.
//
// This material is provided "as is", with absolutely no warranty expressed
// or implied. Any use is at your own risk.
//
// Permission to use or copy this software for any purpose is hereby granted
// without fee, provided the above notices are retained on all copies.
// Permission to modify the code and to distribute modified code is granted,
// provided the above notices are retained, and a notice that the code was
// modified is included with the above copyright notice.
//
// Because Michael Gradman & Corwin Joy have modified this extensively from
// the original version, please contact us if you have any questions

// Edited: 10/26/2003 - Paul Grenyer http://www.paulgrenyer.co.uk, added static_cast as required by MSVC 7.1

#include "variant_row.h"

#include "std_warn_off.h"

#include <sstream>
#include <iostream>
#include <wchar.h>

#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

USING_STD_NAMESPACE

// ********* NullField implementation code ***********
// allows NullField() == NullField() == true
bool NullField::operator==(const NullField & DTL_DUMMY_PARAM_1) const
{
	  return true;
}

// allows NullField() != NullField() == false
bool NullField::operator!=(const NullField &null) const
{
	  return !(*this == null);
}

// ********* dtl_variant_t implementation code ***********
void dtl_variant_t::private_construct(const dtl_variant_t &other) {
	if (other.data != NULL )
			other.data->AddRef();
	data = other.data;
	typeId = other.typeId;
}

dtl_variant_t::dtl_variant_t() : data ( NULL ) , typeId(0) {}

dtl_variant_t::dtl_variant_t(const dtl_variant_t & v) : data(NULL), typeId(0)
{private_construct(v);}

dtl_variant_t::dtl_variant_t(const variant_field &vf) : data(NULL), typeId(0)
{
	private_construct(variant_val(vf));
}

dtl_variant_t::dtl_variant_t(BYTE* p, const TypeTranslation &f) {
		typeId = f.typeId;
		switch (typeId) {
		case C_SHORT:  data = new Impl_t<short>(*(reinterpret_cast<short *>(p)));break;
		case C_USHORT: data = new Impl_t<unsigned short>(*(reinterpret_cast<unsigned short *>(p)));break;
		case C_BOOL: data = new Impl_t<bool>(*(reinterpret_cast<bool *>(p)));break;
		case C_INT: data = new Impl_t<int>(*(reinterpret_cast<int *>(p)));break;
		case C_UINT: data = new Impl_t<unsigned int>(*(reinterpret_cast<unsigned int *>(p)));break;
		case C_LONG: data = new Impl_t<long>(*(reinterpret_cast<long *>(p)));break;
		case C_ULONG: data = new Impl_t<unsigned long>(*(reinterpret_cast<unsigned long *>(p)));break;
		case C_INT64: data = new Impl_t<ODBCINT64>(*(reinterpret_cast<ODBCINT64 *>(p)));break;
		case C_DOUBLE: data = new Impl_t<double>(*(reinterpret_cast<double *>(p)));break;
		case C_FLOAT: data = new Impl_t<float>(*(reinterpret_cast<float *>(p)));break;

		// N.B.!! for CHAR *, don't try to copy the data!  Just point to the address of the original!
		// I.E. Copy the pointer just like the class definition says.
		case C_CHAR_STAR: data = new Impl_t<char *>(reinterpret_cast<char *>(p));break;

		case C_STRING: data = new Impl_t<STD_::string>(*(reinterpret_cast<STD_::string *>(p)));break;
		case C_BLOB: data = new Impl_t<blob>(*(reinterpret_cast<blob *>(p)));break;
#ifndef DTL_NO_UNICODE
		case C_WSTRING: data = new Impl_t<STD_::wstring>(*(reinterpret_cast<STD_::wstring *>(p)));break;
#endif
		case C_TIMESTAMP: data = new Impl_t<struct tagTIMESTAMP_STRUCT>(*(reinterpret_cast<struct tagTIMESTAMP_STRUCT  *>(p)));break;

		case C_DATE: data = new Impl_t<struct tagDATE_STRUCT>(*(reinterpret_cast<struct tagDATE_STRUCT *>(p)));break;

		case C_TIME: data = new Impl_t<struct tagTIME_STRUCT>(*(reinterpret_cast<struct tagTIME_STRUCT *>(p)));break;


		default:
			DTL_THROW VariantException(_TEXT("dtl_variant_t::dtl_variant_t()"),
			  tstring(_TEXT("Invalid field type passed to dtl_variant_t(BYTE* p, TypeTranslation &f)")));
			data = NULL;
			//throw TYPE_NOT_SUPPORTED;
		}
		data->AddRef();
	}

dtl_variant_t::~dtl_variant_t()
{ if ( data != NULL )
     data->Release() ;
}

// exception-safe swap()
void dtl_variant_t::swap(dtl_variant_t &other)
{
  STD_::swap(data, other.data);
  STD_::swap(typeId, other.typeId);
}

// exception-safe assignment
dtl_variant_t &dtl_variant_t::operator=(const dtl_variant_t &other)
{
	  if (this != &other)
	  {
		dtl_variant_t temp(other);
		swap(temp);
	  }

	  return *this;
}

// This generic conversion operator let you retrieve
// the value held.
// We disabled this because we wanted to created specialized versions
// that could also supply conversions when needed.
// To avoid template specialization conflicts, it
// returns an instance of type T, which will be a COPY
// of the value contained.
// template<typename T> operator T () const
//   { return CastFromBase<T>( data )->data ; }
//
//   template<typename T> operator T () const
//    {T *dmy; return CastFromBase(data, dmy)->data ; }

// cast of thousands
// couldn't get template of the form template<T> operator T() to work w/o ambiguity.

#ifndef  _GCC_CAST_BUG
	dtl_variant_t::operator short int() const {
		return numeric_cast(short int());
	}
	dtl_variant_t::operator unsigned short int() const {
		return numeric_cast(unsigned short int());
	}
	dtl_variant_t::operator long int() const {
		return numeric_cast(long int());
	}
	dtl_variant_t::operator unsigned long int() const {
		return numeric_cast(unsigned long int());
	}
	dtl_variant_t::operator unsigned int () const {
		return numeric_cast(unsigned int());
	}

#else
    // gnu C++ 2.95 does not like the casts above
    // for some reason does not recognize e.g. short int as a
    // single typename.  Hacky workaround to fix.
	//
	// this multi-line syntax is also required for Borland 5

	dtl_variant_t::operator short int() const {
		integral_type<short int> s, out;
		out =   numeric_cast(s);
		return out.si;
	}

	dtl_variant_t::operator unsigned short int() const {
		integral_type<unsigned short int> s, out;
		out =   numeric_cast(s);
		return out.si;
	}

	dtl_variant_t::operator long int() const {
		integral_type<long int> s, out;
		out =   numeric_cast(s);
		return out.si;
	}

	dtl_variant_t::operator unsigned long int() const {
		integral_type<unsigned long int> s, out;
		out =   numeric_cast(s);
		return out.si;
	}

	dtl_variant_t::operator unsigned int() const {
		integral_type<unsigned int> s, out;
		out =   numeric_cast(s);
		return out.si;
	}

#endif

	dtl_variant_t::operator bool() const {
		return numeric_cast(bool());
	}

	dtl_variant_t::operator int () const {
		return numeric_cast(int());
	}

#ifndef DTL_NO_ODBCINT64
	dtl_variant_t::operator ODBCINT64 () const {
		ODBCINT64 tmp(0); // gcc 3.2 workaround "long long int()" fails below
		return numeric_cast(tmp);
	}
#endif
	dtl_variant_t::operator float () const {
		return numeric_cast(float());
	}
	dtl_variant_t::operator double () const {
		return numeric_cast(double());

	}

	// operator for strings ... cast of thousands changed to use DTL_TEMPLATE_FUNC as
	// Sun compiler needs template type specified to help it along

	STD_::string dtl_variant_t::get_string() const
    {
	  STD_::ostringstream tostr;

	  switch (typeId)
	  {
	  case C_SHORT:
		{
		  short *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, short)(data, dmy)->data;
		  break;
		}
	  case C_USHORT:
		{
		  unsigned short *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, unsigned short)(data, dmy)->data;

		  break;
		}
	  case C_BOOL:
		{
		  bool *dmy=NULL;
		  bool b = DTL_TEMPLATE_FUNC(CastFromBase, bool)(data, dmy)->data;
		  tostr << (b ? "true" : "false");
		  break;
		}
	  case C_INT:
		{
		  int *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, int)(data, dmy)->data;
		  break;
		}

	  case C_INT64:
		{
		  ODBCINT64 *dmy=NULL;
		  ODBCINT64 tmp = DTL_TEMPLATE_FUNC(CastFromBase, ODBCINT64)(data, dmy)->data;

// convert ODBCINT64 to a string
// on MSVC call specialized function.  for other compilers assume stream operator will do it.
#ifdef _MSC_VER
			char buffer[100];
			_i64toa(tmp,buffer,10);
			tostr << buffer;
#else
		  tostr << tmp;
#endif
		  break;
		}
	  case C_UINT:
		{
		  unsigned int *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, unsigned int)(data, dmy)->data;
		  break;
		}
	  case C_LONG:
		{
		  long *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, long)(data, dmy)->data;
		  break;
		}
	  case C_ULONG:
		{
		  unsigned long *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, unsigned long)(data, dmy)->data;
		  break;
		}
	  case C_DOUBLE:
		{
		  double *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, double)(data, dmy)->data;
		  break;
		}
	  case C_FLOAT:
		{
		  float *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, float)(data, dmy)->data;
		  break;
		}

	  case C_CHAR_STAR:
		{
		  char * *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, char *)(data, dmy)->data;
		  break;
		}

	  case C_STRING:
		{
		  STD_::string *dmy=NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, STD_::string)(data, dmy)->data;
		  break;
		}

#ifndef DTL_NO_UNICODE
	  case C_WSTRING:
		{
		  STD_::wstring *dmy=NULL;
		  tostr << tstring_cast(static_cast<STD_::string *>(NULL),
								DTL_TEMPLATE_FUNC(CastFromBase, STD_::wstring)(data, dmy)->data);
		  break;
		}
#endif

	   case C_BLOB:
		{
		  blob *dmy=NULL, *pblob;
		  pblob = &(DTL_TEMPLATE_FUNC(CastFromBase, blob)(data, dmy)->data);
		  STD_::string tostring;
		  tostring.reserve(pblob->size());
		  for (blob::const_iterator it = pblob->begin(); it != pblob->end(); ++it)
			  tostring += static_cast<char>(*it);
		  return tostring;
		  break;
		}

		// eventually add date library capabilities to convert tstring to time
	  case C_TIMESTAMP:
		{
		  TIMESTAMP_STRUCT *dmy = NULL;
		  tostr << DTL_TEMPLATE_FUNC(CastFromBase, TIMESTAMP_STRUCT)(data, dmy)->data; break;
		}

	  default:
		DTL_THROW VariantException
		  (_TEXT("dtl_variant_t::operator tstring()"),
		   tstring(_TEXT("this variant type cannot be cast to a tstring type")));

	  }

	  STD_::string tostring = tostr.str();

	  return tostring;
	}


    blob dtl_variant_t::get_blob() const
    {
	  switch (typeId)
	  {
		case C_BLOB: return DTL_TEMPLATE_FUNC(CastFromBase, blob)(data, static_cast<blob *>(NULL))->data; break;
		case C_STRING:
		{
			STD_::string *dmy=NULL, *pstring;
			pstring = &(DTL_TEMPLATE_FUNC(CastFromBase, STD_::string)(data, dmy)->data);
			blob toblob;
			toblob.reserve(pstring->size());
			for (STD_::string::const_iterator it = pstring->begin(); it != pstring->end(); ++it)
				toblob += static_cast<BYTE>(*it);
			return toblob;
			break;
		}
		default:
		DTL_THROW VariantException
		  (_TEXT("dtl_variant_t::get_blob()"),
		   tstring(_TEXT("this variant type cannot be cast to a blob type")));
	  }

	  blob toblob;
	  return toblob;

	}

#ifndef DTL_NO_UNICODE

	STD_::wstring dtl_variant_t::get_wstring() const {
		STD_::wostringstream tostr;

		switch (typeId) {

		// cast of thousands changed to use DTL_TEMPLATE_FUNC as
	    // Sun compiler needs template type specified to help it along
#if 0
		case C_SHORT: {short *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}
		case C_USHORT: {unsigned short *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}
		case C_BOOL: {bool *dmy=NULL; bool b = CastFromBase(data, dmy)->data;
			tostr << (b ? L"true" : L"false");
			break;}
		case C_INT: {int *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}
		case C_UINT: {unsigned int *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}
		case C_LONG: {long *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}
		case C_ULONG: {unsigned long *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}
		case C_DOUBLE: {double *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}
		case C_FLOAT: {float *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}
		case C_CHAR_STAR: {char * *dmy=NULL; tostr << CastFromBase(data, dmy)->data; break;}

		case C_STRING: {STD_::string *dmy=NULL; tostr <<
						   tstring_cast(static_cast<STD_::wstring *>(NULL), CastFromBase(data, dmy)->data); break;}

		case C_WSTRING: {STD_::wstring *dmy=NULL; tostr <<
							CastFromBase(data, dmy)->data;
			break;}

		// eventually add date library capabilities to convert tstring to time
		case C_TIMESTAMP: {TIMESTAMP_STRUCT *dmy = NULL; tostr << CastFromBase(data, dmy)->data; break; }
#endif
		case C_SHORT: {short *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, short)(data, dmy)->data; break;}
		case C_USHORT: {unsigned short *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, unsigned short)(data, dmy)->data; break;}
		case C_BOOL: {bool *dmy=NULL; bool b = DTL_TEMPLATE_FUNC(CastFromBase, bool)(data, dmy)->data;
			tostr << (b ? L"true" : L"false");
			break;}
		case C_INT: {int *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, int)(data, dmy)->data; break;}
		case C_UINT: {unsigned int *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, unsigned int)(data, dmy)->data; break;}
		case C_LONG: {long *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, long)(data, dmy)->data; break;}
		case C_ULONG: {unsigned long *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, unsigned long)(data, dmy)->data; break;}
		case C_INT64: {ODBCINT64 *dmy=NULL;
			ODBCINT64 tmp = DTL_TEMPLATE_FUNC(CastFromBase, ODBCINT64)(data, dmy)->data;
// convert ODBCINT64 to a string
// on MSVC call specialized function.  for other compilers assume stream operator will do it.
#ifdef _MSC_VER
			char buffer[100];
			_i64toa(tmp,buffer,10);
			tostr << buffer;
#else
			tostr <<  tmp;
#endif

			break;}
		case C_DOUBLE: {double *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, double)(data, dmy)->data; break;}
		case C_FLOAT: {float *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, float)(data, dmy)->data; break;}
		case C_CHAR_STAR: {char * *dmy=NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, char *)(data, dmy)->data; break;}

		case C_STRING: {STD_::string *dmy=NULL; tostr <<
						   tstring_cast(static_cast<STD_::wstring *>(NULL),
						   DTL_TEMPLATE_FUNC(CastFromBase, STD_::string)(data, dmy)->data); break;}

		case C_WSTRING: {STD_::wstring *dmy=NULL; tostr <<
						   DTL_TEMPLATE_FUNC(CastFromBase, STD_::wstring)(data, dmy)->data;
						   break;}

		// eventually add date library capabilities to convert tstring to time
		case C_TIMESTAMP: {TIMESTAMP_STRUCT *dmy = NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, TIMESTAMP_STRUCT)(data, dmy)->data; break; }

		case C_DATE: {DATE_STRUCT *dmy = NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, DATE_STRUCT)(data, dmy)->data; break; }

		case C_TIME: {TIME_STRUCT *dmy = NULL;
			tostr << DTL_TEMPLATE_FUNC(CastFromBase, TIME_STRUCT)(data, dmy)->data; break; }


		default:
			DTL_THROW VariantException(_TEXT("dtl_variant_t::operator tstring()"),
			  tstring(_TEXT("this variant type cannot be cast to a tstring type")));

		}

		STD_::wstring tostring = tostr.str();

		return tostring;
	}

#endif

	dtl_variant_t::operator STD_::string () const {
	 	  return get_string();
	};

#ifndef DTL_NO_UNICODE
	dtl_variant_t::operator STD_::wstring () const {
	 	  return get_wstring();
	};
#endif


	dtl_variant_t::operator char * () const {
		STD_::string s;
		s = get_string();
		char *c = new char[s.size()+1];
		if (c != NULL)
			strcpy(c, s.c_str());
		return c;
	};

	// cast of thousands changed to use DTL_TEMPLATE_FUNC as
	// Sun compiler needs template type specified to help it along
	dtl_variant_t::operator char() const
    {
	  switch (typeId)
      {
	  case C_SHORT:
		{
		  short *dmy=NULL;
		  // return char(CastFromBase<short>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, short)(data, dmy)->data);
		}
	  case C_USHORT:
		{
		  unsigned short *dmy=NULL;
		  // return char(CastFromBase<unsigned short>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, unsigned short)(data, dmy)->data);
		}
	  case C_BOOL:
		{
		  bool *dmy=NULL;
		  // return char(CastFromBase<bool>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, bool)(data, dmy)->data);
		}
	  case C_INT:
		{
		  int *dmy=NULL;
		  // return char(CastFromBase<int>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, int)(data, dmy)->data);
		}
	  case C_UINT:
		{
		  unsigned int *dmy=NULL;
		  // return char(CastFromBase<unsigned int>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, unsigned int)(data, dmy)->data);
		}
	  case C_LONG:
		{
		  long *dmy=NULL;
		  // return char(CastFromBase<long>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, long)(data, dmy)->data);
		}
	  case C_ULONG:
		{
		  unsigned long *dmy=NULL;
		  // return char(CastFromBase<unsigned long>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, unsigned long)(data, dmy)->data);
		}
	  case C_DOUBLE:
		{
		  double *dmy=NULL;
		  // return char(CastFromBase<double>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, double)(data, dmy)->data);
		}
	  case C_FLOAT:
		{
		  float *dmy=NULL;
		  // return char(CastFromBase<float>(data, dmy)->data);
		  return char(DTL_TEMPLATE_FUNC(CastFromBase, float)(data, dmy)->data);
		}
	  case C_CHAR_STAR:
		{
		  char * *dmy=NULL;
		  // return char(*(CastFromBase<char*>(data, dmy)->data));
		  return char(*(DTL_TEMPLATE_FUNC(CastFromBase, char *)(data, dmy)->data));
		}
	  case C_STRING:
		{
		  STD_::string *dmy=NULL;
		  // return char(*(CastFromBase<STD_::string>(data, dmy)->data.c_str()));
		  return char(*(DTL_TEMPLATE_FUNC(CastFromBase, STD_::string)(data, dmy)->data.c_str()));
		}
	  case C_BLOB:
		{
		  // Try .data() instead of .c_str() since blobs
		  // aren't strings and aren't necessary
		  // NULL-terminated, and it unexpectedly fixes a
		  // complaint by GCC 2.96
		  blob *dmy=NULL;
		  // return char(*(CastFromBase<blob>(data, dmy)->data.c_str()));
		  return char (*(DTL_TEMPLATE_FUNC(CastFromBase, blob)(data, dmy)->data.data()));
		}
#ifndef DTL_NO_UNICODE
	  case C_WSTRING:
		{
		  STD_::wstring *dmy=NULL;
		  // return wctob(*(CastFromBase(data, dmy)->data.c_str()));
		  return wctob(*(DTL_TEMPLATE_FUNC(CastFromBase, STD_::wstring)(data, dmy)->data.c_str()));
		}
#endif
		// eventually may add class capabilities to timestamp to pull out
		// jdate here
		// case C_TIMESTAMP:
	  default:
		DTL_THROW VariantException
		  (_TEXT("dtl_variant_t::operator char()"),
		   tstring(_TEXT("this variant type cannot be cast to a char type")));

		return char();
	  }

	}

	dtl_variant_t::operator struct tagTIMESTAMP_STRUCT () const
    {

	  switch (typeId)
      {

	  case C_TIMESTAMP:
		  {
			  struct tagTIMESTAMP_STRUCT *dmy=NULL;
			  return DTL_TEMPLATE_FUNC(CastFromBase, tagTIMESTAMP_STRUCT)(data, dmy)->data;
		  }
	  case C_CHAR_STAR:
		{
		  char * *dmy=NULL;
		  return (timestamp_t)jtime_c(tstring_cast(static_cast<tstring *>(NULL),
			   STD_::string(DTL_TEMPLATE_FUNC(CastFromBase, char *)(data, dmy)->data)));
		}
	  case C_STRING:
		{
		  STD_::string *dmy=NULL;
		  return (timestamp_t)jtime_c(tstring_cast(static_cast<tstring *>(NULL),
			  (DTL_TEMPLATE_FUNC(CastFromBase, STD_::string)(data, dmy)->data)));
		}

#ifndef DTL_NO_UNICODE

	  case C_WSTRING:
		{
		  STD_::wstring *dmy=NULL, temp;
		  temp = DTL_TEMPLATE_FUNC(CastFromBase, STD_::wstring)(data, dmy)->data;
		  return (timestamp_t)jtime_c(tstring_cast(static_cast<tstring *>(NULL), temp));
		}
#endif
		// eventually may add class capabilities to timestamp to pull out
		// jdate here
		// case C_TIMESTAMP:
	  default:
		DTL_THROW VariantException
		  (_TEXT("dtl_variant_t::operator tagTIMESTAMP_STRUCT()"),
		   tstring(_TEXT("this variant type cannot be cast to a timestamp struct type")));

		return tagTIMESTAMP_STRUCT();
	  }

	}

    // This method returns type enumeration value
    char dtl_variant_t::type() const {
	  return typeId;
	}

	// Map the class to our typeid enumeration
	TypeTranslation dtl_variant_t::type_translation() {
		ETI_Map &SQL_types_to_C = GetSQL_types_to_C();
		ETI_Map::iterator i = SQL_types_to_C.begin();
		for(; i != SQL_types_to_C.end(); i++)
			if (i->second.typeId == typeId)
				break;
		if (i != SQL_types_to_C.end())
		{
		  DTL_THROW VariantException(_TEXT("dtl_variant_t::type()"),
			 tstring(_TEXT("Internal error.  Invalid type encountered in typeId member.")));
		}

		return i->second;
	}

	// powerful stream operator for dtl_variant_t's!!!!
	STD_::ostream &operator<<(STD_::ostream &o, const dtl_variant_t &v)
	{
		o << v.get_string();
		return o;
	}

#ifndef DTL_NO_UNICODE

	STD_::wostream &operator<<(STD_::wostream &o, const dtl_variant_t &v)
	{
		o << tstring_cast(static_cast<STD_::wstring *>(NULL),v.get_string());
		return o;
	}
#endif

// ************* implementation code for variant_row_fields **********

	variant_row_fields::variant_row_fields() : types(), names(), offsets(), row_size(0) { }

	// Construct form fields & names, but compute offsets from scratch
	variant_row_fields::variant_row_fields(STD_::vector<TypeTranslation> &f,	STD_::vector<tstring> &n) : types(f), names(n){
		STD_::vector<TypeTranslation>::iterator i = types.begin();
		row_size = 0;
		for (; i != types.end(); i++) {
			offsets.push_back(row_size);
			row_size += i->size;
		}
	}

	void variant_row_fields::push_back(TypeTranslation &tt, tstring name) {
		offsets.push_back(row_size);
		row_size += tt.size;
		names.push_back(name);
		types.push_back(tt);
	}

	// exception-safe swap()
	void variant_row_fields::swap(variant_row_fields &other)
	{
		STD_::swap(row_size, other.row_size);
		types.swap(other.types);
		names.swap(other.names);
		offsets.swap(other.offsets);
	}

	// exception-safe assignment
	variant_row_fields &variant_row_fields::operator=(const variant_row_fields &other)
	{
		if (this != &other)
		{
			variant_row_fields temp(other);
			swap(temp);
		}

		return *this;

	}

	// accessors
	STD_::vector<TypeTranslation> variant_row_fields::GetTypes()   const   { return types;		  }
	STD_::vector<tstring>	  variant_row_fields::GetNames()   const   { return names;		  }
	STD_::vector<std::ptrdiff_t>		  variant_row_fields::GetOffsets() const   { return offsets;	  }

	size_t variant_row_fields::size() const { return names.size(); }

    // comparison operators
	bool operator==(const variant_row_fields &vf1,
						   const variant_row_fields &vf2)
	{
	   return vf1.types == vf2.types && vf1.names == vf2.names &&
			  vf1.offsets == vf2.offsets && vf1.row_size == vf2.row_size;
	}

	bool operator!=(const variant_row_fields &vf1,
						   const variant_row_fields &vf2)
    {
	   return !(vf1 == vf2);
	}

// *********** implementation code for variant_field ***************
	variant_field::variant_field() : m_val(), p_row(NULL), m_col(0), m_IsNull(false)  {}

	variant_field::variant_field(const variant_field &other) : m_val(other.m_val), p_row(other.p_row),
		m_col(other.m_col), m_IsNull(other.m_IsNull) {}

variant_field::variant_field(const dtl_variant_t &v, variant_row *row, int field, const bool b) : m_val(v),
		p_row(row), m_col(field), m_IsNull(b) {	}

	// exception-safe swap()
	void variant_field::swap(variant_field &other)
	{
		m_val.swap(other.m_val);
		STD_::swap(p_row, other.p_row);
		STD_::swap(m_col, other.m_col);
		STD_::swap(m_IsNull, other.m_IsNull);
	}

#if 1
	// Special assignment operator - cascade changes back to parent row
	variant_field &variant_field::operator=(const variant_field &other)
	{
		if (reinterpret_cast<const BYTE*>(this) != reinterpret_cast<const BYTE*>(&other)) {
			m_val = other.m_val;
			p_row->replace(*this);
			if (other.m_IsNull)
				SetNull();
			else
				ClearNull();
		}

		return *this;
	}
#else
	variant_field &variant_field::operator=(const variant_field &other)
	{
		variant_field tmp(other);
		swap(tmp);
		return *this;
	}

#endif

	// convenience operator to allow the setting of a variant_field to null
	/*PS const*/ variant_field &variant_field::operator=(const NullField & DTL_DUMMY_PARAM_1)
	{
		if (!IsNull())
			SetNull();
		return *this;
	}

    // cast of thousands again!!!!
	// couldn't get template of the form template<T> operator T() to work w/o ambiguity.
	variant_field::operator short () const {
		return m_val.operator short();
	}
	variant_field::operator unsigned short () const {
		return (unsigned short) m_val;
	}
	variant_field::operator long () const {
		return (long) m_val;
	}
	variant_field::operator unsigned long () const {
		return (unsigned long) m_val;
	}
	variant_field::operator unsigned int () const {
		return (unsigned int) m_val;
	}
	variant_field::operator int () const {
		return (int) m_val;
	}
	variant_field::operator float () const {
		return (float) m_val;
	}
	variant_field::operator double () const {
		return (double) m_val;
	}

	variant_field::operator bool() const {
		return (bool) m_val;
	}

	variant_field::operator STD_::string() const {
		return m_val.get_string();
	}

#ifndef DTL_NO_UNICODE

	variant_field::operator STD_::wstring() const {
		return m_val.get_wstring();
	}
#endif

	variant_field::operator char() const
	{
		return (char) m_val;

	}

	variant_field::operator char*() const
	{
		return static_cast<char *>(m_val);
	}

	variant_field::operator struct tagTIMESTAMP_STRUCT() const
	{
		return (struct tagTIMESTAMP_STRUCT) m_val;
	}

	variant_field::operator dtl_variant_t() const
	{
		return m_val;
	}

	// end of cast of thousands

	bool variant_field::IsNull() const {return m_IsNull;}

	// so we can test for null using syntax: var_row[fieldName] == NullField()
	bool variant_field::operator==(const NullField & DTL_DUMMY_PARAM_1) const
	{
		return IsNull();
	}

	bool variant_field::operator!=(const NullField &null) const
	{
		return !(*this == null);
	}

	// allows NullField() == var_row[fieldName]
	bool operator==(const NullField &null, const variant_field &vf)
	{
		return vf == null;
	}

	bool operator!=(const NullField &null, const variant_field &vf)
	{
		return !(null == vf);
	}

	void variant_field::SetNull() {m_IsNull = true; variant_row_SetNull(p_row,m_col);}

	void variant_field::ClearNull() {m_IsNull = false; variant_row_ClearNull(p_row,m_col);}

	dtl_variant_t variant_val(const variant_field &vf)
	{
		return vf.m_val;
	}

	// needed to resolve ambiguity for casts
	STD_::string variant_field::get_string() const
	{
		return m_val.get_string();
	}

	// needed to resolve ambiguity for casts
	blob variant_field::get_blob() const
	{
		return m_val.get_blob();
	}

#ifndef DTL_NO_UNICODE
// needed to resolve ambiguity for casts
	STD_::wstring variant_field::get_wstring() const
	{
		return m_val.get_wstring();
	}
#endif

// needed to resolve ambiguity for casts
	tstring variant_field::get_tstring() const
	{
		return m_val.get_tstring();
	}
	// returns an enumeration listing the type of the variant data
	char variant_field::type() const {
		return m_val.type();
	}

	// stream operator for variant_field's
	STD_::ostream &operator<<(STD_::ostream &o, const variant_field &vf)
	{
		if (!vf.IsNull())
		{
		  dtl_variant_t  temp(vf);
		  o << temp;
		}
		else
		{
		  o << STD_::string("<NULL>");
		}
		return o;
	}

#ifndef DTL_NO_UNICODE

	// stream operator for variant_field's
	STD_::wostream &operator<<(STD_::wostream &o, const variant_field &vf)
	{
		if (!vf.IsNull())
		{
		  dtl_variant_t  temp(vf);
		  o << temp;
		}
		else
		{
		  o << STD_::wstring(L"<NULL>");
		}
		return o;
	}
#endif

// ************ Implementation code for variant_row ************
	void variant_row::init_field(BYTE* v, int typeId) {
		// construct individual fields that are not primitives
		switch (typeId) {
			case C_STRING:
				new(v) STD_::string("");
				break;
			case C_BLOB:
				new(v) blob();
				break;

#if !defined(DTL_NO_UNICODE)
			case C_WSTRING:
				new(v) STD_::wstring(L"");
				break;
#endif
		}

	}

	void variant_row::copy_construct_field(BYTE* dest, const BYTE* src, int typeId) {
		// copy construct individual fields that are not primitives
		// N.B.!! objects to construct here should not make use of their
		// absolute address as part of construction, otherwise variant_row
		// assignment operator will not work
		switch (typeId) {
		case C_STRING: {
				const STD_::string *pstring = reinterpret_cast<const STD_::string *>(src);
				new(dest) STD_::string(*pstring);
				break;
				}

		case C_BLOB: {
				const blob *pstring = reinterpret_cast<const blob *>(src);
				new(dest) blob(*pstring);
				break;
				}
#ifndef DTL_NO_UNICODE
		case C_WSTRING:{
				const STD_::wstring *pwstring = reinterpret_cast<const STD_::wstring *>(src);
				new(dest) STD_::wstring(*pwstring);
				break;
				}
#endif
		}


	}

	void variant_row::destroy_field(BYTE* v, int typeId) {
		// destruct individual fields that are not primitives
		try {
			switch (typeId) {
			case C_STRING: {
			  using STD_::string;
			  STD_::string *pstring = reinterpret_cast<STD_::string *>(v);
#ifdef __hpux // LEAK!  XXX
			  //delete pstring;
#else
			  pstring->~string();
#endif
			  break;
			}

			case C_BLOB: {
			  blob *pstring = reinterpret_cast<blob *>(v);
#ifdef __hpux // LEAK!  XXX
			  //delete pstring;
#else
			  pstring->~blob();
#endif
			  break;
			}
#ifndef DTL_NO_UNICODE
			case C_WSTRING: {
			  using STD_::wstring;
			  STD_::wstring *pwstring = reinterpret_cast<STD_::wstring *>(v);
#ifdef __hpux // LEAK!  XXX
			  //delete pwstring;
#else
			  pwstring->~wstring();
#endif
			  break;
			}
#endif
			}
		}
		catch(...) {}
	}

	void variant_row::init_fields(void) {
		// construct individual fields that are not primitives
		if (p_data == NULL)
			return;
		STD_::vector<TypeTranslation>::iterator i, b;
		b = p_row_fields->types.begin();
		for (i = b; i != p_row_fields->types.end(); i++) {
			init_field(p_data + p_row_fields->offsets[i-b], i->typeId);
		}
	}

	void variant_row::destroy_fields(void) {
		// construct individual fields that are not primitives
		if (p_data == NULL)
			return;
		STD_::vector<TypeTranslation>::iterator i, b;
		b = p_row_fields->types.begin();
		for (i = b; i != p_row_fields->types.end(); i++) {
			destroy_field(p_data + p_row_fields->offsets[i-b], i->typeId);
		}
	}


	void variant_row::destroy(void) {
		if (p_data) {
			if (p_row_fields)
				destroy_fields();
			if (!b_do_not_delete)
				delete[] p_data;
			p_data = NULL;
		}
	}


	void variant_row::copy_construct_fields(const variant_row &other) {
		// copy construct individual fields that are not primitives
		if (p_data == NULL || other.p_data == NULL)
			return;
		STD_::vector<TypeTranslation>::const_iterator i, b;
		b = other.p_row_fields->types.begin();
		for (i = b; i != other.p_row_fields->types.end(); i++) {
				copy_construct_field(p_data + other.p_row_fields->offsets[i-b], other.p_data + other.p_row_fields->offsets[i-b],
				i->typeId);
		}
	}

	// replace field value
	void variant_row::replace(const variant_field &field) {
		BYTE* p = p_data + p_row_fields->offsets[field.m_col];
		char typeId = p_row_fields->types[field.m_col].typeId;
		switch (typeId) {
	  // again g++ does not like these casts
		case C_SHORT: *(reinterpret_cast<short *>(p)) = static_cast<short>(field.m_val); break;
		case C_USHORT: *(reinterpret_cast<unsigned short *>(p)) = static_cast<unsigned short>(field.m_val); break;
		case C_BOOL: *(reinterpret_cast<bool *>(p)) = static_cast<bool>(field.m_val); break;
		case C_UINT: *(reinterpret_cast<unsigned int *>(p)) = static_cast<unsigned int>(field.m_val); break;
		case C_ULONG: *(reinterpret_cast<unsigned long *>(p)) = static_cast<unsigned long>(field.m_val); break;
		case C_LONG: *(reinterpret_cast<long *>(p)) = static_cast<long>(field.m_val); break;
		case C_INT: *(reinterpret_cast<int *>(p)) = static_cast<int>(field.m_val); break;
		case C_INT64: *(reinterpret_cast<ODBCINT64 *>(p)) = static_cast<ODBCINT64>(field.m_val); break;
		case C_DOUBLE: *(reinterpret_cast<double *>(p)) = static_cast<double>(field.m_val); break;
		case C_FLOAT: *(reinterpret_cast<float *>(p)) = static_cast<float>(field.m_val); break;
		case C_TIMESTAMP: *(reinterpret_cast<struct tagTIMESTAMP_STRUCT *>(p)) = static_cast<struct tagTIMESTAMP_STRUCT>(field.m_val); break;
		case C_CHAR_STAR: std_strncpy(reinterpret_cast<char *>(p), static_cast<const char *>(field.m_val), p_row_fields->types[field.m_col].size); break;

	  // need to use char* for the case below to work around g++ error
		case C_STRING: *(reinterpret_cast<STD_::string *>(p)) = field.m_val.get_string(); break;
		case C_BLOB: *(reinterpret_cast<blob *>(p)) = field.m_val.get_blob(); break;
#ifndef DTL_NO_UNICODE
		case C_WSTRING: *(reinterpret_cast<STD_::wstring *>(p)) = field.m_val.get_wstring(); break;
#endif
		default:
			 DTL_THROW VariantException(_TEXT("variant_row::replace()"),
			   tstring(_TEXT("Invalid variant data assigned to row.")));
		}
	}

	STD_::vector<TypeTranslation>::const_iterator
		variant_row::find_field(const tstring &f) const {
		STD_::vector<tstring>::const_iterator i;
		STD_::vector<TypeTranslation>::const_iterator j;
		j = p_row_fields->types.begin();
		for(i = p_row_fields->names.begin(); i !=  p_row_fields->names.end(); i++, j++)
			if (std_tstricmp(i->c_str(), f.c_str())==0)
				return j;
		return j; // return end() if not found
	}

	void variant_row::init(void) {
		if (p_data == NULL) {
			// p_data = (BYTE *)malloc(p_row_fields->row_size);
			p_data = new BYTE[p_row_fields->row_size];

			if (p_data != NULL) {
				std_memset(p_data, 0, p_row_fields->row_size);
				init_fields();
			}
		}

		// set index over existing vr fields
		tt_i = 0;
	}

	bool variant_row::IsInit() const {
		return p_data != NULL;
	}

	variant_row::variant_row() : b_IsNull(true), p_row_fields(NULL),
		p_data(NULL), tt_i(0), b_do_not_delete(false) {	}

	variant_row::variant_row(STD_::vector<TypeTranslation> &types, STD_::vector<tstring> &names) :
	  b_IsNull(true),
	  p_row_fields(new variant_row_fields(types, names)), p_data(NULL), tt_i(0),
	  b_do_not_delete(false){
		init();
	}

    variant_row::variant_row(const variant_row &other) : b_IsNull(other.b_IsNull), p_row_fields(NULL),
		p_data(NULL), tt_i(0), b_do_not_delete(false)
		{
		if (other.p_row_fields) {
			// p_data = (BYTE *)malloc(other.p_row_fields->row_size * sizeof(BYTE));
			p_data = new BYTE[other.p_row_fields->row_size];

			if (p_data != NULL) {
				memcpy(p_data, other.p_data, other.p_row_fields->row_size);
				copy_construct_fields(other);
			}
			p_row_fields = other.p_row_fields;
			tt_i = other.tt_i;
		} else {
			p_row_fields = NULL;
			p_data = NULL;
			tt_i = 0;
		}
	}

	BYTE *variant_row::data_ptr() const{
		return p_data;
	}

	variant_row::variant_row(const variant_row &other, BYTE *ptr) : b_IsNull(other.b_IsNull),
		p_row_fields(other.p_row_fields),
		p_data(ptr), tt_i(other.tt_i), b_do_not_delete(true)
	{
			std_memset(p_data, 0, p_row_fields->row_size);
			init_fields();
	}

	// exception-safe swap()
	void variant_row::swap(variant_row &other)
	{
		p_row_fields.swap(other.p_row_fields);
		STD_::swap(p_data, other.p_data);
		STD_::swap(b_IsNull, other.b_IsNull);
		STD_::swap(tt_i, other.tt_i);
		STD_::swap(b_do_not_delete, other.b_do_not_delete);
	}

	// Slightly complicated assignment operator for two reasons
	// 1. We wanted it to be exception safe
	// 2. If the variant_rows have the same variant_row_fields we need to copy the contents
	// and not change the address so that DynamicDBView can still function
	// with variant_row parameters
	//
    const variant_row & variant_row::operator=(const variant_row &other) {
       if (this != &other) {
		  if (other.p_data != NULL && other.p_row_fields) {
				// BYTE *new_data = (BYTE *)malloc(other.p_row_fields->row_size * sizeof(BYTE));
				BYTE *new_data = new BYTE[other.p_row_fields->row_size];

				if (new_data == NULL)
					DTL_THROW VariantException(_TEXT("variant_row::operator ="), tstring(_TEXT("Out of memory for assigning row.")));
				BYTE *p_data_backup = p_data;
				p_data = new_data;
				try {
					 memcpy(p_data, other.p_data, other.p_row_fields->row_size);
					 copy_construct_fields(other);
				}
				catch(...) {
					p_data = p_data_backup; // rollback changes
					// free(new_data);
					delete[] new_data;
					throw;
				}
				p_data = p_data_backup;
			    // this logic is needed to properly ensure that if the
			    // variant_row_fields are identical, then we leave the old pointers intact
			    // code is more complex as we must properly handle NULL pointers
                bool bCopyData = false;

			    if (p_row_fields.get() == other.p_row_fields.get())
				   bCopyData = true;
			    else // pointers are different but contents may be the same
				  if (p_row_fields  && other.p_row_fields)
						if (*p_row_fields == *other.p_row_fields)
						     bCopyData = true;

				if (bCopyData && p_data != NULL) {
					destroy_fields();
					memcpy(p_data, new_data, other.p_row_fields->row_size);
					// free(new_data);
					delete[] new_data;
				} else {
					destroy();
					p_data = new_data;
				}
		  }
		  p_row_fields = other.p_row_fields;
          b_IsNull = other.b_IsNull;
		  tt_i = other.tt_i;
	   }
       return *this;
	}

	 void variant_row::replace(const variant_row &other) {
       if (this != &other) {
		    bool bSameFields = false;

			if (p_row_fields.get() == other.p_row_fields.get())
			   bSameFields = true;
			else // pointers are different but contents may be the same
			  if (p_row_fields  && other.p_row_fields)
					if (*p_row_fields == *other.p_row_fields)
						 bSameFields = true;

			if (bSameFields || p_data == NULL) {
				*this = other;
				return;
			}

		   // copy field by field, converting as necessary
		   for (size_t i = 0; i < size(); ++i)
		   {
			   const variant_field &from = other[(*p_row_fields).names[i]];
			   // (*this)[i] = other[(*p_row_fields).names[i]];
			   (*this)[ static_cast<int>(i) ] = from;
		   }
	   }
	}


	// return row field by name
	variant_field variant_row::operator[](const tstring &f) {
		STD_::vector<TypeTranslation>::const_iterator i = find_field(f);
		if (p_data == NULL) {
			DTL_THROW VariantException(_TEXT("variant_row::operator[]()"),
			tstring(_TEXT("Invalid field name requested from NULL variant_field class.")));
		}

		if (i != p_row_fields->types.end()) {
			std::ptrdiff_t diff = 0;
			size_t mem_offset = 0;
			BYTE* p = NULL;
			const TypeTranslation *pt = NULL;

			diff = i - p_row_fields->types.begin();

			mem_offset =  p_row_fields->offsets[diff];

			p = p_data + mem_offset;

			pt = &(*i);
			bool isNull = b_IsNull[diff];
			dtl_variant_t v(p, *pt);
			return variant_field(v, this, static_cast< int >( diff ), isNull);

		}
		// throw UNKNOWN field name!
		DTL_THROW VariantException(_TEXT("variant_row::operator[]()"),
			tstring(_TEXT("Invalid field name requested from row class.")));
		return variant_field(dtl_variant_t(0), NULL, 0, false);

	}

	// return row field by number
	variant_field variant_row::operator[](int i) {
		// STL should throw here if field # out of bounds
		if (p_data == NULL) {
			DTL_THROW VariantException(_TEXT("variant_row::operator[]()"),
			tstring(_TEXT("Invalid field name requested from NULL variant_field class.")));
		}
		BYTE* p = p_data + p_row_fields->offsets[i];
		bool isNull = b_IsNull[i];
		return variant_field(dtl_variant_t(p, p_row_fields->types[i]), this, i, isNull);
	}

	// const versions of subscript operators
	// return row field by name
	const variant_field variant_row::operator[](const tstring &f) const {
		STD_::vector<TypeTranslation>::const_iterator i = find_field(f);

		if (p_data == NULL) {
			DTL_THROW VariantException(_TEXT("variant_row::operator[]()"),
			tstring(_TEXT("Invalid field name requested from NULL variant_field class.")));
		}

		if (i != p_row_fields->types.end()) {

			std::ptrdiff_t diff = 0;
			size_t mem_offset = 0;
			BYTE* p = NULL;
			const TypeTranslation *pt = NULL;

			diff = i - p_row_fields->types.begin();

			mem_offset =  p_row_fields->offsets[diff];

			p = p_data + mem_offset;

			pt = &(*i);
			bool isNull = b_IsNull.test(diff);
			dtl_variant_t v(p, *pt);
			return variant_field(	v,
									const_cast<variant_row *>(this),
									static_cast<int>( diff ),
									isNull );

		}
		// throw UNKNOWN field name!
		DTL_THROW VariantException(_TEXT("variant_row::operator[]()"),
			tstring(_TEXT("Invalid field name requested from row class.")));
		return variant_field(dtl_variant_t(0), NULL, 0, false);

	}

	// return row field by number
	const variant_field variant_row::operator[](int i) const {
		// STL should throw here if field # out of bounds
		if (p_data == NULL) {
			DTL_THROW VariantException(_TEXT("variant_row::operator[]()"),
			tstring(_TEXT("Invalid field name requested from NULL variant_field class.")));
		}
		BYTE* p = p_data + p_row_fields->offsets[i];
		bool isNull = b_IsNull.test(i);
		return variant_field(dtl_variant_t(p, p_row_fields->types[i]), const_cast<variant_row *>(this), i, isNull);
	}

	// return # of columns in row
	size_t variant_row::size() const
	{
		return p_row_fields->size();
	}

	size_t variant_row::row_size() const
	{
		return p_row_fields->row_size;
	}


	// return column names for the row
	STD_::vector<tstring> variant_row::GetNames() const

	{
		return p_row_fields->GetNames();
	}

	// return type information for the columns in the row
	STD_::vector<TypeTranslation> variant_row::GetTypes() const
	{
		return p_row_fields->GetTypes();
	}

	const variant_row_fields &variant_row::GetVariantRowFields() const
	{
		return *p_row_fields;
	}

	// used for debugging to print out contents of variant row as a tstring
	STD_::string variant_row::Stringify() const
	{
	   STD_::ostringstream result;

	   for (size_t i = 0; i < size(); i++)
	   {
#if !defined(DTL_OUTPUT_ROWIDS)
		  if (p_row_fields->names[i].c_str() == tstring(_TEXT("ROWID")))
		  {
			  result << "#### ROWID #### ";
			  break;
		  }
#endif
		  result <<  (*this)[static_cast<int>(i)] << ' ';
	   }
	   result << STD_::ends;

	   STD_::string resultstr = result.str();

	   // result.freeze(false);

	   return resultstr;
	}

#ifndef DTL_NO_UNICODE
	// used for debugging to print out contents of variant row as a tstring
	STD_::wstring variant_row::wStringify() const
	{
	   STD_::wostringstream result;

	   for (size_t i = 0; i < size(); i++)
	   {
#if !defined(DTL_OUTPUT_ROWIDS)
		  if (p_row_fields->names[i].c_str() == tstring(_TEXT("ROWID")))
		  {
			  result << L"#### ROWID #### ";
			  break;
		  }
#endif
		  result << (*this)[static_cast<int>(i)] << _TEXT(" ");
	   }
	   result << STD_::ends;

	   STD_::wstring resultstr = result.str();

	   // result.freeze(false);

	   return resultstr;
	}
#endif

	bool operator==(const variant_row &vr1, const variant_row &vr2)
	{
		CountedPtr<variant_row_fields> pRowFields1 = vr1.p_row_fields;
		CountedPtr<variant_row_fields> pRowFields2 = vr2.p_row_fields;

		// make sure variant_row_fields are the same between the two variant_row's
		if (pRowFields1.get() == NULL || pRowFields2.get() == NULL)
			DTL_THROW VariantException(_TEXT("variant_row::operator==()"),
				_TEXT("NULL variant row fields ptr.!"));

		if (*pRowFields1 != *pRowFields2)
			DTL_THROW VariantException(_TEXT("variant_row::operator==()"),
				_TEXT("Types do not match for two variant_row's being compared!"));

		// need offsets so we can get the raw pointer into the variant_row
		// to get the field data out

		STD_::vector<std::ptrdiff_t> offsets = pRowFields1->GetOffsets();

		// now compare the two variant_rows, field by field
		for (size_t i = 0; i < vr1.size(); i++)
		{
			const variant_field &vf1 =  vr1[ static_cast<int>(i) ] ;
			const variant_field &vf2 = vr2[ static_cast<int>(i) ];

			size_t offset = offsets[i];

			// handle nulls
			if (vf1 == NullField() && vf2 == NullField())
				return true; // NULL == NULL
			else if (vf1 == NullField() && vf2 != NullField())
				return false; // NULL != non-NULL
			else if (vf1 != NullField() && vf2 == NullField())
				return false; // non-NULL != NULL

			// both are non-NULL ... perform standard equality check

			// grab raw pointers
			BYTE* pField1 = vr1.data_ptr() + offset;
			BYTE* pField2 = vr2.data_ptr() + offset;

			// equality test performed using GenericCmp()
			if (GenericCmp(pField1, pField2, vf1.type()) != 0)
				return false;
		}

		// all fields equal, return true
		return true;
	}

	bool operator<(const variant_row &vr1, const variant_row &vr2)
	{
		CountedPtr<variant_row_fields> pRowFields1 = vr1.p_row_fields;
		CountedPtr<variant_row_fields> pRowFields2 = vr2.p_row_fields;

		// make sure variant_row_fields are the same between the two variant_row's
		if (pRowFields1.get() == NULL || pRowFields2.get() == NULL)
			DTL_THROW VariantException(_TEXT("variant_row::operator<()"),
				_TEXT("NULL variant row fields ptr.!"));

		if (*pRowFields1 != *pRowFields2)
			DTL_THROW VariantException(_TEXT("variant_row::operator<()"),
				_TEXT("Types do not match for two variant_row's being compared!"));


		// need offsets so we can get the raw pointer into the variant_row
		// to get the field data out

		STD_::vector<std::ptrdiff_t> offsets = pRowFields1->GetOffsets();

		// now compare the two variant_rows, field by field
		for (size_t i = 0; i < vr1.size(); i++)
		{
			const variant_field &vf1 = vr1[static_cast<int>(i)];
			const variant_field &vf2 = vr2[static_cast<int>(i)];

			size_t offset = offsets[i];

			// handle nulls
			if (vf1 == NullField() && vf2 == NullField())
				return false; // !(NULL < NULL)
			else if (vf1 == NullField() && vf2 != NullField())
				return true; // NULL < non-NULL
			else if (vf1 != NullField() && vf2 == NullField())
				return false; // !(non-NULL < NULL)

			// both are non-NULL ... perform standard < check

			// grab raw pointers
			BYTE* pField1 = vr1.data_ptr() + offset;
			BYTE* pField2 = vr2.data_ptr() + offset;

			// equality test performed using GenericCmp()
			int cmp_result = GenericCmp(pField1, pField2, vf1.type());

			if (cmp_result < 0)
				return true;
			else if (cmp_result > 0)
				return false;
		}

		// all fields equal, return false
		return false;
	}

	bool operator!=(const variant_row &vr1, const variant_row &vr2)
	{
		return !(vr1 == vr2);
	}

	bool operator<=(const variant_row &vr1, const variant_row &vr2)
	{
		return (vr1 < vr2) && (vr1 == vr2);
	}

	bool operator>=(const variant_row &vr1, const variant_row &vr2)
	{
		return !(vr1 < vr2);
	}

	bool operator>(const variant_row &vr1, const variant_row &vr2)
	{
		return !((vr1 < vr2) && (vr1 == vr2));
	}

	STD_::ostream &operator<<(STD_::ostream &o, const variant_row &vr)
	{
      o << vr.Stringify();
	  return o;
	}

#ifndef DTL_NO_UNICODE
	STD_::wostream &operator<<(STD_::wostream &o, const variant_row &vr)
	{
      o << vr.wStringify();
	  return o;
	}
#endif

	// clear all null field flags
	void variant_row::ClearNulls() {
		b_IsNull.reset();
	}


	// returns whether field is a NULL value
	bool variant_row::IsNull(const tstring &f) const {
		STD_::vector<TypeTranslation>::const_iterator i = find_field(f);
		size_t n = i - p_row_fields->types.begin();
		return b_IsNull.test(n);
	}

	bool variant_row::IsNull(int i) const
	{
		return b_IsNull.test(i);
	}

	// mark the field with the given name as holding a NULL value
	void variant_row::SetNull(const tstring &f) {
		STD_::vector<TypeTranslation>::const_iterator i = find_field(f);
		size_t n = i - p_row_fields->types.begin();
		b_IsNull.set(n);
	}

	void variant_row::SetNull(int i) {
		b_IsNull.set(i);
	}

	// mark the field with the given name as holding a NULL value
	void variant_row::ClearNull(const tstring &f) {
		STD_::vector<TypeTranslation>::const_iterator i = find_field(f);
		size_t n = i - p_row_fields->types.begin();
		b_IsNull.reset(n);
	}

	void variant_row::ClearNull(int i) {
		b_IsNull.reset(i);
	}

	BYTE *data_ptr(const variant_row *vr)
	{
		return vr->data_ptr();

	}

	variant_row::~variant_row()
	{
		destroy();
	}

	TypeTranslationField variant_row::_short() {
		return add_field((short *)NULL);
	}

	TypeTranslationField variant_row::_unsigned_short() {
		return add_field((unsigned short *)NULL);
	}

	TypeTranslationField variant_row::_int() {
		return add_field((int *)NULL);
	}

	TypeTranslationField variant_row::_unsigned_int() {
		return add_field((unsigned int *)NULL);
	}

	TypeTranslationField variant_row::_long() {
		return add_field((long *)NULL);
	}

	TypeTranslationField variant_row::_unsigned_long() {
		return add_field((unsigned long *)NULL);
	}

	TypeTranslationField variant_row::_double() {
		return add_field((double *)NULL);
	}

	TypeTranslationField variant_row::_float() {
		return add_field((float *)NULL);
	}

	TypeTranslationField variant_row::_timestamp() {
		return add_field((struct tagTIMESTAMP_STRUCT *)NULL);
	}

	TypeTranslationField variant_row::_string() {
		return add_field((STD_::string *)NULL);
	}

#ifndef DTL_NO_UNICODE
	TypeTranslationField variant_row::_wstring() {
		return add_field((STD_::wstring *)NULL);
	}
#endif

	TypeTranslationField variant_row::_tstring() {
		return add_field((tstring *)NULL);
	}

	TypeTranslationField variant_row::_tchar_star() {
		return add_field((TCHAR **)NULL);
	}

#if 0
	// use _timestamp() to bind fields instead

	TypeTranslationField variant_row::_jtime_c() {
		return add_field((jtime_c *)NULL);
	}

#endif

	TypeTranslationField variant_row::_bool() {
		return add_field((bool *)NULL);
	}

	TypeTranslationField variant_row::_blob() {
		return add_field((blob *)NULL);
	}

void variant_row_SetNull(variant_row *vr, int i) {vr->SetNull(i);}
void variant_row_ClearNull(variant_row *vr, int i) {vr->ClearNull(i);}

END_DTL_NAMESPACE
