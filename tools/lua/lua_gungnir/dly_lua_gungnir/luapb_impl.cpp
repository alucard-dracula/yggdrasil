// luapb_impl.cpp

#include "luapb_decl.hpp"
#include <luapb.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_pb_wrap(lua_State *l)
{
	return luaopen_pb(l);
}