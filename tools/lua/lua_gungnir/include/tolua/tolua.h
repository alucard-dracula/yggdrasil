#ifndef tolua_h
#define tolua_h

#include <lua.h>

#ifndef _MSC_VER
#	include <stdint.h>
#else
#	if _MSC_VER < 1600
#		include <mscompatibility/stdint.h>
#	else
#		include <stdint.h>
#	endif //_MSC_VER < 1600
#endif // _MSC_VER

#if !defined(LUA_RIDX_MAINTHREAD)
#define LUA_RIDX_MAINTHREAD	1
#endif

#if !defined(LUA_RIDX_GLOBALS)
#define LUA_RIDX_GLOBALS 2
#endif

#define LUA_RIDX_TRACEBACK 			3
#define LUA_RIDX_UBOX 				4
#define LUA_RIDX_FIXEDMAP			5
#define LUA_RIDX_CHECKVALUE			6
#define LUA_RIDX_PACKVEC3			7
#define LUA_RIDX_UNPACKVEC3			8
#define LUA_RIDX_PACKVEC2 			9
#define LUA_RIDX_UNPACKVEC2			10
#define LUA_RIDX_PACKVEC4			11
#define LUA_RIDX_UNPACKVEC4			12
#define LUA_RIDX_PACKQUAT			13
#define LUA_RIDX_UNPACKQUAT			14
#define LUA_RIDX_PACKCLR			15
#define LUA_RIDX_UNPACKCLR			16
#define LUA_RIDX_PACKLAYERMASK      17
#define LUA_RIDX_UNPACKLAYERMASK    18
#define LUA_RIDX_REQUIRE            19
#define LUA_RIDX_INT64              20
#define LUA_RIDX_VPTR               21
#define LUA_RIDX_UPDATE				22
#define LUA_RIDX_LATEUPDATE			23
#define LUA_RIDX_FIXEDUPDATE		24
#define LUA_RIDX_PRELOAD			25
#define LUA_RIDX_LOADED				26
#define LUA_RIDX_UINT64				27
#define LUA_RIDX_CUSTOMTRACEBACK 	28

#define LUA_NULL_USERDATA 	1
#define TOLUA_NOPEER    	LUA_REGISTRYINDEX 		
#define FLAG_INDEX_ERROR 	1
#define FLAG_INT64       	2

#define MAX_ITEM 512

#define abs_index(L, i)  ((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : lua_gettop(L) + (i) + 1)

LUALIB_API void* tolua_tag();
LUALIB_API void tolua_newudata(lua_State *L, int val);
LUALIB_API int tolua_rawnetobj(lua_State *L, int index);

LUALIB_API char* tolua_tocbuffer(const char *csBuffer, int sz);
LUALIB_API void tolua_freebuffer(void* buffer);

LUALIB_API void tolua_getvec2(lua_State *L, int pos, float* x, float* y);
LUALIB_API void tolua_getvec3(lua_State *L, int pos, float* x, float* y, float* z);
LUALIB_API void tolua_getvec4(lua_State *L, int pos, float* x, float* y, float* z, float* w);
LUALIB_API void tolua_getquat(lua_State *L, int pos, float* x, float* y, float* z, float* w);
LUALIB_API void tolua_getclr(lua_State *L, int pos, float* r, float* g, float* b, float* a);
LUALIB_API int tolua_getlayermask(lua_State *L, int pos);

LUALIB_API void tolua_pushvec2(lua_State *L, float x, float y);
LUALIB_API void tolua_pushvec3(lua_State *L, float x, float y, float z);
LUALIB_API void tolua_pushvec4(lua_State *L, float x, float y, float z, float w);
LUALIB_API void tolua_pushquat(lua_State *L, float x, float y, float z, float w);
LUALIB_API void tolua_pushclr(lua_State *L, float r, float g, float b, float a);
LUALIB_API void tolua_pushlayermask(lua_State *L, int mask);

LUA_API const char* tolua_tolstring(lua_State *L, int index, int* len);
LUA_API void tolua_pushlstring(lua_State *L, const char *s, int l);

LUA_API void* tolua_newuserdata(lua_State *L, int sz);

LUA_API int tolua_objlen(lua_State *L, int idx);

LUA_API bool tolua_toboolean(lua_State *L, int idx);
LUA_API int32_t tolua_tointeger(lua_State *L, int idx);

LUALIB_API int tolua_loadbuffer(lua_State *L, const char *buff, int sz, const char *name);

LUA_API int tolua_getfield(lua_State *L, int idx, const char *field);
LUA_API int tolua_setfield(lua_State *L, int idx, const char *key);

LUA_API int tolua_gettable(lua_State *L, int idx);
LUA_API int tolua_settable(lua_State *L, int idx);

LUA_API int tolua_pushcfunction(lua_State *L, lua_CFunction fn);
LUALIB_API int tolua_argerror(lua_State *L, int narg, const char *extramsg);

LUALIB_API int tolua_error(lua_State *L, const char *msg);
LUALIB_API int tolua_getn(lua_State *L, int i);

LUALIB_API int tolua_strlen(const char *str);

LUALIB_API bool tolua_isvptrtable(lua_State *L, int index);

LUALIB_API void tolua_setindex(lua_State *L);

LUALIB_API void tolua_setnewindex(lua_State *L);
LUALIB_API bool tolua_pushudata(lua_State *L, int index);
LUALIB_API void tolua_pushnewudata(lua_State *L, int metaRef, int index);

LUALIB_API bool tolua_beginmodule(lua_State *L, const char *name);
LUALIB_API void tolua_endmodule(lua_State *L);

LUALIB_API int tolua_beginclass(lua_State *L, const char *name, int baseType, int ref);
LUALIB_API void tolua_endclass(lua_State *L);

LUALIB_API int tolua_beginenum(lua_State *L, const char *name);
LUALIB_API void tolua_endenum(lua_State *L);

LUALIB_API void tolua_beginstaticclass(lua_State *L, const char *name);
LUALIB_API void tolua_endstaticclass(lua_State *L);

LUALIB_API void tolua_constant(lua_State *L, const char *name, double value);
LUALIB_API void tolua_function(lua_State *L, const char *name, lua_CFunction fn);

LUALIB_API void tolua_variable(lua_State *L, const char *name, lua_CFunction get, lua_CFunction set);
LUALIB_API int toluaL_ref(lua_State *L);
LUALIB_API void toluaL_unref(lua_State *L, int reference);

LUA_API lua_State* tolua_getmainstate(lua_State *L1);
LUA_API int tolua_getvaluetype(lua_State *L, int stackPos);

LUALIB_API bool tolua_createtable(lua_State *L, const char *path, int szhint);

LUALIB_API bool tolua_beginpremodule(lua_State *L, const char *path, int szhint);
LUALIB_API bool tolua_endpremodule(lua_State *L, int ref);

LUALIB_API bool tolua_addpreload(lua_State *L, const char *path);
LUALIB_API int tolua_getclassref(lua_State *L, int pos);

LUALIB_API bool tolua_pushluatable(lua_State *L, const char *path);
LUALIB_API const char* tolua_typename(lua_State *L, int lo);

LUALIB_API int tolua_getmetatableref(lua_State *L, int pos);

LUALIB_API int tolua_beginpcall(lua_State *L, int reference);
LUALIB_API void tolua_pushtraceback(lua_State *L);

LUALIB_API int tolua_require(lua_State *L, const char *fileName);

LUALIB_API int tolua_openlualibs(lua_State *L);
LUALIB_API void tolua_openlibs(lua_State *L);

LUALIB_API void tolua_setflag(int bit, bool flag);
LUALIB_API bool tolua_getflag(int bit);

LUALIB_API luaL_Buffer* tolua_buffinit(lua_State *L);
LUALIB_API void tolua_addlstring(luaL_Buffer *b, const char *s, int l);
LUALIB_API void tolua_addstring(luaL_Buffer *b, const char *s);
LUALIB_API void tolua_addchar(luaL_Buffer *b, char c);

LUALIB_API int tolua_update(lua_State *L, float deltaTime, float unscaledTime);
LUALIB_API int tolua_lateupdate(lua_State *L);
LUALIB_API int tolua_fixedupdate(lua_State *L, float fixedTime);

LUALIB_API void tolua_regthis(lua_State *L, lua_CFunction get, lua_CFunction set);
LUALIB_API int tolua_where(lua_State *L, int level);

LUALIB_API int64_t tolua_toint64(lua_State* L, int pos);
LUALIB_API bool tolua_isint64(lua_State* L, int pos);
LUALIB_API void tolua_openint64(lua_State* L);
LUALIB_API int  tolua_newint64(lua_State* L);
LUALIB_API void tolua_pushint64(lua_State* L, int64_t n);

LUALIB_API uint64_t tolua_touint64(lua_State *L, int pos);
LUALIB_API bool tolua_isuint64(lua_State *L, int pos);
LUALIB_API void tolua_openuint64(lua_State* L);
LUALIB_API int  tolua_newuint64(lua_State* L);
LUALIB_API void tolua_pushuint64(lua_State *L, uint64_t n);

extern int toluaflags;

#endif