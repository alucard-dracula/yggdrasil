//dll_def_helper.hpp

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

#ifndef __YGGR_DLL_DEF_HELPER_HPP__
#define __YGGR_DLL_DEF_HELPER_HPP__

#include <yggr/ppex/open.hpp>
#include <yggr/ppex/params_expand.hpp>
#include <yggr/dll_make/dll_mark_define.hpp>

#define YGGR_DLL_MAKE_API_0(__ret_type__, __foo_name__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(void)

#define YGGR_DLL_MAKE_API_1(__ret_type__, __foo_name__, __param_1__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__)

#define YGGR_DLL_MAKE_API_2(__ret_type__, __foo_name__, __param_1__, __param_2__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__)

#define YGGR_DLL_MAKE_API_3(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__)

#define YGGR_DLL_MAKE_API_4(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__, __param_4__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__, __param_4__)

#define YGGR_DLL_MAKE_API_5(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__)

#define YGGR_DLL_MAKE_API_6(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__)

#define YGGR_DLL_MAKE_API_7(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__)

#define YGGR_DLL_MAKE_API_8(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__)

#define YGGR_DLL_MAKE_API_9(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__)

#define YGGR_DLL_MAKE_API_10(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__, __param_10__) \
	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__, __param_10__)

//#define YGGR_DLL_MAKE_API_11(__ret_type__, __foo_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__, __param_10__, __param_11__) \
//	YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__, __param_10__, __param_11__)

#if !defined(YGGR_NO_CXX11_VARIADIC_MACROS) 
#	define YGGR_DLL_MAKE_API_N(__ret_type__, __foo_name__, ... ) \
			YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__( YGGR_PP_PARAMS_EXPAND(__VA_ARGS__) )
#else
	// self def __args__
#	define YGGR_DLL_MAKE_API_N(__ret_type__, __foo_name__, __args__ ) \
			YGGR_DLL_API __ret_type__ YGGR_MODULE_CALL_T() __foo_name__( YGGR_PP_PARAMS_EXPAND(__args__) )
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

#define YGGR_DLL_MAKE_API YGGR_DLL_MAKE_API_N

namespace yggr
{
namespace dll_make
{
} // namespace dll_make
} // namespace yggr

#endif //__YGGR_DLL_DEF_HELPER_HPP__
