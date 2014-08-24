/* Copyright 2000
   Michael Gradman and Corwin Joy

   Permission to use, copy, modify, distribute and sell this software and
   its documentation for any purpose is hereby granted without fee,
   provided that the above copyright notice appears in all copies and
   that both that copyright notice and this permission notice appear in
   supporting documentation.  Corwin Joy and Michael Gradman make no
   representations about the suitability of this software for any
   purpose.  It is provided "as is" without express or implied warranty.
*/ 

// this module contains the basic bind operators for binding DB columns
// and SQL parameters to memory structures
// Initial: 9/6/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 11/20/2000 - MG
// Edited: 12/19/2000 - MG - added namespaces
// Edited: 03/24/2004 - Alexander Motzkau, TypeTranslationField remembers variant_row index

#include "bind_basics.h"
#include "variant_row.h"
#include "DBConnection.h"
#include "DBStmt.h"
#include "clib_fwd.h"
#include "DBException.h"
#include "BoundIO.h"
#include "string_util.h"

#include "std_warn_off.h"

#ifdef DTL_NATIVE_WIN32

	#ifdef DTL_USE_MFC

		#include <afx.h>

	#else

		#include <windows.h>

	#endif

#endif

#include <sql.h>
#include <sqlext.h>

#include <algorithm>
#include <numeric>

#ifdef __BORLANDC__
#include <math.h>
#else
#include <cmath>
#endif

#include "std_warn_on.h"

// #define DTL_VARIANT_USE_FIXED_LEN_STRING
//
// Some ODBC drivers may not support SQLGetData extensions in that this function
// can only be called on the last columns in a SELECT statement.
// Usually this is not a problem since DynamicDBView::select_iterator
// arranges things so that string columns are the last columns.
// However, in the case of DynamicDBView::sql_iterator we "trust the programmer"
// and run the sql statement as given.  This can cause an error if the columns
// are not arranged the way the Oracle ODBC driver requires.  For this reason
// we offer the above #define which makes DynamicDBView use a fixed length string
// for holding VARCHAR type columns.  This makes the column order not matter, but
// some long strings may be truncated.
// For details on the SQLGetData restriction see
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/odbc/htm/odch10pr_11.asp
/*
There are a number of restrictions on using SQLGetData. In general, columns accessed with SQLGetData: 

Must be accessed in order of increasing column number (because of the way the columns of a result set are read from the data source). For example, it is an error to call SQLGetData for column 5 and then call it for column 4. 
Cannot be bound. 

Must have a higher column number than the last bound column. For example, if the last bound column is column 3, it is an error to call SQLGetData for column 2. For this reason, applications should be careful to place long data columns at the end of the select list. 
Cannot be used if SQLFetch or SQLFetchScroll was called to retrieve more than one row. For more information, see Using Block Cursors. 

*/


BEGIN_DTL_NAMESPACE

USING_STD_NAMESPACE

void debug_me() {
  // dummy function to invoke debugger trap
  int i = 7;
  i += 0; // get rid of "unused variable" warning
}

// remove all occurrences of the word "const" in the string representing a type
// this should only be used for field types as this naive implementation
// will remove "const" from identifiers that bear this substring
STD_::string StripConstFromTypename(const STD_::string &typeNm)
{
  // typenames are nonportable on different compilers
  // we must strip the constness differently based on each

  STD_::string const_in_name = "const";

  STD_::string::size_type len_of_const = const_in_name.length();

  STD_::string::size_type idx_found = 0;

  STD_::string::size_type last_idx_found = STD_::string::npos;

  STD_::string constlessTypeNm;

  constlessTypeNm.reserve(typeNm.size());

  // while there are possibly more instances of "const" in the string ...
  while (idx_found != STD_::string::npos)
    {
      // search for next instance of "const" and copy string up until that point

      STD_::string::size_type start_search_idx =
	(last_idx_found == STD_::string::npos ? 0 :
	 last_idx_found + len_of_const);

      idx_found = typeNm.find(const_in_name, start_search_idx);

      // compute arguments to typeNm.substr()

      // if first time in loop, copy from beginning of typeNm
      // else start copying after the last appearance of "const"
      STD_::string::size_type start_cpy_idx =
	(last_idx_found == STD_::string::npos ? 0 : last_idx_found + len_of_const);

      // must adjust start_cpy_idx to first nonspace character
      while(std_isspace(typeNm[start_cpy_idx]) && start_cpy_idx < typeNm.length())
	{
	  ++start_cpy_idx;
	}

      if (start_cpy_idx >= typeNm.length())
	break;

      // if last occurrence of "const" found in last iteration
      // we append the remainder of the typeNm string onto the constless type name
      if (idx_found == STD_::string::npos)
	{
	  constlessTypeNm.append(typeNm.substr(start_cpy_idx));
	  break;
	}

      // otherwise, we must append the part of the string between the occurrences of "const"
      STD_::string::size_type num_chars = idx_found - start_cpy_idx;

      constlessTypeNm.append(typeNm.substr(start_cpy_idx, num_chars));

      // update the last index found variable to be all set for next iteration
      last_idx_found = idx_found;
    }

  return constlessTypeNm;
}

// ************* Implementation code for ETIException ************
ETIException::ETIException(const tstring &meth, const tstring &msg) : 
  RootException(meth, msg, _TEXT("ETIException")) { }

// ************* Implementation code for TypeTranslation **********
TypeTranslation::TypeTranslation() : typeNm(""), typeId(0), sqlType(0), cType(0), complexity(TYPE_INVALID), size(0), bParam(false) { }

TypeTranslation::TypeTranslation(const STD_::string &nm, char id, SQLSMALLINT sql, SQLSMALLINT c,
				 TypeTranslation::TypeComplexity comp, size_t s) : typeNm(nm), typeId(id), sqlType(sql), cType(c),
										   complexity(comp), size(s), bParam(false) { }

// exception-safe swap()
void TypeTranslation::swap(TypeTranslation &other)
{
  typeNm.swap(other.typeNm);
  STD_::swap(typeId, other.typeId);
  STD_::swap(sqlType, other.sqlType);
  STD_::swap(cType, other.cType);
  STD_::swap(complexity, other.complexity);
  STD_::swap(size, other.size);
  STD_::swap(bParam, other.bParam);
}

// exception-safe assignment
TypeTranslation &TypeTranslation::operator=(const TypeTranslation &other)
{
  if (this != &other)
    {
      TypeTranslation temp(other);
      swap(temp);
    }

  return *this;
}

// is this a primitive type
bool TypeTranslation::IsPrimitive() const { return complexity == TYPE_PRIMITIVE; }
bool TypeTranslation::IsParam() const { return bParam;};
void TypeTranslation::SetParam(bool p) {bParam = p;}

bool operator==(const TypeTranslation &tt1, const TypeTranslation &tt2)
{
  return tt1.typeNm == tt2.typeNm && tt1.typeId == tt2.typeId &&
    tt1.sqlType == tt2.sqlType && tt1.cType == tt2.cType &&
    tt1.complexity == tt2.complexity && tt1.size == tt2.size;
}

bool operator!=(const TypeTranslation &tt1, const TypeTranslation &tt2)
{
  return !(tt1 == tt2);
}

// ****************** Implementation code for ETI_Map **************************

// this version of buildETI() is based on the old Pricer's type lists
// probably will want to switch off of the platform to make the map appropriate for it

ETI_Map::ETI_Map() : STD_::map<STD_::string, TypeTranslation>()
{
  build();
}

ETI_Map::ETI_Map(const ETI_Map &other) : STD_::map<STD_::string, TypeTranslation>(other)
{ }

// does the stringified type represent a primitive?
// primitive is defined as being in the map
// if not in map, we assume the type is not primitive
bool ETI_Map::IsPrimitive(const STD_::string &typeNm) const
{
  ETI_Map::const_iterator e_it = this->find(typeNm);

  // type not found --> it's not primitive
  if (e_it == end())
    return false;

  // check the ETI for this type and return whether it's
  // primitive or not
  return ((*e_it).second.IsPrimitive());
}

ETI_Map::iterator ETI_Map::find_by_typeid(char type)
{
  for (iterator map_it = begin(); map_it != end(); ++map_it)
    {
      if (type == map_it->second.typeId)
	return map_it;
    }

  return end();
}

// types supported: short, unsigned short, int, unsigned int, long, unsigned long
//					double, float, struct TIMESTAMP_STRUCT, TCHAR *, tstring
// (added bool 3/15/2001)
// (note: wherever it states we support TCHAR *'s, we actually support
// just TCHAR [] ... character arrays ... trying to actually use TCHAR *'s with
// any of our code will lead to undefined behavior

void ETI_Map::build()
{
  // add to these mappings as needed ...
  (*this)[typeid(short).name()] =
    TypeTranslation(typeid(short).name(), C_SHORT, SQL_INTEGER, SQL_C_SSHORT, TypeTranslation::TYPE_PRIMITIVE, sizeof(short));
  (*this)[typeid(unsigned short).name()] =
    TypeTranslation(typeid(unsigned short).name(), C_USHORT, SQL_INTEGER, SQL_C_USHORT, TypeTranslation::TYPE_PRIMITIVE, sizeof(unsigned short));
  (*this)[typeid(bool).name()] =
    TypeTranslation(typeid(bool).name(), C_BOOL, SQL_INTEGER, SQL_C_BIT, TypeTranslation::TYPE_PRIMITIVE, sizeof(bool));
  (*this)[typeid(int).name()] =
    TypeTranslation(typeid(int).name(), C_INT, SQL_INTEGER, SQL_C_SLONG, TypeTranslation::TYPE_PRIMITIVE, sizeof(int));
  (*this)[typeid(unsigned int).name()] =
    TypeTranslation(typeid(unsigned int).name(), C_UINT, SQL_INTEGER, SQL_C_ULONG, TypeTranslation::TYPE_PRIMITIVE, sizeof(unsigned int));
  (*this)[typeid(long).name()] =
    TypeTranslation(typeid(long).name(), C_LONG, SQL_INTEGER, SQL_C_SLONG, TypeTranslation::TYPE_PRIMITIVE, sizeof(long));
  (*this)[typeid(unsigned long).name()] =
    TypeTranslation(typeid(unsigned long).name(), C_ULONG, SQL_INTEGER, SQL_C_ULONG, TypeTranslation::TYPE_PRIMITIVE, sizeof(unsigned long));
  (*this)[typeid(ODBCINT64).name()] =
	  TypeTranslation(typeid(ODBCINT64).name(), C_INT64, SQL_BIGINT, SQL_C_SBIGINT, TypeTranslation::TYPE_PRIMITIVE, sizeof(ODBCINT64));
  (*this)[typeid(double).name()] =
    TypeTranslation(typeid(double).name(), C_DOUBLE, SQL_DOUBLE, SQL_C_DOUBLE, TypeTranslation::TYPE_PRIMITIVE, sizeof(double));
  (*this)[typeid(float).name()] = TypeTranslation(typeid(float).name(), C_FLOAT, SQL_REAL, SQL_C_FLOAT, TypeTranslation::TYPE_PRIMITIVE, sizeof(float));

  (*this)[typeid(struct tagTIMESTAMP_STRUCT).name()] =
    TypeTranslation(typeid(struct tagTIMESTAMP_STRUCT).name(), C_TIMESTAMP, SQL_TIMESTAMP, SQL_C_TIMESTAMP, TypeTranslation::TYPE_PRIMITIVE, sizeof(struct tagTIMESTAMP_STRUCT));
  
  (*this)[typeid(struct tagDATE_STRUCT).name()] =
    TypeTranslation(typeid(struct tagDATE_STRUCT).name(), C_DATE, SQL_DATE, SQL_C_DATE, TypeTranslation::TYPE_PRIMITIVE, sizeof(struct tagDATE_STRUCT));
  
  (*this)[typeid(struct tagTIME_STRUCT).name()] =
    TypeTranslation(typeid(struct tagTIME_STRUCT).name(), C_TIME, SQL_TIME, SQL_C_TIME, TypeTranslation::TYPE_PRIMITIVE, sizeof(struct tagTIME_STRUCT));
  
  (*this)[typeid(char *).name()] =
    TypeTranslation(typeid(char *).name(), C_CHAR_STAR, SQL_VARCHAR, SQL_C_CHAR, TypeTranslation::TYPE_PRIMITIVE, BoundIO::MINSTRBUFLEN * sizeof(TCHAR));

#ifndef DTL_NO_UNICODE
  (*this)[typeid(wchar_t *).name()] =
    TypeTranslation(typeid(wchar_t *).name(), C_WCHAR_STAR,
		    SQL_WVARCHAR, SQL_C_WCHAR, TypeTranslation::TYPE_PRIMITIVE, sizeof(wchar_t *));
#endif

  (*this)[typeid(STD_::string).name()] =
    TypeTranslation(typeid(STD_::string).name(), C_STRING, SQL_VARCHAR, SQL_C_CHAR, TypeTranslation::TYPE_COMPLEX, sizeof(STD_::string));

  (*this)[typeid(blob).name()] =
    TypeTranslation(typeid(blob).name(), C_BLOB, SQL_VARBINARY, SQL_C_BINARY, TypeTranslation::TYPE_COMPLEX, sizeof(blob));

  
  (*this)[typeid(jtime_c).name()] =
    TypeTranslation(typeid(jtime_c).name(), C_JTIME_C, SQL_TIMESTAMP, SQL_C_TIMESTAMP, TypeTranslation::TYPE_COMPLEX, sizeof(jtime_c));

#if 0
  // Write wide strings to database as ASCII strings
  // This is useful for e.g. Oracle 8.16 and earlier which do not support
  // wide varchar columns.  However, this is a hack, so really it is just
  // better not to try to write wide error strings to databases that don't
  // support them, and if you do try you should get an error.

  (*this)[typeid(STD_::wstring).name()] =
    TypeTranslation(typeid(STD_::wstring).name(), C_WSTRING, SQL_VARCHAR, SQL_C_WCHAR, TypeTranslation::TYPE_COMPLEX, sizeof(STD_::wstring));
#else
#ifndef DTL_NO_UNICODE
  // DLP: added wstring 6/12/2001
  // Write wide string as a wide varchar column
  (*this)[typeid(STD_::wstring).name()] =
    TypeTranslation(typeid(STD_::wstring).name(), C_WSTRING, SQL_WVARCHAR, SQL_C_WCHAR, TypeTranslation::TYPE_COMPLEX, sizeof(STD_::wstring));
#endif
#endif


#ifdef UNICODE
  (*this)[typeid(fake_tcstring).name()] =
	  TypeTranslation(typeid(fake_tcstring).name(), C_TCSTRING, SQL_WVARCHAR, SQL_C_WCHAR, TypeTranslation::TYPE_PRIMITIVE, 0);
#else
  (*this)[typeid(fake_tcstring).name()] =
    TypeTranslation(typeid(fake_tcstring).name(), C_TCSTRING, SQL_VARCHAR, SQL_C_CHAR, TypeTranslation::TYPE_PRIMITIVE, 0);
#endif



}

// ETI_Map SQL_types_to_C = ETI_Map::BuildETI();

// returns the type translation map used by the library
// implemented as a local static in a function rather than a global variable
// to enforce proper initialization order within global variables that must use the map
// to initialize themselves
ETI_Map &GetSQL_types_to_C()
{
  static ETI_Map eti_map;
  return eti_map;
}

const int TYPE_NOT_SUPPORTED = -1000;

// The next function is used by DYNAMIC queries with variant datatypes to
// map SQL column types from SQLGetTypeInfo into target C types to hold values from the columns
// The default mappings shown here were guided by the default conversion table
// for ODBC 3.0 -- Microsoft ODBC 3.0 Programmer's Reference, vol. 2, p. 1218
// wide TCHAR types are M$ specific

// DLP: changed SQL_WCHAR/SQL_WVARCHAR to map to std::wstring 12/6/2001

TypeTranslation Map_SQL_types_to_C(SWORD sql_type) {
  ETI_Map &SQL_types_to_C = GetSQL_types_to_C();
  switch (sql_type) {
  case SQL_CHAR: case SQL_VARCHAR: case SQL_LONGVARCHAR:
#ifdef DTL_NO_UNICODE
  case SQL_WCHAR: case SQL_WVARCHAR: case SQL_WLONGVARCHAR: // cast wide strings to narrow strings	
#endif
# ifdef DTL_VARIANT_USE_FIXED_LEN_STRING
	  return SQL_types_to_C[typeid(char *).name()];
#else
      return SQL_types_to_C[typeid(tstring).name()];
#endif

#ifndef DTL_NO_UNICODE
  case SQL_WCHAR: case SQL_WVARCHAR: case SQL_WLONGVARCHAR:
  # ifdef DTL_VARIANT_USE_FIXED_LEN_STRING
	  return SQL_types_to_C[typeid(char *).name()]; // cast wide strings to fixed length ASCII string
  #else
	  return SQL_types_to_C[typeid(STD_::wstring).name()];
  #endif
#endif

  case SQL_DECIMAL: return SQL_types_to_C[typeid(double).name()];
  case SQL_NUMERIC: return SQL_types_to_C[typeid(double).name()];
  case SQL_BIT: return SQL_types_to_C[typeid(short).name()];
  case SQL_TINYINT: return SQL_types_to_C[typeid(short).name()];

  case SQL_SMALLINT: return SQL_types_to_C[typeid(int).name()];
  case SQL_INTEGER: return SQL_types_to_C[typeid(long).name()];
  case SQL_BIGINT: return SQL_types_to_C[typeid(ODBCINT64).name()];
  case SQL_REAL: return SQL_types_to_C[typeid(float).name()];
  case SQL_FLOAT: case SQL_DOUBLE: return SQL_types_to_C[typeid(double).name()];
  case SQL_BINARY: case SQL_VARBINARY: case SQL_LONGVARBINARY: return SQL_types_to_C[typeid(blob).name()];
  case SQL_GUID: return SQL_types_to_C[typeid(tstring).name()];
  case SQL_DATE: case SQL_TIME: case SQL_TIMESTAMP: 
  case SQL_TYPE_DATE:
  case SQL_TYPE_TIMESTAMP: 
    return SQL_types_to_C[typeid(struct tagTIMESTAMP_STRUCT).name()];
  default: {
    tstring errmsg;
    errmsg.reserve(512);
    errmsg += _TEXT("Unknown SQL column type ");
	tostringstream tstr;
	tstr << sql_type;
	errmsg += tstr.str();
    errmsg += _TEXT(" encountered in Map_SQL_types_to_C()!");
    DTL_THROW ETIException(_TEXT("Map_SQL_types_to_C()"), errmsg);

# ifdef DTL_VARIANT_USE_FIXED_LEN_STRING
	  return SQL_types_to_C[typeid(char *).name()];
#else
      return SQL_types_to_C[typeid(tstring).name()];
#endif
  }
  }
}

static STD_::vector<tstring> parse_function_list (const tstring& s, const tstring& q = _TEXT ("\""))
{
  STD_::vector<tstring> r;
  tstring::size_type pos = 0, lp = 0, rp = 0, x = s.size ( );

  for (tstring::size_type i = 0; i != x; ++i) {
    // This is a cheap hack.  Real handling of SQL string rules needs
    // a lexer/parser soluction.  Example problems: no way to escape
    // the escape character itself; no handling of alternative quote
    // marks; no escape sequences.  XXX

    if (s.find (q, i) == i) {
      do {
		if ((i = s.find (q, ++i)) == tstring::npos)
		  DTL_THROW RootException
			(_TEXT ("parse_function_list (const tstring&)"),
			 _TEXT ("no ending quote mark"));
	  } while (s[i - 1] == '\\');

	  continue;
	}

    switch (s[i]) {
    case tstring::value_type (','):
      if (lp == rp) {
	r.push_back (s.substr (pos, i - pos));
	trim (r.back ( ));
	pos = i + 1;
      }
      break;
    case tstring::value_type ('('): ++lp; break;
    case tstring::value_type (')'): ++rp; break;
    }
  }

  if (lp != rp)
    DTL_THROW RootException
      (_TEXT ("parse_function_list (const tstring&)"),
       _TEXT ("mismatched function parentheses"));

  // Last column doen't end with quote; don't forget it.
  r.push_back (s.substr (pos));
  trim (r.back ( ));

  return r;
}

// Pre-pend table names if there are duplicate field names
// This is primarily to resolve the problem of obtaining
// unqualified field names from the expansion of *.
// So, e.g. 
// select t1.*, t2.* from t1, t2 
// may give duplicate field names
void resolve_duplicates (DBConnection& conn, DBStmt &stmt, STD_::vector<tstring> &fields)
{
	STD_::vector<tstring> sorted_fields(fields);
	STD_::sort(sorted_fields.begin(), sorted_fields.end());
	bool b_duplicate = false;
	STD_::vector<tstring>::iterator it = sorted_fields.begin();
	++it;
	for (; it != sorted_fields.end(); ++it)
	{
		if (*it == *(it-1))
		{
			b_duplicate = true;
			break;
		}
	}

	if (!b_duplicate)
		return;

	
	// Pre-pend table names if there are duplicate field names
	const int MAX_COLNAME = 50;
	RETCODE rc;
	TCHAR szTableName[MAX_COLNAME + 1];
	SQLSMALLINT cbTableName;
	SQLLEN dummy;
	HSTMT hstmt = GetHSTMT(stmt);
	int i;
	tstring tmp, dot = _TEXT (".");
	for (it = fields.begin(), i = 1; it != fields.end(); ++it, ++i)
	{
		if (it->find (dot, 0) != tstring::npos)
			continue; // field already has a table qualifier

		rc = SQLColAttribute (hstmt, i, SQL_DESC_TABLE_NAME,
			reinterpret_cast<SQLTCHAR*>(szTableName), MAX_COLNAME, &cbTableName,
			 &dummy);
		if (!RC_SUCCESS (rc))
		DTL_THROW DBException (tstring (_TEXT ("resolve_duplicates()")),
		   tstring (_TEXT ("Have duplicate field names. Unable to get table name associated with field to resolve ambiguity.")),
		   &conn, &stmt);
		if (szTableName[0] != 0) 
		{
			tmp = szTableName;
			*it = tmp + _TEXT(".") + *it;
		}
	}

	// We may still have duplicate columns that have the same table and field name
	// In this case, alias these columns with _column_number to make them distinct
	sorted_fields = fields;
	STD_::sort(sorted_fields.begin(), sorted_fields.end());
	it = sorted_fields.begin();
	++it;
	for (;it != sorted_fields.end(); ++it)
	{
		if (*it == *(it-1))
		{
			STD_::vector<tstring>::iterator target;
			while ((target = STD_::find(fields.begin(), fields.end(), *it)) != fields.end())
			{
				tostringstream colname;
				colname << _TEXT("_") << (target - fields.begin()) + 1; 
				tstring::size_type dot_pos = target->find(dot, 0);
				tstring field_name;
				if (dot_pos != tstring::npos)
					field_name = tstring(*target, dot_pos+1, tstring::npos);
				else
					field_name = *target;
				field_name = *target + tstring(_TEXT(" AS ")) + field_name + tstring(colname.str());
				*target = field_name;
			}
		}
	}

	

}

void DescColsOrParams(DBConnection& conn, DBStmt &stmt, STD_::vector<TypeTranslation> &vtt, 
					  tstring& TableFields, STD_::vector<tstring> &fields, 
					  STD_::vector<tstring> &columns, bool get_cols, HSTMT *p_have_stmt = NULL)
{
  const int MAX_COLNAME = 50;
  SWORD cCols = 0;
  TCHAR szColName[MAX_COLNAME + 1];
  SQLSMALLINT cbColName;
  SQLSMALLINT fSQLType;
  
  SQLSMALLINT cbScale;
  SQLSMALLINT fNullable;
  SQLULEN cbPrec;
  RETCODE rc;
  TypeTranslation tt;
  HSTMT hstmt;
  if (p_have_stmt == NULL)
	  hstmt = GetHSTMT(stmt);
  else
	  hstmt = *p_have_stmt;

  if (get_cols)
	rc = SQLNumResultCols (hstmt, &cCols);
  else
	rc = SQLNumParams(hstmt, &cCols);

  if (!RC_SUCCESS (rc))
    DTL_THROW DBException (tstring (_TEXT ("DescColsOrParams()")),
		       tstring (_TEXT ("Unable to get number of fields or parameters")),
		       &conn, &stmt);

  for (int i = 1; i <= cCols; ++i) {

	fSQLType = 0;
	szColName[0] = 0;
	if (get_cols) 
	{
		rc = SQLDescribeCol (hstmt, i, reinterpret_cast<SQLTCHAR*>(szColName), MAX_COLNAME, &cbColName,
			 &fSQLType, &cbPrec, &cbScale, &fNullable);

	}
	else
	{
		rc = SQLDescribeParam (hstmt, i, &fSQLType, &cbPrec, &cbScale, &fNullable);
	}
	
	if (!RC_SUCCESS (rc)) {
		if (!get_cols) {
		  // Driver won't tell us parameter type
		  // Bind paramater type as string.  ODBC should convert to target data type on execution.
		  fSQLType = SQL_VARCHAR;  
		} else {
		  DTL_THROW DBException (tstring (_TEXT ("DescColsOrParams()")),
			 tstring (_TEXT ("Unable to get field metadata")),
			 &conn, &stmt);
		}
	}


    tt = Map_SQL_types_to_C(fSQLType);

	if (!get_cols)
		tt.SetParam(true);

    vtt.push_back (tt);

	if (get_cols)
	{
		// N.B. we need to let the SQL parser determine the field names
		// here since complex parsing may be involved to parse functions
		// such as TO_DATE('DD-MM-YYYY', field_name) to a single field
		// name

		// Use what the user passed in, not with ODBC thinks.

		// if user passes in '*' for first field name, get field names from DB
		// otherwise use the field list from user
		if (static_cast<size_t>(cCols) == columns.size() &&
			TableFields.size() > 0 && TableFields[0] != _TEXT('*'))
		{
			fields.push_back (columns[i - 1]);
		}
		else
		{
			if (szColName[0] == 0) 
			{
			  // Even if the driver does not give us a column name we may still be able
			  // to execute in the case of sql_iterator
			  if (TableFields.empty ( )) 
			  {
				  tostringstream colname;
				  colname << _TEXT("?") << i;  // name column "?i"
				  fields.push_back (colname.str());
			  }
			  else
			  {
				  tostringstream errstr;

				  errstr << _TEXT ("Unable to get description for column ") << i
					 << _TEXT (". If you are using a SQL expression such as count(*), sum(...), max(...), 1,  etc. in your ")
					 << _TEXT ("DynamicDBView then you may need to use a sql_iterator with this statement as select, insert and update ")
					 << _TEXT ("may give an error since ODBC does not return a meaningful column name here.");
 
				  tstring errstring = errstr.str();

				  DTL_THROW RootException (tstring (_TEXT ("GetFieldInfo()")), errstring);
			  }

			}
			else
			{
				fields.push_back (tstring (static_cast<TCHAR*>(szColName)));
			}

		}
	}
	else
	{
		// field name is stringified column number
		tostringstream colstr;
		colstr << i-1;
		fields.push_back(colstr.str());
	}
  }

  if (cCols > 0)
	resolve_duplicates (conn, stmt, fields);
}

// look for ' JOIN ' or ',' in the string.
// note: passed as a copy so i can tolower it below
bool OnlyOneTable (tstring TableName)
{
   // look for a ,
	if (STD_::find(TableName.begin(),TableName.end(),',') != TableName.end() )
      return false;

   // look for ' JOIN ' (not case sensitive, including the spaces)
   // make the string lowercase.
   lower_caseify(TableName.begin(),TableName.end());
   if (TableName.find(_TEXT(" join ")) != tstring::npos)
      return false;

   return true;
}

// Analyze a given table and fields to get information about the types
// of the fields in the table Return
variant_row_fields
GetFieldInfo (const tstring& TableName, tstring& TableFields,
	      const tstring& Postfix, tstring& UniqueFields,
	      STD_::vector<tstring>& AppendedFields, 
	      KeyMode keyMode, DBConnection& conn, HSTMT *p_have_stmt = NULL)
{
  SQLSMALLINT fSQLType;
  tstring szSQL;

  typedef STD_::vector<tstring> tslist_t;
  tslist_t columns;

  if (TableFields.empty ( )) {
    // assume sql_iterator / stored proceedure in the view
    szSQL = TableName;

    if (keyMode == USE_AUTO_KEY)
      keyMode = USE_ALL_FIELDS;
  }

  else {
	if (TableName[0] != TCHAR('_'))
	{
		szSQL = _TEXT ("SELECT ") + TableFields + _TEXT (" FROM ")
		  + TableName + _TEXT (" ") + Postfix;

		// This is the wrong solution, but expedient.  The right thing to
		// do is use a simple SQL parser.  Don't try this: "\\".  XXX
		columns = parse_function_list (TableFields, conn.GetQuoteChar ( ));
	}
	else
	{
		// Table name begins with an underscore.
		// Treat this as a call to a SQL catalog function.
		szSQL = TableName + _TEXT (", ") + TableFields;
	}
  }

  TypeTranslation tt;
  STD_::vector<TypeTranslation> vtt;
  tslist_t fields;

  // only need to prepare the statement to get types, no need to
  // execute. this gives best performance since we don't go through the
  // overhead of actually fetching the data

  DBStmt stmt (szSQL, conn, true,  !TableFields.empty());
  stmt.ClearStmtAttrs();
  if (p_have_stmt == NULL)
  {
	stmt.Initialize ( ); 
  }

  // Have prepared SQL statement, now get information about columns
   
  // Get column list
  DescColsOrParams(conn, stmt, vtt, TableFields, fields, columns, true, p_have_stmt);

  // Get parameter list
  DescColsOrParams(conn, stmt, vtt, TableFields, fields, columns, false, p_have_stmt);

  
  if (keyMode == USE_AUTO_KEY) {
    if (OnlyOneTable(TableName)) {
      // Now call SQLSpecialColumns() to get any key fields, return as
      // vector

      // on errors we just won't add these primary key fields to the
      // variant_row_fields

      // get structures we can work with
      STD_::vector<tstring> table_fields_vec = fields;
      STD_::vector<tstring> unique_fields_vec =
		ParseCommaDelimitedList (UniqueFields);

	  

      tstring tableNmNarrow = 
		tstring_cast (static_cast<tstring*>(NULL), TableName); // Table to display

      if (tableNmNarrow.size() > SQL_MAX_TABLE_NAME_LEN)
	    DTL_THROW ETIException(_TEXT("GetFieldInfo()"), _TEXT("Table name string too long!"));

      // Unused.  XXX
      // const size_t TAB_LEN = SQL_MAX_TABLE_NAME_LEN + 1;
      const size_t COL_LEN = SQL_MAX_COLUMN_NAME_LEN + 1;

      TCHAR szPkCol[COL_LEN];  // Primary key column

      SQLHSTMT spec_hstmt;
      SQLLEN cbPkCol, cbSQLType;
      SQLRETURN retcode = SQLAllocStmt (conn.GetHDBC ( ), &spec_hstmt);

      if (! RC_SUCCESS (retcode))
		return variant_row_fields (vtt, fields);

      // Find the column names that can best give a unique identifier
      // for the row
#ifndef  DTL_NO_UNICODE
      retcode = SQLBindCol (spec_hstmt, 2, SQL_C_WCHAR, szPkCol, COL_LEN,
			    &cbPkCol);
#else
      retcode = SQLBindCol (spec_hstmt, 2, SQL_C_CHAR, szPkCol, COL_LEN,
			    &cbPkCol);
#endif
	
      if (!RC_SUCCESS(retcode)) 
	  {
		SQLFreeHandle (SQL_HANDLE_STMT, spec_hstmt);
		return variant_row_fields (vtt, fields);
      }

      retcode = SQLBindCol (spec_hstmt, 3, SQL_C_SSHORT, &fSQLType,
			    sizeof (fSQLType), &cbSQLType);

      if (! RC_SUCCESS (retcode)) {
		SQLFreeHandle (SQL_HANDLE_STMT, spec_hstmt);

		return variant_row_fields (vtt, fields);
      }

      SQLTCHAR* szTable = const_cast<SQLTCHAR*>(reinterpret_cast<const SQLTCHAR*>(tableNmNarrow.data()));

      // Get the names of the columns in the primary key ...

      // Minimum ROWID, key may become invalid when we move off the
      // current row NB!! When we go to scrollable recordsets we
      // *cannot* depend on this rowid staying valid when move to
      // another row.  Also note that key fields may hold NULL values
      retcode = SQLSpecialColumns
		(spec_hstmt, 
		SQL_BEST_ROWID, 
		NULL, 0, // Catalog name
		NULL, 0, // Schema name
		szTable, static_cast<SQLSMALLINT>(tableNmNarrow.size()), // Table name
		SQL_SCOPE_CURROW, 
		SQL_NULLABLE); // Table name

      if (!RC_SUCCESS(retcode)) 
	  {
		SQLFreeHandle (SQL_HANDLE_STMT, spec_hstmt);
		return variant_row_fields (vtt, fields);
      }

      try {
		  retcode = SQLFetch (spec_hstmt);

		  // loop through the keys and push back onto keys and fields
		  // lists

		  while (RC_SUCCESS(retcode)) {				
			tstring keyNm = static_cast<const TCHAR*>(szPkCol);
			tstring finalKeyNm (tstring_cast (static_cast<tstring*>(NULL), keyNm));

			  // see which lists finalKeyNm is in:
			  // We must do two things here:
			  // 1.  if not in unique keys list, add the key to the list
			  // 2.  if not in table fields list, add the key to that list

			  if (STD_::find (unique_fields_vec.begin ( ),
					  unique_fields_vec.end ( ), finalKeyNm)
				  == unique_fields_vec.end ( ))
				unique_fields_vec.push_back(finalKeyNm);
					
			  if (STD_::find (table_fields_vec.begin ( ),
					  table_fields_vec.end ( ), finalKeyNm)
				  == table_fields_vec.end ( )) {
				table_fields_vec.push_back (finalKeyNm);
				AppendedFields.push_back (finalKeyNm);
			  }

			  else { 
				retcode = SQLFetch (spec_hstmt);

				// nothing to do for this field as already in table fields list
				continue;
			  }

			  // Fetch and display the result set. This will be a list of
			  // the columns in the primary key of the table.

			  tt = Map_SQL_types_to_C (fSQLType);
			  vtt.push_back(tt);

			  // N.B. we need to let the SQL parser determine the field
			  // names here since complex parsing may be involved to parse
			  // functions such as TO_DATE('DD-MM-YYYY', field_name) to a
			  // single field name
			  fields.push_back (finalKeyNm);
			  retcode = SQLFetch (spec_hstmt);
		  }
	  
		  // Close the cursor (the hstmt is still allocated).
	      SQLFreeHandle (SQL_HANDLE_STMT, spec_hstmt);
	  
	  }

      catch (...) {
		SQLFreeHandle (SQL_HANDLE_STMT, spec_hstmt);
		throw;
      }

      // return new unique fields list and table fields list
      UniqueFields = MakeDelimitedList (unique_fields_vec);
      TableFields = MakeDelimitedList (table_fields_vec);
    } // end if size() == 1

    else
      keyMode = USE_ALL_FIELDS;
  } // end if autokey

  return variant_row_fields (vtt, fields);
}

// compare the contents of the pointers of the type specified by CTypeStr
// if (*pMember1 < *pMember2)
//	 return -1;
// else if (*pMember1 == *pMember2)
//   return 0;
// else if (*pMember1 > *pMember2)
//	 return 1;

int GenericCmp(BYTE* pMember1, BYTE* pMember2, int typeId)
{
  // types supported: short, unsigned short, int, unsigned int,
  // long, unsigned long, double, float, struct TIMESTAMP_STRUCT,
  // TCHAR *, tstring, bool, TCHAR, STD_::wstring

  switch(typeId) {
  case C_SHORT: {
    short *pShort1 = reinterpret_cast<short*>(pMember1);
    short *pShort2 = reinterpret_cast<short*>(pMember2);

    if (*pShort1 < *pShort2)
      return -1;
    else
      return (*pShort2 < *pShort1);
  }

  case C_USHORT: {
    unsigned short *pUShort1 =
      reinterpret_cast<unsigned short *>(pMember1);
    unsigned short *pUShort2 =
      reinterpret_cast<unsigned short *>(pMember2);

    if (*pUShort1 < *pUShort2)
      return -1;
    else
      return (*pUShort2 < *pUShort1);
  }

  case C_INT: {
    int *pInt1 = reinterpret_cast<int*>(pMember1);
    int *pInt2 = reinterpret_cast<int*>(pMember2);

    if (*pInt1 < *pInt2)
      return -1;
    else
      return (*pInt2 < *pInt1);
  }

  case C_UINT: {
    unsigned int *pUInt1 = reinterpret_cast<unsigned int *>(pMember1);
    unsigned int *pUInt2 = reinterpret_cast<unsigned int *>(pMember2);

    if (*pUInt1 < *pUInt2)
      return -1;
    else
      return (*pUInt2 < *pUInt1);
  }

  case C_LONG: {
    long *pLong1 = reinterpret_cast<long*>(pMember1);
    long *pLong2 = reinterpret_cast<long*>(pMember2);

    if (*pLong1 < *pLong2)
      return -1;
    else
      return (*pLong2 < *pLong1);
  }

  case C_ULONG: {
    unsigned long *pULong1 = reinterpret_cast<unsigned long *>(pMember1);
    unsigned long *pULong2 = reinterpret_cast<unsigned long *>(pMember2);

    if (*pULong1 < *pULong2)
      return -1;
    else
      return (*pULong2 < *pULong1);
  }

  case C_INT64: {
    ODBCINT64 *pULong1 = reinterpret_cast<ODBCINT64*>(pMember1);
    ODBCINT64 *pULong2 = reinterpret_cast<ODBCINT64*>(pMember2);

    if (*pULong1 < *pULong2)
      return -1;
    else
      return (*pULong2 < *pULong1);
  }

  case C_DOUBLE: {
    double *pDouble1 = reinterpret_cast<double*>(pMember1);
    double *pDouble2 = reinterpret_cast<double*>(pMember2);
    //			const double epsilon = 1 >> 20; // 1/1048576
    const double epsilon = pow(2.0, -20); // 1/1048576
    if (*pDouble1 < *pDouble2 - (fabs(*pDouble1)+fabs(*pDouble2))*epsilon)
      return -1;
    else
      return (*pDouble2 < *pDouble1 - (fabs(*pDouble1)+fabs(*pDouble2))*epsilon);
  }

  case C_FLOAT: {
    float *pFloat1 = reinterpret_cast<float*>(pMember1);
    float *pFloat2 = reinterpret_cast<float*>(pMember2);
    //			const double epsilon = 1 >> 20; // 1/1048576
    const double epsilon = pow(2.0, -20); // 1/1048576

    if (*pFloat1 < *pFloat2 - (fabs(*pFloat1)+fabs(*pFloat2))*epsilon)
      return -1;
    else
      return (*pFloat2 < *pFloat1 - (fabs(*pFloat1)+fabs(*pFloat2))*epsilon);
  }

  case C_TIMESTAMP: {
    struct tagTIMESTAMP_STRUCT *pTS1 =
      reinterpret_cast<struct tagTIMESTAMP_STRUCT *>(pMember1);
    struct tagTIMESTAMP_STRUCT *pTS2 = 
      reinterpret_cast<struct tagTIMESTAMP_STRUCT *>(pMember2);

    return TSCmp(*pTS1, *pTS2);
  }

  case C_DATE: {
    struct tagDATE_STRUCT *pTS1 =
      reinterpret_cast<struct tagDATE_STRUCT *>(pMember1);
    struct tagDATE_STRUCT *pTS2 = 
      reinterpret_cast<struct tagDATE_STRUCT *>(pMember2);

    return DCmp(*pTS1, *pTS2);
  }

  case C_TIME: {
    struct tagTIME_STRUCT *pTS1 =
      reinterpret_cast<struct tagTIME_STRUCT *>(pMember1);
    struct tagTIME_STRUCT *pTS2 = 
      reinterpret_cast<struct tagTIME_STRUCT *>(pMember2);

    return TCmp(*pTS1, *pTS2);
  }

  case C_CHAR_STAR: {
    char *pCStr1 = reinterpret_cast<char*>(pMember1);
    char *pCStr2 = reinterpret_cast<char*>(pMember2);

    return std_strcmp(pCStr1, pCStr2);
  }

#ifndef DTL_NO_UNICODE

  case C_WCHAR_STAR: {
    wchar_t *pCStr1 = reinterpret_cast<wchar_t *>(pMember1);
    wchar_t *pCStr2 = reinterpret_cast<wchar_t *>(pMember2);

    return std_wcscmp(pCStr1, pCStr2);
  }
#endif
  case C_BOOL: {
    bool *pBool1 = reinterpret_cast<bool*>(pMember1);
    bool *pBool2 = reinterpret_cast<bool*>(pMember2);

    if (*pBool1 < *pBool2)
      return -1;
    else
      return (*pBool2 < *pBool1);
  }

  case C_STRING: {
    STD_::string *pStr1 = (STD_::string *) pMember1;
    STD_::string *pStr2 = (STD_::string *) pMember2;

    return pStr1->compare(*pStr2);
  }

  case C_BLOB: {
    blob *pStr1 = reinterpret_cast<blob*>(pMember1);
    blob *pStr2 = reinterpret_cast<blob*>(pMember2);

    return pStr1->compare(*pStr2);
  }

#ifndef DTL_NO_UNICODE
    // DLP: added for std::wstring 12/6/2001
  case C_WSTRING: {
    STD_::wstring *pStr1 = (STD_::wstring *) pMember1;
    STD_::wstring *pStr2 = (STD_::wstring *) pMember2;

    return pStr1->compare(*pStr2);
  }
#endif

  case C_JTIME_C: {
    jtime_c *pTS1 = reinterpret_cast<jtime_c *>(pMember1);
    jtime_c *pTS2 = reinterpret_cast<jtime_c *>(pMember2);

    if (*pTS1 < *pTS2)
      return -1;
    else
      return (*pTS2 < *pTS1);
  }

  case C_TCSTRING: {
    fake_tcstring* pStr1 = reinterpret_cast<fake_tcstring*>(pMember1);
    fake_tcstring* pStr2 = reinterpret_cast<fake_tcstring*>(pMember2);

    return std_tstrcmp(pStr1->buf, pStr2->buf);
  }

  default:
    DTL_THROW ETIException(_TEXT("GenericCmp()"), _TEXT("Unable to compare objects of this type!"));
	return 0;
  }
}

// generic hash function
size_t GenericHash(BYTE* pMember1, int typeId)
{
  // types supported: short, unsigned short, int, unsigned int,
  // long, unsigned long, double, float, struct TIMESTAMP_STRUCT,
  // TCHAR *, tstring, bool, TCHAR, STD_::wstring

  switch(typeId) {
  case C_SHORT: {
    short *pShort1 = reinterpret_cast<short*>(pMember1);

    return ((size_t) *pShort1);
  }

  case C_USHORT: {
    unsigned short *pUShort1 =
      reinterpret_cast<unsigned short *>(pMember1);

    return ((size_t) *pUShort1);
  }

  case C_INT: {
    int *pInt1 = reinterpret_cast<int*>(pMember1);

    return ((size_t) *pInt1);
  }

  case C_UINT: {
    unsigned int *pUInt1 = reinterpret_cast<unsigned int *>(pMember1);

    return ((size_t) *pUInt1);
  }

  case C_LONG: {
    long *pLong1 = reinterpret_cast<long*>(pMember1);

    return ((size_t) *pLong1);
  }

  case C_ULONG: {
    unsigned long *pULong1 = reinterpret_cast<unsigned long *>(pMember1);

    return ((size_t) *pULong1);
  }

  case C_INT64: {
    ODBCINT64 *pULong1 = reinterpret_cast<ODBCINT64*>(pMember1);

    return ((size_t) *pULong1);
  }

  case C_DOUBLE: {
    double *pDouble1 = reinterpret_cast<double*>(pMember1);

    // const double epsilon = 1 >> 20; // 1/1048576

    return ((size_t) *pDouble1);
  }

  case C_FLOAT: {
    float *pFloat1 = reinterpret_cast<float*>(pMember1);

    // const double epsilon = 1 >> 20; // 1/1048576

    return ((size_t) *pFloat1);
  }

  case C_TIMESTAMP: {
    struct tagTIMESTAMP_STRUCT *pTS1 =
      reinterpret_cast<struct tagTIMESTAMP_STRUCT *>(pMember1);

    return size_t(jtime_t(jtime_c(*pTS1)));
  }

  case C_DATE: {
    struct tagDATE_STRUCT *pTS1 =
      reinterpret_cast<struct tagDATE_STRUCT *>(pMember1);

    // Assumes the hash doesn't need to generate unique hashes
    assert(sizeof(size_t) <= sizeof(struct tagDATE_STRUCT));

    return *reinterpret_cast<size_t*>(pTS1);
  }

  case C_TIME: {
    struct tagTIME_STRUCT *pTS1 =
      reinterpret_cast<struct tagTIME_STRUCT *>(pMember1);

    // Assumes the hash doesn't need to generate unique hashes
    assert(sizeof(size_t) <= sizeof(struct tagTIME_STRUCT));

    return *reinterpret_cast<size_t*>(pTS1);
  }

  case C_CHAR_STAR: {
    char *pCStr1 = reinterpret_cast<char*>(pMember1);
    return ((size_t) STD_::accumulate(pCStr1, pCStr1 + std_strlen(pCStr1), 1, STD_::multiplies<int>()));
  }

  case C_WCHAR_STAR : {
    wchar_t *pCStr1 = reinterpret_cast<wchar_t *>(pMember1);
    return ((size_t) STD_::accumulate(pCStr1, pCStr1 + std_wcslen(pCStr1), 1, STD_::multiplies<int>()));
  }

  case C_BOOL: {
    bool *pBool1 = reinterpret_cast<bool*>(pMember1);

    return ((size_t) *pBool1);
  }

  case C_STRING: {

    STD_::string *pStr1 = (STD_::string *) pMember1;

    return ((size_t) STD_::accumulate(pStr1->begin(), pStr1->end(), 1, STD_::multiplies<int>()));
  }

  case C_BLOB: {
    blob *pStr1 = reinterpret_cast<blob*>(pMember1);

    return ((size_t) STD_::accumulate(pStr1->begin(), pStr1->end(), 1, STD_::multiplies<int>()));
  }

#ifndef DTL_NO_UNICODE

    // DLP: added for std::wstring 12/6/2001
  case C_WSTRING: {
    STD_::wstring *pStr1 = (STD_::wstring *) pMember1;

    return ((size_t) STD_::accumulate(pStr1->begin(), pStr1->end(), 1, STD_::multiplies<int>()));
  }

#endif

  case C_JTIME_C: {
    jtime_c *pTS1 = reinterpret_cast<jtime_c *>(pMember1);

    return size_t(jtime_t(*pTS1));
  }

  case C_TCSTRING: {
    fake_tcstring* pStr1 = reinterpret_cast<fake_tcstring*>(pMember1);

    return ((size_t) STD_::accumulate
	    (pStr1->buf, pStr1->buf + std_tstrlen(pStr1->buf), 1,
	     STD_::multiplies<int> ( )));
  }

  default:
    DTL_THROW ETIException(_TEXT("GenericHash()"), _TEXT("Unable to hash objects of this type!"));
	return 0;
  }
}


END_DTL_NAMESPACE
