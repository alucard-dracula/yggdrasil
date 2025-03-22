//lua_export_typeinfo.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_TYPEINFO_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_TYPEINFO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/cast_to_string.hpp>

#include <typeinfo>
#include <iostream>

#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const std::type_info& info)
{
	os << info.name();
	return os;
}

} // namespace detail
} // namespace lua_wrap
} // namespace yggr

namespace std
{

using ::yggr::lua_wrap::detail::operator<<;

} // namespace std

#if !defined(YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH)

// 处理到这里 等182测试完成 用DEV 测试其他版本的情况
//#if (106500 < BOOST_VERSION) && (BOOST_VERSION < 106900) && (YGGR_CPP_VER_14 < YGGR_CPP_VERSION)
#if (106500 < BOOST_VERSION) && (BOOST_VERSION < 106900) && (YGGR_CPP_VER_14 < YGGR_CPP_VERSION) \
	|| (defined(_MSC_VER) && (_MSC_VER < 1700))

#	define YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH

#endif // #if (106500 < BOOST_VERSION) && (BOOST_VERSION < 106900) && (YGGR_CPP_VER_14 < YGGR_CPP_VERSION)

#endif // #if !defined(YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH)

namespace yggr
{
namespace lua_wrap
{

#if defined(YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH)

namespace patch
{

template<typename T> inline 
const char* type_info_dot_name(const T& info)
{
	return info.name();
}

#if defined(_MSC_VER)
template<> inline 
const char* type_info_dot_name(const std::type_info& info)
{
	return info.name();
}

#endif // _MSC_VER

} // namespace patch

#endif // #if defined(YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH)

template<typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_type_info(luabind::class_<T, X1, X2, X3>& cobj)
{
	typedef T type_info_type;

	cobj
		.def(luabind::tostring(luabind::const_self))
#if defined(YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH)
		.def( "name", &patch::type_info_dot_name<type_info_type>)
#else
		.def( "name", &type_info_type::name)
#endif // #if defined(YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH)
		.def(luabind::const_self == luabind::const_self)
	;

	return cobj;
}

} // namespace lua_wrap
} // namespace yggr

#if defined(YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH)
#	undef YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH
#endif // #if defined(YGGR_TMP_PP_LUA_EXPORT_TYPEINFO_USING_PATCH)

#endif //__YGGR_LUA_WRAP_LUA_EXPORT_TYPEINFO_HPP__