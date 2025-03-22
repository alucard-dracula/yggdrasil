// lua_impl.cpp

#include "lua_decl.hpp"
#include <lua.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_ffi_wrap(lua_State* l)
{
	return luaopen_ffi(l);
}

// state manipulation
#if LJ_64 && !LJ_GC64
YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() lua_newstate_wrap(lua_Alloc f, void *ud)
{
	return lua_newstate(f, ud);
}
#endif // LJ_64 && !LJ_GC64

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_close_wrap(lua_State* l)
{
	return lua_close(l);
}

YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() lua_newthread_wrap(lua_State* l)
{
	return lua_newthread(l);
}

YGGR_DLL_API lua_CFunction YGGR_MODULE_CALL_T() lua_atpanic_wrap(lua_State* l, lua_CFunction panicf)
{
	return lua_atpanic(l, panicf);
}

// basic stack manipulation
YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_gettop_wrap(lua_State* l)
{
	return lua_gettop(l);
}
       
YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_settop_wrap(lua_State* l, int top)
{
	return lua_settop(l, top);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushvalue_wrap(lua_State* l, int idx)
{
	return lua_pushvalue(l, idx);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_remove_wrap(lua_State* l, int idx)
{
	return lua_remove(l, idx);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_insert_wrap(lua_State* l, int idx)
{
	return lua_insert(l, idx);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_replace_wrap(lua_State* l, int index)
{
	return lua_replace(l, index);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_checkstack_wrap(lua_State* l, int extra)
{
	return lua_checkstack(l, extra);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_xmove_wrap(lua_State* from, lua_State* to, int n)
{
	return lua_xmove(from, to, n);
}

// access functions (stack -> C)
YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_isnumber_wrap(lua_State* l, int idx)
{
	return lua_isnumber(l, idx);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_isstring_wrap(lua_State* l, int idx)
{
	return lua_isstring(l, idx);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_iscfunction_wrap(lua_State* l, int idx)
{
	return lua_iscfunction(l, idx);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_isuserdata_wrap(lua_State* l, int idx)
{
	return lua_isuserdata(l, idx);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_type_wrap(lua_State* l, int idx)
{
	return lua_type(l, idx);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_equal_wrap(lua_State* l, int idx1, int idx2)
{
	return lua_equal(l, idx1, idx2);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_rawequal_wrap(lua_State* l, int idx1, int idx2)
{
	return lua_rawequal(l, idx1, idx2);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_lessthan_wrap(lua_State* l, int idx1, int idx2)
{
	return lua_lessthan(l, idx1, idx2);
}

YGGR_DLL_API double YGGR_MODULE_CALL_T() lua_tonumber_wrap(lua_State* l, int idx)
{
	return lua_tonumber(l, idx);
}

YGGR_DLL_API lua_CFunction YGGR_MODULE_CALL_T() lua_tocfunction_wrap(lua_State* l, int idx)
{
	return lua_tocfunction(l, idx);
}

YGGR_DLL_API void* YGGR_MODULE_CALL_T() lua_touserdata_wrap(lua_State* l, int idx)
{
	return lua_touserdata(l, idx);
}

YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() lua_tothread_wrap(lua_State* l, int idx)
{
	return lua_tothread(l, idx);
}

YGGR_DLL_API const void* YGGR_MODULE_CALL_T() lua_topointer_wrap(lua_State* l, int idx)
{
	return lua_topointer(l, idx);
}

// push functions (C -> stack)
YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushnil_wrap(lua_State* l)
{
	return lua_pushnil(l);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushnumber_wrap(lua_State* l, lua_Number n)
{
	return lua_pushnumber(l, n);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushlstring_wrap(lua_State* l, const char *s, size_t len)
{
	return lua_pushlstring(l, s, len);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushstring_wrap(lua_State* l, const char *s)
{
	return lua_pushstring(l, s);
}
 
YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushcclosure_wrap(lua_State* l, lua_CFunction fn, int n)
{
	return lua_pushcclosure(l, fn, n);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushboolean_wrap(lua_State* l, int b)
{
	return lua_pushboolean(l, b);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_pushlightuserdata_wrap(lua_State* l, void* p)
{
	return lua_pushlightuserdata(l, p);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_pushthread_wrap(lua_State* l)
{
	return lua_pushthread(l);
}

// get functions (Lua -> stack)

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_rawget_wrap(lua_State* l, int idx)
{
	return lua_rawget(l, idx);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_rawgeti_wrap(lua_State* l, int idx, int n)
{
	return lua_rawgeti(l, idx, n);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_createtable_wrap(lua_State* l, int narr, int nrec)
{
	return lua_createtable(l, narr, nrec);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_getmetatable_wrap(lua_State* l, int objindex)
{
	return lua_getmetatable(l, objindex);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_getfenv_wrap(lua_State* l, int idx)
{
	return lua_getfenv(l, idx);
}

// set functions (stack -> Lua)

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_rawset_wrap(lua_State* l, int idx)
{
	return lua_rawset(l, idx);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_rawseti_wrap(lua_State* l, int idx, int n)
{
	return lua_rawseti(l, idx, n);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_setmetatable_wrap(lua_State* l, int objindex)
{
	return lua_setmetatable(l, objindex);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_setfenv_wrap(lua_State* l, int idx)
{
	return lua_setfenv(l, idx);
}

// `load' and `call' functions (load and run Lua code)

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_call_wrap(lua_State* l, int nargs, int nresults)
{
	return lua_call(l, nargs, nresults);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_pcall_wrap(lua_State* l, int nargs, int nresults, int errfunc)
{
	return lua_pcall(l, nargs, nresults, errfunc);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_cpcall_wrap(lua_State* l, lua_CFunction func, void* ud)
{
	return lua_cpcall(l, func, ud);
}

// coroutine functions

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_yield_wrap(lua_State* l, int nresults)
{
	return lua_yield(l, nresults);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_resume_wrap(lua_State* l, int narg)
{
	return lua_resume(l, narg);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_status_wrap(lua_State* l)
{
	return lua_status(l);
}

// garbage-collection function and options

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_gc_wrap(lua_State* l, int what, int data)
{
	return lua_gc(l, what, data);
}

// miscellaneous functions
YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_next_wrap(lua_State* l, int idx)
{
	return lua_next(l, idx);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() lua_concat_wrap(lua_State* l, int n)
{
	return lua_concat(l, n);
}

/*
** {======================================================================
** Debug API
** =======================================================================
*/

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_getstack_wrap(lua_State* l, int level, lua_Debug* ar)
{
	return lua_getstack(l, level, ar);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_getinfo_wrap(lua_State* l, const char* what, lua_Debug* ar)
{
	return lua_getinfo(l, what, ar);
}

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() lua_getlocal_wrap(lua_State* l, const lua_Debug* ar, int n)
{
	return lua_getlocal(l, ar, n);
}

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() lua_setlocal_wrap(lua_State* l, const lua_Debug* ar, int n)
{
	return lua_setlocal(l, ar, n);
}

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() lua_getupvalue_wrap(lua_State* l, int funcindex, int n)
{
	return lua_getupvalue(l, funcindex, n);
}

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() lua_setupvalue_wrap(lua_State* l, int funcindex, int n)
{
	return lua_setupvalue(l, funcindex, n);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_sethook_wrap(lua_State* l, lua_Hook func, int mask, int count)
{
	return lua_sethook(l, func, mask, count);
}

YGGR_DLL_API lua_Hook YGGR_MODULE_CALL_T() lua_gethook_wrap(lua_State* l)
{
	return lua_gethook(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_gethookmask_wrap(lua_State* l)
{
	return lua_gethookmask(l);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() lua_gethookcount_wrap(lua_State* l)
{
	return lua_gethookcount(l);
}

//lualib.h
YGGR_DLL_API void YGGR_MODULE_CALL_T() luaL_openlibs_wrap(lua_State* l)
{
	return luaL_openlibs(l);
}

//lauxlib.h
YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_getmetafield_wrap(lua_State* l, int obj, const char* e)
{
	return luaL_getmetafield(l, obj, e);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_newmetatable_wrap(lua_State* l, const char* tname)
{
	return luaL_newmetatable(l, tname);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() luaL_where_wrap(lua_State* l, int lvl)
{
	return luaL_where(l, lvl);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_ref_wrap(lua_State* l, int t)
{
	return luaL_ref(l, t);
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() luaL_unref_wrap(lua_State* l, int t, int ref)
{
	return luaL_unref(l, t, ref);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_loadfile_wrap(lua_State* l, const char* filename)
{
	return luaL_loadfile(l, filename);
}

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaL_loadstring_wrap(lua_State* l, const char* s)
{
	return luaL_loadstring(l, s);
}

YGGR_DLL_API lua_State* YGGR_MODULE_CALL_T() luaL_newstate_wrap(void)
{
	return luaL_newstate();
}

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() luaL_gsub_wrap(lua_State* l, const char* s, const char* p, const char* r)
{
	return luaL_gsub(l, s, p, r);
}

YGGR_DLL_API const char* YGGR_MODULE_CALL_T() luaL_findtable_wrap(lua_State* l, int idx, const char* fname, int szhint)
{
	return luaL_findtable(l, idx, fname, szhint);
}