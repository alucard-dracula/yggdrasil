// has_move_emulation_enabled_impl.hpp

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

#ifndef __YGGR_MOVE_DETAIL_HAS_MOVE_EMULATION_ENABLED_IMPL_HPP__
#define __YGGR_MOVE_DETAIL_HAS_MOVE_EMULATION_ENABLED_IMPL_HPP__

#include <yggr/type_traits/is_convertible.hpp>

#include <boost/move/core.hpp>
#include <boost/move/move.hpp>

namespace yggr
{
namespace move
{
namespace detail
{

#if defined(BOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(BOOST_MOVE_DOXYGEN_INVOKED)

template<typename T>
struct has_move_emulation_enabled_impl
   : public ::yggr::is_convertible<T, ::boost::rv<T>& >
{
};

template<typename T>
struct has_move_emulation_enabled_impl<T&>
	: public boost::mpl::false_
{
};

template<typename T>
struct has_move_emulation_enabled_impl< ::boost::rv<T> >
	: public boost::mpl::false_
{
};

template<typename T>
struct has_move_emulation_enabled
	: public has_move_emulation_enabled_impl<T>
{
};

template<typename T>
struct has_move_emulation_disabled
	: public boost::mpl::bool_< !has_move_emulation_enabled_impl<T>::value >
{
};

#else

template<typename T>
struct has_move_emulation_enabled
	: public boost::mpl::false_
{
};

template<typename T>
struct has_move_emulation_disabled
	: public boost::mpl::true_
{
};
	
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

} // namespace detail
} // namespace move
} // namespace yggr

#endif // __YGGR_MOVE_DETAIL_HAS_MOVE_EMULATION_ENABLED_IMPL_HPP__
