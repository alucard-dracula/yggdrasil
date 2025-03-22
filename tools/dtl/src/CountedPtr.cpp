#include "CountedPtr.h"

BEGIN_DTL_NAMESPACE

#ifdef DTL_MEM_DEBUG
STD_::set<BYTE *> MemPtr::memChecker;
#endif

END_DTL_NAMESPACE
