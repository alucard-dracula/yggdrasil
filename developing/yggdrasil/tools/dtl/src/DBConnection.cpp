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
// source for DBConnection
// Initial: 9/20/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 12/19/2000 - MG - added namespaces
// Edited: 10/26/2003 - Paul Grenyer http://www.paulgrenyer.co.uk, added static_cast as required by MSVC 7.1


#ifdef _MSC_VER
#pragma	warning(disable: 4786)
#endif

#include "DBConnection.h"
#include "DBException.h"
#include "clib_fwd.h"
#include "dtl_base_types.h"
#include "bind_basics.h"

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

#include <cassert>

#include "std_warn_on.h"

// #define DTL_NO_POOLING
// Note, by default we turn pooling on.  If you want to allow DTL to browse
// for connections you will need to turn ODBC connection pooling off.


BEGIN_DTL_NAMESPACE

DBEnvironment DBConnection::dbe;

DBConnection DBConnection::defaultConn;


static void InitGlobals()
{
	// Initialize and global singletons

	// Initialize the global ETI_Map here to avoid any multi-threading problems
	GetSQL_types_to_C();

	// Initialize global error handler
	GetErrorHandler();

}

// ***************** Implementation code for DBEnvironment ****************
	DBEnvironment::DBEnvironment() : 
		henv(SQL_NULL_HENV), owns_environment(true) {};

	// release the current environment
	void DBEnvironment::Release() {

		// if this is not my own environment, just free my state variables
		// uncomment when we allow multiple environments

		if (!owns_environment)
		{
			henv = SQL_NULL_HENV;
			return;
		}


		RETCODE rc;
		// free environment handle
		if (henv != SQL_NULL_HENV) {
			rc = SQLFreeHandle(SQL_HANDLE_ENV, henv);
			henv = SQL_NULL_HENV;
		}
	}

	DBEnvironment::~DBEnvironment()
	{
		Release();
	}

	// share another HENV with me ... assumes HENV is already open
	void DBEnvironment::Share(HENV new_henv)
	{
		// handle already owned here if same
		if (henv == new_henv)
			return;

		try
		{
			// uncomment when we support multiple environments

			Release(); // release old environment
		}
		catch (...)
		{

		}
		
		henv = new_henv;

		owns_environment = false;
	}

	void DBEnvironment::init(bool bPool) {
		if (henv != SQL_NULL_HENV)
			return;

		// Initialize any globals
		InitGlobals();

		// Before we allocate our first environment handle, try to turn on connection pooling
		// Note we are not checking error codes here, we just try to turn it on,
		// and if the driver does not support pooling there is nothing we can do.
		RETCODE rc;
#ifdef UNICODE
	//	if (std_tstrnicmp(stmt.GetConnection().GetDBMSName().c_str(), _TEXT("Microsoft SQL Server"), std_tstrlen(_TEXT("SQL Server"))) == 0)	
			//	{
			// SQL Server version 7.0 has problems with connection pooling
			// in UNICODE mode.  Disable pooling in this case
	//		bPool = false; 
	//	}
		bPool = false;
#endif

#ifdef	DTL_NO_POOLING
		bPool = false;
#endif

		if (bPool) {
			// pool across the environment handle
			rc = SQLSetEnvAttr(SQL_NULL_HENV, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER,	
				SQL_IS_INTEGER);
			
			if (!RC_SUCCESS(rc))
			{
			   henv = SQL_NULL_HENV;
			   DTL_THROW DBException(_TEXT("DBEnvironment::init()"),
								 _TEXT("Unable to pool across the environment handle!"),
								 NULL, NULL);
			}
		}
		
		rc = SQLAllocEnv(&henv);
		if (!RC_SUCCESS(rc))
		{
		   henv = SQL_NULL_HENV;
		   DTL_THROW DBException(_TEXT("DBEnvironment::init()"),
							 _TEXT("Unable to allocate SQL environment handle!"),
							 NULL, NULL);
		}

#ifdef DTL_ODBC2
		// set the ODBC behavior version. Not sure why this is required, but
		// connection pooling does not seem to work without it
		rc = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC2, 
			 SQL_IS_INTEGER);
#else

		// set the ODBC behavior version. Not sure why this is required, but
		// connection pooling does not seem to work without it
		rc = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 
			 SQL_IS_INTEGER);
#endif

		// handle error
		if (!RC_SUCCESS(rc))
		{
			tostringstream errmsg;
			errmsg << _TEXT("Unable to set ODBC version behavior!");

			rc = SQLFreeHandle(SQL_HANDLE_ENV, henv);
			
			if (!RC_SUCCESS(rc))
				errmsg << _TEXT("  Unable to free environment handle!");

			henv = SQL_NULL_HENV;

			errmsg << STD_::ends;

			tstring errstr = errmsg.str();

			// errmsg.freeze(false);

			DTL_THROW DBException(_TEXT("DBEnvironment::init()"),
							  errstr,
							  NULL, NULL);
		}

		//set the matching condition for using an existing connection in the pool
		//use relaxed matching, i.e. most liberal rules for matching existing connection
		rc = SQLSetEnvAttr(henv, SQL_ATTR_CP_MATCH, (SQLPOINTER) SQL_CP_RELAXED_MATCH, 
			 SQL_IS_INTEGER);

		// handle error
		if (!RC_SUCCESS(rc))
		{
			tostringstream errmsg;
			errmsg << _TEXT("Unable to set matching condition for environment!");

			rc = SQLFreeHandle(SQL_HANDLE_ENV, henv);
			
			if (!RC_SUCCESS(rc))
				errmsg << _TEXT("  Unable to free environment handle!");

			henv = SQL_NULL_HENV;

			errmsg << STD_::ends;

			tstring errstr = errmsg.str();

			// errmsg.freeze(false);

			DTL_THROW DBException(_TEXT("DBEnvironment::init()"),
							  errstr,
							  NULL, NULL);
		}

		owns_environment = true;
	}


	HENV DBEnvironment::GetHENV() const
	{
		if (henv == SQL_NULL_HENV)
			DTL_THROW DBException(_TEXT("DBEnvironment::GetHENV()"),
				_TEXT("Environment is not allocated!"), NULL, NULL);

		return henv;
	}


// ****************** Implementation code for DBConnection *****************

// **** stmt. logging stuff ****
	
// stuff to log stmts out to an ostream

void DBConnection::ClearLog()
{
	sorted_stmt_log.clear();
	stmt_log.clear();
}

void DBConnection::PrintLog(tostream &o) const
{
	for (RawStmtLog::const_iterator it = stmt_log.begin(); it != stmt_log.end(); ++it)
	{
		o << *it << _TEXT("\n\n");
	}
}

void DBConnection::PrintSortedLog(tostream &o) const
{
	for (SortedStmtLog::const_iterator it = sorted_stmt_log.begin(); 
			it != sorted_stmt_log.end(); ++it)
	{
		o << **it << _TEXT("\n\n");
	}	
}

void DBConnection::AddToLog(const tstring &sqlQuery)
{
	stmt_log.push_back(sqlQuery);
	sorted_stmt_log.insert(&stmt_log.back());
}

void DBConnection::EnableLog()
{
	bStmtLogEnabled = true;
}

void DBConnection::DisableLog()
{
	bStmtLogEnabled = false;
}

bool DBConnection::IsLogEnabled() const
{
	return bStmtLogEnabled;
}
	
// **** end stmt. logging stuff ****

// not responsible for allocating hdbc and dbe.henv,
// that is done by the public version of Connect()
void DBConnection::InternalConnect (const TCHAR *szConnStrIn, bool bPrompt, bool auto_commit_setting)
{
	RETCODE rc;    			/* Return code for ODBC functions */
	TCHAR szConnStrOut[2048];  // Microsoft ODBC requires at least 1024
									 // bytes here, otherwise will overwrite
									 // memory
	SWORD cbConnStrOut;
	UWORD fDriverCompletion;
	bool bUseConnectStr;

	// needed statics
	HWND g_AppHwnd = NULL;
#ifdef DTL_NATIVE_WIN32
	g_AppHwnd = GetDesktopWindow();
#endif

	validate();

	if (state == CONNECTED)
	  return;

	// To prompt for connection, use SQL_DRIVER_COMPLETE
	if (bPrompt)
		fDriverCompletion = SQL_DRIVER_COMPLETE;	
	else
		fDriverCompletion = SQL_DRIVER_NOPROMPT;

    // Prompt for connection
	bUseConnectStr = false;
	if (szConnStrIn != NULL)
		bUseConnectStr = true;

#include "merant.h"

	
	if (bUseConnectStr)
		rc  = SQLDriverConnect(	hdbc, 
								g_AppHwnd, (SQLTCHAR *)szConnStrIn, 
								static_cast<SQLSMALLINT>(std_tstrlen((TCHAR *)szConnStrIn)*sizeof(TCHAR)), 
								(SQLTCHAR *)szConnStrOut, 
								sizeof(szConnStrOut),
								&cbConnStrOut, 
								fDriverCompletion);
	else
		rc  = SQLDriverConnect(hdbc, g_AppHwnd, (SQLTCHAR *)szConnStrIn, 
		0, (SQLTCHAR *)szConnStrOut, sizeof(szConnStrOut), 
		&cbConnStrOut, fDriverCompletion);	
		

    if (!RC_SUCCESS(rc))
	{
	   tstring errmsg;
	   errmsg.reserve(256);
	   errmsg += _TEXT("Unable to connect to database using DSN ");
	   errmsg += (TCHAR *) szConnStrIn;
	   errmsg += _TEXT("! \n\n");
	   if (bPrompt) 
	   {
		   errmsg += _TEXT("Note that in order to browse for a connection, connection pooling must be turned off.\n");
		   errmsg += _TEXT("To do this you must #define DTL_NO_POOL or manually initialize the environment with no pooling via \n");
		   errmsg += _TEXT("DBConnection.GetDefaultEnvironment().init(false);  \nbefore you connect for the first time.\n");
		   errmsg += _TEXT("(If you prompt for a connection the first time you connect then pooling is automatically turned off.)\n\n");
	   }


	   DTL_THROW DBException(_TEXT("DBConnection::Connect(const TCHAR *DSN)"),
			errmsg, this, NULL);
	}

	ComputeDBType();
		
	// update state variables
	// exception safety maintained as state variables not updated until
	// connection succeeds end
	// only the tstring assignment can throw afterwards (so it's done first)
    usedDSN = (TCHAR *) szConnStrOut;
    state = CONNECTED;
	auto_commit = true;
		
    // Turn off autocommit so that info is committed as one transaction
    // DBConnection class commits on destruction or if auto_commit is set
    // N.B.!! If we do not do this takes 3 TIMES as long to run write to DB.
    // For sample data set takes 7.6 Seconds w/o autocommit v.s. 22.4 seconds with autocommit.
    if (dbType != DB_MYSQL && dbType != DB_EXCEL && auto_commit_setting == false)
	{
		rc = SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);

		// Throw exception if no auto commit, but still have state as connected
		// since this is a usable connection
	    if (!RC_SUCCESS(rc))
		    DTL_THROW DBException(_TEXT("DBConnection::Connect(const TCHAR *DSN)"),
			  _TEXT("Unable to turn off autocommit!"), this, NULL);
    }

	auto_commit = false;
}

	// set an enum to hold the type of database we are connected to and any other
	// DB specific settings
	void DBConnection::ComputeDBType() {
		tstring db = GetDBMSName();
		const TCHAR *sz = db.c_str();
		if (std_tstrnicmp(sz, 
			_TEXT("Microsoft SQL Server"), std_tstrlen(_TEXT("Microsoft SQL Server"))) == 0)	
			dbType = DB_SQL_SERVER;
		else if (std_tstrnicmp(sz, _TEXT("MySQL"), std_tstrlen(_TEXT("MySQL"))) == 0)
			dbType = DB_MYSQL;
		else if (std_tstrnicmp(sz, _TEXT("Oracle"), std_tstrlen(_TEXT("Oracle"))) == 0)
			dbType = DB_ORACLE;
		else if (std_tstrnicmp(sz, _TEXT("OpenRDA"), std_tstrlen(_TEXT("OpenRDA"))) == 0)
			dbType = DB_OPENRDA;
		else if (std_tstrnicmp(sz, _TEXT("Access"), std_tstrlen(_TEXT("Access"))) == 0)
			dbType = DB_ACCESS;
		else if (std_tstrnicmp(sz, _TEXT("EXCEL"), std_tstrlen(_TEXT("EXCEL"))) == 0)
			dbType = DB_EXCEL;
		else
			dbType = DB_UNKNOWN;

		TCHAR cQuoteChar[10];
		SQLSMALLINT cbStringLen;
		RETCODE rc;

		rc = SQLGetInfo(hdbc, SQL_IDENTIFIER_QUOTE_CHAR, &cQuoteChar, sizeof(cQuoteChar) * sizeof(TCHAR),
			&cbStringLen);

		// Assume the ANSI standard quote character of double quote 
		// if the QuoteChar type cannot be retrieved from the database driver

		if (!RC_SUCCESS(rc) || std_tstrlen(cQuoteChar) == 0 || std_tisspace(cQuoteChar[0]))
			szQuoteChar = _TEXT("\""); 
		else
			szQuoteChar = cQuoteChar;

		// should we be fatal if these fail?
		TCHAR DriverName[100];
		SQLSMALLINT cbDriverName;
		rc = SQLGetInfo(hdbc, SQL_DRIVER_NAME, &DriverName, sizeof(szDriverName) * sizeof(TCHAR),
			&cbDriverName);

		szDriverName = DriverName;

		TCHAR DriverVer[100];
		SQLSMALLINT cbDriverVer;
		rc = SQLGetInfo(hdbc, SQL_DRIVER_VER, &DriverVer, sizeof(szDriverVer) * sizeof(TCHAR),
			&cbDriverVer);

		szDriverVer = DriverVer;

	}

	// connect to DB using the passed in DSN
	// and set the dbe.henv and hdbc members
	// throws exception if unable to connect!
	DBConnection::DBConnection(const tstring &DSN)
		: ValidatedObject(), hdbc(SQL_NULL_HDBC), auto_commit(false), state(CONN_UNALLOCATED),
		  owns_connection(true), dbType(DB_UNKNOWN), szQuoteChar(_TEXT("\"")), 
		  szDriverName(), szDriverVer(), usedDSN(DSN), stmt_log(), sorted_stmt_log(),
		  bStmtLogEnabled(false)
	{ }


	// exception-safe swap()
	void DBConnection::swap(DBConnection &other)
	{
		ValidatedObject::swap(other);
		STD_::swap(hdbc, other.hdbc);
		STD_::swap(state, other.state);
		usedDSN.swap(other.usedDSN);
		STD_::swap(auto_commit, other.auto_commit);
		STD_::swap(owns_connection, other.owns_connection);
		STD_::swap(dbType, other.dbType);
		STD_::swap(szQuoteChar, other.szQuoteChar);
		STD_::swap(szDriverName, other.szDriverName);
		STD_::swap(szDriverVer, other.szDriverVer);
	}

	// Connect to the DB using the passed in DSN if not already connected
	// any existing constructs which reference this connection
	// become invalid if the DSN is different than the DBConnection was using
	// prior to the call to Connect()

	// connect using usedDSN as connection tstring if no arguments passed in
	void DBConnection::Connect()
	{
		Connect(usedDSN); // exception-safe as only operation done
	}

	// cleanup issues here if exception is thrown!!!!
	void DBConnection::Connect(const tstring &DSN, bool bPrompt, bool auto_commit_setting)
	{
		validate();
		
		if (DSN == usedDSN && state == CONNECTED )
			return;

        DBConnection tmp(DSN); // needed for exception safety

		
		RETCODE rc;

		bool bPool = true;

		// cannot prompt for connection if pooling is turned on.
		if (bPrompt)
			bPool = false;

		dbe.init(bPool);		

		assert(dbe.GetHENV() != SQL_NULL_HENV);
			
		// allocate connection
		rc = SQLAllocConnect(dbe.GetHENV(), &tmp.hdbc);

		if (!RC_SUCCESS(rc))
		{
			tmp.invalidate();
			DTL_THROW DBException(_TEXT("DBConnection::DBConnection()"),
							  _TEXT("Unable to allocate new connection handle!"),
							  this, NULL);
		}

		tmp.state = CONN_ALLOCATED;
		tmp.owns_connection = true;

		// try to connect to DB instance
		try {
			tmp.InternalConnect(DSN.c_str(), bPrompt, auto_commit_setting);
		}
		catch (...) 
		{
			if (tmp.state == CONNECTED) // unable to turn off auto-commit, keep connection
				swap(tmp);
			throw; 
		}


		// connection successful ... if we reach here
		// state info properly updated by _TUCHAR * overload of Connect

		swap(tmp); // atomicity and consistency guaranteed through
		           // tmp, process, swap technique
	}

	// are we connected?
	bool DBConnection::IsConnected()
	{
		return (state == CONNECTED);
	}

	// functions to commit or rollback all transactions that have occurred between last 
	// commit or rollback on this connection

	// turn autocommit on or off
	void DBConnection::SetAutoCommit(bool commit)
    {
		validate();

		assert(hdbc != SQL_NULL_HDBC);

		bool old_auto_commit = auto_commit;

		if (old_auto_commit == commit)
			return;

		RETCODE rc = SQL_SUCCESS;

    // don't do anything if using MySQL as it doesn't support transactions
	if (dbType != DB_MYSQL)
    {
			if (commit)
			{
				CommitAll();
				rc = SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_ON);
			}
			else
			{
				rc = SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
			}

			if (!RC_SUCCESS(rc))
			{
				DTL_THROW DBException(_TEXT("DBConnection::SetAutoCommit()"),
					_TEXT("Unable to toggle auto commit!"), this, NULL);
			}
    }

		auto_commit = commit;
	}

	bool DBConnection::GetAutoCommit() const
    {
		return auto_commit;
	}

	// only one mutating operation done, so should be exception-safe
	void DBConnection::CommitAll()
	{
		validate();
		
		// does nothing if not connected
		if (state != CONNECTED)
			return;

		assert(dbe.GetHENV() != SQL_NULL_HENV);
		assert(hdbc != SQL_NULL_HDBC);

		RETCODE rc = SQLTransact(dbe.GetHENV(), hdbc, SQL_COMMIT);


		// Select statements cannot commit.
		if (!RC_SUCCESS(rc))
		{
			invalidate();
			DTL_THROW DBException(_TEXT("DBConnection::CommitAll()"),
							  _TEXT("Unable to commit transaction!"),
							  this, NULL);
		}
	}

	// only one mutating operation done, so should be exception-safe
	// we require this to be no-throw for exception safe operations
	void DBConnection::RollbackAll()
	{
		// does nothing if not connected
		if (state != CONNECTED)
			return;

		// if state is connected we should have non-null hdbc
		assert(hdbc != SQL_NULL_HDBC);

        // no-throw on error
		HENV henv = dbe.GetHENV_NoThrow();

		// only destruct if the parent environment has
		// not been deleted first
		if (henv != SQL_NULL_HENV) {
			// Ignore return
			SQLTransact(dbe.GetHENV_NoThrow() , hdbc, SQL_ROLLBACK);
		}
		
	}
	// cleanup issues here on exceptions!!!
	DBConnection::~DBConnection()
    {
		try
		{
			Release();
		}
		catch (...)
		{

		}
	}

	// share another HDBC with me ... assumes connection to share is open
	// in this implementation, trusts the programmer that he has no open connections
	// under the old henv or henv == new_henv
	void DBConnection::Share(HENV new_henv, HDBC new_hdbc)
	{
		assert(new_henv != SQL_NULL_HENV);
		assert(new_hdbc != SQL_NULL_HDBC);

		// handle already owned if same
		if (hdbc == new_hdbc && dbe.GetHENV() == new_henv)
			return;

		try
		{
			Release(); // release old connection
		}
		catch (...)
		{

		}

		dbe.Share(new_henv);

		// assume connection is already open!
		hdbc = new_hdbc;
		state = CONNECTED;
		usedDSN = _TEXT(""); // old connection tstring now invalid ... we don't know what
					  // the connection tstring of the shared connection is
		owns_connection = false;
		ComputeDBType();
	}

	// only able to guarantee consistency
	// ODBC resources may leak
	void DBConnection::Release()
	{
		// if not my own handle, do not release the actual handle
		// just free up the state variables
		if (!owns_connection)
		{
			hdbc = SQL_NULL_HDBC;
			state = CONN_UNALLOCATED;
			// usedDSN = _TEXT("");			
			return;
		}

		// first release ownership to prevent double destruction
		owns_connection = false;

		tstring errmsg;

		// use a destroy as much as you can approach!
		// need some way to make this exception-safe

		if(CONN_UNALLOCATED == state)
		{
			return	;
		}

		// Rollback on destruction if something has invalidated the
		// connection's state
		// Otherwise, always commit
	    try
		{
		  if (!valid())
			RollbackAll();
		  else
			CommitAll();
		}
		catch (RootException &ex)
		{
			tostringstream errstream;
			errstream << ex.what() << STD_::endl;
			errstream << _TEXT("Failed to CommitAll() or RollbackAll()!");
			errmsg = errstream.str();

			// errstream.freeze(false);

			// set state variables back to an unallocated state
		    // in order to maintain consistency

		    state = CONN_UNALLOCATED;
		    hdbc = SQL_NULL_HDBC;
			// usedDSN = _TEXT("");
			DTL_THROW DBException(_TEXT("DBConnection::Release()"), errmsg, this, NULL);
		}

		// up to here, we're exception safe
		// even if we failed to commit or rollback,
		// we're still in a consistent and atomic state
		
		RETCODE rc = SQL_SUCCESS;
		
		// only try to deallocate connection and environment if actually
		// allocated in the first place
		// problems occur here if we throw here

		switch (state)
		{
		case CONNECTED : 
			rc	= SQLDisconnect(hdbc);
			if (!RC_SUCCESS(rc))
			{
				errmsg += _TEXT("Unable to disconnect using odbc handle!");
					
				// set state variables back to an unallocated state
				// in order to maintain consistency

				state = CONN_UNALLOCATED;
				hdbc = SQL_NULL_HDBC;
				// usedDSN = _TEXT("");

				DTL_THROW DBException(_TEXT("DBConnection::Release()"), errmsg, this, NULL);
				break;
			}

			// fallthrough intentional as also need to free
			// connection handle

		case CONN_ALLOCATED:
				// free connection handle
				rc = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	
				if (!RC_SUCCESS(rc))
				{
					errmsg += _TEXT("Unable to free connection handle!");
					
					// set state variables back to an unallocated state
					// in order to maintain consistency

					state = CONN_UNALLOCATED;
					hdbc = SQL_NULL_HDBC;
					// usedDSN = _TEXT("");

					DTL_THROW DBException(_TEXT("DBConnection;:Release()"), errmsg, this, NULL);
					break;
				}
				
			// fallthrough intentional if we ever need to do anything in the UNALLOCATED case
			
		case CONN_UNALLOCATED: ; // do nothing
		default: ; // do nothing
		}

		// set state variables back to an unallocated state
		// in order to maintain consistency

		state = CONN_UNALLOCATED;
		hdbc = SQL_NULL_HDBC;
		// usedDSN = _TEXT("");

		if (!errmsg.empty())
			DTL_THROW DBException(_TEXT("DBConnection::Release()"), errmsg, NULL, NULL);
	}

	DBConnection &DBConnection::GetDefaultConnection()
	{
		return defaultConn;
	}

	DBEnvironment &DBConnection::GetDefaultEnvironment()
	{
		return dbe;
	}

	// return a list of ODBC data source names by calling SQLDataSources
	// SourceType = Direction parameter for SQLDataSources
	//
	// SourceType = SQL_FETCH_FIRST = get all sources
	// SourceType = SQL_FETCH_FIRST_USER = get only user data sources
	// SourceType = SQL_FETCH_FIRST_SYSTEM = get only system data sources
	//
	// Return vector of pair(ServerName, Description)
	STD_::vector<STD_::pair<tstring, tstring> > DBConnection::GetDataSources(SQLUSMALLINT SourceType)
	{
		RETCODE rc;
		const SQLSMALLINT MAX_LEN = 100;
		SQLTCHAR ServerName[MAX_LEN], Description[MAX_LEN];
		SQLSMALLINT cbServerName, cbDescription;
		STD_::vector<STD_::pair<tstring, tstring> >  sources;

		dbe.init(true);  // init dbe if not already done
		SQLHENV henv = dbe.GetHENV();

		rc = SQLDataSources(henv, SourceType, ServerName, MAX_LEN, &cbServerName, 
			Description, MAX_LEN, &cbDescription);
		
		if (!RC_SUCCESS(rc))
				DTL_THROW DBException(_TEXT("DBConnection::GetDataSources()"),
					_TEXT("Unable to retrieve data source names!"), NULL, NULL);

		sources.push_back(STD_::pair<tstring, tstring>(tstring((TCHAR *)ServerName), tstring((TCHAR *)Description)));

		for (;;)
		{
			rc = SQLDataSources(henv, SQL_FETCH_NEXT, ServerName, MAX_LEN, &cbServerName, 
			Description, MAX_LEN, &cbDescription);

			if (rc == SQL_NO_DATA)
				break;

			if (!RC_SUCCESS(rc))
				DTL_THROW DBException(_TEXT("DBConnection::GetDataSources()"),
					_TEXT("Error while retrieving data source names!"), NULL, NULL);


			sources.push_back(STD_::pair<tstring, tstring>(tstring((TCHAR *)ServerName), tstring((TCHAR *)Description)));

		}

		return sources;
	}

	HDBC DBConnection::GetHDBC() const 
	{ 
		if (state == CONN_UNALLOCATED)
			DTL_THROW DBException(_TEXT("DBConnection::GetHDBC()"),
				_TEXT("Connection is not allocated!"), NULL, NULL);

		return hdbc; 
	}
	HENV DBConnection::GetHENV() const { return dbe.GetHENV(); }
    tstring DBConnection::GetDSN() const { return usedDSN;}

    // return name of the DBMS being used
    tstring DBConnection::GetDBMSName() const
	{
	   assert(hdbc != SQL_NULL_HDBC);

     TCHAR szDriverNm[200];
     SWORD cbDummy;

     RETCODE rc = SQLGetInfo(hdbc, SQL_DBMS_NAME, &szDriverNm, sizeof(szDriverNm),
        &cbDummy);

	   if (!RC_SUCCESS(rc))
				DTL_THROW DBException(_TEXT("DBConnection::GetDBMSName()"),
					_TEXT("Unable to get name of DBMS!"), this, NULL);

     return szDriverNm;
	}

	// return enum for name of the DBMS being used
	DBConnection::DBType DBConnection::GetDBMSEnum() const {
		return dbType;
	}

  // return the DBMS version being used
  tstring DBConnection::GetDBMSVersion() const
	{
	  assert(hdbc != SQL_NULL_HSTMT);

    TCHAR szDriverVersion[200];
	  SWORD cbDummy;
	
	  RETCODE rc = SQLGetInfo(hdbc, SQL_DBMS_VER, &szDriverVersion, sizeof(szDriverVersion),
        &cbDummy);

	  if (!RC_SUCCESS(rc))
				DTL_THROW DBException(_TEXT("DBConnection::GetDBMSVersion()"),
					_TEXT("Unable to get version of DBMS!"), this, NULL);

      return szDriverVersion;
	}

	const tstring & DBConnection::GetQuoteChar(void) const 
	{
	  return szQuoteChar;
	}

	// return name of the odbc driver being used
	tstring DBConnection::GetDriverName() const
	{
		return szDriverName;
	}

    // return version of the odbc driver being used
    tstring DBConnection::GetDriverVersion() const
	{
		return szDriverVer;
	}

// ******************* helper functions *******************
	HDBC GetHDBC(const DBConnection &conn)
	{
		if (conn.state == DBConnection::CONN_UNALLOCATED)
			DTL_THROW DBException(_TEXT("DBConnection::GetHDBC()"),
				_TEXT("Connection is not allocated!"), NULL, NULL);

		return conn.GetHDBC();
	}

	HENV GetHENV(const DBConnection &conn)
	{
		return conn.GetHENV();
	}

END_DTL_NAMESPACE
