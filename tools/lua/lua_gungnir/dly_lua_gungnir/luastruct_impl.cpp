// luastruct_impl.cpp

#include "luastruct_decl.hpp"
#include <luastruct.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_struct_wrap(lua_State* l)
{
	return luaopen_struct(l);
}