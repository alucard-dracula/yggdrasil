// luacjson_impl.cpp

#include "luacjson_decl.hpp"
#include <luacjson.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_cjson_wrap(lua_State *l)
{
	return luaopen_cjson(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_cjson_safe_wrap(lua_State *l)
{
	return luaopen_cjson_safe(l);
}