#ifndef DTL_BASE_TYPES_H_
#define DTL_BASE_TYPES_H_

#include "dtl_config.h" 

// definitions found in SQL.h but replicated here for speed
///////////////////////////////////////////////////////////

typedef signed short            RETCODE;

#ifndef SQL_SUCCESS
#define SQL_SUCCESS                0
#define SQL_SUCCESS_WITH_INFO      1
#endif

///////////////////////////////////////////////////////////
// end SQL.h definitions

BEGIN_DTL_NAMESPACE

typedef unsigned long jtime_t;

// _TEXT("macro") indicating success on return from an ODBC function
bool RC_SUCCESS(RETCODE rc);

END_DTL_NAMESPACE
#endif
