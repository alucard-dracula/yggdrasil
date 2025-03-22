/*
** $Id: llimits.h,v 1.1 2012/08/06 01:43:22 xuyang Exp $
** Limits, basic types, and some other `installation-dependent' definitions
** See Copyright Notice in lua.h
*/

#ifndef llimits_h
#define llimits_h

#ifndef YGGR_LUA_THREAD_SAFE
#	include "llimits_nts.h"
#else
#	include "llimits_ts.h"
#endif // YGGR_LUA_THREAD_SAFE

#endif
