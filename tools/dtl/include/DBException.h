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
// exception class for DB abstractions
// Initial: 9/26/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 11/14/2000 - MG - made to inherit from RootException
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_DB_EXCEPTION_H
#define DTL_DB_EXCEPTION_H


#include "DB_Base.h"
#include "RootException.h"

#include "std_warn_off.h"

#include <sstream>
#include <vector>

#ifdef DTL_NATIVE_WIN32
	#ifdef DTL_USE_MFC
		#include <afx.h>
	#else
		#include <windows.h>
	#endif
#endif

#include <sql.h>
#include <sqlext.h>

#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

// kludge needed to get around header hell
class DBConnection;
class DBStmt;

// exception class for DB Abstractions
class DBException : public RootException
{
  public:
	   // raw information that is used to build an error tstring for ODBC errors
	   struct ODBCErrorInfo
	   {
		  tstring szSQLState;
		  SDWORD nativeErr;
		  tstring errMsg;

		  ODBCErrorInfo() : szSQLState(), nativeErr(0), errMsg() { }

		  ODBCErrorInfo(const tstring &sqlState, const SDWORD &nErr, const tstring &msg) :
		     szSQLState(sqlState), nativeErr(nErr), errMsg(msg) { }

		  friend tostream &operator<<(tostream &o, const ODBCErrorInfo &info)
		  {
		     o << _TEXT("SQL Error! ") << _TEXT("SQLSTATE = ") << info.szSQLState 
			   << _TEXT(" Native err = ") << info.nativeErr << _TEXT(" msg = ") << info.errMsg << STD_::endl;

			 return o;
		  }

		  operator tstring() const
		  {
			 tostringstream ostr;
			 ostr << *this << STD_::ends;
			 tstring str = ostr.str();			 
			 return str;
		  }
	   };

  private:
	static STD_::vector<ODBCErrorInfo> ShowSQLErrors(const DBConnection *pConn, const DBStmt *pStmt);
  public:
    const STD_::vector<ODBCErrorInfo> sqlErrors; // errors generated by ODBC

	DBException(const tstring &meth, const tstring &msg,
			const DBConnection *pConn, const DBStmt *pStmt);

	virtual const char *what() const throw();
	virtual const TCHAR *twhat() const throw();
	virtual ~DBException() throw() {};

	// gets the true ODBC error code that caused the exception
	// this will be the szSQLState member of the first element
	// of the sqlErrors STD_::vector
	// returns empty tstring if no ODBC error occurred
	STD_::pair<tstring, tstring> GetODBCError() const;

	// returns the entire error STD_::vector generated by the exception
	STD_::vector<ODBCErrorInfo> GetAllODBCErrors() const;
};

END_DTL_NAMESPACE
#endif
