// lua_debug_helper.cpp

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

#include <yggr/script/lua_debug_helper.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/exception/exception.hpp>

#include <sstream>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace script
{
namespace lua
{
namespace detail
{

static std::string get_line_info(const lua_Debug& ar)
{
	const char *pl = ar.source, *pr = 0;
	if(!(pl && ar.currentline))
	{
		return std::string();
	}

	pr = pl + std::strlen(pl);

	if(ar.currentline < 2)
	{
		return std::string(pl, pr);
	}
	else
	{
		const char *pml = pl, *pmr = 0;
		for(yggr::u32 i = 0, isize = ar.currentline - 1; i != isize; ++i)
		{
			pml = std::find(pml, pr, '\n') + 1;
		}

		pmr = std::find(pml, pr, '\n') - 1;

		return std::string(pml, pmr);
	}
}

static std::string format_debug_info(const lua_Debug& ar)
{
	std::stringstream ss;
	ss << "at line: " << ar.currentline << ", "
		<< "detail info: " << get_line_info(ar) << ", "
		<< "in source: " << ar.short_src;

	return ss.str();
}

static int handler_lua_pcall(lua_State* l)
{
	lua_Debug ar = {0};

	assert(l);
	if(!l)
	{
		return 0;
	}

#ifdef LUAJIT_VERSION
	lua_getstack(l, 1, &ar);
#else
	yggr::u32 lv = 0;

	for(;lua_getstack(l, lv, &ar); ++lv);

	if(!lv)
	{
		return 0;
	}

	lua_getstack(l, lv, &ar);
#endif // LUAJIT_VERSION

	lua_getinfo(l, "Sln", &ar);

	exception::exception::throw_error(
		lua_script::error_maker_type::make_error(
			lua_script::error_maker_type::E_lua_code_runtime_error_detail,
			format_debug_info(ar)));

	return 1;
}

} // namespace detail

void bind_handler_lua_pcall(luabind::pcall_callback_fun pfoo)
{
	return
		pfoo?
			luabind::set_pcall_callback(pfoo)
			: luabind::set_pcall_callback(&detail::handler_lua_pcall);
}

} // namespace lua
} // namespace script
} // namespace yggr
