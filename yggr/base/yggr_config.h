//yggr_config.h

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

#ifndef __YGGR_YGGR_CONFIG_H__
#define __YGGR_YGGR_CONFIG_H__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#if defined(__APPLE_CC__) || defined(__APPLE__)
# include <TargetConditionals.h>
#endif // #if defined(__APPLE_CC__) || defined(__APPLE__)

#ifdef _MSC_VER

//C4002: too many actual parameters for macro 'identifier'
//C4003: not enough actual parameters for macro
//C4005: 'macro' : macro redefinition
//C4006: #undef expected an identifier
//C4013: warning C4013: 'xxx function' undefined
//C4067: unexpected tokens following preprocessor directive - expected a newline
//C4068: unknown pragma
//C4090: '=' : different 'const' qualifiers
//C4099: 'identifier' : type name first seen using 'objecttype1' now seen using 'objecttype2'
//C4101: 'identifier' : unreferenced local variable
//C4114: same type qualifier used more than once
//C4129: 'C' : unrecognized character escape sequence
//C4141: 'inline' : used more than once
//C4172: returning address of local variable or temporary
//C4293: '<< or >>' : shift count negative or too big, undefined behavior
//C4390: ';' : empty controlled statement found; is this the intent?
//C4552: 'op_symbol' operator has no effect; expected operator with side-effect
//C4700: uninitialized local variable 'name' used
//C4715: 'function' : not all control paths return a value
//C4717: 'function' : recursive on all control paths, function will cause runtime stack overflow

#if !defined(YGGR_WARNING_NOT_TO_ERROR)

#pragma warning (error : 4002)
#pragma warning (error : 4003)
#pragma warning (error : 4005)
#pragma warning (error : 4006)
#pragma warning (error : 4013)
#pragma warning (error : 4067)
#pragma warning (error : 4068)
#pragma warning (error : 4090)
#pragma warning (error : 4099)
#pragma warning (error : 4101)
#pragma warning (error : 4114)
#pragma warning (error : 4129)
#pragma warning (error : 4141)
#pragma warning (error : 4172)
#pragma warning (error : 4293)
#pragma warning (error : 4390)
#pragma warning (error : 4552)
#pragma warning (error : 4700)
#pragma warning (error : 4715)
#pragma warning (error : 4717)

#endif // YGGR_WARNING_NOT_TO_ERROR

#endif // _MSC_VER

/*
Compile Macro info

*		Macro									*Info

BOOST_HAS_ICU								regex use ICU fix utf8 string if undef it, regular_parse can not fix utf8 string

YGGR_SYSTEM_64								compile to 64bit system
YGGR_SYSTEM_128								compile to 128bit system
YGGR_SYSTEM_256								compile to 256bit system

YGGR_AT_WINDOWS								windows platform
YGGR_AT_LINUX								linux platform
YGGR_AT_ANDROID								android platform
YGGR_AT_DARWIN								macos platform
YGGR_AT_IOS									ios platform

YGGR_INCLUDE_SEH_DEBUG						include seh model debuging in windows
YGGR_USE_SEH								use seh model
YGGR_USE_UTF8_EX_MODE						use utf8 ex_mode compatible 6byte utf8 char, 
												default non-defined (if you want to using it add to project or user_config.hpp)

YGGR_TUPLE_LIMIT_LENGTH_CFG					yggr::tuple::tuple's template params limit length, default is 10 same to boost::tuple::tuple
YGGR_PP_TEMPLATE_PARAMS_LEN_CFG				yggr variable length function templates and class template limit length, default is 10
YGGR_PP_PACKET_INFO_PARAMS_LEN				if comparer not support cxx11 variadic_templates,
												using YGGR_PP_PACKET_INFO_PARAMS_LEN config packet_info's "typename T" count,
												default same YGGR_PP_TEMPLATE_PARAMS_LEN_CFG

YGGR_NO_CWCHAR								yggr not has wchar_t
YGGR_NO_CHAR8_T								yggr not has char8_t
YGGR_NO_CHAR16_T							yggr not has char16_t
YGGR_NO_CHAR32_T							yggr not has char32_t

YGGR_NO_WSTRING								yggr not has wstring
YGGR_NO_U8STRING_T							yggr not has u8string
YGGR_NO_U16STRING_T							yggr not has u16string
YGGR_NO_U32STRING_T							yggr not has u32string

YGGR_SYS_DEFAULT_STRING_CHARSET_NAME_CFG	now system string default charset, 
												if undef it windows default using "gbk" other os default is "utf-8"
YGGR_SYS_DEFAULT_WSTRING_2_CHARSET_NAME_CFG	now system wstring (sizeof(wchar_t) == 2) default charset.
												if undef it default using see charset/default_charset_config.hpp
YGGR_SYS_DEFAULT_WSTRING_4_CHARSET_NAME_CFG	now system wstring (sizeof(wchar_t) == 4) default charset, 
												if undef it default using see charset/default_charset_config.hpp
YGGR_HEX_CONV_DEFAULT_MODE					charset::hex_converter default mode (mode: "lowcase(0)", "upper case(1)") default mode is "lowcase"

YGGR_USE_OLD_BOOST_FILESYSTEM				filesystem using older boost filesystem
YGGR_FILE_PATH_USE_BACK_SLANT_ONLY			In the windows platform only use '\' that path segmentation
YGGR_FILE_PATH_REGEX_STRING					filesystem file path regex string, default see file_system/name_check.hpp
YGGR_FILE_NAME_REGEX_STRING					filesystem file name regex string, default see file_system/name_check.hpp
YGGR_FSYS_DIR_HARD_LINK_SUPPORT_ENABLE		filesystem hard_link support directory

YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION	yggr::safe_container not use optimization, example safe_unordered put outside hash operation 

YGGR_USE_STL_STRING							yggr::string use std::string, warning: segment(boost::interporcess) must use boost::container::basic_string
YGGR_USE_BOOST_STRING						yggr::string use boost::container::string (default)
YGGR_USE_STL_CONTAINER						yggr::container use std::contaner, example(std::vector), 
												warning: segment(boost::interporcess) must use boost::container
YGGR_USE_BOOST_CONTAINER					yggr::container use boost::container example(boost::container::vector) (default)
YGGR_USE_FAST_MATH							yggr use fast math foo, default is not use
YGGR_FAST_MATH_ITERATE_TWICE				fast math foo using iterate twice

YGGR_NETWORK_FLOAT_POINT_HN_CONV_ENABLE		enable floating point var (float | double) ntoh foo, default is disable

YGGR_SERIALIZE_SIZE32						serialize size_type use 32bit
YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE		serialize string is not primitive, default is not defined it

YGGR_EXCEPTION_DEBUG						if defined exception::throw_error must output exception msg to the console
YGGR_ERROR_CODE_OUTPUT_USING_UTC_TIME		if defined exception::error_code output using utc_time, default using localtime

YGGR_USING_CXX_VERSION						defined yggr use c++ verion (98, 300, 1100, 1400, 1700, 2000, ...)
YGGR_USING_CXX98							yggr use c++98 and newer
YGGR_USING_CXX03							yggr use c++03 and newer
YGGR_USING_CXX11							yggr use c++11 and newer
YGGR_USING_CXX14							yggr use c++14 and newer
YGGR_USING_CXX17							yggr use c++17 and newer
YGGR_USING_CXX20							yggr use c++20 and newer
YGGR_USING_CXX23							yggr use c++23 and newer

YGGR_MATH_F32_DEFAULT_PRECISION				change math::flaw_comparer using default float precision
YGGR_MATH_F64_DEFAULT_PRECISION				change math::flaw_comparer using default double precision

YGGR_SMART_PTR_USING_STL					yggr::make_shared yggr::construct_shared using std::shared_ptr, default is using boost::shared_ptr

YGGR_TIMER_TASK_NOT_USE_SEH					yggr::timer::timer_mgr::run_tasks not using seh, default seh used
YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME	yggr::time constnt value(e.g: utc_local_time_diff) recalculate every time, default not recalculate

YGGR_ANY_OPERATOR_NOT_REG_UNSAFE			any op any not support unsafe op (e.g. bool & int), default defined
YGGR_PARAMS_USE_SAFE_CAST					if defined yggr::any_val using yggr::any_cast it's slow but safed, else using yggr::unsafe_any_cast is fast but not safe 

YGGR_TPL_CONFIG_USING_TPL					using typedef tpl<...> make config type(e.g. typedef network_config_template<...>), default not defined (fix error c1060)

YGGR_NO_CXX11_HDR_UNORDERED_SET				not has cxx11 unordered_set
YGGR_NO_CXX11_HDR_UNORDERED_MAP				not has cxx11 unordered_map

YGGR_DISABLE_PRAGMA_MESSAGE					disable pragma messgame

YGGR_NO_CXX98_FUNCTION_BASE					not has std::binary_function std::unary_function

YGGR_USING_LEGCACY_STDIO_DEFINITIONS		fix Visual Studio 2015 and later link old RTC error LNK2001: unresolved external symbol __iob_func
  
YGGR_OPENSSL_USING_STATIC_LIB				openssl using static lib, default using dynamic lib

YGGR_HAS_ICU								boost::regex using icu check. default auto_check
YGGR_ICU_USING_NATIVE_NAME					link icu using default icu name. default not defined

YGGR_LUA_THREAD_SAFE						lua using thread safe version

YGGR_ANY_VAL_OP_NOT_INIT_BASE_TYPE			yggr::any_val::any_operator_mgr_basic_t_reg not register all base_type
YGGR_ANY_VAL_OP_INIT_NOT_SUPPORT_BASE_TYPE	yggr::any_val::any_operator_mgr_basic_t_reg not call 
YGGR_ANY_VAL_OP_EXCLUDE_UNSAFE_TYPE			yggr::any_val::any_operator_mgr_basic_t_reg not register unsafe type operator

YGGR_MONGODB_NO_DECIMAL128					yggr::nsql_database_system not support decimal128

*/

//compiler
#if defined(BOOST_MSVC) || defined(_MSC_VER)
#	if !defined(YGGR_MSVC)
#		define YGGR_MSVC 1
#	endif // YGGR_MSVC
#endif // BOOST_MSVC

#if defined(BOOST_GCC) || (defined(__GNUC__) && !(defined(__clang__) && __clang__ ))
#	if !defined(YGGR_GCC)
#		define YGGR_GCC 1
#	endif // YGGR_GCC
#endif // BOOST_GCC

#if defined(BOOST_CLANG) || (defined(__clang__) && __clang__)
#	if !defined(YGGR_CLANG)
#		define YGGR_CLANG 1
#	endif // YGGR_CLANG
#endif // BOOST_CLANG

#if defined(BOOST_INTEL)
#	if !defined(YGGR_INTEL)
#		define YGGR_INTEL 1
#	endif // YGGR_INTEL
#endif // BOOST_INTEL

#if defined(BOOST_BORLANDC)
#	if !defined(YGGR_BORLANDC)
#		define YGGR_BORLANDC 1
#	endif // YGGR_BORLANDC
#endif // BOOST_BORLANDC

#if defined(BOOST_CODEGEARC)
#	if !defined(YGGR_CODEGEARC)
#		define YGGR_CODEGEARC 1
#	endif // YGGR_CODEGEARC
#endif // BOOST_CODEGEARC

#if defined(BOOST_EMBTC)
#	if !defined(YGGR_EMBTC)
#		define YGGR_EMBTC 1
#	endif // YGGR_EMBTC
#endif // BOOST_EMBTC

#if defined(BOOST_EMBTC)
#	if !defined(YGGR_EMBTC)
#		define YGGR_EMBTC 1
#	endif // YGGR_EMBTC
#endif // BOOST_EMBTC

// arch
#if (defined(__arm__) && __arm__) || (defined(__arm) && (__arm))
#	define YGGR_ARM 1
#endif // #if (defined(__arm__) && __arm__) || (defined(__arm) && (__arm))

#if (defined(__arm64__) && __arm64__) || (defined(__arm64) && (__arm64))
#	define YGGR_ARM64 1
#endif // #if (defined(__arm__) && __arm__) || (defined(__arm) && (__arm))

#if defined(YGGR_ARM) || defined(YGGR_ARM64)
#	define YGGR_ARM_PLATFORM 1
#endif // #if defined(YGGR_ARM) || defined(YGGR_ARM64)

#if (defined(__riscv) && __riscv)
#	define YGGR_RISCV 1
#endif // #if (defined(__riscv) && __riscv)

#if defined(YGGR_RISCV)
#	define YGGR_RISCV_PLATFORM 1
#endif // #if defined(YGGR_ARM) || defined(YGGR_ARM64)

#if (defined(_M_IX86) && _M_IX86) \
	|| (defined(__i386) && __i386) ||  (defined(__i386__) && __i386__) \
	|| (defined(__i486) && __i486) ||  (defined(__i486__) && __i486__) \
	|| (defined(__i586) && __i586) ||  (defined(__i586__) && __i586__) \
	|| (defined(__i686) && __i686) ||  (defined(__i686__) && __i686__)
#	define YGGR_X86 1
#endif // i386 i686

#if (defined(_M_X64) && _M_X64) \
	|| (defined(__amd64) && __amd64) ||  (defined(__amd64__) && __amd64__) \
	|| (defined(__x86_64) && __x86_64) ||  (defined(__x86_64__) && __x86_64__)
#	define YGGR_X86_64 1
#endif // amd64 x86_64

#if defined(YGGR_X86) || defined(YGGR_X86_64)
#	define YGGR_X86_PLATFORM 1
#endif // #if defined(YGGR_ARM) || defined(YGGR_ARM64)

#if !defined(YGGR_SYSTEM_64)
#	if defined(YGGR_ARM64) || defined(YGGR_X86_64) \
		|| (defined(__UINTPTR_WIDTH__) && (__UINTPTR_WIDTH__ == 64))
#		define YGGR_SYSTEM_64 1
#	endif //  arm64 x86_64 __UINTPTR_WIDTH__
#endif // YGGR_SYSTEM_64

#ifdef _MSC_VER
#	if defined (YGGR_SYSTEM_64) || defined(WIN64)
#		if! defined(_WIN64)
#			define _WIN64
#		endif // _WIN64
#	endif // YGGR_SYSTEM_64
#endif // _MSC_VER

#if defined(_MSC_VER)
#	if !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS
#	endif //  _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

//#if !defined(YGGR_DLL_DEF)
//
//#	if defined(BOOST_SYMBOL_EXPORT)
//#		undef BOOST_SYMBOL_EXPORT
//#	endif // BOOST_SYMBOL_EXPORT
//
//#	if defined(BOOST_SYMBOL_IMPORT)
//#		undef BOOST_SYMBOL_IMPORT
//#	endif // BOOST_SYMBOL_IMPORT
//
//#	define BOOST_SYMBOL_EXPORT
//#	define BOOST_SYMBOL_IMPORT
//
//#endif // defined(YGGR_DLL_DEF)

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#if !defined BOOST_BIND_GLOBAL_PLACEHOLDERS
#	define BOOST_BIND_GLOBAL_PLACEHOLDERS
#endif // BOOST_BIND_GLOBAL_PLACEHOLDERS

// Android defines
// There is problem with std::atomic on android (and some other platforms).
// See this link for more info:
// https://issuetracker.google.com/issues/36964000

#if defined(__ANDROID__) || defined(ANDROID)
#	if !defined(BOOST_ASIO_DISABLE_STD_ATOMIC)
#		define BOOST_ASIO_DISABLE_STD_ATOMIC 1
#	elif (!BOOST_ASIO_DISABLE_STD_ATOMIC)
#		undef BOOST_ASIO_DISABLE_STD_ATOMIC
#		define BOOST_ASIO_DISABLE_STD_ATOMIC 1
#	endif // BOOST_ASIO_DISABLE_STD_ATOMIC
#endif // #if defined(__ANDROID__) || defined(ANDROID)

#include <yggr/base/version.hpp>
#include <yggr/base/user_config.hpp>
#include <yggr/base/yggr_cpp_ver_config.hpp>
#include <yggr/base/boost_cpp_version_check.hpp>

#include <yggr/base/script_lua_cfg.hpp>
#include <yggr/base/script_python_cfg.hpp>

#if !(defined(YGGR_AT_WINDOWS) || defined(YGGR_AT_DARWIN) || defined(YGGR_AT_LINUX))
#	if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) || defined(WINDOWS) || defined(BOOST_WINDOWS)
#		if !defined(YGGR_AT_WINDOWS)
#			define YGGR_AT_WINDOWS
#		endif // YGGR_AT_WINDOWS
#	elif (defined(__APPLE__) || defined(__APPLE_CC__))
#		if !defined(YGGR_AT_DARWIN)
#			define YGGR_AT_DARWIN
#		endif // YGGR_AT_DARWIN
#	elif defined(__linux__) || defined(__linux)
#		if !defined(YGGR_AT_LINUX)
#			define YGGR_AT_LINUX
#		endif // YGGR_AT_LINUX
#	else
#		if !defined(YGGR_AT_LINUX)
#			define YGGR_AT_LINUX
#		endif // YGGR_AT_LINUX
#	endif // WIN32
#endif // !(defined(YGGR_AT_WINDOWS) || defined(YGGR_AT_DARWIN) || defined(YGGR_AT_LINUX))

#if defined(YGGR_AT_DARWIN) \
	&& (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) \
		|| (defined(TARGET_OS_IOS) && TARGET_OS_IOS)
#	if !defined(YGGR_AT_IOS)
#		define YGGR_AT_IOS
#	endif // YGGR_AT_IOS
#endif // TARGET_OS_IPHONE

#if defined(__ANDROID__) || defined(ANDROID) 
#	if !defined(YGGR_AT_ANDROID)
#		define YGGR_AT_ANDROID
#	endif // YGGR_AT_ANDROID
#endif // TARGET_OS_IPHONE

#if defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS)
#	if !defined(YGGR_AT_MOBILE)
#		define YGGR_AT_MOBILE
#	endif // YGGR_AT_MOBILE
#endif // #if defined(YGGR_AT_ANDROID) || defined(YGGR_AT_IOS)


#if defined(YGGR_AT_WINDOWS)
#	if defined(_WIN32_WINNT) && (_WIN32_WINNT < 0x0600)
#		undef _WIN32_WINNT
#	endif
#	if !defined(_WIN32_WINNT)
#		define _WIN32_WINNT 0x0600
#	endif // _WIN32_WINNT
#endif //YGGR_AT_WINDOWS

#include <yggr/base/compiler_config.hpp>

#ifdef _DEBUG
#	define YGGR_DBG_VAR_ERR() __dbg_e__
#else
#	define YGGR_DBG_VAR_ERR()
#endif // _DEBUG

#include <yggr/base/container_cfg.hpp>

#include <yggr/base/endian.hpp>

#include <yggr/base/noexcept_cfg.hpp>
#include <yggr/base/constexpr_cfg.hpp>
#include <yggr/base/attribute_cfg.hpp>
#include <yggr/base/if_constexpr_cfg.hpp>

#include <yggr/base/cpp17_features_cfg.hpp>
#include <yggr/base/cpp20_features_cfg.hpp>

namespace yggr
{
} // namespace yggr

#endif // __YGGR_YGGR_CONFIG_H__
