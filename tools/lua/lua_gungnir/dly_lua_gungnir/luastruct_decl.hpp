// luastruct_decl.hpp

#ifndef __DLY_LUA_GUNGNIR_LUASTRUCT_HPP__
#define __DLY_LUA_GUNGNIR_LUASTRUCT_HPP__

#include "call_convention_def.hpp"
#include <lua.hpp>

#include <yggr/dll_make/dll_mark_define.hpp>

#include <yggr/dll_make/dll_make_begin.ipp>

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_struct_wrap(lua_State* l);

#include <yggr/dll_make/dll_make_end.ipp>

#endif // __DLY_LUA_GUNGNIR_LUASTRUCT_HPP__