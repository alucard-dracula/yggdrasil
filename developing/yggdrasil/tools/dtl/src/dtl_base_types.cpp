#include "dtl_base_types.h"

BEGIN_DTL_NAMESPACE

// _TEXT("macro") indicating success on return from an ODBC function
bool RC_SUCCESS(RETCODE rc)
{
	return (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO);
}

END_DTL_NAMESPACE
