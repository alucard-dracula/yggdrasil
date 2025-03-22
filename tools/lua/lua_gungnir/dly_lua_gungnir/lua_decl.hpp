// lua_decl.hpp

#ifndef __DLY_LUA_GUNGNIR_LUA_HPP__
#define __DLY_LUA_GUNGNIR_LUA_HPP__

#include "call_convention_def.hpp"
#include <lua.hpp>

#include <yggr/dll_make/dll_mark_define.hpp>

#include <yggr/dll_make/dll_make_begin.ipp>

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_ffi_wrap(lua_State* l);

// state manipulation

#if LJ_64 && !LJ_GC64
YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() lua_newstate_wrap(lua_Alloc f, void *ud)
#endif // LJ_64 && !LJ_GC64

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_close_wrap(lua_State* l);

YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() lua_newthread_wrap(lua_State* l);

YGGR_DLL_API lua_CFunction YGGR_MODULE_CALL_T() lua_atpanic_wrap(lua_State* l, lua_CFunction panicf);


// basic stack manipulation
YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_gettop_wrap(lua_State* l);
       
YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_settop_wrap(lua_State* l, int top);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushvalue_wrap(lua_State* l, int idx);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_remove_wrap(lua_State* l, int idx);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_insert_wrap(lua_State* l, int idx);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_replace_wrap(lua_State* l, int index);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_checkstack_wrap(lua_State* l, int extra);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_xmove_wrap(lua_State* from, lua_State* to, int n);

// access functions (stack -> C)
YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_isnumber_wrap(lua_State* l, int idx);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_isstring_wrap(lua_State* l, int idx);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_iscfunction_wrap(lua_State* l, int idx);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_isuserdata_wrap(lua_State* l, int idx);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_type_wrap(lua_State* l, int idx);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_equal_wrap(lua_State* l, int idx1, int idx2);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_rawequal_wrap(lua_State* l, int idx1, int idx2);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_lessthan_wrap(lua_State* l, int idx1, int idx2);

YGGR_DLL_API lua_Number YGGR_MODULE_CALL_T() lua_tonumber_wrap(lua_State* l, int idx);

YGGR_DLL_API lua_CFunction YGGR_MODULE_CALL_T() lua_tocfunction_wrap(lua_State* l, int idx);

YGGR_DLL_API void* YGGR_MODULE_CALL_T() lua_touserdata_wrap(lua_State* l, int idx);

YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() lua_tothread_wrap(lua_State* l, int idx);

YGGR_DLL_API const void* YGGR_MODULE_CALL_T() lua_topointer_wrap(lua_State* l, int idx);


// push functions (C -> stack)
YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushnil_wrap(lua_State* l);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushnumber_wrap(lua_State* l, lua_Number n);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushlstring_wrap(lua_State* l, const char *s, size_t len);     

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushstring_wrap(lua_State* l, const char *s);       
 
YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushcclosure_wrap(lua_State* l, lua_CFunction fn, int n);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushboolean_wrap(lua_State* l, int b);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushlightuserdata_wrap(lua_State* l, void* p);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_pushthread_wrap(lua_State* l);

// get functions (Lua -> stack)

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_rawget_wrap(lua_State* l, int idx);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_rawgeti_wrap(lua_State* l, int idx, int n);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_createtable_wrap(lua_State* l, int narr, int nrec);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_getmetatable_wrap(lua_State* l, int objindex);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_getfenv_wrap(lua_State* l, int idx);

// set functions (stack -> Lua)

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_rawset_wrap(lua_State* l, int idx);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_rawseti_wrap(lua_State* l, int idx, int n);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_setmetatable_wrap(lua_State* l, int objindex);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_setfenv_wrap(lua_State* l, int idx);

// `load' and `call' functions (load and run Lua code)

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_call_wrap(lua_State* l, int nargs, int nresults);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_pcall_wrap(lua_State* l, int nargs, int nresults, int errfunc);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_cpcall_wrap(lua_State* l, lua_CFunction func, void* ud);

// coroutine functions

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_yield_wrap(lua_State* l, int nresults);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_resume_wrap(lua_State* l, int narg);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_status_wrap(lua_State* l);

// garbage-collection function and options

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_gc_wrap(lua_State* l, int what, int data);

// miscellaneous functions

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_next_wrap(lua_State* l, int idx);

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_concat_wrap(lua_State* l, int n);

/*
** {======================================================================
** Debug API
** =======================================================================
*/

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_getstack_wrap(lua_State* l, int level, lua_Debug* ar);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_getinfo_wrap(lua_State* l, const char* what, lua_Debug* ar);

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() lua_getlocal_wrap(lua_State* l, const lua_Debug* ar, int n);

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() lua_setlocal_wrap(lua_State* l, const lua_Debug* ar, int n);

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() lua_getupvalue_wrap(lua_State* l, int funcindex, int n);

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() lua_setupvalue_wrap(lua_State* l, int funcindex, int n);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_sethook_wrap(lua_State* l, lua_Hook func, int mask, int count);

YGGR_DLL_API lua_Hook YGGR_MODULE_CALL_T() lua_gethook_wrap(lua_State* l);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_gethookmask_wrap(lua_State* l);

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_gethookcount_wrap(lua_State* l);

//lualib.h
YGGR_DLL_API void YGGR_MODULE_CALL_T() luaL_openlibs_wrap(lua_State* l);

//lauxlib.h
YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_getmetafield_wrap(lua_State* l, int obj, const char* e);

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_newmetatable_wrap(lua_State* l, const char* tname);

YGGR_DLL_API void YGGR_MODULE_CALL_T() luaL_where_wrap(lua_State* l, int lvl);

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_ref_wrap(lua_State* l, int t);

YGGR_DLL_API void YGGR_MODULE_CALL_T() luaL_unref_wrap(lua_State* l, int t, int ref);

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_loadfile_wrap(lua_State* l, const char* filename);

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_loadstring_wrap(lua_State* l, const char* s);

YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() luaL_newstate_wrap(void);

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() luaL_gsub_wrap(lua_State* l, const char* s, const char* p, const char* r);

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() luaL_findtable_wrap(lua_State* l, int idx, const char* fname, int szhint);

#include <yggr/dll_make/dll_make_end.ipp>

#endif // __DLY_LUA_GUNGNIR_LUA_HPP__