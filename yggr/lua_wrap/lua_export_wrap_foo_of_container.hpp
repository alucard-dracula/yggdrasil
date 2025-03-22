// lua_export_wrap_foo_of_container.hpp

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

#ifndef __YGGR_LUA_WRAP_LUA_EXPORT_FOO_OF_CONTAINER_HPP__
#define __YGGR_LUA_WRAP_LUA_EXPORT_FOO_OF_CONTAINER_HPP__

#include <yggr/lua_wrap/wrap_foo_of_container.hpp>

#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/operator.hpp>
#include <luabind/scope.hpp>

namespace yggr
{
namespace lua_wrap
{

// size
template<typename Cont> inline
luabind::scope export_foo_size(void)
{
	return luabind::def("size", &wrap_foo_size<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_wraped_size(void)
{
	return luabind::def("wraped_size", &wrap_foo_wraped_size<Cont>);
}

// ssize
template<typename Cont> inline
luabind::scope export_foo_ssize(void)
{
	return luabind::def("ssize", &wrap_foo_ssize<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_wraped_ssize(void)
{
	return luabind::def("wraped_ssize", &wrap_foo_wraped_ssize<Cont>);
}

// resize
template<typename Cont, typename Dis> inline
luabind::scope export_foo_resize(void)
{
	return luabind::def("resize", &wrap_foo_resize<Cont, Dis>);
}

template<typename Cont, typename Dis, typename T> inline
luabind::scope export_foo_resize(void)
{
	return luabind::def("resize", &wrap_foo_resize<Cont, Dis, T>);
}

// empty
template<typename Cont> inline
luabind::scope export_foo_empty(void)
{
	return luabind::def("empty", &wrap_foo_empty<Cont>);
}

// clear
template<typename Cont> inline
luabind::scope export_foo_clear(void)
{
	return luabind::def("clear", &wrap_foo_clear<Cont>);
}

// begin
template<typename Cont> inline
luabind::scope export_foo_ref_begin(void)
{
	return luabind::def("begin_", &wrap_foo_ref_begin<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_cref_begin(void)
{
	return luabind::def("begin_", &wrap_foo_cref_begin<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_cbegin(void)
{
	return luabind::def("cbegin_", &wrap_foo_cbegin<Cont>);
}

// end
template<typename Cont> inline
luabind::scope export_foo_ref_end(void)
{
	return luabind::def("end_", &wrap_foo_ref_end<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_cref_end(void)
{
	return luabind::def("end_", &wrap_foo_cref_end<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_cend(void)
{
	return luabind::def("cend_", &wrap_foo_cend<Cont>);
}

// rbegin
template<typename Cont> inline
luabind::scope export_foo_ref_rbegin(void)
{
	return luabind::def("rbegin_", &wrap_foo_ref_rbegin<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_cref_rbegin(void)
{
	return luabind::def("rbegin_", &wrap_foo_cref_rbegin<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_crbegin(void)
{
	return luabind::def("crbegin_", &wrap_foo_crbegin<Cont>);
}

// rend
template<typename Cont> inline
luabind::scope export_foo_ref_rend(void)
{
	return luabind::def("rend_", &wrap_foo_ref_rend<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_cref_rend(void)
{
	return luabind::def("rend_", &wrap_foo_cref_rend<Cont>);
}

template<typename Cont> inline
luabind::scope export_foo_crend(void)
{
	return luabind::def("crend_", &wrap_foo_crend<Cont>);
}

} // namespace lua_wrap
} // namespace yggr

#endif //__YGGR_LUA_WRAP_LUA_EXPORT_FOO_OF_CONTAINER_HPP__