/* Copyright ? 2000 
Michael Gradman and Corwin Joy 

Permission to use, copy, modify, distribute and sell this software and 
its documentation for any purpose is hereby granted without fee, provided 
that the above copyright notice appears in all copies and that both that 
copyright notice and this permission notice appear in supporting documentation. 
Corwin Joy and Michael Gradman make no representations about the suitability 
of this software for any purpose. 
It is provided "as is" without express or implied warranty.
*/ 
// this class abstracts ODBC statements and their operations
// Initial: 9/5/2000-9/6/2000 - MG
// Revised: 11/12/2000 - CJ - fixed exception handling in Initialize()
// Revised: 11/20/2000 - MG - minor changes to expose proper methods
//							  so we can throw DBExceptions in GetFieldInfo()
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_DBSTMT_H
#define DTL_DBSTMT_H

#include "DB_Base.h"
#include "DBConnection.h"
#include "DBException.h"
#include "string_util.h"

#include "std_warn_off.h"
#include <vector>
#include <set>
#include <list>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

//#define DTL_MEM_DEBUG
#ifdef DTL_MEM_DEBUG

class mcheck {
public:

	void *ptr;
	size_t size;
	unsigned long checksum;
	unsigned long calc_check();
	void write_mem(void);
	mcheck(void *TargetValuePtr, size_t BufferLength);
	bool validate();
	void revalidate();
};

#endif

// class representing a statement attribute
struct Attr
{
   SQLINTEGER attr;
   SQLPOINTER value;
   SQLINTEGER length;

   Attr() : attr(0), value(0), length(0) { }

   Attr(SQLINTEGER att, SQLPOINTER val, SQLINTEGER len) : attr(att), value(val), length(len) { }

   friend inline bool operator<(const Attr &attr1, const Attr &attr2)
   {
		return (attr1.attr < attr2.attr);
   }
};


class DBStmt : public ValidatedObject
{
  public:
	// state of statement
    enum StmtState { STMT_UNALLOCATED = 0, STMT_ALLOCATED, STMT_PREPARED, STMT_EXECUTED };

	// status returned by Fetch()
    enum FetchStatus { ROW_FETCHED, NO_DATA_FETCHED };

	typedef RETCODE NeedDataStatus;

    // ODBC errors deemed fatal to a DBStmt on Execute()
    // causes the DBStmt to become invalidated
    // all other errors do not validate the DBStmt
	static const STD_::vector<tstring> FatalErrors;

	

  private:

	DBConnection *pConn; // connection this statement is using
	HSTMT hstmt;		// this statement
	tstring sqlQuery;	// SQL query tstring for the statement

	StmtState state;	// is hstmt. allocated?

	STD_::set<Attr> attrs; // non-default attributes of the statement

	bool bPrepare;			// if true, use SQLPrepare for this statement


#ifdef DTL_MEM_DEBUG
	STD_::vector<mcheck> vc;
	STD_::vector<mcheck> vp;
#endif


	HSTMT GetHSTMT() const;

	static STD_::vector<tstring> BuildFatalErrorsList();

	// sets the attributes in ODBC that the user set in the DBStmt object
	void LoadAttributes();

	void CallCatalogFunction();
    
  public:

	DBStmt &operator=(const DBStmt &other){
		DBStmt tmp(other);
		tmp.swap(*this);
		return *this;
	}

	DBStmt(const DBStmt &stmt);

	// constructs a DBStmt associated with the connection
	// allocate (and prepare stmt) if alloc_stmt set to true
	DBStmt(const tstring &query = _TEXT(""), const DBConnection &connection = DBConnection::GetDefaultConnection(),
		bool PrepareFlag = false, bool CaseifyFlag = true);
		   
	// no-throw swap
	void swap(DBStmt &other);

	// return reference to the connection associated with this stmt.
	DBConnection &GetConnection() const;

	// set stmt. to refer to a different existing connection
	void SetConnection(const DBConnection &conn);

	// Destroy() may have tricky destruction semantics
    ~DBStmt();

	// is the stmt. ready for execution?
	bool IsReady() const; 

	// does the stmt. refer to an allocated handle?
	bool IsAllocated() const;

	bool IsExecuted() const;

    // check the state of the object against what we know about the hstmt
	virtual bool valid() const;

	// get a statement ready for execution
	void Initialize();

	// prepare statement for repeated execution
	void Prepare();

	// set flag to use SQLPrepare when executing statement
	void SetPrepare(bool flag) {bPrepare = flag;}

	bool GetPrepare() const {return bPrepare;}

	// Call the ODBC MoreResults() function to see if another set of results exists
	// for this statement.
	// Example uses would be multiple select queries in a single statement e.g.
	// _TEXT("SELECT Col1 from Table1; Select Col2 from Table2")
	// or stored proceedures that return multiple result sets
	// See SQLMoreResults in the ODBC docs for details	
	bool MoreResults();

	// execute the stmt.
	NeedDataStatus Execute();

    // for SELECT's only ... fetch next row from recordset	
	FetchStatus Fetch();

	// for SELECT's only ... fetch next block of rows from recordset	
	FetchStatus FetchScroll(SQLSMALLINT FetchOrientation, SQLINTEGER FetchOffset);

	void SetPos(SQLUSMALLINT RowNumber, SQLUSMALLINT Operation, SQLUSMALLINT LockType);

	// destroy this hstmt. ... must use if done with the hstmt completely or the
	// underlying SQL tstring is changed
	// (can always reuse the hstmt with the same query)

	// tricky destructor issues here!
	void Destroy();

	// frees current statement handle and allocates and prepares a new one
	void ReInitialize();

	// reexecute the statement ... should only be used on a select stmt.
	void ReExecute();

	// close the stmt. so we can reexecute it ... use on a SELECT stmt. to reset the
	// cursor to the top of the recordset
	void Reset();

	// form of reset which accepts a new query tstring for the stmt.
	// must destroy and realloc the hstmt in order to bind
	// query tstring to stmt.
	void ResetWithNewQuery(tstring qry);

	// bind a parameter for the statement
	void BindParam(SQLUSMALLINT ParameterNumber, SQLSMALLINT InputOutputType,
					  SQLSMALLINT ValueType,	SQLSMALLINT ParameterType,
					  SQLULEN ColumnSize, SQLSMALLINT DecimalDigits,
					  void *ParameterValuePtr, SQLLEN BufferLength,
					  SQLLEN *StrLen_or_IndPtr);

	// bind a column for the statement
        void BindCol(SQLUSMALLINT ColumnNumber, SQLUSMALLINT TargetType,
                                        void *TargetValuePtr, SQLLEN BufferLength,
                                        SQLLEN *StrLen_or_IndPtr);
	// get data for a field
        FetchStatus GetData(SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
                                        void *TargetValuePtr, SQLLEN BufferLength,
                                        SQLLEN *StrLen_or_IndPtr);

	// put data for a field
	void PutData(const void *DataPtr, SQLLEN StrLen_or_Ind);

	// see which parameter that system is requesting next for PutData()
	NeedDataStatus ParamData(void **DataPtrPtr);	

	// return number of affected rows ... used by UPDATE and DELETE
	long RowCount() const;

	// get the query tstring for this statement
	const tstring &GetQuery() const;

	// Reset Query
	void SetQuery(const tstring &s); 

	// close cursor for the hstmt
	void CloseCursor();

	// clear paramter bindings
	void UnbindParams();

	// clear column bindings
	void UnbindCols();

	// get statement attribute
	void GetStmtAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr, SQLINTEGER BufferLength, 
		SQLINTEGER* StringLengthPointer);

	// set statement attributes ... will override the default behavior in DTL
	void SetStmtAttr(SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER StringLength);

	// Clear the attribute with the given attribute number
	void ClearStmtAttr(SQLINTEGER Attribute);

	// set stmt. attribute immediately ... lost upon reinitialization of stmt.
	void OverrideStmtAttr(SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER StringLength)
	{
	   RETCODE rc = SQLSetStmtAttr(hstmt, Attribute, (SQLPOINTER) (Value), StringLength);

	   if (!RC_SUCCESS(rc))
		   DTL_THROW DBException(_TEXT("DBStmt::SetStmtAttr()"),
							 _TEXT("Unable to set statement attribute for statement \"") + sqlQuery +
							 _TEXT("\"!"), pConn, this);

	}

	void BulkOperations(SQLUSMALLINT Operation);

	void ClearStmtAttrs();

    friend class DBException;

	// kludge to get around header hell
	friend HSTMT GetHSTMT(const DBStmt &stmt);

	// get primary keys for the table passed in
	static STD_::vector<tstring> GetPrimaryKeys(const tstring &tableNm,
		DBConnection &conn);
};

END_DTL_NAMESPACE

#endif
