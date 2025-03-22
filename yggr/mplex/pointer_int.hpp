//pointer_int.hpp

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

#ifndef __YGGR_MPLEX_POINTER_INT_HPP__
#define __YGGR_MPLEX_POINTER_INT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/sysbit.hpp>

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

namespace yggr
{
namespace mplex
{

struct pointer_size
	: public boost::mpl::size_t<sizeof(void*)>
{
};

struct pointer_int
	: public
		boost::mpl::at
		<
			boost::mpl::map
			<
				boost::mpl::pair<sysbit_8, s8>,
				boost::mpl::pair<sysbit_16, s16>,
				boost::mpl::pair<sysbit_32, s32>,
				boost::mpl::pair<sysbit_64, s64>,
				boost::mpl::pair<sysbit_128, void>,
				boost::mpl::pair<sysbit_256, void>
			>, 
			sysbit_now
		>
{	
};

struct pointer_uint
	: public boost::make_unsigned<pointer_int::type>
{
};

} // namespace mplex
} // namespace yggr

#endif //__YGGR_MPLEX_POINTER_INT_HPP__