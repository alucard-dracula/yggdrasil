// lua_export_any_test.cpp

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/script/lua_script.hpp>
//#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/any_val/any_set.hpp>
#include <yggr/any_val/any.hpp>
#include <yggr/charset/string.hpp>

//#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
//#include <luabind/lua_include.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)

// Use _TOOL_IGNORE to make the conversion tool ignore

#include YGGR_PP_LINK_LIB(script_lua)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_s)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_ss)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_ss_ts)

typedef yggr::script::script_mgr<yggr::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

const char* lua_any_inner_type_name(const yggr::any& data)
{
	return data.type().name();
}

void export_any(lua_State* l)
{
	typedef yggr::any any_type;

	yggr_test_assert(l);
	if(!l)
	{
		return;
	}

	luabind::module(l)
	[
		luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
		[
			luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
			[

				luabind::class_< any_type >( YGGR_PP_CAST_TO_STRING(any_type) )
					.def( luabind::constructor< >() )
					.def( luabind::constructor<yggr::s32>() )
					.def( luabind::constructor<const yggr::string&>() )
					.def( luabind::constructor<const any_type& >() )

					.def( "swap", static_cast< void ( any_type::* )( any_type& ) >( &any_type::swap ) )
					.def( "empty", &any_type::empty )
					.def( "inner_type", &lua_any_inner_type_name ),

				luabind::def(
					"any_cast_s32", 
					static_cast<yggr::s32(*)(const any_type&)>(
						&yggr::unsafe_any_cast<yggr::s32, any_type>)),

				luabind::def(
					"any_set_s32", 
					static_cast<any_type&(any_type::*)(const yggr::s32&)>(
						&any_type::operator=<yggr::s32>)),

				luabind::def(
					"any_cast_u32", 
					static_cast<yggr::u32(*)(const any_type&)>(
						&yggr::unsafe_any_cast<yggr::u32, any_type>)),

				luabind::def(
					"any_set_u32", 
					static_cast<any_type&(any_type::*)(const yggr::u32&)>(
						&any_type::operator=<yggr::u32>)),

				luabind::def(
					"any_cast_s64", 
					static_cast<yggr::s64(*)(const any_type&)>(
						&yggr::unsafe_any_cast<yggr::s64, any_type>)),

				luabind::def(
					"any_set_s64", 
					static_cast<any_type&(any_type::*)(const yggr::s64&)>(
						&any_type::operator=<yggr::s64>)),

				luabind::def(
					"any_cast_u64", 
					static_cast<yggr::u64(*)(const any_type&)>(
						&yggr::unsafe_any_cast<yggr::u64, any_type>)),

				luabind::def(
					"any_set_u64", 
					static_cast<any_type&(any_type::*)(const yggr::u64&)>(
						&any_type::operator=<yggr::u64>)),

				luabind::def(
					"any_cast_string", 
					static_cast<const yggr::string&(*)(const any_type&)>(
						&yggr::unsafe_any_cast<const yggr::string&, any_type>)),

				luabind::def(
					"any_set_string", 
					static_cast<any_type&(any_type::*)(const yggr::string&)>(
						&any_type::operator=<yggr::string>))
			]
		]
	];

}

void lua_append_search_path(lua_State* l, const yggr::string& path)
{
	yggr::utf8_string tmp;

	if(path.empty())
	{
		return;
	}
	
	lua_getglobal(l, "package");
	lua_setfield(l, -1, "path");
	tmp += lua_tostring(l, -1);
	tmp += ";";
	tmp += path;
	lua_pushstring(l, tmp.c_str());
	lua_setfield(l, -3, "path");
	lua_pop(l, 2);
}

void lua_append_search_cpath(lua_State* l, const yggr::string& path)
{
	yggr::utf8_string tmp;
	
	if(path.empty())
	{
		return;
	}

	lua_getglobal(l, "package");
	lua_setfield(l, -1, "cpath");
	tmp += lua_tostring(l, -1);
	tmp += ";";
	tmp += path;
	lua_pushstring(l, tmp.c_str());
	lua_setfield(l, -3, "cpath");
	lua_pop(l, 2);
}

void export_all(lua_State* l, const yggr::string& path)
{
	yggr_test_assert(l);
	if(!l)
	{
		return;
	}
	
#ifdef _DEBUG
	std::cout << "luaopen_yggr_lua_base_type" << std::endl;
#endif //_DEBUG

	luabind::open(l);

	// test but not append search_path
	lua_append_search_path(l, path);
	lua_append_search_cpath(l, path);

	export_any(l);
}


void exec_test_s32()
{
	typedef yggr::any any_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code(
					"inner_any_data = yggr.lua.any_type(100)\r\n"
					"function calculate(param)\r\n"
						"print('------lua_test_s32 call start------')\r\n"
						"print('param_inner_type: ' .. tostring(param:inner_type()))\r\n"
						"print('param_data: ' .. yggr.lua.any_cast_s32(param))\r\n"
						"print('inner_data_inner_type: ' .. tostring(inner_any_data:inner_type()))\r\n"
						"print('inner_data: ' .. yggr.lua.any_cast_s32(inner_any_data))\r\n"
						"yggr.lua.any_set_s32(param, 10000)\r\n"
						"print('------lua_test_s32 call return------')\r\n"
						"return inner_any_data\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0, 
							boost::bind(&export_all, _1, yggr::string("")));
	yggr_test_assert(bright);

	any_type na = ANY_SET(yggr::s32, 999); 
	any_type ra ;
	
	bright = mgr.execute_rule<any_type>(ra, yggr::const_args_anchor("test"), boost::ref(na));
	yggr_test_assert(bright);

	yggr_test_assert(yggr::any_cast<yggr::s32>(ra) == 100);
	yggr_test_assert(yggr::any_cast<yggr::s32>(na) == 10000);

	std::cout << yggr::any_cast<yggr::s32>(ra) << std::endl;
	std::cout << yggr::any_cast<yggr::s32>(na) << std::endl;

	std::cout << "------exec_test_s32 end------\n" << std::endl;
}

void exec_test_u32()
{
	typedef yggr::any any_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code(
					"inner_any_data = yggr.lua.any_type()\r\n"
					"yggr.lua.any_set_u32(inner_any_data, 100)\r\n"
					"function calculate(param)\r\n"
						"print('------lua_test_u32 call start------')\r\n"
						"print('param_inner_type: ' .. tostring(param:inner_type()))\r\n"
						"print('param_data: ' .. yggr.lua.any_cast_u32(param))\r\n"
						"print('inner_data_inner_type: ' .. tostring(inner_any_data:inner_type()))\r\n"
						"print('inner_data: ' .. yggr.lua.any_cast_u32(inner_any_data))\r\n"
						"yggr.lua.any_set_u32(param, 10000)\r\n"
						"print('------lua_test_u32 call return------')\r\n"
						"return inner_any_data\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0, 
							boost::bind(&export_all, _1, yggr::string("")));
	yggr_test_assert(bright);

	any_type na = ANY_SET(yggr::u32, 999); 
	any_type ra ;
	
	bright = mgr.execute_rule<any_type>(ra, yggr::const_args_anchor("test"), boost::ref(na));
	yggr_test_assert(bright);

	yggr_test_assert(yggr::any_cast<yggr::u32>(ra) == 100);
	yggr_test_assert(yggr::any_cast<yggr::u32>(na) == 10000);

	std::cout << yggr::any_cast<yggr::u32>(ra) << std::endl;
	std::cout << yggr::any_cast<yggr::u32>(na) << std::endl;

	std::cout << "------exec_test_u32 end------\n" << std::endl;
}

void exec_test_s64()
{
	typedef yggr::any any_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code(
					"inner_any_data = yggr.lua.any_type()\r\n"
					"yggr.lua.any_set_s64(inner_any_data, 100)\r\n"
					"function calculate(param)\r\n"
						"print('------lua_test_s64 call start------')\r\n"
						"print('param_inner_type: ' .. tostring(param:inner_type()))\r\n"
						"print('param_data: ' .. yggr.lua.any_cast_s64(param))\r\n"
						"print('inner_data_inner_type: ' .. tostring(inner_any_data:inner_type()))\r\n"
						"print('inner_data: ' .. yggr.lua.any_cast_s64(inner_any_data))\r\n"
						"yggr.lua.any_set_s64(param, 10000)\r\n"
						"print('------lua_test_s64 call return------')\r\n"
						"return inner_any_data\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0, 
							boost::bind(&export_all, _1, yggr::string("")));
	yggr_test_assert(bright);

	any_type na = ANY_SET(yggr::s64, 999); 
	any_type ra ;
	
	bright = mgr.execute_rule<any_type>(ra, yggr::const_args_anchor("test"), boost::ref(na));
	yggr_test_assert(bright);

	yggr_test_assert(yggr::any_cast<yggr::s64>(ra) == 100);
	yggr_test_assert(yggr::any_cast<yggr::s64>(na) == 10000);

	std::cout << yggr::any_cast<yggr::s64>(ra) << std::endl;
	std::cout << yggr::any_cast<yggr::s64>(na) << std::endl;

	std::cout << "------exec_test_s64 end------\n" << std::endl;
}

void exec_test_u64()
{
	typedef yggr::any any_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code(
					"inner_any_data = yggr.lua.any_type()\r\n"
					"yggr.lua.any_set_u64(inner_any_data, 100)\r\n"
					"function calculate(param)\r\n"
						"print('------lua_test_u64 call start------')\r\n"
						"print('param_inner_type: ' .. tostring(param:inner_type()))\r\n"
						"print('param_data: ' .. yggr.lua.any_cast_u64(param))\r\n"
						"print('inner_data_inner_type: ' .. tostring(inner_any_data:inner_type()))\r\n"
						"print('inner_data: ' .. yggr.lua.any_cast_u64(inner_any_data))\r\n"
						"yggr.lua.any_set_u64(param, 10000)\r\n"
						"print('------lua_test_u64 call return------')\r\n"
						"return inner_any_data\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0, 
							boost::bind(&export_all, _1, yggr::string("")));
	yggr_test_assert(bright);

	any_type na = ANY_SET(yggr::u64, 999); 
	any_type ra ;
	
	bright = mgr.execute_rule<any_type>(ra, yggr::const_args_anchor("test"), boost::ref(na));
	yggr_test_assert(bright);

	yggr_test_assert(yggr::any_cast<yggr::u64>(ra) == 100);
	yggr_test_assert(yggr::any_cast<yggr::u64>(na) == 10000);

	std::cout << yggr::any_cast<yggr::u64>(ra) << std::endl;
	std::cout << yggr::any_cast<yggr::u64>(na) << std::endl;

	std::cout << "------exec_test_u64 end------\n" << std::endl;
}

void exec_test_string()
{
	typedef yggr::any any_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code(
					"inner_any_data = yggr.lua.any_type('lua_any_data')\r\n"
					"function calculate(param)\r\n"
						"print('------lua_test_string call start------')\r\n"
						"print('foo any_cast_s32: ' .. tostring(yggr.lua.any_cast_s32))\r\n"
						"print('foo any_set_s32: ' .. tostring(yggr.lua.any_set_s32))\r\n"
						"print('foo any_cast_u32: ' .. tostring(yggr.lua.any_cast_u32))\r\n"
						"print('foo any_set_u32: ' .. tostring(yggr.lua.any_set_u32))\r\n"
						"print('foo any_cast_s64: ' .. tostring(yggr.lua.any_cast_s64))\r\n"
						"print('foo any_set_s64: ' .. tostring(yggr.lua.any_set_s64))\r\n"
						"print('foo any_cast_u64: ' .. tostring(yggr.lua.any_cast_u64))\r\n"
						"print('foo any_set_u64: ' .. tostring(yggr.lua.any_set_u64))\r\n"
						"print('foo any_cast_string: ' .. tostring(yggr.lua.any_cast_string))\r\n"
						"print('foo any_set_string: ' .. tostring(yggr.lua.any_set_string))\r\n"
						"print('param_inner_type: ' .. tostring(param:inner_type()))\r\n"
						"print('param_data: ' .. yggr.lua.any_cast_string(param))\r\n"
						"print('inner_data_inner_type: ' .. tostring(inner_any_data:inner_type()))\r\n"
						"print('inner_data: ' .. yggr.lua.any_cast_string(inner_any_data))\r\n"
						"yggr.lua.any_set_string(param, 'lua_change_data_to_cpp')\r\n"
						"print('------lua_test_string call return------')\r\n"
						"return inner_any_data\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0, 
							boost::bind(&export_all, _1, yggr::string("")));
	yggr_test_assert(bright);

	any_type na = yggr::string("cpp_any_data"); 
	any_type ra ;
	
	bright = mgr.execute_rule<any_type>(ra, yggr::const_args_anchor("test"), boost::ref(na));
	yggr_test_assert(bright);

	yggr_test_assert(yggr::any_cast<const yggr::string&>(ra) == "lua_any_data");
	yggr_test_assert(yggr::any_cast<const yggr::string&>(na) == "lua_change_data_to_cpp");

	std::cout << yggr::any_cast<const yggr::string&>(ra) << std::endl;
	std::cout << yggr::any_cast<const yggr::string&>(na) << std::endl;

	std::cout << "------exec_test_string end------\n" << std::endl;
}

void exec_test_mix()
{
	typedef yggr::any any_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code(
					"inner_any_data = yggr.lua.any_type()\r\n"
					"yggr.lua.any_set_u64(inner_any_data, 100)\r\n"
					"function calculate(param)\r\n"
						"print('------lua_test_u64 call start------')\r\n"
						"print('param_inner_type: ' .. tostring(param:inner_type()))\r\n"
						"print('param_data: ' .. yggr.lua.any_cast_u64(param))\r\n"
						"print('inner_data_inner_type: ' .. tostring(inner_any_data:inner_type()))\r\n"
						"print('inner_data: ' .. yggr.lua.any_cast_u64(inner_any_data))\r\n"
						"yggr.lua.any_set_string(param, 'lua_change_data_and_type_to_cpp')\r\n"
						"print('------lua_test_u64 call return------')\r\n"
						"return inner_any_data\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0, 
							boost::bind(&export_all, _1, yggr::string("")));
	yggr_test_assert(bright);

	any_type na = ANY_SET(yggr::u64, 999); 
	any_type ra ;
	
	bright = mgr.execute_rule<any_type>(ra, yggr::const_args_anchor("test"), boost::ref(na));
	yggr_test_assert(bright);

	yggr_test_assert(yggr::any_cast<yggr::u64>(ra) == 100);
	yggr_test_assert(yggr::any_cast<const yggr::string&>(na) == "lua_change_data_and_type_to_cpp");

	std::cout << yggr::any_cast<yggr::u64>(ra) << std::endl;
	std::cout << yggr::any_cast<const yggr::string&>(na) << std::endl;

	std::cout << "------exec_test_u64 end------\n" << std::endl;
}

int main(int argc, char *argv[])
{
	exec_test_s32();
	exec_test_u32();

	exec_test_s64();
	exec_test_u64();

	exec_test_string();

	exec_test_mix();
	
	wait_any_key(argc, argv);
	return 0;
}