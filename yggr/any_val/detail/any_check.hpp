//any_check.hpp

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

#ifndef __YGGR_ANY_VAL_DETAIL_ANY_CHECK_HPP__
#define __YGGR_ANY_VAL_DETAIL_ANY_CHECK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace any_val
{

template<typename T>
class is_any : public boost::mpl::false_{};

} // namespace any_val
} // namespace yggr

#define YGGR_IS_ANY_DEF( __type__ ) \
	namespace yggr { namespace any_val { \
	template<> \
	class is_any< __type__ > : public boost::mpl::true_ {}; }}


#define YGGR_ANY_ASSERT( __type__ ) \
	BOOST_MPL_ASSERT((yggr::any_val::is_any< __type__ >))

#endif // __YGGR_ANY_VAL_DETAIL_ANY_CHECK_HPP__
