// cv_mark.hpp

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

#ifndef __YGGR_MPLEX_CV_MARK_HPP__
#define __YGGR_MPLEX_CV_MARK_HPP__

#include <yggr/base/static_constant.hpp>
#include <boost/mpl/integral_c.hpp>

namespace yggr
{
namespace mplex
{

struct cv_mark_t
{
	typedef boost::mpl::integral_c<u32, 0> cv_non_t;
	typedef boost::mpl::integral_c<u32, 1U> cv_const_t;
	typedef boost::mpl::integral_c<u32, 1U << 1> cv_volatile_t;
	typedef boost::mpl::integral_c<u32, (cv_const_t::value | cv_volatile_t::value)> cv_const_volatile_t;

	typedef boost::mpl::integral_c<u32, (cv_const_volatile_t::value + 1)> cv_end_t;

	YGGR_STATIC_CONSTANT(u32, E_cv_non = cv_non_t::value);
	YGGR_STATIC_CONSTANT(u32, E_cv_const = cv_const_t::value);
	YGGR_STATIC_CONSTANT(u32, E_cv_volatile = cv_volatile_t::value);
	YGGR_STATIC_CONSTANT(u32, E_cv_const_volatile = cv_const_volatile_t::value);

	YGGR_STATIC_CONSTANT(u32, E_cv_end = cv_end_t::value);

};


} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_CV_MARK_HPP__
