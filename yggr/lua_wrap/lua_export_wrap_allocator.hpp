//lua_export_wrap_allocator.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_ALLOCATOR_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_ALLOCATOR_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <yggr/lua_wrap/wrap_allocator.hpp>
#include <yggr/lua_wrap/lua_export_typeid.hpp>

#include <boost/type_traits/is_same.hpp>

#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>

namespace yggr
{
namespace lua_wrap
{

template<typename T, typename X1, typename X2, typename X3> inline
luabind::class_<T, X1, X2, X3>&
	export_wrap_allocator(luabind::class_<T, X1, X2, X3>& cobj)
{
	BOOST_MPL_ASSERT((detail::is_wrap_allocator<T>));
	typedef T wrap_alloc_type;
	typedef typename wrap_alloc_type::base_type org_alloc_type;

	cobj
		.def(luabind::constructor<>())
		.def(luabind::constructor<const wrap_alloc_type&>())

		.def(luabind::tostring(luabind::const_self))

		.def(luabind::const_self == luabind::const_self)

		.def("get_allocator", &wrap_alloc_type::get_allocator)
		.def("copy", &wrap_alloc_type::copy)
		.def("max_size", &wrap_alloc_type::max_size)
		.def("wraped_max_size", &wrap_alloc_type::wraped_max_size)

		.def("compare_eq", static_cast<bool (wrap_alloc_type::*)(const wrap_alloc_type&) const>(&wrap_alloc_type::compare_eq))
	;

	return cobj;
}

} // namespace lua_wrap
} // namespace yggr

#endif // __YGGR_LUA_WRAP_LUA_EXPORT_WRAP_ALLOCATOR_HPP__