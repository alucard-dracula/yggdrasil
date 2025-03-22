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

#ifndef NEW_SCOPE_040211_HPP
#define NEW_SCOPE_040211_HPP

#include <luabind/prefix.hpp>
#include <luabind/config.hpp>
#include <luabind/lua_include.hpp>
#include <boost/scoped_ptr.hpp>
#include <memory>

//#ifndef BOOST_NO_0X_HDR_FORWARD_LIST
//    #include <forward_list>
//#endif

namespace luabind {

    struct scope;

} // namespace luabind

namespace luabind { namespace detail {

    /** \struct Interface from which to derive to be able to bind anything
     *
     * \param
     * \param
     * \return
     *
     */

    struct LUABIND_API registration
    {
        registration();
        virtual ~registration();

    protected:
        /** \brief Virtual function you have to override to bind sthing
         * Function is expecting at least one element on top of stack (to check)
        **/
        virtual void register_(lua_State*) const = 0;

    private:
        friend struct ::luabind::scope;
        registration* m_next;
    };

} // namespace luabind::detail





    /** \struct scope
     * Base class to bind things to lua
     *
     *
     */

    struct LUABIND_API scope
    {
        scope();
//#if defined(BOOST_NO_CXX11_SMART_PTR)
//        explicit scope(std::auto_ptr<detail::registration> reg);
//#else
//		explicit scope(const std::unique_ptr<detail::registration>& reg);
//#endif // BOOST_NO_CXX11_SMART_PTR
		explicit scope(const boost::scoped_ptr<detail::registration>& reg);

        scope(scope const& other_);
        ~scope();

        scope& operator=(scope const& other_);

        //! Allows chain call
        scope& operator,(scope s);

        /** \brief Will go through the chain, registering every element of the list and checking that each registration returns a balanced stack.
        **/
        void register_(lua_State* L) const;

    private:

        detail::registration* m_chain;  //!< Single linked list ?
    };


    /** \class
     * Register into a table of name like a classic library
     *
     */
    class LUABIND_API namespace_ : public scope
    {
    public:
        explicit namespace_(char const* name);
        namespace_& operator[](scope s);

    private:
        struct registration_;
        registration_* m_registration;
    };



    /** \class module_
     *
     * \param
     * \param
     * \return
     *
     */

    class LUABIND_API module_
    {
    public:
        /** \brief
         * If given a name, will erase the existing global with the same name
        **/
        module_(lua_State* L_, char const* name);
        void operator[](scope s);

    private:
        lua_State* m_state;
        char const* m_name;
    };


    inline module_ module(lua_State* L, char const* name = 0)
    {
        return module_(L, name);
    }

} // namespace luabind

#endif // NEW_SCOPE_040211_HPP

