//ref.hpp

#ifndef __LUAJIT_COMPATIBILITY_REF_H__
#define __LUAJIT_COMPATIBILITY_REF_H__

#ifdef lauxlib_h
#	ifndef lua_unref
#		define lua_unref(L,ref)        luaL_unref(L, LUA_REGISTRYINDEX, (ref))
#	endif // lua_unref

#	ifndef lua_getref
#		define lua_getref(L,ref)       lua_rawgeti(L, LUA_REGISTRYINDEX, (ref))
#	endif // lua_getref
#endif // lauxlib_h

#endif // __LUAJIT_COMPATIBILITY_REF_H__
