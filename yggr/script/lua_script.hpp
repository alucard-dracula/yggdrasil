//lua_script.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_SCRIPT_LUA_SCRIPT_HPP__
#define __YGGR_SCRIPT_LUA_SCRIPT_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/compatibility/stl_exception.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/exception/exception.hpp>

#include <yggr/base/error_make.hpp>

#include <yggr/script/script_code.hpp>
#include <yggr/script/lua_environment.hpp>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <luabind/luabind.hpp>

#include <utility>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{
namespace script
{
namespace lua
{

class lua_script
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
private:
	typedef lua_environment lua_env_info_type;
	typedef	lua_env_info_type::lua_env_type lua_env_type;

private:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("lua_script")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_19)
			ERROR_CODE_DEF(E_lua_code_compile_error)
			ERROR_CODE_DEF(E_lua_code_runtime_error)
			ERROR_CODE_DEF(E_lua_code_runtime_error_detail)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_19)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_lua_code_compile_error, "lua code compile error")
			ERROR_CODE_MSG(E_lua_code_runtime_error, "lua code runtime error")
			ERROR_CODE_MSG(E_lua_code_runtime_error_detail, "lua code runtime error detail")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------
public:
	typedef ::yggr::string enter_foo_name_type;
	typedef charset::utf8_string string_type;

	typedef string_code string_code_type;
	typedef file_code file_code_type;

private:
	typedef lua_script this_type;

public:
	lua_script(u32 global_env, u32 local_env = 0);

	template<typename Handler_Module>
	lua_script(u32 global_env, u32 local_env,
				const Handler_Module& handler_module)
		: _lua_env(luaL_newstate())
	{
		open_libs(global_env, local_env);
		handler_module(_lua_env);
	}

	lua_script(const enter_foo_name_type& enter_foo,
				u32 global_env, u32 local_env = 0);

	template<typename Handler_Module>
	lua_script(const enter_foo_name_type& enter_foo,
				u32 global_env, u32 local_env,
				const Handler_Module& handler_module)
		: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
	{
		open_libs(global_env, local_env);
		handler_module(_lua_env);
	}

	lua_script(const enter_foo_name_type& enter_foo,
				const string_code_type& code,
				u32 global_env, u32 local_env);

	template<typename Handler_Module>
	lua_script(const enter_foo_name_type& enter_foo,
				const string_code_type& code,
				u32 global_env, u32 local_env,
				const Handler_Module& handler_module)
		: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
	{
		open_libs(global_env, local_env);
		handler_module(_lua_env);
		try
		{
			compile_from_string(code);
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			lua_close(_lua_env);
			_lua_env = 0;
		}
	}

	lua_script(const enter_foo_name_type& enter_foo,
				const file_code_type& code,
				u32 global_env, u32 local_env);


	template<typename Handler_Module>
	lua_script(const enter_foo_name_type& enter_foo,
				const file_code_type& code,
				u32 global_env, u32 local_env,
				const Handler_Module& handler_module)
		: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
	{
		open_libs(global_env, local_env);
		handler_module(_lua_env);
		try
		{
			compile_from_file(code);
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			lua_close(_lua_env);
			_lua_env = 0;
		}
	}

	~lua_script(void);

public:
	inline bool succeed(void) const
	{
		return (_lua_env) && (!_enter_foo.empty());
	}

	inline void do_script(const string_code_type& code)
	{
		compile_from_string(code);
	}

	inline void do_script(const file_code_type& code)
	{
		compile_from_file(code);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename R, typename ...Args> inline
	typename boost::enable_if< boost::is_same<R, void>, void >::type
		execute_rule(BOOST_FWD_REF(Args)... args) const
	{
		return this_type::prv_execute_rule_of_non_ret(boost::forward<Args>(args)...);
	}

	template< typename R, typename ...Args >
	typename boost::disable_if< boost::is_same<R, void>, R >::type
		execute_rule(BOOST_FWD_REF(Args)... args) const
	{
		return this_type::prv_execute_rule_of_ret<R>(boost::forward<Args>(args)...);
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename R \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if< boost::is_same<R, void>, void >::type \
		execute_rule( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return this_type::prv_execute_rule_of_non_ret(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
	\
	template< typename R \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::disable_if< boost::is_same<R, void>, R >::type \
		execute_rule( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return this_type::prv_execute_rule_of_ret<R>(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// must be divided into two version functions with return value and no return value,
// otherwise C2780 error will occur when using function pointer and boost::ref() boost::cref()

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename R, typename ...Args> inline
	typename boost::enable_if< boost::is_same<R, void>, void >::type
		call_function(const enter_foo_name_type& foo_name, BOOST_FWD_REF(Args)... args) const
	{
		return this_type::prv_call_function_of_non_ret(foo_name, boost::forward<Args>(args)...);
	}

	template< typename R, typename ...Args > inline
	typename boost::disable_if< boost::is_same<R, void>, R >::type
		call_function(const enter_foo_name_type& foo_name, BOOST_FWD_REF(Args)... args) const
	{
		return this_type::prv_call_function_of_ret<R>(foo_name, boost::forward<Args>(args)...);
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename R \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if< boost::is_same<R, void>, void >::type \
		call_function(const enter_foo_name_type& foo_name \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return \
			this_type::prv_call_function_of_non_ret( \
				foo_name \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template< typename R \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::disable_if< boost::is_same<R, void>, R >::type \
		call_function(const enter_foo_name_type& foo_name \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return \
			this_type::prv_call_function_of_ret<R>( \
				foo_name \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	void prv_execute_rule_of_non_ret(BOOST_FWD_REF(Args)... args) const
	{
		if(!_lua_env)
		{
			throw error_maker_type::make_error(error_maker_type::E_lua_code_compile_error);
		}

		try
		{
			luabind::object foo_obj = this_type::prv_get_function(_enter_foo);
			luabind::call_function<void>(foo_obj, boost::forward<Args>(args)...);
		}
		catch(const luabind::error& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(_enter_foo, e)));
			throw err;
		}
		catch(const luabind::cast_failed& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(_enter_foo, e)));
			throw err;
		}
		catch(const compatibility::stl_exception& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(_enter_foo, e)));
			throw err;
		}
	}

	template< typename R, typename ...Args > inline
	R prv_execute_rule_of_ret(BOOST_FWD_REF(Args)... args) const
	{
		if(!_lua_env)
		{
			throw error_maker_type::make_error(error_maker_type::E_lua_code_compile_error);
		}

		try
		{
			luabind::object foo_obj = this_type::prv_get_function(_enter_foo);
			return luabind::call_function<R>(foo_obj, boost::forward<Args>(args)... ).operator R();
		}
		catch(const luabind::error& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(_enter_foo, e)));
			throw err;
		}
		catch(const luabind::cast_failed& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(_enter_foo, e)));
			throw err;
		}
		catch(const compatibility::stl_exception& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(_enter_foo, e)));
			throw err;
		}
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, > ) inline \
	void prv_execute_rule_of_non_ret( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		if(!_lua_env) { \
			throw error_maker_type::make_error(error_maker_type::E_lua_code_compile_error); } \
		\
		try {	luabind::object foo_obj = this_type::prv_get_function(_enter_foo); \
				luabind::call_function<void>(foo_obj \
												YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
		catch(const luabind::error& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(_enter_foo, e))); \
			throw err; } \
		catch(const luabind::cast_failed& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(_enter_foo, e))); \
			throw err; } \
		catch(const compatibility::stl_exception& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(_enter_foo, e))); \
			throw err; } } \
	\
	template< typename R \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	R prv_execute_rule_of_ret( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		if(!_lua_env) { \
			throw error_maker_type::make_error(error_maker_type::E_lua_code_compile_error); } \
		\
		try {	luabind::object foo_obj = this_type::prv_get_function(_enter_foo); \
				return luabind::call_function<R>( \
						foo_obj \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ).operator R(); } \
		catch(const luabind::error& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(_enter_foo, e))); \
			throw err; } \
		catch(const luabind::cast_failed& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(_enter_foo, e))); \
			throw err; } \
		catch(const compatibility::stl_exception& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(_enter_foo, e))); \
			throw err; } }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// must be divided into two version functions with return value and no return value,
// otherwise C2780 error will occur when using function pointer and boost::ref() boost::cref()

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	void prv_call_function_of_non_ret(const enter_foo_name_type& foo_name, BOOST_FWD_REF(Args)... args) const
	{
		if(!_lua_env)
		{
			throw error_maker_type::make_error(error_maker_type::E_lua_code_compile_error);
		}

		try
		{
			luabind::object foo_obj = this_type::prv_get_function(foo_name);
			luabind::call_function<void>(foo_obj, boost::forward<Args>(args)...);
		}
		catch(const luabind::error& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(foo_name, e)));
			throw err;
		}
		catch(const luabind::cast_failed& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(foo_name, e)));
			throw err;
		}
		catch(const compatibility::stl_exception& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(foo_name, e)));
			throw err;
		}
	}

	template< typename R, typename ...Args > inline
	R prv_call_function_of_ret(const enter_foo_name_type& foo_name, BOOST_FWD_REF(Args)... args) const
	{
		if(!_lua_env)
		{
			throw error_maker_type::make_error(error_maker_type::E_lua_code_compile_error);
		}

		try
		{
			luabind::object foo_obj = this_type::prv_get_function(foo_name);
			return luabind::call_function<R>(foo_obj, boost::forward<Args>(args)... ).operator R();
		}
		catch(const luabind::error& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(foo_name, e)));
			throw err;
		}
		catch(const luabind::cast_failed& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(foo_name, e)));
			throw err;
		}
		catch(const compatibility::stl_exception& e)
		{
			error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_runtime_error,
							this_type::format_error_msg(foo_name, e)));
			throw err;
		}
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, > ) inline \
	void prv_call_function_of_non_ret(const enter_foo_name_type& foo_name \
										YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		if(!_lua_env) { \
			throw error_maker_type::make_error(error_maker_type::E_lua_code_compile_error); } \
		\
		try {	luabind::object foo_obj = this_type::prv_get_function(foo_name); \
				luabind::call_function<void>(foo_obj \
												YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
		catch(const luabind::error& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(foo_name, e))); \
			throw err; } \
		catch(const luabind::cast_failed& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(foo_name, e))); \
			throw err; } \
		catch(const compatibility::stl_exception& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(foo_name, e))); \
			throw err; } } \
	\
	template< typename R \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	R prv_call_function_of_ret(const enter_foo_name_type& foo_name \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		if(!_lua_env) { \
			throw error_maker_type::make_error(error_maker_type::E_lua_code_compile_error); } \
		\
		try {	luabind::object foo_obj = this_type::prv_get_function(foo_name); \
				return luabind::call_function<R>( \
						foo_obj \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ).operator R(); } \
		catch(const luabind::error& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(foo_name, e))); \
			throw err; } \
		catch(const luabind::cast_failed& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(foo_name, e))); \
			throw err; } \
		catch(const compatibility::stl_exception& e) { \
			error_type err(error_maker_type::make_error( \
							error_maker_type::E_lua_code_runtime_error, \
							this_type::format_error_msg(foo_name, e))); \
			throw err; } }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	::std::string format_error_msg(const enter_foo_name_type& foo_name, const luabind::error& e) const;
	::std::string format_error_msg(const enter_foo_name_type& foo_name, const luabind::cast_failed& e) const;
	::std::string format_error_msg(const enter_foo_name_type& foo_name, const compatibility::stl_exception& e) const;
	::std::string get_exception_msg(void) const;

	void open_libs(u32 env);

	inline void open_libs(u32 g_env_code, u32 l_env_code)
	{
		this_type::open_libs((g_env_code | l_env_code));
	}

	luabind::object prv_get_function(const enter_foo_name_type& foo_name) const;

	void compile_from_string(const string_type& code);
	void compile_from_file(const string_type& fname);

private:
    mutable lua_env_type _lua_env;
	enter_foo_name_type _enter_foo;
};

} // namespace lua
} // namespace script
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif // __YGGR_SCRIPT_LUA_SCRIPT_HPP__
