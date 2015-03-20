//error_make.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_ERROR_MAKE_HPP__
#define __YGGR_ERROR_MAKE_HPP__

#include <yggr/charset/string.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/base/error.hpp>

#define ERROR_CODE_DEF(__code__) __code__,
#define ERROR_CODE_KEY_VAL_DEF(__key__, __val__) __key__ = __val__,

#define ERROR_CODE_DEF_BEGIN(__s_code__) \
public: \
enum { \
	E_success = 0, \
	E_code_start = __s_code__,

#define ERROR_CODE_DEF_NON_CODE_BEGIN() \
public: \
enum { \
	E_success = 0,

#define ERROR_CODE_DEF_END(__e_code__) \
	E_code_end = __e_code__, \
	E_compile_u32 = 0xffffffff };

#define ERROR_CODE_DEF_NON_CODE_END() \
	E_compile_u32 = 0xffffffff };

#define ERROR_CODE_MSG(__code__, __msg__) \
	case __code__: \
	msg += " ------ " __msg__; \
		break;

#define ERROR_CODE_MSG_BEGIN() \
	public: \
	inline static const error_type make_error(const yggr::u32& code) { return this_type::make_error(code, err_string_type()); } \
	\
	inline static const error_type make_error(const yggr::u32& code, const err_string_type& other_msg) { \
		err_string_type msg = err_string_type(this_type::class_name()) \
								+ (other_msg.empty()? "" : " ") \
								+ other_msg; \
		switch(code) { \
		case E_success: \
			msg +=  " ------ success"; \
			break;

#define ERROR_CODE_MSG_END() \
	default: \
		msg += " ------  unknow error"; } \
	return error_type(code, msg); } \

#define ERROR_MAKER_BEGIN(__class_name__) \
	public: \
		class error_maker { \
		public: \
			typedef yggr::error error_type; \
			typedef error_type::err_string_type err_string_type; \
		private: \
			typedef error_maker this_type; \
			\
		private: \
			inline static const char* class_name(void) { return __class_name__; } \

#define ERROR_MAKER_END() \
		}; \
	typedef error_maker error_maker_type; \
	typedef yggr::error error_type;

namespace yggr
{
namespace helper
{
} // namespace helper
} // namespace yggr

#endif //__YGGR_ERROR_MAKE_HPP__
