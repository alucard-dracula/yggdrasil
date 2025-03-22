// lualpeg_impl.cpp

#include "lualpeg_decl.hpp"
#include <lualpeg.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_lpeg_wrap(lua_State* l)
{
	return luaopen_lpeg(l);
}