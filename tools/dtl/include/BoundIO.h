// Emacs, this is -*- c++ -*- code.
/* Copyright 2000
Michael Gradman and Corwin Joy

Permission to use, copy, modify, distribute and sell this software and
its documentation for any purpose is hereby granted without fee, provided
that the above copyright notice appears in all copies and that both that
copyright notice and this permission notice appear in supporting documentation.
Corwin Joy and Michael Gradman make no representations about the suitability
of this software for any purpose.
It is provided "as is" without express or implied warranty.
*/
// class which abstracts the binding of columns and parameters
// Initial: ca. 9/5/2000 - MG
// Revised: 10/6/2000 - MG - added support for relative offsets
// Revised: 11/12/2000 - CJ - wrote faster GenericCmp function
// Edited: 12/19/2000 - MG - added namespaces
// Edited: 03/24/2004 - Alexander Motzkau, BoundIO remembers variant_row index

#ifndef DTL_BOUNDIO_H
#define DTL_BOUNDIO_H

#include "DB_Base.h"
#include "bind_basics.h"
#include "variant_row.h"
#include "DBException.h"
#include "string_util.h"

#include "std_warn_off.h"
#include <string>
#include <set>
#include <assert.h>

#ifdef DTL_NATIVE_WIN32

	#ifdef DTL_USE_MFC

		#include <afx.h>

	#else

		#include <windows.h>

	#endif

#endif

#include <sqltypes.h>

#include "std_warn_on.h"

// using namespace std;

BEGIN_DTL_NAMESPACE

// extern const ETI_Map SQL_types_to_C;
class BoundIOs;
size_t BoundIOs_NumColumns(BoundIOs *pB);

BYTE* data_ptr(variant_row &vr);

class GenerateBoundIOs;

template<class DataObj> class LocalBCA;

// workaround for the wrong version of MakeActualCopyOfMember getting called

template <typename T>
struct MakeActualCopyOfMember
{
   // dest is the address of the target, so we dereference.
   static void copy( BYTE* dest, const T& src )
   {
      *reinterpret_cast<T*>(dest) = src;
   }
};

template <>
struct MakeActualCopyOfMember<TCHAR*>
{
   // dest is the address of the FIRST CHARACTER in the string.
   // above we dereference, here we don't.
   static void copy( BYTE* dest, const TCHAR* src )
   {
      std_tstrcpy(reinterpret_cast<TCHAR*>(dest),src);
   }
};


class BoundIOs_base
{
public:

	virtual BYTE* GetWorkingAddr() { return NULL; };

    virtual size_t GetWorkingObjSize () = 0;

	virtual STD_::string GetWorkingObjType () { return ""; };

	virtual ~BoundIOs_base ( ) { }
};

// see below for documentation
template <typename T> struct correct_raw_ptr { typedef T* type; };
template <> struct correct_raw_ptr<TCHAR*> { typedef TCHAR* type; };

// class which abstracts the binding of columns and parameters
class BoundIO
{
  public:
    // what does the BoundIO structure map data to?
    enum BoundType { UNKNOWN, COLUMN, PARAM };

    // bind as columns or parameters?
    enum ColumnsOrParams { BIND_AS_COL, BIND_AS_PARAM };

	static const size_t MINSTRBUFLEN;

  private:
	BYTE* addr;				// absolute address of data
	std::ptrdiff_t offset;		// relative offset of this field from base address of its DataObj
							// or ParamObj where applicable

	SQLSMALLINT paramType;
	tstring name;
   TypeTranslation typeTranslation;
	SDWORD size;
	SQLLEN bufferLength;

	CountedPtr<SQLLEN> bytesFetched;

	static const SQLLEN lenAtExec; // needed for PutData()

	int VariantRowIdx;		// -1: It isn't a variant row
					// Otherwise: Index into the variant row
	BoundIOs_base *pBoundIOs;	// refers to BoundIOs that this object belongs to

	BoundType bindingType;  // column or param???

   bool can_be_null;

	int colNo;				// column number


	MemPtr strbuf;		   // needed for use with STL strings
						   // points to a buffer allocated
						   // for string or wstring
						   // NULL for all other types


	// workaround for compiler member specialization issue instead of
	// being able to specialize BoundIO::CopyMember(), we cheat
	// we wrote a specialization for variant_row and do nothing otherwise
	// the function below should never get called!
	template <class T> void CopyVariantRowMember(T & DTL_DUMMY_PARAM_1, const dtl_variant_t & DTL_DUMMY_PARAM_2) {
	}

	// form of CopyMember() for variant
	void CopyVariantRowMember(variant_row &vr, const dtl_variant_t &m);

	// only used by GenerateBoundIOs in LocalBCA.h
	BoundIO(const tstring &nm, BoundType bt);

  public:

	BoundIO();

//	BoundIO(const tstring &nm, BoundColMode bcMode, BoundType bt, BoundIOs &parentCollection);

	BoundIO(const tstring &nm, BoundType bt, BoundIOs &parentCollection);

	BoundIO(BoundIOs &parentCollection, const tstring &nm, const TypeTranslation &tt, BYTE* field, BYTE* base_addr, size_t base_size);

	BoundIO(const BoundIO &boundIO);

	// only used by GenerateBoundIOs in LocalBCA.h
	// works like copy constructor, but overrides parent collection ptr.
	// with the reference passed in
	BoundIO(const BoundIO &boundIO, BoundIOs &parentCollection);

	// nothrow swap
	void swap(BoundIO &other);

	// exception safe assignment
	BoundIO &operator=(const BoundIO &boundIO);

	// BoundColMode GetBoundColMode() const;

	BoundType GetBindingType() const;

	void SetBindingType(BoundType bt);

	int GetColNo() const;

	void SetColNo(int iCol);

	bool IsParam()  const;

	bool IsColumn() const;

   void SetNullable(bool x);
   bool IsNullable() const;

	bool IsKnown() const;

	// returns whether the column fetched is null
	bool IsNull() const;

	// set the given column to represent a NULL value for writing parameters
	void SetNull();

	// clear the NULL status given above
	// indicator must be SQL_NTS for strings and 0 otherwise
	// except for strings, all SQL_C types have fixed lengths
	void ClearNull();

	// does this BoundIO represent a tstring?
	bool IsString() const;

	// does this BoundIO represent a variable length string?
	bool IsVariString() const;

	// is this an array_string
	bool IsCString() const;

	bool IsPrimitiveColumn() const {return typeTranslation.IsPrimitive();}

	bool IsJtime() const;

	template<class DataObj> int Compare(DataObj const* key1, DataObj const* key2) const
	{
		// offset into the objects should be valid
		// as we checked the offset back in BoundIOs::operator==()
		// so no check needs to be done here

		// get pointer to the actual member in each DataObj
		// now using our BoundIO offset
		BYTE* pMember1 = data_ptr(key1) + offset;
		BYTE* pMember2 = data_ptr(key2) + offset;

		// now we must cast to the appropriate type based on the
		// type name of the BoundIO and then perform the comparison
	    return GenericCmp(pMember1, pMember2, GetTypeID());
	}

	template<class DataObj> size_t Hash(DataObj *key1) const
	{
		// offset into the objects should be valid
		// as we checked the offset back in BoundIOs::operator==()
		// so no check needs to be done here

		// get pointer to the actual member in each DataObj
		// now using our BoundIO offset
		BYTE* pMember1 = data_ptr(key1) + offset;

		// now we must cast to the appropriate type based on the
		// type name of the BoundIO and then perform the hash
	    if (IsNull())
			return 0;  // null fields hash to the identity element
		else
			return GenericHash(pMember1, GetTypeID());
	}

	// overloads needed for variants to correctly handle null fields
	int Compare(variant_row *key1, variant_row *key2) const;

	size_t Hash(variant_row *key1) const;

	// needed for proper STL tstring handling
	// MoveRead() and MoveWrite()
	// the functions that propagate the data back to the STL tstring
	// upon reading or writing respectively from/to the database
	void MoveRead(DBStmt &stmt);
	void MoveWrite(SQLQueryType sqlQryType, DBStmt &stmt);
	void MoveWriteAfterExec(SQLQueryType sqlQryType, DBStmt &stmt);

	// comparison operator for BoundIO objects ...
	// needed by BoundIOs::operator[]()
    friend bool operator==(const BoundIO &bound1, const BoundIO &bound2);

	// get proper address for use by actual BindCol() and BindParam()
	BYTE* GetRawPtr() const;

	// get size of raw buffer
	SQLLEN GetRawSize() const;

	// get precision of value
	SQLSMALLINT GetPrecision() const;

	// get SQL column size for SQLBindParam purposes of value
	SQLULEN GetColumnSize(const DBConnection &conn) const;

	// assuming buffer length and raw size are the same
	SQLLEN GetBufferLength() const;

	// get C Type
	SQLSMALLINT GetCType() const;

	// get Input/Ouput parameter type
	SQLSMALLINT GetParamType() const;

	void SetParamType(SQLSMALLINT ptype);

	// get SQL Type
	SQLSMALLINT GetSQLType() const;

	// set SQL Type ... use this method to override the default SQL type used
	// vs. your C type
	void SetSQLType(SQLSMALLINT newSqlType);

	// set C Type ... use this method to override the default ODBC C type used
	// when binding a field .e.g.the default SQL_C_type for a char data field
	// is SQL_C_CHAR, but you might want to use SQL_C_TINYINT to hold an int in
	// a char field instead
	void SetCType(SQLSMALLINT newCType);

	BoundIO & SetColumnSize(size_t sz);

	// get type ID
	int GetTypeID() const;

	// get pointer to bytesFetched
	SQLLEN *GetBytesFetchedPtr() const;

	// set pointer to bytesFetched
	void SetBytesFetchedPtr(SQLLEN *ptr);

	// get pointer to length at exec ptr. for PutData()
	SQLLEN *GetLenAtExecPtr() const;

	// returns name of the bound IO object (column name or parameter #)
	const tstring & GetName() const;

	BoundIOs *GetBoundIOsPtr() const;

	void SetBoundIOsPtr(BoundIOs *ptr);

	BYTE* GetAddr() const;

	void SetAddr(BYTE* address);

	void SetStrbufAddr(BYTE *address, size_t sz);

	void SetStrbufAddrOnly(BYTE *address);

	void SetBufferLength(SQLLEN sz);

	// return relative offset from bound DataObj
	std::ptrdiff_t GetOffset() const;
	void SetOffset(std::ptrdiff_t diff);

	void SetAsParam(int numParam);

	// set the field for this BoundIO to NULL if a column and primitive
	void InitNullField();

	virtual ~BoundIO();

    // binding operator ... maps the memory structure (object member) to a database column
    // or SQL parameter
    // will use RTTI to determine the information to fill the BoundIO with
    // and use a lookup map to determine the appropriate SQL and C types for the appropriate // run-time type
    // may need a specialization for strings

    // must check bc.GetBoundColMode()  first ... BIND_ADDRESSES -  bind just the
    // names, do not bind addresses
    // BIND_ADDRESSES - bind both names and addresses
	// give binding operator access to the innards of this class

private:

	template<class memberType> void GenericBindPrimitive(memberType &memberToBind)
	{
//	  std::cerr << "Bind " << typeid (memberToBind).name ( ) << std::endl;
		// common tasks for BIND_ADDRESSES and BIND_AS_INDICES

	    // use RTTI to get the type of the object
		const STD_::string RTTI = DTL_TYPEID_NAME (memberToBind);

		ETI_Map &SQL_types_to_C = GetSQL_types_to_C();
		TypeTranslation &tt = SQL_types_to_C[RTTI];
		tt.size = sizeof(memberToBind);  // we need to reset size here to handle char[]

		BYTE*   base_addr;
		size_t  base_size;

		if (pBoundIOs)
		{
		  base_addr = pBoundIOs->GetWorkingAddr();
		  base_size = pBoundIOs->GetWorkingObjSize();
		}
		else
		{
		  base_addr = addr;
		  base_size = tt.size;
		}

		// exception safety handled by InitFromField
		InitFromField(tt, reinterpret_cast<BYTE*>(&memberToBind), base_addr, base_size);
	}

	template<typename T> void GenericBindString(T &memberToBind)
	{
//	  std::cerr << "Bind " << typeid (memberToBind).name ( ) << std::endl;
		// common tasks for BIND_ADDRESSES and BIND_AS_INDICES

	    // use RTTI to get the type of the object
		const STD_::string RTTI = DTL_TYPEID_NAME (memberToBind);

		ETI_Map &SQL_types_to_C = GetSQL_types_to_C();
		TypeTranslation tt = SQL_types_to_C[RTTI];
		tt.size = memberToBind.capacity();

		BYTE* base_addr;
		size_t base_size;


		if (pBoundIOs)
		{
			base_addr = pBoundIOs->GetWorkingAddr();
			base_size = pBoundIOs->GetWorkingObjSize();
		}
		else
		{
			base_addr = addr;
			base_size = tt.size;
		}

		// exception safe binding handled by InitFromField
		InitFromField(tt, reinterpret_cast<BYTE*>(&memberToBind), base_addr, base_size);
	}

	void GenericBind(STD_::string &memberToBind)
	{
		GenericBindString(memberToBind);
	}

	void GenericBind(blob &memberToBind)
	{
		GenericBindString(memberToBind);
	}

#ifndef DTL_NO_UNICODE
	void GenericBind(STD_::wstring &memberToBind)
	{
		GenericBindString(memberToBind);
	}
   DTL_GENERIC_BIND_OPERATORS(STD_::wstring);
#endif

  // Specialize for tcstring since that is all we
  // understand at this point.

  template<size_t N>
  void GenericBindArrayString (tcstring<N>& memberToBind)
  {
    const STD_::type_info& RTTI = typeid (fake_tcstring);

    ETI_Map& SQL_types_to_C = GetSQL_types_to_C ( );
    TypeTranslation tt = SQL_types_to_C[RTTI.name ( )];
    tt.size = (N + 1) * sizeof(TCHAR);

    BYTE* base_addr;
    size_t base_size;

    if (pBoundIOs) {
      base_addr = pBoundIOs->GetWorkingAddr();
      base_size = pBoundIOs->GetWorkingObjSize();

    } else {
      base_addr = addr;
      base_size = (N + 1) * sizeof(TCHAR);
    }

    // exception safe binding handled by InitFromField
    // InitFromField (tt, &memberToBind, base_addr, base_size);
    InitFromField (tt, reinterpret_cast<BYTE*>(&*(memberToBind.begin())), base_addr, base_size);
  }

  template<size_t N>
	  void GenericBind(tcstring<N> &memberToBind)
  { GenericBindArrayString(memberToBind); }



public:

#define DTL_GENERIC_BIND_OPERATORS(type) \
  BoundIO operator==(type &memberToBind) { \
		paramType = SQL_PARAM_INPUT_OUTPUT; \
		GenericBind(memberToBind); \
		return *this; \
  } \
  \
  BoundIO operator<<(type &memberToBind) { \
		paramType = SQL_PARAM_INPUT;  \
		GenericBind(memberToBind);  \
		return *this;  \
  } \
  \
  BoundIO operator>>(type &memberToBind) { \
		paramType = SQL_PARAM_OUTPUT; \
		GenericBind(memberToBind); \
		return *this; \
  }


// macro used to generate overloads which call GenericBindPrimitive()
#define DTL_GENERIC_BIND_PRIMITIVE(type) \
  inline void GenericBind(type &memberToBind) \
  { GenericBindPrimitive(memberToBind); } \
  \
  DTL_GENERIC_BIND_OPERATORS(type)

  // define operators for our special binders

  // needed to be templated ...
  template<size_t N>
  BoundIO operator==(tcstring<N> &memberToBind) {
		paramType = SQL_PARAM_INPUT_OUTPUT;
		GenericBind(memberToBind);
		return *this;
  }

  template<size_t N>
  BoundIO operator<<(tcstring<N> &memberToBind) {
		paramType = SQL_PARAM_INPUT;
		GenericBind(memberToBind);
		return *this;
  }

  template<size_t N>
  BoundIO operator>>(tcstring<N> &memberToBind) {
		paramType = SQL_PARAM_OUTPUT;
		GenericBind(memberToBind);
		return *this;
  }

  // we defined the GenericBind() for these
#ifndef DTL_NO_UNICODE
   DTL_GENERIC_BIND_OPERATORS(STD_::wstring);
#endif
   DTL_GENERIC_BIND_OPERATORS(STD_::string);
   DTL_GENERIC_BIND_OPERATORS(blob);

   // The GenericBind() can be generated for these
    DTL_GENERIC_BIND_PRIMITIVE(short);
//	DTL_GENERIC_BIND_PRIMITIVE(unsigned short); // not supported as unsigned short is the
												// type used for wchar_t on some compilers
	DTL_GENERIC_BIND_PRIMITIVE(bool);
	DTL_GENERIC_BIND_PRIMITIVE(int);
	DTL_GENERIC_BIND_PRIMITIVE(unsigned int);
	DTL_GENERIC_BIND_PRIMITIVE(long);
	DTL_GENERIC_BIND_PRIMITIVE(unsigned long);
	DTL_GENERIC_BIND_PRIMITIVE(double);
	DTL_GENERIC_BIND_PRIMITIVE(float);

	DTL_GENERIC_BIND_PRIMITIVE(struct tagTIMESTAMP_STRUCT);
	DTL_GENERIC_BIND_PRIMITIVE(struct tagDATE_STRUCT);
	DTL_GENERIC_BIND_PRIMITIVE(struct tagTIME_STRUCT);
	DTL_GENERIC_BIND_PRIMITIVE(jtime_c);

#ifndef DTL_NO_ODBCINT64
	DTL_GENERIC_BIND_PRIMITIVE(ODBCINT64);
#endif




        // if you are binding character arrays in Borland you will
        // need to manually set the size by calling SetColumnSize
        // e.g.
        // struct Rowbuf {char strarray[50];}
        // rowbuf["STRING_FIELD"] == rowbuf.strarray;
        // rowbuf.SetColumnSize(sizeof(rowbuf.strarray));
private:
	void TypeTranslationFieldBind(TypeTranslationField &ttf);
    BoundIO CharBind(tchar_struct memberToBind);
public:
	size_t GetColumnSize() const;

   BoundIO operator==(tchar_struct memberToBind);
   BoundIO operator<<(tchar_struct memberToBind);
   BoundIO operator>>(tchar_struct memberToBind);

   BoundIO operator==(TypeTranslationField ttf);
   BoundIO operator<<(TypeTranslationField ttf);
   BoundIO operator>>(TypeTranslationField ttf);

	// Construct binding definition based on type & address
	void InitFromField(const TypeTranslation &tt, BYTE* field, BYTE* base_addr, size_t base_size);

	template<class DataObj, class DataField>
			  void CopyMember(DataObj &key, const DataField &df)
	{

		  if (VariantRowIdx>=0)
		  {
			  CopyVariantRowMember(key, dtl_variant_t(df));
			  return;
		  }

		  STD_::string dataObjType = DTL_TYPEID_NAME (key);
		  STD_::string baseType = pBoundIOs->GetWorkingObjType();

		  // check types of DataObj and of BoundIO's container base object
		  if (dataObjType != baseType)
		  {
			  tstring errmsg;
			  errmsg.reserve(512);
			  errmsg += _TEXT("Type mismatch for base object!  ");
			  errmsg += _TEXT("Expected type ");
			  errmsg += tstring_cast(static_cast<tstring*>(NULL), baseType);
			  errmsg += _TEXT("!  Instead got ");
			  errmsg += tstring_cast(static_cast<tstring*>(NULL), dataObjType);
			  errmsg += _TEXT("!");

			  DTL_THROW DBException(tstring(_TEXT("BoundIO::CopyMember()")),
					errmsg, NULL, NULL);
		  }

		  // if type names don't match, we've mismatched fields for sure
		  // throw an exception in this case

		  // other than this check, we must trust the programmer!
		  STD_::string dfTypeNm = DTL_TYPEID_NAME (df);

// skip datafield type check in GCC because of different convention
// for typeid().name()'s that we can't handle for constness
#ifndef __GNUC__
		  ETI_Map &SQL_types_to_C = GetSQL_types_to_C();

		  TypeTranslation &tt = SQL_types_to_C[StripConstFromTypename(dfTypeNm)];


		  if (tt.typeId != GetTypeID() && (GetTypeID() == C_TCSTRING && !is_tcstring(df)))
			  DTL_THROW DBException(_TEXT("BoundIO::CopyMember()"),
				_TEXT("Type mismatch in member!  Type of target value: ") +
				tstring_cast(static_cast<tstring*>(NULL), dfTypeNm) +
				_TEXT(" does not match bound column type!"),
				NULL, NULL);


#endif
		  // makes the actual copy of the member

          // In the case of primitive types, rawAddr is Type* and DataField is Type*
          //
          // In the case of TCHAR* (ie static string arrays), rawAddr is TCHAR* and
          // DataField is TCHAR* (DIFFERENT!).
          //
		  // generic case: *rawAddr = df
		  // TCHAR *: std_tstrcpy(*rawAddr, df)

		  MakeActualCopyOfMember<DataField>::copy( data_ptr(&key) + offset, df);
	}

	// needed so char literals can be coerced to pointers implicitly
	// void CopyMember(DataObj &key, const DataField &df);

	void SetAsVariantRow(int idx);

	bool GetIsVariantRow() const;

	int GetVariantRowIdx() const;

	SQLLEN GetActualBufferLength() const;

	friend class BoundIOs;

	friend class GenerateBoundIOs;

	// facilities used by LocalBCA
	// operator that generates a BoundIOs structure from two raw BoundIO's
	BoundIOs operator&&(const BoundIO &addMe);

	TypeTranslation GetTypeTranslation() const;
};

tostream &operator<<(tostream &o, const BoundIO &b);


// a mapping of columns and parameters to memory structures
// (usually for a specific view)
class BoundIOs : public STD_::map<tstring, BoundIO, NonCaseSensitiveLt>,
		 public BoundIOs_base
{
  private:
	// BoundIO::BoundColMode mode;
	BYTE* working_addr;			// base address of the DataObj we're bound to
								// needed so we can calculate offsets of the members
								// only needed in the case of indexes
								// but might be able to be used elsewhere

	STD_::string working_type;			// base object's type (DataObj)
	size_t working_size;			// size of the DataObj we're bound to
								// think of it as sizeof(*working_addr)
  public:
	// Members to support user defined types
	struct UserValidate
	{
		// pointer to source class that has BoundIOs pointing to member fields
		BYTE* src_class;

		// callbacks to specify default InsValidate and SelValidate routines that should
		// be called for this type.  these are used to finalize the reading/writing of any
		// user defined types.
		typedef bool (*validate_callback)(BoundIOs &bios, UserValidate *val);
		validate_callback ins_val, sel_val;

		// specify what set of BoundIOs point to member fields of this class
		STD_::vector<tstring> bios;


		UserValidate(BYTE* src, validate_callback ins, validate_callback sel, STD_::vector<tstring> &bios_names) :
		   src_class(src), ins_val(ins), sel_val(sel), bios(bios_names) {};

	};

  	STD_::vector<UserValidate> vals;

  public:
	int cColumns;

	MemPtr pBytesFetchedArray;
	MemPtr pRowStatusArray;
	CountedPtr<SQLULEN> pNumRowsFetched;
	MemPtr pDatesFetchedArray;


	// BoundIOs(BoundIO::BoundColMode bcMode = BoundIO::BIND_ADDRESSES);
	BoundIOs();

	// Should rebind bases for proper behavior after call to copy constructor!
	BoundIOs(const BoundIOs &boundIOs);

	// no-throw swap
	void swap(BoundIOs &other);

	BoundIOs &operator=(const BoundIOs &boundIOs);

	// Add a user validation call
	void AddUserValidate(UserValidate &uv)
	{
		vals.push_back(uv);
	}

	void AddUserValidate(const STD_::vector<UserValidate>& vs)
	{
		STD_::copy(vs.begin(), vs.end(), STD_::back_inserter(vals));
	}

	void SetUserValidate(const STD_::vector<UserValidate>& vs)
	{
		vals = vs;
	}


	const STD_::vector<UserValidate> &GetUserValidate()
	{
		return vals;
	}

	void SelUserValidate()
	{
		// Apply any user defined validations/constructors now
		for (STD_::vector<UserValidate>::iterator v = vals.begin(); v != vals.end(); ++v)
		{
			v->sel_val(*this, &*v);
		}
	}

	void SelUserValidateAddr(BYTE *new_addr)
	{
		working_addr = new_addr;
		SelUserValidate();

	}

	void InsUserValidate()
	{
		// Apply any user defined validations/constructors now
		for (STD_::vector<UserValidate>::iterator v = vals.begin(); v != vals.end(); ++v)
		{
			v->ins_val(*this, &*v);
		}
	}

	void InsUserValidateAddr(BYTE *new_addr)
	{
		working_addr = new_addr;
		InsUserValidate();

	}

	void RenameUserValidateColumn(const tstring &old_col, const tstring &new_col)
	{
		for (STD_::vector<UserValidate>::iterator v = vals.begin(); v != vals.end(); ++v)
		{
			for (STD_::vector<tstring>::iterator t = v->bios.begin(); t != v->bios.end(); ++t)
				if (*t == old_col)
					*t = new_col;
		}
	}

	// bind base object so we can compute offsets
    // currently only implemented for indices
	template<class DataObj> void BindAsBase(DataObj &rowbuf)
	{
        // attempting to assign working_type first guarantees exception safety
		working_type = DTL_TYPEID_NAME (rowbuf);
		working_addr = reinterpret_cast<BYTE*>(&rowbuf);
		working_size = sizeof(rowbuf);
	}

	// fix column ordering ... needed if column names lose their zero-indexing
	void FixColNos();

	void BindAsBase(BYTE* addr, size_t size, STD_::string name);

	// returns address of current DataObj / ParamObj we are working with in bca / bpa
	BYTE* GetWorkingAddr();

	// Resets the address of the DataObj we are working with & updates all BoundIOs to
	// this new address
	void SetWorkingAddr(BYTE *addr, TIMESTAMP_STRUCT *pts);

	void SetComplexAddr(BYTE *addr, TIMESTAMP_STRUCT *pts);

	bool HasStrings();

	bool HasCStrings();

	// returns size of current DataObj / ParamObj we are working with in bca / bpa
	size_t GetWorkingObjSize();

	// returns stringified type of current DataObj / ParamObj we are working with in bca / bpa
	STD_::string GetWorkingObjType();

	// you must call this function to bind the base address of the DataObj
	// just prior to calling the
    // return the column with the given name
    BoundIO &operator[](const tstring &colName);

	// return the parameter with the given index
	// (uses similar logic to BoundIO::operator[](tstring)
	BoundIO &operator[](unsigned int paramNum);


	// accessors for BoundColMode ... BIND_ADDRESSES or
	// BIND_ADDRESSES
	// BoundIO::BoundColMode GetBoundColMode();

	// return the names of the columns bound to
	STD_::vector<tstring> GetColumnNames() const;

	// return the # of parameters bound
	int NumParams() const;

	// return the # of columns bound
	int NumColumns() const;

	// # of jtime columns
	int NumJtimes();

	// invalidate all existing columns
	void EraseColumns() ;

	// erase a particular column
	void EraseColumn(tstring &col);

	// return true if any of the parameters are output or input_output parameters
	bool HasOutput() const;

	// propagate all bound STL strings to their strbufs
	// We make this non exception-safe, but that should be O.K. since BoundIOs merely
	// hold temporary buffers anyway. Also, making a temporary copy for exception
	// safety here will kill us performance wise.
	void PropagateToSQL(SQLQueryType sqlQryType, DBStmt &stmt);

	// propagation stuff needed after Execute() for PutData()
	void PropagateToSQLAfterExec(SQLQueryType sqlQryType, DBStmt &stmt);

	// find the BoundIO with the param number passed in
	BoundIO &GetBoundIOforColumn(SQLPOINTER paramNum);

		// propagate results back to the bound STL strings ... done on a SELECT
	// We make this non exception-safe, but that should be O.K. since BoundIOs merely
	// hold temporary buffers anyway. Also, making a temporary copy for exception
	// safety here will kill us performance wise.
	void PropagateFromResults(DBStmt &stmt);

	// returns a BoundIOs object which is the set of BoundIO objects that represent
	// the fields that differ from dataObj1 to dataObj2
	template<class DataObj>
		BoundIOs ChangedFields(const DataObj &dataObj1, BoundIOs const& b2, const DataObj &dataObj2) const
	{
	   // accessor ... exception safe
	   BoundIOs changed;
	   changed.vals = vals;

      // assumption: b2 and *this are basically identical, except for
      // the NULL state differences.
      assert(size() == b2.size());
      BoundIOs::const_iterator it2 = b2.begin();
	   for (const_iterator it = begin(); it != end(); ++it, ++it2)
	   {
		 BoundIO const& boundIO = (*it).second;

		 DataObj const* dataObjPtr1 = &dataObj1;
		 DataObj const* dataObjPtr2 = &dataObj2;

		 if ( (boundIO.IsNullable() && boundIO.IsNull() != (*it2).second.IsNull()) ||
             boundIO.Compare(dataObjPtr1, dataObjPtr2) != 0)
          changed.insert(STD_::pair<const tstring, BoundIO>((*it).first, boundIO));
	   }

	   return changed;
	}


	// set the address & initialize bound variant_row fields
	void BindVariantRow(variant_row &vr);

	void ClearNull();

	// set the field for all primitive columns to NULL
	void InitNullFields();

	// returns true if field name passed in null or doesn't exist
	bool IsNullOrNotExists(const tstring &name);

	// facilities used by LocalBCA
	// operator that generates a BoundIOs structure from an
	// already existing BoundIOs and a raw BoundIO
	BoundIOs operator&&(const BoundIO &addMe);

};


size_t BoundIOs_NumColumns(BoundIOs *pB);

tostream &operator<<(tostream &o, const BoundIOs &bs);

END_DTL_NAMESPACE

#endif



