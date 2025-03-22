// lua_export_pair_test.cpp

#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/begin_end.hpp>

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/script/lua_script.hpp>
//#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <yggr/lua_wrap/lua_export_typeinfo.hpp>
#include <yggr/lua_wrap/lua_export_typeid.hpp>
#include <yggr/lua_wrap/lua_export_wrap_pair.hpp>

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

typedef
	yggr::script::script_mgr
	<
		yggr::string, yggr::script::lua::lua_script,
		yggr::script::lua::lua_environment
	> script_mgr_type;

template<typename IBeg, typename IEnd >
struct export_pair_other
{
public:
	typedef IBeg i_type;
	typedef IEnd i_end_type;
	typedef typename boost::mpl::deref<i_type>::type now_pair_type;
	typedef typename now_pair_type::second now_wrap_pair_type;

public:
	template<typename ClassOjb> inline
	ClassOjb& operator()(ClassOjb& cobj) const
	{
		typedef typename boost::mpl::next<i_type>::type next_i_type;
		typedef export_pair_other<next_i_type, i_end_type> next_export_type;

		yggr::lua_wrap::export_wrap_pair_r_other<now_wrap_pair_type>(cobj);
		//yggr::lua_wrap::export_wrap_pair_r_other_fs<yggr::f64, yggr::f32>(cobj); // test compile only

		next_export_type next_export;
		return next_export(cobj);
	}
};

template<typename IEnd>
struct export_pair_other<IEnd, IEnd>
{
	template<typename ClassOjb> inline
	ClassOjb& operator()(ClassOjb& cobj) const
	{
		return cobj;
	}
};

template<typename WrapPairExportCfg,
			typename IBeg = typename boost::mpl::begin<WrapPairExportCfg>::type,
			typename IEnd = typename boost::mpl::end<WrapPairExportCfg>::type >
struct export_pair_self
{
public:
	typedef WrapPairExportCfg cfg_type;
	typedef IBeg i_type;
	typedef IEnd i_end_type;
	typedef typename boost::mpl::deref<i_type>::type now_pair_type;
	typedef typename now_pair_type::first now_name_type;
	typedef typename now_pair_type::second now_wrap_pair_type;

	typedef typename boost::mpl::erase<cfg_type, i_type>::type other_cfg_type;

public:
	inline luabind::namespace_& operator()(luabind::namespace_& nm) const
	{
		typedef luabind::class_<now_wrap_pair_type> now_class_type;
		typedef typename boost::mpl::next<i_type>::type next_i_type;
		typedef export_pair_self<cfg_type, next_i_type, i_end_type> next_export_type;
		typedef
			export_pair_other
			<
				typename boost::mpl::begin<other_cfg_type>::type,
				typename boost::mpl::end<other_cfg_type>::type
			> now_export_pair_other_type;

		now_class_type now_cobj(boost::mpl::c_str<now_name_type>::value);

		yggr::lua_wrap::export_wrap_pair(now_cobj);

		now_export_pair_other_type export_r_other;
		export_r_other(now_cobj);

		nm[now_cobj, yggr::lua_wrap::export_typeid<now_wrap_pair_type>()];
		next_export_type next_export;
		return next_export(nm);
	}
};

template<typename WrapPairExportCfg, typename IEnd>
struct export_pair_self<WrapPairExportCfg, IEnd, IEnd>
{
public:
	inline luabind::namespace_& operator()(luabind::namespace_& nm) const
	{
		return nm;
	}
};

template<typename WrapPairExportCfg>
struct export_pair_t
	: public export_pair_self<WrapPairExportCfg>
{
};

template<typename WrapPairExportCfg>
void export_pair(lua_State* l)
{
	typedef WrapPairExportCfg cfg_type;
	typedef export_pair_t<cfg_type> export_pair_type;

	yggr_test_assert(l);
	if(!l)
	{
		return;
	}

	luabind::module_ mod = luabind::module(l);
	luabind::namespace_ namespace_yggr(YGGR_PP_CAST_TO_STRING(yggr));
	luabind::namespace_ namespace_lua(YGGR_PP_CAST_TO_STRING(lua));

	export_pair_type export_pair_obj;
	export_pair_obj(namespace_lua);

	luabind::class_<std::type_info> type_info_cobj("type_info");
	mod
	[
		namespace_yggr
		[
			namespace_lua
			[
				yggr::lua_wrap::export_type_info(type_info_cobj),
				yggr::lua_wrap::export_typeid<bool>(),
				yggr::lua_wrap::export_typeid<double>(),
				yggr::lua_wrap::export_typeid<std::string>()
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
	typedef
		boost::mpl::map
		<
			boost::mpl::pair
			<
				boost::mpl::string<'s16_', 'u16_', 'pair', '_', 'type'>,
				yggr::lua_wrap::wrap_pair<yggr::s16, yggr::u16>
			>,

			boost::mpl::pair
			<
				boost::mpl::string<'s32_','u32_','pair', '_', 'type'>,
				yggr::lua_wrap::wrap_pair<yggr::s32, yggr::u32>
			>,

			boost::mpl::pair
			<
				boost::mpl::string<'s64_','u64_','pair', '_', 'type'>,
				yggr::lua_wrap::wrap_pair<yggr::s64, yggr::u64>
			>
		> wrap_pair_exprot_cfg_type;

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

	export_pair<wrap_pair_exprot_cfg_type>(l);
}

void test_wrap_pair(script_mgr_type& mgr)
{
	typedef yggr::lua_wrap::wrap_pair<yggr::s16, yggr::u16> s16_u16_wrap_pair_type;
	typedef yggr::lua_wrap::wrap_pair<yggr::s32, yggr::u32> s32_u32_wrap_pair_type;
	typedef yggr::lua_wrap::wrap_pair<yggr::s64, yggr::u64> s64_u64_wrap_pair_type;

	s16_u16_wrap_pair_type s16u16_pair(16, 1016);
	s32_u32_wrap_pair_type s32u32_pair(32, 1032);
	s64_u64_wrap_pair_type s64u64_pair(64, 1064);

	s64_u64_wrap_pair_type rst;

	bool bright =
		mgr.execute_rule<s64_u64_wrap_pair_type>(
			rst,
			yggr::const_args_anchor("lua_test_s16_u16_s32_u32_s64_u64_pair_val_or_ref"),
			s16u16_pair, s32u32_pair, s64u64_pair);


	yggr_test_assert(bright);
	yggr_test_assert(s64u64_pair == rst);

	std::cout << "C++: " << rst << std::endl;
}

void test_wrap_pair_ref(script_mgr_type& mgr)
{
	typedef yggr::lua_wrap::wrap_pair<yggr::s16, yggr::u16> s16_u16_wrap_pair_type;
	typedef yggr::lua_wrap::wrap_pair<yggr::s32, yggr::u32> s32_u32_wrap_pair_type;
	typedef yggr::lua_wrap::wrap_pair<yggr::s64, yggr::u64> s64_u64_wrap_pair_type;

	s16_u16_wrap_pair_type s16u16_pair(16, 1016);
	s32_u32_wrap_pair_type s32u32_pair(32, 1032);
	s64_u64_wrap_pair_type s64u64_pair(64, 1064);

	s64_u64_wrap_pair_type rst;

	bool bright =
		mgr.execute_rule<s64_u64_wrap_pair_type>(
			rst,
			yggr::const_args_anchor("lua_test_s16_u16_s32_u32_s64_u64_pair_val_or_ref"),
			boost::ref(s16u16_pair), boost::ref(s32u32_pair), boost::ref(s64u64_pair));


	yggr_test_assert(bright);
	yggr_test_assert(s64u64_pair == rst);

	std::cout << "C++: " << rst << std::endl;
}

void test_wrap_pair_cref(script_mgr_type& mgr)
{
	typedef yggr::lua_wrap::wrap_pair<yggr::s16, yggr::u16> s16_u16_wrap_pair_type;
	typedef yggr::lua_wrap::wrap_pair<yggr::s32, yggr::u32> s32_u32_wrap_pair_type;
	typedef yggr::lua_wrap::wrap_pair<yggr::s64, yggr::u64> s64_u64_wrap_pair_type;

	s16_u16_wrap_pair_type s16u16_pair(16, 1016);
	s32_u32_wrap_pair_type s32u32_pair(32, 1032);
	s64_u64_wrap_pair_type s64u64_pair(64, 1064);

	s64_u64_wrap_pair_type rst;

	bool bright =
		mgr.execute_rule<s64_u64_wrap_pair_type>(
			rst,
			yggr::const_args_anchor("lua_test_s16_u16_s32_u32_s64_u64_pair_cref"),
			boost::cref(s16u16_pair), boost::cref(s32u32_pair), boost::cref(s64u64_pair));


	yggr_test_assert(bright);
	yggr_test_assert(s64u64_pair == rst);

	std::cout << "C++: " << rst << std::endl;
}

void test_exec(int argc, char *argv[])
{
	script_mgr_type mgr;
	mgr.append_module(script_mgr_type::E_lib_all);

	yggr::string code_test_pair("./lua/lua_test_s16_u16_s32_u32_s64_u64_pair_val_or_ref.lua");
	yggr::string code_test_pair_cref("./lua/lua_test_s16_u16_s32_u32_s64_u64_pair_cref.lua");

	bool bright = mgr.insert("lua_test_s16_u16_s32_u32_s64_u64_pair_val_or_ref", "calculate",
								script_mgr_type::file_code_type(code_test_pair), 0,
								boost::bind(&export_all, _1, yggr::string("")));

	bright = bright &&
				mgr.insert("lua_test_s16_u16_s32_u32_s64_u64_pair_cref", "calculate",
								script_mgr_type::file_code_type(code_test_pair_cref), 0,
								boost::bind(&export_all, _1, yggr::string("")));

	yggr_test_assert(bright);

	test_wrap_pair(mgr);
	test_wrap_pair_ref(mgr);
	test_wrap_pair_cref(mgr);
}

int main(int argc, char *argv[])
{
	test_exec(argc, argv);

	wait_any_key(argc, argv);
	return 0;
}
