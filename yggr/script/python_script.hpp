//python_script.hpp

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

#ifndef __YGGR_SCRIPT_PYTHON_SCRIPT_HPP__
#define __YGGR_SCRIPT_PYTHON_SCRIPT_HPP__

#include <yggr/base/yggrdef.h>

#if (defined(YGGR_MSVC_USING_MTD_FLAG) && YGGR_MSVC_USING_MTD_FLAG) \
	|| (defined(YGGR_MSVC_USING_MT_FLAG) && YGGR_MSVC_USING_MT_FLAG)

#	error "!!! script python not support /MTd or /MT !!!"

#else

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/base/error_make.hpp>

#include <yggr/move/move.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/exception/exception_sort.hpp>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/script/script_code.hpp>

#include <boost/python.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace script
{
namespace python
{

class python_script
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
private:
	typedef boost::python::dict py_dict_type;
	typedef boost::python::object py_object_type;

private:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("python_script")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_18)
			ERROR_CODE_DEF(E_python_code_compile_error)
			ERROR_CODE_DEF(E_python_code_runtime_error)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_18)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_python_code_compile_error, "python code compile error")
			ERROR_CODE_MSG(E_python_code_runtime_error, "python code runtime error")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

public:
	typedef ::yggr::string enter_foo_name_type;
	typedef charset::utf8_string string_type;

	typedef string_code string_code_type;
	typedef file_code file_code_type;

private:
	typedef python_script this_type;

public:
	python_script(const enter_foo_name_type& enter_foo,
					const string_code_type& code,
					py_dict_type& global);

	python_script(const enter_foo_name_type& enter_foo,
					const file_code_type& code,
					py_dict_type& global);

	~python_script(void);

	inline bool succeed(void) const
	{
		return _bsuccess;
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename R, typename ...Args> inline
	typename boost::enable_if< boost::is_same<R, void>, void >::type
		execute_rule(BOOST_FWD_REF(Args)... args) const
	{
		return this_type::prv_execute_rule_of_non_ret(boost::forward<Args>(args)...);
	}

	template< typename R, typename ...Args > inline
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
		if(!_bsuccess)
		{
			throw error_maker_type::make_error(error_maker_type::E_python_code_compile_error);
		}

		try
		{
			boost::python::call<void>(boost::python::object(_local[_enter_foo.data()]).ptr(),
										boost::forward<Args>(args)...);
		}
		catch(const boost::python::error_already_set&)
		{
			boost::python::handle_exception();
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error,
															std::string("boost_python_already_set\r\n") + get_exception_msg()));
			throw err;
		}
		catch(const compatibility::stl_exception& e)
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error,
															e.what() + std::string("\r\n") + get_exception_msg()));
			throw err;
		}
	}

	template<typename R, typename ...Args> inline
	R prv_execute_rule_of_ret(BOOST_FWD_REF(Args)... args) const
	{
		if(!_bsuccess)
		{
			throw error_maker_type::make_error(error_maker_type::E_python_code_compile_error);
		}

		try
		{
			//return boost::python::call<R>(boost::python::object(_local[_enter_foo.data()]).ptr(),
			//								boost::forward<Args>(args)...);

			boost::python::object res = boost::python::object(_local[_enter_foo.data()])(boost::forward<Args>(args)...);
			return boost::python::extract<R>(res);
		}
		catch(const boost::python::error_already_set&)
		{
			boost::python::handle_exception();
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error,
															std::string("boost_python_already_set\r\n") + get_exception_msg()));
			throw err;
		}
		catch(const compatibility::stl_exception& e)
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error,
															e.what() + std::string("\r\n") + get_exception_msg()));
			throw err;
		}
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, > ) inline \
	void prv_execute_rule_of_non_ret( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		if(!_bsuccess) { \
			throw error_maker_type::make_error(error_maker_type::E_python_code_compile_error); } \
		\
		try { boost::python::call<void>(boost::python::object(_local[_enter_foo.data()]).ptr() \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_OP_BOOST_ANYREF( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
		catch(const boost::python::error_already_set&) { \
			boost::python::handle_exception(); \
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error, \
															std::string("boost_python_already_set\r\n") + get_exception_msg())); \
			throw err; } \
		catch(const compatibility::stl_exception& e) { \
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error, \
															e.what() + std::string("\r\n") + get_exception_msg())); \
			throw err; } } \
	\
	template< typename R \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	R prv_execute_rule_of_ret( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		if(!_bsuccess) { \
			throw error_maker_type::make_error(error_maker_type::E_python_code_compile_error); } \
		\
		try { return boost::python::call<R>(boost::python::object(_local[_enter_foo.data()]).ptr() \
												YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_OP_BOOST_ANYREF( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
		catch(const boost::python::error_already_set&) { \
			boost::python::handle_exception(); \
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error, \
															std::string("boost_python_already_set\r\n") + get_exception_msg())); \
			throw err; } \
		catch(const compatibility::stl_exception& e) { \
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error, \
															e.what() + std::string("\r\n") + get_exception_msg())); \
			throw err; } }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	void prv_call_function_of_non_ret(const enter_foo_name_type& foo_name, BOOST_FWD_REF(Args)... args) const
	{
		if(!_bsuccess)
		{
			throw error_maker_type::make_error(error_maker_type::E_python_code_compile_error);
		}

		try
		{
			boost::python::call<void>(boost::python::object(_local[foo_name.data()]).ptr(),
										boost::forward<Args>(args)...);
		}
		catch(const boost::python::error_already_set&)
		{
			boost::python::handle_exception();
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error,
															get_exception_msg()));
			throw err;
		}
		catch(const compatibility::stl_exception& e)
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error,
															e.what() + std::string("rn") + get_exception_msg()));
			throw err;
		}
	}

	template<typename R, typename ...Args> inline
	R prv_call_function_of_ret(const enter_foo_name_type& foo_name, BOOST_FWD_REF(Args)... args) const
	{
		if(!_bsuccess)
		{
			throw error_maker_type::make_error(error_maker_type::E_python_code_compile_error);
		}

		try
		{ 
			return boost::python::call<R>(boost::python::object(_local[foo_name.data()]).ptr(),
											boost::forward<Args>(args)...);
		}
		catch(const boost::python::error_already_set&)
		{
			boost::python::handle_exception();
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error,
															get_exception_msg()));
			throw err;
		}
		catch(const compatibility::stl_exception& e)
		{
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error,
															e.what() + std::string("rn") + get_exception_msg()));
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
		if(!_bsuccess) { \
			throw error_maker_type::make_error(error_maker_type::E_python_code_compile_error); } \
		\
		try { boost::python::call<void>(boost::python::object(_local[foo_name.data()]).ptr() \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_OP_BOOST_ANYREF( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
		catch(const boost::python::error_already_set&) { \
			boost::python::handle_exception(); \
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error, \
															get_exception_msg())); \
			throw err; } \
		catch(const compatibility::stl_exception& e) { \
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error, \
															e.what() + std::string("\r\n") + get_exception_msg())); \
			throw err; } } \
	\
	template< typename R \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	R prv_call_function_of_ret( const enter_foo_name_type& foo_name \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		if(!_bsuccess) { \
			throw error_maker_type::make_error(error_maker_type::E_python_code_compile_error); } \
		\
		try { return boost::python::call<R>(boost::python::object(_local[foo_name.data()]).ptr() \
												YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_OP_BOOST_ANYREF( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
		catch(const boost::python::error_already_set&) { \
			boost::python::handle_exception(); \
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error, \
															get_exception_msg())); \
			throw err; } \
		catch(const compatibility::stl_exception& e) { \
			error_type err(error_maker_type::make_error(error_maker_type::E_python_code_runtime_error, \
															e.what() + std::string("\r\n") + get_exception_msg())); \
			throw err; } }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


private:
	std::string get_exception_msg(void) const;
	void compile_from_string(const string_type& code);
	void compile_from_file(const string_type& code);

private:
	enter_foo_name_type _enter_foo;
	py_dict_type _local;
	py_object_type _object;
	bool _bsuccess;
};

} // namespace python
} // namespace script
} // namespace yggr

#endif // YGGR_MSVC_USING_MTD_FLAG

#endif // __YGGR_SCRIPT_PYTHON_SCRIPT_HPP__
