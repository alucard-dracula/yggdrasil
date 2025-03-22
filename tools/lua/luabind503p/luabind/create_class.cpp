// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#define LUABIND_BUILDING

#include <luabind/lua_include.hpp>

#include <luabind/luabind.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace luabind { namespace detail
{
	namespace
	{
		// expects two tables on the lua stack:
		// 1: destination
		// 2: source

		void copy_member_table(lua_State* L)
		{
			lua_pushnil(L);

            //Pops a key from the stack, and pushes a key–value pair from the table at the given index (the "next" pair after the given key).
            //If there are no more elements in the table, then lua_next returns 0 (and pushes nothing).
			while (lua_next(L, -2))
			{
			    // Prevents copy of __init and __finalize
				lua_pushstring(L, "__init");
#if LUA_VERSION_NUM < 502
				if (lua_equal(L, -1, -3))
#else
				if (lua_compare(L, -1, -3, LUA_OPEQ))
#endif // LUA_VERSION_NUM < 502
				{
					lua_pop(L, 2);
					continue;
				}
				else lua_pop(L, 1); // __init string

				lua_pushstring(L, "__finalize");
#if LUA_VERSION_NUM < 502
				if (lua_equal(L, -1, -3))
#else
				if (lua_compare(L, -1, -3, LUA_OPEQ))
#endif // LUA_VERSION_NUM < 502
				{
					lua_pop(L, 2);
					continue;
				}
				else lua_pop(L, 1); // __finalize string

				lua_pushvalue(L, -2); // copy key
				lua_insert(L, -2);
				lua_settable(L, -5);
			}
		}
	}



	int create_class::stage2(lua_State* L)
	{
	    // Gets class_rep from upvalue
		class_rep* crep = static_cast<class_rep*>(lua_touserdata(L, lua_upvalueindex(1)));

		assert((crep != 0) && "internal error, please report");
		assert((is_class_rep(L, lua_upvalueindex(1))) && "internal error, please report");

	#ifndef LUABIND_NO_ERROR_CHECKING

		if (!is_class_rep(L, 1))
		{
			lua_pushstring(L, "expected class to derive from or a newline");
			lua_error(L);
		}

	#endif

		class_rep* base = static_cast<class_rep*>(lua_touserdata(L, 1));
		class_rep::base_info binfo;

		binfo.pointer_offset = 0;
		binfo.base = base;
		crep->add_base_class(binfo);

		// copy base class members

		crep->get_table(L);
		base->get_table(L);
		// Copies content of base table
		copy_member_table(L);

		crep->get_default_table(L);
		base->get_default_table(L);
		copy_member_table(L);

		crep->set_type(base->type());

#if LUA_VERSION_NUM < 502
		return 0;
#else
        return LUABIND_OK;
#endif // LUA_VERSION_NUM < 502
	}


	int create_class::stage1(lua_State* L)
	{

	#ifndef LUABIND_NO_ERROR_CHECKING

		if (lua_gettop(L) != 1 || lua_type(L, 1) != LUA_TSTRING || lua_isnumber(L, 1))
		{
			lua_pushstring(L, "invalid construct, expected class name");
			lua_error(L);
		}

        //! Checks no extra nulls
#if LUA_VERSION_NUM < 502
		if (std::strlen(lua_tostring(L, 1)) != lua_strlen(L, 1))
#else
		if (std::strlen(lua_tostring(L, 1)) != lua_rawlen(L, 1))
#endif // LUA_VERSION_NUM < 502
		{
			lua_pushstring(L, "luabind does not support class names with extra nulls");
			lua_error(L);
		}

	#endif
        // class name
		const char* newClassName = lua_tostring(L, 1);


		void* allocatedClassRepSpace = lua_newuserdata(L, sizeof(class_rep));
		new(allocatedClassRepSpace) class_rep(L, newClassName);      //!< Calls constructor (this is called a  placement new

		// make the class globally available
#if LUA_VERSION_NUM < 502
		lua_pushstring(L, newClassName);
		lua_pushvalue(L, -2);
		lua_settable(L, LUA_GLOBALSINDEX);
#else
        lua_pushvalue(L, -1);
        lua_setglobal(L,newClassName);
#endif // LUA_VERSION_NUM < 502

		// also add it to the closure as return value
		lua_pushcclosure(L, &stage2, 1);

		return LUABIND_OK;
	}

}}

