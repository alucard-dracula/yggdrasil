// lua_gungnir.cpp

#include "lua_decl.hpp"
#include "luabit_decl.hpp"
#include "luacjson_decl.hpp"
#include "lualpeg_decl.hpp"
#include "luapb_decl.hpp"
#include "luasocket_decl.hpp"
#include "tolua_decl.hpp"

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#if defined(_MSC_VER)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(msc99rt)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(luabit)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(luacjson)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(lualpeg)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(luapb)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(luasocket)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(luastruct)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(tolua)
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(script_lua_ss)
#include YGGR_PP_LINK_LIB(network)

#include <yggr/dll_make/dll_main.ipp>
