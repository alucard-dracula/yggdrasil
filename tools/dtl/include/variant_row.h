// -*- C++ -*-
/* Copyright � 2000
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
// Edited: 03/24/2004 - Alexander Motzkau, give the variant_row index to the TypeTranslationField

#ifndef VARIANT_H
#define VARIANT_H

#include "std_warn_off.h"
#include <algorithm>
#include <vector>
#include <string>
#include <bitset>
#include <iosfwd>
#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#include "std_warn_on.h"

#include "dtl_config.h"
#include "date_util.h"
#include "bind_basics.h"
#include "VariantException.h"
#include "CountedPtr.h"
#include "clib_fwd.h"

BEGIN_DTL_NAMESPACE

class variant_row ;
class variant_field;
class dtl_variant_t;

// marker used to indicate we want a field to be null
struct NullField
{
   // allows NullField() == NullField() == true
   bool operator==(const NullField &null) const;

   // allows NullField() != NullField() == false
   bool operator!=(const NullField &null) const;

};

void variant_row_SetNull(variant_row *vr, int i);
void variant_row_ClearNull(variant_row *vr, int i);

// needed for dtl_variant_t(const variant_field &vf) constructor
dtl_variant_t variant_val(const variant_field &vf);


// Note: Some compilers fail to explicitely instantiate member template functions:
// They won't compile: int n = my_variant.get<int>()
// One solution is to add an extra dummy argument to help name lookup.
//
// To work around this, some methods will have this
// extra argument. This extra argument is not used so it is declared as a const* so
// you can pass (T const*)NULL.
//


#ifdef _MSC_VER
// 'int' : forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning(disable: 4800)
#endif

#ifdef __GNUC__
#  define _GCC_CAST_BUG
#endif

#ifdef __BORLANDC__
#  define _GCC_CAST_BUG
#endif

#ifdef _GCC_CAST_BUG
// gnu C++ 2.95 for some reason does not recognize e.g. short int as a
// single typename.  Hacky workaround to fix
// This class has been moved from within the dtl_variant_t class because of
// problems with the aCC compiler (A.03.33) on HPUX
template<class TT>
class integral_type
{
public:

  TT  si;

  integral_type() : si(0) {};

  template<class UU>
  integral_type(UU t)
    { si = static_cast<TT>(t); }
};
#endif

// This base class provides polymorphism to Impl<>, due
// to the virtual destructor.
// It implements the reference counting over Impl<>.
class ImplBase_t
{
public:
  ImplBase_t() : refs ( 0 ) {}
  virtual ~ImplBase_t() {}
  void AddRef () { refs ++ ; }
  void Release() { refs -- ;
  if ( refs == 0 )
	delete this ;
  }
  size_t refs ;
};


// An instance of Impl<T> is just a straight copy of a
// value of type T, but re-typed as a polymorphic type.
// As long as the copy constructor for T is properly
// implemented, the value will be properly copied.
template<typename T>
class Impl_t : public ImplBase_t
{
public:
  Impl_t ( T const& v ) : data ( v ) {}
  ~Impl_t () {}
  T data ;
};

// Class dtl_variant_t
// Encapsulates a value of arbitrary type.
// It can be constructed with values of any type without
// explicit casting. Its value can be retrieved by
// converting it to a copy of apropriate type.
// If an attempt is made to convert it to a type other than
// the type used with the constructor, invalid_argument is
// thrown.
// dtl_variant_t objects can be efficiently and safely copied
// without loosing the value and type.
//
// Original Mar 2000, Fernando Luis Cacciola Carballal.
//                      to avoid overload resolution problems.
//
class dtl_variant_t
{
protected:
    template<typename T>
    static Impl_t<T>* CastFromBase ( ImplBase_t* v , T const* =NULL )
    {
      // This upcast will fail if T is other than the T used
      // with the constructor of dtl_variant_t.
      Impl_t<T>* p = dynamic_cast<Impl_t<T>*> ( v ) ;
      if ( p == NULL )
	  {
		  STD_::string errmsg;
		  errmsg.reserve(512);
		  errmsg += typeid(T).name();
		  errmsg += " is not a valid type cast for this data";
		  DTL_THROW VariantException(_TEXT("dtl_variant_t::CastFromBase()"),
             tstring_cast((tstring *) NULL, errmsg));
	  }
      return p ;
    }

    ImplBase_t* data ;

	char typeId;

	// Generic cast operator for numeric types
	template<typename T> T
    numeric_cast(T DTL_DUMMY_PARAM_1) const
    {
	  switch (typeId)
	  {
	  case C_SHORT:
		{
		  short *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, short)(data, dmy)->data);
		}
	  case C_USHORT:
		{
		  unsigned short *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, unsigned short)(data, dmy)->data);
		}
	  case C_BOOL:
		{
		  bool *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, bool)(data, dmy)->data);
		}
	  case C_INT:
		{
		  int *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, int)(data, dmy)->data);
		}
	  case C_UINT:
		{
		  unsigned int *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, unsigned int)(data, dmy)->data);
		}
	  case C_LONG:
		{
		  long *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, long)(data, dmy)->data);
		}
	  case C_ULONG:
		{
		  unsigned long *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, unsigned long)(data, dmy)->data);
		}
	  case C_INT64:
		{
		  ODBCINT64 *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, ODBCINT64)(data, dmy)->data);
		}

      case C_DOUBLE:
		{
		  double *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, double)(data, dmy)->data);
		}
	  case C_FLOAT:
		{
		  float *dmy=NULL;
		  return T(DTL_TEMPLATE_FUNC(CastFromBase, float)(data, dmy)->data);
		}
	  // tstring cases must specially handle _TEXT("true") and _TEXT("false")
	  // to provide proper conversion to bool
	  case C_CHAR_STAR:
		{
		  char* *dmy=NULL;

		  // handle _TEXT("true") and _TEXT("false") in a case insensitive
		  // manner convert to 1 and 0 respectively ... should work for bool
		  STD_::string str_to_cast =
			STD_::string(DTL_TEMPLATE_FUNC(CastFromBase, char *)(data, dmy)->data);

		  // tolower() the tstring
		  STD_::transform(str_to_cast.begin(),
						  str_to_cast.end(),
						  str_to_cast.begin(),
						  std_ttolower);

		  if (str_to_cast == "true")
			return T(1);
			else if (str_to_cast == "false")
			  return T(0);
		  else // else we have a number
			return T(std_atof(str_to_cast.c_str()));

		  // return T(std_tatof(CastFromBase(data, dmy)->data));
		}

	  case C_STRING:
		{
		  STD_::string *dmy=NULL;

		  // handle _TEXT("true") and _TEXT("false") in a case insensitive
		  // manner convert to 1 and 0 respectively ... should work for bool
		  STD_::string str_to_cast =
			STD_::string(DTL_TEMPLATE_FUNC(CastFromBase, STD_::string)(data, dmy)->data);

		  // tolower() the tstring
		  STD_::transform(str_to_cast.begin(),
						  str_to_cast.end(),
						  str_to_cast.begin(),
						  std_ttolower);

		  if (str_to_cast == "true")
			return T(1);
		  else if (str_to_cast == "false")
			return T(0);
		  else // else we have a number
			return T(std_atof(str_to_cast.c_str()));

		  // return T(std_tatof(CastFromBase(data, dmy)->data.c_str()));
		}
#ifndef DTL_NO_UNICODE
	  case C_WSTRING:
		{
		  STD_::wstring *dmy=NULL;

		  // handle _TEXT("true") and _TEXT("false") in a case insensitive
		  // manner convert to 1 and 0 respectively ... should work for bool
		  STD_::wstring str_to_cast =
			STD_::wstring(DTL_TEMPLATE_FUNC(CastFromBase, STD_::wstring)(data, dmy)->data);

		  // tolower() the tstring
		  STD_::transform(str_to_cast.begin(),
						  str_to_cast.end(),
						  str_to_cast.begin(),
						  std_ttolower);

		  if (str_to_cast == L"true")
			return T(1);
		  else if (str_to_cast == L"false")
			return T(0);
		  else // else we have a number
			{
			  STD_::wistringstream numstr(str_to_cast);
			  double d;
			  numstr >> d;

			  return T(d);

			  // return T(std_tatof(CastFromBase(data, dmy)->data.c_str()));
			}
		}
#endif
		// eventually may add class capabilities to timestamp to pull out
		// jdate here
		// case C_TIMESTAMP:

		default:
		  DTL_THROW VariantException
			(_TEXT("dtl_variant_t::numeric_cast()"),
			 tstring_cast((tstring *)NULL, STD_::string(typeid(T).name()))+
			 tstring(_TEXT(" cannot be cast to a numeric type")));

		  return T(0);
	  }

	}

    // Map the class to our typeid enumeration
	template<class T> char find_typeid(const T & DTL_DUMMY_PARAM_1) {
		// use RTTI to get the type of the object
		static STD_::string nameOfSelf = typeid(dtl_variant_t).name();
		STD_::string nameOfType = DTL_TYPEID_NAME (T);
	        STD_::string::size_type del = 6, pos;

		// remove "const" descriptors from the type name
		while ((pos = nameOfType.find("const ", 0)) != STD_::string::npos) {
			nameOfType.erase(pos, del);
		}

		if (nameOfType == nameOfSelf)
			return 0;

	    // make sure the type is in the ETI map!  If not,
		// a default TypeTranslation object will be built, with the
		// TypeComplexity as invalid
		// if we check the TypeComplexity and it is TYPE_INVALID , we've got an exception!
		ETI_Map &SQL_types_to_C = GetSQL_types_to_C();
		TypeTranslation &tt = SQL_types_to_C[nameOfType];

		if (tt.complexity == TypeTranslation::TYPE_INVALID)
		{
			STD_::ostringstream str;
			str << static_cast<unsigned int>(SQL_types_to_C.size());
			STD_::string errmsg;
			errmsg.reserve(512);
			errmsg += typeid(T).name();
			errmsg += " looked up as ";
			errmsg += nameOfType;
			errmsg += " in list of ";
			errmsg += str.str();
			errmsg += "types.";
			errmsg += "This is not a valid type";
		    DTL_THROW VariantException(_TEXT("dtl_variant_t::find_typeid()"),
			    tstring_cast((tstring *)NULL, errmsg));
		}

		return tt.typeId;
	}

	template<typename T> void private_construct(const T &other)
    {
		data = new Impl_t<T>(other);
		data->AddRef() ;
		typeId = find_typeid(other);
	}

  	void private_construct(const dtl_variant_t &other);

public :

  dtl_variant_t();

  // This member template constructor allows you to
  // instance a dtl_variant_t object with a value of any type.

  // N.B.!!! we *must* have explicit below.
  // If not the following would be legal via an unwanted conversion to dtl_variant_t

  /*
  using namespace std;
using namespace dtl;

class A
{
};

int main()
{
  A a;
  cout << a << endl;  // ERROR.  The compiler converts "a" to a dtl_variant_t w/o explicit This code will now erroneously compile! -- but fail at runtime
}
  */

  template<typename T>
#ifndef _MSC_VER
	  explicit  // Visual C++ 7 chokes on explicit here, in Visual C++ 6 explicit does not do anything
#endif
	  dtl_variant_t (const T & v)
  {private_construct(v);}


  dtl_variant_t (const dtl_variant_t & v);

  dtl_variant_t (const variant_field &vf);

  dtl_variant_t(BYTE* p, const TypeTranslation &f);

  ~dtl_variant_t();

  template<typename T> /*PS const*/ dtl_variant_t & operator=(const T &other) {
		if (reinterpret_cast<const BYTE*>(this) != reinterpret_cast<const BYTE*>(&other)) {
			if (data != NULL)
				data->Release();
			private_construct(other);
		}
		return *this;
	}

  // exception-safe swap()
  void swap(dtl_variant_t &other);

  // exception-safe assignment
  dtl_variant_t &operator=(const dtl_variant_t &other);

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

	operator short int() const;

	operator unsigned short int() const;

	operator long int() const;

	operator unsigned long int() const;

	operator unsigned int () const;

	operator bool() const;

	operator int () const;

#ifndef DTL_NO_ODBCINT64
	operator ODBCINT64 () const;
#endif

	operator float () const;

	operator double () const;

	// operator for strings

	STD_::string get_string() const;

	blob get_blob() const;

#ifndef DTL_NO_UNICODE
	STD_::wstring get_wstring() const;
#endif

	tstring get_tstring() const {
#ifndef DTL_NO_UNICODE
		return get_wstring();
#else
		return get_string();
#endif
	}

	operator STD_::string () const;
#ifndef DTL_NO_UNICODE
	operator STD_::wstring () const;
#endif

	operator char * () const;

	operator char() const;

    operator struct tagTIMESTAMP_STRUCT () const;

	// end cast of thousands

  // With expressions of the form: int a = my_variant ,
  // the compiler needs to take the template argument T
  // from the type of the left side of the assigment.
  // Most compilers won't do that, so you might need this
  // non-operator form, as in:
  // int a = my_variant.get<int>();
  // This forms returns a REFERENCE and not a COPY, which
  // will be significant in some cases.
  template<typename T> const T & get( T const* dmy =NULL ) const
    { return CastFromBase( data , dmy )->data ; }

  // This method can be used to test if this dtl_variant_t
  // holds a value of type T. It takes no arguments,
  // so it can only be used with a explicit template
  // instancing:
  //   dtl_variant_t var(3) ; if ( var.is_type<int>() ) ...
  template<typename T> bool is_type( T const* =NULL ) const
     { return typeid(*data)==typeid(Impl_t<T>); }

  // This method can be used to test if this dtl_variant_t
  // holds a value of type T.
  // It takes one argument, so it can be used with
  // a test variable of the desired type:
  //   int n=3; dtl_variant_t var(n) ;
  //   if ( var.is_type_as(n) ) ...
  template<typename T> bool is_type_as(T const& v) const
    { return typeid(*data)==typeid(Impl_t<T>); }

  template<typename T> bool is_type_as(T const* v) const
    { return typeid(*data)==typeid(Impl_t<T>); }

  // This method returns type enumeration value
  char type() const;

  // Map the class to our typeid enumeration
  TypeTranslation type_translation();
} ;


//
// non-member auxiliary functions.
// Needed for compilers that fail with: my_variant.get<int>().
//

// usage: int n = variant_cast<int>(my_variant);
template<typename T>
T variant_cast  (const dtl_variant_t& v)
{
     return v.get((T const*)(NULL));
}

// usage: bool b = variant_is_type_as(my_variant,my_type_variable);
//
// NOTE: For some reason MSVC fails to properly instantiate this functions unless
// the 'T const&' argument is added.
template<typename T>
bool variant_is_type_as (const dtl_variant_t& v, T const& t)
{
  return v.is_type_as(t);
}
template<typename T>
bool variant_is_type_as (const dtl_variant_t& v, T const* t)
{
  return v.is_type_as(t);
}

// powerful stream operator for dtl_variant_t's!!!!
STD_::ostream &operator<<(STD_::ostream &o, const dtl_variant_t &v);
#if !defined(DTL_NO_UNICODE)
STD_::wostream &operator<<(STD_::wostream &o, const dtl_variant_t &v);
#endif

// this module defines basic variant types for dynamic row binding
// Initial: 11/16/2000 - CJ
class variant_row_fields {
public:
	STD_::vector<TypeTranslation> types;
	STD_::vector<tstring> names;
   // TODO this is used as an address in variant_row, and
   // an offset in DynaDBView.  Which is it?!
	STD_::vector<std::ptrdiff_t> offsets;
	size_t row_size;

	variant_row_fields();

	// Construct form fields & names, but compute offsets from scratch
	variant_row_fields(STD_::vector<TypeTranslation> &f,	STD_::vector<tstring> &n);

	void push_back(TypeTranslation &tt, tstring name);

	// exception-safe swap()
	void swap(variant_row_fields &other);

	// exception-safe assignment
	variant_row_fields &operator=(const variant_row_fields &other);

	// accessors
	STD_::vector<TypeTranslation> GetTypes()   const;
	STD_::vector<tstring>	  GetNames()   const;
	STD_::vector<std::ptrdiff_t>		  GetOffsets() const;

	size_t size() const;

    // comparison operators
	friend bool operator==(const variant_row_fields &vf1,
						   const variant_row_fields &vf2);

	friend bool operator!=(const variant_row_fields &vf1,
						   const variant_row_fields &vf2);

};

// stream operator for variant_field's
STD_::ostream &operator<<(STD_::ostream &o, const variant_field &vf);
#if !defined(DTL_NO_UNICODE)
STD_::wostream &operator<<(STD_::wostream &o, const variant_field &vf);
#endif

class DynamicRowBCA;

// Class to hold a row of data as a variant set of fields
#define MAX_VARIANT_ROW_FIELDS 512

class variant_row {

friend class variant_field;
friend class DynamicRowBCA;

private:

	STD_::bitset<MAX_VARIANT_ROW_FIELDS> b_IsNull;
	CountedPtr<variant_row_fields> p_row_fields;
	BYTE *p_data;
	// needed for reentrancy of BCA and BPA
	size_t tt_i; // index # of field being manipulated in already existing variant row

	bool b_do_not_delete;

	void init_field(BYTE* v, int typeId);

	void copy_construct_field(BYTE* dest, const BYTE* src, int typeId);

	void destroy_field(BYTE* v, int typeId);

	void init_fields(void);

	void destroy_fields(void);

	void destroy(void);

	void copy_construct_fields(const variant_row &other);

	// replace field value
	void replace(const variant_field &field);

	STD_::vector<TypeTranslation>::const_iterator find_field(const tstring &f) const;

public:
	void replace(const variant_row &other);


	void init(void);

	bool IsInit() const;

	variant_row();

	variant_row(STD_::vector<TypeTranslation> &types, STD_::vector<tstring> &names);

	variant_row(const variant_row &other);

	variant_row(const variant_row &other, BYTE *ptr);

	BYTE *data_ptr() const;

	void set_data_ptr(BYTE *ptr);

	// exception-safe swap()
	void swap(variant_row &other);

	// Slightly complicated assignment operator for two reasons
	// 1. We wanted it to be exception safe
	// 2. If the variant_rows have the same variant_row_fields we need to copy the contents
	// and not change the address so that DynamicDBView can still function
	// with variant_row parameters
	//
    const variant_row & operator=(const variant_row &other);

	// return row field by name
	variant_field operator[](const tstring &f);

	// return row field by number
	variant_field operator[](int i);

	// const versions of subscript operators
	// return row field by name
	const variant_field operator[](const tstring &f) const;

	// return row field by number
	const variant_field operator[](int i) const;

	// return # of columns in row
	size_t size() const;

	// return size of row
	size_t row_size() const;

	// return column names for the row
	STD_::vector<tstring> GetNames() const;

	// return type information for the columns in the row
	STD_::vector<TypeTranslation> GetTypes() const;

	// return type information for the columns in the row
	const variant_row_fields &GetVariantRowFields() const;

	// used for debugging to print out contents of variant row as a tstring
	STD_::string Stringify() const;
#ifndef DTL_NO_UNICODE
	STD_::wstring wStringify() const;
#endif

	friend bool operator==(const variant_row &vr1, const variant_row &vr2);

	friend bool operator<(const variant_row &vr1, const variant_row &vr2);

	friend bool operator!=(const variant_row &vr1, const variant_row &vr2);

	friend bool operator<=(const variant_row &vr1, const variant_row &vr2);

	friend bool operator>=(const variant_row &vr1, const variant_row &vr2);

	friend bool operator>(const variant_row &vr1, const variant_row &vr2);

	friend STD_::ostream &operator<<(STD_::ostream &o, const variant_row &vr);
#if !defined(DTL_NO_UNICODE)
	friend STD_::wostream &operator<<(STD_::wostream &o, const variant_row &vr);
#endif

	// clear all null field flags
	void ClearNulls();

	// returns whether field is a NULL value
	bool IsNull(const tstring &f) const;

	bool IsNull(int i) const;

	// mark the field with the given name as holding a NULL value
	void SetNull(const tstring &f);

	void SetNull(int i);

	// mark the field with the given name as holding a NULL value
	void ClearNull(const tstring &f);

	void ClearNull(int i);

	virtual ~variant_row();

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable : 4101) // shut up warning about 'not using' dummy variable below
#endif

	// functions to do on-the-fly binding / variant row creation
	template <class T> TypeTranslationField add_field(T * DTL_DUMMY_PARAM_1) {

		if (p_data != NULL)
		{

			TypeTranslationField ttf(p_row_fields->types.back(), // type translation


               // This is an offset from the base row address
				static_cast<BYTE*>(NULL) + p_row_fields->offsets.back(), // field offset


				static_cast<BYTE*>(NULL), // base_addr
				32768, // large arbitrary size to disable field check
				p_row_fields->names.back(), // field name
				tt_i++ // field nr
			);

			return ttf;
		}


		if (p_row_fields.get() == NULL)
			 p_row_fields = new variant_row_fields;

		const STD_::string nameOfType = DTL_TYPEID_NAME (T);

		ETI_Map &SQL_types_to_C = GetSQL_types_to_C();
		TypeTranslation &tt = SQL_types_to_C[nameOfType];
		p_row_fields->push_back(tt, _TEXT(""));

		return TypeTranslationField(tt,

               // TODO WHY is this described as a field offset, when its typed
               // as an address? This is used in DynaDBView as an offset, so
               // it SHALL become an offset now.  THIS MUST BE STRAIGHTENED OUT
				static_cast<BYTE*>(NULL) + p_row_fields->offsets.back(), // field offset
			static_cast<BYTE*>(NULL), // base_addr
			32768, // large arbitrary size to disable field check
			p_row_fields->names.back(), // field_name
			p_row_fields->size()-1 // field_nr
		);
	}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

	TypeTranslationField _short();

	TypeTranslationField _unsigned_short();

	TypeTranslationField _int();

	TypeTranslationField _unsigned_int();

	TypeTranslationField _long();

	TypeTranslationField _unsigned_long();

	TypeTranslationField _ODBCINT64();

    TypeTranslationField _double();

	TypeTranslationField _float();

	TypeTranslationField _timestamp();

	TypeTranslationField _string();

	TypeTranslationField _tchar_star();

#ifndef DTL_NO_UNICODE
	TypeTranslationField _wstring();
#endif

	TypeTranslationField _tstring();

    // Not supported.  Use _timestamp()
	// TypeTranslationField _jtime_c();

	TypeTranslationField _bool();

	TypeTranslationField _blob();

public:
	// tag to indicate failed conversion ... forcing code to burp in compiler
	// for bulk_copy() if variant_row used
	// conversion operator to failed_conversion declared private for this reason
	class failed_conversion { };

private:
	operator failed_conversion() { return failed_conversion(); }
};

class variant_field {
    friend class variant_row;
	dtl_variant_t m_val;
	variant_row *p_row;
	int m_col;
	bool m_IsNull;
public:

	variant_field();

	variant_field(const variant_field &other);

	variant_field(const dtl_variant_t &v, variant_row *row, int field, const bool b = false);

	// exception-safe swap()
	void swap(variant_field &other);

	// Special assignment operator - cascade changes back to parent row
	template<typename T> /*PS const*/ variant_field & operator=(const T &other) {
		if (reinterpret_cast<const BYTE*>(this) != reinterpret_cast<const BYTE*>(&other)) {
			m_val = other;
			p_row->replace(*this);
			ClearNull(); // if we are assigning, by definition assigned value is != NULL
		}
		return *this;
	}

	variant_field &operator=(const variant_field &other);

	// convenience operator to allow the setting of a variant_field to null
	/*PS const*/ variant_field & operator=(const NullField &null);

    // cast of thousands again!!!!
	// couldn't get template of the form template<T> operator T() to work w/o ambiguity.
	operator short () const;

	operator unsigned short () const;

	operator long () const;

	operator unsigned long () const;

	operator unsigned int () const;

	operator int () const;
#ifndef DTL_NO_ODBCINT64
	operator ODBCINT64 () const;
#endif

	operator float () const;

	operator double () const;

	operator bool() const;

	operator STD_::string() const;
#ifndef DTL_NO_UNICODE
	operator STD_::wstring() const;
#endif

	operator char() const;

	operator char*() const;

	operator struct tagTIMESTAMP_STRUCT() const;

	operator dtl_variant_t() const;

	// end of cast of thousands

	bool IsNull() const;

	// so we can test for null using syntax: var_row[fieldName] == NullField()
	bool operator==(const NullField &null) const;

	bool operator!=(const NullField &null) const;

	// allows NullField() == var_row[fieldName]
	friend bool operator==(const NullField &null, const variant_field &vf);

	friend bool operator!=(const NullField &null, const variant_field &vf);

	void SetNull();

	void ClearNull();

	friend dtl_variant_t variant_val(const variant_field &vf);

    // needed to resolve ambiguity for casts
    STD_::string get_string() const;
	blob get_blob() const;

#ifndef DTL_NO_UNICODE
	STD_::wstring get_wstring() const;
#endif

	tstring get_tstring() const;


	// returns an enumeration listing the type of the variant data
	char type() const;

	// This method can be used to test if this dtl_variant_t
    // holds a value of type T. It takes no arguments,
    // so it can only be used with a explicit template
    // instancing:
    //   dtl_variant_t var(3) ; if ( var.is_type<int>() ) ...
    template<typename T> bool is_type( T const*t =NULL ) const {
		return m_val.is_type(t);
	}

	virtual ~variant_field() { }
};

template<class T> BYTE *data_ptr(const T *t) {return (BYTE *)t;}
BYTE *data_ptr(const variant_row *vr);

END_DTL_NAMESPACE

#endif
