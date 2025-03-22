//node_handle_foo_hacking_midx_make_augmented_interface_hacking.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLER_FOO_HACKING_MIDX_MAKE_AUGMENTED_INTERFACE_HACKING_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLER_FOO_HACKING_MIDX_MAKE_AUGMENTED_INTERFACE_HACKING_HPP__

#include <yggr/base/yggrdef.h>

#if (BOOST_VERSION < 105900)

#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

// !!! gcc not support direct BOOST_MPL_ASSERT((false)) and static_assert(false), so not impl it !!!

#if defined(_MSC_VER)

template<typename T, typename AP>
struct make_augmented_interface_hacking
{
	/*
	*	if a compilation error occurs here, 
	*	it means that the template specialization of an AugmentPolicy is missing
	*/
	BOOST_MPL_ASSERT((boost::mpl::false_));
};

#else

template<typename T, typename AP>
struct make_augmented_interface_hacking;

#endif // _MSC_VER




} // namespace detail
} // namespace container
} // namespace yggr

#endif // (BOOST_VERSION < 105900)

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLER_FOO_HACKING_MIDX_MAKE_AUGMENTED_INTERFACE_HACKING_HPP__