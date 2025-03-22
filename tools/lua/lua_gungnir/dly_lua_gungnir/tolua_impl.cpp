// lua_impl.cpp

#include "tolua_decl.hpp"
#include <tolua.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


YGGR_DLL_API void* YGGR_MODULE_CALL_T() tolua_tag_wrap(void)
{
	return tolua_tag();
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_newudata_wrap(lua_State* l, int val)
{
	return tolua_newudata(l, val);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_rawnetobj_wrap(lua_State* l, int index)
{
	return tolua_rawnetobj(l, index);
}

YGGR_DLL_API char* YGGR_MODULE_CALL_T() tolua_tocbuffer_wrap(const char* csBuffer, int sz)
{
	return tolua_tocbuffer(csBuffer, sz);
}
YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_freebuffer_wrap(void* buffer)
{
	return tolua_freebuffer(buffer);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getvec2_wrap(lua_State* l, int pos, float* x, float* y)
{
	return tolua_getvec2(l, pos, x, y);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getvec3_wrap(lua_State* l, int pos, float* x, float* y, float* z)
{
	return tolua_getvec3(l, pos, x, y, z);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getvec4_wrap(lua_State* l, int pos, float* x, float* y, float* z, float* w)
{
	return tolua_getvec4(l, pos, x, y, z, w);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getquat_wrap(lua_State* l, int pos, float* x, float* y, float* z, float* w)
{
	return tolua_getquat(l, pos, x, y, z, w);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_getclr_wrap(lua_State* l, int pos, float* r, float* g, float* b, float* a)
{
	return tolua_getclr(l, pos, r, g, b, a);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getlayermask_wrap(lua_State* l, int pos)
{
	return tolua_getlayermask(l, pos);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushvec2_wrap(lua_State* l, float x, float y)
{
	return tolua_pushvec2(l, x, y);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushvec3_wrap(lua_State* l, float x, float y, float z)
{
	return tolua_pushvec3(l, x, y, z);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushvec4_wrap(lua_State* l, float x, float y, float z, float w)
{
	return tolua_pushvec4(l, x, y, z, w);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushquat_wrap(lua_State* l, float x, float y, float z, float w)
{
	return tolua_pushquat(l, x, y, z, w);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushclr_wrap(lua_State* l, float r, float g, float b, float a)
{
	return tolua_pushclr(l, r, g, b, a);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushlayermask_wrap(lua_State* l, int mask)
{
	return tolua_pushlayermask(l, mask);
}

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() tolua_tolstring_wrap(lua_State* l, int index, int* len)
{
	return tolua_tolstring(l, index, len);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushlstring_wrap(lua_State* l, const char* s, int len)
{
	return tolua_pushlstring(l, s, len);
}

YGGR_DLL_API void* YGGR_MODULE_CALL_T() tolua_newuserdata_wrap(lua_State* l, int sz)
{
	return tolua_newuserdata(l, sz);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_objlen_wrap(lua_State* l, int idx)
{
	return tolua_objlen(l, idx);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_toboolean_wrap(lua_State* l, int idx)
{
	return tolua_toboolean(l, idx);
}

YGGR_DLL_API int32_t YGGR_MODULE_CALL_T() tolua_tointeger_wrap(lua_State* l, int idx)
{
	return tolua_tointeger(l, idx);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_loadbuffer_wrap(lua_State* l, const char* buff, int sz, const char* name)
{
	return tolua_loadbuffer(l, buff, sz, name);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getfield_wrap(lua_State* l, int idx, const char* field)
{
	return tolua_getfield(l, idx, field);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_setfield_wrap(lua_State* l, int idx, const char* key)
{
	return tolua_setfield(l, idx, key);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_gettable_wrap(lua_State* l, int idx)
{
	return tolua_gettable(l, idx);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_settable_wrap(lua_State* l, int idx)
{
	return tolua_settable(l, idx);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_pushcfunction_wrap(lua_State* l, lua_CFunction fn)
{
	return tolua_pushcfunction(l, fn);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_argerror_wrap(lua_State* l, int narg, const char* extramsg)
{
	return tolua_argerror(l, narg, extramsg);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_error_wrap(lua_State* l, const char* msg)
{
	return tolua_error(l, msg);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getn_wrap(lua_State* l, int i)
{
	return tolua_getn(l, i);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_strlen_wrap(const char* str)
{
	return tolua_strlen(str);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_isvptrtable_wrap(lua_State* l, int index)
{
	return tolua_isvptrtable(l, index);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_setindex_wrap(lua_State* l)
{
	return tolua_setindex(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_setnewindex_wrap(lua_State* l)
{
	return tolua_setnewindex(l);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_pushudata_wrap(lua_State* l, int index)
{
	return tolua_pushudata(l, index);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushnewudata_wrap(lua_State* l, int metaRef, int index)
{
	return tolua_pushnewudata(l, metaRef, index);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_beginmodule_wrap(lua_State* l, const char* name)
{
	return tolua_beginmodule(l, name);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_endmodule_wrap(lua_State* l)
{
	return tolua_endmodule(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_beginclass_wrap(lua_State* l, const char* name, int baseType, int ref)
{
	return tolua_beginclass(l, name, baseType, ref);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_endclass_wrap(lua_State* l)
{
	return tolua_endclass(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_beginenum_wrap(lua_State* l, const char* name)
{
	return tolua_beginenum(l, name);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_endenum_wrap(lua_State* l)
{
	return tolua_endenum(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_beginstaticclass_wrap(lua_State* l, const char* name)
{
	return tolua_beginstaticclass(l, name);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_endstaticclass_wrap(lua_State* l)
{
	return tolua_endstaticclass(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_constant_wrap(lua_State* l, const char* name, double value)
{
	return tolua_constant(l, name, value);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_function_wrap(lua_State* l, const char* name, lua_CFunction fn)
{
	return tolua_function(l, name, fn);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_variable_wrap(lua_State* l, const char* name, lua_CFunction get, lua_CFunction set)
{
	return tolua_variable(l, name, get, set);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() toluaL_ref_wrap(lua_State* l)
{
	return toluaL_ref(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() toluaL_unref_wrap(lua_State* l, int reference)
{
	return toluaL_unref(l, reference);
}

YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() tolua_getmainstate_wrap(lua_State* L1)
{
	return tolua_getmainstate(L1);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getvaluetype_wrap(lua_State* l, int stackPos)
{
	return tolua_getvaluetype(l, stackPos);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_createtable_wrap(lua_State* l, const char* path, int szhint)
{
	return tolua_createtable(l, path, szhint);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_beginpremodule_wrap(lua_State* l, const char* path, int szhint)
{
	return tolua_beginpremodule(l, path, szhint);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_endpremodule_wrap(lua_State* l, int ref)
{
	return tolua_endpremodule(l, ref);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_addpreload_wrap(lua_State* l, const char* path)
{
	return tolua_addpreload(l, path);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getclassref_wrap(lua_State* l, int pos)
{
	return tolua_getclassref(l, pos);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_pushluatable_wrap(lua_State* l, const char* path)
{
	return tolua_pushluatable(l, path);
}

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() tolua_typename_wrap(lua_State* l, int lo)
{
	return tolua_typename(l, lo);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_getmetatableref_wrap(lua_State* l, int pos)
{
	return tolua_getmetatableref(l, pos);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_beginpcall_wrap(lua_State* l, int reference)
{
	return tolua_beginpcall(l, reference);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushtraceback_wrap(lua_State* l)
{
	return tolua_pushtraceback(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_require_wrap(lua_State* l, const char* fileName)
{
	return tolua_require(l, fileName);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_openlualibs_wrap(lua_State* l)
{
	return tolua_openlualibs(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_openlibs_wrap(lua_State* l)
{
	return tolua_openlibs(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_setflag_wrap(int bit, bool flag)
{
	return tolua_setflag(bit, flag);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_getflag_wrap(int bit)
{
	return tolua_getflag(bit);
}

YGGR_DLL_API luaL_Buffer* YGGR_MODULE_CALL_T() tolua_buffinit_wrap(lua_State* l)
{
	return tolua_buffinit(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_addlstring_wrap(luaL_Buffer* b, const char* s, int len)
{
	return tolua_addlstring(b, s, len);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_addstring_wrap(luaL_Buffer* b, const char* s)
{
	return tolua_addstring(b, s);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_addchar_wrap(luaL_Buffer* b, char c)
{
	return tolua_addchar(b, c);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_update_wrap(lua_State* l, float deltaTime, float unscaledTime)
{
	return tolua_update(l, deltaTime, unscaledTime);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_lateupdate_wrap(lua_State* l)
{
	return tolua_lateupdate(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_fixedupdate_wrap(lua_State* l, float fixedTime)
{
	return tolua_fixedupdate(l, fixedTime);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_regthis_wrap(lua_State* l, lua_CFunction get, lua_CFunction set)
{
	return tolua_regthis(l, get, set);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_where_wrap(lua_State* l, int level)
{
	return tolua_where(l, level);
}

YGGR_DLL_API int64_t YGGR_MODULE_CALL_T() tolua_toint64_wrap(lua_State* l, int pos)
{
	return tolua_toint64(l, pos);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_isint64_wrap(lua_State* l, int pos)
{
	return tolua_isint64(l, pos);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_openint64_wrap(lua_State* l)
{
	return tolua_openint64(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() tolua_newint64_wrap(lua_State* l)
{
	return tolua_newint64(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushint64_wrap(lua_State* l, int64_t n)
{
	return tolua_pushint64(l, n);
}

YGGR_DLL_API uint64_t YGGR_MODULE_CALL_T() tolua_touint64_wrap(lua_State* l, int pos)
{
	return tolua_touint64(l, pos);
}

YGGR_DLL_API bool YGGR_MODULE_CALL_T() tolua_isuint64_wrap(lua_State* l, int pos)
{
	return tolua_isuint64(l, pos);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_openuint64_wrap(lua_State* l)
{
	return tolua_openuint64(l);
}

YGGR_DLL_API int  YGGR_MODULE_CALL_T() tolua_newuint64_wrap(lua_State* l)
{
	return tolua_newuint64(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() tolua_pushuint64_wrap(lua_State* l, uint64_t n)
{
	return tolua_pushuint64(l, n);
}