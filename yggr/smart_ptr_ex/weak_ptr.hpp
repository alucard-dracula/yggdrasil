// weak_ptr.hpp

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

#ifndef __YGGR_SMART_PTR_EX_WEAK_PTR_HPP__
#define __YGGR_SMART_PTR_EX_WEAK_PTR_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/weak_ptr.hpp>

#if !defined(YGGR_NO_CXX11_SMART_PTR)
#	include <memory>
#endif // YGGR_NO_CXX11_SMART_PTR

namespace yggr
{
namespace smart_ptr_ex
{
#if !defined(YGGR_NO_CXX11_SMART_PTR) && defined(YGGR_SMART_PTR_USING_STL)
	using std::weak_ptr;
#else
	using boost::weak_ptr;
#endif // !defined(YGGR_NO_CXX11_SMART_PTR) && defined(YGGR_SMART_PTR_USING_STL)
} // namespace smart_ptr_ex

using smart_ptr_ex::weak_ptr;

} // namespace yggr

#endif // __YGGR_SMART_PTR_EX_WEAK_PTR_HPP__
