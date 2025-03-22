// YGGR_MODULE_CALL_T() tolua_decl.hpp

#ifndef __DLY_LUA_GUNGNIR_TOLUA_HPP__
#define __DLY_LUA_GUNGNIR_TOLUA_HPP__

#include "call_convention_def.hpp"

#include <lua.hpp>

#include <yggr/dll_make/dll_mark_define.hpp>

#include <yggr/dll_make/dll_make_begin.ipp>

YGGR_DLL_API void* YGGR_MODULE_CALL_T() tolua_tag_wrap(void);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_newudata_wrap(lua_State* l, int val);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_rawnetobj_wrap(lua_State* l, int index);

YGGR_DLL_API char* YGGR_MODULE_CALL_T() tolua_tocbuffer_wrap(const char* csBuffer, int sz);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_freebuffer_wrap(void* buffer);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getvec2_wrap(lua_State* l, int pos, float* x, float* y);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getvec3_wrap(lua_State* l, int pos, float* x, float* y, float* z);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getvec4_wrap(lua_State* l, int pos, float* x, float* y, float* z, float* w);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getquat_wrap(lua_State* l, int pos, float* x, float* y, float* z, float* w);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getclr_wrap(lua_State* l, int pos, float* r, float* g, float* b, float* a);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getlayermask_wrap(lua_State* l, int pos);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushvec2_wrap(lua_State* l, float x, float y);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushvec3_wrap(lua_State* l, float x, float y, float z);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushvec4_wrap(lua_State* l, float x, float y, float z, float w);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushquat_wrap(lua_State* l, float x, float y, float z, float w);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushclr_wrap(lua_State* l, float r, float g, float b, float a);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushlayermask_wrap(lua_State* l, int mask);

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() tolua_tolstring_wrap(lua_State* l, int index, int* len);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushlstring_wrap(lua_State* l, const char* s, int len);

YGGR_DLL_API void* YGGR_MODULE_CALL_T() tolua_newuserdata_wrap(lua_State* l, int sz);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_objlen_wrap(lua_State* l, int idx);

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_toboolean_wrap(lua_State* l, int idx);
YGGR_DLL_API int32_t YGGR_MODULE_CALL_T() tolua_tointeger_wrap(lua_State* l, int idx);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_loadbuffer_wrap(lua_State* l, const char* buff, int sz, const char* name);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getfield_wrap(lua_State* l, int idx, const char* field);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_setfield_wrap(lua_State* l, int idx, const char* key);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_gettable_wrap(lua_State* l, int idx);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_settable_wrap(lua_State* l, int idx);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_pushcfunction_wrap(lua_State* l, lua_CFunction fn);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_argerror_wrap(lua_State* l, int narg, const char* extramsg);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_error_wrap(lua_State* l, const char* msg);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getn_wrap(lua_State* l, int i);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_strlen_wrap(const char* str);

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_isvptrtable_wrap(lua_State* l, int index);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_setindex_wrap(lua_State* l);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_setnewindex_wrap(lua_State* l);
YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_pushudata_wrap(lua_State* l, int index);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushnewudata_wrap(lua_State* l, int metaRef, int index);

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_beginmodule_wrap(lua_State* l, const char* name);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_endmodule_wrap(lua_State* l);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_beginclass_wrap(lua_State* l, const char* name, int baseType, int ref);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_endclass_wrap(lua_State* l);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_beginenum_wrap(lua_State* l, const char* name);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_endenum_wrap(lua_State* l);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_beginstaticclass_wrap(lua_State* l, const char* name);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_endstaticclass_wrap(lua_State* l);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_constant_wrap(lua_State* l, const char* name, double value);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_function_wrap(lua_State* l, const char* name, lua_CFunction fn);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_variable_wrap(lua_State* l, const char* name, lua_CFunction get, lua_CFunction set);
YGGR_DLL_API int YGGR_MODULE_CALL_T() toluaL_ref_wrap(lua_State* l);
YGGR_DLL_API void YGGR_MODULE_CALL_T() toluaL_unref_wrap(lua_State* l, int reference);

YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() tolua_getmainstate_wrap(lua_State* L1);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getvaluetype_wrap(lua_State* l, int stackPos);

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_createtable_wrap(lua_State* l, const char* path, int szhint);

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_beginpremodule_wrap(lua_State* l, const char* path, int szhint);
YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_endpremodule_wrap(lua_State* l, int ref);

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_addpreload_wrap(lua_State* l, const char* path);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getclassref_wrap(lua_State* l, int pos);

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_pushluatable_wrap(lua_State* l, const char* path);
YGGR_DLL_API const char* YGGR_MODULE_CALL_T() tolua_typename_wrap(lua_State* l, int lo);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getmetatableref_wrap(lua_State* l, int pos);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_beginpcall_wrap(lua_State* l, int reference);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushtraceback_wrap(lua_State* l);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_require_wrap(lua_State* l, const char* fileName);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_openlualibs_wrap(lua_State* l);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_openlibs_wrap(lua_State* l);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_setflag_wrap(int bit, bool flag);
YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_getflag_wrap(int bit);

YGGR_DLL_API luaL_Buffer* YGGR_MODULE_CALL_T() tolua_buffinit_wrap(lua_State* l);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_addlstring_wrap(luaL_Buffer* b, const char* s, int len);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_addstring_wrap(luaL_Buffer* b, const char* s);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_addchar_wrap(luaL_Buffer* b, char c);

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_update_wrap(lua_State* l, float deltaTime, float unscaledTime);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_lateupdate_wrap(lua_State* l);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_fixedupdate_wrap(lua_State* l, float fixedTime);

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_regthis_wrap(lua_State* l, lua_CFunction get, lua_CFunction set);
YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_where_wrap(lua_State* l, int level);

YGGR_DLL_API int64_t YGGR_MODULE_CALL_T() tolua_toint64_wrap(lua_State* l, int pos);
YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_isint64_wrap(lua_State* l, int pos);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_openint64_wrap(lua_State* l);
YGGR_DLL_API int  YGGR_MODULE_CALL_T() tolua_newint64_wrap(lua_State* l);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushint64_wrap(lua_State* l, int64_t n);

YGGR_DLL_API uint64_t YGGR_MODULE_CALL_T() tolua_touint64_wrap(lua_State* l, int pos);
YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_isuint64_wrap(lua_State* l, int pos);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_openuint64_wrap(lua_State* l);
YGGR_DLL_API int  YGGR_MODULE_CALL_T() tolua_newuint64_wrap(lua_State* l);
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushuint64_wrap(lua_State* l, uint64_t n);

#include <yggr/dll_make/dll_make_end.ipp>

#endif // __DLY_LUA_GUNGNIR_TOLUA_HPP__