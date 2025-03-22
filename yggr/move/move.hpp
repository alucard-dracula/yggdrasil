//move.hpp

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

#ifndef __YGGR_MOVE_MOVE_HPP__
#define __YGGR_MOVE_MOVE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/config.hpp>
#include <yggr/move/has_move_emulation.hpp>
#include <yggr/move/detail/move_return.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace move
{

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
template<typename T> inline
T& rvref_to_ref(BOOST_RV_REF(T) t)
{
	return t;
}

template<typename T> inline
T* rvref_to_pointer(BOOST_RV_REF(T) t)
{
	return boost::addressof(rvref_to_ref(boost::forward<T>(t)));
}

#else

template<typename T> inline
T& rvref_to_ref(T& t)
{
	return t;
}

template<typename T> inline
T* rvref_to_pointer(T& t)
{
	return boost::addressof(t);
}

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES


} // namespace move

using move::rvref_to_ref;
using move::rvref_to_pointer;

} // namespace yggr


#endif // __YGGR_MOVE_MOVE_HPP__
