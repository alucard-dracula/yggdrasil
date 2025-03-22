// luacjson_decl.hpp

#ifndef __DLY_LUA_GUNGNIR_LUACJSON_HPP__
#define __DLY_LUA_GUNGNIR_LUACJSON_HPP__

#include "call_convention_def.hpp"
#include <lua.hpp>

#include <yggr/dll_make/dll_mark_define.hpp>

#include <yggr/dll_make/dll_make_begin.ipp>

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_cjson_wrap(lua_State *l);
YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_cjson_safe_wrap(lua_State *l);

#include <yggr/dll_make/dll_make_end.ipp>

#endif // __DLY_LUA_GUNGNIR_LUACJSON_HPP__