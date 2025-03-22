/*
** $Id: lstate.h,v 1.1 2012/05/18 10:44:06 xuyang Exp $
** Global State
** See Copyright Notice in lua.h
*/

#ifndef lstate_h
#define lstate_h

#ifndef YGGR_LUA_THREAD_SAFE
#	include "lstate_nts.h"
#else
#	include "lstate_ts.h"
#endif // YGGR_LUA_THREAD_SAFE
#endif
