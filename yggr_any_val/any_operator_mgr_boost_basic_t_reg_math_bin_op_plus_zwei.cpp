// any_operator_mgr_boost_basic_t_reg_zwei.cpp

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

#include <yggr/any_val/any_operator_mgr_boost_basic_t_reg.hpp>
#include <yggr/any_val/any_operator_mgr_reg_macro.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace any_val
{

/*static*/ 
bool any_operator_mgr_boost_basic_t_reg::prv_s_register_base_type_plus_set_zwei(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	// std::string boost::container::string charset::string
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::string, char);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::string, std::string);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::string, char);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::string, boost::container::string);

#if !(defined(YGGR_NO_WSTRING) || defined(YGGR_AT_ANDROID))
	//std::wstring boost::container::wstring charset::wstring
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::wstring, wchar_t);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::wstring, std::wstring);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::wstring, wchar_t);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::wstring, boost::container::wstring);
#endif // !(defined(YGGR_NO_WSTRING) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	//std::u8string boost::container::u8string charset::u8string
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::u8string, c8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::u8string, std::u8string);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::u8string, c8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::u8string, boost::container::u8string);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	//std::u16string boost::container::u16string charset::u16string
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::u16string, c16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::u16string, std::u16string);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::u16string, c16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::u16string, boost::container::u16string);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	//std::u32string boost::container::u32string charset::u32string
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::u32string, c32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, std::u32string, std::u32string);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::u32string, c32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, boost::container::u32string, boost::container::u32string);
#endif // YGGR_NO_CHAR32_T

	// yggr::utf8_string
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, char);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, std::string);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, boost::container::string);

#if !(defined(YGGR_NO_WSTRING) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, wchar_t);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, std::wstring);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, boost::container::wstring);
#endif // !(defined(YGGR_NO_WSTRING) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, c8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, std::u8string);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, boost::container::u8string);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, c16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, std::u16string);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, boost::container::u16string);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, c32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, std::u32string);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, boost::container::u32string);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, charset::utf8_char);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, charset::utf8_string, charset::utf8_string);

	return bright;
}



} // namespace any_val
} // namespace yggr
