// luasocket_impl.cpp

#include "luasocket_decl.hpp"
#include <luasocket.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_socket_core_wrap(lua_State *l)
{
	return luaopen_socket_core(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_mime_core_wrap(lua_State *l)
{
	return luaopen_mime_core(l);
}