// lua_export_params_test.cpp

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
#include <yggr/any_val/params.hpp>

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

template<typename T>
const T& params_data_get(const yggr::any_val::params<yggr::string>& params, const yggr::string& key)
{
	return params.get_value<T>(key);
}

template<typename T>
yggr::any_val::params<yggr::string>& 
	params_data_set(yggr::any_val::params<yggr::string>& params, const yggr::string& key, const T& val)
{
	if(key != yggr::string())
	{
		params[key] = val;
	}

	return params;
}

void export_params(lua_State* l)
{
	typedef yggr::any_val::params<yggr::string> params_type;

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

				luabind::class_< params_type >( YGGR_PP_CAST_TO_STRING(params_type) )
					.def( luabind::constructor< >() )
					.def( luabind::constructor<const params_type& >() )

					.def( "copy_from", 
							static_cast<params_type&(params_type::*)(const params_type&)>(
								&params_type::copy_from))

					.def( "swap", static_cast< void(params_type::*)(params_type&) >(&params_type::swap) )

					.def( "clear", static_cast<void(params_type::*)(void)>(&params_type::clear) )

					.def( "erase", &params_type::erase)

					.def( "empty", static_cast<bool(params_type::*)(void) const>(&params_type::empty) )
					.def( "empty_data", static_cast<bool(params_type::*)(const params_type::key_type&) const>(&params_type::empty) )
					
					.def( "copy_datas_from", static_cast<params_type&(params_type::*)(const params_type&)>(&params_type::copy_datas_from))
					.def( "merge_datas_from", static_cast<params_type&(params_type::*)(const params_type&)>(&params_type::merge_datas_from))

					.def( "swap_datas", &params_type::swap_datas)
					.def( "swap_data", &params_type::swap_data )

					.def( "inner_data_type", &params_type::type_name )
					
					.def( "get_data_s32", &params_data_get<yggr::s32>)
					.def( "set_data_s32", &params_data_set<yggr::s32>)

					.def( "get_data_u32", &params_data_get<yggr::u32>)
					.def( "set_data_u32", &params_data_set<yggr::u32>)

					.def( "get_data_s64", &params_data_get<yggr::s64>)
					.def( "set_data_s64", &params_data_set<yggr::s64>)

					.def( "get_data_u64", &params_data_get<yggr::u64>)
					.def( "set_data_u64", &params_data_set<yggr::u64>)

					.def( "get_data_string", &params_data_get<yggr::string>)
					.def( "set_data_string", &params_data_set<yggr::string>)
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
	export_params(l);
}

void exec_test()
{
	typedef yggr::any_val::params<yggr::string> params_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code(
					"inner_param_data = yggr.lua.params_type()\r\n"
					"inner_param_data:set_data_s32('s32', 10)\r\n"
					"inner_param_data:set_data_u32('u32', 11)\r\n"
					"inner_param_data:set_data_s64('s64', 20)\r\n"
					"inner_param_data:set_data_u64('u64', 21)\r\n"
					"inner_param_data:set_data_string('string', 'lua_string')\r\n"


					"function calculate(param)\r\n"
						"print('------lua_test_params call start------')\r\n"
						"print('foo params_type.copy_from: ' .. tostring(inner_param_data.copy_from))\r\n"

						"print('foo params_type.swap: ' .. tostring(inner_param_data.swap))\r\n"
						"print('foo params_type.clear: ' .. tostring(inner_param_data.clear))\r\n"

						"print('foo params_type.erase: ' .. tostring(inner_param_data.erase))\r\n"

						"print('foo params_type.empty: ' .. tostring(inner_param_data.empty))\r\n"
						"print('foo params_type.empty_data: ' .. tostring(inner_param_data.empty_data))\r\n"

						"print('foo params_type.copy_datas_from: ' .. tostring(inner_param_data.copy_datas_from))\r\n"
						"print('foo params_type.merge_datas_from: ' .. tostring(inner_param_data.merge_datas_from))\r\n"

						"print('foo params_type.swap_datas: ' .. tostring(inner_param_data.swap_datas))\r\n"
						"print('foo params_type.swap_data: ' .. tostring(inner_param_data.swap_data))\r\n"

						"print('foo params_type.inner_data_type: ' .. tostring(inner_param_data.inner_data_type))\r\n"

						"print('foo params_type.get_data_s32: ' .. tostring(inner_param_data.get_data_s32))\r\n"
						"print('foo params_type.set_data_s32: ' .. tostring(inner_param_data.set_data_s32))\r\n"

						"print('foo params_type.get_data_u32: ' .. tostring(inner_param_data.get_data_u32))\r\n"
						"print('foo params_type.set_data_u32: ' .. tostring(inner_param_data.set_data_u32))\r\n"

						"print('foo params_type.get_data_s64: ' .. tostring(inner_param_data.get_data_s64))\r\n"
						"print('foo params_type.set_data_s64: ' .. tostring(inner_param_data.set_data_s64))\r\n"

						"print('foo params_type.get_data_u64: ' .. tostring(inner_param_data.get_data_u64))\r\n"
						"print('foo params_type.set_data_u64: ' .. tostring(inner_param_data.set_data_u64))\r\n"

						"print('foo params_type.get_data_string: ' .. tostring(inner_param_data.get_data_string))\r\n"
						"print('foo params_type.set_data_string: ' .. tostring(inner_param_data.set_data_string))\r\n"

						"print('cpp_inner_s32_type: ' .. tostring(param:inner_data_type('s32')))\r\n"
						"print('cpp_inner_u32_type: ' .. tostring(param:inner_data_type('u32')))\r\n"
						"print('cpp_inner_s64_type: ' .. tostring(param:inner_data_type('s64')))\r\n"
						"print('cpp_inner_u64_type: ' .. tostring(param:inner_data_type('u64')))\r\n"
						"print('cpp_inner_string_type: ' .. tostring(param:inner_data_type('string')))\r\n"

						"print('cpp_data_s32: ' .. tostring(param:get_data_s32('s32')))\r\n"
						"print('cpp_data_u32: ' .. tostring(param:get_data_u32('u32')))\r\n"
						"print('cpp_data_s64: ' .. tostring(param:get_data_s64('s64')))\r\n"
						"print('cpp_data_u64: ' .. tostring(param:get_data_u64('u64')))\r\n"
						"print('cpp_data_string: ' .. param:get_data_string('string'))\r\n"

						"print('lua_data_s32_type: ' .. tostring(inner_param_data:inner_data_type('s32')))\r\n"
						"print('lua_data_u32_type: ' .. tostring(inner_param_data:inner_data_type('u32')))\r\n"
						"print('lua_data_s64_type: ' .. tostring(inner_param_data:inner_data_type('s64')))\r\n"
						"print('lua_data_u64_type: ' .. tostring(inner_param_data:inner_data_type('u64')))\r\n"
						"print('lua_data_string_type: ' .. tostring(inner_param_data:inner_data_type('string')))\r\n"

						"print('lua_data_s32: ' .. tostring(inner_param_data:get_data_s32('s32')))\r\n"
						"print('lua_data_u32: ' .. tostring(inner_param_data:get_data_u32('u32')))\r\n"
						"print('lua_data_s64: ' .. tostring(inner_param_data:get_data_s64('s64')))\r\n"
						"print('lua_data_u64: ' .. tostring(inner_param_data:get_data_u64('u64')))\r\n"
						"print('lua_data_string: ' .. inner_param_data:get_data_string('string'))\r\n"

						"param:set_data_s32('s32', 1000)\r\n"
						"param:set_data_u32('u32', 1010)\r\n"
						"param:set_data_s64('s64', 2000)\r\n"
						"param:set_data_u64('u64', 2010)\r\n"
						"param:set_data_string('string', 'lua_changed_cpp_string')\r\n"

						"print('------lua_test_params call return------')\r\n"
						"return inner_param_data\r\n"
					"end\r\n"
				);

		
		

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0, 
							boost::bind(&export_all, _1, yggr::string("")));
	yggr_test_assert(bright);

	params_type na;

	na["s32"] = ANY_SET(yggr::s32, 100);
	na["u32"] = ANY_SET(yggr::u32, 101);

	na["s64"] = ANY_SET(yggr::s64, 200);
	na["u64"] = ANY_SET(yggr::u64, 201);

	na["string"] = yggr::string("cpp_string");

	params_type ra ;
	
	bright = mgr.execute_rule<params_type>(ra, yggr::const_args_anchor("test"), boost::ref(na));
	yggr_test_assert(bright);

	try
	{
		yggr_test_assert(na.get_value<yggr::s32>("s32") == 1000);
		yggr_test_assert(na.get_value<yggr::u32>("u32") == 1010);
		yggr_test_assert(na.get_value<yggr::s64>("s64") == 2000);
		yggr_test_assert(na.get_value<yggr::u64>("u64") == 2010);
		yggr_test_assert(na.get_value<yggr::string>("string") == "lua_changed_cpp_string");

		yggr_test_assert(ra.get_value<yggr::s32>("s32") == 10);
		yggr_test_assert(ra.get_value<yggr::u32>("u32") == 11);
		yggr_test_assert(ra.get_value<yggr::s64>("s64") == 20);
		yggr_test_assert(ra.get_value<yggr::u64>("u64") == 21);
		yggr_test_assert(ra.get_value<yggr::string>("string") == "lua_string");
	}
	catch(...)
	{
		yggr_test_assert(false);
	}

	std::cout << "------exec_test_string end------\n" << std::endl;
}


int main(int argc, char *argv[])
{
	exec_test();

	wait_any_key(argc, argv);
	return 0;
}