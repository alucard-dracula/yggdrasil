// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

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

#include <luabind/scope.hpp>
#include <luabind/detail/debug.hpp>
#include <luabind/detail/stack_utils.hpp>
#include <luabind/detail/scoped_ptr_helper.hpp>
#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace luabind { namespace detail {

    registration::registration()
        : m_next(0)
    {
    }

    registration::~registration()
    {
        delete m_next;
    }

    } // namespace detail



    scope::scope()
        : m_chain(0)
    {
    }

    // ownership is transfered through auto_ptr

//#if defined(BOOST_NO_CXX11_SMART_PTR)
//    scope::scope(std::auto_ptr<detail::registration> reg)
//        : m_chain(reg.release())
//    {
//    }
//#else
//	scope::scope(const std::unique_ptr<detail::registration>& reg)
//        : m_chain(const_cast<std::unique_ptr<detail::registration>&>(reg).release())
//    {
//    }
//#endif // BOOST_NO_CXX11_SMART_PTR
	scope::scope(const boost::scoped_ptr<detail::registration>& reg)
        : m_chain(detail::scoped_ptr_helper::release_ownership_trans_scoped_ptr(reg))
    {
    }

    scope::scope(scope const& other)
        : m_chain(other.m_chain)
    {
        const_cast<scope&>(other).m_chain = 0;
    }

    scope& scope::operator=(scope const& other_)
    {
        delete m_chain;
        m_chain = other_.m_chain;
        const_cast<scope&>(other_).m_chain = 0;
        return *this;
    }

    scope::~scope()
    {
        delete m_chain;
    }

    scope& scope::operator,(scope s)
    {
        // If first registration
        if (!m_chain)
        {
            m_chain = s.m_chain;
            s.m_chain = 0;
            return *this;
        }

        for (detail::registration* c = m_chain; ; c = c->m_next)
        {

            if (c->m_next == 0)
            {
                c->m_next = s.m_chain;
                s.m_chain = 0;
                break;
            }
        }

        return *this;
    }


    void scope::register_(lua_State* L) const
    {
        // Register each element
        for (detail::registration* r = m_chain; r != 0; r = r->m_next)
        {
			LUABIND_CHECK_STACK(L);
            r->register_(L);
        }
    }

} // namespace luabind



namespace luabind {

/*
// Used only once
    namespace {

        struct lua_pop_stack
        {
            lua_pop_stack(lua_State* L)
                : m_state(L)
            {
            }

            ~lua_pop_stack()
            {
                lua_pop(m_state, 1);
            }

            lua_State* m_state;
        };

    } // namespace unnamed
*/

    module_::module_(lua_State* L, char const* name = 0)
        : m_state(L)
        , m_name(name)
    {
    }

    void module_::operator[](scope s)
    {
        // If named
        if (m_name)
        {

#if LUA_VERSION_NUM < 502
			lua_pushstring(m_state, m_name);
            lua_gettable(m_state, LUA_GLOBALSINDEX);
#else
            lua_getglobal(m_state, m_name);
#endif // LUA_VERSION_NUM < 502

            if (!lua_istable(m_state, -1))
            {
                lua_pop(m_state, 1);    // pops existing entry with same name

                lua_newtable(m_state);

#if LUA_VERSION_NUM < 502
				lua_pushstring(m_state, m_name);
                lua_pushvalue(m_state, -2);
                lua_settable(m_state, LUA_GLOBALSINDEX);
#else
                lua_pushvalue(m_state, -1);
                lua_setglobal(m_state,m_name);
#endif // LUA_VERSION_NUM < 502
            }
        }
        else
        {
#if LUA_VERSION_NUM < 502
			lua_pushvalue(m_state, LUA_GLOBALSINDEX);
#else
			lua_rawgeti(m_state, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#endif // LUA_VERSION_NUM < 502
        }

        // At this point we a table on stack

        //lua_pop_stack guard(m_state);

        s.register_(m_state);

        lua_pop(m_state, 1); //! Pops previous table
    }


    struct namespace_::registration_ : detail::registration
    {
        registration_(char const* name)
            : m_name(name)
        {
            assert(name != 0);
        }

        void register_(lua_State* L) const
        {
			LUABIND_CHECK_STACK(L);
            assert(lua_gettop(L) >= 1);
#if LUA_VERSION_NUM < 502
            lua_pushstring(L, m_name);
            lua_gettable(L, -2);
#else
            lua_getfield(L,-1,m_name);
#endif // LUA_VERSION_NUM < 502

			//detail::stack_pop p(L, 1); // pops the table on exit

            if (!lua_istable(L, -1))
            {
                lua_pop(L, 1);

                lua_newtable(L);
                lua_pushstring(L, m_name);
                lua_pushvalue(L, -2);
                lua_settable(L, -4);
            }

            m_scope.register_(L);

            lua_pop(L, 1);
        }

        char const* m_name;
        scope m_scope;
    };

//#if defined(BOOST_NO_CXX11_SMART_PTR)
//
//    namespace_::namespace_(char const* name)
//        : scope(std::auto_ptr<detail::registration>(
//              m_registration = new registration_(name)))
//    {
//    }
//
//#else
//
//	 namespace_::namespace_(char const* name)
//        : scope(std::unique_ptr<detail::registration>(
//              m_registration = new registration_(name)))
//    {
//    }
//
//#endif // BOOST_NO_CXX11_SMART_PTR

	 namespace_::namespace_(char const* name)
        : scope(boost::scoped_ptr<detail::registration>(
              m_registration = new registration_(name)))
    {
    }

    namespace_& namespace_::operator[](scope s)
    {
        m_registration->m_scope.operator,(s);
        return *this;
    }

} // namespace luabind

