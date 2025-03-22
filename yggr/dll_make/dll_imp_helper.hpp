//dll_imp_helper.hpp

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

#ifndef __YGGR_DLL_IMP_HELPER_HPP__
#define __YGGR_DLL_IMP_HELPER_HPP__

#include <yggr/ppex/open.hpp>
#include <yggr/dll_make/dll_mark_define.hpp>

#define YGGR_DLL_MAKE_API_TYPE_0(__ret_type__, __foo_type_name__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(void)

#define YGGR_DLL_MAKE_API_TYPE_1(__ret_type__, __foo_type_name__, __param_1__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__)

#define YGGR_DLL_MAKE_API_TYPE_2(__ret_type__, __foo_type_name__, __param_1__, __param_2__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__)

#define YGGR_DLL_MAKE_API_TYPE_3(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__)

#define YGGR_DLL_MAKE_API_TYPE_4(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__, __param_4__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__, __param_4__)

#define YGGR_DLL_MAKE_API_TYPE_5(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__)

#define YGGR_DLL_MAKE_API_TYPE_6(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__)

#define YGGR_DLL_MAKE_API_TYPE_7(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__)

#define YGGR_DLL_MAKE_API_TYPE_8(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__)

#define YGGR_DLL_MAKE_API_TYPE_9(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__)

#define YGGR_DLL_MAKE_API_TYPE_10(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__, __param_10__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__, __param_10__)

#define YGGR_DLL_MAKE_API_TYPE_11(__ret_type__, __foo_type_name__, __param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__, __param_10__, __param_11__) \
	typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)(__param_1__, __param_2__, __param_3__, __param_4__, __param_5__, __param_6__, __param_7__, __param_8__, __param_9__, __param_10__, __param_11__)

#if !defined( YGGR_NO_CXX11_VARIADIC_MACROS ) 
#	define YGGR_DLL_MAKE_API_TYPE_N(__ret_type__, __foo_type_name__, ...) \
			typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)( __VA_ARGS__ )
#else
	// self def __args__
#	define YGGR_DLL_MAKE_API_TYPE_N(__ret_type__, __foo_type_name__, __args__) \
			typedef __ret_type__ (YGGR_MODULE_CALL_T() *__foo_type_name__)( __args__ )
#endif //YGGR_NO_CXX11_VARIADIC_MACROS

#define YGGR_DLL_MAKE_API_TYPE YGGR_DLL_MAKE_API_TYPE_N

namespace yggr
{
namespace dll_make
{
} // namespace dll_make
} // namespace yggr

#endif //__YGGR_DLL_IMP_HELPER_HPP__
