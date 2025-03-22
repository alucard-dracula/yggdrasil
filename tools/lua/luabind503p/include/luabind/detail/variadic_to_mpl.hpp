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

#ifndef BOOST_NO_VARIADIC_TEMPLATES


#ifndef LUABIND_VARIADIC_TO_MPL_HPP_INCLUDED
#define LUABIND_VARIADIC_TO_MPL_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/remove.hpp>


namespace luabind { namespace detail {




/** \brief Converts a variadic template into a Boost.MPL vector
 * Code pasted from http://blog.shandyba.com/2009/12/17/converting-variadic-template-arguments-pack-to-boost-mpl-sequence/
 *
 * \param
 * \param
 * \return
 *
 */

//General definition of the helper class
template <typename ...Args> struct mplVectorFromVariadic;

//This specialization does the actual job: it splits the whole pack
//into 2 parts: one single type T and the rest of types Args...
//As soon as it is done T is added to an mpl::vector.
//"bottom--up" recursion is used to fetch all types
template <class T, typename ...Args>
struct mplVectorFromVariadic<T, Args...>
{
    // sizeof... not yet available, couls use a special stryct to do the trick but whatever...
    //typedef typename BOOST_PP_CAT(mpl::vector,sizeof...(Args))<Args...>::type type;
    typedef typename mpl::push_front<typename mplVectorFromVariadic<Args...>::type, T>::type type;
};

//This is a specialization for the case when only one type is passed
//and also finishes recursive descent
template <class T>
struct mplVectorFromVariadic<T>
{
    typedef typename mpl::vector<T> type;
};

//This one handles the case when no types where passed at all
template <>
struct mplVectorFromVariadic<>
{
    typedef typename mpl::vector<> type;
};



}}

#endif

#endif // BOOST_NO_VARIADIC_TEMPLATES
