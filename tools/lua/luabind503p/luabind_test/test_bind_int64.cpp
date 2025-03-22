// test_bind_int64.cpp

#include <luabind/luabind.hpp>

#include <boost/cstdint.hpp>

#include <iostream>

#include <luabind/lua_include.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#ifdef _MSC_VER
#	include <luabind/msvc_link.hpp>
#	pragma comment LUABIND_COMPILE_LINK_LIB()
#	pragma comment LINK_LUABIND_IMPL_LIB()
#endif // _MSC_VER

#define USING64 1

const char* get_exception_msg(lua_State* lua_env)
{
	if(!lua_env)
	{
		return "empty lua environment";
	}

	const char* str_err = lua_tostring(lua_env, -1);

	if(!str_err)
	{
		return "lua environment error";
	}

	lua_pop(lua_env, 1 );
	return str_err;
}

int main(int argc, char* argv[])
{
#if USING64
	boost::int64_t int64 = 0;
#else
	boost::int32_t int64 = 0;
#endif // 0, 1

#if USING64
	boost::uint64_t uint64 = 0;
#else
	boost::uint32_t uint64 = 0;
#endif // 0, 1

	lua_State* lua_env = luaL_newstate();

#if (502 <= LUA_VERSION_NUM ) || (defined(LUAJIT_VERSION_NUM))
	luaopen_base(lua_env);
	luaopen_io(lua_env);
	luaopen_os(lua_env);
#else
#	define __LUABIND_TMP_PP_LUAOPEN(__env__, __foo__, __name__) { \
				lua_pushcfunction(__env__, __foo__); \
				lua_pushstring(__env__, __name__); \
				lua_call(__env__, 1, 0); }

	__LUABIND_TMP_PP_LUAOPEN(lua_env, luaopen_base, "");
	__LUABIND_TMP_PP_LUAOPEN(lua_env, luaopen_io, LUA_IOLIBNAME);
	__LUABIND_TMP_PP_LUAOPEN(lua_env, luaopen_os, LUA_OSLIBNAME);

#	undef __LUABIND_TMP_PP_LUAOPEN
#endif // (502 <= LUA_VERSION_NUM ) || (defined(LUAJIT_VERSION_NUM))

	if(0 != luaL_dofile(lua_env, "test_native_int_64.lua"))
	{
		std::cout << get_exception_msg(lua_env) << std::endl;
		goto run_end;
	}

#if USING64
	int64 = static_cast<boost::int64_t>(1) << 40;
#else
	int64 = static_cast<boost::int32_t>(1) << 20;
#endif // 0, 1
	
	try 
	{ 
#if USING64
		std::cout << luabind::call_function<boost::int64_t, boost::int64_t>(lua_env, "calculate_int64", int64) << std::endl; 
#else
		std::cout << luabind::call_function<boost::int32_t, boost::int32_t>(lua_env, "calculate_int64", int64) << std::endl; 
#endif //USING64
	} 
	catch(const luabind::error& e) 
	{ 
		std::cout << e.what() << std::endl;
		std::cout << get_exception_msg(lua_env) << std::endl;
		goto run_end;
	} 
	catch(const luabind::cast_failed& e) 
	{ 
		std::cout << e.what() << std::endl;
		std::cout << get_exception_msg(lua_env) << std::endl;
		goto run_end;
	} 
	catch(...) 
	{ 
		std::cout << get_exception_msg(lua_env) << std::endl;
		goto run_end;
	} 

#if USING64
	uint64 = static_cast<boost::uint64_t>(1) << 40;
#else
	uint64 = static_cast<boost::uint32_t>(1) << 20;
#endif // 0, 1
	
	try 
	{ 
#if USING64
		std::cout << luabind::call_function<boost::uint64_t, boost::uint64_t>(lua_env, "calculate_uint64", uint64) << std::endl; 
#else
		std::cout << luabind::call_function<boost::uint32_t, boost::uint32_t>(lua_env, "calculate_uint64", uint64) << std::endl; 
#endif //USING64
	} 
	catch(const luabind::error& e) 
	{ 
		std::cout << e.what() << std::endl;
		std::cout << get_exception_msg(lua_env) << std::endl;
		goto run_end;
	} 
	catch(const luabind::cast_failed& e) 
	{ 
		std::cout << e.what() << std::endl;
		std::cout << get_exception_msg(lua_env) << std::endl;
		goto run_end;
	} 
	catch(...) 
	{ 
		std::cout << get_exception_msg(lua_env) << std::endl;
		goto run_end;
	} 

run_end:

	if(lua_env)
	{
		lua_close(lua_env);
	}

	char cc = 0;
	std::cin >> cc;
	return 0;
}
