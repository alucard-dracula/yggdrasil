//lua_export_wrap_pair.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_PAIR_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_PAIR_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/lua_export_typeid.hpp>
#include <yggr/lua_wrap/wrap_pair.hpp>

#include <boost/type_traits/is_same.hpp>

#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>

namespace yggr
{
namespace lua_wrap
{

template<typename RF, typename RS, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_pair_r_other_fs(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_pair<T>));
	typedef T wrap_pair_type;
	typedef RF other_first_type;
	typedef RS other_second_type;

	cobj
		.def(luabind::constructor<const other_first_type&, const other_second_type&>())

		.def("set_first", &wrap_pair_type::template set_first<other_first_type>)
		.def("set_second", &wrap_pair_type::template set_second<other_second_type>)
	;

	return cobj;
}

template<typename R, typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_pair_r_other(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_pair<T>));
	typedef T wrap_pair_type;
	typedef R other_wrap_pair_type;
	
	cobj
		.def(luabind::constructor<const other_wrap_pair_type&>())

		.def(luabind::const_self <= luabind::other<other_wrap_pair_type>())
		.def(luabind::const_self < luabind::other<other_wrap_pair_type>())
		.def(luabind::const_self == luabind::other<other_wrap_pair_type>())

		.def("compare_eq", 
				static_cast<bool (wrap_pair_type::*)(const other_wrap_pair_type&) const>(
					&wrap_pair_type::template compare_eq<other_wrap_pair_type>))
		.def("compare", 
				static_cast<s32 (wrap_pair_type::*)(const other_wrap_pair_type&) const>(
					&wrap_pair_type::template compare<other_wrap_pair_type>))
	;

	return cobj;
}

template<typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_pair(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_pair<T>));
	typedef T wrap_pair_type;
	typedef typename wrap_pair_type::first_type first_type;
	typedef typename wrap_pair_type::second_type second_type;

	cobj
		.def(luabind::constructor<>())
		.def(luabind::constructor<const first_type&, const second_type&>())
		.def(luabind::constructor<const wrap_pair_type&>())

		.def(luabind::tostring(luabind::const_self))

		.def(luabind::const_self <= luabind::const_self)
		.def(luabind::const_self < luabind::const_self)
		.def(luabind::const_self == luabind::const_self)

		.def("swap", static_cast<void (wrap_pair_type::*)(wrap_pair_type&)>(&wrap_pair_type::swap))

		.def("copy", &wrap_pair_type::copy)

		.def("get_first", &wrap_pair_type::get_first)
		.def("get_second", &wrap_pair_type::get_second)

		.def("set_first", &wrap_pair_type::template set_first<first_type>)
		.def("set_second", &wrap_pair_type::template set_second<second_type>)

		.def("compare_eq", static_cast<bool (wrap_pair_type::*)(const wrap_pair_type&) const>(&wrap_pair_type::compare_eq))
		.def("compare", static_cast<s32 (wrap_pair_type::*)(const wrap_pair_type&) const>(&wrap_pair_type::compare))
	;

	return cobj;
}

} // namespace lua_wrap
} // namespace yggr

#endif //__YGGR_LUA_WRAP_LUA_EXPORT_WRAP_PAIR_HPP__
