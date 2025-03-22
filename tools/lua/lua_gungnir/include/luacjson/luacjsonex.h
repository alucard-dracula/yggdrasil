//luacjsonex.h

#ifndef __LUACJSONEX_H__
#define __LUACJSONEX_H__

#include <lua.h>

LUALIB_API int luaopen_cjson(lua_State *l);
LUALIB_API int luaopen_cjson_safe(lua_State *l);

#endif // __LUACJSONEX_H__
