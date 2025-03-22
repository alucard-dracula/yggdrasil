// lua_export_uuid_test.cpp

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/mplex/typename_expand.hpp>

#include <yggr/script/lua_script.hpp>
//#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr/ids/uuid.hpp>

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

//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_s)
#include YGGR_PP_LINK_LIB(script_lua_ss)
//#include YGGR_PP_LINK_LIB_TOOL_IGNORE(script_lua_ss_ts)

typedef yggr::script::script_mgr<yggr::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

void export_uuid(lua_State* l)
{
	typedef yggr::ids::uuid uuid_type;

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

				luabind::class_< uuid_type >( YGGR_PP_CAST_TO_STRING(uuid_type) )
					.def( luabind::constructor< >() )
					.def( luabind::constructor< const yggr::string& >() )
					.def( luabind::constructor< const char*, const char*>() )
					.def( luabind::constructor< const uuid_type& >() )
					.def( luabind::tostring( luabind::const_self ) )
					.def( luabind::const_self == luabind::const_self )

					.def( "swap", static_cast< void ( uuid_type::* )( uuid_type& ) >( &uuid_type::swap ) )
					.def( "swap", static_cast< void ( uuid_type::* )( yggr::string& ) >( &uuid_type::base_type::swap<yggr::string> ) )

					.def( "compare", static_cast< yggr::s32 ( uuid_type::* )( const uuid_type& ) const >( &uuid_type::compare ) )
					.def( "compare", static_cast< yggr::s32 ( uuid_type::* )( const yggr::string& ) const >( &uuid_type::base_type::compare<yggr::string> ) )

					.def( "compare_eq", static_cast< bool ( uuid_type::* )( const uuid_type& ) const >( &uuid_type::compare_eq ) )
					.def( "compare_eq", static_cast< bool ( uuid_type::* )( const yggr::string& ) const >( &uuid_type::compare_eq<yggr::string> ) )

					.def( "assign", static_cast< void ( uuid_type::* )( const uuid_type&) >( &uuid_type::assign ) )
					.def( "assign", static_cast< bool ( uuid_type::* )( const yggr::string& ) >( &uuid_type::assign<yggr::string> ) )

					.def( "empty", static_cast< bool ( uuid_type::* )(void) const >( &uuid_type::empty ) ) 
					.def( "is_nil", static_cast< bool ( uuid_type::* )(void) const >( &uuid_type::is_nil ) ) 

					.def( "at", static_cast< char& ( uuid_type::* )( std::size_t ) >( &uuid_type::operator[] ) )
					.def( "at", static_cast< const char& ( uuid_type::* )( std::size_t ) const >( &uuid_type::operator[] ) )

					.def( "hash", static_cast< std::size_t ( uuid_type::* )( void ) const >( &uuid_type::hash ) )
					.def( "variant", static_cast< yggr::u32 ( uuid_type::* )( void ) const >( &uuid_type::variant ) )
					.def( "version", static_cast< yggr::u32 ( uuid_type::* )( void ) const >( &uuid_type::version ) ),

				luabind::def("uuid_from_code", 
								static_cast< bool (*)(uuid_type& id, const yggr::string&) >(
									&uuid_type::from_code<
										yggr::string::traits_type, 
										yggr::mplex::typename_expand_get<yggr::string, 2>::type>)),

				luabind::def("uuid_create_of_code", 
								static_cast< uuid_type (*)(const yggr::string&) >(
									&uuid_type::create_of_code<
										yggr::string::traits_type,
										yggr::mplex::typename_expand_get<yggr::string, 2>::type>))

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

	export_uuid(l);
}




void exec_test()
{
	typedef yggr::ids::uuid uuid_type;
	typedef yggr::ids::id_generator<uuid_type, yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> gen_type;

	script_mgr_type mgr;

	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code(
					"ssid = yggr.lua.uuid_type()\r\n"
					"function calculate(param)\r\n"
						"print(param)\r\n"
						"print(tostring(ssid))\r\n"
						"print(yggr.lua.uuid_create_of_code(\"11223344556677889900112233445566\"))"
						"return param\r\n"
					"end\r\n"
				);

	bool bright = false;
    
	bright = mgr.insert("test", "calculate", script_mgr_type::string_code_type(code), 0, 
							boost::bind(&export_all, _1, yggr::string("")));
	yggr_test_assert(bright);

	gen_type gen;
	uuid_type na = gen(); 
	uuid_type ra;
	
	bright = mgr.execute_rule<uuid_type>(ra, yggr::const_args_anchor("test"), boost::cref(na));
	yggr_test_assert(bright);
	yggr_test_assert(ra == na);
}

int main(int argc, char *argv[])
{
	exec_test();
	
	wait_any_key(argc, argv);
	return 0;
}