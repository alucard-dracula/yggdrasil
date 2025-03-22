// luabit_impl.cpp

#include "luabit_decl.hpp"
#include <luabit.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

YGGR_DLL_API int YGGR_MODULE_CALL_T() luaopen_bit_wrap(lua_State* l)
{
	return luaopen_bit(l);
}